#pragma once
#include "pixel.h"
#include "hsl_image.h"
#include <cmath>

namespace ColorConversion
{
	
	void HSLtoRGB(Image& rgb_img, const HSL_Image& hsl_img) {
		
	}
	
	HSL_Image RGBtoHSL(const Image& rgb_img) {
		HSL_Image res(rgb_img.width(), rgb_img.height());
		for (size_t i = 0; i < rgb_img.width() * rgb_img.height(); i++)
		{
			setHSLfromRGB(res.getHSLPixelArray()[i], rgb_img.getPixelArray()[i]);
		}
		return res;
	}
	
	void setHSLImagefromRGBImage(HSL_Image& hsl_img, const Image& rgb_img) {
		if (rgb_img.height() != hsl_img.height() || rgb_img.width() != hsl_img.width()) return;
		for (size_t i = 0; i < hsl_img.width() * hsl_img.height(); i++)
		{
			setHSLfromRGB(hsl_img.getHSLPixelArray()[i], rgb_img.getPixelArray()[i]);
		}
	}
	
};