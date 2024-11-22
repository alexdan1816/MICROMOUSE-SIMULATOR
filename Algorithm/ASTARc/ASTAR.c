#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "API.h"
#include "AstarLib.h"
int main(int argc, char *argv[])
{
    logmess("Running...");
    API_setColor(0, 0, 'G');
    API_setText(0, 0, "abc");
    /*MAZE EXPLORING*/
    // create maze
    maze *_maze = NULL;
    _maze = maze_init(_maze);

    // create visit list
    List *_list = create_list();

    // create path stack
    Stack *_path = create_stack();

    // create start point, start direct
    int x = 0;
    int y = 0;
    short direct = NORTH;
    add_list(&_list, x, y);

    // Explore maze
    DFS(&x, &y, &direct, &_list, &_path, &_maze);
    FinishExplore(&x, &y, &direct, &_path);
    free(_list);
    free(_path);

    /*ACTIVATE A STAR*/
    x = 0;
    y = 0;
    // reset value
    for (int i = 0; i < MAZESIZE; i++)
    {
        for (int j = 0; j < MAZESIZE; j++)
        {
            _maze->_this_map[i][j]->status = NONVISITED;
            _maze->_this_map[i][j]->fcost = HIGHESTVAL;
            _maze->_this_map[i][j]->gcost = 0;
        }
    }

    PriorityQueue *_queue = create_queue();

    PQueueNode *_start_node = create_PQNode(x, y, _maze->_this_map[x][y]->hcost, 0); /*Add first cell to queue*/
    _maze->_this_map[0][0]->status = VISITED;
    enqueue(&_queue, _start_node);

    A_star(&_queue, _maze);

    // trace path

    for (int i = 0; i < MAZESIZE; i++) /*reset cell's status*/
    {
        for (int j = 0; j < MAZESIZE; j++)
        {
            _maze->_this_map[i][j]->status = NONVISITED;
        }
    }

    trace_path(&_maze->_this_map[Xgoal][Ygoal]);
    move_to_goal(_maze, &x, &y, &direct);

    free(_queue);
    free_maze(_maze);
    return 0;
}
