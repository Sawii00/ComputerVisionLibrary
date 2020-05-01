#pragma once
#include "image_manipulation.h"
#include "Utils.h"
#include "drawing.h"

void testAll()
{
	Img rgb, gray, hsl;
	load(rgb, "resources/img.bmp");
	imgcpy(rgb, hsl, ImageType::HSL);
	imgcpy(rgb, gray, ImageType::GRAY);
	
	/*
			Utils::TimedBlock th_hsl("ThresholdHSL");
			threshold(hsl, { 10, 0.1, 0.4 }, { 100, 0.2, 0.1 });
			th_hsl.stopTimedBlock();
			Utils::TimedBlock th_rgb("ThresholdRGB");
			threshold(rgb, { 10, 10, 10 }, { 66, 10, 165 });
			th_rgb.stopTimedBlock();
			Utils::TimedBlock th_gray("ThresholdGRAY");
			threshold(gray, { 10}, { 50 });
			th_gray.stopTimedBlock();
			
			 */
	drawRect(rgb, 100, 100, 100, 100, Color::BLUE, DrawingPositionMode::UPPER_CORNER);
	drawRect(rgb, 800 , 250, 247, 430, Color::GREEN, DrawingPositionMode::CENTER);
	
	drawRect(hsl, 100, 100, 100, 100, Color::BLUE, DrawingPositionMode::UPPER_CORNER);
	drawRect(hsl, 800 , 250, 247, 430, Color::GREEN, DrawingPositionMode::CENTER);
	
	drawRect(gray, 100, 100, 100, 100, Color::BLUE, DrawingPositionMode::UPPER_CORNER);
	drawRect(gray, 800 , 250, 247, 430, Color::GREEN, DrawingPositionMode::CENTER);
	
	show(rgb);
	show(hsl);
	show(gray);
}