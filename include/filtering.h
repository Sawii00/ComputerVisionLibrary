#pragma once

#include "image.h"
#include "filter.h"
#include "Utils.h"

#define BOX_VAL_3 1.0f/9

namespace Filters
{
	void gaussianBlur(RGBImage& image, uint8_t kernel_size, uint8_t passes = 1, float sigma = 1.0f, uint8_t thread_n = 4)
	{
		if (!(kernel_size & 0x1))return;
		
		SeparableFilter filter(kernel_size);
		float* arr = new float[kernel_size];
		if (!Utils::gaussianSamples(arr, kernel_size, sigma))
		{
			delete[] arr;
			return;
		}
		filter.build(arr, kernel_size);
		delete[] arr;
		// TODO(Sawii00): possible improvement (implement multiple passes directly into the function (avoids multiple overhead from calls)
		image.convolve(filter, filter, passes, thread_n);
	}
	
	void boxBlur(RGBImage& image, uint8_t kernel_size = 3, uint8_t passes = 1, uint8_t thread_n = 4)
	{
		if (!(kernel_size & 0x1))return;
		
		if (kernel_size == 3)
		{
			//it was tested the non-separable convolution is faster with 3x3
			Filter filter(kernel_size, false);
			static float f[9] = { BOX_VAL_3, BOX_VAL_3, BOX_VAL_3, BOX_VAL_3, BOX_VAL_3, BOX_VAL_3, BOX_VAL_3, BOX_VAL_3, BOX_VAL_3 };
			filter.build(f, 9);
			
			image.convolve(filter, passes, thread_n);
		}
		else
		{
			
			SeparableFilter filter(kernel_size);
			safe_array<float> vals(kernel_size);
			
			for (int i = 0; i < kernel_size; i++)
			{
				vals[i] = 1.0f / kernel_size;
			}
			
			filter.build(vals.getArray(), kernel_size);
			
			image.convolve(filter, filter, passes, thread_n);
		}
		
	}
	
	
	void threshold(RGBImage& rgb, Pixel& t) {
		
		for (size_t i = 0; i < rgb.width() * rgb.height(); i++)
		{
			if(rgb.getPixelArray()[i] >= t)rgb.getPixelArray()[i].setBinaryOne();
			else rgb.getPixelArray()[i].setBinaryZero();
		}
		
	}
	
	
	void threshold(HSLImage& hsl, HSL_Pixel& t)
	{
		
		for (size_t i = 0; i < hsl.width() * hsl.height(); i++)
		{
			if(hsl.getPixelArray()[i] >= t)hsl.getPixelArray()[i].setBinaryOne();
			else hsl.getPixelArray()[i].setBinaryZero();
		}
		
	}
	
	
	// TODO(Sawii00): Write this
	void medianBlur(RGBImage& image, uint8_t kernel_size = 3)
	{
		if (!(kernel_size & 0x1))return;
	}
	
	
	
}










