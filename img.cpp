#include "img.h"


inline void Img::setType(ImageType type)
{
	m_type = 0;
	m_type | type;
}

inline bool Img::isRGB() const
{
	return m_type & ImageType::RGB;
}

inline bool Img::isHSL() const
{
	return m_type & ImageType::HSL;
}

inline bool Img::isGRAY() const
{
	return m_type & ImageType::GRAY;
}

inline bool Img::isEmpty() const
{
	return m_empty;
}

inline bool Img::isBINARY() const
{
	return m_type & ImageType::BINARY;
}

inline ImageType Img::getType() const
{
	if (isRGB())
		return RGB;
	else if (isHSL())
		return HSL;
	else if(isGRAY())
		return GRAY;
	else if(isBINARY())
		return BINARY;
	else
		return DEFAULT;
}


void Img::buildRGB() {
	if (m_empty) return;
	if (!m_rgb_layer.alreadyInitialized()) m_rgb_layer.constructArray(m_width * m_height);
	if (!m_rgb_buffer) m_rgb_buffer = new RGBPixel[m_width * m_height];
}
void Img::buildHSL() {
	if (m_empty) return;
	if (!m_hsl_layer.alreadyInitialized()) m_hsl_layer.constructArray(m_width * m_height);
	if (!m_hsl_buffer) m_hsl_buffer = new HSLPixel[m_width * m_height];
}
void Img::buildGRAY() {
	if (m_empty) return;
	if (!m_gray_layer.alreadyInitialized()) m_gray_layer.constructArray(m_width * m_height);
	if (!m_gray_buffer) m_gray_buffer = new GRAYPixel[m_width * m_height];
}

void Img::freeRGB() {
	if (m_empty) return;
	m_rgb_layer.free();
	if (m_rgb_buffer) {
		delete[] m_rgb_buffer;
		m_rgb_buffer = nullptr;
	}
}
void Img::freeHSL() {
	if (m_empty) return;
	m_hsl_layer.free();
	if (m_hsl_buffer) {
		delete[] m_hsl_buffer;
		m_hsl_buffer = nullptr;
	}
}
void Img::freeGRAY() {
	if (m_empty) return;
	m_gray_layer.free();
	if (m_gray_buffer) {
		delete[] m_gray_buffer;
		m_gray_buffer = nullptr;
	}
}

Img::Img(const Img& img) : m_type(img.getType()), m_width(img.width()), m_height(img.height()) {
	if (img.isEmpty())
		throw ("Cannot use empty image as constructor parameter");
	
	m_empty = false;
	switch (m_type) {
		case (ImageType::RGB):
		buildRGB();
		memcpy(m_rgb_layer.getArray(), img.getArray<RGBPixel>(), sizeof(RGBPixel) * m_width * m_height);
		break;
		
		case (ImageType::HSL):
		buildHSL();
		memcpy(m_hsl_layer.getArray(), img.getArray<HSLPixel>(), sizeof(HSLPixel) * m_width * m_height);
		break;
		
		case (ImageType::GRAY):
		buildGRAY();
		memcpy(m_gray_layer.getArray(), img.getArray<GRAYPixel>(), sizeof(GRAYPixel) * m_width * m_height);
		break;
		
		case (ImageType::BINARY):
		buildGRAY();
		memcpy(m_gray_layer.getArray(), img.getArray<GRAYPixel>(), sizeof(GRAYPixel) * m_width * m_height);
		break;
		
		default: throw "Invalid Color Plane";
	}
}
Img::Img(size_t width, size_t height, ImageType image_type, Color color) :
m_type(image_type), m_width(width), m_height(height)
{
	m_empty = false;
	switch (m_type) {
		case (ImageType::RGB): {
			buildRGB();
			RGBPixel* array_loc = m_rgb_layer.getArray();
			for (size_t i = 0; i < m_rgb_layer.size; i++)
				memcpy(array_loc + i, rgb_colors + color, sizeof(RGBPixel));
			break;
		}
		
		case (ImageType::HSL): {
			buildHSL();
			HSLPixel* array_loc = m_hsl_layer.getArray();
			for (size_t i = 0; i < m_hsl_layer.size; i++)
				memcpy(array_loc + i, hsl_colors + color, sizeof(HSLPixel));
			break;
		}
		case (ImageType::BINARY):
		case (ImageType::GRAY): {
			buildGRAY();
			GRAYPixel black_pixel(0);
			GRAYPixel* array_loc = m_gray_layer.getArray();
			for (size_t i = 0; i < m_hsl_layer.size; i++)
				memcpy(array_loc + i, &black_pixel, sizeof(GRAYPixel));
			break;
		}
		
		
		
		default: throw "Invalid Color Plane";
	}
}

