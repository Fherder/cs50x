#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float grayscale;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            grayscale = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
            grayscale = round(grayscale);

            image[i][j].rgbtBlue = grayscale;
            image[i][j].rgbtGreen = grayscale;
            image[i][j].rgbtRed = grayscale;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int half = width / 2;

    int red, green, blue, opposite;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < half; j++)
        {
            red = image[i][j].rgbtRed;
            green = image[i][j].rgbtGreen;
            blue = image[i][j].rgbtBlue;

            opposite = (width - 1) - j;

            image[i][j].rgbtRed = image[i][opposite].rgbtRed;
            image[i][j].rgbtGreen = image[i][opposite].rgbtGreen;
            image[i][j].rgbtBlue = image[i][opposite].rgbtBlue;

            image[i][opposite].rgbtRed = red;
            image[i][opposite].rgbtGreen = green;
            image[i][opposite].rgbtBlue = blue;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float red, green, blue, avgRed, avgGreen, avgBlue;
    int counter, top, bottom, left, right, x, y;

    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {

        top = i - 1;
        bottom = i + 1;

        for (int j = 0; j < width; j++)
        {
            left = j - 1;
            right = j + 1;

            red = 0;
            green = 0;
            blue = 0;
            counter = 0;

            for (x = top; x <= bottom; x++)
            {
                for (y = left; y <= right; y++)
                {
                    if (y >= 0 && y < height && x >= 0 && x < width)
                    {
                        red = red + copy[x][y].rgbtRed;
                        green = green + copy[x][y].rgbtGreen;
                        blue = blue + copy[x][y].rgbtBlue;
                        counter++;
                    }
                }
            }

            avgRed = red / (float) counter;
            red = round(avgRed);
            if (red > 255)
            {
                red = 255;
            }

            avgGreen = green / (float) counter;
            green = round(avgGreen);
            if (green > 255)
            {
                green = 255;
            }

            avgBlue = blue / (float) counter;
            blue = round(avgBlue);
            if (blue > 255)
            {
                blue = 255;
            }

            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    float red, blue, green, Xred, Xgreen, Xblue, Yred, Ygreen, Yblue, sqRed, sqGreen, sqBlue;
    int top, bottom, left, right, x, y, row, column;

    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {

        top = i - 1;
        bottom = i + 1;

        for (int j = 0; j < width; j++)
        {
            left = j - 1;
            right = j + 1;

            Xred = 0;
            Xgreen = 0;
            Xblue = 0;

            Yred = 0;
            Ygreen = 0;
            Yblue = 0;

            row = 0;

            for (x = top; x <= bottom; x++)
            {
                column = 0;
                for (y = left; y <= right; y++)
                {
                    if (y >= 0 && y < height && x >= 0 && x < width)
                    {
                        Xred = Xred + (copy[x][y].rgbtRed * Gx[row][column]);
                        Xgreen = Xgreen + (copy[x][y].rgbtGreen * Gx[row][column]);
                        Xblue = Xblue + (copy[x][y].rgbtBlue * Gx[row][column]);

                        Yred = Yred + (copy[x][y].rgbtRed * Gy[row][column]);
                        Ygreen = Ygreen + (copy[x][y].rgbtGreen * Gy[row][column]);
                        Yblue = Yblue + (copy[x][y].rgbtBlue * Gy[row][column]);
                    }
                    column++;
                }
                row++;
            }

            sqRed = sqrtf(Xred * Xred + Yred * Yred);
            red = round(sqRed);
            if (red > 255)
            {
                red = 255;
            }

            sqGreen = sqrtf(Xgreen * Xgreen + Ygreen * Ygreen);
            green = round(sqGreen);
            if (green > 255)
            {
                green = 255;
            }

            sqBlue = sqrtf(Xblue * Xblue + Yblue * Yblue);
            blue = round(sqBlue);
            if (blue > 255)
            {
                blue = 255;
            }

            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;
        }
    }
    return;
}
