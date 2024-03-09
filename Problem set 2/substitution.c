#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

string valid(int count, string key);

int main(int argc, string argv[])
{
    string key = valid(argc, argv[1]);
    if (key == NULL)
    {
        return 1;
    }
    string plaintext = get_string("plaintext: ");
    string ciphertext = plaintext;

    for (int i = 0, len = strlen(plaintext); i < len; i++)
    {
        if (isalpha(plaintext[i]))
        {
            int index = toupper(plaintext[i]) - 'A';
            if (isupper(plaintext[i]))
            {
                ciphertext[i] = toupper(key[index]);
            }
            else
            {
                ciphertext[i] = tolower(key[index]);
            }
        }
    }
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

string valid(int count, string key)
{
    if (count != 2)
    {
        printf("Usage: ./substitution key\n");
        return NULL;
    }

    else if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return NULL;
    }
    for (int j = 0, len = strlen(key); j < len; j++)
    {
        for (int i = j + 1, length = strlen(key); i < length; i++)
        {
            if (key[j] == key[i])
            {
                printf("Key can not have duplicate characters\n");
                return NULL;
            }
        }

        if (!(isalpha(key[j]) || key[j] == '!' || key[j] == '?'))
        {
            printf("Key must only contain alphabetic characters.\n");
            return NULL;
        }
    }

    return key;
}
