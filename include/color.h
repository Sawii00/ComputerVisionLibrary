#pragma once
#include "pixel.h"


static RGBPixel rgb_colors[] = {
	{255,0,0,255}, //Red
	{0,255,0,255}, //Green
	{0,0,255,255}, //Blue
	{0,0,0,255}, //Black
	{255,255,255,255}, //White
	{229, 229, 229, 255}, //Gray 10%
	{203, 203, 203, 255}, //Gray 20%
	{128, 128, 128, 255}, //Gray 50%
	{64, 64, 64, 255} //Gray 75%
};


static HSLPixel hsl_colors[] = {
	{0 , 1.0f , 0.5f}, //Red
	{120 , 1.0f , 0.5f}, //Green
	{240 , 1.0f , 0.5f}, //Blue
	{0, 0.0f, 0.0f}, //Black
	{0, 0.0f, 1.0f}, //White
	{0, 0.0f, 0.9f}, //Gray 10%
	{0, 0.0f, 0.8f}, //Gray 20%
	{0, 0.0f, 0.5f}, //Gray 50%
	{0, 0.0f, 0.25f} //Gray 75%
};


static GRAYPixel gray_colors[] = {
	{0}, //Red
	{0}, //Green
	{0}, //Blue
	{0}, //Black
	{255}, //White
	{229}, //Gray 10%
	{203}, //Gray 20%
	{128}, //Gray 50%
	{64} //Gray 75%
};

static GRAYPixel binary_colors[] = {
	{255}, //Red
	{255}, //Green
	{255}, //Blue
	{0}, //Black
	{255}, //White
	{255}, //Gray 10%
	{255}, //Gray 20%
	{255}, //Gray 50%
	{255} //Gray 75%
};


enum Color
{
	RED = 0, GREEN, BLUE, BLACK, WHITE, GRAY_10, GRAY_20, GRAY_50, GRAY_75
};
