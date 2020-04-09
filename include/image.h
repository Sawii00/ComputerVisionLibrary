#pragma once

#include <SFML/Graphics.hpp>
#include <thread>
#include <fstream>
#include <cstdint>
#include <iostream>
#include "safe_array.h"
#include "Utils.h"
#include "filter.h"
#include "GPU_utils.h"

class HSLImage;

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

/*
Class Abstraction of an RGBImage with methods for working on it
*/
class RGBImage {
	
	private:
	safe_array<Pixel> m_array;
	sf::Image m_img;
	size_t w = 0;
	size_t h = 0;
	const char* m_name = nullptr;
	bool cuda_support = true;
	Pixel*  m_buffer = nullptr;
	bool AVX_SUPPORT = false;
	bool SSE_SUPPORT = false;
	
	friend class UnitTesting;
	
	void setupVectorSupport()
	{
		if (Utils::AVXAvailable())
		{
			AVX_SUPPORT = true;
		}
		else if (Utils::SSEAvailable())
		{
			SSE_SUPPORT = true;
		}
	}
	
	/*
Internal Method which computes the Horizontal Filtering of an RGBImage (Executed on separate threads)
- start is included
- finish is excluded
*/
	static void separable_thread_operation_h(SeparableFilter* horizontal, uint32_t start, uint32_t finish, RGBImage* im) {
		//start included, finish excluded
		
		KernelPixel result;
		int stride = int(horizontal->getSideSize() / 2);
		
		for (int y = start; y < finish; y++) {//y of the matrix
			for (int x = 0; x < im->width(); x++) {//x of the matrix
				result.reset();
				
				for (int k = 0; k < horizontal->getSideSize(); k++) { //x of the filter
					result += (KernelPixel(Utils::mirrorGet(im->getPixelArray(), (x + k - stride), y, im->width(), im->height())) * horizontal->get(k));
				}
				
				im->m_buffer[im->width() * y + x] = result.getPixel();
			}
		}
	}
	
	/*
Internal Method which computes the Vertical  Filtering of an RGBImage (Executed on separate threads)
- start is included
- finish is excluded
*/
	static void separable_thread_operation_v(SeparableFilter* vertical, uint32_t start, uint32_t finish, RGBImage* im) {
		KernelPixel result;
		int stride = int(vertical->getSideSize() / 2);
		
		for (int y = start; y < finish; y++) {//y of the matrix
			for (int x = 0; x < im->width(); x++) {//x of the matrix
				result.reset();
				
				for (int l = 0; l < vertical->getSideSize(); l++) { //y of the filter
					result += (KernelPixel(Utils::mirrorGet(im->m_buffer, x, (y + l - stride), im->width(), im->height())) * vertical->get(l));
				}
				
				im->getPixelArray()[im->width() * y + x] = result.getPixel();
			}
		}
	}
	
	static void non_separable_thread_operation(Filter* filter, uint32_t start, uint32_t finish, RGBImage* im) {
		KernelPixel result;
		auto stride = int(filter->getSideSize() / 2);
		
		for (int y = start; y < finish; y++) {//y of the matrix
			for (int x = 0; x < im->width(); x++) {//x of the matrix
				result.reset();
				
				for (int l = 0; l < filter->getSideSize(); l++) { //y of the filter
					for (int k = 0; k < filter->getSideSize(); k++) { //x of the filter
						result += (KernelPixel(Utils::mirrorGet(im->getPixelArray(), (x + k - stride), (y + l - stride), im->width(), im->height())) * filter->get(l * filter->getSideSize() + k));
					}
				}
				
				im->m_buffer[im->width() * y + x] = result.getPixel();
			}
		}
	}
	
	static void non_separable_thread_operation3D(Filter* filter, uint32_t start, uint32_t finish, RGBImage* im) {
		KernelPixel result;
		auto stride = int(filter->getSideSize() / 2);
		
		for (int y = start; y < finish; y++) {//y of the matrix
			for (int x = 0; x < im->width(); x++) {//x of the matrix
				result.reset();
				
				for (int l = 0; l < filter->getSideSize(); l++) { //y of the filter
					for (int k = 0; k < filter->getSideSize(); k++) { // x of the filter
						result += (KernelPixel(Utils::mirrorGet(im->getPixelArray(), (x + k - stride), (y + l - stride), im->width(), im->height())) * filter->getKP(l * filter->getSideSize() + k));
					}
				}
				
				im->m_buffer[im->width() * y + x] = result.getPixel();
			}
		}
	}
	
