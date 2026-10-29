# coms3270P1 - Rainfall simulation

A C program that reads point cloud terrain data and roughly simulates how water flows over it.

## Building
    make        # builds watershed, stata, display
    make clean  # removes objects, executables, out.gif

## Input format
Plain text file, one point per line (x, y, z) each doubles.
x,y are centers of the cell, z is height. Points form a full rectangular grid
row order does not matter
Sample files: pointcloud.txt, test3x3.txt 

## Programs
### watershed
    ./watershed <ifile> <iter> <iwater> <wcoef> <ecoef> <ofilebase> [seq]
| arg | meaning |
|---------|
| ifile | input point-cloud file |
| iter | number of simulation steps |
| iwater | initial water depth in every cell |
| wcoef | flow coefficient, 0.0–0.2 |
| ecoef | evaporation coefficient, 0.9–1.0 |
| ofilebase | output name; writes `<ofilebase>.gif` at the end |
| seq | optional; writes `<ofilebase><step>.gif` every seq steps instead |

Example: `./watershed pointcloud.txt 100 1.0 0.1 0.95 out 10


### display
    ./display < pointcloud.txt      # writes out.gif (grayscale height map)
### stata
    ./stata                          # prints min, max, average height
    
## Known Behavior
- If multiple points are the min/max height, only the first is reported
- Input must be a complete rectangle
- Invalid Arguements print an error and exit at status 1