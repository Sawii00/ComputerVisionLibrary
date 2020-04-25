#pragma once
#include <cstdint>
#include <immintrin.h>
#include "Utils.h"

struct HSLPixel;


struct RGBPixel
{
	
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
	
	RGBPixel(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
		: r(_r), g(_g), b(_b), a(_a) {}
	
	RGBPixel(uint8_t val = 0)
		:r(val), g(val), b(val), a(0xFF)
	{}
	
	RGBPixel(const HSLPixel& hsl)
	{
		this->set(hsl);
	}
	
	inline RGBPixel& operator+=(const RGBPixel& rhs)
	{
		this->r = Utils::clampPixel((float)this->r + rhs.r);
		this->g = Utils::clampPixel((float)this->g + rhs.g);
		this->b = Utils::clampPixel((float)this->b + rhs.b);
		
		return *this;
	}
	
	inline RGBPixel& operator*=(const float value)
	{
		this->r = Utils::clampPixel((float)r * value);
		this->g = Utils::clampPixel((float)g * value);
		this->b = Utils::clampPixel((float)b * value);
		return *this;
	}
	
	
	
	inline bool operator<(const RGBPixel& rhs)
	{
		return(this->r < rhs.r && this->g < rhs.g && this->b < rhs.b);
	}
	
	
	inline bool operator<=(const RGBPixel& rhs)
	{
		return(this->r <= rhs.r && this->g <= rhs.g && this->b <= rhs.b);
	}
	
	
	inline bool operator>(const RGBPixel& rhs)
	{
		return(this->r > rhs.r && this->g > rhs.g && this->b > rhs.b);
	}
	
	
	inline bool operator>=(const RGBPixel& rhs)
	{
		return(this->r >= rhs.r && this->g >= rhs.g && this->b >= rhs.b);
	}
	
	
	inline bool operator==(const RGBPixel& rhs)
	{
		return(this->r == rhs.r && this->g == rhs.g && this->b == rhs.b);
	}
	
	
	inline bool operator!=(const RGBPixel& rhs)
	{
		return !(*this == rhs);
	}
	
	void set(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	
	void set(uint8_t val = 0)
	{
		r = val;
		g = val;
		b = val;
		a = 0xFF;
	}
	
	void set(const HSLPixel& hsl);
	
	void set(const RGBPixel& pixel)
	{
		r = pixel.r;
		g = pixel.g;
		b = pixel.b;
		a = pixel.a;
	}
	
	void reset()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}
	
};


struct GRAYPixel
{
	public:
	uint8_t val;
	
	GRAYPixel(uint8_t v = 0)
		:val(v) {}
	
	GRAYPixel(const RGBPixel& rgb)
	{
		val = round((float)(0.3f * rgb.r) + (float)(0.59f * rgb.g) + (float)(0.11f * rgb.b));
	}
	
	GRAYPixel(const HSLPixel& hsl)
	{
		this->set(hsl);
	}
	
	void set(const RGBPixel& rgb) {
		val = round((float)(0.3f * rgb.r) + (float)(0.59f * rgb.g) + (float)(0.11f * rgb.b));
	}
	
	void set(const HSLPixel& hsl);
	
	
	inline GRAYPixel& operator+=(const GRAYPixel& rhs)
	{
		this->val = Utils::clampPixel((float)this->val + rhs.val);
		return *this;
	}
	
	inline GRAYPixel& operator*=(const float value)
	{
		this->val = Utils::clampPixel((float)val * value);
		return *this;
	}
	
	inline bool isBinaryZero() const
	{
		return this->val == 0x00;
	}
	
	inline bool isBinaryOne() const
	{
		return this->val == 0xFF;
	}
	
	inline void setBinaryZero()
	{
		this->val = 0x00;
	}
	
	inline void setBinaryOne()
	{
		this->val = 0xFF;
	}
	
