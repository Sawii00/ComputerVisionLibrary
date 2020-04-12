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
// TODO(Sawii00): Fix bug of skipped pixels
void drawRotatedRect(RGBImage &img, size_t x, size_t y, size_t width, size_t height, float angle, Color color, DrawingPositionMode mode = DrawingPositionMode::DEFAULT)
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

/*
void drawRect(RGBImage &img, size_t x1, size_t y1, size_t x2, size_t y2, Color color)
{
	size_t height = abs((int) y2 - (int) y1);
	size_t width = abs((int) x2 - (int) x1);
	
	if(x1 > x2)
	{
		size_t temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	
	drawRect(img, x1, y1, width, height, color),
}
 */

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
// TODO(Sawii00): Fix the rotation by adding the translation back to the origin.
void drawRotatedBoundingRect(RGBImage &img, size_t x, size_t y, size_t width, size_t height, float angle, size_t thickness, Color color, DrawingPositionMode mode = DrawingPositionMode::DEFAULT)
{
	
	
	
	float transformation_matrix[4] =
	{
		cos(angle), -sin(angle), sin(angle), cos(angle)
	};
	
	float final_x;
	float final_y;
	
	if(mode == DrawingPositionMode::DEFAULT)
	{
		// NOTE(Sawii00): does not need to check if it exceeds the height and width because it skips the loop and does not draw anything if that happens
		
		
		for(size_t y_ = y ; y_ < y + thickness && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x ; x_ < x + width && x_ < img.width() ; x_++)
			{
				final_x = x_ * transformation_matrix[0] + y_ * transformation_matrix[1];
				final_y = x_ * transformation_matrix[2] + y_ * transformation_matrix[3];
				img.getPixel(abs(round(final_x)), abs(round(final_y))) = rgb_colors[color];
			}
		}
		
		for(size_t y_ = y + height - thickness ; y_ < y + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x ; x_ < x + width && x_ < img.width() ; x_++)
			{
				final_x = x_ * transformation_matrix[0] + y_ * transformation_matrix[1];
				final_y = x_ * transformation_matrix[2] + y_ * transformation_matrix[3];
				img.getPixel(abs(round(final_x)), abs(round(final_y))) = rgb_colors[color];
			}
		}
		
		for(size_t y_ = y ; y_ < y + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x ; x_ < x + thickness && x_ < img.width() ; x_++)
			{
				final_x = x_ * transformation_matrix[0] + y_ * transformation_matrix[1];
				final_y = x_ * transformation_matrix[2] + y_ * transformation_matrix[3];
				img.getPixel(abs(round(final_x)), abs(round(final_y))) = rgb_colors[color];
			}
		}
		
		for(size_t y_ = y ; y_ < y + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x + width - thickness ; x_ < x + width && x_ < img.width() ; x_++)
			{
				final_x = x_ * transformation_matrix[0] + y_ * transformation_matrix[1];
				final_y = x_ * transformation_matrix[2] + y_ * transformation_matrix[3];
				img.getPixel(abs(round(final_x)), abs(round(final_y))) = rgb_colors[color];
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
				final_x = x_ * transformation_matrix[0] + y_ * transformation_matrix[1];
				final_y = x_ * transformation_matrix[2] + y_ * transformation_matrix[3];
				img.getPixel(abs(round(final_x)), abs(round(final_y))) = rgb_colors[color];
			}
		}
		
		for(size_t y_ = y_start + height - thickness ; y_ < y_start + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x_start ; x_ < x_start + width && x_ < img.width() ; x_++)
			{
				final_x = x_ * transformation_matrix[0] + y_ * transformation_matrix[1];
				final_y = x_ * transformation_matrix[2] + y_ * transformation_matrix[3];
				img.getPixel(abs(round(final_x)), abs(round(final_y))) = rgb_colors[color];
			}
		}
		
		for(size_t y_ = y_start ; y_ < y_start + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x_start ; x_ < x_start + thickness && x_ < img.width() ; x_++)
			{
				final_x = x_ * transformation_matrix[0] + y_ * transformation_matrix[1];
				final_y = x_ * transformation_matrix[2] + y_ * transformation_matrix[3];
				img.getPixel(abs(round(final_x)), abs(round(final_y))) = rgb_colors[color];
			}
		}
		
		for(size_t y_ = y_start ; y_ < y_start + height && y_ < img.height() ; y_++)
		{
			for(size_t x_ = x_start + width - thickness ; x_ < x_start + width && x_ < img.width() ; x_++)
			{
				final_x = x_ * transformation_matrix[0] + y_ * transformation_matrix[1];
				final_y = x_ * transformation_matrix[2] + y_ * transformation_matrix[3];
				img.getPixel(abs(round(final_x)), abs(round(final_y))) = rgb_colors[color];
			}
		}
		
	}
	else
	{
		return;
	}
	
	
}




