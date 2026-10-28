#include <stdio.h>
#include "util.h"

void *allocateArray(int rows, int columns, int size){
    //check proper format
    if(rows <= 0 || columns <= 0 || size <= 0){
        return NULL;
    }
    return calloc((size_t)rows * (size_t)columns, (size_t)size);

}