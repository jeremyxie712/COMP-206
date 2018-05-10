#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dashline(int size)
{
    printf("|");
    for (int i = 2; i < ((size + 3) * 7) + 1; i++)
    {
        printf("-");
    }
    printf("| \n");
}

int main(int argc, char *argv[])
{
    int date[] = {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30};
    char *month[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    char *sun = "Sunday";
    char *mon = "Monday";
    char *tue = "Tuesday";
    char *wed = "Wednesday";
    char *thu = "Thursday";
    char *fri = "Friday";
    char *sat = "Saturday";

    int daysize = atoi(argv[1]);
    int firstday = atoi(argv[2]);

    if (daysize < 2)
    {
        printf("The daysize must be 2 or greater. \n");
        exit(0);
    }
    if (firstday < 1 || firstday > 7)
    {
        printf("The first day must be within the range of 1 to 7. \n");
        exit(0);
    }
    //This is to print the month
    for (int j = 0; j < 12; j++)
    {
        if (j > 0)
        {
            printf("\n");
        }
        dashline(daysize);
        int space = ((daysize + 3) * 7 + 1);
        char *string = month[j];
        printf("|");
        printf("%*s", (int)((space - 2 - strlen(month[j])) / 2), " ");
        printf("%s", month[j]);
        printf("%*s", (int)((space - 1 - strlen(month[j])) / 2), " ");
        printf("|\n");
        dashline(daysize);
        //This is to print the day-of-week
        int size = ((daysize + 3) * 7 + 1);

        int day = 0;
        for (int i = 0; i < size; i++)
        {
            if (i == 0 || (i % (daysize + 3) == 0))
            {
                printf("|");
            }
            else if (i == size - 1)
            {
                printf("|");
            }
            else if ((i - 2) % (daysize + 3) == 0)
            {
                i--;
                char *pt;
                switch (day)
                {
                case 0:
                    pt = sun;
                    break;
                case 1:
                    pt = mon;
                    break;
                case 2:
                    pt = tue;
                    break;
                case 3:
                    pt = wed;
                    break;
                case 4:
                    pt = thu;
                    break;
                case 5:
                    pt = fri;
                    break;
                case 6:
                    pt = sat;
                    break;
                default:
                    printf("Error");
                    break;
                }

                int counter = 0;
                while (*pt != '\0' && counter < daysize)
                {
                    printf("%c", *pt);
                    counter++;
                    i++;
                    pt++;
                }
                day++;
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
        dashline(daysize);

        int i;
        for (i = 1; i < 31 + firstday - 1; i++)
        {
            if (i == 1)
            {
                printf("|");
            }
            if (i < firstday)
            {
                printf("%*s", daysize + 2, " ");
                printf("|");
            }
            if (i >= firstday)
            {
                printf(" %d", i - firstday + 1);
                if (i - firstday + 1 >= 10)
                {
                    printf("%*s", daysize - 1, " ");
                }
                if (i - firstday + 1 < 10)
                {
                    printf("%*s", daysize, " ");
                }
                printf("|");
            }
            if (i % 7 == 0 && i != 31 + firstday - 2)
            {
                printf("\n|");
            }
            if (i == 31 + firstday - 2)
            {
                break;
            }
        }
        firstday = i % 7 + 1;

        if (firstday != 1)
        {
            for (int k = 1; k <= 7 - firstday + 1; k++)
            {
                printf("%*s", daysize + 2, " ");
                printf("|");
            }
        }
        if (j == 11)
        {
            printf("\n");
            dashline(daysize);
        }
    }
    return 0;
}
