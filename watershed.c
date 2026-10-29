#include <stdio.h>
#include <stdlib.h>
#include "pointcloud.h"

int main(int argc, char *argv[]) {
    if (argc != 7 && argc != 8) {
        fprintf(stderr, "usage: %s <ifile> <iter> <iwater> <wcoef> <ecoef> <ofilebase> [seq]\n", argv[0]);
        return 1;
    }
    int iter, seq = 0;
    double iwater, wcoef, ecoef;
    if (sscanf(argv[2], "%d",  &iter)   != 1 || iter < 0 ||
        sscanf(argv[3], "%lf", &iwater) != 1 ||
        sscanf(argv[4], "%lf", &wcoef)  != 1 || wcoef < 0.0 || wcoef > 0.2 ||
        sscanf(argv[5], "%lf", &ecoef)  != 1 || ecoef < 0.9 || ecoef > 1.0 ||
        (argc == 8 && (sscanf(argv[7], "%d", &seq) != 1 || seq < 1))) {
        fprintf(stderr, "error: bad numeric argument\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) { perror("fopen"); return 1; }
    pointcloud_t pc;
    int err = readPointCloud(fp, &pc);
    fclose(fp);
    if (err) { fprintf(stderr, "error reading pointcloud\n"); return 1; }

    pc.wcoef = wcoef;
    pc.ecoef = ecoef;
    initializeWatershed(&pc, iwater);

    char name[512];
    for (int step = 1; step <= iter; step++) {
        watershedStep(&pc);
        if (seq > 0 && step % seq == 0) {
            snprintf(name, sizeof name, "%s%d.gif", argv[6], step);
            imagePointCloudWater(&pc, iwater, name);
        }
    }
    if (seq == 0) {
        snprintf(name, sizeof name, "%s.gif", argv[6]);
        imagePointCloudWater(&pc, iwater, name);
    }

    free(pc.points.data);
    free(pc.scratch);
    return 0;
}