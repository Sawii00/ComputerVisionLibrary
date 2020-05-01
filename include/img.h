
#pragma once
#include <cstdint>
#include "safe_array.h"
#include "pixel.h"
#include "color.h"
#include "GPU_utils.h"
#include "drawing.h"

enum DrawingPositionMode;

enum ImageType
{
	DEFAULT = 0, RGB = 1, HSL = 2, GRAY = 4, BINARY = 8
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
	
	inline void setType(ImageType type);
	
	void buildRGB();
	void buildHSL();
	void buildGRAY();
	
	void freeRGB();
	void freeHSL();
	void freeGRAY();
	
	
	// NOTE(Sawii00): FRIENDS
	friend void threshold(Img& img, std::initializer_list<float>min, std::initializer_list<float>max);
	friend void drawRect(Img &img, size_t x, size_t y, size_t width, size_t height, Color color, DrawingPositionMode mode);
	friend void canny(Img& img, float min, float max);
	
	public:
	inline bool isRGB() const;
	inline bool isHSL() const;
	inline bool isGRAY() const;
	inline bool isBINARY() const;
	inline ImageType getType() const;
	inline bool isEmpty() const;
	
	
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
	
	
	inline bool hasCudaSupport() const { return m_cuda_support; }
	inline bool hasCudaSupport(){ return m_cuda_support; }
	inline void setCudaSupport(bool val){m_cuda_support = val;}
	void swapBuffer();
	uint8_t* getBuffer();
	inline size_t height() const;
	inline size_t width() const;
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
			case 12:
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
	
	template <class T>
		T* getArray()
	{
		switch (sizeof(T))
		{
			case 4:
			{
				return (T*)m_rgb_layer.getArray();
			}
			case 12:
			{
				return (T*)m_hsl_layer.getArray();
			}
			case 1:
			{
				return (T*)m_gray_layer.getArray();
			}
			default:
			{
				return nullptr;
			}
		}
	}
	
	template <class T>
		T* getBuffer()
	{
		switch (sizeof(T))
		{
			case 4:
			{
				return (T*)m_rgb_buffer;
			}
			case 12:
			{
				return (T*)m_hsl_buffer;
			}
			case 1:
			{
				return (T*)m_gray_buffer;
			}
			default:
			{
				return nullptr;
			}
		}
	}
	
	template <class T>
		const T* getArray() const
	{
		switch (sizeof(T))
		{
			case 4:
			{
				return (T*)m_rgb_layer.getArray();
			}
			case 12:
			{
				return (T*)m_hsl_layer.getArray();
			}
			case 1:
			{
				return (T*)m_gray_layer.getArray();
			}
			default:
			{
				return nullptr;
			}
		}
	}
	
	
};

