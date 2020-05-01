#include "convolution.h"

void Private::RGB_separable_thread_operation_h(SeparableFilter* horizontal, uint32_t start, uint32_t finish, Img* im) {
	//start included, finish excluded
	KernelPixel result;
	int stride = int(horizontal->getSideSize() / 2);
	
	auto pixel_ptr = im->getArray<RGBPixel>();
	auto buffer_ptr = im->getBuffer<RGBPixel>();
	
	for (int y = start; y < finish; y++) {//y of the matrix
		for (int x = 0; x < im->width(); x++) {//x of the matrix
			result.reset();
			
			for (int k = 0; k < horizontal->getSideSize(); k++) { //x of the filter
				result += (KernelPixel(Utils::mirrorGet(pixel_ptr, (x + k - stride), y, im->width(), im->height())) * horizontal->get(k));
			}
			
			buffer_ptr[im->width() * y + x] = result.getPixel();
		}
	}
}
void Private::RGB_separable_thread_operation_v(SeparableFilter* vertical, uint32_t start, uint32_t finish, Img* im) {
	KernelPixel result;
	int stride = int(vertical->getSideSize() / 2);
	
	auto pixel_ptr = im->getArray<RGBPixel>();
	auto buffer_ptr = im->getBuffer<RGBPixel>();
	
	for (int y = start; y < finish; y++) {//y of the matrix
		for (int x = 0; x < im->width(); x++) {//x of the matrix
			result.reset();
			
			for (int l = 0; l < vertical->getSideSize(); l++) { //y of the filter
				result += (KernelPixel(Utils::mirrorGet(buffer_ptr, x, (y + l - stride), im->width(), im->height())) * vertical->get(l));
			}
			
			pixel_ptr[im->width() * y + x] = result.getPixel();
		}
	}
}
void Private::RGB_non_separable_thread_operation(Filter* filter, uint32_t start, uint32_t finish, Img* im) {
	KernelPixel result;
	auto stride = int(filter->getSideSize() / 2);
	
	auto pixel_ptr = im->getArray<RGBPixel>();
	auto buffer_ptr = im->getBuffer<RGBPixel>();
	
	for (int y = start; y < finish; y++) {//y of the matrix
		for (int x = 0; x < im->width(); x++) {//x of the matrix
			result.reset();
			
			for (int l = 0; l < filter->getSideSize(); l++) { //y of the filter
				for (int k = 0; k < filter->getSideSize(); k++) { //x of the filter
					result += (KernelPixel(Utils::mirrorGet(pixel_ptr, (x + k - stride), (y + l - stride), im->width(), im->height())) * filter->get(l * filter->getSideSize() + k));
				}
			}
			
			buffer_ptr[im->width() * y + x] = result.getPixel();
		}
	}
}
void Private::RGB_non_separable_thread_operation3D(Filter* filter, uint32_t start, uint32_t finish, Img* im) {
	KernelPixel result;
	auto stride = int(filter->getSideSize() / 2);
	
	auto pixel_ptr = im->getArray<RGBPixel>();
	auto buffer_ptr = im->getBuffer<RGBPixel>();
	
	for (int y = start; y < finish; y++) {//y of the matrix
		for (int x = 0; x < im->width(); x++) {//x of the matrix
			result.reset();
			
			for (int l = 0; l < filter->getSideSize(); l++) { //y of the filter
				for (int k = 0; k < filter->getSideSize(); k++) { // x of the filter
					result += (KernelPixel(Utils::mirrorGet(pixel_ptr, (x + k - stride), (y + l - stride), im->width(), im->height())) * filter->getKP(l * filter->getSideSize() + k));
				}
			}
			
			buffer_ptr[im->width() * y + x] = result.getPixel();
		}
	}
}
void Private::RGB_multiThread_1D_Separable_Convolution(Img* img, SeparableFilter& vertical, SeparableFilter& horizontal, uint8_t passes, uint8_t thread_n) {
	if (!img->isRGB()) return;
	uint32_t first_h;
	uint32_t other_h;
	
	first_h = (img->height() / thread_n) + (img->height() % thread_n);
	other_h = img->height() / thread_n;
	
	safe_array<std::thread> threads(thread_n);
	for (size_t j = 0; j < passes; j++)
	{
		for (size_t i = 0; i < thread_n; i++)
		{
			threads[i] = std::thread(Private::RGB_separable_thread_operation_h, &horizontal, (i > 0 ? first_h + (i - 1) * other_h : 0), first_h + i * other_h, img);
		}
		
		for (size_t i = 0; i < thread_n; i++)
		{
			threads[i].join();
		}
		
		for (size_t i = 0; i < thread_n; i++)
		{
			threads[i] = std::thread(Private::RGB_separable_thread_operation_v, &vertical, (i > 0 ? first_h + (i - 1) * other_h : 0), first_h + i * other_h, img);
		}
		
		for (size_t i = 0; i < thread_n; i++)
		{
			threads[i].join();
		}
	}
}
void Private::RGB_multiThread_1D_Convolution(Img* img, Filter& filter, uint8_t passes, uint8_t thread_n) {
	if (!img->isRGB()) return;
	uint32_t first_h;
	uint32_t other_h;
	
	first_h = (img->height() / thread_n) + (img->height() % thread_n);
	other_h = img->height() / thread_n;
	
	safe_array<std::thread> threads(thread_n);
	for (size_t j = 0; j < passes; j++)
	{
		for (size_t i = 0; i < thread_n; i++)
		{
			threads[i] = std::thread(Private::RGB_non_separable_thread_operation, &filter, (i > 0 ? first_h + (i - 1) * other_h : 0), first_h + i * other_h, img);
		}
		
		for (size_t i = 0; i < thread_n; i++)
		{
			threads[i].join();
		}
	}
	img->swapBuffer();
}
void Private::RGB_multiThread_3D_Convolution(Img* img, Filter& filter, uint8_t passes, uint8_t thread_n) {
	if (!img->isRGB()) return;
	if (!filter.is_three_channel())
		return;
	uint32_t first_h;
	uint32_t other_h;
	
	first_h = (img->height() / thread_n) + (img->height() % thread_n);
	other_h = img->height() / thread_n;
	
	safe_array<std::thread> threads(thread_n);
	for (size_t j = 0; j < passes; j++)
	{
		for (size_t i = 0; i < thread_n; i++)
		{
			threads[i] = std::thread(Private::RGB_non_separable_thread_operation3D, &filter, (i > 0 ? first_h + (i - 1) * other_h : 0), first_h + i * other_h, img);
		}
		
		for (size_t i = 0; i < thread_n; i++)
		{
			threads[i].join();
		}
	}
	img->swapBuffer();
}
void Private::RGB_CUDA_Accelerated_1D_Convolution(Img* img, Filter& kernel, uint8_t passes, uint8_t thread_n) {
	if (!img->isRGB()) return;
	if (img->hasCudaSupport()) {
		if (!(GPU_utils::gpuConvolve(kernel.getFloatArray(), img->getPointerToArray(), kernel.getSideSize(), kernel.getSideSize(), img->width(), img->height(), passes))) {
			std::cout << "\n\n!!!CUDA support not found in host environment. CPU-only multi-threaded convolution will be used from now on!!!\n\n";
			img->setCudaSupport(false);
		}
	}
	else {
		Private::RGB_multiThread_1D_Convolution(img, kernel, passes, thread_n);
	}
}

