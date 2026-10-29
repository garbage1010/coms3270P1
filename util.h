/** @file util.h  
    Array allocation helper, index macro, and expanding List. 
*/
#ifndef UTIL_H
#define UTIL_H 

#define LIST_INITIAL_SIZE 10
/** Index into a 1-D array holding a 2-D grid row by row */
#define INDEX(row, col, columns) (((row) * (columns)) + (col))

/** Expanding array. Doubles when full */
typedef struct
{
    int max_size;
    int max_element_size;
    void* data;
    int size;
} List;
/** Initialize empty list. returns 1 on success, 0 ion fail */
int listInit(List* l, int max_elmt_size);
/** Compy *elmt to the end, growing if neccessary. Returns 1 on ok, 0 on failure  */
int listAddEnd(List* l, void* elmt);
/** Pointer to 'index', or NULL if out of range */
void *listGet(List* l, int index);



//add comments for prototype
void *allocateArray(int rows, int columns, int size);

#endif 