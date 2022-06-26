- INITIALLY COME TO THIS DIRECTORY IN TERMINAL

- BEFORE CREATE YOUR INPUT FILE NAMING IT - "inp-params.txt"
    It should consist the following parameters: 
    line 1 >> n, the number of processes; 
    -> followed by n lines consisting of Pi(process id), t (processing time), 
        p (period) and k (the number of times each process repeats).
    Example file with format:
        line 1  >>   10
        line 2  >>   1 20 90 10
        line 3  >>   2 30 250 10
        line 4  >>   3 70 370 10
        line 5  >>   4 50 330 10
        line 6  >>   5 125 2000 10
        line 7  >>   6 35 80 10
        line 8  >>   7 25 50 10
        line 9  >>   8 20 100 10
        line 10 >>   9 25 40 10
        line 11 >>   10 30 75 10



- COMMAND TO COMPILE: 
  - FOR "Assgn3-EDF-CS20BTECH11038.cpp"
    "g++ -std=c++2a Assgn3-EDF-CS20BTECH11038.cpp -o EDF"
        -> This will create 'EDF' named executable file

- COMMAND TO RUN THE EXECUTABLE (only after compiling) OF "EDF": 
    "./EDF"
        -> This will create two files:
            - "EDF-Log.txt" (contatins the log of how processes are running)
            - "EDF-Stats.txt" (contains the final statistics of output)



- COMMAND TO COMPILE: 
  - FOR "Assgn3-RMS-CS20BTECH11038.cpp"
    "g++ -std=c++2a Assgn3-RMS-CS20BTECH11038.cpp -o RMS"
        -> This will create 'RMS' named executable file

- COMMAND TO RUN THE EXECUTABLE (only after compiling) OF "RMS": 
    "./RMS"
        -> This will create two files:
            - "RMS-Log.txt" (contatins the log of how processes are running)
            - "RM-Stats.txt" (contains the final statistics of output)