#include "filter.h"
#include "image.h"
#include "Utils.h"

int main() {
	Timer t;
	Image img;
	Image img2;
	if (img.loadBMP("img.bmp"))
	{
		t.start();
		img2 = img;
		Filter filter(3, 3, false);
		float f[9] = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
		filter.build(f, 9);

		Filter f2(3, 3, true);
		KernelPixel val(0.005, 0.4, 0.005);
		KernelPixel vals[9] = { val, val, val, val, val, val ,val ,val, val };
		f2.build(vals, 9);

		img.convolve1D(filter);

		img.convolve3D(f2);

		img += (img2 * 0.8);

		std::cout << "Time: " << t.elapsedMilliseconds();
		img.displayImage();
	}
	t.stop();

	return 0;
}