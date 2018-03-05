#ifndef  BMPTOTEXTURE_CPP
#define  BMPTOTEXTURE_CPP
#include <stdio.h>

class BTT
{
public:
	unsigned char * BmpToTexture(char *filename, int *width, int *height);

private:
	int ReadInt(FILE *fp);
	short ReadShort(FILE *fp);
	const int birgb = { 0 };
	struct bmfh
	{
		short bfType;
		int bfSize;
		short bfReserved1;
		short bfReserved2;
		int bfOffBits;
	} FileHeader;
	struct bmih
	{
		int biSize;
		int biWidth;
		int biHeight;
		short biPlanes;
		short biBitCount;
		int biCompression;
		int biSizeImage;
		int biXPelsPerMeter;
		int biYPelsPerMeter;
		int biClrUsed;
		int biClrImportant;
	} InfoHeader;
};

#endif