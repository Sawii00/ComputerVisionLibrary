#pragma once
#include <cstdint>
#include <immintrin.h>

struct Pixel {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	Pixel(uint8_t _r = 0, uint8_t _g = 0, uint8_t _b = 0, uint8_t _a = 0)
		: r(_r), g(_g), b(_b), a(_a) {}

	inline Pixel& operator+=(const Pixel& rhs) {
		this->r = clampPixel((float)this->r + rhs.r);
		this->g = clampPixel((float)this->g + rhs.g);
		this->b = clampPixel((float)this->b + rhs.b);

		return *this;
	}

	inline Pixel& operator*=(const float value) {
		this->r = clampPixel((float)r * value);
		this->g = clampPixel((float)g * value);
		this->b = clampPixel((float)b * value);
		return *this;
	}

	void set(uint8_t _r = 0, uint8_t _g = 0, uint8_t _b = 0, uint8_t _a = 0) {
		r = _r;
		g = _g;
		b = _b;
		a = _a;
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

	uint8_t total_val() {
		return (uint8_t)((0.114f * float(r)) + (0.299f * float(g)) + (0.587f * float(b))) * (float(a) / 255.0);
	}
	void reset() {
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}
};

struct KernelPixel {
public:
	float r, g, b;

	KernelPixel(float _r = 0, float _g = 0, float _b = 0)
		:r(_r), g(_g), b(_b) {}

	KernelPixel(const KernelPixel& rhs) {
		this->r = rhs.r;
		this->g = rhs.g;
		this->b = rhs.b;
	}
	KernelPixel(const Pixel& rhs) {
		this->r = rhs.r;
		this->g = rhs.g;
		this->b = rhs.b;
	}

	Pixel getPixel() const {
		return Pixel(clampPixel(r), clampPixel(g), clampPixel(b), 0xFF);
	}

	void reset() {
		r = 0;
		g = 0;
		b = 0;
	}

	inline KernelPixel& operator+=(const KernelPixel& rhs) {
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
	inline KernelPixel operator+(const KernelPixel& rhs) {
		KernelPixel res(*this);
		res += rhs;
		return res;
	}
	inline KernelPixel& operator-=(const KernelPixel& rhs) {
		this->r -= rhs.r;
		this->g -= rhs.g;
		this->b -= rhs.b;
		return *this;
	}

	inline KernelPixel operator-(const KernelPixel& rhs) {
		KernelPixel res(*this);
		res -= rhs;
		return res;
	}
	inline KernelPixel& operator*=(const float scalar) {
		this->r *= scalar;
		this->g *= scalar;
		this->b *= scalar;
		return *this;
	}
	inline KernelPixel operator*(const float scalar) {
		KernelPixel res(*this);
		res *= scalar;
		return res;
	}

	inline KernelPixel& operator*=(const KernelPixel& rhs) {
		this->r *= rhs.r;
		this->g *= rhs.g;
		this->b *= rhs.b;
		return *this;
	}
	inline KernelPixel operator*(const KernelPixel& rhs) {
		KernelPixel res(*this);
		res *= rhs;
		return res;
	}

	inline KernelPixel& operator/=(const float scalar) {
		this->r /= scalar;
		this->g /= scalar;
		this->b /= scalar;
		return *this;
	}
	inline KernelPixel operator/(const float scalar) {
		KernelPixel res(*this);
		res /= scalar;
		return res;
	}
};
