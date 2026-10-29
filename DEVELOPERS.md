# Developer Notes

## File structure
| file | purpose |
|---------|
| util.c/.h | `allocateArray`, `INDEX` macro, expanding `List` |
| pointcloud.c/.h | reading, stats, imaging, watershed simulation |
| bmp.c/.h | third-party bitmap library (wernsey/bitmap), unmodified |
| watershed.c | main for `watershed` |
| display.c | main for `display` |
| pointstat.c | main for `stata` |


## Data structures
- `pcd_t`: x, y, z, wd (water depth) + north/south/east/west pointers
- `List`: expanding array, starts at 10 elements, doubles when full.
  `listGet` returns a pointer INTO the list, so pointers into `data`
  are invalidated by a later `listAddEnd` (realloc). That's why neighbor
  pointers are set only after reading finishes.
- `pointcloud_t`: points, rows, cols, stats (minz/maxz), wcoef, ecoef,
  scratch buffer (next-step water, one double per point).


## Design decisions
- readPointCloud sorts by (y, x) so row 0 is the southernmost row and
  neighbors come from INDEX
- Columns = number of points sharing the lowest y
- Missing neighbor => elevation is c1.z - 2 (water drains off the edge).


## Testing
test3x3.txt, pointcloud.txt;

## Posible improvements
- more error checking in readPointCloud