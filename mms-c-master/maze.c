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
maze *flood_value(Node **_this_node, maze *_this_maze)
{
    if (*_this_node == NULL || _this_maze == NULL)
    {
        logmess("Error: Null pointer in flood_value.");
        return _this_maze;
    }

    short x = (*_this_node)->x;
    short y = (*_this_node)->y;

    _this_maze->_this_map[x][y]->value = get_smallest_neighbor_value(*_this_node) + 1;
    (*_this_node)->value = _this_maze->_this_map[x][y]->value;

    char text[BUFFER_SIZE];
    snprintf(text, BUFFER_SIZE, "%d", _this_maze->_this_map[x][y]->value);
    API_setText(x, y, text); // Hiển thị giá trị lên ô
    logmess("done changing value");

    return _this_maze;
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
    if (_this_queue->top == -1)
    {
        logmess("queue is empty");
        return true;
    }
    else
    {
        logmess("queue is not empty");
        return false;
    }
}
queue *addQ(Node *_this_cell, queue *_this_queue)
{
    if (_this_cell->value == 0)
    {
        return _this_queue;
    }
    _this_queue->top++;
    _this_queue->_queue[_this_queue->top] = _this_cell;
    logmess("add this cell to queue");
    return _this_queue;
}
Node *popQ(queue **_this_queue)
{
    if ((*_this_queue)->top == -1) // Kiểm tra nếu hàng đợi trống
    {
        logmess("Error: Attempting to pop from an empty queue.");
        return NULL;
    }
    (*_this_queue)->top--;
    return (*_this_queue)->_queue[(*_this_queue)->top + 1];
}
queue *add_all_neighbors(Node *_this_node, queue *_this_queue)
{
    // Kiểm tra nếu ô nằm ở cạnh phải
    if (_this_node->x == MAZESIZE - 1)
    {
        if (_this_node->y == MAZESIZE - 1) // Góc trên phải
        {
            if (_this_node->down != NULL)
                _this_queue = addQ(_this_node->down, _this_queue);
            if (_this_node->left != NULL)
                _this_queue = addQ(_this_node->left, _this_queue);
        }
        else if (_this_node->y == 0) // Góc dưới phải
        {
            if (_this_node->up != NULL)
                _this_queue = addQ(_this_node->up, _this_queue);
            if (_this_node->left != NULL)
                _this_queue = addQ(_this_node->left, _this_queue);
        }
        else // Cạnh phải (không nằm ở góc)
        {
            if (_this_node->up != NULL)
                _this_queue = addQ(_this_node->up, _this_queue);
            if (_this_node->down != NULL)
                _this_queue = addQ(_this_node->down, _this_queue);
            if (_this_node->left != NULL)
                _this_queue = addQ(_this_node->left, _this_queue);
        }
    }
    // Kiểm tra nếu ô nằm ở cạnh trái
    else if (_this_node->x == 0)
    {
        if (_this_node->y == MAZESIZE - 1) // Góc trên trái
        {
            if (_this_node->down != NULL)
                _this_queue = addQ(_this_node->down, _this_queue);
            if (_this_node->right != NULL)
                _this_queue = addQ(_this_node->right, _this_queue);
        }
        else if (_this_node->y == 0) // Góc dưới trái
        {
            if (_this_node->up != NULL)
                _this_queue = addQ(_this_node->up, _this_queue);
            if (_this_node->right != NULL)
                _this_queue = addQ(_this_node->right, _this_queue);
        }
        else // Cạnh trái (không nằm ở góc)
        {
            if (_this_node->up != NULL)
                _this_queue = addQ(_this_node->up, _this_queue);
            if (_this_node->down != NULL)
                _this_queue = addQ(_this_node->down, _this_queue);
            if (_this_node->right != NULL)
                _this_queue = addQ(_this_node->right, _this_queue);
        }
    }
    // Kiểm tra nếu ô nằm ở cạnh trên (không phải góc)
    else if (_this_node->y == MAZESIZE - 1)
    {
        if (_this_node->left != NULL)
            _this_queue = addQ(_this_node->left, _this_queue);
        if (_this_node->down != NULL)
            _this_queue = addQ(_this_node->down, _this_queue);
        if (_this_node->right != NULL)
            _this_queue = addQ(_this_node->right, _this_queue);
    }
    // Kiểm tra nếu ô nằm ở cạnh dưới (không phải góc)
    else if (_this_node->y == 0)
    {
        if (_this_node->left != NULL)
            _this_queue = addQ(_this_node->left, _this_queue);
        if (_this_node->up != NULL)
            _this_queue = addQ(_this_node->up, _this_queue);
        if (_this_node->right != NULL)
            _this_queue = addQ(_this_node->right, _this_queue);
    }
    else // Trường hợp ô nằm bên trong mê cung (không nằm ở cạnh hay góc)
    {
        if (_this_node->up != NULL)
            _this_queue = addQ(_this_node->up, _this_queue);
        if (_this_node->down != NULL)
            _this_queue = addQ(_this_node->down, _this_queue);
        if (_this_node->left != NULL)
            _this_queue = addQ(_this_node->left, _this_queue);
        if (_this_node->right != NULL)
            _this_queue = addQ(_this_node->right, _this_queue);
    }
    logmess("done adding neighbors");
    return _this_queue;
}

bool check_for_smallest_neighbors(Node *_this_cell)
{
    int current_value = _this_cell->value;
    // check up
    if (_this_cell->up != NULL && _this_cell->up->down != NULL && _this_cell->up->value + 1 == current_value)
    {
        logmess("north cell detected");
        return true;
    }
    // check down
    if (_this_cell->down != NULL && _this_cell->down->up != NULL && _this_cell->down->value + 1 == current_value)
    {
        logmess("south cell detected");
        return true;
    }
    // check right
    if (_this_cell->right != NULL && _this_cell->right->left != NULL && _this_cell->right->value + 1 == current_value)
    {
        logmess("right cell detected");
        return true;
    }
    // check left
    if (_this_cell->left != NULL && _this_cell->left->right != NULL && _this_cell->left->value + 1 == current_value)
    {
        logmess("left cell detected");
        return true;
    }
    logmess("smallest cell non detected");
    return false;
}
int get_smallest_neighbor_value(Node *_this_cell)
{
    int min = HIGHESTVAL;
    if (_this_cell->up != NULL && _this_cell->up->down != NULL && _this_cell->up->value < min)
    {
        min = _this_cell->up->value;
    }
    // check down
    if (_this_cell->down != NULL && _this_cell->down->up != NULL && _this_cell->down->value < min)
    {
        min = _this_cell->down->value;
    }
    // check right
    if (_this_cell->right != NULL && _this_cell->right->left != NULL && _this_cell->right->value < min)
    {
        min = _this_cell->right->value;
    }
    // check left
    if (_this_cell->left != NULL && _this_cell->left->right != NULL && _this_cell->left->value < min)
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
        logmess("North way");
        return NORTH;
    }
    // SOUTH CELL
    if (_this_cell->down != NULL && _this_cell->down->value == min)
    {
        logmess("South way");
        return SOUTH;
    }
    // EAST CELL
    if (_this_cell->right != NULL && _this_cell->right->value == min)
    {
        logmess("Right way");
        return EAST;
    }
    // WEST CELL
    if (_this_cell->left != NULL && _this_cell->left->value == min)
    {
        logmess("Left way");
        return WEST;
    }

    return -1;
}
