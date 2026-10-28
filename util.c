#include <stdio.h>
#include "util.h"

void *allocateArray(int rows, int columns, int size){
    //check proper format
    if(rows <= 0 || columns <= 0 || size <= 0){
        return NULL;
    }
    return calloc((size_t)rows * (size_t)columns, (size_t)size);

}

int listInit(List *l, int max_elmt_size){
    l->data = malloc((size_t)LIST_INITIAL_SIZE * (size_t)max_elmt_size);
    if (l->data == NULL) {
        return 0;
    }
    l->max_size = LIST_INITIAL_SIZE;
    l->max_element_size = max_elmt_size;
    l->size = 0;
    return 1;
}

//no room left, double size
int listAddEnd(List *l, void *elmt){
    if (l->size == l->max_size) {
        int new_max = l->max_size * 2;
        void *tmp = realloc(l->data,
                            (size_t)new_max * (size_t)l->max_element_size);
        if (tmp == NULL) {
            return 0;        //keep untouched
        }
        l->data = tmp;
        l->max_size = new_max;
    }

    //copy
    char *dest = (char *)l->data + (size_t)l->size * (size_t)l->max_element_size;
    memcpy(dest, elmt, (size_t)l->max_element_size);
    l->size++;
    return 1;
}

void *listGet(List *l, int index){
    if (index < 0 || index >= l->size) {
        return NULL;
    }
    return (char *)l->data + (size_t)index * (size_t)l->max_element_size;
}