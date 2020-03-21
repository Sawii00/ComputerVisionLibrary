#include <cstdint>

#define min_(a,b) (((a) < (b)) ? (a) : (b))
#define max_(a,b) (((a) > (b)) ? (a) : (b))
#define cudaMirrorGet(x, y, w, h) min_(abs(x), 2 * (w - 1) - x) + w * min_(abs(y), 2 * (h - 1) - y);

class GPU_utils {
public:
	static int gpuConvolve(float* filter, uint8_t** im, int filter_width, int filter_height, int im_width, int im_height);
};