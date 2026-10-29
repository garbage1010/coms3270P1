#ifndef POINTCLOUD_H
#define POINTCLOUD_H
#include <stdio.h>
#include "util.h"

typedef struct pcd {
    double x; //cell coordinate (m)
    double y; //cell coordinate (m)
    double z;  //terrain height
    double wd; //current water depth

    //pointers - neighbors
    struct pcd *north;
    struct pcd *south;
    struct pcd *east;
    struct pcd *west;
} pcd_t;

typedef struct {
    double minz, maxz;
} stats_t;

typedef struct {
    List points;        
    int rows;           
    int cols;           
    stats_t stats;      // min/max height, for imaging
    double wcoef, ecoef;
    double *scratch;    // next-step water, one double per point
} pointcloud_t;


void stat_part_a(void);
int readPointCloudData(FILE *stream, List *lst);
void imagePointCloud( List *l, char *filename );
int  readPointCloud(FILE *stream, pointcloud_t *pc);
int  initializeWatershed(pointcloud_t *, double);
void watershedStep(pointcloud_t *pc);
void imagePointCloudWater(pointcloud_t *pc, double maxwd, char *filename);

#endif