/*
void drawBoundingRect(RGBImage &img, size_t x1, size_t y1, size_t x2, size_t y2, size_t thickness, Color color)
{
	size_t height = abs((int) y2 - (int) y1);
	size_t width = abs((int) x2 - (int) x1);
	
	if(x1 > x2)
	{
		size_t temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	drawBoundingRect(img, x1, y1, width, height, thickness, color),
}
 */

namespace Private
{
	void drawLineWithRect(RGBImage & img, size_t x1, size_t y1, size_t x2, size_t y2, size_t thickness, Color color)
	{
		
		int _x1;
		int _x2;
		int _y1;
		int _y2;
		
		if(y1 > y2)
		{
			_y1 = y2;
			_y2 = y1;
			_x1 = x2;
			_x2 = x1;
		}
		else
		{
			_y1 = y1;
			_y2 = y2;
			_x1 = x1;
			_x2 = x2;
		}
		
		size_t width = thickness;
		size_t height = round(sqrt((_x1-_x2)*(_x1-_x2)+(_y1-_y2)*(_y1-_y2)));
		
		if(x2 != x1)
		{
			float m = ((float)y2 - y1)/((float)x2-x1);
			
			
			
			if(_x2 == _x1)
			{
				drawRect(img, _x2, _y2, width, height, color);
			}
			else
			{
				float angle = PI/2 - (atan(abs(_y2 - _y1)/abs(_x2 - _x1)));
				angle = m < 0 ? angle : -angle;
				drawRotatedRect(img, _x1, _y1, width, height, angle , color);
			}
			
		}
		else
		{
			drawRect(img, x1, y1, width, height, color);
		}
	}
	
	void plotLineLow(RGBImage &img, size_t x1, size_t y1, size_t x2, size_t y2,  Color color)
	{
		
		int dx = x2-x1;
		int dy = y2-y1;
		int yi = 1;
		
		if(dy < 0)
		{
			yi = -1;
			dy = -dy;
		}
		
		int D = 2*dy - dx;
		size_t y = y1;
		
		
		for(size_t x = x1; x <= x2 && x < img.width(); x++)
		{
			img.getPixel(x, y) = rgb_colors[color];
			if(D > 0)
			{
				y = y + yi;
				D = D - 2*dx;
			}
			D += 2*dy;
		}
		
		
	}
	
	
	void plotLineHigh(RGBImage &img, size_t x1, size_t y1, size_t x2, size_t y2,  Color color)
	{
		
		
		int dx = x2-x1;
		int dy = y2-y1;
		int xi = 1;
		
		if(dx < 0)
		{
			xi = -1;
			dx = -dx;
		}
		int D = 2*dx - dy;
		size_t x = x1;
		
		
		for(size_t y = y1; y <= y2 && y < img.height(); y++)
		{
			img.getPixel(x, y) = rgb_colors[color];
			if(D > 0)
			{
				x= x + xi;
				D = D - 2*dy;
			}
			D += 2*dx;
		}
		
	}
	
	// TODO(Sawii00): Add NAIVE thickness
	void drawLineImproved(RGBImage &img, size_t x1, size_t y1, size_t x2, size_t y2, Color color )
	{
		
		if(abs((int)y2 - (int) y1) < abs((int)x2 - (int)x1))
		{
			if(x1 > x2)
			{
				plotLineLow(img, x2, y2, x1, y1, color);
			}
			else
			{
				plotLineLow(img, x1, y1, x2, y2, color);
			}
			
			
		}
		else
		{
			
			if(y1 > y2)
			{
				plotLineHigh(img, x2, y2, x1, y1, color);
			}
			else
			{
				plotLineHigh(img, x1, y1, x2, y2, color);
			}
			
		}
		
	}
}


void drawLine(RGBImage &img, size_t x1, size_t y1, size_t x2, size_t y2, size_t thickness, Color color)
{
	if(!thickness)return;
	
	if(thickness > 1)
	{
		Private::drawLineWithRect(img, x1, y1, x2, y2, thickness, color);
	}
	else
	{
		Private::drawLineImproved(img, x1, y1, x2, y2, color);
	}
	
}


