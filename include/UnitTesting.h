#pragma once
#include "image.h"
#include "filter.h"
#include "filtering.h"

using namespace Utils;

class UnitTesting {
	private:
	void testNormalConvolution1D(RGBImage image)
	{
		Filter box_filter(3, false);
		float val = 1.0f / 9;
		float f[9] = { val,val,val,val,val,val,val,val,val };
		box_filter.build(f, 9);
		
		Utils::TimedBlock t("Convolution1D");
		image.convolve1D(box_filter);
		image.convolve1D(box_filter);
		image.convolve1D(box_filter);
		image.convolve1D(box_filter);
		image.convolve1D(box_filter);
		t.stopTimedBlock();
		image.displayImage();
	}
	
	void testNormalConvolution3D(RGBImage image)
	{
		Filter color_booster(3, true);
		KernelPixel val(0.005f, 0.4f, 0.005f);
		KernelPixel vals[9] = { val,val,val,val,val,val,val,val,val };
		color_booster.build(vals, 9);
		
		Utils::TimedBlock t("Convolution3D");
		image.convolve3D(color_booster);
		t.stopTimedBlock();
		image.displayImage();
	}
	
	void testNormalSeparableConvolution(RGBImage image)
	{
		float f_v[3] = { 1,2,1 };
		float f_h[3] = { -1,0,1 };
		SeparableFilter vertical(f_v, 3);
		SeparableFilter horizontal(f_h, 3);
		
		Utils::TimedBlock t("NormalSeparableConvolution");
		image.separableConvolution1D(vertical, horizontal);
		image.separableConvolution1D(vertical, horizontal);
		image.separableConvolution1D(vertical, horizontal);
		image.separableConvolution1D(vertical, horizontal);
		image.separableConvolution1D(vertical, horizontal);
		t.stopTimedBlock();
		
		image.displayImage();
	}
	
	void testMultiThreadedConvolutionSeparable(RGBImage image)
	{
		float f_v[3] = { 1,2,1 };
		float f_h[3] = { -1,0,1 };
		SeparableFilter vertical(f_v, 3);
		SeparableFilter horizontal(f_h, 3);
		
		//TimedBlock t("MultiThreadedConvolutionSeparable");
		//image.multiThread_1D_Separable_Convolution(vertical, horizontal, 9);
		//image.multiThread_1D_Separable_Convolution(vertical, horizontal, 9);
		//image.multiThread_1D_Separable_Convolution(vertical, horizontal, 9);
		//image.multiThread_1D_Separable_Convolution(vertical, horizontal, 9);
		image.multiThread_1D_Separable_Convolution(vertical, horizontal, 9);
		//t.stopTimedBlock();
		
		image.displayImage();
	}
	
	void testMultiThreaded1DConvolution(RGBImage image)
	{
		Filter edge_detection(3, false);
		float f[9] = { 1, 2, 1, 0, 0, 0,-1, -2, -1 };
		edge_detection.build(f, 9);
		
		Utils::TimedBlock t("MultiThreaded1DNormalConvolution");
		//image.multiThread_1D_Convolution(edge_detection, 9);
		//image.multiThread_1D_Convolution(edge_detection, 9);
		//image.multiThread_1D_Convolution(edge_detection, 9);
		//image.multiThread_1D_Convolution(edge_detection, 9);
		image.multiThread_1D_Convolution(edge_detection, 9);
		t.stopTimedBlock();
		image.displayImage();
	}
	void testMultiThreaded3DConvolution(RGBImage image)
	{
		Filter color_booster(3, true);
		KernelPixel val(0.005f, 0.4f, 0.005f);
		KernelPixel vals[9] = { val,val,val,val,val,val,val,val,val };
		color_booster.build(vals, 9);
		
		Utils::TimedBlock t("MultiThreaded3DNormalConvolution");
		image.multiThread_3D_Convolution(color_booster, 9);
		t.stopTimedBlock();
		image.displayImage();
	}
	
	void testGPUConvolution(RGBImage image)
	{
		Filter box_filter(3, false);
		float val = 1.0f / 9;
		float f[9] = { val,val,val,val,val,val,val,val,val };
		box_filter.build(f, 9);
		
		Utils::TimedBlock t("GPUConvolution");
		image.CUDA_Accelerated_1D_Convolution(box_filter);
		t.stopTimedBlock();
		image.displayImage();
	}
	
	public:
	void runTests(RGBImage& img)
	{
		//testNormalConvolution1D(img);
		//testNormalConvolution3D(img);
		//testNormalSeparableConvolution(img);
		//testMultiThreaded1DConvolution(img);
		//testMultiThreaded3DConvolution(img);
		//testMultiThreadedConvolutionSeparable(img);
		//testGPUConvolution(img);
		//testGPUConvolution(img);
		
		//RGBImage im2 = img;
		/*
				RGBImage im3 = img;
				RGBImage im4 = img;
		
				Utils::TimedBlock gauss("GaussianBlur");
				Filters::gaussianBlur(img, 5, 5, 1.0f, 9);
				gauss.stopTimedBlock();
				img.displayImage();
		
				Utils::TimedBlock box3("BoxBlur3");
				Filters::boxBlur(im2, 3, 5, 9);
				box3.stopTimedBlock();
				im2.displayImage();
				Utils::TimedBlock box5("BoxBlur7");
				Filters::boxBlur(im3, 7, 5, 9);
				box5.stopTimedBlock();
				im3.displayImage();
				Utils::TimedBlock box9("BoxBlur9");
				Filters::boxBlur(im4, 9, 5, 9);
				box9.stopTimedBlock();
				im4.displayImage();
				 */
		
		HSLImage hsl(img);
		HSL_Pixel hsl_pixel(50,0.4,0.1);
		Pixel rgb_pixel(0,0,0);
		TimedBlock th1("ThresholdRGB");
		Filters::threshold(img, rgb_pixel);
		th1.stopTimedBlock();
		TimedBlock th2("ThresholdHSL");
		Filters::threshold(hsl, hsl_pixel);
		th2.stopTimedBlock();
		
		img.displayImage();
		hsl.displayImage();
		
		
		
	}
};