	/*
Inefficient Convolution between the current RGBImage and the specificed Kernel (1D Kernel) (Applied to all the Color Planes)
*/
	void convolve1D(Filter& kernel) {
		KernelPixel result;
		int stride = int(kernel.getSideSize() / 2);
		for (int y = 0; y < this->height(); y++) {//y of the matrix
			for (int x = 0; x < this->width(); x++) {//x of the matrix
				result.reset();
				
				for (int l = 0; l < kernel.getSideSize(); l++) { //y of the filter
					for (int k = 0; k < kernel.getSideSize(); k++) { // x of the filter
						result += KernelPixel(Utils::mirrorGet(m_array.getArray(), (x + k - stride), (y + l - stride), this->width(), this->height())) * kernel.get(l * kernel.getSideSize() + k);
					}
				}
				
				this->m_buffer[this->width() * y + x] = result.getPixel();
			}
		}
		
		m_array.swapArray(m_buffer, m_array.size);
	}
	
	/*
Inefficient Convolution between the current RGBImage and the specified Kernel (3D Kernel) (Applied separately to each Color Plane)
*/
	void convolve3D(Filter& kernel) {
		KernelPixel result;
		
		int stride = int(kernel.getSideSize() / 2);
		for (int y = 0; y < this->height(); y++) {//y of the matrix
			for (int x = 0; x < this->width(); x++) {//x of the matrix
				result.reset();
				
				for (int l = 0; l < kernel.getSideSize(); l++) { //y of the filter
					for (int k = 0; k < kernel.getSideSize(); k++) { // x of the filter
						result += (KernelPixel(Utils::mirrorGet(m_array.getArray(), (x + k - stride), (y + l - stride), this->width(), this->height())) * kernel.getKP(l * kernel.getSideSize() + k));
					}
				}
				
				this->m_buffer[this->width() * y + x] = result.getPixel();
			}
		}
		
		m_array.swapArray(m_buffer, m_array.size);
	}
	
	/*
Single-Threaded implementation of the Convolution between the Current RGBImage and a Separable Kernel (Split in the two Components)
NOTES:
1) Vertical component goes first
2) Horizontal component goes second
*/
	void separableConvolution1D(SeparableFilter& vertical, SeparableFilter& horizontal) {
		KernelPixel result;
		
		int stride = int(vertical.getSideSize() / 2);
		
		for (int y = 0; y < this->height(); y++) {//y of the matrix
			for (int x = 0; x < this->width(); x++) {//x of the matrix
				result.reset();
				
				for (int k = 0; k < horizontal.getSideSize(); k++) { //x of the filter
					result += (KernelPixel(Utils::mirrorGet(m_array.getArray(), (x + k - stride), y, this->width(), this->height())) * horizontal.get(k));
				}
				
				m_buffer[this->width() * y + x] = result.getPixel();
			}
		}
		
		for (int y = 0; y < this->height(); y++) {//y of the matrix
			for (int x = 0; x < this->width(); x++) {//x of the matrix
				result.reset();
				
				for (int l = 0; l < vertical.getSideSize(); l++) { //y of the filter
					result += (KernelPixel(Utils::mirrorGet(m_buffer, x, (y + l - stride), this->width(), this->height())) * vertical.get(l));
				}
				
				m_array[this->width() * y + x] = result.getPixel();
			}
		}
	}
	
