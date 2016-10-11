#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

void show_vtr(int* in, int num)
{
    for(int i = 0; i < num; i++)
        printf("%2d ", *(in + i));
    printf("\n");
}

void show_mtx(int* in, int w, int h)
{
    for(int i = 0; i < h; i++)
        show_vtr(in + w * i, w);
}

void show_sse_mtx(__m128i a, __m128i b, __m128i c, __m128i d)
{
    show_vtr((int *) &a, 4);
    show_vtr((int *) &b, 4);
    show_vtr((int *) &c, 4);
    show_vtr((int *) &d, 4);
}

void show_avx_mtx(__m256i a, __m256i b, __m256i c, __m256i d, __m256i e, __m256i f, __m256i g, __m256i h)
{
    show_vtr((int *) &a, 8);
    show_vtr((int *) &b, 8);
    show_vtr((int *) &c, 8);
    show_vtr((int *) &d, 8);
    show_vtr((int *) &e, 8);
    show_vtr((int *) &f, 8);
    show_vtr((int *) &g, 8);
    show_vtr((int *) &h, 8);
}

#endif
