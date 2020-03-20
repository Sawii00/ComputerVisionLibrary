#pragma once
#include <cstdint>
#include <chrono>
#include <iostream>
#include <ctime>
#include <cmath>

#define min_(a,b) (((a) < (b)) ? (a) : (b))

#ifdef DEBUG_MODE
#include <string>
#include <intrin.h>
#endif

uint64_t power(uint64_t n, uint64_t e)
{
	while (e > 1) {
		n *= n;
		e--;
	}
	return n;
}

template <class T>
T mirrorGet(T* m_arr, int x, int y, int w, int h)
{
	return m_arr[min_(abs(x), 2 * (w - 1) - x) + w * min_(abs(y), 2 * (h - 1) - y)];
}

uint8_t clampPixel(float f)
{
	if (f < 0)f = 0;
	else if (f > 255) f = 255;
    
	return (uint8_t)f;
}
#ifdef DEBUG_MODE

class TimedBlock
{
    private:
    
	std::string name;
	uint64_t cycle_count;
	std::chrono::time_point<std::chrono::system_clock> m_StartTime;
	std::chrono::time_point<std::chrono::system_clock> m_EndTime;
	uint64_t time;
    
	void displayTimedBlock()
	{
		if (this->time > 100000)
		{
			std::string out = "Name: " + this->name + ", CycleCount: " + std::to_string(this->cycle_count) + " ExecutionTime: " + std::to_string(this->time / 1000) + "ms";
			std::cout << out << std::endl;
		}
		else
		{
			std::string out = "Name: " + this->name + ", CycleCount: " + std::to_string(this->cycle_count) + " ExecutionTime: " + std::to_string(this->time) + "us";
			std::cout << out << std::endl;
		}
	}
    
    public:
    
	TimedBlock(std::string&& name)
	{
		this->name = name;
		this->time = 0;
		this->m_StartTime = std::chrono::system_clock::now();
		this->cycle_count = __rdtsc();
	}
    
	inline void stopTimedBlock()
	{
		this->cycle_count = __rdtsc() - this->cycle_count;
		this->m_EndTime = std::chrono::system_clock::now();
		this->time = std::chrono::duration_cast<std::chrono::microseconds>(m_EndTime - m_StartTime).count();
		displayTimedBlock();
	}
};

#endif
