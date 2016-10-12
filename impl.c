#ifndef TRANSPOSE_IMPL
#define TRANSPOSE_IMPL

#include "impl.h"

#ifdef DEBUG
#include "debug.h"
#endif

void naive_transpose(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            *(dst + x * h + y) = *(src + y * w + x);
}

void sse_transpose(int *src, int *dst, int w, int h)
{
#ifdef DEBUG
    static int sse_times = 0;
#endif
    for (int x = 0; x < w; x += 4) {
        for (int y = 0; y < h; y += 4) {
            __m128i I0 = _mm_loadu_si128((__m128i *)(src + (y + 0) * w + x));
            __m128i I1 = _mm_loadu_si128((__m128i *)(src + (y + 1) * w + x));
            __m128i I2 = _mm_loadu_si128((__m128i *)(src + (y + 2) * w + x));
            __m128i I3 = _mm_loadu_si128((__m128i *)(src + (y + 3) * w + x));

#ifdef DEBUG
            if( sse_times < 1) {
                printf("load\n");
                show_sse_mtx(I0, I1, I2, I3);
            }
#endif
            __m128i T0 = _mm_unpacklo_epi32(I0, I1);
            __m128i T1 = _mm_unpacklo_epi32(I2, I3);
            __m128i T2 = _mm_unpackhi_epi32(I0, I1);
            __m128i T3 = _mm_unpackhi_epi32(I2, I3);

#ifdef DEBUG
            if( sse_times < 1) {
                printf("unpacklo/hi 32\n");
                show_sse_mtx(T0, T1, T2, T3);
            }
#endif

            I0 = _mm_unpacklo_epi64(T0, T1);
            I1 = _mm_unpackhi_epi64(T0, T1);
            I2 = _mm_unpacklo_epi64(T2, T3);
            I3 = _mm_unpackhi_epi64(T2, T3);

#ifdef DEBUG
            if( sse_times < 1) {
                printf("unpacklo/hi 64\n");
                show_sse_mtx(I0, I1, I2, I3);
            }
#endif

            _mm_storeu_si128((__m128i *)(dst + ((x + 0) * h) + y), I0);
            _mm_storeu_si128((__m128i *)(dst + ((x + 1) * h) + y), I1);
            _mm_storeu_si128((__m128i *)(dst + ((x + 2) * h) + y), I2);
            _mm_storeu_si128((__m128i *)(dst + ((x + 3) * h) + y), I3);
        }
    }
#ifdef DEBUG
    sse_times++;
#endif
}

void sse_prefetch_transpose(int *src, int *dst, int w, int h)
{
    register __m128i I0, I1, I2, I3, T0, T1, T2, T3;
    for (register int x = 0; x < w; x += 4) {
        for (register int y = 0; y < h; y += 4) {
#define PFDIST  8

            _mm_prefetch(src+(y + PFDIST + 0) *w + x, _MM_HINT_T0);
            _mm_prefetch(src+(y + PFDIST + 1) *w + x, _MM_HINT_T0);
            _mm_prefetch(src+(y + PFDIST + 2) *w + x, _MM_HINT_T0);
            _mm_prefetch(src+(y + PFDIST + 3) *w + x, _MM_HINT_T0);

            I0 = _mm_load_si128 ((__m128i *)(src + (y + 0) * w + x));
            I1 = _mm_load_si128 ((__m128i *)(src + (y + 1) * w + x));
            I2 = _mm_load_si128 ((__m128i *)(src + (y + 2) * w + x));
            I3 = _mm_load_si128 ((__m128i *)(src + (y + 3) * w + x));
            T0 = _mm_unpacklo_epi32(I0, I1);
            T1 = _mm_unpacklo_epi32(I2, I3);
            T2 = _mm_unpackhi_epi32(I0, I1);
            T3 = _mm_unpackhi_epi32(I2, I3);
            I0 = _mm_unpacklo_epi64(T0, T1);
            I1 = _mm_unpackhi_epi64(T0, T1);
            I2 = _mm_unpacklo_epi64(T2, T3);
            I3 = _mm_unpackhi_epi64(T2, T3);
            _mm_store_si128((__m128i *)(dst + ((x + 0) * h) + y), I0);
            _mm_store_si128((__m128i *)(dst + ((x + 1) * h) + y), I1);
            _mm_store_si128((__m128i *)(dst + ((x + 2) * h) + y), I2);
            _mm_store_si128((__m128i *)(dst + ((x + 3) * h) + y), I3);
        }
    }
}

