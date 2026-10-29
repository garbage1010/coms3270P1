/*
 * display.c - main program for the "display"
 *
 * Reads a point-cloud stream from standard input and makes
 * height map of it to out.gif (black = lowest, white = highest).
 *
 * Usage: ./display < pointcloud.txt
 * Exit status: 0 on success, 1 if allocation fails or no data is read.
 */
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "pointcloud.h"

int main(void)
{
    List lst;
    /* Create an empty list of pcd_t structs. */
    if (!listInit(&lst, sizeof(pcd_t))) {
        fprintf(stderr, "Error: could not allocate list\n");
        return 1;
    }
    /* readPointCloudData returns the column count, or -1 if memory ran out. */
    int cols = readPointCloudData(stdin, &lst);
    if (cols < 0) {
        fprintf(stderr, "Error: out of memory while reading data\n");
        free(lst.data);
        return 1;
    }
    if (cols == 0) {
        fprintf(stderr, "Error: no point-cloud data read\n");
        free(lst.data);
        return 1;
    }

    imagePointCloud(&lst, "out.gif");

    free(lst.data);
    return 0;
}