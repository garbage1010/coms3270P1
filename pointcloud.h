#ifdef POINTCLOUD_H
#define POINTCLOUD_H

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
void stat_part_a(const);
int readPointCloudData(FILE *stream, List *lst);
void imagePointCloud( List *l, char *filename );

#endif
