#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "API.h"
#include "maze.h"

bool check_goal(short x, short y)
{
    if (x == (MAZESIZE / 2) || x == (MAZESIZE / 2) - 1)
    {
        if (y == (MAZESIZE / 2) || y == (MAZESIZE / 2) - 1)
        {
            API_setColor(x, y, 'G');
            return true;
        }
    }
    return false;
}
void move(struct maze *_maze, short *x, short *y, short *direct)
{
    short next_direct = get_smallest_neighbor_dir(_maze->_this_map[*x][*y]);
    // Kiểm tra nếu không có hướng hợp lệ
    if (next_direct == -1)
    {
    logmess("No valid direction found, skipping move.");
    return;
    }


    // Xử lý di chuyển theo hướng xác định
    if (next_direct == EAST)
    {
        if (*direct == NORTH)
        {
            logmess("Turning Right");
            API_turnRight();
        }
        else if (*direct == SOUTH)
        {
            logmess("Turning Left");
            API_turnLeft();
        }
        else if (*direct == WEST)
        {
            logmess("Turning Back");
            API_turnRight();
            API_turnRight();
        }
        logmess("Moving Forward");
        API_moveForward();
        (*x) = (*x) + 1; // Cập nhật tọa độ sau khi di chuyển
    }
    else if (next_direct == NORTH)
    {
        if (*direct == EAST)
        {
            logmess("Turning Left");
            API_turnLeft();
        }
        else if (*direct == SOUTH)
        {
            logmess("Turning Back");
            API_turnRight();
            API_turnRight();
        }
        else if (*direct == WEST)
        {
            logmess("Turning Right");
            API_turnRight();
        }
        logmess("Moving Forward");
        API_moveForward();
        (*y) = (*y) + 1;
    }
    else if (next_direct == SOUTH)
    {
        if (*direct == NORTH)
        {
            logmess("Turning Back");
            API_turnRight();
            API_turnRight();
        }
        else if (*direct == EAST)
        {
            logmess("Turning Right");
            API_turnRight();
        }
        else if (*direct == WEST)
        {
            logmess("Turning Left");
            API_turnLeft();
        }
        logmess("Moving Forward");
        API_moveForward();
        (*y) = (*y) - 1;
    }
    else if (next_direct == WEST)
    {
        if (*direct == NORTH)
        {
            logmess("Turning Left");
            API_turnLeft();
        }
        else if (*direct == EAST)
        {
            logmess("Turning Back");
            API_turnRight();
            API_turnRight();
        }
        else if (*direct == SOUTH)
        {
            logmess("Turning Right");
            API_turnRight();
        }
        logmess("Moving Forward");
        API_moveForward();
        (*x) = (*x) - 1;
    }

    // Cập nhật hướng hiện tại
    *direct = next_direct;
}

int main(int argc, char *argv[])
{
    logmess("Running...");
    API_setColor(0, 0, 'G');
    API_setText(0, 0, "abc");
    struct maze *_maze = maze_init(_maze);
    struct queue *_queue = initQ();
    short direct = NORTH;
    short x = 0;
    short y = 0;
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "Walls (0;0) : North: %d, East: %d, South: %d, West: %d",
             _maze->_this_map[0][0]->wallup,
             _maze->_this_map[0][0]->wallright,
             _maze->_this_map[0][0]->walldown,
             _maze->_this_map[0][0]->wallleft);
    logmess(buffer);
    while (!check_goal(x, y))
    {
        set_wall(_maze->_this_map[x][y], direct);
        if (!check_for_smallest_neighbors(_maze->_this_map[x][y]))
        {
            _queue = addQ(_maze->_this_map[x][y], _queue);
            _maze = floodfill(_maze, _queue);
        }
        move(_maze, &x, &y, &direct);
        API_setColor(x, y, 'R');
        
    }
}