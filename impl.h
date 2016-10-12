#include <stdio.h>
#include <stdlib.h>
#include <xmmintrin.h>
#include <immintrin.h>
#include <pthread.h>
#include <assert.h>

void naive_transpose(int *src, int *dst, int w, int h);

void sse_transpose(int *src, int *dst, int w, int h);

void sse_prefetch_transpose(int *src, int *dst, int w, int h);

void sse_transpose_align(int *src, int *dst, int w, int h);

void sse_prefetch_transpose_align(int *src, int *dst, int w, int h);

void avx_transpose(int *src, int *dst, int w, int h);

//Pthread
struct _sse {
    int* src;
    int* dst;
    int    w;
    int    h;
};

typedef struct _sse *sse_arg;

sse_arg new_sse_arg(int* src, int* dst, int w, int h);

void sse_pthread_transpose(int *src, int *dst, int w, int h, int thrd_num);

void sse_thread(void *_arg);

void sse_pthread_prefetch_transpose(int *src, int *dst, int w, int h, int thrd_num);

void sse_prefetch_thread(void *_arg);

void sse_unpack32_transpose(int *src, int *dst, int w, int h);
