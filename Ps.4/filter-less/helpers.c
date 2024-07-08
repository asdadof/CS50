#include "helpers.h"
#include <stdio.h>
#include <math.h>

void reverseArray(int arr[], int size);
void max_check(float red, float blue, float green);

void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = avg;
        }
    }
    return;
}
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float red = (.393 * image[i][j].rgbtRed) + (.769 * image[i][j].rgbtGreen) + (.189 * image[i][j].rgbtBlue);
            float green = (.349 * image[i][j].rgbtRed) + (.686 * image[i][j].rgbtGreen) + (.168 * image[i][j].rgbtBlue);
            float blue = (.272 * image[i][j].rgbtRed) + (.534 * image[i][j].rgbtGreen) + (.131 * image[i][j].rgbtBlue);

            if (red > 255)
            {
                red = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }
            if (green > 255)
            {
                green = 255;
            }

            image[i][j].rgbtRed = round(red);
            image[i][j].rgbtGreen = round(green);
            image[i][j].rgbtBlue = round(blue);
        }
    }
}

void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}

void blur(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE temp[height][width];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float sumRed = 0, sumGreen = 0, sumBlue = 0;
            int count = 0;
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    int neighborY = y + dy;
                    int neighborX = x + dx;
                    if (neighborY >= 0 && neighborY < height && neighborX >= 0 && neighborX < width)
                    {
                        sumRed += image[neighborY][neighborX].rgbtRed;
                        sumGreen += image[neighborY][neighborX].rgbtGreen;
                        sumBlue += image[neighborY][neighborX].rgbtBlue;
                        count++;
                    }
                }
            }
            temp[y][x].rgbtRed = round(sumRed / count);
            temp[y][x].rgbtGreen = round(sumGreen / count);
            temp[y][x].rgbtBlue = round(sumBlue / count);
        }
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            image[y][x] = temp[y][x];
        }
    }
}

void reverseArray(int arr[], int size) {
    int start = 0;
    int end = size - 1;
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

void max_check(float red, float blue, float green)
{
    if (red > 255)
    {
        red = 255;
    }
        if (blue > 255)
    {
        blue = 255;
    }
        if (green > 255)
    {
        green = 255;
    }
    return;
}
