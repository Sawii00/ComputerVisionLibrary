#pragma once
#include <cstdint>
#include <immintrin.h>
#include "Utils.h"

struct HSL_Pixel;

struct Pixel
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
	
	Pixel(uint8_t _r = 0, uint8_t _g = 0, uint8_t _b = 0, uint8_t _a = 0)
		: r(_r), g(_g), b(_b), a(_a) {}
	
	Pixel(HSL_Pixel& hsl)
	{
		this->set(hsl);
	}
	
	inline Pixel& operator+=(const Pixel& rhs)
	{
		this->r = Utils::clampPixel((float)this->r + rhs.r);
		this->g = Utils::clampPixel((float)this->g + rhs.g);
		this->b = Utils::clampPixel((float)this->b + rhs.b);
		
		return *this;
	}
	
	inline Pixel& operator*=(const float value)
	{
		this->r = Utils::clampPixel((float)r * value);
		this->g = Utils::clampPixel((float)g * value);
		this->b = Utils::clampPixel((float)b * value);
		return *this;
	}
	
	inline bool operator<(const Pixel& rhs)
	{
		return(this->r < rhs.r && this->g < rhs.g && this->b < rhs.b);
	}
	
	
	inline bool operator<=(const Pixel& rhs)
	{
		return(this->r <= rhs.r && this->g <= rhs.g && this->b <= rhs.b);
	}
	
	
	inline bool operator>(const Pixel& rhs)
	{
		return !(*this <= rhs);
	}
	
	
	inline bool operator>=(const Pixel& rhs)
	{
		return !(*this < rhs);
	}
	
	
	inline bool operator==(const Pixel& rhs)
	{
		return(this->r == rhs.r && this->g == rhs.g && this->b == rhs.b);
	}
	
	
	inline bool operator!=(const Pixel& rhs)
	{
		return !(*this == rhs);
	}
	
	
	void set(HSL_Pixel& hsl);
	
	void set(uint8_t _r = 0, uint8_t _g = 0, uint8_t _b = 0, uint8_t _a = 0)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	
	void set(const Pixel& pixel)
	{
		r = pixel.r;
		g = pixel.g;
		b = pixel.b;
		a = pixel.a;
	}
	
	void setBinaryZero()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0xFF;
	}
	
	void setBinaryOne()
	{
		r = 0xFF;
		g = 0xFF;
		b = 0xFF;
		a = 0xFF;
	}
	
	bool isBinaryOne()
	{
		return(*(uint32_t*)this == 0xFFFFFFFF);
	}
	
	bool isBinaryZero()
	{
		return(*(uint32_t*)this == 0xFF000000);
	}
	
	uint8_t total_val()
	{
		return (uint8_t)((0.114f * float(r)) + (0.299f * float(g)) + (0.587f * float(b))) * (float(a) / 255.0);
	}
	
	void reset()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}
	/*
		HSL_Pixel toHSL()
		{
			return HSL_Pixel(*this);
		}
		*/
};

struct KernelPixel
{
	public:
	float r, g, b;
	
	KernelPixel(float _r = 0, float _g = 0, float _b = 0)
		:r(_r), g(_g), b(_b) {}
	
	KernelPixel(const KernelPixel& rhs)
	{
		this->r = rhs.r;
		this->g = rhs.g;
		this->b = rhs.b;
	}
	KernelPixel(const Pixel& rhs)
	{
		this->r = rhs.r;
		this->g = rhs.g;
		this->b = rhs.b;
	}
	
	Pixel getPixel() const
	{
		return Pixel(Utils::clampPixel(r), Utils::clampPixel(g), Utils::clampPixel(b), 0xFF);
	}
	
	void reset()
	{
		r = 0;
		g = 0;
		b = 0;
	}
	
	inline KernelPixel& operator+=(const KernelPixel& rhs)
	{
#if 1
		
		this->r += rhs.r;
		this->g += rhs.g;
		this->b += rhs.b;
#else
		// NOTE(Sawii): Not worth using for a single pixel (Slower than without)
		__m128i mask = _mm_setr_epi32(-1, -1, -1, 0);
		__m128 p1 = _mm_maskload_ps((float*)this, mask);
		__m128 p2 = _mm_maskload_ps((float*)&rhs, mask);
		
		__m128 res = _mm_add_ps(p1, p2);
		
		_mm_maskstore_ps((float*)this, mask, res);
		
#endif
		return *this;
	}
	inline KernelPixel operator+(const KernelPixel& rhs)
	{
		KernelPixel res(*this);
		res += rhs;
		return res;
	}
	inline KernelPixel& operator-=(const KernelPixel& rhs)
	{
		this->r -= rhs.r;
		this->g -= rhs.g;
		this->b -= rhs.b;
		return *this;
	}
	
