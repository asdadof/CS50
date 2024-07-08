#include <stdio.h>

#define BLOCK_SIZE 512
#define FILENAME_SIZE 8

typedef unsigned char BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }

    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    FILE *outptr = NULL;

    BYTE buffer[BLOCK_SIZE];

    int jpegCount = 0;

    char filename[FILENAME_SIZE];

    while (fread(buffer, BLOCK_SIZE, 1, inptr) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (outptr != NULL)
            {
                fclose(outptr);
            }
            sprintf(filename, "%03d.jpg", jpegCount++);

            outptr = fopen(filename, "w");
            if (outptr == NULL)
            {
                fprintf(stderr, "Could not create %s.\n", filename);
                return 3;
            }
        }

        if (outptr != NULL)
        {
            fwrite(buffer, BLOCK_SIZE, 1, outptr);
        }
    }

    if (outptr != NULL)
    {
        fclose(outptr);
    }

    fclose(inptr);

    return 0;
}
