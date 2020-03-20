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
    
	if (img.loadBMP("img.bmp"))
	{
		Image img2 = img;
		Image img3 = img;
		Image img4 = img;
		
		testNormalConvolution1D(img);
		testNormalConvolution3D(img2);
		testNormalSeparableConvolution(img3);
		testMultiThreadedConvolution(img4);
		
        system("pause");
		return 0;
	}
}