void sse_transpose_align(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x += 4) {
        for (int y = 0; y < h; y += 4) {
            __m128i I0 = _mm_load_si128((__m128i *)(src + (y + 0) * w + x));
            __m128i I1 = _mm_load_si128((__m128i *)(src + (y + 1) * w + x));
            __m128i I2 = _mm_load_si128((__m128i *)(src + (y + 2) * w + x));
            __m128i I3 = _mm_load_si128((__m128i *)(src + (y + 3) * w + x));

#ifdef DEBUG
            static int sse_times = 0;
            if( sse_times < 1) {
                printf("load\n");
                show_sse_mtx(I0, I1, I2, I3);
            }
#endif
            __m128i T0 = _mm_unpacklo_epi32(I0, I1);
            __m128i T1 = _mm_unpacklo_epi32(I2, I3);
            __m128i T2 = _mm_unpackhi_epi32(I0, I1);
            __m128i T3 = _mm_unpackhi_epi32(I2, I3);

#ifdef DEBUG
            if( sse_times < 1) {
                printf("unpacklo/hi 32\n");
                show_sse_mtx(T0, T1, T2, T3);
            }
#endif

            I0 = _mm_unpacklo_epi64(T0, T1);
            I1 = _mm_unpackhi_epi64(T0, T1);
            I2 = _mm_unpacklo_epi64(T2, T3);
            I3 = _mm_unpackhi_epi64(T2, T3);

#ifdef DEBUG
            if( sse_times++ < 1) {
                printf("unpacklo/hi 64\n");
                show_sse_mtx(I0, I1, I2, I3);
            }
#endif

            _mm_store_si128((__m128i *)(dst + ((x + 0) * h) + y), I0);
            _mm_store_si128((__m128i *)(dst + ((x + 1) * h) + y), I1);
            _mm_store_si128((__m128i *)(dst + ((x + 2) * h) + y), I2);
            _mm_store_si128((__m128i *)(dst + ((x + 3) * h) + y), I3);
        }
    }
}

void sse_prefetch_transpose_align(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x += 4) {
        for (int y = 0; y < h; y += 4) {
#define PFDIST  8
            _mm_prefetch(src+(y + PFDIST + 0) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 1) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 2) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 3) *w + x, _MM_HINT_T1);

            __m128i I0 = _mm_load_si128 ((__m128i *)(src + (y + 0) * w + x));
            __m128i I1 = _mm_load_si128 ((__m128i *)(src + (y + 1) * w + x));
            __m128i I2 = _mm_load_si128 ((__m128i *)(src + (y + 2) * w + x));
            __m128i I3 = _mm_load_si128 ((__m128i *)(src + (y + 3) * w + x));
            __m128i T0 = _mm_unpacklo_epi32(I0, I1);
            __m128i T1 = _mm_unpacklo_epi32(I2, I3);
            __m128i T2 = _mm_unpackhi_epi32(I0, I1);
            __m128i T3 = _mm_unpackhi_epi32(I2, I3);
            I0 = _mm_unpacklo_epi64(T0, T1);
            I1 = _mm_unpackhi_epi64(T0, T1);
            I2 = _mm_unpacklo_epi64(T2, T3);
            I3 = _mm_unpackhi_epi64(T2, T3);
            _mm_store_si128((__m128i *)(dst + ((x + 0) * h) + y), I0);
            _mm_store_si128((__m128i *)(dst + ((x + 1) * h) + y), I1);
            _mm_store_si128((__m128i *)(dst + ((x + 2) * h) + y), I2);
            _mm_store_si128((__m128i *)(dst + ((x + 3) * h) + y), I3);
        }
    }
}

