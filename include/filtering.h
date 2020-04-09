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
		if(!image.hasCudaSupport())
		{
			SeparableFilter filter(kernel_size);
			float* arr = new float[kernel_size];
			if (!Utils::gaussianSamples(arr, kernel_size, sigma, true))
			{
				delete[] arr;
				return;
			}
			filter.build(arr);
			delete[] arr;
			image.convolve(filter, filter, passes, thread_n);
		}
		else
		{
			Filter filter(kernel_size);
			float* arr = new float[kernel_size* kernel_size];
			if (!Utils::gaussianSamples(arr, kernel_size * kernel_size, sigma))
			{
				delete[] arr;
				return;
			}
			filter.build(arr);
			delete[] arr;
			image.convolve(filter, passes, thread_n);
			
		}
		
		
	}
	
	void boxBlur(RGBImage& image, uint8_t kernel_size = 3, uint8_t passes = 1, uint8_t thread_n = 4)
	{
		if (!(kernel_size & 0x1))return;
		
		if(image.hasCudaSupport())
		{
			
			uint32_t total_size = kernel_size * kernel_size;
			Filter filter(kernel_size, false);
			safe_array<float> vals (total_size);
			
			for (int i = 0; i < total_size; i++)
			{
				vals[i] = 1.0f / total_size;
			}
			
			filter.build(vals.getArray());
			
			image.convolve(filter, passes, thread_n);
			
		}
		else
		{
			
			if (kernel_size == 3)
			{
				// NOTE(Sawii00): it was tested the non-separable convolution is faster with 3x3
				Filter filter(kernel_size, false);
				static float f[9] = { BOX_VAL_3, BOX_VAL_3, BOX_VAL_3, BOX_VAL_3, BOX_VAL_3, BOX_VAL_3, BOX_VAL_3, BOX_VAL_3, BOX_VAL_3 };
				filter.build(f);
				
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
				
				filter.build(vals);
				
				image.convolve(filter, filter, passes, thread_n);
			}
			
		}
	}
	
	
	void threshold(RGBImage& rgb, Pixel& t) {
		
		for (size_t i = 0; i < rgb.width() * rgb.height(); i++)
		{
			if(rgb.getPixelArray()[i] >= t)rgb.getPixelArray()[i].setBinaryOne();
			else rgb.getPixelArray()[i].setBinaryZero();
		}
		
	}
	
	
	void threshold(RGBImage& rgb, Pixel& min, Pixel& max) {
		
		for (size_t i = 0; i < rgb.width() * rgb.height(); i++)
		{
			if(rgb.getPixelArray()[i] >= min && rgb.getPixelArray()[i] <= max )rgb.getPixelArray()[i].setBinaryOne();
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
	
	
	void threshold(HSLImage& hsl, HSL_Pixel& min, HSL_Pixel& max)
	{
		
		for (size_t i = 0; i < hsl.width() * hsl.height(); i++)
		{
			if(hsl.getPixelArray()[i] >= min && hsl.getPixelArray()[i] <= max)hsl.getPixelArray()[i].setBinaryOne();
			else hsl.getPixelArray()[i].setBinaryZero();
		}
		
	}
	
	
}










