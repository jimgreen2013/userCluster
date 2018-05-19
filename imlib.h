#ifndef _IMLIB_H_
#define _IMLIB_H_

typedef struct image {
    int w;
    int h;
    int bpp;
    union {
        uint8_t *pixels;
        uint8_t *data;
    };
} image_t;

#endif
