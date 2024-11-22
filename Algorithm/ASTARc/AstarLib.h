#ifndef astar2_h
#define astar2_h

#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4

#define MAZESIZE 16

#define Xstart 0
#define Ystart 0

#define Xgoal 7
#define Ygoal 7

#define QSIZE 128
#define BUFFER_SIZE 128

#define HIGHESTVAL 255
#define DEFAULTVAL 125
#define CELLQUANTITY 256
#define TRUE 1
#define FALSE 0

#define VISITED 1
#define NONVISITED 0

void logmess(char *text);
// define cell
typedef struct Node
{
    short x;
    short y;
    int gcost;
    int hcost;
    int fcost;
    int status;
    short wallup, walldown, wallleft, wallright;

    struct Node *up;
    struct Node *down;
    struct Node *left;
    struct Node *right;

    struct Node *parent;
    int trace;
} Node;
// define maze
typedef struct maze
{
    Node *_this_map[MAZESIZE][MAZESIZE];
} maze;
// define visited list for dfs
typedef struct List_part
{
    int x, y;
} List_part;
typedef struct List
{
    List_part *List[CELLQUANTITY];
    int n;
} List;
// define stack for dfs path
typedef struct Stack
{
    int x, y;
    struct Stack *next;
} Stack;
// define PriorityQueue for A* algorithm
typedef struct PQueueNode
{
    int x;
    int y;

    int fcost;
    int hcost;
    int gcost;

    struct PQueueNode *next;
} PQueueNode;
typedef struct PriorityQueue
{
    PQueueNode *queue_head;
} PriorityQueue;

// function for cell
Node *
createNode(const short x, const short y);
// *
// *
// *
// function for maze
maze *maze_init(maze *_maze);
// *
// *
// *
// function for stack path for dfs
Stack *create_stack();
void add_path(Stack **_path, int _x, int _y);
void delete_path(Stack **_path);
Stack *peek_path(Stack *_path);
// *
// *
// *
// functions for visit list for dfs
List *create_list();
bool is_visited(List *list, int _x, int _y);
bool is_full(List *_list);
void add_list(List **_list, int _x, int _y);
// *
// *
// *
// DFS FOR EXPLORING MAZE
short next_direction_dfs(List *_list, int _x, int _y, short _current_dir);
short next_direction_backtrack(Stack **_path, short current_dir, int cur_x, int cur_y);
void set_wall(Node *_this_cell, short direct);
void DFS(int *x, int *y, short *direct, List **_list, Stack **_path, maze **_maze);
void BackTrack(int *x, int *y, short *direct, List *_list, Stack **_path, maze **_maze);
void FinishExplore(int *x, int *y, short *direct, Stack **_path);
// *
// *
// *
// function for PriorityQueue
PriorityQueue *create_queue();
PQueueNode *create_PQNode(int _x, int _y, int _h_cost, int _g_cost);

bool isEmptyPQ(PriorityQueue *_queue);

void enqueue(PriorityQueue **_queue, PQueueNode *_new_Node);
PQueueNode *popQ(PriorityQueue **_queue);
// *
// *
// *
// A* algorithm
bool isGoal(int x, int y);

void add_child_cell(PQueueNode *current, int child_cell_x, int child_cell_y, PriorityQueue **_queue, maze *_maze);
void A_star(PriorityQueue **_queue, maze *_maze);
void trace_path(Node **_GOAL);

void move_to_goal(maze *_maze, int *x, int *y, short *direct);
void move_to_goal_step(int *x, int *y, short *direct, short next_direct);

void free_maze(maze *_maze);
#endif // astar2_h