#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int MAX = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n"); // Solo acepta un argumento
        return 1;
    }

    FILE *card = fopen(argv[1], "r"); // Abrir tarjeta de memoria
    FILE *img = NULL;

    uint8_t buffer[MAX];
    int counter = 0;
    char file[8];

    while (fread(buffer, 1, MAX, card) == MAX)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            ((buffer[3] & 0xf0) == 0xe0))
        {
            if (img != NULL)
            {
                fclose(img);
            }

            sprintf(file, "%03i.jpg", counter);
            counter++;

            img = fopen(file, "w");

            fwrite(buffer, 1, MAX, img);
        }
        else
        {
            if (img != NULL)
            {
                fwrite(buffer, 1, MAX, img);
            }
        }
    }

    fclose(img);
    fclose(card);
    return 0;
}
