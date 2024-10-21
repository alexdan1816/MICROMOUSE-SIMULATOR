#include <iostream>
#include <string>
#include "API.h"

void log(const std::string& text) {
    std::cerr << text << std::endl;
}

struct list
{
    int x;
    int y;
    list* next;
};

list*init() // khởi tạo danh sách path và visit
{
    return nullptr;
}
void addlist(list *&_list, int x, int y) //thêm tọa độ vào danh sách
{
    list*newNode = new list;
    newNode->x = x;
    newNode->y = y;
    newNode->next = nullptr;
    if(_list == nullptr)
    {
        _list = newNode;
    }
    else 
    {
        newNode->next = _list;
        _list = newNode;
    }
}
list *deletepath(list *_path) //xóa tọa độ sai trong lộ trình (path)
{
    list*todelete = _path;
    _path = _path->next;
    delete(todelete);
    return _path;
}

int check(int x, int y, list *visit) // kiểm tra xem ô đã được thăm chưa
{
    list *check = visit;
    while (check !=nullptr)
    {
        if(check->x == x && check->y == y)
        {
            return 0;
        }
        else 
        check = check->next;
    }
    return 1;
}
static int direct = 1;
int check_goal(int x, int y) // kiểm tra xem ô đã được thăm chưa
{
    if ((x == 7 && y == 7) || (x == 7 && y == 8) || (x == 8 && y == 7) || (x == 8 && y == 8)) return 1;
    return 0;
}
void logList(const list* lst, const std::string& listName) {
    std::string logMessage = listName + ": ";
    while (lst != nullptr) {
        logMessage += "(" + std::to_string(lst->x) + ", " + std::to_string(lst->y) + ") ";
        lst = lst->next;
    }
    log(logMessage); // Gọi hàm log để in ra
}
void DFS(list *&_path, list *&_visit, int *x, int *y); //prototype của dfs
void BT(list *&_path,list *_visit,int *x, int *y) // hàm đệ quy backtracking
{
    log("backtracking...");
    list *current = _path; // tọa độ cần xóa để về điểm trước đó
    if (current == NULL || current->next == NULL) //kiểm tra
    {
        log("No more paths to backtrack.");
        return;
    }
    list *previous = current->next;
    // Thực hiện quay về điểm trước đó dựa theo tọa độ hiện tại, tọa độ trước đó, hướng hiện tại
    API::setColor(*x,*y,'y'); //tô đỏ các ô sai
    if (direct == 1) // Đang hướng Bắc (north)
    {
        if (current->x < previous->x) // Quay phải để sang Đông
        {
            API::turnRight();
            API::moveForward();
            direct = 2;
            *x = *x + 1;
        }
        else if (current->x > previous->x) // Quay trái để sang Tây
        {
            API::turnLeft();
            API::moveForward();
            direct = 4;
            *x = *x - 1;
        }
        else if (current->y > previous->y) // Quay ngược để đi xuống (south)
        {
            API::turnLeft();
            API::turnLeft();
            API::moveForward();
            direct = 3;
            *y = *y - 1;
        }
        else if (current->y < previous->y) // Tiến lên
        {
            API::moveForward();
            *y = *y + 1;
        }
    }
    else if (direct == 3) // Đang hướng Nam (south)
    {
        if (current->x < previous->x) // Quay trái để sang Đông
        {
            API::turnLeft();
            API::moveForward();
            direct = 2;
            *x = *x + 1;
        }
        else if (current->x > previous->x) // Quay phải để sang Tây
        {
            API::turnRight();
            API::moveForward();
            direct = 4;
            *x = *x - 1;
        }
        else if (current->y < previous->y) // Quay ngược để đi lên (north)
        {
            API::turnLeft();
            API::turnLeft();
            API::moveForward();
            direct = 1;
            *y = *y + 1;
        }
        else if (current->y > previous->y) // Tiến lui
        {
            API::moveForward();
            *y = *y - 1;
        }
    }
    else if (direct == 2) // Đang hướng Đông (east)
    {
        if (current->x < previous->x) // Tiến lên
        {
            API::moveForward();
            *x = *x + 1;
        }
        else if (current->x > previous->x) // Quay ngược để đi về Tây
        {
            API::turnLeft();
            API::turnLeft();
            API::moveForward();
            direct = 4;
            *x = *x - 1;
        }
        else if (current->y > previous->y) // Quay phải để đi xuống
        {
            API::turnRight();
            API::moveForward();
            direct = 3;
            *y = *y - 1;
        }
        else if (current->y < previous->y) // Quay trái để đi lên
        {
            API::turnLeft();
            API::moveForward();
            direct = 1;
            *y = *y + 1;
        }
    }
    else if (direct == 4) // Đang hướng Tây (west)
    {
        if (current->x < previous->x) // Quay ngược để đi về Đông
        {
            API::turnLeft();
            API::turnLeft();
            API::moveForward();
            direct = 2;
            *x = *x + 1;
        }
        else if (current->x > previous->x) // Tiến lui
        {
            API::moveForward();
            *x = *x - 1;
        }
        else if (current->y > previous->y) // Quay trái để đi xuống
        {
            API::turnLeft();
            API::moveForward();
            direct = 3;
            *y = *y - 1;
        }
        else if (current->y < previous->y) // Quay phải để đi lên
        {
            API::turnRight();
            API::moveForward();
            direct = 1;
            *y = *y + 1;
        }
    }
    
    delete(current);
    _path = deletepath(_path);
    logList(_path, "newPath");
    log("searching for available way");
    //kiểm tra các ô phía trước, trái, phải ô vừa backtrack, nếu có ô chưa thăm, thực hiện gọi dfs
    if (direct == 1)
    {
        if (!API::wallFront()) 
        {
            if (check(*x, *y + 1, _visit))
            {
                API::moveForward();
                *y = *y + 1;
                log("go straight");
                DFS(_path, _visit, x, y);
            }
            log("way not found");
        }
        if (!API::wallLeft()) // rẽ trái đổi direct
        {
            if (check(*x- 1, *y, _visit))
            {
                API::turnLeft();
                direct = 4;
                API::moveForward();
                *x = *x - 1;
                log("go left");
                DFS(_path, _visit, x, y);
            }
            log("way not found");
        }
        if (!API::wallRight())
        {
            if (check(*x + 1, *y, _visit))
            {
                API::turnRight();
                direct = 2;
                API::moveForward();
                *x = *x + 1;
                log("go right");
                DFS(_path, _visit, x, y);
            }
            log("way not found");
        }   
        BT(_path,_visit, x,y);
    }
    else if (direct == 2)
    {
        if (!API::wallFront()) // đi thẳng không đổi direct
        {
            if (check(*x + 1, *y, _visit))
            {
                API::moveForward();
                *x = *x + 1;
                log("go straight");
                DFS(_path, _visit, x, y);
            }
            log("way not found");
        }
        if (!API::wallLeft()) // rẽ trái đổi direct
        {
            if (check(*x, *y + 1, _visit))
            {
                API::turnLeft();
                direct = 1;
                API::moveForward();
                *y = *y + 1;
                log("go left");
                DFS(_path, _visit, x, y);
            }
            log("way not found");
        }
        if (!API::wallRight())
        {
            if (check(*x, *y - 1,_visit))
            {
                API::turnRight();
                direct = 3;
                API::moveForward();
                *y = *y - 1;
                log("go right");
                DFS(_path, _visit, x, y);
            }
            log("way not found");
        }
        BT(_path,_visit, x,y);
        
    }
    else if (direct == 3)
    {
        if (!API::wallFront()) // đi thẳng không đổi direct
        {
            if (check(*x, *y - 1,_visit))
            {
                API::moveForward();
                *y = *y - 1;
                log("go straight");
                DFS(_path, _visit, x, y);
            }
            log("way not found");
        }
        if (!API::wallLeft()) // rẽ trái đổi direct
        {
            if (check(*x + 1, *y, _visit))
            {
                API::turnLeft();
                direct = 2;
                API::moveForward();
                *x = *x + 1;
                log("go left");
                DFS(_path, _visit, x, y);
            }
            log("way not found");
        }
        if (!API::wallRight())
        {
            if (check(*x - 1, *y, _visit))
            {
                API::turnRight();
                direct = 4;
                API::moveForward();
                *x = *x - 1;
                log("go right");
                DFS(_path, _visit,x, y);
            }
            log("way not found");
        }
        BT(_path,_visit, x,y);  
    }
    else if (direct == 4)
    {
        if (!API::wallFront()) // đi thẳng không đổi direct
        {
            if (check(*x- 1, *y, _visit))
            {
                API::moveForward();
                *x = *x - 1;
                log("go straight");
                DFS(_path, _visit,x, y);
            }
            log("way not found");
        }
        if (!API::wallLeft()) // rẽ trái đổi direct
        {
            if (check(*x, *y - 1, _visit))
            {
                API::turnLeft();
                direct = 3;
                API::moveForward();
                *y = *y - 1;
                log("go left");
                DFS(_path, _visit, x, y);
            }
            log("way not found");
        }
        if (!API::wallRight())
        {
            if (check(*x, *y + 1, _visit))
            {
                API::turnRight();
                direct = 1;
                API::moveForward();
                *y = *y + 1;
                log("go right");
                DFS(_path, _visit, x, y);
            }
            log("way not found");
        }        
        BT(_path,_visit, x,y);        
    }
    else {
        BT(_path,_visit,x,y);
    }

}
void DFS(list *&_path, list *&_visit, int *x, int *y) // hàm đệ quy DFS
{   
    if(check_goal(*x,*y)) //kiểm tra xem đã đến đích chưa
    {
        log("Goal reached!");
        return;
    }
    addlist(_path, *x, *y); // thêm vào lộ trình
    addlist(_visit, *x, *y); //thêm vào danh sách đã thăm
    logList(_path, "Path");
    logList(_visit, "Visit");
    API::setColor(*x,*y,'b');

    //kiểm tra các điểm bên trái, bên phải, trước mặt
    if(direct == 1)
    {
        if(!API::wallFront()) //không có tường phía trước
        {
            if(check((*x),(*y)+1, _visit))
            {
                API::moveForward();
                *y = *y + 1;
                log("go straight...");
                DFS(_path, _visit, x, y);
            }
        }
        if (!API::wallLeft()) // rẽ trái đổi direct
        {
            if (check((*x) - 1, (*y), _visit))
            {
                API::turnLeft();
                direct = 4;
                API::moveForward();
                (*x) = (*x) - 1;
                log("go left");
                DFS(_path, _visit, x, y);
            }
        }
        if (!API::wallRight())
        {
            if (check((*x) + 1, (*y), _visit))
            {
                API::turnRight();
                direct = 2;
                API::moveForward();
                (*x) = (*x) + 1;

                log("go right");
                DFS(_path, _visit, x, y);
            }
        }
        BT(_path,_visit, x,y);
    }
    else if (direct == 2)
    {
        if (!API::wallFront()) 
        {
            if (check((*x) + 1, (*y), _visit))
            {
                API::moveForward();
                (*x) = (*x) + 1;

                log("go straight");
                DFS(_path, _visit, x, y);
            }
        }
        if (!API::wallLeft()) 
        {
            if (check((*x), (*y) + 1, _visit))
            {
                API::turnLeft();
                direct = 1;
                API::moveForward();
                (*y) = (*y) + 1;

                log("go left");
                DFS(_path, _visit, x, y);
            }
        }
        if (!API::wallRight())
        {
            if (check((*x), (*y) - 1, _visit))
            {
                API::turnRight();
                direct = 3;
                API::moveForward();
                (*y) = (*y) - 1;
                log("go right");
                DFS(_path, _visit, x, y);
            }
        }
        BT(_path,_visit, x,y);
    }
    else if (direct == 3)
    {
        if (!API::wallFront()) 
        {
            if (check((*x), (*y) - 1, _visit))
            {
                API::moveForward();
                (*y) = (*y) - 1;
                log("go straight");
                DFS(_path, _visit, x, y);
            }
        }
        if (!API::wallLeft()) // rẽ trái đổi direct
        {
            if (check((*x) + 1, (*y), _visit))
            {
                API::turnLeft();
                direct = 2;
                API::moveForward();
                (*x) = (*x) + 1;
                log("go left");
                DFS(_path, _visit, x, y);
            }
        }
        if (!API::wallRight())
        {
            if (check((*x) - 1, (*y), _visit))
            {
                API::turnRight();
                direct = 4;
                API::moveForward();
                (*x) = (*x) - 1;
                log("go right");
                DFS(_path, _visit, x, y);
            }
        }
        BT(_path,_visit, x,y);
    }
    else if (direct == 4)
    {
        if (!API::wallFront()) 
        {
            if (check((*x) - 1, (*y), _visit))
            {
                API::moveForward();
                (*x) = (*x) - 1;
                log("go straight");
                DFS(_path, _visit, x, y);
            }
        }
        if (!API::wallLeft()) // rẽ trái đổi direct
        {
            if (check((*x), (*y) - 1, _visit))
            {
                API::turnLeft();
                direct = 3;
                API::moveForward();
                (*y) = (*y) - 1;
                log("go left");
                DFS(_path, _visit, x, y);
            }
        }
        if (!API::wallRight())
        {
            if (check((*x), (*y) + 1, _visit))
            {
                API::turnRight();
                direct = 1;
                API::moveForward();
                (*y) = (*y) + 1;
                log("go right");
                DFS(_path, _visit, x, y);
            }
        }
        BT(_path,_visit, x,y);
    }
}


int main(int argc, char* argv[]) 
{
    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "abc");
    int x = 0;
    int y = 0;

    list*_path = init();
    list*_visit = init();

        DFS(_path,_visit, &x,&y);
    
    delete(_path);
    delete(_visit);
    return 0;
}