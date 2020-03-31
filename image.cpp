#include "image.h"

void RGBImage::set(HSLImage& hsl)
{
	if (this->height() != hsl.height() || this->width() != hsl.width()) return;
	for (size_t i = 0; i < hsl.width() * hsl.height(); i++)
	{
		this->m_array[i].set(hsl.getHSLPixelArray()[i]);
	}
}

HSLImage RGBImage::toHSL()
{
	HSLImage res(*this);
	return res;
}