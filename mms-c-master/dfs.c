#include <stdio.h>
#define MAX 100
#include "API.h"
static int vistied[16][16];
typedef struct
{
    char c[MAX];
    unsigned int n;
} Stack;

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
    int i=3;
    while (i<3)
    {
        API_moveForward();
        i++;
    }
}
