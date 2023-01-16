#include "../include/fileOpener.h"

enum openFileStatus openFile(FILE** file, const char* const filename, const char* const flags){
        *file=fopen(filename, flags);
        if (*file ==NULL){
                return(SMTH_WRONG);
        }
        return(0);
}
void closeFile(FILE** file){
        fclose(*file);
}
