// including the libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

/*
 * CONDITION: the input file should be of format
 *      line 1 >>   Number of threads
 *      line 2 >>   source point (X,Y)
 *      line 3 >>   number of points in the set
 *      line 4 >>   Set of destination points {(X1,Y1),(X2,Y2),......(Xn,Yn)}
 */

// declaring a struct to store the point
struct _point_t
{
        double x;
        double y;
};
typedef struct _point_t point_t;

// declaring the global variables
int num_of_threads;
int num_of_points;
point_t p_main;
point_t* p_array;
point_t* thread_array;

// function to get the square of distance between two points
double getSquareDistance(point_t p1, point_t p2);

// function to find point with minimum distance form the main point
point_t findMin(point_t *arr, int from_end, int to_end);

// function to assign work to threads
int thread_turn = 0;
void *doSelection(void *param);

int main()
{
        // taking the name of input file
        char filename[50];
        printf("Enter the name of sample text file: ");
        scanf("%s", filename);
        
        // opening the input file
        FILE *input;
        input = fopen(filename, "r");
        
        // checking for any error in opening file
        if(input == NULL)
        {
                fprintf(stderr, "Cannot open file\n");
                exit(0);
        }

        // taking the input from the file
        char enter, brac1, comma, brac2;
        fscanf(input, "%d%c", &num_of_threads, &enter);
        fscanf(input, "%c%lf%c%lf%c%c", &brac1, &p_main.x, &comma, &p_main.y, &brac2, &enter);
        fscanf(input, "%d%c", &num_of_points, &enter);

        p_array = (point_t*)malloc(num_of_points * sizeof(point_t));
        thread_array = (point_t*)malloc(num_of_threads * sizeof(point_t));

        for(unsigned i = 0; i < num_of_points; i++)
        {
                fscanf(input, "%c%lf%c%lf%c", &brac1, &p_array[i].x, &comma, &p_array[i].y, &brac2);
        }

        // closing the input file
        fclose(input);

        // variables to check the time
        time_t begin, end;

        // creating array of thread ids
        pthread_t threads[num_of_threads];

        // starting the clock
        begin = clock();

        // assigning work to each thread
        for (int i = 0; i < num_of_threads; i++)
                pthread_create(&threads[i], NULL, &doSelection, (void*)NULL);

        // joining all the threads to main thread
        for (int i = 0; i < num_of_threads; i++)
                pthread_join(threads[i], NULL);

        // selecting the point with minimum distance from the points selected by each thread
        point_t p = thread_array[0];
        double min = getSquareDistance(p_main, thread_array[0]);
        for(unsigned i = 1; i < num_of_threads; i++)
        {
                double dst = getSquareDistance(p_main, thread_array[i]);
                if(dst < min)
                {
                        min = dst;
                        p = thread_array[i];
                }
        }

        // stopping the clock
        end = clock();

        // calculating the time taken and writing to the output
        printf("%f microseconds\n", (double)(end - begin));

        // writing the selected point to the output
        printf("(%lf,%lf)\n", p.x, p.y);

        // creating the name of output file
        char outputfile[100] = "output";
        strcat(outputfile, filename);

        // opening the output file to write
        FILE *output;
        output = fopen(outputfile, "w");
        // checking for any array in opening the output file
        if(output == NULL)
        {
                fprintf(stderr, "Error in opening ouput file\n");
                exit(0);
        }

        // calculating the time taken and writing to the output
        fprintf(output, "%f microseconds\n", (double)(end - begin));

        // writing the selected point to the output
        fprintf(output, "(%lf,%lf)\n", p.x, p.y);

        // closing the output file
        fclose(output);

        printf("Ouput is in created file, \"%s\"\n", outputfile);

        return EXIT_SUCCESS;
}

// function to get the square of distance between two points
double getSquareDistance(point_t p1, point_t p2)
{
        return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
}

// function to find point with minimum distance form the main point
point_t findMin(point_t *arr, int from_end, int to_end)
{
        point_t p = p_array[from_end];
        double min = getSquareDistance(p_main, arr[from_end]);
        for(int i = from_end + 1; i <= to_end; i++)
        {
                double dst = getSquareDistance(p_main, arr[i]);
                if(dst < min)
                {
                        min = dst;
                        p = arr[i];
                }
        }

        return p;
}

// function to assign work to threads
void *doSelection(void *param)
{
        int thread_num = thread_turn++;
        
        // depending on the thread order we are assigning a particular part to that thread
        int left_point = thread_num * (num_of_points / (float)num_of_threads);
        int right_point = (thread_num + 1) * (num_of_points / (float)num_of_threads) - 1;

        // writing the selected point ouput to a global array
        thread_array[thread_num] = findMin(p_array, left_point, right_point);

        pthread_exit(0);
}