#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "API.h"
#include "maze.h"

void log(char *text)
{
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}
int main(int argc, char *argv[])
{
    log("Running...");
    API_setColor(0, 0, 'G');
    API_setText(0, 0, "abc");
    struct maze *_maze = mazeinit();
    struct LIFOqueue *_queue = QueueInit();
    int direct = NORTH;
    short x = 0;
    short y = 0;
    setwall(direct, x, y, &_maze);
    while (true)
    {
        move(&x, &y, &_maze, &_queue, &direct);
    }
}