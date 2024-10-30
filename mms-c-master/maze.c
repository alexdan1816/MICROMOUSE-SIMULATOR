#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "API.h"
#include "maze.h"

void logmess(char *text)
{
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}
// functions for node
Node *createNode(const short x, const short y, const int value)
{
    Node *_newNode = (struct Node *)malloc(sizeof(struct Node));
    _newNode->x = x;
    _newNode->y = y;
    _newNode->value = value;
    _newNode->visited = 0;
    _newNode->down = NULL;
    _newNode->left = NULL;
    _newNode->right = NULL;
    _newNode->up = NULL;
    return _newNode;
}
void deleteNode(Node **_this_node)
{
    free(*(_this_node));
    return;
}

// function for maze
maze *maze_init(maze *_maze)
{
    _maze = (maze *)malloc(sizeof(maze));
    if (!_maze)
    {
        fprintf(stderr, "Memory allocation failed for maze.\n");
        return NULL;
    }

    // Khởi tạo từng ô trong mê cung với giá trị mặc định
    for (short i = 0; i < MAZESIZE; i++)
    {
        for (short j = 0; j < MAZESIZE; j++)
        {
            int value;
            // Thiết lập giá trị cho từng vị trí
            if ((i == (MAZESIZE / 2) || i == (MAZESIZE / 2) - 1) &&
                (j == (MAZESIZE / 2) || j == (MAZESIZE / 2) - 1))
            {
                value = 0; // Ô đích ở giữa mê cung
            }
            else if (i <= 7 && j <= 7)
            {
                value = (7 - i) + (7 - j);
            }
            else if (i <= 7 && j >= 8)
            {
                value = (7 - i) + (j - 8);
            }
            else if (i >= 8 && j <= 7)
            {
                value = (i - 8) + (7 - j);
            }
            else
            {
                value = (i - 8) + (j - 8);
            }
            _maze->_this_map[i][j] = createNode(i, j, value);
        }
    }

    // Thiết lập các con trỏ cho từng ô để liên kết với ô liền kề
    for (short i = 0; i < MAZESIZE; i++)
    {
        for (short j = 0; j < MAZESIZE; j++)
        {
            _maze->_this_map[i][j]->up = (j < MAZESIZE - 1) ? _maze->_this_map[i][j + 1] : NULL;
            _maze->_this_map[i][j]->down = (j > 0) ? _maze->_this_map[i][j - 1] : NULL;
            _maze->_this_map[i][j]->left = (i > 0) ? _maze->_this_map[i - 1][j] : NULL;
            _maze->_this_map[i][j]->right = (i < MAZESIZE - 1) ? _maze->_this_map[i + 1][j] : NULL;
        }
    }

    // Xử lý các góc và cạnh của mê cung
    _maze->_this_map[0][0]->left = NULL; // Góc dưới trái
    _maze->_this_map[0][0]->down = NULL;
    _maze->_this_map[0][MAZESIZE - 1]->left = NULL; // Góc trên trái
    _maze->_this_map[0][MAZESIZE - 1]->up = NULL;
    _maze->_this_map[MAZESIZE - 1][0]->right = NULL; // Góc dưới phải
    _maze->_this_map[MAZESIZE - 1][0]->down = NULL;
    _maze->_this_map[MAZESIZE - 1][MAZESIZE - 1]->right = NULL; // Góc trên phải
    _maze->_this_map[MAZESIZE - 1][MAZESIZE - 1]->up = NULL;

    for (short i = 1; i < MAZESIZE - 1; i++)
    {
        _maze->_this_map[i][0]->down = NULL;             // Cạnh dưới
        _maze->_this_map[i][MAZESIZE - 1]->up = NULL;    // Cạnh trên
        _maze->_this_map[0][i]->left = NULL;             // Cạnh trái
        _maze->_this_map[MAZESIZE - 1][i]->right = NULL; // Cạnh phải
    }

    // Hiển thị giá trị của mỗi ô lên màn hình mê cung
    for (short i = 0; i < MAZESIZE; i++)
    {
        for (short j = 0; j < MAZESIZE; j++)
        {
            char text[BUFFER_SIZE];
            snprintf(text, BUFFER_SIZE, "%d", _maze->_this_map[i][j]->value);
            API_setText(i, j, text); // Hiển thị giá trị lên ô
        }
    }

    return _maze;
}
void flood_value(Node *_this_node)
{
    _this_node->value = get_smallest_neighbor_value(_this_node) + 1;
    logmess("floodfilling");
    char text[BUFFER_SIZE];
    snprintf(text, BUFFER_SIZE, "%d", _this_node->value);
    API_setText(_this_node->x, _this_node->y, text); // Hiển thị giá trị lên ô
}
void set_visited(Node **_this_cell)
{
    API_setColor((*_this_cell)->x, (*_this_cell)->y, 'g');
    (*_this_cell)->visited = 1;
    return;
}
Node *set_wall(Node *_this_cell, const short direct)
{
    if (direct == NORTH)
    {
        if (API_wallFront())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'n');
            _this_cell->up = NULL;
        }
        if (API_wallLeft())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'w');
            _this_cell->left = NULL;
        }
        if (API_wallRight())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'e');
            _this_cell->right = NULL;
        }
    }
    if (direct == EAST)
    {
        if (API_wallFront())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'e');
            _this_cell->right = NULL;
        }
        if (API_wallLeft())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'n');
            _this_cell->up = NULL;
        }
        if (API_wallRight())
        {
            API_setWall(_this_cell->x, _this_cell->y, 's');
            _this_cell->down = NULL;
        }
    }
    if (direct == SOUTH)
    {
        if (API_wallFront())
        {
            API_setWall(_this_cell->x, _this_cell->y, 's');
            _this_cell->down = NULL;
        }
        if (API_wallLeft())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'e');
            _this_cell->right = NULL;
        }
        if (API_wallRight())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'w');
            _this_cell->left = NULL;
        }
    }
    if (direct == WEST)
    {
        if (API_wallFront())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'w');
            _this_cell->left = NULL;
        }
        if (API_wallLeft())
        {
            API_setWall(_this_cell->x, _this_cell->y, 's');
            _this_cell->down = NULL;
        }
        if (API_wallRight())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'n');
            _this_cell->up = NULL;
        }
    }
    return _this_cell;
}

