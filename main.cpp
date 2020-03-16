#include "filter.h"
#include "image.h"
#include "Utils.h"

int main() {
	Image img;
	Image img2;
	if (img.loadBMP("img.bmp"))
	{
		img2 = img;
		Filter filter(3, 3, false);
		float f[9] = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
		filter.build(f, 9);
        
		Filter f2(3, 3, true);
		KernelPixel val(0.005, 0.4, 0.005);
		KernelPixel vals[9] = { val, val, val, val, val, val ,val ,val, val };
		f2.build(vals, 9);
        Timer t;
        t.start();
		img.convolve1D(filter);
        
		img.convolve3D(f2);
        
		img += (img2 * 0.8);
        
        t.stop();
		std::cout << "Time: " << t.elapsedMilliseconds();
		img.displayImage();
	}
    
	return 0;
}