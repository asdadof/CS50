#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int find_score(string word);

int main(void)
{
    string first = get_string("Number 1: ");
    string second = get_string("Number 2: ");

    int score1 = find_score(first);
    int score2 = find_score(second);

    if (score1 > score2)
    {
        printf("Player 1 wins!");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!");
    }
    else
    {
        printf("Tie!");
    }
}

int find_score(string word)
{
    int sum = 0;
    int a = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        if (isupper(word[i]))
        {
            a = word[i] - 65;
        }
        else
        {
            a = word[i] - (65 + 32);
        }

        if (a < 0)
        {
            break;
        }
        else
        {
            sum += POINTS[a];
        }
    }
    return sum;
}
