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
	
	if(img.loadBMP("resources/boiler.bmp"))
	{
		UnitTesting test;
		//test.runTests(img);
		
		drawRect(img, img.width() - 50, img.height() / 2, 100, 100, Color::RED);
		
		drawRect(img, img.width(),img.height(), 50, 50, Color::BLUE, DrawingPositionMode::CENTER);
		
		drawBoundingRect(img, img.width() - 50, img.height() - 25, 100, 100, 5, Color::GREEN);
		
		drawBoundingRect(img, img.width() / 2, img.height() / 2, 100, 100, 5, Color::WHITE, DrawingPositionMode::CENTER);
		
		drawLine(img, 15, 15, 46, 50, 5, Color::RED);
		drawLine(img, img.width() / 2, 0, img.width(), img.height(), 15, Color::RED);
		drawLine(img, 20,0, 20, 340, 10, Color::BLUE);
		
		img.displayImage();
		
		system("pause");
		return 0;
	}
	
}