Img::Img(size_t width, size_t height, ImageType image_type) :
m_type(image_type), m_width(width), m_height(height)
{
	m_empty = false;
	switch (m_type) {
		case (ImageType::RGB):
		buildRGB();
		break;
		
		case (ImageType::HSL):
		buildHSL();
		break;
		
		case (ImageType::GRAY):
		buildGRAY();
		break;
		
		case (ImageType::BINARY):
		buildGRAY();
		break;
		
		default: throw "Invalid Color Plane";
	}
}

Img::~Img()
{
	if (m_rgb_buffer)
		delete[] m_rgb_buffer;
	if (m_hsl_buffer)
		delete[] m_hsl_buffer;
	if (m_gray_buffer)
		delete[] m_gray_buffer;
}

Img& Img::operator= (const Img& img) {
	if (img.isEmpty())
		return (*this);
	
	m_empty = false;
	
	if (m_type == img.getType() && m_width == img.width() && m_height == img.height()) {
		switch (m_type) {
			case (RGB):
			memcpy(m_rgb_layer.getArray(), img.getArray<RGBPixel>(), sizeof(RGBPixel) * m_width * m_height);
			break;
			case (HSL):
			memcpy(m_hsl_layer.getArray(), img.getArray<HSLPixel>(), sizeof(HSLPixel) * m_width * m_height);
			break;
			case (GRAY):
			memcpy(m_gray_layer.getArray(), img.getArray<GRAYPixel>(), sizeof(GRAYPixel) * m_width * m_height);
			break;
			case (BINARY):
			memcpy(m_gray_layer.getArray(), img.getArray<GRAYPixel>(), sizeof(GRAYPixel) * m_width * m_height);
			break;
			default:
			break;
		}
		return (*this);
	}
	
	freeRGB();
	freeHSL();
	freeGRAY();
	m_type = img.getType();
	m_width = img.width();
	m_height = img.height();
	
	switch (img.getType()) {
		case (RGB):
		buildRGB();
		memcpy(m_rgb_layer.getArray(), img.getArray<RGBPixel>(), sizeof(RGBPixel) * m_width * m_height);
		break;
		case (HSL):
		buildHSL();
		memcpy(m_hsl_layer.getArray(), img.getArray<HSLPixel>(), sizeof(HSLPixel) * m_width * m_height);
		break;
		case (GRAY):
		buildGRAY();
		memcpy(m_gray_layer.getArray(), img.getArray<GRAYPixel>(), sizeof(GRAYPixel) * m_width * m_height);
		break;
		case (BINARY):
		buildGRAY();
		memcpy(m_gray_layer.getArray(), img.getArray<GRAYPixel>(), sizeof(GRAYPixel) * m_width * m_height);
		break;
		default:
		break;
	}
	return (*this);
}

Img& Img::operator+= (const Img& img)
{
	if (this->m_width != img.width() || this->m_height != img.height())
	{
		throw "Image Sizes do not match";
	}
	
	if (this->m_type != img.m_type)
	{
		throw "Incompatible Types";
	}
	//TODO: IMPLEMENT GRAY SUM VIA getPointerToArray()
	
	if (m_type == ImageType::HSL)
	{
		for (int i = 0; i < m_hsl_layer.getSize(); i++) {
			m_hsl_layer[i] += img.getArray<HSLPixel>()[i];
		}
	}
	else
	{
		uint32_t remainder = this->m_width * this->m_height % 8;
		
		for (int i = 0; i < this->m_width * this->m_height; i += 8) {
			__m256i first = _mm256_loadu_si256((__m256i*)(this->getArray<RGBPixel>() + i));
			__m256i second = _mm256_loadu_si256((__m256i*)(img.getArray<RGBPixel>() + i));
			first = _mm256_adds_epu8(first, second);
			_mm256_storeu_si256((__m256i*)(this->getArray<RGBPixel>() + i), first);
		}
		
		for (int i = 0; i < remainder; i++) {
			this->getArray<RGBPixel>()[this->m_width * this->m_height - 1 - i] += img.getArray<RGBPixel>()[this->m_width * this->m_height - 1 - i];
		}
	}
	return *this;
}

Img Img::operator+ (const Img& img)
{
	Img res(*this);
	res += img;
	return res;
}

