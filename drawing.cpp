#include "drawing.h"

void drawRect(Img& img, size_t x, size_t y, size_t width, size_t height, Color color, DrawingPositionMode mode)
{
	
	// NOTE(Sawii00): does not need to check if it exceeds the height and width because it skips the loop and does not draw anything if that happens
	
	char* arr = nullptr;
	uint8_t pixel_type_size = 0;
	char* color_ptr = nullptr;
	
	if(img.getType() == ImageType::RGB)
	{
		arr = (char*)img.m_rgb_layer.getArray();
		pixel_type_size = 4;
		color_ptr = (char*)(rgb_colors + color);
	}
	else if(img.getType() == ImageType::HSL)
	{
		arr = (char*)img.m_hsl_layer.getArray();
		pixel_type_size = 12;
		color_ptr = (char*)(hsl_colors + color);
	}
	else if(img.getType() == ImageType::GRAY || img.getType() == ImageType::BINARY)
	{
		arr = (char*)img.m_gray_layer.getArray();
		pixel_type_size = 1;
		color_ptr = (char*)(gray_colors + color);
	}
	else
	{
		return;
	}
	
	size_t initial_y;
	size_t initial_x;
	
	size_t final_y;
	size_t final_x;
	
	if(mode == DrawingPositionMode::UPPER_CORNER)
	{
		initial_y = y;
		initial_x = x;
		final_y = y + height;
		final_x = x + width;
	}
	else if(mode == DrawingPositionMode::CENTER)
	{
		size_t stride_x = width / 2;
		size_t stride_y = height / 2;
		
		initial_x = (int)(x - stride_x) < 0 ? 0 : x - stride_x;
		initial_y = (int)(y - stride_y) < 0 ? 0 : y - stride_y;
		final_x = x + stride_x;
		final_y = y + stride_y;
	}
	else
	{
		return;
	}
	
	Utils::TimedBlock setup("SetupCode");
	
	uint64_t costant = img.width() * pixel_type_size;
	
	for(size_t y_ = initial_y ; y_ < final_y && y_ < img.height() ; y_++)
	{
		for(size_t x_ = initial_x ; x_ < final_x && x_ < img.width() ; x_++)
		{
			memcpy(arr + y_ * costant + x_ * pixel_type_size, color_ptr, pixel_type_size);
			
		}
	}
	setup.stopTimedBlock();
	
	
}

/*
void drawRotatedRect(Img &img, size_t x, size_t y, size_t width, size_t height, float angle, Color color, DrawingPositionMode mode)
{
	
	float transformation_matrix[4] =
	{
		cos(angle), -sin(angle), sin(angle), cos(angle)
	};
	
	if(mode == DrawingPositionMode::DEFAULT)
	{
		// NOTE(Sawii00): does not need to check if it exceeds the height and width because it skips the loop and does not draw anything if that happens
		
		float final_x;
		float final_y;
		
		// TODO(Sawii00): Vectorize this
		for(int y_ = 0 ; y_ <  height && y_ < img.height() ; y_++)
		{
			for(int x_ = 0 ; x_ < width && x_ < img.width() ; x_++)
			{
				final_x = x_ * transformation_matrix[0] + y_ * transformation_matrix[1];
				final_y = x_ * transformation_matrix[2] + y_ * transformation_matrix[3];
				img.getPixel(abs(round(final_x+x)), abs(round(final_y+y))) = rgb_colors[color];
			}
		}
		
	}
	else if(mode == DrawingPositionMode::CENTER)
	{
		// NOTE(Sawii00): If the width or height is not even, it excludes 1 pixel to draw it even
		int stride_x = width / 2;
		int stride_y = height / 2;
		
		int x_val;
		int y_val;
		c
			float final_x;
		float final_y;
		
		
		
		for(int y_ = -stride_y ; y_ < stride_y && y_ < (int)img.height() ; y_++)
		{
			for(int x_ = -stride_x ; x_ < stride_x && x_ < (int)img.width() ; x_++)
			{
				final_x = x_ * transformation_matrix[0] + y_ * transformation_matrix[1];
				final_y = x_ * transformation_matrix[2] + y_ * transformation_matrix[3];
				x_val = round(final_x + x);
				y_val = round(final_y + y);
				if(x_val < 0 || y_val < 0 || x_val >= img.width() || y_val >= img.height())continue;
				img.getPixel(abs(round(x_val)), abs(round(y_val))) = rgb_colors[color];
			}
		}
		
	}
	else
	{
		return;
	}
	
	
	
	
	
	
}
 */