	/*
Multi-Threaded implementation of the Convolution between the Current RGBImage and a Separable Kernel (Split in the two Components)
NOTES:
1) Vertical component goes first
2) Horizontal component goes second
*/
	void multiThread_1D_Separable_Convolution(SeparableFilter& vertical, SeparableFilter& horizontal,
											  uint8_t passes = 1,
											  uint8_t thread_n = 4) {
		uint32_t first_h;
		uint32_t other_h;
		
		first_h = (this->h / thread_n) + (this->h % thread_n);
		other_h = this->h / thread_n;
		
		safe_array<std::thread> threads(thread_n);
		for (size_t j = 0; j < passes; j++)
		{
			for (size_t i = 0; i < thread_n; i++)
			{
				threads[i] = std::thread(RGBImage::separable_thread_operation_h, &horizontal, (i > 0 ? first_h + (i - 1) * other_h : 0), first_h + i * other_h, this);
			}
			
			for (size_t i = 0; i < thread_n; i++)
			{
				threads[i].join();
			}
			
			for (size_t i = 0; i < thread_n; i++)
			{
				threads[i] = std::thread(RGBImage::separable_thread_operation_v, &vertical, (i > 0 ? first_h + (i - 1) * other_h : 0), first_h + i * other_h, this);
			}
			
			for (size_t i = 0; i < thread_n; i++)
			{
				threads[i].join();
			}
		}
	}
	
	void multiThread_1D_Convolution(Filter& filter, uint8_t passes = 1, uint8_t thread_n = 4) {
		uint32_t first_h;
		uint32_t other_h;
		
		first_h = (this->h / thread_n) + (this->h % thread_n);
		other_h = this->h / thread_n;
		
		safe_array<std::thread> threads(thread_n);
		for (size_t j = 0; j < passes; j++)
		{
			for (size_t i = 0; i < thread_n; i++)
			{
				threads[i] = std::thread(RGBImage::non_separable_thread_operation, &filter, (i > 0 ? first_h + (i - 1) * other_h : 0), first_h + i * other_h, this);
			}
			
			for (size_t i = 0; i < thread_n; i++)
			{
				threads[i].join();
			}
		}
		m_array.swapArray(m_buffer, m_array.size);
	}
	
	void multiThread_3D_Convolution(Filter& filter, uint8_t passes = 1, uint8_t thread_n = 4) {
		if (!filter.is_three_channel())
			return;
		uint32_t first_h;
		uint32_t other_h;
		
		first_h = (this->h / thread_n) + (this->h % thread_n);
		other_h = this->h / thread_n;
		
		safe_array<std::thread> threads(thread_n);
		for (size_t j = 0; j < passes; j++)
		{
			for (size_t i = 0; i < thread_n; i++)
			{
				threads[i] = std::thread(RGBImage::non_separable_thread_operation3D, &filter, (i > 0 ? first_h + (i - 1) * other_h : 0), first_h + i * other_h, this);
			}
			
			for (size_t i = 0; i < thread_n; i++)
			{
				threads[i].join();
			}
		}
		m_array.swapArray(m_buffer, m_array.size);
	}
	
	void CUDA_Accelerated_1D_Convolution(Filter& kernel, uint8_t passes = 1, uint8_t thread_n = 4)
	{
		if (cuda_support) {
			if (!(GPU_utils::gpuConvolve(kernel.getFloatArray(), (uint8_t**)m_array.getPointerToArray(), kernel.getSideSize(), kernel.getSideSize(), this->width(), this->height(), passes))) {
				std::cout << "\n\n!!!CUDA support not found in host environment. CPU-only multi-threaded convolution will be used from now on!!!\n\n";
				cuda_support = false;
			}
		}
		else {
			multiThread_1D_Convolution(kernel, passes, thread_n);
		}
	}
	
	public:
	RGBImage() {
	};
	
	RGBImage(const char* filepath) :m_name(filepath) {
	};
	
	RGBImage(const RGBImage& img) {
		w = img.width();
		h = img.height();
		m_array.constructArray(w * h);
		m_name = img.getName();
		memcpy(m_array.getArray(), img.getPixelArray(), sizeof(Pixel) * w * h);
		m_buffer = new Pixel[m_array.size];
	}
	
	RGBImage(size_t w_, size_t h_) :w(w_), h(h_) {
		m_array.constructArray(w * h);
		m_name = "";
	};
	
	RGBImage(HSLImage &hsl)
	{
		this->set(hsl);
	}
	
	~RGBImage()
	{
		delete[] m_buffer;
	}
	
	void set(HSLImage& hsl);
	
	bool hasCudaSupport() const { return cuda_support; }
	
	Pixel** getPointerToArray()
	{
		return m_array.getPointerToArray();
	}
	
	void setCUDASupport(bool val) {
		cuda_support = val;
	}
	
