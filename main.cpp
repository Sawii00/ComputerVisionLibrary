#include <immintrin.h>
#include <random>
#include <iostream>
#include "filter.h"
#include "image.h"
#include "Utils.h"
#include "UnitTesting.h"
#include "morphology.h"

/*
PLANS:
 - erosion/dilation
- color conversion
*/

int main() {
	Image img;
	
	if (img.loadBMP("resources/img.bmp"))
	{
		UnitTesting test;
		//test.runTests(img);
		
		img.thresholdSegment(70);
		TimedBlock erosion("Dilate");
		morph::close(img, 5);
		erosion.stopTimedBlock();
		img.displayImage();
		system("pause");
		return 0;
	}
}