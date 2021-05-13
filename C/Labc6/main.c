#include <stdio.h>
#include <stdlib.h>
typedef struct Grid Grid;
typedef void* GridElement;

struct Grid {
    // implement this structure
    int rows;
    int cols;
    GridElement* data;
};

Grid * create_grid(int rows, int cols)
{
    // implement this function
    if (rows <= 0 || cols <=0) return NULL;
    Grid* grid = (Grid*) malloc(sizeof(Grid));
    if (grid ==NULL) return NULL;
    grid->rows = rows;
    grid->cols = cols;
    grid->data = (GridElement*) calloc(rows * cols, sizeof(GridElement));
    if (grid->data == NULL)
    {
        free(grid);
        return NULL;
    }

    return grid;
}

int add_to_grid(Grid *g, int row, int col, GridElement new_elem)
{
    // implement this function
    if (g == NULL || new_elem == NULL)
    {
        return -1;
    }
    if ((row > g->rows) || (row < 0) || (col < 0) || (g->cols < col))
    {
        return -1;
    }
    if (g->data[row * (g->cols) + col] == NULL)
    {
        g->data[row * (g->cols) + col] = new_elem;
        return 0;
    }
    return -1;
}

void free_grid(Grid **g)
{
    // implement this function
    if (g == NULL)
        return;
    if (*g == NULL)
        return;

    if ((*g)->data != NULL)
    {
        free((*g)->data);
    }
    free(*g);
    *g = NULL;
}

int main()
{
    printf("Hello, World!\n");
    return 0;
}
