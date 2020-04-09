#pragma once
#include <cmath>
#include "Utils.h"

static Pixel rgb_colors[] = {
	{255,0,0,255}, //Red
	{0,255,0,255}, //Green
	{0,0,255,255}, //Blue
	{0,0,0,255}, //Black
	{255,255,255,255} //White
};


static HSL_Pixel hsl_colors[] = {
	{0 , 1.0f , 0.5f}, //Red
	{120 , 1.0f , 0.5f}, //Green
	{240 , 1.0f , 0.5f}, //Blue
	{0, 0.0f, 0.0f}, //Black
	{0, 0.0f, 1.0f} //White
};


enum Color
{
	RED = 0, GREEN, BLUE, BLACK, WHITE
};

enum DrawingPositionMode
{
	DEFAULT = 1, CENTER
};

void drawRect(RGBImage &img, size_t x, size_t y, size_t width, size_t height, Color color, DrawingPositionMode mode = DrawingPositionMode::DEFAULT)
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
		
		size_t y_start = (int)(y - stride_y) < 0 ? 0 : y - stride_y;
		size_t x_start = (int)(x - stride_x) < 0 ? 0 : x - stride_x;
		
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

void drawBoundingRect(RGBImage &img, size_t x, size_t y, size_t width, size_t height, size_t thickness, Color color, DrawingPositionMode mode = DrawingPositionMode::DEFAULT)
{
	
	if(mode == DrawingPositionMode::DEFAULT)
	{
		// NOTE(Sawii00): does not need to check if it exceeds the height and width because it skips the loop and does not draw anything if that happens
		for(size_t y_ = y ; y_ < y + thickness && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x ; x_ < x + width && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = rgb_colors[color];
			}
		}
		
		for(size_t y_ = y + height - thickness ; y_ < y + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x ; x_ < x + width && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = rgb_colors[color];
			}
		}
		
		for(size_t y_ = y ; y_ < y + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x ; x_ < x + thickness && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = rgb_colors[color];
			}
		}
		
		for(size_t y_ = y ; y_ < y + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x + width - thickness ; x_ < x + width && x_ < img.width() ; x_++)
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
		
		size_t y_start = (int)(y - stride_y) < 0 ? 0 : y - stride_y;
		size_t x_start = (int)(x - stride_x) < 0 ? 0 : x - stride_x;
		
		for(size_t y_ = y_start ; y_ < y_start + thickness && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x_start ; x_ < x_start + width && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = rgb_colors[color];
			}
		}
		
		for(size_t y_ = y_start + height - thickness ; y_ < y_start + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x_start ; x_ < x_start + width && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = rgb_colors[color];
			}
		}
		
		for(size_t y_ = y_start ; y_ < y_start + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x_start ; x_ < x_start + thickness && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = rgb_colors[color];
			}
		}
		
		for(size_t y_ = y_start ; y_ < y_start + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x_start + width - thickness ; x_ < x_start + width && x_ < img.width() ; x_++)
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

// TODO(Sawii00): Implement Bresenham Algorithm (FOR CIRCLES TOO)
void drawLine(RGBImage &img, size_t x1, size_t y1, size_t x2, size_t y2, size_t thickness , Color color)
{
	if(x1 != x2)
	{
		size_t temp = minVal<size_t>(x1, x2);
		x2 = x1 + x2 - temp;
		x1 = temp;
		temp = minVal<size_t>(y1, y2);
		y2 = y1 + y2 - temp;
		y1 = temp;
		
		size_t y = 0;
		float m = (float)(y2 - y1) / (x2 - x1);
		// TODO(Sawii00): vertical lines
		int stride = (int)(thickness / 2);
		
		for(int t = -stride; t <= stride; t++)
		{
			for(size_t x = x1 ; x <= x2 && x < img.width() ; x++)
			{
				y = round(m * (x- x1)) + y1 + t;
				if(y >= img.height())break;
				
				img.getPixel(x, y) = rgb_colors[color];
			}
		}
		
	}
	else
	{
		// NOTE(Sawii00): Vertical Lines here
		
		int stride = (int)(thickness / 2);
		
		for(int t = -stride; t <= stride; t++)
		{
			for(size_t y = y1 ; y <= y2 && y < img.height() ; y++)
			{
				img.getPixel(x1, y) = rgb_colors[color];
			}
		}
		
		
	}
}




