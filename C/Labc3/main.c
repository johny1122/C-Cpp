#include <stdio.h>

void anagram()
{
    int buffer = 20;
    char str1[buffer];
    char str2[buffer];
    char input[40];

    fgets(input, 40, stdin);
    sscanf(input,"%s %s\n", str1, str2);
    int index = 0;
    while(str1[index] != '\0' && str2[index] != '\0')
    {
        index++;
    }

    if (str1[index] == '\0' && str2[index] == '\0')
    {
        int match = 0;
        for (int i = 0; i < index; i++)
        {
            for (int j = 0; j < index; j++)
            {
                if(str1[i] == str2[j])
                {
                    str1[i] = '*';
                    str2[j] = '*';
                    match++;
                    break;
                }
            }
        }
        if(match == index)
        {
            printf("true");
        }
        else
        {
            printf("false");
        }
    }
    else
    {
        printf("false");
    }
}


int main()
{

    return 0;
}
