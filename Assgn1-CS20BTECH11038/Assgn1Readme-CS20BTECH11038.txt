- INITIALLY COME TO THIS DIRECTORY IN TERMINAL

- COMMAND TO COMPILE: 
    "gcc Assgn1Src-CS20BTECH11038.c -pthread"
        -> This will create 'a.out' named executable file

- COMMAND TO RUN THE EXECUTABLE (only after compiling): 
    "./a.out"
        -> then it will ask for name of input file(with extension) with array
            to be sorted,
            A sample input file should be in following format:
             line 1 >>   Number of threads
             line 2 >>   source point (X,Y)
             line 3 >>   number of points in the set
             line 4 >>   Set of destination points {(X1,Y1),(X2,Y2),......(Xn,Yn)}
            Example file with format:
             line 1 >>   2
             line 2 >>   (1,1)
             line 3 >>   4
             line 4 >>   (2,2)(4,5)(0,3)(1,2)
        -> after we enter the input file name, the output will be shown in the terminal