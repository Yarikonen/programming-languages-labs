#include "../include/imageStruct.h"
#include <stdlib.h>
struct image newImage(const uint64_t width, const uint64_t height){
        struct pixel* pixel = malloc(sizeof(struct pixel)*(width*height));
        return(struct image){
                .width=width,
                .height=height,
                .data= pixel

        };
}
void delImage(struct image* image){
        free(image->data);


}
