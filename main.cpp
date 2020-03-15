#include "filter.h"
#include "image.h"

int main() {
	Image img;
	if (img.loadBMP("img.bmp"))
	{
		Filter filter(3, 3, false);
		float f[9] = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
		filter.build(f, 9);

		img.convolve1D(filter);

		img.displayImage();
	}
	return 0;
}