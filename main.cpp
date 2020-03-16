#include "filter.h"
#include "image.h"
#include "Utils.h"
#include <iostream>

int main() {
	Image img;
	Image img2;
	Image img3;
	if (img.loadBMP("D:\\Download\\bmp24.bmp"))
	{
		img2 = img;
		img3 = img;
		/*
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
		img.displayImage();*/

		/*Filter f1(1, 3, false);
		Filter f2(3, 1, false);
		float f_1[3] = { 1,2,1 };
		float f_2[3] = { -1,0,1 };
		f1.build(f_1, 3);
		f2.build(f_2, 3);

		Filter f3(3, 3, true);
		KernelPixel val(0.005, 0.4, 0.005);
		KernelPixel vals[9] = { val, val, val, val, val, val ,val ,val, val };
		f3.build(vals, 9);

		img2.separableConvolution1D(f1, f2);
		img2.convolve3D(f3);
		img2 += (img * 0.6);
		img2.displayImage();*/

		Filter gau(7, 7, false);
		img3.displayImage();
		if (gau.buildAsGaussian())
			img3.convolve1D(gau);
		img3.displayImage();
	}

	return 0;
}