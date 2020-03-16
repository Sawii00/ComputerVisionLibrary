#pragma once
#include <cstdint>
#include <chrono>

#include <iostream>
#include <ctime>
#include <cmath>

#define min_(a,b) (((a) < (b)) ? (a) : (b))

uint64_t power(uint64_t n, uint64_t e) {
	while (e > 1) {
		n *= n;
		e--;
	}
	return n;
}
template <class T>
T mirrorGet(T* m_arr, int x, int y, int w, int h) {
    return m_arr[min_(abs(x), 2 * (w - 1) - x) + w * min_(abs(y), 2 * (h - 1) - y)];
}

uint8_t clampPixel(float f) {
	if (f < 0)f = 0;
	else if (f > 255) f = 255;
    
	return (uint8_t)f;
}

class Timer
{
    public:
	void start()
	{
		m_StartTime = std::chrono::system_clock::now();
		m_bRunning = true;
	}
    
	void stop()
	{
		m_EndTime = std::chrono::system_clock::now();
		m_bRunning = false;
	}
    
	double elapsedMicroseconds()
	{
		std::chrono::time_point<std::chrono::system_clock> endTime;
        
		if (m_bRunning)
		{
			endTime = std::chrono::system_clock::now();
		}
		else
		{
			endTime = m_EndTime;
		}
        
		return std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_StartTime).count();
	}
    
	double elapsedMilliseconds()
	{
		return elapsedMicroseconds() / 1000.0;
	}
    
	double elapsedSeconds()
	{
		return elapsedMicroseconds() / 1000000.0;
	}
    
    private:
	std::chrono::time_point<std::chrono::system_clock> m_StartTime;
	std::chrono::time_point<std::chrono::system_clock> m_EndTime;
	bool                                               m_bRunning = false;
};