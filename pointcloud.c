#include <stdio.h>

stat_part_a(){

    float x, y, h;
    size_t count = 0;

    float x_min = 0.0, y_min = 0.0, h_min = 0.0;
    float x_max = 0.0, y_max = 0.0, h_max = 0.0;
    double height_sum = 0.0; 

    //iterate through data.. sum and assign min/max
    while(scanf("%f %f %f", &x, &y, &h) == 3){
        if(count == 0 || h < h_min){
            x_min = x; y_min = y; h_min = h;
        }
        if(count == 0 || h > h_max){
            x_max = x; y_max = y; h_max = h;
        }

        height_sum += h;
        count++;
    }

    //sanity check
    if(count == 0){
        printf("Error: no points read\n");
    }

    //print
    printf("max height (x,y,z): (%f, &f, %f)\n", x_max, y_max, h_max);
    printf("min height (x,y,z): (%f, &f, %f)\n", x_min, y_min, h_min);
    printf("average height: %f\n", height_sum / count);
    

}