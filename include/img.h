#pragma once
#include <cstdint>
#include "safe_array.h"
#include "pixel.h"
#include "color.h"
#include "GPU_utils.h"



enum ImageType
{
	RGB = 1, HSL = 2, GRAY = 4
};


class Img {
	
	private:
	uint8_t m_type = 0;
	bool m_cuda_support = true;
	
	size_t m_width = 0;
	size_t m_height = 0;
	
	bool m_empty = true;
	
	safe_array<RGBPixel> m_rgb_layer;
	safe_array<HSLPixel> m_hsl_layer;
	safe_array<GRAYPixel> m_gray_layer;
	
	RGBPixel* m_rgb_buffer = nullptr;
	HSLPixel* m_hsl_buffer = nullptr;
	GRAYPixel* m_gray_buffer = nullptr;
	
	void setType(ImageType type);
	
	
	
	public:
	bool isRGB() const;
	bool isHSL() const;
	bool isGRAY() const;
	ImageType getType() const;
	bool isEmpty() const;
	
	
	Img() = default;
	~Img();
	Img(const Img& img);
	Img(size_t width, size_t height, ImageType image_type, Color color = Color::BLACK);
	Img(size_t width, size_t height, ImageType image_type);
		
	
	Img& operator= (const Img& img);
	Img& operator+= (const Img& img);
	Img operator+ (const Img& img);
	Img& operator-= (const Img& img);
	Img operator- (const Img& img);
	Img& operator*= (const float val);
	Img operator* (const float val);
	Img& operator/= (const float val);
	Img operator/ (const float val);
	
	
	uint8_t** getPointerToArray();

	
	uint8_t* getArray();
	const uint8_t* getArray() const;
	bool hasCudaSupport() const { return m_cuda_support; }
	bool hasCudaSupport(){ return m_cuda_support; }
	void swapBuffer();
	uint8_t* getBuffer();
	size_t height() const;
	size_t width() const;
	void clear();
	void build(size_t width, size_t height, ImageType image_type);

	void toHSL();
	void toRGB();
	void toGRAY();
	


	template <class T>
	T& getPixel(size_t x, size_t y)
	{
		switch (sizeof(T))
		{
		case 4:
		{
			return (T&)m_rgb_layer[y * m_width + x];
		}
		case 10:
		{
			return (T&)m_hsl_layer[y * m_width + x];
		}
		case 1:
		{
			return (T&)m_gray_layer[y * m_width + x];
		}
		default:
		{
			throw "Undefined ColorPlane";
		}
		}
	}

	
};

