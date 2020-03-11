#pragma once
#include "safe_array.h"

class Filter {
private:
	size_t m_width, m_height;
	safe_array<uint8_t> m_arr;
	bool m_three_channels = false;

public:
	Filter(size_t w, size_t h, bool three_channels = false)
		:m_width(w), m_height(h), m_three_channels(three_channels)
	{
		size_t size = m_height * m_width;
		size = three_channels ? size * 3 : size; //if three channels we need to triplicate the array
		m_arr.constructArray(size);
	}
};