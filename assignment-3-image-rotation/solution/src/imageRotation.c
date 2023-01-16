#include "../include/imageRotation.h"

enum rotationStatus rotateImage( struct image const source, struct image* rezImage ){
        *rezImage = newImage(source.height,source.width);
        if (rezImage->data==0){
                return MALLOC_ERROR;
        }
        uint64_t rezHeight = rezImage->height;
        uint64_t rezWidth = rezImage->width;
        for(size_t i=0;i<rezHeight;i++ ){
                for(size_t j=0; j<rezWidth;j++){
                        *((rezImage->data)+(rezWidth)*i+j)=*((source.data) + (rezWidth -j-1)*rezHeight+i);
                }
        }
        return ROTATION_OK;

}
