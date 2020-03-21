#pragma once
#include <cstdint>
#include <cmath>
#define PI 3.14159

//@TODO(Sawii00): check if it improves performance to only calculate half of the values (its symmetrical)
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