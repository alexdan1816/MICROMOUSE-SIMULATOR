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
    while (!isEmpty((*_queue)))
    {
        Node *_this_node = popQ(_queue);
        if (!check_for_smallest_neighbors(_this_node))
        {
            _maze = flood_value(&_this_node, _maze);
        }
        (*_queue) = add_all_neighbors(_this_node, (*_queue));
    }
    return _maze;
}
void move(struct maze *_maze, short *x, short *y, short *direct)
{
    short next_direct = get_smallest_neighbor_dir(_maze->_this_map[*x][*y]);
    if (next_direct == NORTH)
    {
        (*y) = (*y) + 1;
        if (*direct == NORTH)
        {
            logmess("Moving Forward");
            API_moveForward();
        }
        else if (*direct == EAST)
        {
            logmess("Turning Left");
            API_turnLeft();
            logmess("Moving Forward");
            API_moveForward();
        }
        else if (*direct == SOUTH)
        {
            logmess("Turning Back");
            API_turnRight();
            API_turnRight();
            logmess("Moving Forward");
            API_moveForward();
        }
        else if (*direct == WEST)
        {
            logmess("Turning Right");
            API_turnRight();
            logmess("Moving Forward");
            API_moveForward();
        }
    }
    else if (next_direct == EAST)
    {
        (*x) = (*x) + 1;
        if (*direct == NORTH)
        {
            logmess("Turning Right");
            API_turnRight();
            logmess("Moving Forward");
            API_moveForward();
        }
        else if (*direct == EAST)
        {
            logmess("Moving Forward");
            API_moveForward();
        }
        else if (*direct == SOUTH)
        {
            logmess("Turning Left");
            API_turnLeft();
            logmess("Moving Forward");
            API_moveForward();
        }
        else if (*direct == WEST)
        {
            logmess("Turning Back");
            API_turnRight();
            API_turnRight();
            logmess("Moving Forward");
            API_moveForward();
        }
    }
    else if (next_direct == SOUTH)
    {
        (*y) = (*y) - 1;
        if (*direct == NORTH)
        {
            logmess("Turning Back");
            API_turnRight();
            API_turnRight();
            logmess("Moving Forward");
            API_moveForward();
        }
        else if (*direct == EAST)
        {
            logmess("Turning Right");
            API_turnRight();
            logmess("Moving Forward");
            API_moveForward();
        }
        else if (*direct == SOUTH)
        {
            logmess("Moving Forward");
            API_moveForward();
        }
        else if (*direct == WEST)
        {
            logmess("Turning Left");
            API_turnLeft();
            logmess("Moving Forward");
            API_moveForward();
        }
    }
    else if (next_direct == WEST)
    {
        (*x) = (*x) - 1;
        if (*direct == NORTH)
        {
            logmess("Turning Left");
            API_turnLeft();
            logmess("Moving Forward");
            API_moveForward();
        }
        else if (*direct == EAST)
        {
            logmess("Turning Back");
            API_turnRight();
            API_turnRight();
            logmess("Moving Forward");
            API_moveForward();
        }
        else if (*direct == SOUTH)
        {
            logmess("Turning Right");
            API_turnRight();
            logmess("Moving Forward");
            API_moveForward();
        }
        else if (*direct == WEST)
        {
            logmess("Moving Forward");
            API_moveForward();
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
    while (true)
    {
        move(_maze, &x, &y, &direct);
        _maze->_this_map[x][y] = set_wall(_maze->_this_map[x][y], direct);
        if (!check_for_smallest_neighbors(_maze->_this_map[x][y]))
        {
            _queue = addQ(_maze->_this_map[x][y], _queue);
            _maze = floodfill(_maze, &_queue);
        }
    }
}