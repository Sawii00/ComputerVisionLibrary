#pragma once
#include "image_manipulation.h"
#include "convolution.h"
#include "morphology.h"
#include "Utils.h"
#include "drawing.h"

inline void resetImg(Img& rgb_bak, Img& rgb, Img& hsl, Img& gray) {
	imgcpy(rgb_bak, rgb);
	imgcpy(rgb_bak, hsl, ImageType::HSL);
	imgcpy(rgb_bak, gray, ImageType::GRAY);
}


void testAll()
{
	
	
	
	
	Img rgb_bak, rgb, gray, hsl;
	
	Utils::TimedBlock ld_rgb("Load IMG to RGB");
	load(rgb_bak, "resources/img.bmp");
	ld_rgb.stopTimedBlock();
	
	Utils::TimedBlock drawRectCorner("DrawRectCorner");
	//drawRect(rgb_bak, 100, 100, 100, 100, Color::BLUE);
	drawRectCorner.stopTimedBlock();
	
	Utils::TimedBlock drawRectCenter("DrawRectCenter");
	//drawRect(rgb_bak, 400, 500, 50, 100, Color::GREEN, DrawingPositionMode::CENTER);
	drawRectCenter.stopTimedBlock();
	
	
	Utils::TimedBlock drawRectCornerRotated("DrawRectCornerRotated");
	//drawRotatedRect(rgb_bak, 300, 400, 100, 100, 30.0, Color::BLUE);
	drawRectCornerRotated.stopTimedBlock();
	
	Utils::TimedBlock drawRectCenterRotated("DrawRectCenterRotated");
	//drawRotatedRect(rgb_bak, 600, 400, 75, 100, 45.0, Color::GREEN, DrawingPositionMode::CENTER);
	drawRectCenterRotated.stopTimedBlock();
	
	
	Utils::TimedBlock drawLineImproved("DrawLineImproved");
	//drawLine(rgb_bak, 50, 75, 100, 150, 1, Color::GREEN);
	drawLineImproved.stopTimedBlock();
	
	
	Utils::TimedBlock drawLineSlow("DrawLineSlow");
	drawLine(rgb_bak, 350, 75, 100, 10, 30, Color::RED);
	drawLineSlow.stopTimedBlock();
	
	
	show(rgb_bak);
	
	// NOTE(Sawii00): Claiming the GPU
	
	Filter filter(3);
	float f[] = {
		1,1,1,1,1,1,1,1,1,
	};
	filter.build(f);
	convolve(rgb_bak, filter);
	
	
	Utils::TimedBlock cp_rgb_rgb("Copy RGB to RGB");
	imgcpy(rgb_bak, rgb);
	cp_rgb_rgb.stopTimedBlock();
	
	Utils::TimedBlock cp_rgb_gray("Copy RGB to GRAY");
	imgcpy(rgb_bak, gray, ImageType::GRAY);
	cp_rgb_gray.stopTimedBlock();
	
	Utils::TimedBlock cp_rgb_hsl("Copy RGB to HSL");
	imgcpy(rgb_bak, hsl, ImageType::HSL);
	cp_rgb_hsl.stopTimedBlock();
	
	/*
		show(rgb);
		show(hsl);
		show(gray);
	 */
	
	Utils::TimedBlock th_hsl("ThresholdHSL");
	threshold(hsl, { 10, 0.1, 0.4 }, { 100, 0.2, 0.1 });
	th_hsl.stopTimedBlock();
	
	Utils::TimedBlock th_rgb("ThresholdRGB");
	threshold(rgb, { 10, 10, 10 }, { 66, 10, 165 });
	th_rgb.stopTimedBlock();
	
	Utils::TimedBlock th_gray("ThresholdGRAY");
	threshold(gray, { 10 }, { 50 });
	th_gray.stopTimedBlock();
	
	/*
		show(rgb);
		show(hsl);
		show(gray);
	 */
	
	resetImg(rgb_bak, rgb, hsl, gray);
	
	Utils::TimedBlock g_blur_rgb_gpu("RGB GPU Gaussian Blur");
	gaussianBlur(rgb);
	g_blur_rgb_gpu.stopTimedBlock();
	
	Utils::TimedBlock g_blur_gray_gpu("Gray GPU Gaussian Blur");
	gaussianBlur(gray);
	g_blur_gray_gpu.stopTimedBlock();
	
	/*
		show(rgb);
		show(gray);
	 */
	
	resetImg(rgb_bak, rgb, hsl, gray);
	
	rgb.setCudaSupport(false);
	gray.setCudaSupport(false);
	Utils::TimedBlock g_blur_rgb_cpu("RGB CPU Gaussian Blur");
	gaussianBlur(rgb);
	g_blur_rgb_cpu.stopTimedBlock();
	
	Utils::TimedBlock g_blur_gray_cpu("Gray CPU Gaussian Blur");
	gaussianBlur(gray);
	g_blur_gray_cpu.stopTimedBlock();
	
	/*
		show(rgb);
		show(gray);
	 */
	
	rgb.setCudaSupport(true);
	gray.setCudaSupport(true);
	
	resetImg(rgb_bak, rgb, hsl, gray);
	
	Utils::TimedBlock box_blur_rgb_gpu("RGB GPU Box Blur");
	boxBlur(rgb);
	box_blur_rgb_gpu.stopTimedBlock();
	
	Utils::TimedBlock box_blur_gray_gpu("Gray GPU Box Blur");
	boxBlur(gray);
	box_blur_gray_gpu.stopTimedBlock();
	
	/*
		show(rgb);
		show(gray);
	 */
	
	resetImg(rgb_bak, rgb, hsl, gray);
	
	rgb.setCudaSupport(false);
	gray.setCudaSupport(false);
	Utils::TimedBlock box_blur_rgb_cpu("RGB CPU Box Blur");
	boxBlur(rgb);
	box_blur_rgb_cpu.stopTimedBlock();
	
	Utils::TimedBlock box_blur_gray_cpu("Gray CPU Box Blur");
	boxBlur(gray);
	box_blur_gray_cpu.stopTimedBlock();
	
	/*
		show(rgb);
		show(gray);
	 */
	
	rgb.setCudaSupport(true);
	gray.setCudaSupport(true);
	
	resetImg(rgb_bak, rgb, hsl, gray);
	
	rgb.setCudaSupport(false);
	gray.setCudaSupport(false);
	Utils::TimedBlock box_blur_rgb_cpu_7("RGB CPU Box Blur 7x7");
	boxBlur(rgb, 7);
	box_blur_rgb_cpu_7.stopTimedBlock();
	
	Utils::TimedBlock box_blur_gray_cpu_7("Gray CPU Box Blur 7x7");
	boxBlur(gray, 7);
	box_blur_gray_cpu_7.stopTimedBlock();
	
	/*
		show(rgb);
		show(gray);
	 */
	
	rgb.setCudaSupport(true);
	gray.setCudaSupport(true);
	
	resetImg(rgb_bak, rgb, hsl, gray);
	
	Utils::TimedBlock canny_rgb("Canny GPU RGB");
	canny(rgb, 60, 180);
	canny_rgb.stopTimedBlock();
	
	Utils::TimedBlock canny_gray("Canny GPU GRAY");
	canny(gray, 60, 180);
	canny_gray.stopTimedBlock();
	
	/*
		show(rgb);
		show(gray);
		 */
	
}