	RGBImage& operator= (const RGBImage& img) {
		w = img.width();
		h = img.height();
		m_array.constructArray(w * h);
		m_name = img.getName();
		memcpy(m_array.getArray(), img.getPixelArray(), sizeof(Pixel) * w * h);
		if (!m_buffer)		m_buffer = new Pixel[m_array.size];
		return *this;
	}
	
	RGBImage& operator+=(const RGBImage& rhs) {
		REQUIRE(this->w == rhs.width() || this->h == rhs.height(), "Image sizes do not match!");
		
		if (AVX_SUPPORT)
		{
			uint32_t remainder = this->w * this->h % 8;
			
			for (int i = 0; i < this->w * this->h; i += 8) {
				__m256i first = _mm256_loadu_si256((__m256i*)(this->getPixelArray() + i));
				__m256i second = _mm256_loadu_si256((__m256i*)(rhs.getPixelArray() + i));
				first = _mm256_adds_epu8(first, second);
				_mm256_storeu_si256((__m256i*)(this->getPixelArray() + i), first);
			}
			
			for (int i = 0; i < remainder; i++) {
				this->getPixelArray()[this->w * this->h - 1 - i] += rhs.getPixelArray()[this->w*this->h - 1 - i];
			}
		}
		else if (SSE_SUPPORT)
		{
			uint32_t remainder = this->w * this->h % 4;
			
			for (int i = 0; i < this->w * this->h; i += 4) {
				__m128i first = _mm_loadu_si128((__m128i*)(this->getPixelArray() + i));
				__m128i second = _mm_loadu_si128((__m128i*)(rhs.getPixelArray() + i));
				first = _mm_adds_epu8(first, second);
				_mm_storeu_si128((__m128i*)(this->getPixelArray() + i), first);
			}
			
			for (int i = 0; i < remainder; i++) {
				this->getPixelArray()[this->w * this->h - 1 - i] += rhs.getPixelArray()[this->w*this->h - 1 - i];
			}
		}
		else {
			for (size_t i = 0; i < this->w * this->h; i++)
			{
				this->getPixelArray()[i] += rhs.getPixelArray()[i];
			}
		}
		return *this;
	}
	
	RGBImage operator+(const RGBImage& rhs) {
		RGBImage res(*this);
		res += rhs;
		return res;
	}
	
	Pixel& getPixel(uint32_t x, uint32_t y)
	{
		return m_array[y * w + x];
	}
	
	RGBImage& operator*=(const float value) {
		for (size_t i = 0; i < this->w * this->h; i++)
		{
			this->getPixelArray()[i] *= value;
		}
		return *this;
	}
	
	RGBImage operator*(const float value) {
		RGBImage res(*this);
		res *= value;
		return res;
	}
	
	Pixel* getPixelArray() const {
		return m_array.getArray();
	}
	
	Pixel* getBuffer() const
	{
		return m_buffer;
	}
	
	void swapBuffer()
	{
		Pixel* temp = m_buffer;
		m_buffer = m_array.getArray();
		*m_array.getPointerToArray() = temp;
	}
	
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
	
	/*
	Loads a PPM Image from a filepath
	*/
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
			m_array.constructArray(w * h); //array is RGBA for each pixel
			
			//BUFFER CONSTRUCTION NEEDS THE SIZE
			m_buffer = new Pixel[m_array.size];
			
			int counter = 0;
			
