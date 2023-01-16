#include <inttypes.h>
#include <stdio.h>

#include "../include/enums.h"
#include"../include/bmpHeaderDescription.h"
#include"../include/imageStruct.h"

enum headerState readHeader(FILE*in, struct bmpHeader* bmpHeader){
        fread(bmpHeader, sizeof(struct bmpHeader), 1, in);
        return(checkHeader(bmpHeader));
}
enum headerState checkHeader(const struct bmpHeader * bmpHeader){
        if(bmpHeader->biSize<=0 || bmpHeader->biHeight<=0||bmpHeader->biWidth<=0 || bmpHeader->biSizeImage<=0){
                return INVALID_HEADER;

        }
        else{
                return HEADER_OK;
        }

}
enum headerState writeHeader(FILE* out, struct bmpHeader* bmpHeader){
        fwrite(bmpHeader, sizeof(struct bmpHeader),1, out);
        return(checkHeader(bmpHeader));
}
uint32_t sizeofImage(uint32_t width, uint32_t height){
        return( (uint32_t)( (height)*(sizeof(struct pixel)*(width)+  (uint64_t) padding(width))));
}

struct bmpHeader newHeader(const struct image * image){
        return (struct bmpHeader) {
                .bfType= BMP_SIGNATURE, 
                .bfileSize= sizeof(struct bmpHeader) + sizeofImage(image->width, image->height) ,
                .bfReserved=0 , //MUST BE ZERO
                .bOffBits =BMP_OFF_BITS ,
                .biSize = HEADER_SIZE,
                .biWidth = image->width,
                .biHeight = image->height,
                .biPlanes =PLANES ,
                .biBitCount =BIT_COUNT ,
                .biCompression =COMPRESSION ,
                .biSizeImage = sizeofImage(image->width, image->height),
                .biXPelsPerMeter = NOT_USED,
                .biYPelsPerMeter = NOT_USED,
                .biClrUsed =NOT_USED ,
                .biClrImportant = NOT_USED

        };
        
}
