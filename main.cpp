#include <immintrin.h>
#include <random>
#include <iostream>
#include "filter.h"
#include "image.h"
//#include "Utils.h"
#include "UnitTesting.h"
#include "morphology.h"
#include "filtering.h"
#include "drawings.h"

/*
PLANS:
- Unify Support boleans (Has Cuda SUpport does not need to exist in both hsl and rgb image)

- Render Shapes on screen
- Blob detection
- Canny edge detection

*/

int main() {
	RGBImage img;
	
	if(img.loadBMP("resources/test2.bmp"))
	{
		//UnitTesting test;
		//test.runTests(img);
		//Utils::TimedBlock rect("RectNormal");
		//drawRect(img, img.width() / 2, img.height() / 2, 100, 300, Color::RED);
		//rect.stopTimedBlock();
		//drawRect(img, img.width(),img.height(), 50, 50, Color::BLUE, DrawingPositionMode::CENTER);
		
		//drawBoundingRect(img, img.width() - 50, img.height() - 25, 100, 100, 5, Color::GREEN);
		
		//drawBoundingRect(img, img.width() / 2, img.height() / 2, 100, 100, 5, Color::WHITE, DrawingPositionMode::CENTER);
		
		//drawLine(img, 15, 15, 46, 50, 5, Color::RED);
		//drawLineImproved(img, img.width() / 2, 0, 0, img.height(), 15, Color::RED);
		//Utils::TimedBlock rect_rotated("RotatedRect");
		//drawRotatedRect(img, img.width() / 2, img.height(), 100, 300, 45.0f, Color::GREEN, DrawingPositionMode::CENTER);
		//rect_rotated.stopTimedBlock();
		
		
		//Utils::TimedBlock normal("NormalLine");
		//drawLine(img, 80, 450, 100, 200, 5, Color::RED);
		//normal.stopTimedBlock();
		//Utils::TimedBlock improved("ImprovedLine");
		//drawLineImproved(img, 80, 450, 100, 200, 5, Color::WHITE);
		//improved.stopTimedBlock();
		//Utils::TimedBlock rect("LineWithRect");
		//drawLineWithRect(img, 80, 450, 100, 200, 5, Color::RED);
		//rect.stopTimedBlock();
		
		Filter green_filter(3, true);
        KernelPixel green_val(0.005, 0.4, 0.005);
        KernelPixel green_vals[9] = { green_val,green_val,green_val,green_val,green_val,green_val,green_val,green_val,green_val };
        green_filter.build(green_vals);
        green_filter *= 0.8;
		
        Filter sharpen(3, false);
        float sharp[9] = { 0,-1,0,-1,5,-1,0,-1,0 };
        sharpen.build(sharp);
		

		
        Filters::gaussianBlur(img, 7);
		img.saveBMP("Blurred.bmp");
        Filters::canny(img, 50, 150);
        img.convolve(green_filter);
        img.convolve(sharpen);
        img.displayImage();
        img.saveBMP("MetCanny.bmp");
		
		
		//img.displayImage();
		
		
		system("pause");
		return 0;
	}
	
}


