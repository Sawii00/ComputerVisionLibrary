#pragma once
#include <initializer_list>
#include "img.h"
#include <immintrin.h>

#define _mm256_cmpge_epu8(a, b) _mm256_cmpeq_epi8(_mm256_max_epu8(a, b), a)
#define _mm256_cmple_epu8(a, b) _mm256_cmpge_epu8(b, a)


int erode(Img& image, uint8_t kernel_size = 3);
int dilate(Img& image, uint8_t kernel_size = 3);
int close(Img& image, uint8_t kernel_size = 3);
int open(Img& image, uint8_t kernel_size = 3);

void threshold(Img& img, std::initializer_list<float> min, std::initializer_list<float> max);