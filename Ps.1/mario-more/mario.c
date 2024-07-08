#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int size;

    do
    {

        size = get_int("size: ");
    }

    while (size <= 0 || size > 8);

    for (int i = 0; i < size; i++)
    {

        for (int j = i + 1; j < size; j++)
        {
            printf(" ");
        }
        for (int j = i + 1; j > 0; j--)
        {
            printf("#");
        }
        for (int j = 0; j < 2; j++)
        {
            printf(" ");
        }
        for (int j = i + 1; j > 0; j--)
        {
            printf("#");
        }
        printf("\n");
    }
}




    if (total%10 == 0)
    {
        printf("Valid\n");
    }
    else
    {
        printf("Not valid\n");
    }
