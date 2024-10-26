#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "API.h"
#include "maze.h"
// Function to create a new Node
Node *new_Node(short x, short y)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->x = x;
    node->y = y;
    node->value = 0;   // Mặc định giá trị là 0
    node->visited = 0; // Chưa được thăm
    node->up = NULL;
    node->down = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}
// Function to create a new Maze
struct maze *mazeinit()
{
    log("creating maze...");
    struct maze *_maze = (struct maze *)malloc(sizeof(struct maze));
    // init cells in maze
    for (int i = 0; i < MAZESIZE; i++)
    {
        for (int j = 0; j < MAZESIZE; j++)
        {
            if (i == 7 && j == 7 || i == 7 && j == 8 || i == 8 && j == 7 || i == 8 && j == 8) // set up for 4 center cells
            {
                _maze->map[i][j] = new_Node(i, j);
                _maze->map[i][j]->value = 0;
                char text[BUFFER_SIZE];
                snprintf(text, BUFFER_SIZE, "%d", 0);
                API_setText(i, j, text);
            }
            else if (i <= 7 && j <= 7) // caculate value of cells based on location
            {
                _maze->map[i][j] = new_Node(i, j);
                _maze->map[i][j]->value = (7 - i) + (7 - j);
                // display value on the cell
                char text[BUFFER_SIZE];
                snprintf(text, BUFFER_SIZE, "%d", _maze->map[i][j]->value);
                API_setText(i, j, text);
            }
            else if (i <= 7 && j >= 8)
            {
                _maze->map[i][j] = new_Node(i, j);
                _maze->map[i][j]->value = (7 - i) + abs(8 - j);
                char text[BUFFER_SIZE];
                snprintf(text, BUFFER_SIZE, "%d", _maze->map[i][j]->value);
                API_setText(i, j, text);
            }
            else if (i >= 8 && j <= 7)
            {
                _maze->map[i][j] = new_Node(i, j);
                _maze->map[i][j]->value = abs(8 - i) + (7 - j);
                char text[BUFFER_SIZE];
                snprintf(text, BUFFER_SIZE, "%d", _maze->map[i][j]->value);
                API_setText(i, j, text);
            }
            else if (i >= 8 && j >= 8)
            {
                _maze->map[i][j] = new_Node(i, j);
                _maze->map[i][j]->value = abs(8 - i) + abs(8 - j);
                char text[BUFFER_SIZE];
                snprintf(text, BUFFER_SIZE, "%d", _maze->map[i][j]->value);
                API_setText(i, j, text);
            }
        }
    }
    for (int i = 0; i < MAZESIZE; i++) // connecting cells
    {
        for (int j = 0; j < MAZESIZE; j++)
        {
            if (i == 0 && j == 0)
            {
                _maze->map[i][j]->left = NULL;
                _maze->map[i][j]->down = NULL;
                _maze->map[i][j]->up = _maze->map[i][j + 1];
                _maze->map[i][j]->right = _maze->map[i + 1][j];
            }
            else if (i == 0 && j == MAZESIZE - 1)
            {
                _maze->map[i][j]->left = NULL;
                _maze->map[i][j]->up = NULL;
                _maze->map[i][j]->right = _maze->map[i + 1][j];
                _maze->map[i][j]->down = _maze->map[i][j - 1];
            }
            else if (i == MAZESIZE - 1 && j == MAZESIZE - 1)
            {
                _maze->map[i][j]->right = NULL;
                _maze->map[i][j]->up = NULL;
                _maze->map[i][j]->left = _maze->map[i - 1][j];
                _maze->map[i][j]->down = _maze->map[i][j - 1];
            }
            else if (i == MAZESIZE - 1 && j == 0)
            {
                _maze->map[i][j]->right = NULL;
                _maze->map[i][j]->down = NULL;
                _maze->map[i][j]->left = _maze->map[i - 1][j];
                _maze->map[i][j]->up = _maze->map[i][j + 1];
            }
            else if (i == 0 && j != 0 && j != MAZESIZE - 1) // lefthand edge
            {
                _maze->map[i][j]->left = NULL;
                _maze->map[i][j]->right = _maze->map[i + 1][j];
                _maze->map[i][j]->up = _maze->map[i][j + 1];
                _maze->map[i][j]->down = _maze->map[i][j - 1];
            }
            else if (i != 0 && i != MAZESIZE - 1 && j == MAZESIZE - 1) // top edge
            {
                _maze->map[i][j]->up = NULL;
                _maze->map[i][j]->right = _maze->map[i + 1][j];
                _maze->map[i][j]->left = _maze->map[i - 1][j];
                _maze->map[i][j]->down = _maze->map[i][j - 1];
            }
            else if (j != 0 && j != MAZESIZE - 1 && i == MAZESIZE - 1) // righthand edge
            {
                _maze->map[i][j]->right = NULL;
                _maze->map[i][j]->left = _maze->map[i - 1][j];
                _maze->map[i][j]->up = _maze->map[i][j + 1];
                _maze->map[i][j]->down = _maze->map[i][j - 1];
            }
            else if (i != 0 && i != MAZESIZE - 1 && j == 0) // bottom edge
            {
                _maze->map[i][j]->down = NULL;
                _maze->map[i][j]->right = _maze->map[i + 1][j];
                _maze->map[i][j]->left = _maze->map[i - 1][j];
                _maze->map[i][j]->up = _maze->map[i][j + 1];
            }
            else
            {
                _maze->map[i][j]->down = _maze->map[i][j - 1];
                _maze->map[i][j]->right = _maze->map[i + 1][j];
                _maze->map[i][j]->left = _maze->map[i - 1][j];
                _maze->map[i][j]->up = _maze->map[i][j + 1];
            }
        }
    }
    log("done creating maze...");
    return _maze;
}

