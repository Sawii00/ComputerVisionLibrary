#include "img.h"
#include "image_manipulation.h"
/*
PLANS:
- Unify Support boleans (Has Cuda SUpport does not need to exist in both hsl and rgb image)

- Render Shapes on screen
- Blob detection
- Canny edge detection

*/

int main() {
	
	Img img;
	if (load(img, "resources/test2.bmp", ImageType::GRAY))
	{
		show(img);
		save(img, "testozzo.bmp");
	}


	
}