Img& Img::operator-= (const Img& img)
{
	if (this->m_width != img.width() || this->m_height != img.height())
	{
		throw "Image Sizes do not match";
	}
	
	if (this->m_type != img.m_type)
	{
		throw "Incompatible Types";
	}
	
	//TODO: IMPLEMENT GRAY SUB VIA getPointerToArray()
	
	if (m_type == ImageType::HSL)
	{
		for (int i = 0; i < m_hsl_layer.getSize(); i++) {
			m_hsl_layer[i] -= (HSLPixel)img.getArray<HSLPixel>()[i];
		}
	}
	else
	{
		uint32_t remainder = this->m_width * this->m_height % 8;
		
		for (int i = 0; i < this->m_width * this->m_height; i += 8) {
			__m256i first = _mm256_loadu_si256((__m256i*)(this->getArray<RGBPixel>() + i));
			__m256i second = _mm256_loadu_si256((__m256i*)(img.getArray<RGBPixel>() + i));
			first = _mm256_subs_epu8(first, second);
			_mm256_storeu_si256((__m256i*)(this->getArray<RGBPixel>() + i), first);
		}
		
		for (int i = 0; i < remainder; i++) {
			this->getArray<RGBPixel>()[this->m_width * this->m_height - 1 - i] -= img.getArray<RGBPixel>()[this->m_width * this->m_height - 1 - i];
		}
	}
	return *this;
}

Img Img::operator- (const Img& img)
{
	Img res(*this);
	res -= img;
	return res;
}

Img& Img::operator*= (const float val)
{
	
	switch (m_type)
	{
		case ImageType::RGB:
		{
			for(int i = 0; i < m_rgb_layer.getSize(); i++)
			{
				m_rgb_layer[i] *= val;
			}
		}break;
		
		case ImageType::HSL:
		{
			for(int i = 0; i < m_hsl_layer.getSize(); i++)
			{
				m_hsl_layer[i] *= val;
			}
		}break;
		
		case ImageType::BINARY:
		case ImageType::GRAY:
		{
			for(int i = 0; i < m_gray_layer.getSize(); i++)
			{
				m_gray_layer[i] *= val;
			}
		}break;
		
		default:
		{
			throw "Invalid Color Plane";
		}
	}
	return *this;
	
}

Img Img::operator* (const float val)
{
	Img copy(*this);
	copy*=val;
	return copy;
}

Img& Img::operator/= (const float val)
{
	(*this)*=(1.0f/val);
	return *this;
}

Img Img::operator/ (const float val)
{
	Img copy(*this);
	copy/=val;
	return copy;
}

uint8_t** Img::getPointerToArray()
{
	switch (m_type)
	{
		case ImageType::RGB:
		{
			return (uint8_t**)m_rgb_layer.getPointerToArray();
		}
		case ImageType::HSL:
		{
			return (uint8_t**)m_hsl_layer.getPointerToArray();
			
		}
		case ImageType::BINARY:
		case ImageType::GRAY:
		{
			return (uint8_t**)m_gray_layer.getPointerToArray();
		}
		default:
		{
			return nullptr;
		}
	}
}

/*

const uint8_t* Img::getArray() const
{
	switch (m_type)
	{
		case ImageType::RGB:
		{
			return (uint8_t*)m_rgb_layer.getArray();
		}
		case ImageType::HSL:
		{
			return (uint8_t*)m_hsl_layer.getArray();
			
		}
		case ImageType::GRAY:
		{
			return (uint8_t*)m_gray_layer.getArray();
		}
		default:
		{
			return nullptr;
		}
		
	}
}
 */

uint8_t* Img::getBuffer()
{
	switch (m_type)
	{
		case ImageType::RGB:
		{
			return (uint8_t*)m_rgb_buffer;
		}
		case ImageType::HSL:
		{
			return (uint8_t*)m_hsl_buffer;
		}
		case ImageType::GRAY:
		{
			return (uint8_t*)m_gray_buffer;
		}
		case ImageType::BINARY:
		{
			return (uint8_t*)m_gray_buffer;
		}
		default:
		{
			return nullptr;
		}
	}
}


void Img::swapBuffer()
{
	switch(m_type)
	{
		case ImageType::RGB:
		{
			RGBPixel* temp = m_rgb_buffer;
			m_rgb_buffer = m_rgb_layer.getArray();
			*m_rgb_layer.getPointerToArray() = temp;
		}break;
		
		case ImageType::HSL:
		{
			HSLPixel* temp = m_hsl_buffer;
			m_hsl_buffer = m_hsl_layer.getArray();
			*m_hsl_layer.getPointerToArray() = temp;
		}break;
		
		case ImageType::BINARY:
		case ImageType::GRAY:
		{
			GRAYPixel* temp = m_gray_buffer;
			m_gray_buffer = m_gray_layer.getArray();
			*m_gray_layer.getPointerToArray() = temp;
		}break;
		
		default:
		{
			return;
		}
	}
}