void Private::GRAY_separable_thread_operation_h(SeparableFilter* horizontal, uint32_t start, uint32_t finish, Img* im)
{
	//start included, finish excluded
	GRAYKernelPixel result;
	int stride = int(horizontal->getSideSize() / 2);
	
	auto pixel_ptr = im->getArray<GRAYPixel>();
	auto buffer_ptr = im->getBuffer<GRAYPixel>();
	
	for (int y = start; y < finish; y++) {//y of the matrix
		for (int x = 0; x < im->width(); x++) {//x of the matrix
			result.reset();
			
			for (int k = 0; k < horizontal->getSideSize(); k++) { //x of the filter
				result += (GRAYKernelPixel(Utils::mirrorGet(pixel_ptr, (x + k - stride), y, im->width(), im->height())) * horizontal->get(k));
			}
			
			buffer_ptr[im->width() * y + x] = result.getPixel();
		}
	}
}

void Private::GRAY_separable_thread_operation_v(SeparableFilter* vertical, uint32_t start, uint32_t finish, Img* im)
{
	GRAYKernelPixel result;
	int stride = int(vertical->getSideSize() / 2);
	
	auto pixel_ptr = im->getArray<GRAYPixel>();
	auto buffer_ptr = im->getBuffer<GRAYPixel>();
	
	for (int y = start; y < finish; y++) {//y of the matrix
		for (int x = 0; x < im->width(); x++) {//x of the matrix
			result.reset();
			
			for (int l = 0; l < vertical->getSideSize(); l++) { //y of the filter
				result += (GRAYKernelPixel(Utils::mirrorGet(buffer_ptr, x, (y + l - stride), im->width(), im->height())) * vertical->get(l));
			}
			
			pixel_ptr[im->width() * y + x] = result.getPixel();
		}
	}
}

