#pragma once
#include "pixel.h"
#include "img.h"
#include "color.h"




enum DrawingPositionMode
{
	UPPER_CORNER = 1, CENTER
};



class Img;

void drawRect(Img &img, size_t x, size_t y, size_t width, size_t height, Color color, DrawingPositionMode mode = DrawingPositionMode::UPPER_CORNER);

void drawRotatedRect(Img &img, size_t x, size_t y, size_t width, size_t height, float angle, Color color, DrawingPositionMode mode = DrawingPositionMode::UPPER_CORNER);

void drawBoundingRect(Img &img, size_t x, size_t y, size_t width, size_t height, size_t thickness, Color color, DrawingPositionMode mode = DrawingPositionMode::UPPER_CORNER);

void drawRotatedBoundingRect(Img &img, size_t x, size_t y, size_t width, size_t height, float angle, size_t thickness, Color color, DrawingPositionMode mode = DrawingPositionMode::UPPER_CORNER);

static void drawLineWithRect(Img & img, size_t x1, size_t y1, size_t x2, size_t y2, size_t thickness, Color color);

static void plotLineLow(Img &img, size_t x1, size_t y1, size_t x2, size_t y2,  Color color);

static void plotLineHigh(Img &img, size_t x1, size_t y1, size_t x2, size_t y2,  Color color);

static void drawLineImproved(Img &img, size_t x1, size_t y1, size_t x2, size_t y2, Color color );


void drawLine(Img &img, size_t x1, size_t y1, size_t x2, size_t y2, size_t thickness, Color color);