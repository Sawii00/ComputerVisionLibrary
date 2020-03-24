#pragma once
#include "image.h"
#include "filter.h"

class UnitTesting {
	private:
	void testNormalConvolution1D(Image image)
	{
		Filter box_filter(3, 3, false);
		float val = 1.0f / 9;
		float f[9] = { val,val,val,val,val,val,val,val,val };
		box_filter.build(f, 9);
		
		TimedBlock t("Convolution1D");
		image.convolve1D(box_filter);
		image.convolve1D(box_filter);
		image.convolve1D(box_filter);
		image.convolve1D(box_filter);
		image.convolve1D(box_filter);
		t.stopTimedBlock();
		image.displayImage();
	}
	
	void testNormalConvolution3D(Image image)
	{
		Filter color_booster(3, 3, true);
		KernelPixel val(0.005f, 0.4f, 0.005f);
		KernelPixel vals[9] = { val,val,val,val,val,val,val,val,val };
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
		image.separableConvolution1D(vertical, horizontal);
		image.separableConvolution1D(vertical, horizontal);
		image.separableConvolution1D(vertical, horizontal);
		image.separableConvolution1D(vertical, horizontal);
		t.stopTimedBlock();
		
		image.displayImage();
	}
	
	void testMultiThreadedConvolutionSeparable(Image image)
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
	
	void testMultiThreaded1DConvolution(Image image)
	{
		Filter edge_detection(3, 3, false);
		float f[9] = {1, 2, 1, 0, 0, 0,-1, -2, -1 };
		edge_detection.build(f, 9);
		
		TimedBlock t("MultiThreaded1DNormalConvolution");
		//image.multiThread_1D_Convolution(edge_detection, 9);
		//image.multiThread_1D_Convolution(edge_detection, 9);
		//image.multiThread_1D_Convolution(edge_detection, 9);
		//image.multiThread_1D_Convolution(edge_detection, 9);
		image.multiThread_1D_Convolution(edge_detection, 9);
		t.stopTimedBlock();
		image.displayImage();
	}
	void testMultiThreaded3DConvolution(Image image)
	{
		Filter color_booster(3, 3, true);
		KernelPixel val(0.005f, 0.4f, 0.005f);
		KernelPixel vals[9] = { val,val,val,val,val,val,val,val,val };
		color_booster.build(vals, 9);
		
		TimedBlock t("MultiThreaded3DNormalConvolution");
		image.multiThread_3D_Convolution(color_booster, 9);
		t.stopTimedBlock();
		image.displayImage();
	}
	
	void testGPUConvolution(Image image)
	{
		Filter box_filter(3, 3, false);
		float val = 1.0f / 9;
		float f[9] = { val,val,val,val,val,val,val,val,val };
		box_filter.build(f, 9);
		
		TimedBlock t("GPUConvolution");
		image.CUDA_Accelerated_1D_Convolution(box_filter);
		t.stopTimedBlock();
		image.displayImage();
	}
	
	public:
	void runTests(Image& img)
	{
		//testNormalConvolution1D(img);
		//testNormalConvolution3D(img);
		//testNormalSeparableConvolution(img);
		testMultiThreaded1DConvolution(img);
		//testMultiThreaded1DConvolution(img);
		//testMultiThreaded1DConvolution(img);
		//testMultiThreaded3DConvolution(img);
		//testMultiThreadedConvolutionSeparable(img);
		//testMultiThreadedConvolutionSeparable(img);
		testMultiThreadedConvolutionSeparable(img);
		//testGPUConvolution(img);
		//testGPUConvolution(img);
		//testGPUConvolution(img);
		//testGPUConvolution(img);
		//testGPUConvolution(img);
		img.gaussianBlur(5, 5, 1.0, 9);
		img.displayImage();
		
	}
};