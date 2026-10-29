#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
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
    printf("max height (x,y,z): (%f, %f, %f)\n", x_max, y_max, h_max);
    printf("min height (x,y,z): (%f, %f, %f)\n", x_min, y_min, h_min);
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
        p.wd = 0.0;
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

//part c
static int cmpPoints(const void *a, const void *b) {
    const pcd_t *p = a, *q = b;
    if (p->y < q->y) return -1;
    if (p->y > q->y) return  1;
    if (p->x < q->x) return -1;
    if (p->x > q->x) return  1;
    return 0;
}

int readPointCloud(FILE *stream, pointcloud_t *pc) {
    if (!listInit(&pc->points, sizeof(pcd_t))) return -1;
    if (readPointCloudData(stream, &pc->points) < 0 || pc->points.size == 0) {
        free(pc->points.data);
        return -1;
    }

    pcd_t *pts = pc->points.data;
    int n = pc->points.size;
    qsort(pts, n, sizeof(pcd_t), cmpPoints);      // south->north, west->east

    int cols = 0;                                  // points sharing the lowest y
    while (cols < n && pts[cols].y == pts[0].y) cols++;
    if (n % cols != 0) { free(pts); return -1; }   // not a full rectangle
    pc->cols = cols;
    pc->rows = n / cols;

    pc->stats.minz = pc->stats.maxz = pts[0].z;
    for (int i = 1; i < n; i++) {
        if (pts[i].z < pc->stats.minz) pc->stats.minz = pts[i].z;
        if (pts[i].z > pc->stats.maxz) pc->stats.maxz = pts[i].z;
    }

    pc->scratch = allocateArray(pc->rows, pc->cols, sizeof(double));
    if (!pc->scratch) { free(pts); return -1; }

    for (int r = 0; r < pc->rows; r++) {
        for (int c = 0; c < pc->cols; c++) {
            pcd_t *p = &pts[INDEX(r, c, cols)];
            p->north = (r + 1 < pc->rows) ? &pts[INDEX(r + 1, c, cols)] : NULL;
            p->south = (r > 0)            ? &pts[INDEX(r - 1, c, cols)] : NULL;
            p->east  = (c + 1 < cols)     ? &pts[INDEX(r, c + 1, cols)] : NULL;
            p->west  = (c > 0)            ? &pts[INDEX(r, c - 1, cols)] : NULL;
        }
    }
    return 0;
}

int initializeWatershed(pointcloud_t *pc, double amount) {
    if (pc == NULL) return 1;
    pcd_t *pts = pc->points.data;
    for (int i = 0; i < pc->points.size; i++) pts[i].wd = amount;
    return 0;
}
// f(C1,C2),  change in C1's water due to neighbor C2
static double flow(const pcd_t *c1, const pcd_t *c2, double wcoef) {
    double e2 = c2 ? c2->z : c1->z - 2.0;
    return (e2 - c1->z) * wcoef;
}

// total change for one cell
static double waterDelta(const pcd_t *c, double wcoef, double ecoef) {
    return flow(c, c->east,  wcoef) + flow(c, c->west,  wcoef)
         + flow(c, c->south, wcoef) + flow(c, c->north, wcoef)
         - c->wd * ecoef;
}

void watershedStep(pointcloud_t *pc) {
    pcd_t *pts = pc->points.data;
    int n = pc->points.size;
    for (int i = 0; i < n; i++)
        pc->scratch[i] = pts[i].wd + waterDelta(&pts[i], pc->wcoef, pc->ecoef);
    for (int i = 0; i < n; i++)
        pts[i].wd = pc->scratch[i];
}
//output
void imagePointCloudWater(pointcloud_t *pc, double maxwd, char *filename) {
    if (!pc || !filename || maxwd <= 0) return;
    Bitmap *bmp = bm_create(pc->cols, pc->rows);
    if (!bmp) return;

    pcd_t *pts = pc->points.data;
    double range = pc->stats.maxz - pc->stats.minz;

    for (int r = 0; r < pc->rows; r++) {
        for (int c = 0; c < pc->cols; c++) {
            pcd_t *p = &pts[INDEX(r, c, pc->cols)];
            double gray = (range > 0) ? (p->z - pc->stats.minz) / range * 255.0 : 0.0;
            double a = p->wd / maxwd;
            if (a < 0) a = 0;
            if (a > 1) a = 1;
            int rg = (int)(gray * (1.0 - a) + 0.5);
            int b  = (int)(gray * (1.0 - a) + 255.0 * a + 0.5);
            bm_set(bmp, c, pc->rows - 1 - r, bm_rgb(rg, rg, b));
        }
    }
    bm_save(bmp, filename);
    bm_free(bmp);
}