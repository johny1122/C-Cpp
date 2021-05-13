#include <stdio.h>
#include <stdlib.h>
/**
 * The main function
 */
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Error");
    }
    int arg = atoi(argv[1]);
    if (arg == 1)
    {
        printf("Hello, World!");
    }
    else if (arg == 2)
    {
        printf("Welcome to C!");
    }
    return 0;
}

