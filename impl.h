#include <stdio.h>
#include <xmmintrin.h>
#include <immintrin.h>
//#include <emmintrin.h>

void naive_transpose(int *src, int *dst, int w, int h);

void sse_transpose(int *src, int *dst, int w, int h);

void sse_prefetch_transpose(int *src, int *dst, int w, int h);

void sse_transpose_align(int *src, int *dst, int w, int h);

void sse_prefetch_transpose_align(int *src, int *dst, int w, int h);

void avx_transpose(int *src, int *dst, int w, int h);
