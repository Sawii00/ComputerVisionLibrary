#include "pixel.h"
#include <cmath>

void RGBPixel::set(const HSLPixel& hsl)
{
	float c = (1 - abs(2 * hsl.l - 1)) * hsl.s;
	float x = c * (1 - abs(fmodf((float(hsl.h) / 60.0), 2.0f) - 1));
	float m_ = hsl.l - c / 2;
	
	Utils::clampTo1(x);
	Utils::clampTo1(m_);
	
	uint8_t params[6][4] = {
		{(c + m_) * 255.0f, (x + m_) * 255.0f, m_ * 255.0f, 255},
		{(x + m_) * 255.0f, (c + m_) * 255.0f, m_ * 255.0f, 255},
		{m_ * 255.0f, (c + m_) * 255.0f, (x + m_) * 255.0f, 255},
		{m_ * 255.0f, (x + m_) * 255.0f, (c + m_) * 255.0f, 255},
		{(x + m_) * 255.0f, m_ * 255.0f, (c + m_) * 255.0f, 255},
		{(c + m_) * 255.0f, m_ * 255.0f, (x + m_) * 255.0f, 255}
	};
	r = params[int(hsl.h / 60)][0];
	g = params[int(hsl.h / 60)][1];
	b = params[int(hsl.h / 60)][2];
	a = params[int(hsl.h / 60)][3];
}

void GRAYPixel::set(const HSLPixel& hsl) {
	this->val = round(hsl.l * 255.0f);
}