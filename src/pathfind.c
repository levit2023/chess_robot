#include <stdio.h>
// #include <string.h>
#include <math.h>
#include "movement.h"

#define WHEEL_DIAMETER 0.052 //wheel diameter in meters
#define WHEEL_SEPARATION 0.08643 //wheel separation in meters
#define GRID_SIZE 0.25 //size of a single square in meters

struct movement path_array[9] = {0}; //initialize to all 0

struct movement *find_path(pathfind_mode_t mode, int pos_x, int pos_y, int set_x, int set_y){
    double WHEEL_CIRCUMFERENCE = M_PI * WHEEL_DIAMETER; //distance traveled in one rotation
    double TURN_CIRCUMFERENCE = M_PI * WHEEL_SEPARATION; //distance the wheels have to move to make a full revolution

    switch (mode) {
        case DIRECT: { //verified
            double dx = (set_x - pos_x) * GRID_SIZE;
            double dy = (set_y - pos_y) * GRID_SIZE;

            //turn to face new position
            double target_angle = atan2(dx, dy);
            double angle_mag = fabs(target_angle);
            int turn_dir = (target_angle - (0.5 * M_PI) > 0) ? TURN_LEFT : TURN_RIGHT;
            turn_dir = dx > 0 && dy >= 0 ? turn_dir : (turn_dir == TURN_LEFT ? TURN_RIGHT : TURN_LEFT);
            path_array[0].dir = turn_dir;
            path_array[0].rot = (angle_mag / (2.0 * M_PI)) * TURN_CIRCUMFERENCE / WHEEL_CIRCUMFERENCE;

            //travel straight to new square
            double dist = sqrt(dx*dx + dy*dy);
            path_array[1].dir = FORWARD;
            path_array[1].rot = dist / WHEEL_CIRCUMFERENCE;

            //turn back to face forward
            path_array[2].dir = (turn_dir == TURN_LEFT) ? TURN_RIGHT : TURN_LEFT;
            path_array[2].rot = path_array[0].rot;

            break;
        }
        case ON_GRID: { //verified
            int dx = set_x - pos_x;
            int dy = set_y - pos_y;

            int step = 0;

            //horizontal turn
            if (dx != 0) {
                path_array[step].dir = (dx > 0) ? TURN_RIGHT : TURN_LEFT;
                path_array[step].rot = (0.25 * TURN_CIRCUMFERENCE) / WHEEL_CIRCUMFERENCE;
                step ++;

                //move horizontally
                path_array[step].dir = FORWARD;
                path_array[step].rot = fabs(dx) * GRID_SIZE / WHEEL_CIRCUMFERENCE;
                step ++;

                //turn back to vertical
                path_array[step].dir = (dx > 0) ?
                    TURN_LEFT :
                    TURN_RIGHT;
                path_array[step].rot = (0.25 * TURN_CIRCUMFERENCE) / WHEEL_CIRCUMFERENCE;
                step ++;
            }

            //vertical movement
            if (dy != 0) {
                path_array[step].dir = (dy > 0) ? FORWARD : BACKWARD;
                path_array[step].rot = fabs(dy) * GRID_SIZE / WHEEL_CIRCUMFERENCE;
                step ++;
            }

            break;
        }
        case OFF_GRID: { //verified
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
                path_array[2].rot = ((set_x - pos_x) * GRID_SIZE - GRID_SIZE / 2.0) / WHEEL_CIRCUMFERENCE;
            } else {
                path_array[2].rot = fabs((-(set_x - pos_x) * GRID_SIZE - GRID_SIZE / 2.0)) / WHEEL_CIRCUMFERENCE;
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
                path_array[4].rot = ((set_y - pos_y) * GRID_SIZE - GRID_SIZE / 2.0) / WHEEL_CIRCUMFERENCE;
            } else {
                path_array[4].rot = -((set_y - pos_y) * GRID_SIZE - GRID_SIZE / 2.0) / WHEEL_CIRCUMFERENCE;
            }
        
            //turn to horizontal to align piece on-grid horizontally
            if (set_x - pos_x >= 0) {
                if (set_y - pos_y > 0) {
                    path_array[5].dir = TURN_RIGHT;
                } else {
                    path_array[5].dir = TURN_LEFT;
                }
            } else if (set_x == pos_x) {
                path_array[5].dir = TURN_RIGHT;
            } else {
                if (set_y - pos_y > 0) {
                    path_array[5].dir = TURN_LEFT;
                } else {
                    path_array[5].dir = TURN_RIGHT;
                }
            }
            path_array[5].rot = (0.25 * TURN_CIRCUMFERENCE) / WHEEL_CIRCUMFERENCE;
        
            path_array[6].dir = FORWARD;
            path_array[6].rot = (GRID_SIZE / 2.0) / WHEEL_CIRCUMFERENCE;
        
            if (set_x - pos_x >= 0) {
                if (set_y - pos_y > 0) {
                    path_array[7].dir = TURN_LEFT;
                } else {
                    if (set_x - pos_x == 0) {
                        path_array[7].dir = TURN_LEFT;
                    } else {
                        path_array[7].dir = TURN_LEFT;
                    }
                }
            } else if (set_x == pos_x) {
                path_array[7].dir = TURN_RIGHT;
            } else {
                if (set_y - pos_y > 0) {
                    path_array[7].dir = TURN_RIGHT;
                } else {
                    path_array[7].dir = TURN_RIGHT;
                }
            }
            path_array[7].rot = (0.25 * TURN_CIRCUMFERENCE) / WHEEL_CIRCUMFERENCE;
            
            break;
        }
    }
    
    return &path_array;
}