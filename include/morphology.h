#pragma once

#include "image.h"

namespace morph {
	//https://hal-upec-upem.archives-ouvertes.fr/hal-00692897/document
	int erode(RGBImage& image, uint8_t kernel_size)
	{
		//check if it's binary and kernel_size is odd

		uint8_t starting_point = kernel_size / 2;
		bool discard = false;
		for (int i = starting_point; i < image.height() - starting_point; i++)
		{
			for (int j = starting_point; j < image.width() - starting_point; j++)
			{
				discard = false;
				for (int k = -starting_point; k <= starting_point; k++)
				{
					for (int l = -starting_point; l <= starting_point; l++)
					{
						if (image.getPixel(j + l, i + k).isBinaryZero())
						{
							discard = true;
							break;
						}
					}
					if (discard)break;
				}
				if (discard)image.getBuffer()[i * image.width() + j].setBinaryZero();
				else image.getBuffer()[i*image.width() + j].setBinaryOne();
			}
		}
		image.swapBuffer();
		return 1;
	}

	int dilate(RGBImage& image, uint8_t kernel_size)
	{
		//check if it's binary and kernel_size is odd

		uint8_t starting_point = kernel_size / 2;
		bool discard = false;
		for (int i = starting_point; i < image.height() - starting_point; i++)
		{
			for (int j = starting_point; j < image.width() - starting_point; j++)
			{
				discard = false;
				for (int k = -starting_point; k <= starting_point; k++)
				{
					for (int l = -starting_point; l <= starting_point; l++)
					{
						if (image.getPixel(j + l, i + k).isBinaryOne())
						{
							discard = true;
							break;
						}
					}
					if (discard)break;
				}
				if (discard)image.getBuffer()[i * image.width() + j].setBinaryOne();
				else image.getBuffer()[i*image.width() + j].setBinaryZero();
			}
		}
		image.swapBuffer();

		return 1;
	}

	int close(RGBImage& image, uint8_t kernel_size)
	{
		return dilate(image, kernel_size) && erode(image, kernel_size);
	}

	int open(RGBImage& image, uint8_t kernel_size)
	{
		return erode(image, kernel_size) && dilate(image, kernel_size);
	}
}