#include "image.h"

int main() {
	Image img;
	img.loadBMP("bmp32.bmp");
	img.displayImage();
	return 0;
}