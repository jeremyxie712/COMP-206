#include "A4_sort_helpers.h"
sem_t *targetArray[27];

// Function: read_all()
// Provided to read an entire file, line by line.
// No need to change this one.
void read_all(char *filename)
{

    FILE *fp = fopen(filename, "r");
    int curr_line = 0;

    while (curr_line < MAX_NUMBER_LINES &&
           fgets(text_array[curr_line], MAX_LINE_LENGTH, fp))
    {
        curr_line++;
    }

    text_array[curr_line][0] = '\0';
    fclose(fp);
}

// Function: read_all()
// Provided to read only the lines of a file staring with first_letter.
// No need to change this one.
void read_by_letter(char *filename, char first_letter)
{

    FILE *fp = fopen(filename, "r");
    int curr_line = 0;
    text_array[curr_line][0] = '\0';

    while (fgets(text_array[curr_line], MAX_LINE_LENGTH, fp))
    {
        if (text_array[curr_line][0] == first_letter)
        {
            curr_line++;
        }

        if (curr_line == MAX_NUMBER_LINES)
        {
            sprintf(buf, "ERROR: Attempted to read too many lines from file.\n");
            write(1, buf, strlen(buf));
            break;
        }
    }

    text_array[curr_line][0] = '\0';
    fclose(fp);
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q1.
void sort_words()
{

    for (int i = 0; (i < MAX_NUMBER_LINES && text_array[i][0] != '\0'); i++)
    {
        for (int j = i + 1; (j < MAX_NUMBER_LINES && text_array[j][0] != '\0'); j++)
        {
            if (strcmp(text_array[i], text_array[j]) > 0)
            {
                strcpy(buf, text_array[i]);
                strcpy(text_array[i], text_array[j]);
                strcpy(text_array[j], buf);
            }
        }
    }
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q2.
int initialize()
{
    // Remove the current place-holder code, and write your own

    for (int i = 1; i < 26; i++)
    {
        char name[20] = "semaphore_";
        char string[2];
        string[0] = 'b' + i - 1;
        string[1] = '\0';
        strcat(name, string);
        sem_unlink(name);
        targetArray[i] = sem_open(name, O_CREAT, 0666, 0);
    }
    sem_unlink("semaphore_a");
    sem_unlink("complete");
    targetArray[0] = sem_open("semaphore_a", O_CREAT, 0666, 1);
    targetArray[26] = sem_open("complete", O_CREAT, 0666, 0);

    FILE *ptr_file = fopen("output_filename", "w");
    fclose(ptr_file);

    return 0;
}

// YOU MUST COMPLETE THIS FUNCTION FOR Q2 and Q3.
int process_by_letter(char *input_filename, char first_letter)
{
    // For Q2, keep the following 2 lines in your solution (maybe not at the start).
    // Add lines above or below to ensure the "This process will sort..." lines
    // are printed in the right order (alphabetical).

    sem_wait(targetArray[first_letter - 'a']);
    read_by_letter(input_filename, first_letter);
    sort_words();

    FILE *ptr_file = fopen("output_filename", "a");

    for (int i = 0; (i < MAX_NUMBER_LINES && text_array[i][0] != '\0'); i++)
    {
        fwrite(text_array[i], sizeof(char), strlen(text_array[i]), ptr_file);
    }
    fclose(ptr_file);

    // sprintf(buf, "This process will sort the letter %c.\n", first_letter);
    // write(1, buf, strlen(buf));

    sem_post(targetArray[first_letter - 'a' + 1]);
    // For Q3, uncomment the following 2 lines and integrate them with your overall solution.

    return 0;
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q2 and Q3.
int finalize()
{
    // For Q2, keep the following 2 lines in your solution (maybe not at the start).
    // Add lines above or below to ensure the "Sorting complete!" line
    // is printed at the very end, after all letter lines.
    sem_wait(targetArray[26]);
    char line[MAX_LINE_LENGTH];
    FILE *ptr_file = fopen("output_filename", "r");
    while (fgets(line, sizeof(line), ptr_file))
    {
        write(1, line, strlen(line));
    }

    sprintf(buf, "Sorting complete!\n");
    write(1, buf, strlen(buf));

    // For Q3, come up with a way to accumulate the sorted results from each
    // letter process and print the overal sorted values to standard out.
    // You are not allowed to read from the input file, or call sort_words
    // directly from this function.

    return 0;
}
