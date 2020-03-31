#include "Utils.h"

namespace Utils
{
	bool AVX2Available()
	{
		union
		{
			int regs[4];

			struct
			{
				int eax, ebx, ecx, edx;
			};
		};

		__cpuid(regs, 0x7);

		return ebx & 5;
	}

	bool AVX512Available()
	{
		union
		{
			int regs[4];

			struct
			{
				int eax, ebx, ecx, edx;
			};
		};

		__cpuid(regs, 0x7);

		return ebx & 16;
	}

	bool AVXAvailable()
	{
		union
		{
			int regs[4];

			struct
			{
				int eax, ebx, ecx, edx;
			};
		};

		__cpuid(regs, 0x1);

		return ecx & 28;
	}

	bool SSEAvailable()
	{
		union
		{
			int regs[4];

			struct
			{
				int eax, ebx, ecx, edx;
			};
		};

		__cpuid(regs, 0x1);

		return edx & 25;
	}

	uint64_t power(uint64_t n, uint64_t e)
	{
		while (e > 1) {
			n *= n;
			e--;
		}
		return n;
	}

	uint8_t clampPixel(float f)
	{
		if (f < 0)f = 0;
		else if (f > 255) f = 255;

		return (uint8_t)f;
	}

	void REQUIRE(bool condition, const char* message, size_t index) {
		if (!condition) {
			throw message;
		}
	}
	void REQUIRE(bool condition, const char* message) {
		if (!condition) {
			throw message;
		}
	}

	int gaussianSamples(float* buffer, uint8_t n, float sigma)
	{
		if (sigma <= 0 || !(n & 0x1))return 0;
		if (sigma == 1.0f && n <= 7)
		{
			switch (n)
			{
			case 0x1:
			{
				return 0;
			}
			case 0x3:
			{
				buffer[0] = 0.2744f;
				buffer[1] = 0.4464f;
				buffer[2] = 0.2744f;
				break;
			}
			case 0x5:
			{
				buffer[0] = 0.0545f;
				buffer[1] = 0.244f;
				buffer[2] = 0.403f;
				buffer[3] = 0.244f;
				buffer[4] = 0.0545f;
				break;
			}
			case 0x7:
			{
				buffer[0] = 0.004f;
				buffer[1] = 0.0541f;
				buffer[2] = 0.242f;
				buffer[3] = 0.4f;
				buffer[4] = 0.242f;
				buffer[5] = 0.0541f;
				buffer[6] = 0.004f;
				break;
			}
			};
		}
		else
		{
			float prefix = 1 / (std::sqrt(2 * PI*sigma));
			float sum = 0;
			float val;
			int counter = 0;
			for (int i = -n / 2; i <= n / 2 && counter < n; i++)
			{
				val = prefix * std::exp(-(i*i) / (2 * sigma*sigma));
				buffer[counter++] = val;
				sum += val;
			}

			for (int i = 0; i < n; i++)
			{
				buffer[i] /= sum;
			}
		}
		return 1;
	}
}