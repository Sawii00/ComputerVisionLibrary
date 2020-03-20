#include "filter.h"
#include "image.h"
#include "Utils.h"
#include <iostream>
#include <random>
#include <immintrin.h>
#include "UnitTesting.h"

//TODO(SAWII): read source code of OpenCV... look at TBB, read pdfs, implement image operations simd

int main() {
	Image img;

	if (img.loadBMP("resources/img.bmp"))
	{
		//testNormalConvolution1D(img);
		//testNormalConvolution3D(img);
		//testNormalSeparableConvolution(img);
		//testMultiThreadedConvolution(img);
		testGPUConvolution(img);
		testGPUConvolution(img);

		system("pause");
		return 0;
	}
}