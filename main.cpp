#include "image.h"

int main() {
	Image img;
	if (img.loadBMP("bmp8.bmp"))
		img.displayImage();
	return 0;
}