			if (type_string == "P1" || type_string == "P4") {
				while (!file.eof() && counter < m_array.size) {
					file >> temp;
					bit = std::stoi(temp);
					m_array[counter] = Pixel(255 * bit, 255 * bit, 255 * bit, 255);
					counter++;
				}
			}
			else if (type_string == "P2" || type_string == "P5") {
				file >> temp; //discard max val
				
				while (!file.eof() && counter < m_array.size) {
					file >> temp;
					bit = std::stoi(temp);
					m_array[counter] = Pixel(bit, bit, bit, 255);
					counter++;
				}
			}
			else if (type_string == "P3" || type_string == "P6") {
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
	
	// TODO(Sawii00): Implement PNG Decoding
	int loadPNG(const char* filepath);
	
	/*@TODO:
		- implement decompression wherever it applies
	*/
	
	/*
	Loads a BMP from a filepath
	NOTES:
	1) Compression not yet implemented
	2) Bitmasks not yet implemented
	*/
	int loadBMP(const char* filepath) {
		m_name = filepath;
		std::ifstream file(filepath, std::ios::binary);
		
		if (!file.is_open())
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
		
		m_array.constructArray(bit_info.height * bit_info.width);
		
		//BUFFER CREATIONS REQUIRES WIDTH AND HEIGHT AND CANNOT BE DONE IN THE CONSTRUCTOR
		m_buffer = new Pixel[m_array.size];
		
		safe_array<Pixel> palette;
		
		if (bit_info.compression > 0) //We do not support crazy compression methods eg huffman
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
			uint16_t palette_number = bit_info.number_of_color_in_palette == 0 ? Utils::power(2, bit_info.bits_per_pixel) : bit_info.number_of_color_in_palette;
			palette.constructArray(palette_number);
			
			file.read((char*)palette.getArray(), palette_number * 4);
			for (int i = 0; i < palette_number; i++) {
				uint8_t b = palette[i].b;
				palette[i].b = palette[i].r;
				palette[i].r = b;
				palette[i].a = 0xFF;
			}
			
			file.seekg(header.image_starting_offset, file.beg);
			
			// TODO(Sawii00): CHECK AND DEBUG DECOMPRESSION
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
					m_array[bit_info.width * (bit_info.height - 1 - i) + j / bit_info.bits_per_pixel] = palette[el];
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
					
					m_array[bit_info.width * (bit_info.height - 1 - i) + j / 16] = Pixel((el >> 0xA & 0x1F) * 8, (el >> 0x5 & 0x1F) * 8, (el & 0x1F) * 8, 0xFF);
				}
			}
		}
		
		else if (bit_info.bits_per_pixel == 24)
		{
			file.seekg(header.image_starting_offset, file.beg);
			
			//no palette present for bpp > 8
			for (int i = 0; i < bit_info.height; i++) {
				file.read((char*)row.getArray(), bytes_per_row);
				//........rrrrrrrrggggggggbbbbbbbb
				for (uint64_t j = 0; j < effective_width_bits; j += 24) {
					uint32_t el = *(uint32_t*)(row.getArray() + j / 8);
					m_array[bit_info.width * (bit_info.height - 1 - i) + j / 24] = Pixel((el >> 0x10 & 0xFF), (el >> 0x8 & 0xFF), (el & 0xFF), 0xFF);
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
					m_array[bit_info.width * (bit_info.height - 1 - i) + j / 32] = Pixel((el >> 0x10 & 0xFF), (el >> 0x8 & 0xFF), (el & 0xFF), 0xFF);
				}
			}
		}
		
		else {
			file.close();
			return 0;
		}
		file.close();
		
		return 1;
	}
	
	/*
	Saves current RGBImage as BMP to the given filepath
	*/
	int saveBMP(const char* filepath) {
		BMPHeader h;
		h.res1 = 0;
		h.res2 = 0;
		h.image_starting_offset = 54;
		BITMAPINFOHEADER info_h;
		info_h.header_size = 40;
		info_h.width = this->w;
		info_h.height = this->h;
		info_h.color_planes = 1;
		info_h.bits_per_pixel = 24;
		info_h.compression = 0;
		info_h.horizontal_resolution = 0;
		info_h.vertical_resolution = 0; //NO CLUE IF IT IMPACTS THE IMAGE
		info_h.number_of_color_in_palette = 0;
		info_h.number_of_important_colors = 0;
		
		uint8_t padding = (3 * info_h.width) % 4 ? 4 - (3 * info_h.width) % 4 : 0;
		
		info_h.final_image_size = info_h.height * (info_h.width * 3 + padding);
		h.file_size = info_h.final_image_size + 54;
		
		std::ofstream outfile(filepath);
		if (!outfile.is_open())
			return 0;
		
		//header are written out first
		outfile.put(0x42);
		outfile.put(0x4D);
		outfile.write((char*)&h, 12);
		outfile.write((char*)&info_h, info_h.header_size);
		
		char pad_val = 0x0;
		
		for (int i = 0; i < info_h.height; i++) {
			for (int j = 0; j < info_h.width; j++) {
				Pixel val = m_array[info_h.width * (info_h.height - 1 - i) + j];
				outfile.put(val.b);
				outfile.put(val.g);
				outfile.put(val.r);
			}
			outfile.write(&pad_val, padding);
		}
		
		outfile.close();
	}
	
	/*
	Displays the current RGBImage through SFML
	*/
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
	
	void convolve(Filter& filter, uint8_t passes = 1, uint8_t thread_number = 4) {
		if (!(filter.getSideSize() & 1)) {
			std::cout << "\n!!Wrong filter dimensions: it must be square and odd!!\n";
			return;
		}
		
		if (filter.is_three_channel())
		{
			multiThread_3D_Convolution(filter, passes, thread_number);
		}
		else
		{
			if (cuda_support)
			{
				CUDA_Accelerated_1D_Convolution(filter, passes, thread_number);
			}
			else
			{
				multiThread_1D_Convolution(filter, passes, thread_number);
			}
		}
	}
	
	void convolve(SeparableFilter& vertical, SeparableFilter& horizontal, uint8_t passes = 1, uint8_t thread_number = 4) {
		if (vertical.getSideSize() != horizontal.getSideSize() || !(vertical.getSideSize() & 0x1))
		{
			std::cout << "\n!!Wrong filter dimensions: separable filters must match dimensions and odd!!\n";
			return;
		}
		multiThread_1D_Separable_Convolution(vertical, horizontal, passes, thread_number);
	}
	
	HSLImage toHSL();
};

