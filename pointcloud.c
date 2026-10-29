#include <stdio.h>
#include <stddef.h>
#include "pointcloud.h"
#include "util.h"
#include "bmp.h"

//part a
void stat_part_a(const char *filename){
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        perror("Could not open file");
        return;
    }

    float x, y, h;
    size_t count = 0;

    float x_min = 0.0, y_min = 0.0, h_min = 0.0;
    float x_max = 0.0, y_max = 0.0, h_max = 0.0;
    double height_sum = 0.0; 

    //iterate through data.. sum and assign min/max
    while(fscanf(fp, "%f %f %f", &x, &y, &h) == 3){
        if(count == 0 || h < h_min){
            x_min = x; y_min = y; h_min = h;
        }
        if(count == 0 || h > h_max){
            x_max = x; y_max = y; h_max = h;
        }

        height_sum += h;
        count++;
    }
    fclose(fp);
    //sanity check
    if(count == 0){
        printf("Error: no points read\n");
    }

    //print
    printf("max height (x,y,z): (%f, &f, %f)\n", x_max, y_max, h_max);
    printf("min height (x,y,z): (%f, &f, %f)\n", x_min, y_min, h_min);
    printf("average height: %f\n", height_sum / count);
}

//part b
int readPointCloudData(FILE *stream, List *lst)
{
    pcd_t p;
    double first_y = 0.0;
    int columns = 0;
    int count = 0;
    int counting = 1;

    while (fscanf(stream, "%lf %lf %lf", &p.x, &p.y, &p.z) == 3) {
        p.water = 0.0;
        p.north = p.south = p.east = p.west = NULL;

        if (count == 0) {
            first_y = p.y;
        }
        if (counting && p.y == first_y) {
            columns++;
        } else {
            counting = 0;
        }

        if (!listAddEnd(lst, &p)) {
            return -1;
        }
        count++;
    }
    return columns;
}

static int countColumns(List *l)
{
    pcd_t *first = listGet(l, 0);
    int cols = 0;

    if (first == NULL) {
        return 0;
    }
    while (cols < l->size &&
           ((pcd_t *)listGet(l, cols))->y == first->y) {
        cols++;
    }
    return cols;
}

void imagePointCloud(List *l, char *filename)
{
    if (l == NULL || l->size == 0 || filename == NULL) {
        return;
    }

    int cols = countColumns(l);
    int rows = l->size / cols;

    //find min and max height.
    double minz = ((pcd_t *)listGet(l, 0))->z;
    double maxz = minz;
    for (int i = 1; i < l->size; i++) {
        double z = ((pcd_t *)listGet(l, i))->z;
        if (z < minz) minz = z;
        if (z > maxz) maxz = z;
    }

    Bitmap *bmp = bm_create(cols, rows);
    if (bmp == NULL) {
        return;
    }

    // Pass 2: map each height to a gray level and set the pixel
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            pcd_t *p = listGet(l, INDEX(r, c, cols));
            int gray = 0;
            if (maxz > minz) {
                gray = (int)((p->z - minz) / (maxz - minz) * 255.0 + 0.5);
            }
            bm_set(bmp, c, r, bm_rgb(gray, gray, gray));
        }
    }

    bm_save(bmp, filename); 
    bm_free(bmp);
}