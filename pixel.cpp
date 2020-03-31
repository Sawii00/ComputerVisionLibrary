#include "pixel.h"
#include <cmath>

void Pixel::set(HSL_Pixel& hsl)
{
	float c = (1 - abs(2 * hsl.l - 1)) * hsl.s;
	float x = c * (1 - abs(int((float(hsl.h) / 60.0)) % 2 - 1));
	float m_ = hsl.l - c / 2;
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