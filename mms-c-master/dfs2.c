#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "API.h"

void log(char *text)
{
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}
static int direct = 1;
// tạo danh sách lưu đường đi
typedef struct path
{
    int x;
    int y;
    char turn;
    struct path *next;
} path;
struct path *initpath()
{
    return NULL;
}
struct path *addpath(struct path *_path, int x, int y, char _turn)
{
    struct path *newNode = (struct path *)malloc(sizeof(struct path));
    newNode->x = x;
    newNode->y = y;
    newNode->turn = _turn;
    newNode->next = NULL;
    if (_path == NULL)
    {
        return newNode;
    }
    newNode->next = _path;
    return newNode;
}
struct path *deletepath(struct path *_path)
{
    struct path *toDelete = _path;
    _path = _path->next;
    free(toDelete);
    return _path;
}
// tạo danh sách nơi đã thăm
typedef struct visited
{
    int x;
    int y;
    struct visited *next;
} visited;
struct visited *initvisitedlist()
{
    return NULL;
}
struct visited *addvisited(int x, int y, struct visited *_visited)
{
    struct visited *newNode = (struct visited *)malloc(sizeof(struct visited));
    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;
    if (_visited == NULL)
    {
        return newNode;
    }
    else
    {
        newNode->next = _visited;
        return newNode;
    }
}

// check các ô xung quanh xem đã thăm chưa. đã thăm trả về false, chưa thăm trả về true
bool check(int x, int y, struct visited *_visited)
{
    struct visited *temp = _visited;
    while (temp != NULL)
    {
        if (temp->x == x && temp->y == y)
        {
            return false;
        }
        temp = temp->next;
    }
    return true;
}

