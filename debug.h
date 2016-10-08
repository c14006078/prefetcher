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
#endif
