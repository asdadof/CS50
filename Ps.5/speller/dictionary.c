#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

const unsigned int N = 676;

node *table[N];

unsigned int counter = 0;

bool check(const char *word)
{
    unsigned int index = hash(word);

    node *node = table[index];

    if (node == NULL)
    {
        return false;
    }

    while (node != NULL)
    {
        if (strcasecmp(node->word, word) == 0)
        {
            return true;
        }
        else
        {
            node = node->next;
        }
    }
    return false;
}

unsigned int hash(const char *word)
{
    unsigned int index = ((toupper(word[0]) - 'A') * 26);
    if (strlen(word) < 2)
    {
        if (index > N)
        {
            index %= N;
        }
        return index;
    }
    if (index > N)
    {
        index %= N;
    }
    index = (index + (toupper(word[1]) - 'A'));
    return index;
}

bool load(const char *dictionary)
{
    FILE *input = fopen(dictionary, "r");
    if (input == NULL)
    {
        return false;
    }
    char next_word[LENGTH + 1];

    while (fscanf(input, "%s", next_word) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(input);
            return false;
        }
        strcpy(n->word, next_word);
        int hash_value = hash(next_word);
        n->next = table[hash_value];
        table[hash_value] = n;
        counter++;
    }
    fclose(input);
    return true;
}

unsigned int size(void)
{
    if (counter > 0)
    {
        return counter;
    }
    else
    {
        return 0;
    }
}

bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *n = table[i];
        while (n != NULL)
        {
            node *temp = n;
            n = n->next;
            free(temp);
        }
        table[i] = NULL;
    }
    return true;
}
