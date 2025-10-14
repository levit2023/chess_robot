// #include <stdio.h>
// #include <string.h>
// #include <math.h>
#include "movement.h"

double WHEEL_DIAMETER = 0.052; //wheel diameter in meters
double WHEEL_SEPARATION = 0.08643; //wheel separation in meters
double GRID_SIZE = 0.2; //size of a single square in meters

struct movement path_array[7] = {0}; //initialize to all 0

struct movement *find_path(int pos_x, int pos_y, int set_x, int set_y){
    double WHEEL_CIRCUMFERENCE = 3.141592653589 * WHEEL_DIAMETER; //distance traveled in one rotation
    double TURN_CIRCUMFERENCE = 3.141592653589 * WHEEL_SEPARATION; //distance the wheels have to move to make a full revolution
    //first move onto the movement grid (between pieces)
    path_array[0].dir = FORWARD;
    path_array[0].rot = (GRID_SIZE / 2.0) / WHEEL_CIRCUMFERENCE;

    //turn to move horizontally
    if (set_x - pos_x > 0) {
        path_array[1].dir = TURN_RIGHT;
    } else {
        path_array[1].dir = TURN_LEFT;
    }
    path_array[1].rot = (0.25 * TURN_CIRCUMFERENCE) / WHEEL_CIRCUMFERENCE; //rotate 90 degrees

    //horizontal movement
    path_array[2].dir = FORWARD;
    //(|set x - current x| - grid size / 2) / wheel circumference
    if (set_x - pos_x > 0) {
        path_array[2].rot = ((set_x - pos_x) - GRID_SIZE / 2.0) / WHEEL_CIRCUMFERENCE;
    } else {
        path_array[2].rot = -((set_x - pos_x) - GRID_SIZE / 2.0) / WHEEL_CIRCUMFERENCE;
    }

    //turn to vertical movement
    if (set_x - pos_x > 0) {
        if (set_y - pos_y > 0) {
            path_array[3].dir = TURN_LEFT;
        } else {
            path_array[3].dir = TURN_RIGHT;
        }
    } else {
        if (set_y - pos_y > 0) {
            path_array[3].dir = TURN_RIGHT;
        } else {
            path_array[3].dir = TURN_LEFT;
        }
    }
    path_array[3].rot = (0.25 * TURN_CIRCUMFERENCE) / WHEEL_CIRCUMFERENCE; //rotate 90 degrees

    //vertical movement
    path_array[4].dir = FORWARD;
    //(|set x - current x| - grid size / 2) / wheel circumference
    if (set_y - pos_y > 0) {
        path_array[4].rot = ((set_y - pos_y) - GRID_SIZE / 2.0) / WHEEL_CIRCUMFERENCE;
    } else {
        path_array[4].rot = -((set_y - pos_y) - GRID_SIZE / 2.0) / WHEEL_CIRCUMFERENCE;
    }

    //turn to horizontal to align piece on-grid horizontally
    if (set_x - pos_x > 0) {
        if (set_y - pos_y > 0) {
            path_array[5].dir = TURN_RIGHT;
        } else {
            path_array[5].dir = TURN_LEFT;
        }
    } else {
        if (set_y - pos_y > 0) {
            path_array[5].dir = TURN_LEFT;
        } else {
            path_array[5].dir = TURN_RIGHT;
        }
    }
    path_array[5].rot = (0.25 * TURN_CIRCUMFERENCE) / WHEEL_CIRCUMFERENCE;

    //align to grid horizontally
    path_array[6].dir = FORWARD;
    path_array[6].rot = (GRID_SIZE / 2.0) / WHEEL_CIRCUMFERENCE;
    
    //last turn
    if (set_x - pos_x > 0) {
        if (set_y - pos_y > 0) {
            path_array[7].dir = TURN_LEFT;
        } else {
            path_array[7].dir = TURN_RIGHT;
        }
    } else {
        if (set_y - pos_y > 0) {
            path_array[7].dir = TURN_RIGHT;
        } else {
            path_array[7].dir = TURN_LEFT;
        }
    }
    path_array[7].rot = (0.25 * TURN_CIRCUMFERENCE) / WHEEL_CIRCUMFERENCE; 

    //center piece on square
    path_array[8].dir = BACKWARD;
    path_array[8].rot = (0.25 * TURN_CIRCUMFERENCE) / WHEEL_CIRCUMFERENCE; 
    
    return &path_array;
}