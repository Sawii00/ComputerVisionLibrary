#pragma once
#include "safe_array.h"

class Pixel;

class KernelPixel {
public:
	float r, g, b, a;

	KernelPixel(float _r = 0, float _g = 0, float _b = 0, float _a = 0)
		:r(_r), g(_g), b(_b), a(_a) {}

	KernelPixel(const KernelPixel& rhs) {
		this->r = rhs.r;
		this->g = rhs.g;
		this->b = rhs.b;
		this->a = rhs.a;
	}

	void reset() {
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}

	inline KernelPixel operator+(const KernelPixel& rhs) {
		KernelPixel res(*this);
		res += rhs;
		return res;
	}
	inline KernelPixel& operator+=(const KernelPixel& rhs) {
		this->r += rhs.r;
		this->g += rhs.g;
		this->b += rhs.b;
		this->a += rhs.a;
		return *this;
	}
	inline KernelPixel operator-(const KernelPixel& rhs) {
		KernelPixel res(*this);
		res -= rhs;
		return res;
	}
	inline KernelPixel& operator-=(const KernelPixel& rhs) {
		this->r -= rhs.r;
		this->g -= rhs.g;
		this->b -= rhs.b;
		this->a -= rhs.a;
		return *this;
	}

	inline KernelPixel operator*(const float scalar) {
		KernelPixel res(*this);
		res *= scalar;
		return res;
	}

	inline KernelPixel& operator*=(const float scalar) {
		this->r *= scalar;
		this->g *= scalar;
		this->b *= scalar;
		this->a *= scalar;
		return *this;
	}
	inline KernelPixel operator/(const float scalar) {
		KernelPixel res(*this);
		res /= scalar;
		return res;
	}

	inline KernelPixel& operator/=(const float scalar) {
		this->r /= scalar;
		this->g /= scalar;
		this->b /= scalar;
		this->a /= scalar;
		return *this;
	}
};

class Filter {
private:
	size_t m_width, m_height;
	safe_array<float> m_arr;
	bool m_three_channels = false;

public:
	Filter(size_t w, size_t h, bool three_channels = false)
		:m_width(w), m_height(h), m_three_channels(three_channels)
	{
		size_t size = m_height * m_width;
		size = three_channels ? size * 3 : size; //if three channels we need to triplicate the array
		m_arr.constructArray(size);
	}

	float get(size_t index) {
		return m_arr[index];
	}

	size_t getByteCount() const {
		return m_arr.size;
	}

	size_t height() const {
		return m_height;
	}
	size_t width() const {
		return m_width;
	}
};