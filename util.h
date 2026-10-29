#ifndef UTIL_H
#define UTIL_H 

#define LIST_INITIAL_SIZE 10
#define INDEX(row, col, columns) (((row) * (columns)) + (col))

//utility structures - given
typedef struct
{
    int max_size;
    int max_element_size;
    void* data;
    int size;
} List;
//given prototype functions
int listInit(List* l, int max_elmt_size);
int listAddEnd(List* l, void* elmt);
void *listGet(List* l, int index);



//add comments for prototype
void *allocateArray(int rows, int columns, int size);

#endif 