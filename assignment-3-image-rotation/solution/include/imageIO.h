#ifndef IMAGEIO
#define IMAGEIO
#include "enums.h"
#include "imageStruct.h"

#include <stdio.h>
enum readStatus readImage(FILE* in, struct image* read);
enum writeStatus writeImage(FILE* out, struct image const* img);
#endif
