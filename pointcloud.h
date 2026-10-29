#ifndef POINTCLOUD_H
#define POINTCLOUD_H
#include <stdio.h>
#include "util.h"

typedef struct pcd {
    double x;
    double y;
    double z;
    double water;

    //pointers
    struct pcd *north;
    struct pcd *south;
    struct pcd *east;
    struct pcd *west;
} pcd_t;
void stat_part_a(const char *filename);
int readPointCloudData(FILE *stream, List *lst);
void imagePointCloud( List *l, char *filename );

#endif