void avx_transpose(int *src, int *dst, int w, int h)
{
    __m256i I0, I1, I2, I3, I4, I5, I6, I7,
            T0, T1, T2, T3, T4, T5, T6, T7;
    for (int x = 0; x < w; x += 8) {
        for (int y = 0; y < h; y += 8) {
            I0 = _mm256_loadu_si256((__m256i *)(src + (y + 0) * w + x));
            I1 = _mm256_loadu_si256((__m256i *)(src + (y + 1) * w + x));
            I2 = _mm256_loadu_si256((__m256i *)(src + (y + 2) * w + x));
            I3 = _mm256_loadu_si256((__m256i *)(src + (y + 3) * w + x));
            I4 = _mm256_loadu_si256((__m256i *)(src + (y + 4) * w + x));
            I5 = _mm256_loadu_si256((__m256i *)(src + (y + 5) * w + x));
            I6 = _mm256_loadu_si256((__m256i *)(src + (y + 6) * w + x));
            I7 = _mm256_loadu_si256((__m256i *)(src + (y + 7) * w + x));

#ifdef DEBUG
            static int avx_times = 0;
            if( avx_times < 1) {
                printf("avx load\n");
                show_avx_mtx(I0, I1, I2, I3, I4, I5, I6, I7);
            }
#endif
            T0 = _mm256_unpacklo_epi32(I0, I1);
            T1 = _mm256_unpacklo_epi32(I2, I3);
            T2 = _mm256_unpacklo_epi32(I4, I5);
            T3 = _mm256_unpacklo_epi32(I6, I7);

            T4 = _mm256_unpackhi_epi32(I0, I1);
            T5 = _mm256_unpackhi_epi32(I2, I3);
            T6 = _mm256_unpackhi_epi32(I4, I5);
            T7 = _mm256_unpackhi_epi32(I6, I7);

#ifdef DEBUG
            if( avx_times < 1) {
                printf("avx unpacklo/hi 32\n");
                show_avx_mtx(T0, T1, T2, T3, T4, T5, T6, T7);
            }
#endif

            I0 = _mm256_unpacklo_epi64(T0, T1);
            I1 = _mm256_unpackhi_epi64(T0, T1);
            I2 = _mm256_unpacklo_epi64(T2, T3);
            I3 = _mm256_unpackhi_epi64(T2, T3);
            I4 = _mm256_unpacklo_epi64(T4, T5);
            I5 = _mm256_unpackhi_epi64(T4, T5);
            I6 = _mm256_unpacklo_epi64(T6, T7);
            I7 = _mm256_unpackhi_epi64(T6, T7);

#ifdef DEBUG
            if( avx_times++ < 1) {
                printf("avx unpacklo/hi 64\n");
                show_avx_mtx(I0, I1, I2, I3, I4, I5, I6, I7);
            }
#endif
            //FIXME: we need a better way for unpack_ep128(latency 3)

            T0 = _mm256_permute2x128_si256(I0, I1, 0x20);//unpacklo_ep128
            T1 = _mm256_permute2x128_si256(I0, I1, 0x31);//unpackhigh_ep128
            T2 = _mm256_permute2x128_si256(I2, I3, 0x20);
            T3 = _mm256_permute2x128_si256(I2, I3, 0x31);
            T4 = _mm256_permute2x128_si256(I4, I5, 0x20);
            T5 = _mm256_permute2x128_si256(I4, I5, 0x31);
            T6 = _mm256_permute2x128_si256(I6, I7, 0x20);
            T7 = _mm256_permute2x128_si256(I6, I7, 0x31);

#ifdef DEBUG
            if( avx_times < 1) {
                printf("avx unpacklo/hi 128\n");
                show_avx_mtx(T0, T1, T2, T3, T4, T5, T6, T7);
            }
#endif

            _mm256_storeu_si256((__m256i *)(dst + ((x + 0) * h) + y), T0);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 1) * h) + y), T1);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 2) * h) + y), T2);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 3) * h) + y), T3);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 4) * h) + y), T4);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 5) * h) + y), T5);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 6) * h) + y), T6);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 7) * h) + y), T7);

        }
    }
}

