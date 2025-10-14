#include <movement.h>
#include <utils.h>
#include <stdio.h>

static int x_pos = STARTING_X;
static int y_pos = STARTING_Y;
void move_to (int x, int y) {
    struct movement *path_addr;
    path_addr = find_path(x_pos, y_pos, x, y);

    for (int i = 0; i < 10; i ++){
        printf("Direction: %d, Rotations: %.2lf\n", path_addr[i].dir, path_addr[i].rot);
        stepper_steps(path_addr[i].dir, (int)(path_addr[i].rot * 4096));
        while (!stepper_idle) {
            tight_loop_contents();
        }
    }
    x_pos = x;
    y_pos = y;
}