#include "drawing.h"



static void drawLineWithRect(Img & img, size_t x1, size_t y1, size_t x2, size_t y2, size_t thickness, Color color)
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

static void plotLineLow(Img &img, size_t x1, size_t y1, size_t x2, size_t y2,  Color color)
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
	
	auto img_width = img.width();
	
	auto type = img.getType();
	
	if(type == ImageType::RGB)
	{
		RGBPixel* arr = img.getRGBArray();
		RGBPixel color_pixel = rgb_colors[color];
		
		
		for(size_t x = x1; x <= x2 && x < img_width; x++)
		{
			arr[x + y*img_width]=color_pixel;
			if(D > 0)
			{
				y = y + yi;
				D = D - 2*dx;
			}
			D += 2*dy;
		}
	}
	else if(type == ImageType::HSL)
	{
		HSLPixel* arr = img.getHSLArray();
		HSLPixel color_pixel = hsl_colors[color];
		
		for(size_t x = x1; x <= x2 && x < img_width; x++)
		{
			arr[x + y*img_width]=color_pixel;
			if(D > 0)
			{
				y = y + yi;
				D = D - 2*dx;
			}
			D += 2*dy;
		}
	}
	else if(type == ImageType::GRAY)
	{
		GRAYPixel* arr = img.getGRAYArray();
		GRAYPixel color_pixel = gray_colors[color];
		
		for(size_t x = x1; x <= x2 && x < img_width; x++)
		{
			arr[x + y*img_width]=color_pixel;
			if(D > 0)
			{
				y = y + yi;
				D = D - 2*dx;
			}
			D += 2*dy;
		}
	}
	else if(type == ImageType::BINARY)
	{
		GRAYPixel* arr = img.getGRAYArray();
		
		for(size_t x = x1; x <= x2 && x < img_width; x++)
		{
			arr[x + y*img_width] = 0xFF;
			if(D > 0)
			{
				y = y + yi;
				D = D - 2*dx;
			}
			D += 2*dy;
		}
	}
	else
	{
		return;
	}
	
	
	
}


static void plotLineHigh(Img  &img, size_t x1, size_t y1, size_t x2, size_t y2,  Color color)
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
	
	auto img_height = img.height();
	auto img_width = img.width();
	auto type = img.getType();
	
	if(type == ImageType::RGB)
	{
		RGBPixel* arr = img.getRGBArray();
		RGBPixel color_pixel = rgb_colors[color];
		
		
		for(size_t y = y1; y <= y2 && y < img_height; y++)
		{
			arr[x + y * img_width ]= color_pixel;
			if(D > 0)
			{
				x= x + xi;
				D = D - 2*dy;
			}
			D += 2*dx;
		}
	}
	else if(type == ImageType::HSL)
	{
		HSLPixel* arr = img.getHSLArray();
		HSLPixel color_pixel = hsl_colors[color];
		
		
		for(size_t y = y1; y <= y2 && y < img_height; y++)
		{
			arr[x + y*img_width ]= color_pixel;
			if(D > 0)
			{
				x= x + xi;
				D = D - 2*dy;
			}
			D += 2*dx;
		}
	}
	else if(type == ImageType::GRAY)
	{
		GRAYPixel* arr = img.getGRAYArray();
		GRAYPixel color_pixel = gray_colors[color];
		
		
		for(size_t y = y1; y <= y2 && y < img_height; y++)
		{
			arr[x + y*img_width ]= color_pixel;
			if(D > 0)
			{
				x= x + xi;
				D = D - 2*dy;
			}
			D += 2*dx;
		}
	}
	else if(type == ImageType::BINARY)
	{
		GRAYPixel* arr =  img.getGRAYArray();
		
		
		for(size_t y = y1; y <= y2 && y < img_height; y++)
		{
			arr[x + y*img_width ]= 0xFF;
			if(D > 0)
			{
				x= x + xi;
				D = D - 2*dy;
			}
			D += 2*dx;
		}
	}
	else
	{
		return;
	}
	
	
	
}

// TODO(Sawii00): Add NAIVE thickness
static void drawLineImproved(Img &img, size_t x1, size_t y1, size_t x2, size_t y2, Color color )
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



void drawLine(Img &img, size_t x1, size_t y1, size_t x2, size_t y2, size_t thickness, Color color)
{
	if(!thickness)return;
	
	if(thickness > 1)
	{
		drawLineWithRect(img, x1, y1, x2, y2, thickness, color);
	}
	else
	{
		drawLineImproved(img, x1, y1, x2, y2, color);
	}
	
}