inline size_t Img::height() const
{
	return this->m_height;
}

inline size_t Img::width() const
{
	return this->m_width;
}

void Img::clear()
{
	
	switch (m_type)
	{
		
		case ImageType::RGB:
		{
			memset(m_rgb_layer.getArray(), 0x00, sizeof(RGBPixel)*m_rgb_layer.getSize());
		}
		case ImageType::HSL:
		{
			memset(m_hsl_layer.getArray(), 0x00, sizeof(HSLPixel)*m_hsl_layer.getSize());
		}
		case ImageType::BINARY:
		case ImageType::GRAY:
		{
			memset(m_gray_layer.getArray(), 0x00, sizeof(GRAYPixel)*m_gray_layer.getSize());
		}
		default:
		{
			throw "Invalid Color Plane";
		}
	};
	
	
}

void Img::build(size_t width, size_t height, ImageType image_type)
{
	if (!m_empty) {
		return;
	}
	m_empty = false;
	m_type = image_type;
	m_width = width;
	m_height = height;
	switch (m_type) {
		case (ImageType::RGB):
		buildRGB();
		break;
		
		case (ImageType::HSL):
		buildHSL();
		break;
		
		case (ImageType::GRAY):
		buildGRAY();
		break;
		case (ImageType::BINARY):
		buildGRAY();
		break;
		default: throw "Invalid Color Plane";
	}
}

void Img::toRGB() {
	if (m_empty) return;
	switch (m_type) {
		case (ImageType::RGB):
		break;
		case (ImageType::GRAY):
		buildRGB();
		for (size_t i = 0; i < m_width * m_height; i++)
		{
			this->m_rgb_layer[i].set(m_gray_layer[i].val);
		}
		freeGRAY();
		m_type = ImageType::RGB;
		break;
		case (ImageType::BINARY):
		buildRGB();
		for (size_t i = 0; i < m_width * m_height; i++)
		{
			this->m_rgb_layer[i].set(m_gray_layer[i].val);
		}
		freeGRAY();
		m_type = ImageType::RGB;
		break;
		case (ImageType::HSL):
		buildRGB();
		if (m_cuda_support) {
			if (!(GPU_utils::gpuHSLtoRGBImage(
											  (uint8_t**)m_hsl_layer.getPointerToArray(), (uint8_t**)m_rgb_layer.getPointerToArray(), m_width, m_height)))
			{
				m_cuda_support = false;
				for (size_t i = 0; i < m_width * m_height; i++)
				{
					this->m_rgb_layer[i].set(m_hsl_layer[i]);
				}
			}
		}
		else {
			for (size_t i = 0; i < m_width * m_height; i++)
			{
				this->m_rgb_layer[i].set(m_hsl_layer[i]);
			}
		}
		freeHSL();
		m_type = ImageType::RGB;
		break;
	}
}

void Img::toHSL() {
	if (m_empty) return;
	switch (m_type) {
		case (ImageType::RGB):
		buildHSL();
		if (m_cuda_support) {
			if (!(GPU_utils::gpuRGBtoHSLImage(
											  (uint8_t**)m_rgb_layer.getPointerToArray(), (uint8_t**)m_hsl_layer.getPointerToArray(), m_width, m_height)))
			{
				m_cuda_support = false;
				for (size_t i = 0; i < m_width * m_height; i++)
				{
					this->m_hsl_layer[i].set(m_rgb_layer[i]);
				}
			}
		}
		else {
			for (size_t i = 0; i < m_width * m_height; i++)
			{
				this->m_hsl_layer[i].set(m_rgb_layer[i]);
			}
		}
		freeRGB();
		m_type = ImageType::HSL;
		break;
		
		default:
		break;
	}
}

void Img::toGRAY() {
	if (m_empty) return;
	switch (m_type) {
		case (ImageType::HSL):
		buildGRAY();
		for (size_t i = 0; i < m_width * m_height; i++)
		{
			this->m_gray_layer[i].set(m_hsl_layer[i]);
		}
		freeHSL();
		m_type = ImageType::GRAY;
		break;
		case (ImageType::RGB):
		buildGRAY();
		for (size_t i = 0; i < m_width * m_height; i++)
		{
			this->m_gray_layer[i].set(m_rgb_layer[i]);
		}
		freeRGB();
		m_type = ImageType::GRAY;
		break;
		default:
		break;
	}
}