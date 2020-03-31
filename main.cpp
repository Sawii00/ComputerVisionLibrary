#include <immintrin.h>
#include <random>
#include <iostream>
#include "filter.h"
#include "image.h"
//#include "Utils.h"
#include "UnitTesting.h"
#include "morphology.h"
#include "filtering.h"
/*
PLANS:
- HSL Thresholding
- add passes to Gpu Library and Recompile

*/

int main() {
	RGBImage img;

	if (img.loadBMP("resources/img.bmp"))
	{
		UnitTesting test;
		test.runTests(img);
		system("pause");
		return 0;
	}
}