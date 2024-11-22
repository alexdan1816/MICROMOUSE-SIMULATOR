#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "API.h"
#include "AstarLib.h"
void logmess(char *text)
{
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}

// *
// *
// *
// function for cell
Node *createNode(const short x, const short y)
{
    Node *_newNode = (struct Node *)malloc(sizeof(struct Node));
    _newNode->x = x;
    _newNode->y = y;

    _newNode->gcost = 0;
    _newNode->hcost = 0;
    _newNode->fcost = 0;

    _newNode->status = NONVISITED;

    _newNode->wallup = FALSE;
    _newNode->walldown = FALSE;
    _newNode->wallright = FALSE;
    _newNode->wallleft = FALSE;

    _newNode->up = NULL;
    _newNode->down = NULL;
    _newNode->right = NULL;
    _newNode->left = NULL;

    _newNode->trace = FALSE;
    _newNode->parent = NULL;
    return _newNode;
}
// *
// *
// *
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
            _maze->_this_map[i][j] = createNode(i, j);
            if (i == 0 && j == 0)
            {
                _maze->_this_map[i][j]->gcost = 0;
                _maze->_this_map[i][j]->hcost = abs(i - Xgoal) + abs(j - Ygoal);
                _maze->_this_map[i][j]->fcost = DEFAULTVAL;
                _maze->_this_map[i][j]->status = 1;
            }
            _maze->_this_map[i][j]->gcost = 0;
            _maze->_this_map[i][j]->hcost = abs(i - Xgoal) + abs(j - Ygoal);
            _maze->_this_map[i][j]->fcost = DEFAULTVAL;
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
        _maze->_this_map[0][i]->wallleft = TRUE;
        API_setWall(0, i, 'w');
        _maze->_this_map[MAZESIZE - 1][i]->wallright = TRUE;
        API_setWall(MAZESIZE - 1, i, 'e');
        _maze->_this_map[i][MAZESIZE - 1]->wallup = TRUE;
        API_setWall(i, MAZESIZE - 1, 'n');
        _maze->_this_map[i][0]->walldown = TRUE;
        API_setWall(i, 0, 's');
    }

    // Hiển thị giá trị của mỗi ô lên màn hình mê cung
    for (short i = 0; i < MAZESIZE; i++)
    {
        for (short j = 0; j < MAZESIZE; j++)
        {
            API_setNumber(i, j, _maze->_this_map[i][j]->fcost);
        }
    }
    return _maze;
}
// *
// *
// *
// function for stack path for dfs
Stack *create_stack()
{
    Stack *_new_part = (Stack *)malloc(sizeof(Stack));
    _new_part->x = 0;
    _new_part->y = 0;
    _new_part->next = NULL;
    return _new_part;
}
void add_path(Stack **_path, int _x, int _y)
{
    Stack *_new_path_part = create_stack();
    if (_new_path_part == NULL) // Kiểm tra cấp phát bộ nhớ
    {
        logmess("Failed to allocate memory for new path part.");
        exit(1); // Thoát hoặc xử lý lỗi tùy ý
    }

    _new_path_part->x = _x;
    _new_path_part->y = _y;

    // Thêm phần tử vào đầu ngăn xếp
    _new_path_part->next = (*_path);
    (*_path) = _new_path_part;
}
void delete_path(Stack **_path)
{
    if (_path == NULL || *_path == NULL) // Kiểm tra con trỏ và nội dung
    {
        logmess("Path stack is empty. Nothing to delete.");
        return; // Không làm gì nếu ngăn xếp rỗng
    }

    Stack *_temp = (*_path);   // Lưu đỉnh ngăn xếp hiện tại
    (*_path) = (*_path)->next; // Di chuyển đỉnh sang phần tử tiếp theo
    free(_temp);               // Giải phóng bộ nhớ của đỉnh cũ
    return;
}
Stack *peek_path(Stack *_path)
{
    return _path;
}
bool is_empty_path(Stack *_path)
{
    return _path == NULL;
}
// *
// *
// *
// functions for visit list for dfs
List *create_list()
{
    List *_new_list = (List *)malloc(sizeof(List));
    if (_new_list == NULL)
    {
        logmess("Failed to allocate memory for List.");
        exit(1);
    }
    for (int i = 0; i < CELLQUANTITY; i++)
    {
        _new_list->List[i] = (List_part *)malloc(sizeof(List_part));
        if (_new_list->List[i] == NULL)
        {
            logmess("Failed to allocate memory for List part.");
            exit(1);
        }
    }
    _new_list->n = 0;
    return _new_list;
}
bool is_visited(List *_list, int _x, int _y)
{
    if (_list == NULL)
    {
        printf("Error: List does not exist.\n");
        return FALSE;
    }
    for (int i = 0; i < _list->n; i++)
    {
        if (_list->List[i]->x == _x && _list->List[i]->y == _y)
        {
            return TRUE; // Trả về TRUE nếu tìm thấy
        }
    }
    return FALSE; // Nếu không, trả về FALSE
}
bool is_full(List *_list)
{
    return _list->n == CELLQUANTITY;
}
void add_list(List **_list, int _x, int _y)
{
    (*_list)->List[(*_list)->n]->x = _x;
    (*_list)->List[(*_list)->n]->y = _y;
    (*_list)->n++;
    return;
}
// *
// *
// *
// function for display
void logList(List *_list)
{
    if (_list == NULL || _list->n == 0)
    {
        logmess("Visited List is empty.");
        return;
    }

    char buffer[256];
    sprintf(buffer, "Visited List (%d items):", _list->n);
    logmess(buffer);

    for (int i = 0; i < _list->n; i++)
    {
        sprintf(buffer, "Node %d: (x: %d, y: %d)", i, _list->List[i]->x, _list->List[i]->y);
        logmess(buffer);
    }
}
void logPath(Stack *_path)
{
    if (_path == NULL)
    {
        logmess("Path Stack is empty.");
        return;
    }

    logmess("Current Path Stack:");
    char buffer[256];
    int index = 0;

    while (_path != NULL)
    {
        sprintf(buffer, "Step %d: (x: %d, y: %d)", index++, _path->x, _path->y);
        logmess(buffer);
        _path = _path->next;
    }
}
// *
// *
// *
// DFS FOR EXPLORING MAZE
short next_direction_dfs(List *_list, int _x, int _y, short _current_dir)
{
    if (_current_dir == NORTH)
    {
        if (!API_wallFront())
        {
            if (!is_visited(_list, _x, _y + 1))
            {
                return NORTH;
            }
        }
        if (!API_wallLeft())
        {
            if (!is_visited(_list, _x - 1, _y))
            {
                return WEST;
            }
        }
        if (!API_wallRight())
        {
            if (!is_visited(_list, _x + 1, _y))
            {
                return EAST;
            }
        }
    }
    if (_current_dir == EAST)
    {
        if (!API_wallFront())
        {
            if (!is_visited(_list, _x + 1, _y))
            {
                return EAST;
            }
        }
        if (!API_wallLeft())
        {
            if (!is_visited(_list, _x, _y + 1))
            {
                return NORTH;
            }
        }
        if (!API_wallRight())
        {
            if (!is_visited(_list, _x, _y - 1))
            {
                return SOUTH;
            }
        }
    }
    if (_current_dir == SOUTH)
    {
        if (!API_wallFront())
        {
            if (!is_visited(_list, _x, _y - 1))
            {
                return SOUTH;
            }
        }
        if (!API_wallLeft())
        {
            if (!is_visited(_list, _x + 1, _y))
            {
                return EAST;
            }
        }
        if (!API_wallRight())
        {
            if (!is_visited(_list, _x - 1, _y))
            {
                return WEST;
            }
        }
    }
    if (_current_dir == WEST)
    {
        if (!API_wallFront())
        {
            if (!is_visited(_list, _x - 1, _y))
            {
                return WEST;
            }
        }
        if (!API_wallLeft())
        {
            if (!is_visited(_list, _x, _y - 1))
            {
                return SOUTH;
            }
        }
        if (!API_wallRight())
        {
            if (!is_visited(_list, _x, _y + 1))
            {
                return NORTH;
            }
        }
    }

    // logmess("Dead end returning backtrack...");
    return -1;
}
short next_direction_backtrack(Stack **_path, short current_dir, int cur_x, int cur_y)
{
    if (*_path == NULL)
    {
        logmess("Path stack is empty. No backtrack direction.");
        return -1; // Ngăn xếp trống, không thể backtrack
    }

    Stack *_next_cell = peek_path((*_path));
    if (_next_cell == NULL)
    {
        logmess("No valid cell to backtrack to.");
        return -1;
    }

    if (cur_x < _next_cell->x)
    {
        // logmess("east find");
        return EAST;
    }
    else if (cur_x > _next_cell->x)
    {
        // logmess("west find");
        return WEST;
    }
    else if (cur_y < _next_cell->y)
    {
        // logmess("north find");
        return NORTH;
    }
    else if (cur_y > _next_cell->y)
    {
        // logmess("south find");
        return SOUTH;
    }
    else
    {
        logmess("No movement needed.");
        return -1;
    }
}
void set_wall(Node *_this_cell, short direct)
{
    if (direct == NORTH)
    {
        if (API_wallFront())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'n');
            _this_cell->wallup = TRUE;
        }
        if (API_wallLeft())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'w');
            _this_cell->wallleft = TRUE;
        }
        if (API_wallRight())
        {
            API_setWall(_this_cell->x, _this_cell->y, 'e');
            _this_cell->wallright = TRUE;
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
void move_and_update(int *x, int *y, short next_dir, short *current_dir)
{
    // Xác định hành động dựa trên hướng hiện tại và hướng tiếp theo
    if (*current_dir == next_dir)
    {
        // Đi thẳng
        API_moveForward();
    }
    else if ((*current_dir == NORTH && next_dir == EAST) ||
             (*current_dir == EAST && next_dir == SOUTH) ||
             (*current_dir == SOUTH && next_dir == WEST) ||
             (*current_dir == WEST && next_dir == NORTH))
    {
        // Quay phải
        API_turnRight();
        API_moveForward();
    }
    else if ((*current_dir == NORTH && next_dir == WEST) ||
             (*current_dir == WEST && next_dir == SOUTH) ||
             (*current_dir == SOUTH && next_dir == EAST) ||
             (*current_dir == EAST && next_dir == NORTH))
    {
        // Quay trái
        API_turnLeft();
        API_moveForward();
    }

    // Cập nhật vị trí dựa trên hướng tiếp theo
    switch (next_dir)
    {
    case NORTH:
        (*y)++;
        break;
    case EAST:
        (*x)++;
        break;
    case SOUTH:
        (*y)--;
        break;
    case WEST:
        (*x)--;
        break;
    }

    // Đánh dấu vị trí đã đi qua
    API_setColor(*x, *y, 'Y');
    *current_dir = next_dir;
}
void move_no_update(int *x, int *y, short next_dir, short *current_dir)
{
    // Xác định hành động dựa trên hướng hiện tại và hướng tiếp theo
    if (*current_dir == next_dir)
    {
        // Đi thẳng
        API_moveForward();
    }
    else if ((*current_dir == NORTH && next_dir == EAST) ||
             (*current_dir == EAST && next_dir == SOUTH) ||
             (*current_dir == SOUTH && next_dir == WEST) ||
             (*current_dir == WEST && next_dir == NORTH))
    {
        // Quay phải
        API_turnRight();
        API_moveForward();
    }
    else if ((*current_dir == NORTH && next_dir == WEST) ||
             (*current_dir == WEST && next_dir == SOUTH) ||
             (*current_dir == SOUTH && next_dir == EAST) ||
             (*current_dir == EAST && next_dir == NORTH))
    {
        // Quay trái
        API_turnLeft();
        API_moveForward();
    }
    else
    {
        API_turnLeft();
        API_turnLeft();
        API_moveForward();
    }

    // Cập nhật vị trí dựa trên hướng tiếp theo
    switch (next_dir)
    {
    case NORTH:
        (*y)++;
        break;
    case EAST:
        (*x)++;
        break;
    case SOUTH:
        (*y)--;
        break;
    case WEST:
        (*x)--;
        break;
    }

    *current_dir = next_dir;
}
void DFS(int *x, int *y, short *direct, List **_list, Stack **_path, maze **_maze)
{
    set_wall((*_maze)->_this_map[*x][*y], *direct);

    if (!is_full(*_list))
    {
        int next_dir = next_direction_dfs((*_list), *x, *y, *direct);
        // char buffer[256];
        // sprintf(buffer, "Current direction : %d", (*direct));
        // logmess(buffer);
        // char buffer1[256];
        // sprintf(buffer1, "Next direction : %d", (next_dir));
        // logmess(buffer1);
        if (next_dir == -1)
        {
            if (*_path == NULL)
            {
                logmess("Path stack is empty. Exploration finished.");
                return; // Kết thúc DFS nếu không còn đường backtrack
            }

            BackTrack(x, y, direct, *_list, _path, _maze);
        }
        else
        {
            move_and_update(x, y, next_dir, direct);
            if (!is_visited(*_list, *x, *y))
            {
                add_list(_list, *x, *y);
                add_path(_path, *x, *y);
                // logList(*_list);
                // logPath(*_path);
            }
            DFS(x, y, direct, _list, _path, _maze);
        }
    }
}
void BackTrack(int *x, int *y, short *direct, List *_list, Stack **_path, maze **_maze)
{
    logmess("Backtracking...");

    if (*_path == NULL)
    {
        logmess("Path stack is empty. Cannot backtrack further.");
        return; // Không thể backtrack nếu ngăn xếp trống
    }

    delete_path(_path);

    if (*_path == NULL)
    {
        logmess("Path stack is empty after deletion. Returning to DFS.");
        return; // Ngăn xếp trống sau khi xóa, quay lại DFS
    }

    int next_dir = next_direction_backtrack(_path, *direct, *x, *y);
    // logmess("Next direction determined:");
    switch (*direct)
    {
    case NORTH:
        // logmess("NORTH");
        break;
    case EAST:
        // logmess("EAST");
        break;
    case SOUTH:
        // logmess("SOUTH");
        break;
    case WEST:
        // logmess("WEST");
        break;
    default:
        logmess("INVALID DIRECTION!");
        break;
    }
    if (*direct == NORTH)
    {
        if (next_dir == NORTH)
        {
            // logmess("move foward...");

            API_moveForward();

            (*y)++;
            *direct = next_dir;
        }
        if (next_dir == EAST)
        {
            // logmess("turn right...");

            API_turnRight();
            API_moveForward();

            (*x)++;
            *direct = next_dir;
        }
        if (next_dir == WEST)
        {
            // logmess("turn left...");

            API_turnLeft();
            API_moveForward();

            (*x)--;
            *direct = next_dir;
        }
        if (next_dir == SOUTH)
        {
            // logmess("turn around...");

            API_turnLeft();
            API_turnLeft();
            API_moveForward();

            (*y)--;
            *direct = next_dir;
        }
    }
    else if (*direct == EAST)
    {
        if (next_dir == EAST)
        {
            // logmess("move foward...");

            API_moveForward();

            (*x)++;
            *direct = next_dir;
        }
        if (next_dir == SOUTH)
        {
            // logmess("turn right...");

            API_turnRight();
            API_moveForward();

            (*y)--;
            *direct = next_dir;
        }
        if (next_dir == NORTH)
        {
            // logmess("turn left...");

            API_turnLeft();
            API_moveForward();

            (*y)++;
            *direct = next_dir;
        }
        if (next_dir == WEST)
        {
            // logmess("turn around...");

            API_turnLeft();
            API_turnLeft();
            API_moveForward();

            (*x)--;
            *direct = next_dir;
        }
    }
    else if (*direct == SOUTH)
    {
        if (next_dir == SOUTH)
        {
            // logmess("move foward...");

            API_moveForward();

            (*y)--;
            *direct = next_dir;
        }
        if (next_dir == WEST)
        {
            // logmess("turn right...");

            API_turnRight();
            API_moveForward();

            (*x)--;
            *direct = next_dir;
        }
        if (next_dir == EAST)
        {
            // logmess("turn left...");

            API_turnLeft();
            API_moveForward();

            (*x)++;
            *direct = next_dir;
        }
        if (next_dir == NORTH)
        {
            // logmess("turn around...");

            API_turnLeft();
            API_turnLeft();
            API_moveForward();

            (*y)++;
            *direct = next_dir;
        }
    }
    else if (*direct == WEST)
    {
        if (next_dir == WEST)
        {
            // logmess("move foward...");

            API_moveForward();

            (*x)--;
            *direct = next_dir;
        }
        if (next_dir == NORTH)
        {
            // logmess("turn right...");

            API_turnRight();
            API_moveForward();

            (*y)++;
            *direct = next_dir;
        }
        if (next_dir == SOUTH)
        {
            // logmess("turn left...");

            API_turnLeft();
            API_moveForward();

            (*y)--;
            *direct = next_dir;
        }
        if (next_dir == EAST)
        {
            // logmess("turn around...");

            API_turnLeft();
            API_turnLeft();
            API_moveForward();

            (*x)++;
            *direct = next_dir;
        }
    }

    DFS(x, y, direct, &_list, _path, _maze);
}
void FinishExplore(int *x, int *y, short *direct, Stack **_path)
{
    if (_path == NULL || *_path == NULL || is_empty_path(*_path))
    {
        logmess("Path stack is empty. Already at starting point.");
        return;
    }
    delete_path(_path);
    while (!is_empty_path(*_path))
    {
        Stack *_next_cell = peek_path((*_path));
        if (_next_cell == NULL)
        {
            logmess("Error: Could not peek next cell.");
            break;
        }

        if (*x == 0 && *y == 0)
        {
            logmess("Reached starting point.");
            break;
        }

        short _next_dir = next_direction_backtrack(_path, *direct, *x, *y);
        if (_next_dir == -1)
        {
            logmess("Invalid backtrack direction.");
            break;
        }

        move_no_update(x, y, _next_dir, direct);
        delete_path(_path);
    }

    // Sau khi quay về xuất phát, xoay robot về hướng ban đầu
    API_turnLeft();
    API_turnLeft();
    if ((*direct) == NORTH)
        *direct = SOUTH;
    else if ((*direct) == EAST)
        *direct = WEST;
    else if ((*direct) == SOUTH)
        *direct = NORTH;
    else if ((*direct) == WEST)
        *direct = EAST;

    logmess("Finished Exploring...");
    logmess("Activate A* Algorithm.");
}
// *
// *
// *
// function for PriorityQueue
PriorityQueue *create_queue()
{
    PriorityQueue *_queue = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    _queue->queue_head = NULL;
    return _queue;
}
PQueueNode *create_PQNode(int _x, int _y, int _h_cost, int _g_cost)
{
    PQueueNode *_new_PQNoode = (PQueueNode *)malloc(sizeof(PQueueNode));

    _new_PQNoode->x = _x;
    _new_PQNoode->y = _y;

    _new_PQNoode->gcost = _g_cost;
    _new_PQNoode->hcost = _h_cost;
    _new_PQNoode->fcost = _g_cost + _h_cost;

    _new_PQNoode->next = NULL;

    return _new_PQNoode;
}

bool isEmptyPQ(PriorityQueue *_queue)
{
    return _queue->queue_head == NULL;
}

void enqueue(PriorityQueue **_queue, PQueueNode *_new_Node)
{
    if (isEmptyPQ((*_queue)))

        (*_queue)->queue_head = _new_Node;

    else
    {
        if (_new_Node->fcost < (*_queue)->queue_head->fcost ||
            _new_Node->fcost == (*_queue)->queue_head->fcost && _new_Node->hcost < (*_queue)->queue_head->hcost)
        {
            _new_Node->next = (*_queue)->queue_head;
            (*_queue)->queue_head = _new_Node;
        }
        else
        {
            // Chèn vào giữa hoặc cuối
            PQueueNode *temp = (*_queue)->queue_head;
            while (temp->next != NULL &&
                   (temp->next->fcost < _new_Node->fcost ||
                    (temp->next->fcost == _new_Node->fcost &&
                     temp->next->hcost <= _new_Node->hcost)))
            {
                temp = temp->next;
            }

            _new_Node->next = temp->next;
            temp->next = _new_Node;
        }
    }
    return;
}
PQueueNode *popQ(PriorityQueue **_queue)
{
    if (isEmptyPQ((*_queue)))
    {
        printf("Queue is empty...");
        return NULL;
    }

    PQueueNode *temp = (*_queue)->queue_head;
    (*_queue)->queue_head = (*_queue)->queue_head->next;

    temp->next = NULL;

    return temp;
}
// *
// *
// *
// A* algorithm
bool isGoal(int x, int y)
{
    return x == Xgoal && y == Ygoal;
}
bool isStart(int x, int y)
{
    return x == Xstart && y == Ystart;
}
void add_child_cell(PQueueNode *current, int child_cell_x, int child_cell_y, PriorityQueue **_queue, maze *_maze)
{
    if (_maze->_this_map[child_cell_x][child_cell_y]->status == VISITED)
        return; // Bỏ qua nếu ô đã được duyệt.

    int new_gcost = current->gcost + 1;
    int new_fcost = new_gcost + _maze->_this_map[child_cell_x][child_cell_y]->hcost;

    if (_maze->_this_map[child_cell_x][child_cell_y]->fcost > new_fcost)
    {
        _maze->_this_map[child_cell_x][child_cell_y]->gcost = new_gcost;
        _maze->_this_map[child_cell_x][child_cell_y]->fcost = new_fcost;

        // Ngăn việc gán parent trỏ về chính nó
        if (current->x == child_cell_x && current->y == child_cell_y)
        {
            logmess("Error: Node trying to set itself as parent.");
            return;
        }

        // Đảm bảo rằng parent được thiết lập đúng
        _maze->_this_map[child_cell_x][child_cell_y]->parent = _maze->_this_map[current->x][current->y];

        // Đánh dấu là đã duyệt
        _maze->_this_map[child_cell_x][child_cell_y]->status = VISITED;

        PQueueNode *child_node = create_PQNode(child_cell_x, child_cell_y,
                                               _maze->_this_map[child_cell_x][child_cell_y]->hcost, new_gcost);
        enqueue(_queue, child_node);
    }
}

void A_star(PriorityQueue **_queue, maze *_maze)
{
    while (!isEmptyPQ(*_queue))
    {
        PQueueNode *_current = popQ(_queue);
        if (isGoal(_current->x, _current->y))
        {
            logmess("Goal reached!");
            free(_current);
            break;
        }

        // Xử lý các ô con
        int x = _current->x;
        int y = _current->y;

        API_setColor(x, y, 'G');

        if (!_maze->_this_map[x][y]->wallup && y + 1 < MAZESIZE)
            add_child_cell(_current, x, y + 1, _queue, _maze);
        if (!_maze->_this_map[x][y]->walldown && y - 1 >= 0)
            add_child_cell(_current, x, y - 1, _queue, _maze);
        if (!_maze->_this_map[x][y]->wallright && x + 1 < MAZESIZE)
            add_child_cell(_current, x + 1, y, _queue, _maze);
        if (!_maze->_this_map[x][y]->wallleft && x - 1 >= 0)
            add_child_cell(_current, x - 1, y, _queue, _maze);

        free(_current);
    }

    logmess("A* Algorithm finished.");
}
void trace_path(Node **_GOAL)
{
    Node *temp = (*_GOAL);
    int loop_count = 0;
    const int MAX_LOOP = 1000; // Giới hạn vòng lặp để tránh tràn bộ nhớ

    logmess("Start tracing path...");

    while (1)
    {
        // Đánh dấu nút đã truy vết
        temp->trace = TRUE;
        API_setColor(temp->x, temp->y, 'B');

        // Dừng nếu không có `parent`
        if (temp->parent == NULL)
        {
            break;
        }

        // Chuyển sang nút `parent`
        temp = temp->parent;
    }

    logmess("Tracing path completed.");
    return;
}

void move_to_goal_step(int *x, int *y, short *direct, short next_direct)
{
    if (*direct == next_direct)
    {
        // Đi thẳng
        API_moveForward();
    }
    else if ((*direct == NORTH && next_direct == EAST) ||
             (*direct == EAST && next_direct == SOUTH) ||
             (*direct == SOUTH && next_direct == WEST) ||
             (*direct == WEST && next_direct == NORTH))
    {
        // Quay phải
        API_turnRight();
        API_moveForward();
    }
    else if ((*direct == NORTH && next_direct == WEST) ||
             (*direct == WEST && next_direct == SOUTH) ||
             (*direct == SOUTH && next_direct == EAST) ||
             (*direct == EAST && next_direct == NORTH))
    {
        // Quay trái
        API_turnLeft();
        API_moveForward();
    }

    // Cập nhật vị trí sau khi di chuyển
    switch (next_direct)
    {
    case NORTH:
        (*y)++;
        break;
    case EAST:
        (*x)++;
        break;
    case SOUTH:
        (*y)--;
        break;
    case WEST:
        (*x)--;
        break;
    }
    *direct = next_direct;
    return;
}
void move_to_goal(maze *_maze, int *x, int *y, short *direct)
{
    logmess("Starting movement to goal...");

    // Lặp cho đến khi đạt được mục tiêu
    while (!isGoal(*x, *y))
    {
        Node *current_cell = _maze->_this_map[*x][*y];
        current_cell->status = VISITED;

        if (current_cell->trace)
        {
            if (!current_cell->wallup && current_cell->up->trace && current_cell->up->status == NONVISITED)
                move_to_goal_step(x, y, direct, NORTH);
            else if (!current_cell->walldown && current_cell->down->trace && current_cell->down->status == NONVISITED)
                move_to_goal_step(x, y, direct, SOUTH);
            else if (!current_cell->wallleft && current_cell->left->trace && current_cell->left->status == NONVISITED)
                move_to_goal_step(x, y, direct, WEST);
            else if (!current_cell->wallright && current_cell->right->trace && current_cell->right->status == NONVISITED)
                move_to_goal_step(x, y, direct, EAST);
            else
            {
                logmess("No valid trace direction found. Stopping...");
                break;
            }
        }
        else
        {
            logmess("Trace path lost. Unable to continue.");
            break;
        }
    }

    if (isGoal(*x, *y))
        logmess("Goal successfully reached!");
    else
        logmess("Failed to reach goal.");
}

void free_maze(maze *_maze)
{
    for (int i = 0; i < MAZESIZE; i++)
    {
        for (int j = 0; j < MAZESIZE; j++)
        {
            if (_maze->_this_map[i][j])
            {
                free(_maze->_this_map[i][j]);
            }
        }
    }
    free(_maze);
}
