#pragma once
#include "safe_array.h"
#include "Utils.h"
#include "pixel.h"
#include "math.h"

class SeparableFilter
{
public:

	SeparableFilter(float* arr, size_t size)
		:m_size(size)
	{
		m_arr.constructArray(arr, size);
	}

	SeparableFilter(size_t size)
		:m_size(size)
	{
		m_arr.constructArray(size);
	}

	int constructFilter(float* arr, size_t size)
	{
		if (size != this->m_size || this->m_arr.alreadyInitialized())return 0;
		m_arr.setArray(arr, size);
	}

	float get(size_t index) {
		return m_arr[index];
	}

	KernelPixel getKP(size_t index) {
		return KernelPixel(m_arr[3 * index], m_arr[3 * index + 1], m_arr[3 * index + 2]);
	}

	size_t getSize() const {
		return m_size;
	}

private:
	size_t m_size;
	safe_array<float> m_arr;
};

class Filter
{
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

	int build(float* values, size_t size) {
		if (m_three_channels) return 0; //Tried to use single values for 3-ch kernel
		m_arr.setArray(values, size);

		return 1;
	}

	int build(KernelPixel* values, size_t size) {
		if (!(m_three_channels)) return 0; //Tried to use three_channels for 1-ch kernel
		m_arr.setArray((float*)values, size * 3);

		return 1;
	}

	float get(size_t index) {
		return m_arr[index];
	}

	KernelPixel getKP(size_t index) {
		return KernelPixel(m_arr[3 * index], m_arr[3 * index + 1], m_arr[3 * index + 2]);
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

	float* getFloatArray() const {
		return m_arr.getArray();
	}

	const bool is_three_channel() const {
		return m_three_channels;
	}
};