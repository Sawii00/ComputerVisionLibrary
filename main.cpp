#include "img.h"
#include "testing.h"

/*
PLANS:
- Unify Support boleans (Has Cuda SUpport does not need to exist in both hsl and rgb image)

- Render Shapes on screen
- Blob detection
- Canny edge detection

REFACTOR:
- ToGRAY in GPU
- Vectorize Stuff


// NOTE(Sawii00): IF possible avoid call to getArray or getPointer to array multiple times... call it once at the beginning and store the value for the usage


*/

int main() {
	
	std::cout << sizeof(RGBPixel) << " " << sizeof(HSLPixel) << " " << sizeof(GRAYPixel);
	
	testAll();
	
	system("pause");
	return 0;
	
}


