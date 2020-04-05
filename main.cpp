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
- Unify Support boleans (Has Cuda SUpport does not need to exist in both hsl and rgb image)
*/

int main() {
	RGBImage img;
	
	/*
		if (img.loadBMP("resources/img.bmp"))
		{
			UnitTesting test;
			test.runTests(img);
			system("pause");
			return 0;
		}
			 */
	
	if(img.loadBMP("resources/boiler.bmp"))
	{
		HSLImage hsl(img);
		
		HSL_Pixel t(180, 1.0f, 0.3f);
		
		Filters::threshold(hsl, t);
		
	}
	
	
}