#ifndef bmpHeader2
#define bmpHeader2
#define BMP_SIGNATURE 0x4D42
#define NOT_USED 0
#define BMP_OFF_BITS 54
#define HEADER_SIZE 40
#define BIT_COUNT 24
#define PLANES 1
#define COMPRESSION 0
#include "enums.h"
#include "imageStruct.h"
#include  <stdint.h>
#include <stdio.h>

#pragma pack(push, 1)
struct bmpHeader 
{
        uint16_t bfType;
        uint32_t  bfileSize;
        uint32_t bfReserved;
        uint32_t bOffBits;
        uint32_t biSize;
        uint32_t biWidth;
        uint32_t  biHeight;
        uint16_t  biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        uint32_t biXPelsPerMeter;
        uint32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t  biClrImportant;
};
#pragma pack(pop)


enum headerState readHeader(FILE*in, struct bmpHeader* bmpHeader);
enum headerState checkHeader(const struct bmpHeader * bmpHeader);
enum headerState writeHeader(FILE* out, struct bmpHeader* bmpHeader);
struct bmpHeader newHeader(const struct image* image);
#endif
