#pragma once
#include "image_manipulation.h"
#include "Utils.h"

void testAll()
{
	Img rgb, gray, hsl;
	load(rgb, "resources/img.bmp");
	imgcpy(rgb, gray, ImageType::GRAY);
	imgcpy(rgb, hsl, ImageType::HSL);
	
	Utils::TimedBlock th_hsl("ThresholdHSL");
	threshold(hsl, { 10, 0.1, 0.4 }, { 100, 0.2, 0.1 });
	th_hsl.stopTimedBlock();
	Utils::TimedBlock th_rgb("ThresholdRGB");
	threshold(rgb, { 10, 10, 10 }, { 66, 10, 165 });
	th_rgb.stopTimedBlock();
	Utils::TimedBlock th_gray("ThresholdGRAY");
	threshold(gray, { 10}, { 50 });
	th_gray.stopTimedBlock();
	show(rgb);
	show(hsl);
	show(gray);
	
}