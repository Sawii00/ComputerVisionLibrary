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

int dilate(Img& image, uint8_t kernel_size )
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

int close(Img& image, uint8_t kernel_size)
{
	return dilate(image, kernel_size) && erode(image, kernel_size);
}


int open(Img& image, uint8_t kernel_size)
{
	return erode(image, kernel_size) && dilate(image, kernel_size);
}



void threshold(Img& img, std::initializer_list<float> min, std::initializer_list<float> max)
{
	
	switch(img.m_type)
	{
		case ImageType::RGB:
		{
			if(min.size() != 3 || max.size() != 3)
				throw "Invalid Number of parameters";
			
			RGBPixel min_pixel((uint8_t)min.begin()[0], (uint8_t)min.begin()[1], (uint8_t)min.begin()[2], 0xFF);
			RGBPixel max_pixel((uint8_t)max.begin()[0], (uint8_t)max.begin()[1], (uint8_t)max.begin()[2], 0xFF);
			
			img.buildGRAY();
			
			
			RGBPixel* rgb_arr = img.getArray<RGBPixel>();
			GRAYPixel* gray_arr = img.getArray<GRAYPixel>();
			
			
			for (size_t i = 0; i < img.width() * img.height(); i++)
			{
				if(rgb_arr[i] >= min_pixel && rgb_arr[i] <= max_pixel) gray_arr[i].setBinaryOne();
				else gray_arr[i].setBinaryZero();
			}
			
			img.freeRGB();
			img.m_type = ImageType::BINARY;
		}break;
		
		case ImageType::HSL:
		{
			if(min.size() != 3 || max.size() != 3)
				throw "Invalid Number of parameters";
			
			HSLPixel min_pixel((uint16_t)min.begin()[0], (float)min.begin()[1], (float)min.begin()[2]);
			HSLPixel max_pixel((uint16_t)max.begin()[0], (float)max.begin()[1], (float)max.begin()[2]);
			
			img.buildGRAY();
			
			HSLPixel* hsl_arr = img.getArray<HSLPixel>();
			GRAYPixel* gray_arr = img.getArray<GRAYPixel>();
			
			for (size_t i = 0; i < img.width() * img.height(); i++)
			{
				if(hsl_arr[i] >= min_pixel && hsl_arr[i] <= max_pixel) gray_arr[i].setBinaryOne();
				else gray_arr[i].setBinaryZero();
			}
			
			img.freeHSL();
			img.m_type = ImageType::BINARY;
		}break;
		
		case ImageType::BINARY:
		return;
		
		case ImageType::GRAY:
		{
			if(min.size() != 1 || max.size() != 1)
				throw "Invalid Number of parameters";
			
			int8_t min_val = min.begin()[0];
			int8_t max_val = max.begin()[0];
			
			uint8_t remainder = (img.width() * img.height()) % 32;
			uint64_t avx_final_val = (img.width() * img.height()) / 32;
			__m256i thresh_min = _mm256_set1_epi8(min_val);
			__m256i thresh_max = _mm256_set1_epi8(max_val);
			__m256i binary_one = _mm256_set1_epi8(0xFF);
			__m256i binary_zero = _mm256_set1_epi8(0x00);
			uint8_t* array_pointer = *img.getPointerToArray();
			
			
			
			for(int i = 0; i < avx_final_val; i++)
			{
				__m256i row = _mm256_loadu_si256((__m256i*)(array_pointer + 32 * i));
				__m256i mask_min = _mm256_cmpge_epu8(row, thresh_min);
				__m256i mask_max = _mm256_cmple_epu8(row, thresh_max);
				__m256i mask = _mm256_and_si256(mask_min, mask_max);
				__m256i not_mask = _mm256_andnot_si256(mask, binary_one);
				_mm256_maskstore_epi64((int64_t*)(array_pointer + 32 * i), mask, binary_one);
				_mm256_maskstore_epi64((int64_t*)(array_pointer + 32 * i), not_mask, binary_zero);
			}
			
			
			// NOTE(Sawii00): This has not been tested.
			
			GRAYPixel* gray_arr = img.getArray<GRAYPixel>();
			
			for(int i = img.width() * img.height() - 1 ; i >= img.width() * img.height() - remainder; i--)
			{
				if(gray_arr[i] >= min_val && gray_arr[i] <= max_val)
				{
					gray_arr[i].setBinaryOne();
				}
				else
				{
					gray_arr[i].setBinaryZero();
				}
				
			}
			
			
			img.m_type = ImageType::BINARY;
		}return;
		
		default:
		throw "Invalid Color Plane";
	}
	
	
}