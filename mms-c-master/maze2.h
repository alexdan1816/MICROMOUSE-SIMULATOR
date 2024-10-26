#ifndef MAZE_H
#define MAZE_H

#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4
#define north 'n'
#define east 'e'
#define south 's'
#define west 'w'

#define MAZESIZE 16
#define GOAL

#define Xstart 0
#define Ystart 0

#define QSIZE 80
#define BUFFER_SIZE 4

typedef struct Node // Node structure
{
    short x;
    short y;
    short value;
    short visited;

    struct Node *up;
    struct Node *down;
    struct Node *left;
    struct Node *right;
} Node;
typedef struct maze // maze structure
{
    Node *map[MAZESIZE][MAZESIZE];
} maze;
typedef struct location
{
    int x;
    int y;
} location;
typedef struct LIFOqueue
{
    struct location *Q[QSIZE];
    int top;
} LIFOqueue;
// typedef struct LIFOqueue // Queue Last In First Out
// {
//     struct Node *Q[QSIZE];
//     int top;
// } LIFOqueue;

// Node functions
struct Node *new_Node(short x, short y);

// Maze functions
struct maze *mazeinit();
struct maze *reflood(struct LIFOqueue **_queue, struct maze **_maze);
struct maze *connectcell(struct maze *maze);
// Queue function
struct LIFOqueue *QueueInit();
struct location *popQ(struct LIFOqueue **_queue);
struct LIFOqueue *addQ(struct LIFOqueue *_queue, short x, short y);
struct LIFOqueue *deleteQ(struct LIFOqueue *_queue);
// Maze explore
void setwall(int direct, short x, short y, struct maze **_maze);
void move(short *x, short *y, struct maze **_maze, struct LIFOqueue **_queue, int *direct);

void printQueue(struct LIFOqueue *queue);
void log(char *text);
#endif // MAZE_H;