	inline bool operator<(const GRAYPixel& rhs)
	{
		return(this->val < rhs.val);
	}
	
	
	inline bool operator<=(const GRAYPixel& rhs)
	{
		return(this->val <= rhs.val);
	}
	
	
	inline bool operator>(const GRAYPixel& rhs)
	{
		return(this->val > rhs.val);
	}
	
	
	inline bool operator>=(const GRAYPixel& rhs)
	{
		return(this->val >= rhs.val);
	}
	
	
	inline bool operator==(const GRAYPixel& rhs)
	{
		return(this->val == rhs.val);
	}
	
	
	inline bool operator!=(const GRAYPixel& rhs)
	{
		return !(*this == rhs);
	}
	
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
	KernelPixel(const RGBPixel& rhs)
	{
		this->r = rhs.r;
		this->g = rhs.g;
		this->b = rhs.b;
	}
	
	RGBPixel getPixel() const
	{
		return RGBPixel(Utils::clampPixel(r), Utils::clampPixel(g), Utils::clampPixel(b), 0xFF);
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




struct HSLPixel
{
	uint32_t h;
	float s;
	float l;
	
	HSLPixel(uint32_t _h = 0, float _s = 0, float _l = 0)
		: h(_h), s(_s), l(_l) {}
	
	HSLPixel(const RGBPixel& rgb_pixel)
	{
		this->set(rgb_pixel);
	}
	
	HSLPixel& operator+=(const HSLPixel& rhs)
	{
		this->h = Utils::maxVal(0, Utils::minVal(360, (int)(this->h + rhs.h)));
		this->l = Utils::maxVal(0.0f, Utils::minVal((float)100, this->l + rhs.l));
		this->s = Utils::maxVal(0.0f, Utils::minVal((float)100, this->s + rhs.s));
		
		return *this;
	}
	
	
	HSLPixel operator+(const HSLPixel& rhs)
	{
		HSLPixel res(*this);
		res += rhs;
		return res;
	}
	
	
	inline HSLPixel& operator-=(const HSLPixel& rhs)
	{
		this->h = Utils::maxVal(0, Utils::minVal(360, (int)(this->h - rhs.h)));
		this->l = Utils::maxVal(0.0f, Utils::minVal((float)100, this->l - rhs.l));
		this->s = Utils::maxVal(0.0f, Utils::minVal((float)100, this->s - rhs.s));
		
		return *this;
	}
	
	
	HSLPixel operator-(const HSLPixel& rhs)
	{
		HSLPixel res(*this);
		res -= rhs;
		return res;
	}
	
	
	HSLPixel& operator*=(const float value)
	{
		this->h = (uint32_t)(this->h * value) % 360;
		this->l = Utils::maxVal(0.0f, Utils::minVal(100.0f, this->l * value));
		this->s = Utils::maxVal(0.0f, Utils::minVal(100.0f, this->s * value));
		
		return *this;
	}
	
	
	HSLPixel operator*(const float value)
	{
		HSLPixel res(*this);
		res *= value;
		return res;
	}
	
	
	inline bool operator<(const HSLPixel& rhs)
	{
		return(this->h < rhs.h && this->s < rhs.s && this->l < rhs.l);
	}
	
	
	inline bool operator<=(const HSLPixel& rhs)
	{
		return(this->h <= rhs.h && this->s <= rhs.s && this->l <= rhs.l);
	}
	
	
	inline bool operator>(const HSLPixel& rhs)
	{
		return(this->h > rhs.h && this->s > rhs.s && this->l > rhs.l);
	}
	
	
	inline bool operator>=(const HSLPixel& rhs)
	{
		return(this->h >= rhs.h && this->s >= rhs.s && this->l >= rhs.l);
	}
	
	
	inline bool operator==(const HSLPixel& rhs)
	{
		return(this->h == rhs.h && this->s == rhs.s && this->l == rhs.l);
	}
	
	
	inline bool operator!=(const HSLPixel& rhs)
	{
		return !(*this == rhs);
	}
	
	
	
	void set(uint32_t _h = 0, float _s = 0, float _l = 0)
	{
		h = _h;
		s = _s;
		l = _l;
	}
	
	
	bool isBinaryOne() const
	{
		return(l == 1.0f);
	}
	
	bool isBinaryZero() const
	{
		return(l == 0.0f);
	}
	
	void setBinaryZero()
	{
		l = 0.0f;
	}
	
	void setBinaryOne()
	{
		l = 1.0f;
	}
	
	void set(const RGBPixel& rgb_pixel) {
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
	
	RGBPixel toRGB()
	{
		return RGBPixel(*this);
	}
	
	
};
