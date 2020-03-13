#include "image.h"

int main() {
	Image img;
	if (img.loadBMP("bmp24.bmp"))
		img.displayImage();
	return 0;
}