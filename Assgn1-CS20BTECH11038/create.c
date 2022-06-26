#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(0));
    FILE *input;
        input = fopen("sample1.txt", "w");
        // checking for any array in opening the output file
        if(input == NULL)
        {
                fprintf(stderr, "Error in opening ouput file\n");
                exit(0);
        }

        int num_of_threads = 16;
        int num_of_points = 4000;
        char enter = '\n';
        char brac1 = '(';
        char brac2 = ')';
        char comma = ',';

        fprintf(input, "%d%c", num_of_threads, enter);
        fprintf(input, "%c%d%c%d%c%c", brac1, rand()%100, comma, rand()%100, brac2, enter);
        fprintf(input, "%d%c", num_of_points, enter);

        for(unsigned i = 0; i < num_of_points; i++)
        {
            fprintf(input, "%c%d%c%d%c", brac1, rand()%100, comma, rand()%100, brac2);
        }

        // closing the output file
        fclose(input);

    return EXIT_SUCCESS;
}