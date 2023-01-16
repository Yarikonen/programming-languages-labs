#ifndef IMGSTRUCT
#define IMGSTRUCT
#include <inttypes.h>
#pragma pack(push, 1)
struct pixel{
        uint8_t b;
        uint8_t g;
        uint8_t r;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct image {
        uint64_t width; 
        uint64_t height;
        struct pixel* data;
};
#pragma pack(pop)
struct image newImage(const uint64_t width,const uint64_t height);
uint8_t padding(const uint32_t width);
void delImage(struct image* image);
#endif
