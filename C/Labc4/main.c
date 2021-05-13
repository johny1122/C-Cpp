#include <stdio.h>
#include <stdlib.h>

#define ALLOCATE 10

typedef struct Triangle
{
    int a, b, c;
} Triangle;

int main()
{
    int capacity = ALLOCATE;
    Triangle *arr = (Triangle*) malloc(sizeof(Triangle) * capacity);

    int allsum = 0;
    int counter = 0;
    int a, b, c = 0;
    while(scanf("%d %d %d",&a, &b, &c) != EOF)
    {
        printf("*****\n");
        arr[counter].a = a;
        arr[counter].b = b;
        arr[counter].c = c;
        allsum += a + b + c;
        counter++;
        if (counter == capacity)
        {
            capacity += ALLOCATE;
            arr = (Triangle*) realloc(arr,sizeof(Triangle) * capacity);
        }
    }
    counter--;
    while(counter>=0)
    {
        printf("%d %d %d\n", arr[counter].a, arr[counter].b, arr[counter].c);
        counter--;
    }
    printf("sum = %d\n", allsum);
    free(arr);
}