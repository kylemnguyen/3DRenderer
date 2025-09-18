#include "color.h"

/*
 * Vec3 unpackRGB(unsigned int packedRGB);
   void writeColour(FILE *ppmFile, Vec3 color);
   int compareColor(const void *a, const void *b);

 * */

Vec3 unpackRGB(unsigned int packedRGB) {


	unsigned int x, y, z;

	x = (packedRGB / pow(2, 16)) - 1;
    packedRGB = packedRGB % (int)pow(2, 16);

	y = (packedRGB / pow(2, 8)) - 1;
    packedRGB = packedRGB % (int)pow(2, 8);

	z = packedRGB;

	Vec3 color = (Vec3){x, y, z};
	
	return color;
}


void writeColour(FILE *ppmFile, Vec3 color) {

	// get an integer value that is between 0 and 255
	int r = (int)fmax(0, fmin(255, color.x)); 
	int g = (int)fmax(0, fmin(255, color.y));
	int b = (int)fmax(0, fmin(255, color.z));


	fprintf(ppmFile, "%d %d %d ", r, g, b );
}

int compareColor(const void *a, const void *b)
{
    int a1 = 0, b1 = 0;
    for (int i = 0; i < sizeof(int); i++)
    {
        a1 |= (*((unsigned char*)a + i) & 0x0F) << (i * 8);
        b1 |= (*((unsigned char*)b + i) & 0x0F) << (i * 8);
    }

    return (a1 < b1) ? -1 : (b1 < a1) ? 1 : (*((int*)a) < *((int*)b)) ? -1 : (*((int*)a) > *((int*)b)) ? 1 : 0;
}