void drawRect(Img& img, size_t x, size_t y, size_t width, size_t height, Color color, DrawingPositionMode mode)
{
	
	// NOTE(Sawii00): does not need to check if it exceeds the height and width because it skips the loop and does not draw anything if that happens
	
	
	size_t initial_y;
	size_t initial_x;
	
	size_t final_y;
	size_t final_x;
	
	ImageType type = img.getType();
	
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
	
	
	auto img_width = img.width();
	auto img_height = img.height();
	
	
	if(type == ImageType::RGB)
	{
		RGBPixel* arr =  img.getRGBArray();
		RGBPixel color_pixel = rgb_colors[color];
		
		
		for(size_t y_ = initial_y ; y_ < final_y && y_ < img_height ; y_++)
		{
			for(size_t x_ = initial_x ; x_ < final_x && x_ < img_width ; x_++)
			{
				arr[y_ * img_width + x_] = color_pixel;
			}
		}
	}
	else if(type == ImageType::HSL)
	{
		HSLPixel* arr =  img.getHSLArray();
		HSLPixel color_pixel = hsl_colors[color];
		
		for(size_t y_ = initial_y ; y_ < final_y && y_ < img_height ; y_++)
		{
			for(size_t x_ = initial_x ; x_ < final_x && x_ < img_width ; x_++)
			{
				arr[y_ * img_width + x_] = color_pixel;
			}
		}
	}
	else if(type == ImageType::GRAY)
	{
		GRAYPixel* arr =  img.getGRAYArray();
		GRAYPixel color_pixel = gray_colors[color];
		
		for(size_t y_ = initial_y ; y_ < final_y && y_ < img_height ; y_++)
		{
			for(size_t x_ = initial_x ; x_ < final_x && x_ < img_width ; x_++)
			{
				arr[y_ * img_width + x_] = color_pixel;
			}
		}
	}
	else if(type == ImageType::BINARY)
	{
		GRAYPixel* arr =  img.getGRAYArray();
		
		for(size_t y_ = initial_y ; y_ < final_y && y_ < img_height ; y_++)
		{
			for(size_t x_ = initial_x ; x_ < final_x && x_ < img_width ; x_++)
			{
				arr[y_ * img_width + x_] = 0xFF;
			}
		}
	}
	else
	{
		return;
	}
	
}


void drawRotatedRect(Img &img, size_t x, size_t y, size_t width, size_t height, float angle, Color color, DrawingPositionMode mode)
{
	
	float transformation_matrix[4] =
	{
		cos(angle), -sin(angle), sin(angle), cos(angle)
	};
	
	
	int initial_y;
	int initial_x;
	
	int final_y;
	int final_x;
	
	ImageType type = img.getType();
	int img_width = img.width();
	int img_height = img.height();
	
	
	if(mode == DrawingPositionMode::UPPER_CORNER)
	{
		initial_y = 0;
		initial_x = 0;
		final_y = height;
		final_x = width;
	}
	else if(mode == DrawingPositionMode::CENTER)
	{
		int stride_x = width / 2;
		int stride_y = height / 2;
		
		initial_x = -stride_x;
		initial_y = -stride_y;
		final_x = stride_x;
		final_y = stride_y;
	}
	else
	{
		return;
	}
	
	
	float computed_final_x;
	float computed_final_y;
	
	if(type == ImageType::RGB)
	{
		RGBPixel* arr = img.getRGBArray();
		RGBPixel color_pixel = rgb_colors[color];
		
		
		for(int y_ = initial_y ; y_ < final_y && y_ < img_height ; y_++)
		{
			for(int x_ = initial_x ; x_ < final_x && x_ < img_width ; x_++)
			{
				computed_final_x = x_ * transformation_matrix[0] + y_ * transformation_matrix[1];
				computed_final_y = x_ * transformation_matrix[2] + y_ * transformation_matrix[3];
				// TODO(Sawii00): FIX THIS
				arr[(uint32_t)(abs(round(y + computed_final_y)) * img_width + abs(round(x + computed_final_x)))] = color_pixel;
			}
		}
	}
	else if(type == ImageType::HSL)
	{
		HSLPixel* arr =  img.getHSLArray();
		HSLPixel color_pixel = hsl_colors[color];
		
		for(int y_ = initial_y ; y_ < final_y && y_ < img_height ; y_++)
		{
			for(int x_ = initial_x ; x_ < final_x && x_ < img_width ; x_++)
			{
				computed_final_x = x_ * transformation_matrix[0] + y_ * transformation_matrix[1];
				computed_final_y = x_ * transformation_matrix[2] + y_ * transformation_matrix[3];
				
				arr[(uint32_t)(abs(round(y + computed_final_y)) * img_width + abs(round(x + computed_final_x)))] = color_pixel;
			}
		}
	}
	else if(type == ImageType::GRAY)
	{
		GRAYPixel* arr = img.getGRAYArray();
		GRAYPixel color_pixel = gray_colors[color];
		
		for(int y_ = initial_y ; y_ < final_y && y_ < img_height ; y_++)
		{
			for(int x_ = initial_x ; x_ < final_x && x_ < img_width ; x_++)
			{
				computed_final_x = x_ * transformation_matrix[0] + y_ * transformation_matrix[1];
				computed_final_y = x_ * transformation_matrix[2] + y_ * transformation_matrix[3];
				
				arr[(uint32_t)(abs(round(y + computed_final_y)) * img_width + abs(round(x + computed_final_x)))] = color_pixel;
			}
		}
	}
	else if(type == ImageType::BINARY)
	{
		GRAYPixel* arr =  img.getGRAYArray();
		
		for(int y_ = initial_y ; y_ < final_y && y_ < img_height ; y_++)
		{
			for(int x_ = initial_x ; x_ < final_x && x_ < img_width ; x_++)
			{
				computed_final_x = x_ * transformation_matrix[0] + y_ * transformation_matrix[1];
				computed_final_y = x_ * transformation_matrix[2] + y_ * transformation_matrix[3];
				
				arr[(uint32_t)(abs(round(y + computed_final_y)) * img_width + abs(round(x + computed_final_x)))] = 0xFF;
			}
		}
	}
	else
	{
		return;
	}
	
	
	
	
	
}

