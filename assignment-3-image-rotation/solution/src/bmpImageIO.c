


#include "../include/bmpHeaderDescription.h"
#include "../include/enums.h"
#include "../include/imageIO.h"

#include <inttypes.h>
#include  <stdint.h>
#include <stdio.h>


uint8_t padding(const uint32_t width){
        return((uint8_t) ((4-3*width%4)%4)      );

}


enum readStatus readImage(FILE* in, struct image* read){
        
        struct bmpHeader bmpHeader;
        if(readHeader(in, &bmpHeader)==INVALID_HEADER){
                return(READ_INVALID_HEADER);
        }
        uint32_t height = bmpHeader.biHeight;
        uint32_t width = bmpHeader.biWidth;
      
        if(fseek(in, bmpHeader.bOffBits ,SEEK_SET)!=0){
                return READ_INVALID_HEADER;
        };
        uint8_t paddingVal = padding(width);
        *read=newImage(width,height);
        
        if (read->data==NULL){
                return READ_ALLOCATION_ERROR;
        }
        for(size_t i=0; i<height; i++){
                
                     if (fread(read->data + i*width, sizeof(struct pixel)*width,1,in )!=1){
                        delImage(read);
                        return READ_INVALID_BITS;
                }   
                
               if (fseek(in, (int64_t) paddingVal,SEEK_CUR)!=0){
                        delImage(read);
                        return READ_PADDING_ERROR;
                };
                
        }
        return(READ_OK);
}
enum writeStatus writeImage(FILE* out, struct image const* img){
        struct bmpHeader bmpHeader = newHeader(img);
        uint8_t paddingVal = padding(bmpHeader.biWidth);
      
        if(writeHeader(out, &bmpHeader)==INVALID_HEADER){
                return(WRITE_INVALID_HEADER);
        }
        uint32_t height = bmpHeader.biHeight;
        uint32_t width = bmpHeader.biWidth;

        if (fseek(out, bmpHeader.bOffBits ,SEEK_SET)!=0){
                return WRITE_INVALID_HEADER;
        }
        for(size_t i=0; i<height; i++){
                
                     if (fwrite(img->data + i*width, sizeof(struct pixel)*width,1,out )!=1){
                        return WRITE_INVALID_BITS;
                }   
                
                if (fseek(out, paddingVal,SEEK_CUR)!=0){
                        return WRITE_PADDING_ERROR;
                };
                
        }
        
        return WRITE_OK;

}

