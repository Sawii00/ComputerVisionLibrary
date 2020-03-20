#pragma once
#include "image.h"
#include "filter.h"

void testNormalConvolution1D(Image image)
{
	Filter box_filter(3,3,false);
	float val = 1.0f/9;
	float f[9] = {val,val,val,val,val,val,val,val,val};
	box_filter.build(f, 9);
	
	TimedBlock t("Convolution1D");
	image.convolve1D(box_filter);
	t.stopTimedBlock();
	image.displayImage();
	
}

void testNormalConvolution3D(Image image)
{
	Filter color_booster(3,3,true);
	KernelPixel val(0.005f, 0.4f, 0.005f);
	KernelPixel vals[9] = {val,val,val,val,val,val,val,val,val};
	color_booster.build(vals, 9);
	
	TimedBlock t("Convolution3D");
	image.convolve3D(color_booster);
	t.stopTimedBlock();
	image.displayImage();
	
}

void testNormalSeparableConvolution(Image image)
{
	
	float f_v[3] = { 1,2,1 };
	float f_h[3] = { -1,0,1 };
	SeparableFilter vertical(f_v, 3);
	SeparableFilter horizontal(f_h, 3);
	
	
	TimedBlock t("NormalSeparableConvolution");
	image.separableConvolution1D(vertical, horizontal);
	t.stopTimedBlock();
	
	image.displayImage();
	
	
}

void testMultiThreadedConvolution(Image image)
{
	
	float f_v[3] = { 1,2,1 };
	float f_h[3] = { -1,0,1 };
	SeparableFilter vertical(f_v, 3);
	SeparableFilter horizontal(f_h, 3);
	
	
	TimedBlock t("NormalSeparableConvolution");
	image.convolveThreading(vertical, horizontal);
	t.stopTimedBlock();
	
	image.displayImage();
	
	
}

void testGaussianBlur(Image image)
{
	
}