//sse pthread version
void sse_pthread_transpose(int *src, int *dst, int w, int h, int thrd_num)
{
    pthread_t *tid = (pthread_t *) malloc(sizeof(pthread_t) * thrd_num);
    sse_arg* arg = (sse_arg*) malloc(sizeof(sse_arg) * thrd_num);

    assert((h % thrd_num) == 0 && "Matrix size should be divisible by Thread Num");

    int split = h / thrd_num;//split the matrix
    for(int i = 0; i < thrd_num; i++) {
        arg[i] = new_sse_arg(src + i * split * w, dst + i * split, w, split);
        pthread_create(&tid[i], NULL, (void *) &sse_thread, (void *) arg[i]);
        //pthread_join(tid[i], NULL);
    }

    for(int i = 0; i < thrd_num; i++)
        pthread_join(tid[i], NULL);
}



void sse_thread(void *_arg)
{
    sse_arg arg = (sse_arg) _arg;

    int *src = arg->src;
    int *dst = arg->dst;
    int w = arg->w;
    int h = arg->h;

#ifdef DEBUG
    static int sse_times = 0;
#endif
    for (int x = 0; x < w; x += 4) {
        for (int y = 0; y < h; y += 4) {
            __m128i I0 = _mm_loadu_si128((__m128i *)(src + (y + 0) * w + x));
            __m128i I1 = _mm_loadu_si128((__m128i *)(src + (y + 1) * w + x));
            __m128i I2 = _mm_loadu_si128((__m128i *)(src + (y + 2) * w + x));
            __m128i I3 = _mm_loadu_si128((__m128i *)(src + (y + 3) * w + x));

#ifdef DEBUG
            if( sse_times < 1) {
                printf("load\n");
                show_sse_mtx(I0, I1, I2, I3);
            }
#endif
            __m128i T0 = _mm_unpacklo_epi32(I0, I1);
            __m128i T1 = _mm_unpacklo_epi32(I2, I3);
            __m128i T2 = _mm_unpackhi_epi32(I0, I1);
            __m128i T3 = _mm_unpackhi_epi32(I2, I3);

#ifdef DEBUG
            if( sse_times < 1) {
                printf("unpacklo/hi 32\n");
                show_sse_mtx(T0, T1, T2, T3);
            }
#endif

            I0 = _mm_unpacklo_epi64(T0, T1);
            I1 = _mm_unpackhi_epi64(T0, T1);
            I2 = _mm_unpacklo_epi64(T2, T3);
            I3 = _mm_unpackhi_epi64(T2, T3);

#ifdef DEBUG
            if( sse_times < 1) {
                printf("unpacklo/hi 64\n");
                show_sse_mtx(I0, I1, I2, I3);
            }
#endif

            _mm_storeu_si128((__m128i *)(dst + ((x + 0) * w) + y), I0);
            _mm_storeu_si128((__m128i *)(dst + ((x + 1) * w) + y), I1);
            _mm_storeu_si128((__m128i *)(dst + ((x + 2) * w) + y), I2);
            _mm_storeu_si128((__m128i *)(dst + ((x + 3) * w) + y), I3);
        }
    }
}

//sse pthread prefetch version
void sse_pthread_prefetch_transpose(int *src, int *dst, int w, int h, int thrd_num)
{
    pthread_t *tid = (pthread_t *) malloc(sizeof(pthread_t) * thrd_num);
    sse_arg* arg = (sse_arg*) malloc(sizeof(sse_arg) * thrd_num);

    assert((h % thrd_num) == 0 && "Matrix size should be divisible by Thread Num");

    int split = h / thrd_num;//split the matrix
    for(int i = 0; i < thrd_num; i++) {
        arg[i] = new_sse_arg(src + i * split * w, dst + i * split, w, split);
        pthread_create(&tid[i], NULL, (void *) &sse_prefetch_thread, (void *) arg[i]);
        //pthread_join(tid[i], NULL);
    }

    for(int i = 0; i < thrd_num; i++)
        pthread_join(tid[i], NULL);
}

