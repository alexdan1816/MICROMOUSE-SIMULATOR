#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "API.h"
#include "maze.h"
// hàm hiển thị queue
void logmess(char *text)
{
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}
void log_queue_contents(queue *_this_queue)
{
    if (_this_queue->top == -1)
    {
        logmess("Queue is empty.");
        return;
    }

    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "Queue has %d elements:", _this_queue->top + 1);
    logmess(buffer);

    for (int i = 0; i <= _this_queue->top; i++)
    {
        Node *node = _this_queue->_queue[i];
        snprintf(buffer, BUFFER_SIZE, "Element %d -> x: %d, y: %d, value: %d", i, node->x, node->y, node->value);
        logmess(buffer);
    }
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
    _newNode->wallup = FALSE;
    _newNode->walldown = FALSE;
    _newNode->wallright = FALSE;
    _newNode->wallleft = FALSE;
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
    for (int i = 0; i < MAZESIZE; i++)
    {
        // Cạnh trái của mê cung -> Tường bên trái
        _maze->_this_map[0][i]->wallleft = TRUE;
        API_setWall(0, i, 'w');

        // Cạnh phải của mê cung -> Tường bên phải
        _maze->_this_map[MAZESIZE - 1][i]->wallright = TRUE;
        API_setWall(MAZESIZE - 1, i, 'e');

        // Cạnh trên của mê cung -> Tường trên
        _maze->_this_map[i][MAZESIZE - 1]->wallup = TRUE;
        API_setWall(i, MAZESIZE - 1, 'n');

        // Cạnh dưới của mê cung -> Tường dưới
        _maze->_this_map[i][0]->walldown = TRUE;
        API_setWall(i, 0, 's');
    }

    // Hiển thị giá trị của mỗi ô lên màn hình mê cung
    for (short i = 0; i < MAZESIZE; i++)
    {
        for (short j = 0; j < MAZESIZE; j++)
        {
            API_setNumber(i, j, _maze->_this_map[i][j]->value);
        }
    }

    return _maze;
}
void set_visited(Node **_this_cell)
{
    API_setColor((*_this_cell)->x, (*_this_cell)->y, 'g');
    (*_this_cell)->visited = 1;
    return;
}
void set_wall(Node *_this_cell, const short direct)
{
    if (direct == NORTH)
    {
        if (API_wallFront())
        {
            _this_cell->wallup = TRUE;
            API_setWall(_this_cell->x, _this_cell->y, 'n');
        }
        if (API_wallLeft())
        {
            _this_cell->wallleft = TRUE;
            API_setWall(_this_cell->x, _this_cell->y, 'w');
        }
        if (API_wallRight())
        {
            _this_cell->wallright = TRUE;
            API_setWall(_this_cell->x, _this_cell->y, 'e');
        }
    }
    if (direct == EAST)
    {
        if (API_wallFront())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'e');
            _this_cell->wallright = TRUE;
        }
        if (API_wallLeft())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'n');
            _this_cell->wallup = TRUE;
        }
        if (API_wallRight())
        {
            API_setWall(_this_cell->x, _this_cell->y, 's');
            _this_cell->walldown = TRUE;
        }
    }
    if (direct == SOUTH)
    {
        if (API_wallFront())
        {
            API_setWall(_this_cell->x, _this_cell->y, 's');
            _this_cell->walldown = TRUE;
        }
        if (API_wallLeft())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'e');
            _this_cell->wallright = TRUE;
        }
        if (API_wallRight())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'w');
            _this_cell->wallleft = TRUE;
        }
    }
    if (direct == WEST)
    {
        if (API_wallFront())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'w');
            _this_cell->wallleft = TRUE;
        }
        if (API_wallLeft())
        {
            API_setWall(_this_cell->x, _this_cell->y, 's');
            _this_cell->walldown = TRUE;
        }
        if (API_wallRight())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'n');
            _this_cell->wallup = TRUE;
        }
    }
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
        return true;
    }
    else
    {
        return false;
    }
}
queue *addQ(Node *_this_cell, queue *_this_queue)
{
    if (_this_cell->value == 0 || is_in_queue(_this_queue, _this_cell))
    {
        return _this_queue;
    }
    _this_queue->top++;
    _this_queue->_queue[_this_queue->top] = _this_cell;
    return _this_queue;
}
Node *popQ(queue *_this_queue)
{
    if (_this_queue->top == -1) // Kiểm tra nếu hàng đợi trống
    {
        logmess("Error: Attempting to pop from an empty queue.");
        return NULL;
    }
    _this_queue->top--;
    return _this_queue->_queue[_this_queue->top + 1];
}
queue *add_all_neighbors(Node *_this_node, queue *_this_queue, maze *_maze)
{
    // Kiểm tra nếu ô nằm ở cạnh phải
    short x = _this_node->x;
    short y = _this_node->y;
    if (_this_node->right != NULL && _this_node->right->value != 0)
    {
        if (!check_for_smallest_neighbors(_this_node->right))
        {
            _this_queue = addQ(_this_node->right, _this_queue);
        }
    }
    if (_this_node->left != NULL && _this_node->left->value != 0)
    {
        if (!check_for_smallest_neighbors(_this_node->left))
        {
            _this_queue = addQ(_this_node->left, _this_queue);
        }
    }
    if (_this_node->up != NULL && _this_node->up->value != 0)
    {
        if (!check_for_smallest_neighbors(_this_node->up))
        {
            _this_queue = addQ(_this_node->up, _this_queue);
        }
    }
    if (_this_node->down != NULL && _this_node->down->value != 0)
    {
        if (!check_for_smallest_neighbors(_this_node->down))
        {
            _this_queue = addQ(_this_node->down, _this_queue);
        }
    }
    return _this_queue;
}
bool is_in_queue(queue *_this_queue, Node *_this_cell)
{
    for (int i = 0; i <= _this_queue->top; i++)
    {
        if (_this_queue->_queue[i] == _this_cell)
        {
            return true;
        }
    }
    return false;
}