bool DFS(struct path **_path, struct visited **_visited, int(*x), int(*y), char *_turn);
// quay lui về điểm trước đó
void Backtrack(struct path **_path, struct visited *_visited)
{
    log("backtracking...");

    // Lấy vị trí hiện tại (current) và vị trí trước đó (previous)
    struct path *current = (*_path);
    if (current == NULL || current->next == NULL)
    {
        log("No more paths to backtrack.");
        return;
    }

    struct path *previous = current->next;

    // Xử lý theo hướng hiện tại (direct) của robot
    if (direct == 1) // Đang hướng Bắc (north)
    {
        if (current->x < previous->x) // Quay phải để sang Đông
        {
            API_turnRight();
            API_moveForward();
            direct = 2;
        }
        else if (current->x > previous->x) // Quay trái để sang Tây
        {
            API_turnLeft();
            API_moveForward();
            direct = 4;
        }
        else if (current->y > previous->y) // Quay ngược để đi xuống (south)
        {
            API_turnLeft();
            API_turnLeft();
            API_moveForward();
            direct = 3;
        }
        else if (current->y < previous->y) // Tiến lên
        {
            API_moveForward();
        }
    }
    else if (direct == 3) // Đang hướng Nam (south)
    {
        if (current->x < previous->x) // Quay trái để sang Đông
        {
            API_turnLeft();
            API_moveForward();
            direct = 2;
        }
        else if (current->x > previous->x) // Quay phải để sang Tây
        {
            API_turnRight();
            API_moveForward();
            direct = 4;
        }
        else if (current->y < previous->y) // Quay ngược để đi lên (north)
        {
            API_turnLeft();
            API_turnLeft();
            API_moveForward();
            direct = 1;
        }
        else if (current->y > previous->y) // Tiến lui
        {
            API_moveForward();
        }
    }
    else if (direct == 2) // Đang hướng Đông (east)
    {
        if (current->x < previous->x) // Tiến lên
        {
            API_moveForward();
        }
        else if (current->x > previous->x) // Quay ngược để đi về Tây
        {
            API_turnLeft();
            API_turnLeft();
            API_moveForward();
            direct = 4;
        }
        else if (current->y > previous->y) // Quay phải để đi xuống
        {
            API_turnRight();
            API_moveForward();
            direct = 3;
        }
        else if (current->y < previous->y) // Quay trái để đi lên
        {
            API_turnLeft();
            API_moveForward();
            direct = 1;
        }
    }
    else if (direct == 4) // Đang hướng Tây (west)
    {
        if (current->x < previous->x) // Quay ngược để đi về Đông
        {
            API_turnLeft();
            API_turnLeft();
            API_moveForward();
            direct = 2;
        }
        else if (current->x > previous->x) // Tiến lui
        {
            API_moveForward();
        }
        else if (current->y > previous->y) // Quay trái để đi xuống
        {
            API_turnLeft();
            API_moveForward();
            direct = 3;
        }
        else if (current->y < previous->y) // Quay phải để đi lên
        {
            API_turnRight();
            API_moveForward();
            direct = 1;
        }
    }

    // Xóa vị trí hiện tại khỏi danh sách đường đi
    (*_path) = deletepath(*_path);
    free(current);
    DFS(&*_path, &_visited, &(*_path)->x, &(*_path)->y, &(*_path)->turn);
}
// recursive
bool DFS(struct path **_path, struct visited **_visited, int(*x), int(*y), char *_turn)
{
    // đánh dấu điểm mới đã được thăm
    *_visited = addvisited(*x, *y, *_visited);
    *_path = addpath(*_path, (*x), (*y), *_turn);

    // kiểm tra các điểm gần kề (trước mặt, trái, phải)
    // nếu chưa được thăm gọi dfs
    if (direct == 1)
    {
        if (!API_wallFront()) // đi thẳng không đổi direct
        {
            if (check((*x), (*y) + 1, *_visited))
            {
                API_moveForward();
                (*y) = (*y) + 1;
                *_turn = 'b';
                log("go straight");
                DFS(&*_path, _visited, x, y, _turn);
                return 1;
            }
        }
        else if (!API_wallLeft()) // rẽ trái đổi direct
        {
            if (check((*x) - 1, (*y), *_visited))
            {
                API_turnLeft();
                direct = 4;
                API_moveForward();
                (*x) = (*x) - 1;
                *_turn = 'l';
                log("go left");
                DFS(&*_path, _visited, x, y, _turn);
                return 1;
            }
        }
        else if (!API_wallRight())
        {
            if (check((*x) + 1, (*y), *_visited))
            {
                API_turnRight();
                direct = 2;
                API_moveForward();
                (*x) = (*x) + 1;
                *_turn = 'r';
                log("go right");
                DFS(&*_path, _visited, x, y, _turn);
                return 1;
            }
        }
        else
        {
            Backtrack(&*_path, *_visited);
        }
    }
    else if (direct == 2)
    {
        if (!API_wallFront()) // đi thẳng không đổi direct
        {
            if (check((*x) + 1, (*y), *_visited))
            {
                API_moveForward();
                (*x) = (*x) + 1;
                *_turn = 'b';
                log("go straight");
                DFS(&*_path, _visited, x, y, _turn);
                return 1;
            }
        }
        else if (!API_wallLeft()) // rẽ trái đổi direct
        {
            if (check((*x), (*y) + 1, *_visited))
            {
                API_turnLeft();
                direct = 1;
                API_moveForward();
                (*y) = (*y) + 1;
                *_turn = 'l';
                log("go left");
                DFS(&*_path, _visited, x, y, _turn);
                return 1;
            }
        }
        else if (!API_wallRight())
        {
            if (check((*x), (*y) - 1, *_visited))
            {
                API_turnRight();
                direct = 3;
                API_moveForward();
                (*y) = (*y) - 1;
                *_turn = 'r';
                log("go right");
                DFS(&*_path, _visited, x, y, _turn);
                return 1;
            }
        }
        else
        {
            Backtrack(&*_path, *_visited);
        }
    }
    else if (direct == 3)
    {
        if (!API_wallFront()) // đi thẳng không đổi direct
        {
            if (check((*x), (*y) - 1, *_visited))
            {
                API_moveForward();
                (*y) = (*y) - 1;
                *_turn = 'b';
                log("go straight");
                DFS(&*_path, _visited, x, y, _turn);
                return 1;
            }
        }
        else if (!API_wallLeft()) // rẽ trái đổi direct
        {
            if (check((*x) + 1, (*y), *_visited))
            {
                API_turnLeft();
                direct = 2;
                API_moveForward();
                (*x) = (*x) + 1;
                *_turn = 'l';
                log("go left");
                DFS(&*_path, _visited, x, y, _turn);
                return 1;
            }
        }
        else if (!API_wallRight())
        {
            if (check((*x) + 1, (*y), *_visited))
            {
                API_turnRight();
                direct = 4;
                API_moveForward();
                (*x) = (*x) - 1;
                *_turn = 'r';
                log("go right");
                DFS(&*_path, _visited, x, y, _turn);
                return 1;
            }
        }
        else
        {
            Backtrack(&*_path, *_visited);
        }
    }
    else if (direct == 4)
    {
        if (!API_wallFront()) // đi thẳng không đổi direct
        {
            if (check((*x) - 1, (*y), *_visited))
            {
                API_moveForward();
                (*x) = (*x) - 1;
                *_turn = 'b';
                log("go straight");
                DFS(&*_path, _visited, x, y, _turn);
                return 1;
            }
        }
        else if (!API_wallLeft()) // rẽ trái đổi direct
        {
            if (check((*x), (*y) - 1, *_visited))
            {
                API_turnLeft();
                direct = 3;
                API_moveForward();
                (*y) = (*y) - 1;
                *_turn = 'l';
                log("go left");
                DFS(&*_path, _visited, x, y, _turn);
                return 1;
            }
        }
        else if (!API_wallRight())
        {
            if (check((*x), (*y) + 1, *_visited))
            {
                API_turnRight();
                direct = 1;
                API_moveForward();
                (*y) = (*y) + 1;
                *_turn = 'r';
                log("go right");
                DFS(&*_path, _visited, x, y, _turn);
                return 1;
            }
        }
        else
        {
            Backtrack(&*_path, *_visited);
        }
    }
    else
    {
        log("direct does not exit");
        return 0;
    }
}
// struct path *Backtracking(struct path *_path)
// {
//     API_turnLeft();
//     API_turnLeft();
//     if (direct == 1)
//     {
//         direct = 3;
//         if (_path->turn == 'b')
//         {
//             API_moveForward();
//             return (*_path) = deletepath(*_path);
//         }
//         else if (_path->turn == 'r') // trước đó đã rẽ phải để đến ô này
//         {
//             API_moveForward();
//             API_turnLeft(); // rẽ trái để quay chính xác
//             direct = 2;
//             return (*_path) = deletepath(*_path);
//         }
//         else if (_path->turn == 'l') // trước đó dã rẽ trái để đến ô này
//         {
//             API_moveForward();
//             API_turnRight(); // rẽ phải để quay chính xác
//             direct = 4;
//             return (*_path) = deletepath(*_path);
//         }
//     }
//     else if (direct == 2)
//     {
//         direct = 4;
//         if (_path->turn == 'b')
//         {
//             API_moveForward();
//             return (*_path) = deletepath(*_path);
//         }
//         else if (_path->turn == 'r') // trước đó đã rẽ phải để đến ô này
//         {
//             API_moveForward();
//             API_turnLeft(); // rẽ trái để quay chính xác
//             direct = 3;
//             return (*_path) = deletepath(*_path);
//         }
//         else if (_path->turn == 'l') // trước đó dã rẽ trái để đến ô này
//         {
//             API_moveForward();
//             API_turnRight(); // rẽ phải để quay chính xác
//             direct = 1;
//             return (*_path) = deletepath(*_path);
//         }
//     }
//     else if (direct == 3)
//     {
//         direct = 1;
//         if (_path->turn == 'b')
//         {
//             API_moveForward();
//             return (*_path) = deletepath(*_path);
//         }
//         else if (_path->turn == 'r') // trước đó đã rẽ phải để đến ô này
//         {
//             API_moveForward();
//             API_turnLeft(); // rẽ trái để quay chính xác
//             direct = 4;
//             return (*_path) = deletepath(*_path);
//         }
//         else if (_path->turn == 'l') // trước đó dã rẽ trái để đến ô này
//         {
//             API_moveForward();
//             API_turnRight(); // rẽ phải để quay chính xác
//             direct = 2;
//             return (*_path) = deletepath(*_path);
//         }
//     }
//     else if (direct == 4)
//     {
//         direct = 2;
//         if (_path->turn == 'b')
//         {
//             API_moveForward();
//             return (*_path) = deletepath(*_path);
//         }
//         else if (_path->turn == 'r') // trước đó đã rẽ phải để đến ô này
//         {
//             API_moveForward();
//             API_turnLeft(); // rẽ trái để quay chính xác
//             direct = 1;
//             return (*_path) = deletepath(*_path);
//         }
//         else if (_path->turn == 'l') // trước đó dã rẽ trái để đến ô này
//         {
//             API_moveForward();
//             API_turnRight(); // rẽ phải để quay chính xác
//             direct = 3;
//             return (*_path) = deletepath(*_path);
//         }
//     }
//     return 1;
// }

int main(int argc, char *argv[])
{
    log("Running...");
    API_setColor(0, 0, 'G');
    API_setText(0, 0, "abc");
    // direct = 1 : north;
    // direct = 2 : south;
    // direct = 3 : east;
    // direct = 4 : west;
    struct path *_path = initpath();
    struct visited *_visited = initvisitedlist();

    int x = 0;
    int y = 0;
    char _turn;
    _turn = 'b';
    while (1)
    {
        DFS(&_path, &_visited, &x, &y, &_turn);
    }
    return 0;
}