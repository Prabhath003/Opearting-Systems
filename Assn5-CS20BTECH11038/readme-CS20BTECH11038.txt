- INITIALLY COME TO THIS DIRECTORY IN TERMINAL

- BEFORE ALL CREATE YOUR INPUT FILE NAMING IT - "inp-params.txt"
    It should consist of the following parameters: 
        -> nw, nr, kw, kr, mucs, murem; 
    An example file with format:
        line 1  >>10 10 10 10 5 20


Disclaimer: Please compile every time you run executable

- COMMAND TO COMPILE: 
  - FOR "Assn5-rw-CS20BTECH11038.cpp"
    "g++ -std=c++2a -pthread Assn5-rw-CS20BTECH11038.cpp -o rw"
        -> This will create 'rw' named executable file

- COMMAND TO RUN THE EXECUTABLE (only after compiling) OF "RW": 
    "./rw"
        -> This will create two files:
            - "RW-log.txt" (contains the log of how processes ran)
            - "RW-Average_time.txt (contains info about average times of each thread and worst-case time)



- COMMAND TO COMPILE: 
  - FOR "Assn5-frw-CS20BTECH11038.cpp"
    "g++ -std=c++2a -pthread Assn5-frw-CS20BTECH11038.cpp -o frw"
        -> This will create 'frw' named executable file

- COMMAND TO RUN THE EXECUTABLE (only after compiling) OF "FairRW": 
    "./frw"
        -> This will create two files:
            - "FairRW-log.txt" (contains the log of how processes ran)
            - "FairRW-Average_time.txt (contains info about average times of each thread and worst-case time)