#pragma once
#include <cstdint>

#define min_(a,b) (((a) < (b)) ? (a) : (b))
#define max_(a,b) (((a) > (b)) ? (a) : (b))
#define max3_(a, b, c) (a > b) ? max_(a, c) : max_(b, c)
#define min3_(a, b, c) (a < b) ? min_(a, c) : min(b, c)
#define cudaMirrorGet(x, y, w, h) min_(abs(x), 2 * (w - 1) - x) + w * min_(abs(y), 2 * (h - 1) - y);

class GPU_utils{
	public:
	static int gpuConvolve(float* filter, uint8_t** im, int filter_width, int filter_height, int im_width, int im_height, int passages = 1);
	static int gpuRGBtoHSLImage(uint8_t** rgb_img, uint8_t** hsl_img, int img_w, int img_h);
	static int gpuHSLtoRGBImage(uint8_t** hsl_img, uint8_t** rgb_img, int img_w, int img_h);
	static int gpuCannyEdge(uint8_t** rgb_img, int im_w, int img_h, float thold_high, float thold_low);
};