bool check_for_smallest_neighbors(Node *_this_cell)
{
    int current_value = _this_cell->value;
    // Kiểm tra ô phía Bắc
    if (_this_cell->up != NULL && _this_cell->wallup == 0 && _this_cell->up->value + 1 == current_value)
    {
        return true;
    }
    // Kiểm tra ô phía Nam
    if (_this_cell->down != NULL && _this_cell->walldown == 0 && _this_cell->down->value + 1 == current_value)
    {
        return true;
    }
    // Kiểm tra ô phía Đông
    if (_this_cell->right != NULL && _this_cell->wallright == 0 && _this_cell->right->value + 1 == current_value)
    {
        return true;
    }
    // Kiểm tra ô phía Tây
    if (_this_cell->left != NULL && _this_cell->wallleft == 0 && _this_cell->left->value + 1 == current_value)
    {
        return true;
    }
    return false;
}
int get_smallest_neighbor_value(Node *_this_cell)
{
    int min = HIGHESTVAL;
    if (_this_cell->up != NULL && _this_cell->wallup == 0 && _this_cell->up->value < min)
    {
        min = _this_cell->up->value;
    }
    // check down
    if (_this_cell->down != NULL && _this_cell->walldown == 0 && _this_cell->down->value < min)
    {
        min = _this_cell->down->value;
    }
    // check right
    if (_this_cell->right != NULL && _this_cell->wallright == 0 && _this_cell->right->value < min)
    {
        min = _this_cell->right->value;
    }
    // check left
    if (_this_cell->left != NULL && _this_cell->wallleft == 0 && _this_cell->left->value < min)
    {
        min = _this_cell->left->value;
    }
    return min;
}
short get_smallest_neighbor_dir(Node *_this_cell)
{
    int min = get_smallest_neighbor_value(_this_cell);

    // Kiểm tra từng hướng để tìm ô có giá trị nhỏ nhất
    if (_this_cell->right != NULL && !_this_cell->wallright && _this_cell->right->value == min)
    {
        return EAST;
    }
    if (_this_cell->up != NULL && !_this_cell->wallup && _this_cell->up->value == min)
    {
        return NORTH;
    }
    if (_this_cell->down != NULL && !_this_cell->walldown && _this_cell->down->value == min)
    {
        return SOUTH;
    }
    if (_this_cell->left != NULL && !_this_cell->wallleft && _this_cell->left->value == min)
    {
        return WEST;
    }

    return -1; // Không tìm thấy hướng hợp lệ
}

maze *floodfill(maze *_maze, queue *_queue)
{
    while (!isEmpty(_queue))
    {
        Node *_this_node = popQ(_queue);
        if (!check_for_smallest_neighbors(_this_node))
        {
            short x = _this_node->x;
            short y = _this_node->y;
            logmess("floodfilling");
            _maze->_this_map[x][y]->value = get_smallest_neighbor_value(_this_node) + 1;
            API_setNumber(x, y, _maze->_this_map[x][y]->value);
            _queue = add_all_neighbors(_this_node, _queue, _maze);
        }
    }
    return _maze;
}
