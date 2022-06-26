// including the libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

/*
 * CONDITION: the input file should be of format
 *      line 1 >>   Number of threads
 *      line 2 >>   array size
 *      line 3 >>   array elements with only one space between
 */

// variables to store the required parameters
int thread_turn, num_of_threads, size_of_arr, *array;

// function to do merge sort
void mergeSort(int arr[], int left_end, int right_end);
void merge(int arr[], int left_end, int mid_point, int right_end);

// function to assign work to the threads
void *doSort(void *param);

int main()
{
    // taking the name of input file
    char filename[100];
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

    // reading the the file and storing in the variables
    fscanf(input, "%d", &num_of_threads);
    fscanf(input, "%d", &size_of_arr);

    int arr[size_of_arr];
    array = arr;
    // reading and creating the array
    for(unsigned i = 0; i < size_of_arr; i++)
        if(fscanf(input, "%d", &array[i]) == EOF)
        {
            fprintf(stderr, "The number elements available are not equal to array size\n");
            exit(0);
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
    thread_turn = 0;
    for (int i = 0; i < num_of_threads; i++)
        pthread_create(&threads[i], NULL, doSort, (void*)NULL);

    // joining all the threads to main thread
    for (int i = 0; i < num_of_threads; i++)
        pthread_join(threads[i], NULL);

    // merging all the parts of array which are sorted using the threads
    for(unsigned i = 0; i < num_of_threads - 1; i++)
    {
        int right_point = (i + 2) * (size_of_arr / (float)num_of_threads) - 1;
        int mid_point = (i + 1) * (size_of_arr / (float)num_of_threads) - 1;

        merge(array, 0, mid_point, right_point);
    }

    // stopping the clock
    end = clock();

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

    // calculating the time taken and writing to the output file
    fprintf(output, "Time taken: %f\n", (end - begin) / (double)CLOCKS_PER_SEC);

    // writing the sorted array to the output file
    fprintf(output, "Sorted array: ");
    for (int i = 0; i < size_of_arr; i++)
        fprintf(output, "%d ", array[i]);

    // closing the output file
    fclose(output);

    printf("Ouput is in created file, \"%s\"\n", outputfile);

    return EXIT_SUCCESS;
}

// function to merge to parts of a given array
void merge(int arr[], int left_end, int mid_point, int right_end)
{
    // caculating the lenghts of the two parts
    int left_len, right_len;
    left_len = mid_point - left_end + 1;
    right_len = right_end - mid_point;

    // copying the two parts into two arrays
    int left_temp[left_len], right_temp[right_len];

    for (unsigned i = 0; i < left_len; i++)
        left_temp[i] = arr[left_end + i];
    for (unsigned i = 0; i < right_len; i++)
        right_temp[i] = arr[mid_point + 1 + i];

    // and writing back to the array in sequencial order from two arrays
    unsigned i, j, k;
    for(i = 0, j = 0, k = left_end; i < left_len && j < right_len; k++)
    {
        if(left_temp[i] <= right_temp[j])
        {
            arr[k] = left_temp[i];
            i++;
        }
        else
        {
            arr[k] = right_temp[j];
            j++;
        }
    }

    while (i < left_len)
    {
        arr[k] = left_temp[i];
        i++;
        k++;
    }

    while (j < right_len)
    {
        arr[k] = right_temp[j];
        j++;
        k++;
    }
}

// function to do merge sort
void mergeSort(int arr[], int left_end, int right_end)
{
    // checking if input is correct
    if(left_end >= right_end)
        return;
    // calculating the mid point and merge sorting those two parts separately
    int mid_point = left_end + (right_end - left_end) / 2;
    mergeSort(arr, left_end, mid_point);
    mergeSort(arr, mid_point + 1, right_end);

    // then merging the two parts
    merge(arr, left_end, mid_point, right_end);
}

// function to assign work to thread
void *doSort(void *param)
{
    int thread_num = thread_turn++;
    // depending on the thread order we are assigning a particular part to that thread
    int left_point = thread_num * (size_of_arr / (float)num_of_threads);
    int right_point = (thread_num + 1) * (size_of_arr / (float)num_of_threads) - 1;

    // checking if the bounds of the part are correct and then calling the mergesort algo on this part
    if(left_point < right_point)
        mergeSort(array, left_point, right_point);
    
    pthread_exit(0);
}