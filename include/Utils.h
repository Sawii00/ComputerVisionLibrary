#pragma once
#include <cstdint>
#include <chrono>
#include <iostream>
#include <ctime>
#include <cmath>

#ifdef DEBUG_MODE
#include <string>
#include <intrin.h>
#endif

#define PI 3.14159f
namespace Utils
{
	template <class T>
		inline T minVal(T a, T b) { return((a < b) ? a : b); }
	template <class T>
		inline T maxVal(T a, T b) { return((a > b) ? a : b); }
	template <class T>
		inline T minVal(T a, T b, T c) { return minVal(a, minVal(b, c)); }
	template <class T>
		inline T maxVal(T a, T b, T c) { return maxVal(a, maxVal(b, c)); }
	
	template <class T>
		T mirrorGet(T* m_arr, int x, int y, int w, int h)
	{
		return m_arr[minVal(abs(x), 2 * (w - 1) - x) + w * minVal(abs(y), 2 * (h - 1) - y)];
	}
	
	bool AVX2Available();
	bool AVX512Available();
	bool AVXAvailable();
	bool SSEAvailable();
	uint64_t power(uint64_t n, uint64_t e);
	uint8_t clampPixel(float f);
	
	float clampTo1(float f);
	
	
	inline float radToDeg(float rads)
	{
		return rads * 180 / PI;
	}
	
	int gaussianSamples(float* buffer, uint8_t n, float sigma, bool separable = false);
	void REQUIRE(bool condition, const char* message, size_t index);
	void REQUIRE(bool condition, const char* message);
	
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
}