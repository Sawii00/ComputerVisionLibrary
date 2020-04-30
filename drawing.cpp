#include "drawing.h"

void drawRect(Img& img, size_t x, size_t y, size_t width, size_t height, Color color, DrawingPositionMode mode)
{
	if(img.getType() == ImageType::RGB)
	{
		
		
		if(mode == DrawingPositionMode::DEFAULT)
		{
			// NOTE(Sawii00): does not need to check if it exceeds the height and width because it skips the loop and does not draw anything if that happens
			for(size_t y_ = y ; y_ < y + height && y_ < img.height() ; y_++)
			{
				for(size_t x_ = x ; x_ < x + width && x_ < img.width() ; x_++)
				{
					img.getPixel(x_, y_) = rgb_colors[color];
				}
			}
			
		}
		else if(mode == DrawingPositionMode::CENTER)
		{
			// NOTE(Sawii00): If the width or height is not even, it excludes 1 pixel to draw it even
			size_t stride_x = width / 2;
			size_t stride_y = height / 2;
			
			size_t x_start = (int)(x - stride_x) < 0 ? 0 : x - stride_x;
			size_t y_start = (int)(y - stride_y) < 0 ? 0 : y - stride_y;
			
			for(size_t y_ = y_start ; y_ <= y + stride_y && y_ < img.height() ; y_++)
			{
				for(size_t x_ = x_start ; x_ <= x + stride_x && x_ < img.width() ; x_++)
				{
					img.getPixel(x_, y_) = rgb_colors[color];
				}
			}
			
		}
		else
		{
			return;
		}
	}
	else if(img.getType() == ImageType::HSL)
	{
		
	}
	else if(img.getType() == ImageType::GRAY ||img.getType() == ImageType::BINARY)
	{
		
	}
	else
	{
		return;
	}
}
