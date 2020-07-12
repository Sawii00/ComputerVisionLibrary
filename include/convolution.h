#pragma once

#include <thread>
#include <fstream>
#include <cstdint>
#include <iostream>
#include "img.h"
#include "safe_array.h"
#include "Utils.h"
#include "filter.h"
#include "GPU_utils.h"

static void RGB_separable_thread_operation_h(SeparableFilter* horizontal, uint32_t start, uint32_t finish, Img* im);
static void RGB_separable_thread_operation_v(SeparableFilter* vertical, uint32_t start, uint32_t finish, Img* im);
static void RGB_non_separable_thread_operation(Filter* filter, uint32_t start, uint32_t finish, Img* im);
static void RGB_non_separable_thread_operation3D(Filter* filter, uint32_t start, uint32_t finish, Img* im);
static void RGB_multiThread_1D_Separable_Convolution(Img* img, SeparableFilter& vertical, SeparableFilter& horizontal, uint8_t passes = 1, uint8_t thread_n = 4);
static void RGB_multiThread_1D_Convolution(Img* img, Filter& filter, uint8_t passes = 1, uint8_t thread_n = 4);
static void RGB_multiThread_3D_Convolution(Img* img, Filter& filter, uint8_t passes = 1, uint8_t thread_n = 4);
static void RGB_CUDA_Accelerated_1D_Convolution(Img* img, Filter& kernel, uint8_t passes = 1, uint8_t thread_n = 4);

static void GRAY_separable_thread_operation_h(SeparableFilter* horizontal, uint32_t start, uint32_t finish, Img* im);
static void GRAY_separable_thread_operation_v(SeparableFilter* vertical, uint32_t start, uint32_t finish, Img* im);
static void GRAY_non_separable_thread_operation(Filter* filter, uint32_t start, uint32_t finish, Img* im);
static void GRAY_multiThread_1D_Separable_Convolution(Img* img, SeparableFilter& vertical, SeparableFilter& horizontal, uint8_t passes = 1, uint8_t thread_n = 4);
static void GRAY_multiThread_1D_Convolution(Img* img, Filter& filter, uint8_t passes = 1, uint8_t thread_n = 4);
static void GRAY_CUDA_Accelerated_1D_Convolution(Img* img, Filter& kernel, uint8_t passes = 1, uint8_t thread_n = 4);

void convolve(Img& img, Filter& filter, uint8_t passes = 1, uint8_t thread_number = 4);
void convolve(Img& img, SeparableFilter& vertical, SeparableFilter& horizontal, uint8_t passes = 1, uint8_t thread_number = 4);