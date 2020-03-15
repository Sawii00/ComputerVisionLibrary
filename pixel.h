#pragma once
#include <cstdint>

struct Pixel {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	Pixel(uint8_t _r = 0, uint8_t _g = 0, uint8_t _b = 0, uint8_t _a = 0)
		: r(_r), g(_g), b(_b), a(_a) {}

	void set(uint8_t _r = 0, uint8_t _g = 0, uint8_t _b = 0, uint8_t _a = 0) {
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	uint8_t total_val() {
		return ((0.3 * float(r)) + (0.59 * float(g)) + (0.11 * float(b))) * (float(a) / 255.0);
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
		this->r += rhs.r;
		this->g += rhs.g;
		this->b += rhs.b;
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
