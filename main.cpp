#include <immintrin.h>
#include <random>
#include <iostream>
#include "filter.h"
#include "image.h"
#include "Utils.h"
#include "UnitTesting.h"

//TODO(SAWII): read source code of OpenCV... look at TBB, read pdfs, implement image operations simd

// TODO(Sawii00): CHECK PERFORMANCE BOTTLENECK SEPARABLEMULTITHREADED VS NORMALMULTITHREADED

int main() {
	Image img;

	if (img.loadBMP("resources/img.bmp"))
	{
		UnitTesting test;
		test.runTests(img);

		system("pause");
		return 0;
	}
}