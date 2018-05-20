/*
 * BMP reader/writer
 *
 */
#include <stdlib.h>
#include "imlib.h"

#define ERR_NOT_OPEN 1;
#define ERR_NOT_BMP 2;
#define ERR_FILE_CORRUPTED 3; 
#define ERR_NOT_SUPPORT 4;

int bmp_read(image_t *img, const char *path)
{
    FILE *fp = fopen(path, "rb");
	if (fp == NULL){
		printf("fail to open image, image file path is %s", path);
		return ERR_NOT_OPEN;
	}
	char temp = '0';
	fread(&temp, 1, 1, fp);
	if (temp != 'B'){
		printf("not bmp image.", path);
		return ERR_NOT_BMP;
	}
	fread(&temp, 1, 1, fp);
	if (temp != 'M'){
		printf("not bmp image.", path);
		return NOT_BMP;
	}
	uint32_t file_size = 0;
	fread(&file_size, 4, 1, fp);
	char ignore[4];
	fread(&ignore, 4, 1, fp);
	uint32_t header_size = 0;
	fread(&header_size, 4, 1, fp);
	uint32_t data_size = file_size - header_size;
	if (data_size % 4){
		printf("bmp file corrupted:data size is not a multiple of 4 bytes");
		return ERR_FILE_CORRUPTED;
	}
	uint32_t dib_header_size = 0;
	fread(&dib_header_size, 4, 1, fp);
	if (dib_header_size != 40){
		printf("unsupport dib header.");
		return ERR_NOT_SUPPORT;
	}
	uint32_t width = 0, height = 0;
	fread(&width, 4, 1, fp);
	fread(&height, 4, 1, fp);
	if (width == 0 || height == 0) {
		printf("bmp width or height is 0.");
		return 	ERR_FILE_CORRUPTED;
	}
	img->w = abs(width);
	img->h = abs(height);
	

	return 0;
}
