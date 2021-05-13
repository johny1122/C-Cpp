#include <stdio.h>

void reverseInput()
{
    int num = 19234128;
    int reverse = 0;
    while (num > 10)
    {
        reverse += num % 10;
        num /= 10;
        reverse *= 10;
    }
    reverse += num;
    printf("%d", reverse);
}

int main()
{
    reverseInput();
    return 0;
}