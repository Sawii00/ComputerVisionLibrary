#pragma once

static RGBPixel rgb_colors[] = {
	{255,0,0,255}, //Red
	{0,255,0,255}, //Green
	{0,0,255,255}, //Blue
	{0,0,0,255}, //Black
	{255,255,255,255} //White
};


static HSLPixel hsl_colors[] = {
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


void drawRect(Img &img, size_t x, size_t y, size_t width, size_t height, Color color, DrawingPositionMode mode = DrawingPositionMode::DEFAULT);

void drawRotatedRect(Img &img, size_t x, size_t y, size_t width, size_t height, float angle, Color color, DrawingPositionMode mode = DrawingPositionMode::DEFAULT);

void drawBoundingRect(Img &img, size_t x, size_t y, size_t width, size_t height, size_t thickness, Color color, DrawingPositionMode mode = DrawingPositionMode::DEFAULT);

void drawRotatedBoundingRect(Img &img, size_t x, size_t y, size_t width, size_t height, float angle, size_t thickness, Color color, DrawingPositionMode mode = DrawingPositionMode::DEFAULT);

namespace Private
{
	void drawLineWithRect(Img & img, size_t x1, size_t y1, size_t x2, size_t y2, size_t thickness, Color color);
	
	void plotLineLow(Img &img, size_t x1, size_t y1, size_t x2, size_t y2,  Color color);
	
	void plotLineHigh(Img &img, size_t x1, size_t y1, size_t x2, size_t y2,  Color color);
	
	void drawLineImproved(Img &img, size_t x1, size_t y1, size_t x2, size_t y2, Color color );
	
};

void drawLine(Img &img, size_t x1, size_t y1, size_t x2, size_t y2, size_t thickness, Color color);