/*
 TODO(Sawii00): Comment this up

 */

class HSLImage
{
	private:
	safe_array<HSL_Pixel> m_array;
	size_t w = 0;
	size_t h = 0;
	const char* m_name = nullptr;
	bool cuda_support = true;
	HSL_Pixel* m_buffer = nullptr;
	
	public:
	
	HSLImage() {};
	
	HSLImage(size_t w_, size_t h_) :w(w_), h(h_) {
		m_array.constructArray(w * h);
		m_name = "";
	};
	
	HSLImage(const HSLImage& img) {
		w = img.width();
		h = img.height();
		m_array.constructArray(w * h);
		m_name = img.getName();
		memcpy(m_array.getArray(), img.getPixelArray(), sizeof(Pixel) * w * h);
		m_buffer = new HSL_Pixel[w * h];
	}
	
	HSLImage(RGBImage& rgb_img) {
		this->set(rgb_img);
	}
	
	~HSLImage()
	{
		delete[] m_buffer;
	}
	
	void set(RGBImage& rgb_img)
	{
		
		if (rgb_img.height() <= 0 || rgb_img.width() <= 0) return;
		if (m_array.isNullPointer() || this->w != rgb_img.width() || this->h != rgb_img.height())
		{
			this->w = rgb_img.width();
			this->h = rgb_img.height();
			
			if (!m_array.isNullPointer())
				m_array.free();
			m_array.constructArray(this->w * this->h);
			m_name = rgb_img.getName();
			if (m_buffer)
				delete[] m_buffer;
			m_buffer = new HSL_Pixel[this->w * this->h];
		}
		
		
		if (rgb_img.hasCudaSupport())
		{
			if (!(GPU_utils::gpuRGBtoHSLImage((uint8_t**)rgb_img.getPointerToArray(), (uint8_t**)m_array.getPointerToArray(), w, h)))
			{
				for (size_t i = 0; i < w * h; i++)
				{
					m_array[i].set(rgb_img.getPixelArray()[i]);
				}
			}
		}
		else
		{
			for (size_t i = 0; i < w * h; i++)
			{
				m_array[i].set(rgb_img.getPixelArray()[i]);
			}
		}
	}
	
	HSL_Pixel* getPixelArray() const {
		return m_array.getArray();
	}
	
	HSL_Pixel** getPointerToArray()
	{
		return m_array.getPointerToArray();
	}
	
	HSL_Pixel& getPixel(uint32_t x, uint32_t y)
	{
		return m_array[y * w + x];
	}
	
	
	HSL_Pixel* getBuffer() const
	{
		return m_buffer;
	}
	
	
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
	
	void swapBuffer()
	{
		HSL_Pixel* temp = m_buffer;
		m_buffer = m_array.getArray();
		*m_array.getPointerToArray() = temp;
	}
	
	void displayImage() {
		RGBImage img = this->toRGB();
		img.displayImage();
	}
	
	RGBImage toRGB()
	{
		RGBImage res(*this);
		return res;
	}
};