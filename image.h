#pragma once

#include <SFML/Graphics.hpp>
#include "safe_array.h"
#include <fstream>
#include <cstdint>
#include "Utils.h"
#include <iostream>

struct Pixel {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	Pixel(uint8_t _r = 0, uint8_t _g = 0, uint8_t _b = 0, uint8_t _a = 0)
		: r(_r), g(_g), b(_b), a(_a) {}
};

enum ImageType {
	PPM, PGM, PBM, PNG, BMP
};

struct BMPHeader {
	uint32_t size; //in bytes
	uint16_t res1; //reserved
	uint16_t res2; //reserved
	uint32_t image_starting_offset;
};

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

class Image {
private:
	safe_array<Pixel> m_array;
	sf::Image m_img;
	size_t w = 0;
	size_t h = 0;
	const char* m_name = nullptr;
	ImageType m_type;

public:
	Image() {	};
	Image(const char* filepath) :m_name(filepath) {
	};

	size_t height() const {
		return h;
	}
	size_t width() const {
		return w;
	}

	const char* getName() const {
		return m_name;
	}

	void clearImage() {
		w = 0;
		h = 0;
		m_array.clear();
	};

	int loadPPM(const char* filepath) {
		m_name = filepath;
		std::ifstream file(filepath);

		if (file.is_open()) {
			m_array.clear();
			std::string type_string, temp;

			std::getline(file, type_string);

			file >> temp;
			w = std::stoi(temp);
			file >> temp;
			h = std::stoi(temp);

			uint8_t bit;

			m_array.constructArray(w*h); //array is RGBA for each pixel

			int counter = 0;

			if (type_string == "P1" || type_string == "P4") {
				m_type = ImageType::PBM;

				while (!file.eof() && counter < m_array.size) {
					file >> temp;
					bit = std::stoi(temp);
					m_array[counter] = Pixel(255 * bit, 255 * bit, 255 * bit, 255);
					counter++;
				}
			}
			else if (type_string == "P2" || type_string == "P5") {
				m_type = ImageType::PGM;
				file >> temp; //discard max val

				while (!file.eof() && counter < m_array.size) {
					file >> temp;
					bit = std::stoi(temp);
					m_array[counter] = Pixel(bit, bit, bit, 255);
					counter++;
				}
			}
			else if (type_string == "P3" || type_string == "P6") {
				m_type = ImageType::PPM;
				file >> temp; //discard max val
				while (!file.eof() && counter < m_array.size) {
					std::string t1;
					std::string t2;
					std::string t3;
					file >> t1;
					file >> t2;
					file >> t3;
					m_array[counter] = Pixel(std::stoi(t1), std::stoi(t2), std::stoi(t3), 255);

					counter++;
				}
			}
			else {
				return 0;
			}
		}
		else {
			return 0; //could not open file
		}
		return 1;
	}

	int loadPNG(const char* filepath) {}

	/*@TODO:
		- refactor code to reduce if statements and base formulas on BPP
		- implement bitfields (MASKS) and maybe convert the formulas for default masks use
		- implement decompression wherever it applies
	*/