void Private::GRAY_non_separable_thread_operation(Filter* filter, uint32_t start, uint32_t finish, Img* im)
{
	GRAYKernelPixel result;
	auto stride = int(filter->getSideSize() / 2);
	
	auto pixel_ptr = im->getArray<GRAYPixel>();
	auto buffer_ptr = im->getBuffer<GRAYPixel>();
	
	for (int y = start; y < finish; y++) {//y of the matrix
		for (int x = 0; x < im->width(); x++) {//x of the matrix
			result.reset();
			
			for (int l = 0; l < filter->getSideSize(); l++) { //y of the filter
				for (int k = 0; k < filter->getSideSize(); k++) { //x of the filter
					result += (GRAYKernelPixel(Utils::mirrorGet(pixel_ptr, (x + k - stride), (y + l - stride), im->width(), im->height())) * filter->get(l * filter->getSideSize() + k));
				}
			}
			
			buffer_ptr[im->width() * y + x] = result.getPixel();
		}
	}
}

void Private::GRAY_multiThread_1D_Separable_Convolution(Img* img, SeparableFilter& vertical, SeparableFilter& horizontal, uint8_t passes, uint8_t thread_n)
{
	if (!(img->isGRAY() || img->isBINARY())) return;
	uint32_t first_h;
	uint32_t other_h;
	
	first_h = (img->height() / thread_n) + (img->height() % thread_n);
	other_h = img->height() / thread_n;
	
	safe_array<std::thread> threads(thread_n);
	for (size_t j = 0; j < passes; j++)
	{
		for (size_t i = 0; i < thread_n; i++)
		{
			threads[i] = std::thread(Private::GRAY_separable_thread_operation_h, &horizontal, (i > 0 ? first_h + (i - 1) * other_h : 0), first_h + i * other_h, img);
		}
		
		for (size_t i = 0; i < thread_n; i++)
		{
			threads[i].join();
		}
		
		for (size_t i = 0; i < thread_n; i++)
		{
			threads[i] = std::thread(Private::GRAY_separable_thread_operation_v, &vertical, (i > 0 ? first_h + (i - 1) * other_h : 0), first_h + i * other_h, img);
		}
		
		for (size_t i = 0; i < thread_n; i++)
		{
			threads[i].join();
		}
	}
}

