#include <stdlib.h>
#include "imlib.h"

void bmp_read(image_t *img, const char *path)
{
    FILE *fp = fopen(path, "r");
	if (fp == NULL)
		printf("fail to open image, image file path is %s", path);
	
}