struct LIFOqueue *QueueInit() // initialize Queue
{
    struct LIFOqueue *_queue = (struct LIFOqueue *)malloc(sizeof(struct LIFOqueue));
    if (_queue == NULL)
    {
        return NULL;
    }

    // Khởi tạo tất cả các phần tử trong mảng Q là NULL
    for (int i = 0; i < QSIZE; i++)
    {
        _queue->Q[i] = NULL;
    }
    _queue->top = -1;
    return _queue;
}
struct Node *popQ(struct LIFOqueue **_queue) // take out top cell
{
    if ((*_queue)->top == -1)
    {
        return NULL;
    }
    struct Node *takeout = (*_queue)->Q[(*_queue)->top];
    (*_queue)->Q[(*_queue)->top] = NULL;
    (*_queue)->top--;

    return takeout;
}
struct LIFOqueue *addQ(struct LIFOqueue *_queue, struct Node *_cell) // add cell
{
    _queue->top++;
    _queue->Q[_queue->top] = _cell;
    return _queue;
}

struct maze *reflood(struct LIFOqueue **_queue, struct maze *_maze)
{
    log("reflooding...");
    while ((*_queue)->top != -1)
    {
        log("continue reflooding...");
        struct Node *popped = popQ(_queue);
        int min = 0;
        short xlocate = popped->x;
        short ylocate = popped->y;
        bool flag = false;                              // if current node has available cells (lower value than current)
        if (_maze->map[xlocate][ylocate]->down != NULL) // if this cell available
        {
            if (_maze->map[xlocate][ylocate]->value == _maze->map[xlocate][ylocate]->down->value + 1) // if this cell's value lower than popped cell add this cell to queue
            {
                (*_queue) = addQ(*_queue, _maze->map[xlocate][ylocate]->down);
                flag = true;
            }
            else // if this cell's value higher or equal to popped cell, take minimum value is this cell's value
            {
                min = _maze->map[xlocate][ylocate]->down->value;
            }
        }
        if (_maze->map[xlocate][ylocate]->up != NULL)
        {
            if (_maze->map[xlocate][ylocate]->value == _maze->map[xlocate][ylocate]->up->value + 1)
            {
                (*_queue) = addQ((*_queue), _maze->map[xlocate][ylocate]->up);
                flag = true;
            }
            else
            {
                if (min == 0)
                {
                    min = _maze->map[xlocate][ylocate]->up->value;
                }
                else if (min > _maze->map[xlocate][ylocate]->up->value)
                {
                    min = _maze->map[xlocate][ylocate]->up->value;
                }
            }
        }
        if (_maze->map[xlocate][ylocate]->left != NULL)
        {
            if (_maze->map[xlocate][ylocate]->value = _maze->map[xlocate][ylocate]->left->value + 1)
            {
                (*_queue) = addQ((*_queue), _maze->map[xlocate][ylocate]->left);
                flag = true;
            }
            else
            {
                if (min == 0)
                {
                    min = _maze->map[xlocate][ylocate]->left->value;
                }
                else if (min > _maze->map[xlocate][ylocate]->left->value)
                {
                    min = _maze->map[xlocate][ylocate]->left->value;
                }
            }
        }
        if (_maze->map[xlocate][ylocate]->right != NULL)
        {
            if (_maze->map[xlocate][ylocate]->value > _maze->map[xlocate][ylocate]->right->value)
            {
                (*_queue) = addQ((*_queue), _maze->map[xlocate][ylocate]->right);
                flag = true;
            }
            else
            {
                if (min == 0)
                {
                    min = _maze->map[xlocate][ylocate]->right->value;
                }
                else if (min > _maze->map[xlocate][ylocate]->right->value)
                {
                    min = _maze->map[xlocate][ylocate]->right->value;
                }
            }
        }

        if (flag == false) // no available cell then reflood this cell and change value
        {
            log("changing this cell's value...");
            _maze->map[xlocate][ylocate]->value = min + 1;
            char text[BUFFER_SIZE];
            snprintf(text, BUFFER_SIZE, "%d", min + 1);
            API_setText(xlocate, ylocate, text);
        }
    }
    return _maze;
}
// Maze explore
void setwall(int direct, short x, short y, struct maze **_maze) // set wall and connect cellnode
{
    log("start setting wall...");
    if (direct == NORTH)
    {
        if (API_wallFront())
        {
            API_setWall(x, y, 'n');
            (*_maze)->map[x][y]->up = NULL;
        }
        if (API_wallLeft())
        {
            API_setWall(x, y, 'w');
            (*_maze)->map[x][y]->left = NULL;
        }
        if (API_wallRight())
        {
            API_setWall(x, y, 'e');
            (*_maze)->map[x][y]->right = NULL;
        }
    }
    else if (direct == EAST)
    {

        if (API_wallFront())
        {
            API_setWall(x, y, 'e');
            (*_maze)->map[x][y]->right = NULL;
        }
        if (API_wallLeft())
        {
            API_setWall(x, y, 'n');
            (*_maze)->map[x][y]->up = NULL;
        }
        if (API_wallRight())
        {
            API_setWall(x, y, 's');
            (*_maze)->map[x][y]->down = NULL;
        }
    }
    else if (direct == SOUTH)
    {
        if (API_wallFront())
        {
            API_setWall(x, y, 's');
            (*_maze)->map[x][y]->down = NULL;
        }
        if (API_wallLeft())
        {
            API_setWall(x, y, 'e');
            (*_maze)->map[x][y]->right = NULL;
        }
        if (API_wallRight())
        {
            API_setWall(x, y, 'w');
            (*_maze)->map[x][y]->left = NULL;
        }
    }
    else if (direct == WEST)
    {
        if (API_wallFront())
        {
            API_setWall(x, y, 'w');
            (*_maze)->map[x][y]->left = NULL;
        }
        if (API_wallLeft())
        {
            API_setWall(x, y, 's');
            (*_maze)->map[x][y]->down = NULL;
        }
        if (API_wallRight())
        {
            API_setWall(x, y, 'n');
            (*_maze)->map[x][y]->up = NULL;
        }
    }
}
void move(short *x, short *y, struct maze **_maze, struct LIFOqueue **_queue, int *direct)
{
    if (*direct == NORTH)
    {
        if (!API_wallFront())
        {
            if ((*_maze)->map[*x][*y]->value == (*_maze)->map[*x][*y]->up->value + 1)
            {
                log("move forward...");
                API_moveForward();
                *y = *y + 1;
                return;
            }
        }
        if (!API_wallLeft())
        {
            if ((*_maze)->map[*x][*y]->value == (*_maze)->map[*x][*y]->left->value + 1)
            {
                log("turn left...");
                API_turnLeft();
                API_moveForward();
                *direct = WEST;
                *x = *x - 1;
                return;
            }
        }
        if (!API_wallRight())
        {
            if ((*_maze)->map[*x][*y]->value == (*_maze)->map[*x][*y]->right->value + 1)
            {
                log("turn right...");
                API_turnRight();
                API_moveForward();
                *direct = EAST;
                *x = *x + 1;
                return;
            }
        }
        log("turn aroung");
        API_turnLeft();
        API_turnLeft();
        *direct = SOUTH;
        (*_queue) = addQ(*_queue, (*_maze)->map[*x][*y]);
        (*_maze) = reflood(_queue, *_maze);
    }
    if (*direct == EAST)
    {
        if (!API_wallFront())
        {
            if ((*_maze)->map[*x][*y]->value > (*_maze)->map[*x][*y]->right->value)
            {
                log("move forward...");
                API_moveForward();
                *x = *x + 1;
                return;
            }
        }
        if (!API_wallLeft())
        {
            if ((*_maze)->map[*x][*y]->value > (*_maze)->map[*x][*y]->up->value)
            {
                log("turn left...");
                API_turnLeft();
                API_moveForward();
                *direct = NORTH;
                *y = *y + 1;
                return;
            }
        }
        if (!API_wallRight())
        {
            if ((*_maze)->map[*x][*y]->value > (*_maze)->map[*x][*y]->down->value)
            {
                log("turn right...");
                API_turnRight();
                API_moveForward();
                *direct = SOUTH;
                *y = *y - 1;
                return;
            }
            log("turn aroung");
            API_turnLeft();
            API_turnLeft();
            *direct = WEST;
            (*_queue) = addQ(*_queue, (*_maze)->map[*x][*y]);
            (*_maze) = reflood(_queue, *_maze);
        }
    }
    if (*direct == SOUTH)
    {
        if (!API_wallFront())
        {
            log("move forward...");
            if ((*_maze)->map[*x][*y]->value > (*_maze)->map[*x][*y]->down->value)
            {
                API_moveForward();
                *y = *y - 1;
                return;
            }
        }
        if (!API_wallLeft())
        {
            if ((*_maze)->map[*x][*y]->value > (*_maze)->map[*x][*y]->right->value)
            {
                log("turn left...");
                API_turnLeft();
                API_moveForward();
                *direct = EAST;
                *x = *x + 1;
                return;
            }
        }
        if (!API_wallRight())
        {
            if ((*_maze)->map[*x][*y]->value > (*_maze)->map[*x][*y]->left->value)
            {
                log("turn right...");
                API_turnRight();
                API_moveForward();
                *direct = WEST;
                *y = *y - 1;
                return;
            }
        }
        log("turn aroung");
        API_turnLeft();
        API_turnLeft();
        *direct = NORTH;
        (*_queue) = addQ(*_queue, (*_maze)->map[*x][*y]);
        (*_maze) = reflood(_queue, *_maze);
    }
    if (*direct == WEST)
    {
        if (!API_wallFront())
        {
            if ((*_maze)->map[*x][*y]->value > (*_maze)->map[*x][*y]->left->value)
            {
                log("move forward...");
                API_moveForward();
                *x = *x - 1;
                return;
            }
        }
        if (!API_wallLeft())
        {
            if ((*_maze)->map[*x][*y]->value > (*_maze)->map[*x][*y]->down->value)
            {
                log("turn left...");
                API_turnLeft();
                API_moveForward();
                *direct = SOUTH;
                *y = *y - 1;
                return;
            }
        }
        if (!API_wallRight())
        {
            if ((*_maze)->map[*x][*y]->value > (*_maze)->map[*x][*y]->up->value)
            {
                log("turn right...");
                API_turnRight();
                API_moveForward();
                *direct = NORTH;
                *y = *y + 1;
                return;
            }
        }
        log("turn aroung");
        API_turnLeft();
        API_turnLeft();
        *direct = EAST;
        (*_queue) = addQ(*_queue, (*_maze)->map[*x][*y]);
        (*_maze) = reflood(_queue, *_maze);
    }
}
