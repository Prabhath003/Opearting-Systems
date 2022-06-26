- INITIALLY COME TO THIS DIRECTORY IN TERMINAL

- COMMAND TO COMPILE: 
    "gcc Assgn2Src-CS20BTECH11038.c -pthread"
        -> This will create 'a.out' named executable file

- COMMAND TO RUN THE EXECUTABLE (only after compiling): 
    "./a.out"
        -> then it will ask for name of input file(with extension) with array
            to be sorted,
            A sample input file should following format:
             line 1 >>   Number of threads
             line 2 >>   array size
             line 3 >>   array elements with only one space between
            Example file with format:
             line 1 >>   2
             line 2 >>   5
             line 3 >>   4 -54 234 -94 18
        -> after we enter the input file name it will show the name
            of ouput file created.
        -> the output file will contain the time taken to sort and the sorted array