	int loadBMP(const char* filepath) {
		m_name = filepath;
		std::ifstream file(filepath, std::ios::binary);

		if (!file.is_open())
			//throw "Invalid File";
			return 0;
		BMPHeader header;
		BITMAPINFOHEADER bit_info;

		uint8_t buf[12];
		file.ignore(2);
		file.read((char*)buf, 12);

		header = *(BMPHeader*)buf;

		uint8_t head2[40];
		file.read((char*)head2, 40);

		bit_info = *(BITMAPINFOHEADER*)head2;
		this->h = bit_info.height;
		this->w = bit_info.width;

		m_array.constructArray(bit_info.height*bit_info.width);

		safe_array<Pixel> palette;

		if (bit_info.compression > 2) //We do not support crazy compression methods eg huffman
			return 0;

		uint64_t padding = 32 - (bit_info.width * bit_info.bits_per_pixel) % 32;

		uint64_t effective_width_bits = bit_info.width * bit_info.bits_per_pixel;

		uint64_t n_bytes = padding == 32 ? bit_info.height * effective_width_bits : bit_info.height * (effective_width_bits + padding);
		n_bytes /= 8;

		uint64_t bytes_per_row = n_bytes / bit_info.height;

		safe_array<uint8_t> row;
		row.constructArray(bytes_per_row);

		if (bit_info.bits_per_pixel <= 8) {
			//palette present
			uint16_t palette_number = bit_info.number_of_color_in_palette == 0 ? power(2, bit_info.bits_per_pixel) : bit_info.number_of_color_in_palette;
			palette.constructArray(palette_number);

			file.read((char*)palette.getArray(), palette_number * 4);
			for (int i = 0; i < palette_number; i++) {
				uint8_t b = palette[i].b;
				palette[i].b = palette[i].r;
				palette[i].r = b;
				palette[i].a = 0xFF;
			}

			file.seekg(header.image_starting_offset, file.beg);
			/*
			if (bit_info.compression == 1) {
				uint64_t n_of_pixel_extracted = 0;

				uint64_t x = 0;
				uint64_t y = 0;
				char first;
				char second;

				while (n_of_pixel_extracted < bit_info.width * bit_info.height) {
					if (x >= bit_info.width) {
						x = 0;
						y = y < bit_info.height ? y + 1 : y;
					}
					file.get(first);
					file.get(second);

					if (first) {
						for (int i = 0; i < first; i++) {
							m_array[bit_info.width*(bit_info.height - 1 - y) + x] = palette[(uint8_t)second];
						}
						x += first;
						n_of_pixel_extracted += first;
					}
					else {
						switch (second) {
						case 0: {
							//end of line
//							y++;
	//						x = 0;
							break;
						}
						case 1: {
							//end of bitmap
							return 1;
						}
						case 2: {
							//delta (not supported yet)
							char val;
							file.get(val);
							x += val;
							file.get(val);
							y += val;
							break;
						}
						default:
						{
							//ABSOLUTE MODE
							char val;
							uint64_t pad = (second + 2) % 4 ? 4 - (second + 2) % 4 : 0;
							for (int i = 0; i < second; i++) {
								file.get(val);
								uint64_t index = bit_info.width*(bit_info.height - 1 - y) + x;
								if (index >= m_array.size || (uint8_t)val >= palette.size)
									std::cout << "pippo";
								m_array[index] = palette[(uint8_t)val];
								x++;
								n_of_pixel_extracted++;
							}
							file.ignore(pad);
							break;
						}
						}
					}
				}
			}

			*/
			for (int i = 0; i < bit_info.height; i++) {
				file.read((char*)row.getArray(), bytes_per_row);
				for (uint64_t j = 0; j < effective_width_bits; j += bit_info.bits_per_pixel) {
					uint8_t el = row[(uint64_t)j / 8];
					switch (bit_info.bits_per_pixel)
					{
					case 0x1: {
						el = el >> (7 - j % 8) & 0x1;
						break;
					}
					case 0x2: {
						el = el >> (6 - j % 8) & 0x3;
						break;
					}
					case 0x4: {
						el = el >> (4 - j % 8) & 0xF;
						break;
					}
					}
					m_array[bit_info.width*(bit_info.height - 1 - i) + j / bit_info.bits_per_pixel] = palette[el];
				}
				row.clear();
			}
		}
		//@TODO(bitfields)
		else if (bit_info.bits_per_pixel == 16)
		{
			file.seekg(header.image_starting_offset, file.beg);

			//no palette present for bpp > 8
			for (int i = 0; i < bit_info.height; i++) {
				file.read((char*)row.getArray(), bytes_per_row);

				//el is in the form: 0RGB 0rrrrrgggggbbbbb

				for (uint64_t j = 0; j < effective_width_bits; j += 16) {
					uint16_t el = *(uint16_t*)(row.getArray() + j / 8);
					m_array[bit_info.width*(bit_info.height - 1 - i) + j / 16] = Pixel((el >> 0xA & 0x1F) * 8, (el >> 0x5 & 0x1F) * 8, (el & 0x1F) * 8, 0xFF);
				}
			}
		}
		#if 1
		else if (bit_info.bits_per_pixel == 24)
		{
			file.seekg(header.image_starting_offset, file.beg);

			//no palette present for bpp > 8
			for (int i = 0; i < bit_info.height; i++) {
				file.read((char*)row.getArray(), bytes_per_row);
				//........rrrrrrrrggggggggbbbbbbbb
				for (uint64_t j = 0; j < effective_width_bits; j += 24) {
					uint32_t el = *(uint32_t*)(row.getArray() + j / 8);
					m_array[bit_info.width*(bit_info.height - 1 - i) + j / 24] = Pixel((el >> 0x10 & 0xFF), (el >> 0x8 & 0xFF), (el & 0xFF), 0xFF);
				}
			}
		}
		else if (bit_info.bits_per_pixel == 32)
		{
			file.seekg(header.image_starting_offset, file.beg);

			//no palette present for bpp > 8
			for (int i = 0; i < bit_info.height; i++) {
				file.read((char*)row.getArray(), bytes_per_row);
				//rrrrrrrrggggggggbbbbbbbbaaaaaaaa
				for (uint64_t j = 0; j < effective_width_bits; j += 32) {
					uint32_t el = *(uint32_t*)(row.getArray() + j / 8);
					m_array[bit_info.width*(bit_info.height - 1 - i) + j / 32] = Pixel((el >> 0x10 & 0xFF), (el >> 0x8 & 0xFF), (el & 0xFF), 0xFF);
				}
			}
		}
		#else

		else if (bit_info.bits_per_pixel == 24 || bit_info.bits_per_pixel == 32)
		{
			for (int i = 0; i < bit_info.height; i++) {
				file.read((char*)row.getArray(), bytes_per_row);
				//rrrrrrrrggggggggbbbbbbbbaaaaaaaa
				for (uint64_t j = 0; j < effective_width_bits; j += bit_info.bits_per_pixel) {
					uint32_t el = *(uint32_t*)(row.getArray() + j / 8);
					m_array[bit_info.width*(bit_info.height - 1 - i) + j / bit_info.bits_per_pixel] = Pixel((el >> 0x10 & 0xFF), (el >> 0x8 & 0xFF), (el & 0xFF), 0xFF);
				}
			}
		}

		#endif
		else {
			return 0;
		}
		return 1;
	}

	void saveBMP() {
		/*BMPHeader h;
		h.size = 54 + m_array.size;
		h.res1 = 0;
		h.res2 = 0;
		h.image_starting_offset = 54;
		BITMAPINFOHEADER info_h;
		info_h.header_size = 40;
		info_h.width = this->w;
		info_h.height = this->h;
		info_h.color_planes = 1;
		info_h.bits_per_pixel = 24;
		info_h.compression = 0;*/
	}

	void displayImage() {
		sf::RenderWindow window(sf::VideoMode(w, h), m_name);

		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
				}

				window.clear(sf::Color::Black);

				//draw image
				sf::Sprite sprite;
				sf::Texture texture;
				m_img.create(w, h, (uint8_t*)(m_array.getArray()));
				texture.loadFromImage(m_img);
				sprite.setTexture(texture);
				window.draw(sprite);

				window.display();
			}
		}
	}
};
