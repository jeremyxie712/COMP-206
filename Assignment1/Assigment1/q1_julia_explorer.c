#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *ptr_file;
    ptr_file = fopen(argv[1], "rt");
    if (ptr_file == NULL)
    {
        printf("Error: Bad file. \n");
        return -1;
    }
    if (argc < 4)
    {
        printf("Error: Bad float arg. \n");
        return -1;
    }
    if (!atof(argv[2]) || !atof(argv[3]))
    {
        printf("Error: Bad float arg. \n");
        return -1;
    }

    int counter = 0;
    char letter, c;
    while (!feof(ptr_file))
    {
        fscanf(ptr_file, "%c", &letter);
        counter++;
    }

    char array[counter + 1];
    char *buffer = array;
    int k = 0;
    fseek(ptr_file, 0, SEEK_SET);
    while (!feof(ptr_file))
    {
        fscanf(ptr_file, "%c", &buffer[k]);
        k++;
    }

    //ptr_file = fopen(argv[1], "rt");
    int i = 2;
    float tmp;
    while (*buffer)
    {
        if (*buffer == '#')
        {
            tmp = atof(argv[i]);
            buffer++;
            printf("%.6f", tmp);
            i++;
            while (*buffer != '#')
            {
                buffer++;
            }
            buffer++;
        }
        printf("%c", *buffer);
        buffer++;
    }
    return 0;
}
