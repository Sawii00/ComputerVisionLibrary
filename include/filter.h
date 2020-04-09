#pragma once
#include "safe_array.h"
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
	
	int build(float* arr)
	{
		if (this->m_arr.alreadyInitialized())return 0;
		m_arr.setArray(arr, m_size);
	}
	
	int build(safe_array<float> &arr)
	{
		if (arr.getSize() != this->m_size || this->m_arr.alreadyInitialized())return 0;
		m_arr.setArray(arr);
	}
	
	float get(size_t index) {
		return m_arr[index];
	}
	
	KernelPixel getKP(size_t index) {
		return KernelPixel(m_arr[3 * index], m_arr[3 * index + 1], m_arr[3 * index + 2]);
	}
	
	size_t getSideSize() const {
		return m_size;
	}
	
	private:
	size_t m_size;
	safe_array<float> m_arr;
};

class Filter
{
	private:
	size_t m_side;
	safe_array<float> m_arr;
	bool m_three_channels = false;
	
	public:
	Filter(size_t side_size, bool three_channels = false)
		:m_side(side_size), m_three_channels(three_channels)
	{
		size_t byte_count = m_side * m_side;
		byte_count = three_channels ? byte_count * 3 : byte_count; //if three channels we need to triplicate the array
		m_arr.constructArray(byte_count);
	}
	
	Filter(const Filter& filter) {
		m_side = filter.getSideSize();
		m_three_channels = filter.is_three_channel();
		m_arr.constructArray(filter.getFloatArray(), filter.getByteCount() / sizeof(float));
	}
	
	
	int build(float* arr)
	{
		if (this->m_arr.alreadyInitialized())return 0;
		m_arr.setArray(arr, m_side * m_side);
	}
	
	
	int build(KernelPixel* arr)
	{
		if (this->m_arr.alreadyInitialized())return 0;
		m_arr.setArray((float*)arr, m_side * m_side * 3);
	}
	
	int build(safe_array<float>& values) {
		if (m_three_channels  || this->m_arr.alreadyInitialized()) return 0; //Tried to use single values for 3-ch kernel
		m_arr.setArray(values);
		
		return 1;
	}
	
	int build(safe_array<KernelPixel>& values) {
		if (!(m_three_channels) || this->m_arr.alreadyInitialized()) return 0; //Tried to use three_channels for 1-ch kernel
		m_arr.setArray((float*)values.getArray(), values.getSize() * 3);
		
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
	
	size_t getSideSize() const
	{
		return m_side;
	}
	
	float* getFloatArray() const {
		return m_arr.getArray();
	}
	
	const bool is_three_channel() const {
		return m_three_channels;
	}
	
	Filter& operator*= (const float value) {
		for (size_t i = 0; i < m_side*m_side; i++)
		{
			m_arr[i] *= value;
		}
		
		return *this;
	}
	
	Filter operator* (const float value) {
		Filter res(*this);
		res *= value;
		return res;
	}
};