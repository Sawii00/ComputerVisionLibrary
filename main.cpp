#include "image.h"

int main() {
	Image img;
	if (img.loadBMP("test.bmp"))
		img.displayImage();
	return 0;
}