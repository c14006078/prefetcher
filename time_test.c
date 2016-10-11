#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>

#include <xmmintrin.h>
#include <immintrin.h>

#define TEST_W 4096
#define TEST_H 4096

/* provide the implementations of naive_transpose,
 * sse_transpose, sse_prefetch_transpose
 */

#include "impl.h"

static long diff_in_us(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec * 1000000.0 + diff.tv_nsec / 1000.0);
}

int main(int argc, char *argv[])
{
    {
        struct timespec start, end;

#ifdef ALIGN
#include <malloc.h>
        int *src  = (int *) memalign(sizeof(int) * TEST_W * TEST_H, TEST_W * sizeof(int));
        int *out = (int *) memalign(sizeof(int) * TEST_W * TEST_H, TEST_W * sizeof(int));
        /*memset( src, 0, sizeof(int) * TEST_W * TEST_H);
        memset( out, 0, sizeof(int) * TEST_W * TEST_H);*/

        /*        int _src[TEST_W * TEST_H] __attribute__((aligned(16)));
                int _out[TEST_W * TEST_H] __attribute__((aligned(16)));

                int *src = _src;
                int *out = _out;*/
#else
        int *src  = (int *) malloc(sizeof(int) * TEST_W * TEST_H);
        int *out = (int *) malloc(sizeof(int) * TEST_W * TEST_H);
#endif

        srand(time(NULL));
        for (int y = 0; y < TEST_H; y++)
            for (int x = 0; x < TEST_W; x++)
                *(src + y * TEST_W + x) = rand();

#ifdef SSE_PREFETCH
        clock_gettime(CLOCK_REALTIME, &start);
#ifdef ALIGN
        sse_prefetch_transpose_align(src, out, TEST_W, TEST_H);
#else
        sse_prefetch_transpose(src, out, TEST_W, TEST_H);
#endif
        clock_gettime(CLOCK_REALTIME, &end);
        printf("sse prefetch: \t %ld us\n", diff_in_us(start, end));
#endif


#ifdef SSE
        clock_gettime(CLOCK_REALTIME, &start);
#ifdef ALIGN
        sse_transpose_align(src, out, TEST_W, TEST_H);
#else
        sse_transpose(src, out, TEST_W, TEST_H);
#endif
        clock_gettime(CLOCK_REALTIME, &end);
        printf("sse: \t\t %ld us\n", diff_in_us(start, end));
#endif

#ifdef AVX
        clock_gettime(CLOCK_REALTIME, &start);
        avx_transpose(src, out, TEST_W, TEST_H);
        clock_gettime(CLOCK_REALTIME, &end);
        printf("avx: \t\t %ld us\n", diff_in_us(start, end));
#endif

#ifdef SSE_PTHREAD
        clock_gettime(CLOCK_REALTIME, &start);
        sse_pthread_transpose(src, out, TEST_W, TEST_H, 8);//FIXME: dynamic
        clock_gettime(CLOCK_REALTIME, &end);
        printf("sse pthread: \t\t %ld us\n", diff_in_us(start, end));
#endif


#ifdef NAIVE
        clock_gettime(CLOCK_REALTIME, &start);
        naive_transpose(src, out, TEST_W, TEST_H);
        clock_gettime(CLOCK_REALTIME, &end);
        printf("naive: \t\t %ld us\n", diff_in_us(start, end));
#endif

#ifndef ALIGN
        free(src);
        free(out);
#endif
    }

    return 0;
}
