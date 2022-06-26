- INITIALLY COME TO THIS DIRECTORY IN TERMINAL

- BEFORE ALL CREATE YOUR INPUT FILE NAMING IT - "inp-params.txt"
    It should consist of the following parameters: 
        -> n, k, lamda1, lamda2; 
    Example file with format:
        line 1  >>10 10 5 20


Disclaimer: Please compile every time you run executable

- COMMAND TO COMPILE: 
  - FOR "SrcAssgn4-tas-CS20BTECH11038.cpp"
    "g++ -std=c++2a -pthread SrcAssgn4-tas-CS20BTECH11038.cpp -o tas"
        -> This will create 'tas' named executable file

- COMMAND TO RUN THE EXECUTABLE (only after compiling) OF "TAS": 
    "./tas"
        -> This will create a file:
            - "TAS-ME.txt" (contatins the log of how processes ran)



- COMMAND TO COMPILE: 
  - FOR "SrcAssgn4-cas-CS20BTECH11038.cpp"
    "g++ -std=c++2a -pthread SrcAssgn4-cas-CS20BTECH11038.cpp -o cas"
        -> This will create 'cas' named executable file

- COMMAND TO RUN THE EXECUTABLE (only after compiling) OF "CAS": 
    "./cas"
        -> This will create a file:
            - "CAS-ME.txt" (contatins the log of how processes ran)



- COMMAND TO COMPILE: 
  - FOR "SrcAssgn4-cas-bounded-CS20BTECH11038.cpp"
    "g++ -std=c++2a -pthread SrcAssgn4-cas-bounded-CS20BTECH11038.cpp -o bcas"
        -> This will create 'bcas' named executable file

- COMMAND TO RUN THE EXECUTABLE (only after compiling) OF "TAS": 
    "./bcas"
        -> This will create a file:
            - "Bounded-CAS-ME.txt" (contatins the log of how processes ran)




(you can print the waiting time by removing the comments for the statement to publish it in the ‘testCS()’ function for easy cross-checking)