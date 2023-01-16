#include "../include/main.h"

static void errorPrint(const char* string){
     fprintf(stderr, "%s\n",string );
     
}
int main( int argc, char** argv ) {
    const char* errorArray[6];
    errorArray[0]="Ok, good job!";
    errorArray[1]="Smth wrong with signature";
    errorArray[2]="SMTH wrong with bits";
    errorArray[3]="Smth wrong with header";
    errorArray[4]="Smth wrong with padding";
    errorArray[5]="Allocation error";
    if (argc!=3){
        errorPrint("Wrong arguments");
        return(0);
    }
   

    FILE* file ;
    FILE* out;
    if (openFile(&file, argv[1], "r")!=0){
        errorPrint("Smth wrong with reading file opening");
        return(0);
    } 
    
    if( openFile(&out, argv[2], "wb")!=0){
        errorPrint("Smth wrong with writing file opening");
        return(0);

    }
   
    struct image image; 
    struct image image2;

    enum readStatus rs=readImage(file,&image);
    if (rs!=0){
            errorPrint(errorArray[rs]);
            closeFile(&file);
            closeFile(&out);
            return(0);
    }
   
    if(rotateImage(image,&image2)!=0){
        errorPrint("MallocError issue");
        delImage(&image2);
        delImage(&image);
        closeFile(&file);
        closeFile(&out);
        return(0);
    }
    
    enum writeStatus ws= writeImage(out, &image2);
    if (ws!=0){
            errorPrint(errorArray[rs]);
            delImage(&image2);
            delImage(&image);
            
            
    }
    closeFile(&file);
    closeFile(&out);
    delImage(&image);
    delImage(&image2);
    return(0);
    
   
}

