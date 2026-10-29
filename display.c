#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "pointcloud.h"

int main(void)
{
    List lst;

    if (!listInit(&lst, sizeof(pcd_t))) {
        fprintf(stderr, "Error: could not allocate list\n");
        return 1;
    }

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