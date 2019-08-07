#ifndef _IMLIB_H_
#define _IMLIB_H_
//test commit
typedef struct image {
    int w;
    int h;
    int bpp;
    union {
        uint8_t *pixels;
        uint8_t *data;
    };
} image_t;

//t11111
#endif
