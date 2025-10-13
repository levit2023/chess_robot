// #include <stdio.h>
// #include <string.h>
// #include <math.h>

double WHEEL_DIAMETER = 0.052; //wheel diameter in meters
double WHEEL_SEPARATION = 0.08643; //wheel separation in meters
double GRID_SIZE = 0.2; //size of a single square in meters

typedef enum {
    FORWARD = 0,
    BACKWARD = 1,
    LEFT = 2,
    RIGHT = 3
} direction;

struct movement{
    direction dir; //direction to move in
    int rot; //number of rotations
};

struct movement path_array[64] = {0}; //initialize to all 0

struct movement *find_path(int pos_x, int pos_y, int set_x, int set_y){
    double WHEEL_CIRCUMFERENCE = 3.141592653589 * WHEEL_DIAMETER; //distance traveled in one rotation

    //first move onto the movement grid (between pieces)
    path_array[0].dir = FORWARD;
    path_array[0].rot = WHEEL_CIRCUMFERENCE / (GRID_SIZE / 2.0);
    
    return &path_array;
}