void Private::GRAY_multiThread_1D_Convolution(Img* img, Filter& filter, uint8_t passes, uint8_t thread_n)
{
	if (!(img->isGRAY() || img->isBINARY())) return;
	uint32_t first_h;
	uint32_t other_h;
	
	first_h = (img->height() / thread_n) + (img->height() % thread_n);
	other_h = img->height() / thread_n;
	
	safe_array<std::thread> threads(thread_n);
	for (size_t j = 0; j < passes; j++)
	{
		for (size_t i = 0; i < thread_n; i++)
		{
			threads[i] = std::thread(Private::GRAY_non_separable_thread_operation, &filter, (i > 0 ? first_h + (i - 1) * other_h : 0), first_h + i * other_h, img);
		}
		
		for (size_t i = 0; i < thread_n; i++)
		{
			threads[i].join();
		}
	}
	img->swapBuffer();
}

void Private::GRAY_CUDA_Accelerated_1D_Convolution(Img* img, Filter& kernel, uint8_t passes, uint8_t thread_n) {
	if (!(img->isGRAY() || img->isBINARY())) return;
	if (img->hasCudaSupport()) {
		if (!(GPU_utils::gpuGrayConvolve(kernel.getFloatArray(), img->getPointerToArray(), kernel.getSideSize(), kernel.getSideSize(), img->width(), img->height(), passes))) {
			std::cout << "\n\n!!!CUDA support not found in host environment. CPU-only multi-threaded convolution will be used from now on!!!\n\n";
			img->setCudaSupport(false);
		}
	}
	else {
		Private::GRAY_multiThread_1D_Convolution(img, kernel, passes, thread_n);
	}
}

void convolve(Img& img, Filter& filter, uint8_t passes, uint8_t thread_number) {
	if (!(filter.getSideSize() & 1)) {
		std::cout << "\n!!Wrong filter dimensions: it must be square and odd!!\n";
		return;
	}
	
	if (img.isRGB()) {
		if (filter.is_three_channel())
		{
			Private::RGB_multiThread_3D_Convolution(&img, filter, passes, thread_number);
		}
		else
		{
			if (img.hasCudaSupport())
			{
				Private::RGB_CUDA_Accelerated_1D_Convolution(&img, filter, passes, thread_number);
			}
			else
			{
				Private::RGB_multiThread_1D_Convolution(&img, filter, passes, thread_number);
			}
		}
	}
	else if (img.isGRAY() || img.isBINARY())
	{
		if (filter.is_three_channel())
		{
			std::cout << "\n!!Wrong filter dimensions: cannot apply 3D filter to Gray scale image!!\n";
			return;
		}
		else
		{
			if (img.hasCudaSupport())
			{
				Private::GRAY_CUDA_Accelerated_1D_Convolution(&img, filter, passes, thread_number);
			}
			else
			{
				Private::GRAY_multiThread_1D_Convolution(&img, filter, passes, thread_number);
			}
		}
	}
}

void convolve(Img& img, SeparableFilter& vertical, SeparableFilter& horizontal, uint8_t passes, uint8_t thread_number)
{
	if (img.isRGB())
	{
		if (vertical.getSideSize() != horizontal.getSideSize() || !(vertical.getSideSize() & 0x1))
		{
			std::cout << "\n!!Wrong filter dimensions: separable filters must match dimensions and odd!!\n";
			return;
		}
		Private::RGB_multiThread_1D_Separable_Convolution(&img, vertical, horizontal, passes, thread_number);
	}
	else if (img.isGRAY() || img.isBINARY())
	{
		if (vertical.getSideSize() != horizontal.getSideSize() || !(vertical.getSideSize() & 0x1))
		{
			std::cout << "\n!!Wrong filter dimensions: separable filters must match dimensions and odd!!\n";
			return;
		}
		Private::GRAY_multiThread_1D_Separable_Convolution(&img, vertical, horizontal, passes, thread_number);
	}
}