#ifndef MAZE_H
#define MAZE_H

#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4

#define MAZESIZE 16

#define Xstart 0
#define Ystart 0

#define QSIZE 80
#define BUFFER_SIZE 4

#define HIGHESTVAL 255

void logmess(char *text);
// define the nodes stand for maze cells
typedef struct Node
{
    short x;
    short y;
    int value;
    int visited;

    struct Node *up;
    struct Node *down;
    struct Node *left;
    struct Node *right;
} Node;
// function for Node
Node *createNode(const short x, const short y, const int value);
void deleteNode(Node **_this_node);

// define the structure of maze
typedef struct maze
{
    Node *_this_map[MAZESIZE][MAZESIZE];
} maze;
// functions for maze
maze *maze_init(maze *_maze);
maze *flood_value(Node **_this_node, maze *_this_maze);
void set_visited(Node **_this_cell);
Node *set_wall(Node *_this_cell, const short direct);
// bool accessible_neighbor_check(Node *_this_cell, int current_value);
// int min_value_check(Node *_this_cell);

// define the queue for floodfill
typedef struct queue
{
    Node *_queue[QSIZE];
    int top;
} queue;
// functions for queue
queue *initQ();
queue *addQ(Node *_this_cell, queue *_this_queue);
Node *popQ(queue **_this_queue);
bool isEmpty(queue *_this_queue);
queue *add_all_neighbors(Node *_this_node, queue *_this_queue);

// floodfill assisting functions
// bool accessible_neighbor_check(Node *_this_cell, int current_value);
// int min_value_take(Node *_this_cell);
// bool lower_neighbor_check(Node *_this_cell, int current_value);
// void logmess(char *text);
// queue *add_neighbors(Node *_this_Cell, queue *_this_queue);
bool check_for_smallest_neighbors(Node *_this_cell);
int get_smallest_neighbor_value(Node *_this_cell);
short get_smallest_neighbor_dir(Node *_this_cell);

#endif // MAZE_H;