void sse_prefetch_thread(void *_arg)
{
    sse_arg arg = (sse_arg) _arg;

    int *src = arg->src;
    int *dst = arg->dst;
    int w = arg->w;
    int h = arg->h;

    register __m128i I0, I1, I2, I3, T0, T1, T2, T3;
    for (register int x = 0; x < w; x += 4) {
        for (register int y = 0; y < h; y += 4) {
#define PFDIST  8

            _mm_prefetch(src+(y + PFDIST + 0) *w + x, _MM_HINT_T0);
            _mm_prefetch(src+(y + PFDIST + 1) *w + x, _MM_HINT_T0);
            _mm_prefetch(src+(y + PFDIST + 2) *w + x, _MM_HINT_T0);
            _mm_prefetch(src+(y + PFDIST + 3) *w + x, _MM_HINT_T0);

            I0 = _mm_load_si128 ((__m128i *)(src + (y + 0) * w + x));
            I1 = _mm_load_si128 ((__m128i *)(src + (y + 1) * w + x));
            I2 = _mm_load_si128 ((__m128i *)(src + (y + 2) * w + x));
            I3 = _mm_load_si128 ((__m128i *)(src + (y + 3) * w + x));
            T0 = _mm_unpacklo_epi32(I0, I1);
            T1 = _mm_unpacklo_epi32(I2, I3);
            T2 = _mm_unpackhi_epi32(I0, I1);
            T3 = _mm_unpackhi_epi32(I2, I3);
            I0 = _mm_unpacklo_epi64(T0, T1);
            I1 = _mm_unpackhi_epi64(T0, T1);
            I2 = _mm_unpacklo_epi64(T2, T3);
            I3 = _mm_unpackhi_epi64(T2, T3);

            _mm_storeu_si128((__m128i *)(dst + ((x + 0) * w) + y), I0);
            _mm_storeu_si128((__m128i *)(dst + ((x + 1) * w) + y), I1);
            _mm_storeu_si128((__m128i *)(dst + ((x + 2) * w) + y), I2);
            _mm_storeu_si128((__m128i *)(dst + ((x + 3) * w) + y), I3);
        }
    }
}

sse_arg new_sse_arg(int* src, int* dst, int w, int h)
{
    sse_arg arg = (sse_arg) malloc(sizeof(struct _sse));

    arg->src = src;
    arg->dst = dst;
    arg->w   =   w;
    arg->h   =   h;

    return arg;
}

//Just use unpack32 no 64
void sse_unpack32_transpose(int *src, int *dst, int w, int h)
{
#ifdef DEBUG
    static int sse_unpack32_times = 0;
#endif
    for (int x = 0; x < w; x += 4) {
        for (int y = 0; y < h; y += 4) {
            __m128i I0 = _mm_loadu_si128((__m128i *)(src + (y + 0) * w + x));
            __m128i I1 = _mm_loadu_si128((__m128i *)(src + (y + 1) * w + x));
            __m128i I2 = _mm_loadu_si128((__m128i *)(src + (y + 2) * w + x));
            __m128i I3 = _mm_loadu_si128((__m128i *)(src + (y + 3) * w + x));

#ifdef DEBUG
            if( sse_unpack32_times < 1) {
                printf("load\n");
                show_sse_mtx(I0, I1, I2, I3);
            }
#endif
            __m128i T0 = _mm_unpacklo_epi32(I0, I2);
            __m128i T1 = _mm_unpacklo_epi32(I1, I3);
            __m128i T2 = _mm_unpackhi_epi32(I0, I2);
            __m128i T3 = _mm_unpackhi_epi32(I1, I3);

#ifdef DEBUG
            if( sse_unpack32_times < 1) {
                printf("unpacklo/hi 32\n");
                show_sse_mtx(T0, T1, T2, T3);
            }
#endif

            I0 = _mm_unpacklo_epi32(T0, T1);
            I1 = _mm_unpackhi_epi32(T0, T1);
            I2 = _mm_unpacklo_epi32(T2, T3);
            I3 = _mm_unpackhi_epi32(T2, T3);

#ifdef DEBUG
            if( sse_times < 1) {
                printf("unpacklo/hi 32\n");
                show_sse_mtx(I0, I1, I2, I3);
            }
#endif

            _mm_storeu_si128((__m128i *)(dst + ((x + 0) * h) + y), I0);
            _mm_storeu_si128((__m128i *)(dst + ((x + 1) * h) + y), I1);
            _mm_storeu_si128((__m128i *)(dst + ((x + 2) * h) + y), I2);
            _mm_storeu_si128((__m128i *)(dst + ((x + 3) * h) + y), I3);
        }
    }
#ifdef DEBUG
    sse_times++;
#endif
}

#endif /* TRANSPOSE_IMPL */
