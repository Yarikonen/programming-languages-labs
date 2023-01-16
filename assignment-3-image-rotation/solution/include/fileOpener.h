#ifndef FILEOPENER
#define FILEOPENER
#include "enums.h"
#include <stdio.h>

enum openFileStatus openFile(FILE** file, const char* const filename, const char* const flags);
void closeFile(FILE** file);
#endif
