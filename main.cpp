#include "image.h"

int main() {
	Image img;
	if (img.loadBMP("bmp24.bmp"))
	{
		img.thresholdSegment(1);
		img.displayImage();

		/*if (img.saveBMP("bmp24Replica.bmp")) {
			Image img2;
			if (img2.loadBMP("bmp24Replica.bmp"))
				img2.displayImage();
		}*/
	}
	return 0;
}