// function for queue
queue *initQ()
{
    queue *new_Queue = (struct queue *)malloc(sizeof(struct queue));
    new_Queue->top = -1;
    return new_Queue;
}
bool isEmpty(queue *_this_queue)
{
    return _this_queue->top == -1;
}
queue *addQ(Node *_this_cell, queue *_this_queue)
{
    if (_this_cell->value == 0)
    {
        return _this_queue;
    }
    _this_queue->top++;
    _this_queue->_queue[_this_queue->top] = _this_cell;
    return _this_queue;
}
void popQ(queue **_this_queue, Node **_this_node)
{
    (*_this_node) = (*_this_queue)->_queue[(*_this_queue)->top];
    (*_this_queue)->top--;
}
queue *add_all_neighbors(Node *_this_node, queue *_this_queue)
{
    if (_this_node->x == MAZESIZE - 1)
    {
        if (_this_node->y == MAZESIZE - 1) // TOP RIGHT CORNER
        {
            _this_queue = addQ(_this_node->down, _this_queue);
            _this_queue = addQ(_this_node->left, _this_queue);
            return _this_queue;
        }
        else if (_this_node->y == 0) // BOTTOM RIGHT CORNER
        {
            _this_queue = addQ(_this_node->up, _this_queue);
            _this_queue = addQ(_this_node->left, _this_queue);
            return _this_queue;
        }
        else // RIGHT SIDE
        {
            _this_queue = addQ(_this_node->up, _this_queue);
            _this_queue = addQ(_this_node->left, _this_queue);
            _this_queue = addQ(_this_node->down, _this_queue);
            return _this_queue;
        }
    }
    if (_this_node->x == 0)
    {
        if (_this_node->y == MAZESIZE - 1) // TOP LEFT CORNER
        {
            _this_queue = addQ(_this_node->down, _this_queue);
            _this_queue = addQ(_this_node->right, _this_queue);
            return _this_queue;
        }
        else // LEFT SIDE
        {
            _this_queue = addQ(_this_node->up, _this_queue);
            _this_queue = addQ(_this_node->right, _this_queue);
            _this_queue = addQ(_this_node->down, _this_queue);
            return _this_queue;
        }
    }
    if (_this_node->y == MAZESIZE - 1) // UP SIDE
    {
        _this_queue = addQ(_this_node->left, _this_queue);
        _this_queue = addQ(_this_node->right, _this_queue);
        _this_queue = addQ(_this_node->down, _this_queue);
        return _this_queue;
    }
    if (_this_node->y == 0) // DOWN SIDE
    {
        _this_queue = addQ(_this_node->left, _this_queue);
        _this_queue = addQ(_this_node->right, _this_queue);
        _this_queue = addQ(_this_node->up, _this_queue);
        return _this_queue;
    }
}
// floodfill assisting functions
// bool lower_neighbor_check(Node *_this_cell, int curent_value)
// {
//     return curent_value == _this_cell->value + 1;
// }
// bool accessible_neighbor_check(Node *_this_cell, int current_value)
// {
//     if (_this_cell->up != NULL)
//     {
//         if (_this_cell->up->value + 1 == current_value)
//             return true;
//     }
//     else if (_this_cell->down != NULL)
//     {
//         if (_this_cell->down->value + 1 == current_value)
//             return true;
//     }
//     else if (_this_cell->right != NULL)
//     {
//         if (_this_cell->right->value + 1 == current_value)
//             return true;
//     }
//     else if (_this_cell->left != NULL)
//     {
//         if (_this_cell->left->value + 1 == current_value)
//             return true;
//     }
//     else
//         return false;
// }
// int min_value_check(Node *_this_cell)
// {
//     int min = 0;
//     if (_this_cell->up != NULL)
//     {
//         min = _this_cell->up->value;
//     }
//     if (_this_cell->down != NULL)
//     {
//         if (min >= _this_cell->down->value || min == 0)
//         {
//             min = _this_cell->down->value;
//         }
//     }
//     if (_this_cell->left != NULL)
//     {
//         if (min >= _this_cell->left->value || min == 0)
//         {
//             min = _this_cell->left->value;
//         }
//     }
//     if (_this_cell->right != NULL)
//     {
//         if (min >= _this_cell->right->value || min == 0)
//         {
//             min = _this_cell->right->value;
//         }
//     }
//     return min;
// }
// queue *add_neighbors(Node *_this_Cell, queue *_this_queue)
// {
//     if (_this_Cell->up->value == 0)
//     {
//         _this_queue = addQ(_this_Cell->down, _this_queue);
//         _this_queue = addQ(_this_Cell->right, _this_queue);
//         _this_queue = addQ(_this_Cell->left, _this_queue);
//     }
//     else if (_this_Cell->down->value == 0)
//     {
//         _this_queue = addQ(_this_Cell->up, _this_queue);
//         _this_queue = addQ(_this_Cell->right, _this_queue);
//         _this_queue = addQ(_this_Cell->left, _this_queue);
//     }
//     else if (_this_Cell->right->value == 0)
//     {
//         _this_queue = addQ(_this_Cell->up, _this_queue);
//         _this_queue = addQ(_this_Cell->down, _this_queue);
//         _this_queue = addQ(_this_Cell->left, _this_queue);
//     }
//     else if (_this_Cell->left->value == 0)
//     {
//         _this_queue = addQ(_this_Cell->up, _this_queue);
//         _this_queue = addQ(_this_Cell->right, _this_queue);
//         _this_queue = addQ(_this_Cell->down, _this_queue);
//     }
// }
bool check_for_smallest_neighbors(Node *_this_cell)
{
    int current_value = _this_cell->value;
    // check up
    if (_this_cell->up != NULL && _this_cell->up->down != NULL && _this_cell->up->value + 1 == current_value)
    {
        return true;
    }
    // check down
    if (_this_cell->down != NULL && _this_cell->down->up != NULL && _this_cell->down->value + 1 == current_value)
    {
        return true;
    }
    // check right
    if (_this_cell->right != NULL && _this_cell->right->left != NULL && _this_cell->down->value + 1 == current_value)
    {
        return true;
    }
    // check left
    if (_this_cell->left != NULL && _this_cell->left->right != NULL && _this_cell->left->value + 1 == current_value)
    {
        return true;
    }
    return false;
}
int get_smallest_neighbor_value(Node *_this_cell)
{
    int min = HIGHESTVAL;
    if (_this_cell->up != NULL && _this_cell->up->down != NULL && _this_cell->up->value < min)
        ;
    {
        min = _this_cell->up->value;
    }
    // check down
    if (_this_cell->down != NULL && _this_cell->down->up != NULL && _this_cell->down->value < min)
        ;
    {
        min = _this_cell->down->value;
    }
    // check right
    if (_this_cell->right != NULL && _this_cell->right->left != NULL && _this_cell->right->value < min)
        ;
    {
        min = _this_cell->right->value;
    }
    // check left
    if (_this_cell->left != NULL && _this_cell->left->right != NULL && _this_cell->left->value < min)
        ;
    {
        min = _this_cell->left->value;
    }
    return min;
}
short get_smallest_neighbor_dir(Node *_this_cell)
{
    int min = get_smallest_neighbor_value(_this_cell);
    // NORTH CELL
    if (_this_cell->up != NULL && _this_cell->up->value == min)
    {
        return NORTH;
    }
    // SOUTH CELL
    if (_this_cell->down != NULL && _this_cell->down->value == min)
    {
        return SOUTH;
    }
    // EAST CELL
    if (_this_cell->right != NULL && _this_cell->right->value == min)
    {
        return EAST;
    }
    // WEST CELL
    if (_this_cell->left != NULL && _this_cell->left->value == min)
    {
        return WEST;
    }
}
