#include "image.h"

void RGBImage::set(HSLImage& hsl)
{
	if (hsl.height() <= 0 || hsl.width() <= 0) return;
	if (m_array.isNullPointer() || this->w != hsl.width() || this->h != hsl.height())
	{
		this->w = hsl.width();
		this->h = hsl.height();

		if (!m_array.isNullPointer())
			m_array.free();
		m_array.constructArray(this->w * this->h);
		m_name = hsl.getName();
		if (m_buffer)
			delete[] m_buffer;
		m_buffer = new Pixel[this->w * this->h];
	}
	if (this->hasCudaSupport())
	{
		if (!(GPU_utils::gpuHSLtoRGBImage((uint8_t**)hsl.getPointerToArray(), (uint8_t**)m_array.getPointerToArray(), w, h)))
		{
			for (size_t i = 0; i < w * h; i++)
			{
				this->m_array[i].set(hsl.getPixelArray()[i]);
			}
		}
		else
		{
			for (size_t i = 0; i < w * h; i++)
			{
				this->m_array[i].set(hsl.getPixelArray()[i]);
			}
		}


	}
}
	HSLImage RGBImage::toHSL()
	{
		HSLImage res(*this);
		return res;
	}