/*

HSL_IMAGES VERSION OF THE DRAWINGS


*/





// TODO(Sawii00): Test this
void drawRect(HSLImage  &img, size_t x, size_t y, size_t width, size_t height, Color color, DrawingPositionMode mode = DrawingPositionMode::DEFAULT)
{
	
	if(mode == DrawingPositionMode::DEFAULT)
	{
		// NOTE(Sawii00): does not need to check if it exceeds the height and width because it skips the loop and does not draw anything if that happens
		for(size_t y_ = y ; y_ < y + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x ; x_ < x + width && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = hsl_colors[color];
			}
		}
		
	}
	else if(mode == DrawingPositionMode::CENTER)
	{
		// NOTE(Sawii00): If the width or height is not even, it excludes 1 pixel to draw it even
		size_t stride_x = width / 2;
		size_t stride_y = height / 2;
		
		size_t y_start = (int)(y - stride_y) < 0 ? 0 : y - stride_y;
		size_t x_start = (int)(x - stride_x) < 0 ? 0 : x - stride_x;
		
		for(size_t y_ = y_start ; y_ <= y + stride_y && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x_start ; x_ <= x + stride_x && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = hsl_colors[color];
			}
		}
		
	}
	else
	{
		return;
	}
	
	
}

void drawBoundingRect(HSLImage &img, size_t x, size_t y, size_t width, size_t height, size_t thickness, Color color, DrawingPositionMode mode = DrawingPositionMode::DEFAULT)
{
	
	if(mode == DrawingPositionMode::DEFAULT)
	{
		// NOTE(Sawii00): does not need to check if it exceeds the height and width because it skips the loop and does not draw anything if that happens
		for(size_t y_ = y ; y_ < y + thickness && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x ; x_ < x + width && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = hsl_colors[color];
			}
		}
		
		for(size_t y_ = y + height - thickness ; y_ < y + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x ; x_ < x + width && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = hsl_colors[color];
			}
		}
		
		for(size_t y_ = y ; y_ < y + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x ; x_ < x + thickness && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = hsl_colors[color];
			}
		}
		
		for(size_t y_ = y ; y_ < y + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x + width - thickness ; x_ < x + width && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = hsl_colors[color];
			}
		}
		
		
	}
	else if(mode == DrawingPositionMode::CENTER)
	{
		
		// NOTE(Sawii00): If the width or height is not even, it excludes 1 pixel to draw it even
		size_t stride_x = width / 2;
		size_t stride_y = height / 2;
		
		size_t y_start = (int)(y - stride_y) < 0 ? 0 : y - stride_y;
		size_t x_start = (int)(x - stride_x) < 0 ? 0 : x - stride_x;
		
		for(size_t y_ = y_start ; y_ < y_start + thickness && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x_start ; x_ < x_start + width && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = hsl_colors[color];
			}
		}
		
		for(size_t y_ = y_start + height - thickness ; y_ < y_start + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x_start ; x_ < x_start + width && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = hsl_colors[color];
			}
		}
		
		for(size_t y_ = y_start ; y_ < y_start + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x_start ; x_ < x_start + thickness && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = hsl_colors[color];
			}
		}
		
		for(size_t y_ = y_start ; y_ < y_start + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x_start + width - thickness ; x_ < x_start + width && x_ < img.width() ; x_++)
			{
				img.getPixel(x_, y_) = hsl_colors[color];
			}
		}
		
	}
	else
	{
		return;
	}
	
}

