#pragma once
#include <fstream>
#include <SFML/Graphics.hpp>
#include "img.h"


/*
First 14 bytes of a BMP Image is the header

*/
struct BMPHeader {
	uint32_t file_size; //in bytes
	uint16_t res1; //reserved
	uint16_t res2; //reserved
	uint32_t image_starting_offset;
};

/*
40 Bytes of additional information (BITMAPINFOHEADER)
*/
struct BITMAPINFOHEADER {
	uint32_t header_size;
	int32_t width;
	int32_t height;
	uint16_t color_planes;
	uint16_t bits_per_pixel;
	uint32_t compression;
	uint32_t final_image_size;
	int32_t horizontal_resolution;
	int32_t vertical_resolution;
	uint32_t number_of_color_in_palette;
	uint32_t number_of_important_colors;
};

int save(Img& img, const char* filepath);
int load(Img& img, const char* file_path, ImageType type = ImageType::RGB);
void show(Img& img, const char* window_name = "Image");