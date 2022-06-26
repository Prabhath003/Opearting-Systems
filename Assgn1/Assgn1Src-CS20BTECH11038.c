/* INCLUDING THE LIBRARIES */
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* COMMAND LINE ARGUMENTS TO TAKE INPUT FROM THROUGH COMMAND LINE */
int main(int argc, char *argv[])
{
    int n;

    /* TAKING INPUT FROM COMMAND LINE */
    /* ARGUMENTS PASSES ARE SUFFICIENT */
    if(argc == 2)
    {
        /* VERIFYING THAT THE INPUT IS POSITIVE INTEGER */
        if(atoi(argv[1]) <= 0)
        {
            fprintf(stderr, "Entered non-positive integer!\n");
            return 1;
        }
        else
        {
            /* COPYING THE INPUT IN VARIABLE n */
            n = atoi(argv[1]);
            printf("Input number is: %d\n", n);
        }
    }
    /* IF MORE THAN REQUIRED ARGUMENTS ARE PASSED */
    else if(argc > 2)
    {
        fprintf(stderr, "More arguments than required!\n");
        return 1;
    }
    /* IF ARGUMENTS LESS THAN REQUIRED FORM */
    else
    {
        fprintf(stderr, "atleast one extra argument in expected!\n");
        return 1;
    }

    pid_t pid;
    /* FORK A CHILD PROCESS */
    pid = fork(); /* GETTING PID VALUE OF PROCESS */

    /* PID VALUE LESS THAN 0 */
    if(pid < 0)
    {
        /* ERROR OCCURED */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    /* PID VALUE EQUAL TO 0 */
    else if(pid == 0)
    {
        printf("Child process...\n");
        /* CHILD PROCESS */
        /* PRINTING THE SEQUENCE OF COLLATZ NUMBERS FOR n */
        printf("%d", n);
        while(n > 1)
        {
            /* n = n / 2, IF n IS EVEN */
            if(n % 2 == 0)
                n = n / 2;
            /* n = 3n + 1, IF n IS ODD */
            else
                n = 3 * n + 1;
            printf(", %d", n);
        }
        printf("\n");

        /* END OF CHILD PROCESS */
        printf("...Child Process Complete\n");
    }
    else
    {
        /* PRAENT PROCESS */
        /* PARENT WILL WAIT FOR THE CHILD TO COMPLETE */
        printf("Parent Process...\n");
        
        wait(NULL); /* WAITING */

        /* SHOWING THAT PARENT AND CHILD PROCESSES HAVE DIFFERENT COPIES OF n */
        printf("The value of n: %d\n",n);

        /* END OF PARENT PROCESS */
        printf("...Parent Process Complete\n");
    }
    return 0;
}