	inline KernelPixel operator-(const KernelPixel& rhs)
	{
		KernelPixel res(*this);
		res -= rhs;
		return res;
	}
	inline KernelPixel& operator*=(const float scalar)
	{
		this->r *= scalar;
		this->g *= scalar;
		this->b *= scalar;
		return *this;
	}
	inline KernelPixel operator*(const float scalar)
	{
		KernelPixel res(*this);
		res *= scalar;
		return res;
	}
	
	inline KernelPixel& operator*=(const KernelPixel& rhs)
	{
		this->r *= rhs.r;
		this->g *= rhs.g;
		this->b *= rhs.b;
		return *this;
	}
	inline KernelPixel operator*(const KernelPixel& rhs)
	{
		KernelPixel res(*this);
		res *= rhs;
		return res;
	}
	
	inline KernelPixel& operator/=(const float scalar)
	{
		this->r /= scalar;
		this->g /= scalar;
		this->b /= scalar;
		return *this;
	}
	inline KernelPixel operator/(const float scalar)
	{
		KernelPixel res(*this);
		res /= scalar;
		return res;
	}
};




struct HSL_Pixel
{
	uint32_t h;
	float s;
	float l;
	
	HSL_Pixel(uint32_t _h = 0, float _s = 0, float _l = 0)
		: h(_h), s(_s), l(_l) {}
	
	HSL_Pixel(const Pixel& rgb_pixel)
	{
		this->set(rgb_pixel);
	}
	
	inline HSL_Pixel& operator+=(const HSL_Pixel& rhs)
	{
		this->h = (rhs.h + this->h) % 360;
		this->l = Utils::maxVal(0.0f, Utils::minVal((float)100, this->l + rhs.l));
		this->s = Utils::maxVal(0.0f, Utils::minVal((float)100, this->s + rhs.s));
		
		return *this;
	}
	
	inline HSL_Pixel& operator*=(const float value)
	{
		this->h = (uint32_t)(this->h * value) % 360;
		this->l = Utils::maxVal(0.0f, Utils::minVal(100.0f, this->l * value));
		this->s = Utils::maxVal(0.0f, Utils::minVal(100.0f, this->s * value));
		
		return *this;
	}
	
	
	inline bool operator<(const HSL_Pixel& rhs)
	{
		return(this->h < rhs.h && this->s < rhs.s && this->l < rhs.l);
	}
	
	
	inline bool operator<=(const HSL_Pixel& rhs)
	{
		return(this->h <= rhs.h && this->s <= rhs.s && this->l <= rhs.l);
	}
	
	
	inline bool operator>(const HSL_Pixel& rhs)
	{
		return !(*this <= rhs);
	}
	
	
	inline bool operator>=(const HSL_Pixel& rhs)
	{
		return !(*this < rhs);
	}
	
	
	inline bool operator==(const HSL_Pixel& rhs)
	{
		return(this->h == rhs.h && this->s == rhs.s && this->l == rhs.l);
	}
	
	
	inline bool operator!=(const HSL_Pixel& rhs)
	{
		return !(*this == rhs);
	}
	
	
	
	void set(uint32_t _h = 0, float _s = 0, float _l = 0)
	{
		h = _h;
		s = _s;
		l = _l;
	}
	
	void setBinaryZero()
	{
		h = 0.0f;
		s = 0.0f;
		l = 0.0f;
	}
	
	void setBinaryOne()
	{
		l = 1.0f;
	}
	
	void set(const Pixel& rgb_pixel) {
		float r = ((float)rgb_pixel.r) / 255.0f;
		float g = ((float)rgb_pixel.g) / 255.0f;
		float b = ((float)rgb_pixel.b) / 255.0f;
		float max_val = Utils::maxVal(r, g, b);
		float min_val = Utils::minVal(r, g, b);
		float c = max_val - min_val;
		this->l = (max_val + min_val) / 2;
		if (c == 0) {
			this->h = 0;
			this->s = 0;
		}
		else {
			this->s = c / (1 - abs(2 * this->l - 1));
			if (max_val == r) {
				float segment = (g - b) / c;
				this->h = uint32_t((segment + ((segment >= 0) ? 0 : 6)) * 60.0f);
			}
			else if (max_val == g) {
				this->h = uint32_t((((b - r) / c) + (2)) * 60.0f);
			}
			else {
				this->h = uint32_t((((r - g) / c) + (4)) * 60.0f);
			}
		}
	}
	
	void reset() {
		h = 0;
		s = 0;
		l = 0;
	}
	
	Pixel toRGB()
	{
		return Pixel(*this);
	}
	
	
};
