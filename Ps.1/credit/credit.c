#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long long number = get_long("Number: ");
    long long card = number;
    int counter = 0;

    while (number > 0)
    {
        number /= 10;
        counter++;
    }
    if (counter != 13 && counter != 15 && counter != 16)
    {
        printf("INVALID\n");
        return 0;
    }

    int sum1 = 0;
    int sum2 = 0;
    int mod1;
    int mod2;
    int total;
    long long rest = card;
    do
    {
        mod1 = card % 10;
        card /= 10;
        sum1 += mod1;

        mod2 = card % 10;
        card /= 10;

        if (mod2 * 2 > 9)
        {
            sum2 += (mod2 * 2) % 10;
            sum2 += (mod2 * 2) / 10;
        }
        else
        {
            mod2 *= 2;
            sum2 += mod2;
        }
    }
    while (card > 0);

    total = sum1 + sum2;
    do
    {
        rest /= 10;
    }

    while (rest > 100);

    if ((rest == 34 || rest == 37) && (counter == 15) && (total % 10 == 0))
    {
        printf("AMEX\n");
    }
    else if ((rest == 51 || rest == 52 || rest == 53 || rest == 54 || rest == 55) &&
             (counter == 16) && (total % 10 == 0))
    {
        printf("MASTERCARD\n");
    }
    else if ((rest / 10 == 4) && (counter == 13 || counter == 16) && (total % 10 == 0))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
        return 0;
    }
}
