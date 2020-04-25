#include "morphology.h"


int erode(Img& image, uint8_t kernel_size)
{
	if(!image.isBINARY())return 0;
	
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
					if (image.getPixel<GRAYPixel>(j + l, i + k).isBinaryZero())
					{
						discard = true;
						break;
					}
				}
				if (discard)break;
			}
			if (discard)image.getBuffer<GRAYPixel>()[i * image.width() + j].setBinaryZero();
			else image.getBuffer<GRAYPixel>()[i*image.width() + j].setBinaryOne();
		}
	}
	image.swapBuffer();
	return 1;
}

int dilate(Img& image, uint8_t kernel_size = 3)
{
	if(!image.isBINARY())return 0;
	
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
					if (image.getPixel<GRAYPixel>(j + l, i + k).isBinaryOne())
					{
						discard = true;
						break;
					}
				}
				if (discard)break;
			}
			if (discard)image.getBuffer<GRAYPixel>()[i * image.width() + j].setBinaryOne();
			else image.getBuffer<GRAYPixel>()[i*image.width() + j].setBinaryZero();
		}
	}
	
	image.swapBuffer();
	return 1;
}

int close(Img& image, uint8_t kernel_size = 3)
{
	return dilate(image, kernel_size) && erode(image, kernel_size);
}


int open(Img& image, uint8_t kernel_size = 3)
{
	return erode(image, kernel_size) && dilate(image, kernel_size);
}



void threshold(Img& img, std::initializer_list<float> min, std::initializer_list<float> max)
{
	switch(m_type)
	{
		case ImageType::RGB:
		{
			if(min.size() != 3 || max.size() != 3)
				throw "Invalid Number of parameters";
			
			
			
			
		}break;
		
		case ImageType::HSL:
		{
			
		}break;
		
		case ImageType::BINARY:
		return;
		
		case ImageType::GRAY:
		{
			
		}break;
		
	}
	
	
}