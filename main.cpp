#include "filter.h"
#include "image.h"
#include "Utils.h"
#include <iostream>
#include <random>
#include <immintrin.h>

//TODO(SAWII): read source code of OpenCV... look at TBB, read pdfs, implement image operations simd

int main() {
	Image img;
	Image img2;
	Image img3;

	if (img.loadBMP("img.bmp"))
	{
		img2 = img;
		img3 = img;

		Filter filter(3, 3, false);
		float f[9] = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
		filter.build(f, 9);

		Filter f2(3, 3, true);
		KernelPixel val(0.005, 0.4, 0.005);
		KernelPixel vals[9] = { val, val, val, val, val, val ,val ,val, val };
		f2.build(vals, 9);

		TimedBlock test("Test");
		img.convolve1D(filter);

		img.convolve3D(f2);

		img += (img2 * 0.8);
		test.stopTimedBlock();

		img.displayImage();

		/*
										Filter f1(1, 3, false);
										Filter f2(3, 1, false);
										float f_1[3] = { 1,2,1 };
										float f_2[3] = { -1,0,1 };
										f1.build(f_1, 3);
										f2.build(f_2, 3);

										Filter f3(3, 3, true);
										KernelPixel val(0.005, 0.4, 0.005);
										KernelPixel vals[9] = { val, val, val, val, val, val ,val ,val, val };
										f3.build(vals, 9);
										Timer t;
										t.start();
										img2.separableConvolution1D(f1, f2);
										img2.convolve3D(f3);
										img2 += (img * 0.6);
										t.stop();

										std::cout<<"Time: "<<t.elapsedSeconds();

										img2.displayImage();

											Filter gau(7, 7, false);
											img3.displayImage();
											Timer t;
											t.start();
											if (gau.buildAsGaussian())
												img3.convolve1D(gau);
											t.stop();
											std::cout<<"Time: "<<t.elapsedMilliseconds();
											img3.displayImage();

											Filter f2(1, 3, false);
											Filter f1(3, 1, false);
											float val = 1.0f / 3;
											float f_[3] = { val,val,val };
											f1.build(f_, 3);
											f2.build(f_, 3);

											Timer t;
											t.start();
											img.convolveThreading(f2, f1);
											t.stop();

											std::cout << "Time: " << t.elapsedMicroseconds();
											img.displayImage();
										}

											srand(time(NULL));
										float* sburlo1 = new float[100000000];
										float* sburlo2 = new float[100000000];
										for (int i = 0; i < 100000000; i++) {
											sburlo1[i] = rand();
											sburlo2[i] = rand();
										}

										Timer t;
										t.start();
										for (int i = 0; i < 100000000; i++) {
											sburlo1[i] += sburlo2[i];
										}
										t.stop();
										std::cout << "CPU: " << t.elapsedMicroseconds() << '\n';

										int remaining = (100000000) % 8;
										int steps = (100000000) / 8;
										t.start();
										for (register int i = 0; i < steps; i++) {
											__m256 first = _mm256_add_ps(_mm256_load_ps(sburlo1 + i * 8), _mm256_load_ps(sburlo2 + i * 8));
											_mm256_storeu_ps(sburlo1 + 8 * i, first);
										}
										for (register int i = 100000000 - remaining; i < 100000000; i++) {
											sburlo1[i] += *(sburlo2 + i);
										}
										t.stop();
										std::cout << "SIMD: " << t.elapsedMicroseconds();
										system("pause");*/
		return 0;
	}
}