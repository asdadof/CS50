#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


float L(string paragraph);

float S(string paragraph);

float final(float avg_wps, float avg_lpw);

int main(void)
{
    string paragraph = get_string("Input: ");
    float avg_letters = L(paragraph);
    float avg_words = S(paragraph);
    float subindex = 0.0588 * avg_letters - 0.296 * avg_words - 15.8;
    int index = round(subindex);

    if (index <= 1)
    {
        printf("Before Grade 1\n");
    }

    else if (index>16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
    }
float L(string paragraph)
{
    float letters = 0;
    float words = 1;
    for (int i = 0, len = strlen(paragraph); i < len; i++)
    {
        if (paragraph[i] == ' ')
        {
            words += 1;
        }
        else if (isalpha(paragraph[i]))
        {
            letters += 1;
        }
    }

    float avg = (letters / words)*100;
    return avg;
}
float S(string paragraph)
{
    float words = 1;
    float sentences = 0;
    for (int i = 0, len = strlen(paragraph); i < len; i++)
    {
        if (paragraph[i] == ' ')
        {
            words += 1;
        }
        else if (paragraph[i] == '.' || paragraph[i] == '!' || paragraph[i] == '?')
        {
            sentences += 1;
        }

    }

    float avg = (sentences/words)*100;
    return avg;
}

float final(float avg_wps, float avg_lpw)
{
    float index = 0.0588 * avg_lpw - 0.296 * avg_wps - 15.8;
    return index;
}
