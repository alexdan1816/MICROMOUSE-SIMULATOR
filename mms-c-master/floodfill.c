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
            return true;
        }
    }
    return false;
}
maze *floodfill(maze *_maze, queue **_queue)
{
    logmess("floodfilling");
    while (!isEmpty(*_queue))
    {
        while (!isEmpty((*_queue)))
        {
            Node *_this_node;
            popQ(_queue, &_this_node);
            if (!check_for_smallest_neighbors(_this_node))
            {
                flood_value(_this_node);
            }
            (*_queue) = add_all_neighbors(_this_node, (*_queue));
        }
    }
    return _maze;
}
void move(struct maze *_maze, short *x, short *y, short *direct)
{
    short next_direct = get_smallest_neighbor_dir(_maze->_this_map[*x][*y]);
    if (next_direct == NORTH)
    {
        (*y) = (*y) - 1;
        if (*direct == NORTH)
        {
            API_moveForward();
            logmess("Moving Forward");
        }
        else if (*direct == EAST)
        {
            API_turnLeft();
            logmess("Turning Left");
            API_moveForward();
        }
        else if (*direct == SOUTH)
        {
            API_turnRight();
            API_turnRight();
            logmess("Turning Back");
            API_moveForward();
            logmess("Moving Forward");
        }
        else if (*direct == WEST)
        {
            API_turnRight();
            logmess("Turning Right");
            API_moveForward();
            logmess("Moving Forward");
        }
    }
    else if (next_direct == EAST)
    {
        (*x) = (*x) + 1;
        if (*direct == NORTH)
        {
            API_turnRight();
            logmess("Turning Right");
            API_moveForward();
            logmess("Moving Forward");
        }
        else if (*direct == EAST)
        {
            API_moveForward();
            logmess("Moving Forward");
        }
        else if (*direct == SOUTH)
        {
            API_turnLeft();
            logmess("Turning Left");
            API_moveForward();
            logmess("Moving Forward");
        }
        else if (*direct == WEST)
        {
            API_turnRight();
            API_turnRight();
            logmess("Turning Back");
            API_moveForward();
            logmess("Moving Forward");
        }
    }
    else if (next_direct == SOUTH)
    {
        (*y) = (*y) + 1;
        if (*direct == NORTH)
        {
            API_turnRight();
            API_turnRight();
            logmess("Turning Back");
            API_moveForward();
            logmess("Moving Forward");
        }
        else if (*direct == EAST)
        {
            API_turnRight();
            logmess("Turning Right");
            API_moveForward();
            logmess("Moving Forward");
        }
        else if (*direct == SOUTH)
        {
            API_moveForward();
            logmess("Moving Forward");
        }
        else if (*direct == WEST)
        {
            API_turnLeft();
            API_moveForward();
            logmess("Moving Forward");
        }
    }
    else if (next_direct == WEST)
    {
        (*x) = (*x) - 1;
        if (*direct == NORTH)
        {
            API_turnLeft();
            logmess("Turning Left");
            API_moveForward();
            logmess("Moving Forward");
        }
        else if (*direct == EAST)
        {
            API_turnRight();
            API_turnRight();
            logmess("Turning Back");
            API_moveForward();
            logmess("Moving Forward");
        }
        else if (*direct == SOUTH)
        {
            API_turnRight();
            logmess("Turning Right");
            API_moveForward();
            logmess("Moving Forward");
        }
        else if (*direct == WEST)
        {
            API_moveForward();
            logmess("Moving Forward");
        }
    }
    *direct = next_direct;
    return;
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
    _maze->_this_map[x][y] = set_wall(_maze->_this_map[x][y], direct);
    move(_maze, &x, &y, &direct);

    while (true)
    {
        direct = get_smallest_neighbor_dir(_maze->_this_map[x][y]);
        move(_maze, &x, &y, &direct);
        if (!check_for_smallest_neighbors)
        {
            _queue = addQ(_maze->_this_map[x][y], _queue);
            _maze = floodfill(_maze, &_queue);
        }
    }
}