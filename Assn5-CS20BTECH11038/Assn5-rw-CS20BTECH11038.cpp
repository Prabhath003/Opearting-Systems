// including the libraries
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <random>
#include <semaphore.h>

// declaring the global variables
int nw, nr, kw, kr;
double mucs, murem;

// declaring the semaphores
sem_t rw_mutex;
sem_t mutex;
int read_count;

// a semaphore just to print statements to log file with mutual exclusion
sem_t print;

// variables to seed
int csSeed = std::chrono::system_clock::now().time_since_epoch().count();
int rmSeed = std::chrono::system_clock::now().time_since_epoch().count();

// random generators
std::default_random_engine csGenerator(csSeed);
std::default_random_engine rmGenerator(rmSeed);

// variable to create log file
std::fstream logfile;

// vectors to store the waiting time
std::vector<double> writerWaitingTime;
std::vector<double> readerWaitingTime;
// variable to store the worst case time
double writerWorstTime = 0;
double readerWorstTime = 0;

// function to get the current date and time
std::string getSysTime()
{
    time_t tmp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm *ltm = localtime(&tmp);
    std::string time = std::to_string(5 + ltm->tm_hour) + ":" + std::to_string(30 + ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);
    return time;
}

// variable to showing the thread number or ID
int writer_thread_turn = 1;

void writer()
{
    int thread_ID = writer_thread_turn++;

    // std::thread::id id = std::this_thread::get_id();

    // every thread goes through the critical section kw + 1 times
    for(int i = 0; i <= kw; i++)
    {
        // requesting to enter the CS
        std::string reqTime = getSysTime();
        auto start = std::chrono::high_resolution_clock::now();

        sem_wait(&print);
        logfile << i << "th CS Request by Writer Thread " << thread_ID << " at " << reqTime << std::endl;
        sem_post(&print);

        // Entry section
        sem_wait(&rw_mutex);

        std::string enterTime = getSysTime();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop -start);

        sem_wait(&print);
        logfile << i << "th CS Entry by Writer Thread " << thread_ID << " at " << enterTime << std::endl;

        writerWaitingTime[thread_ID] += duration.count();
        if(writerWorstTime < duration.count())
            writerWorstTime = duration.count();

        sem_post(&print);

        // Critical section
        // randCSTime
        // creating a random waiting time using exponential distribution
        std::exponential_distribution<double> dist1(1 / mucs);
        double number = dist1(csGenerator);
        std::chrono::duration<double, std::milli> randCSTime(number);
        std::this_thread::sleep_for(randCSTime); // simulate a thread writing in CS

        // exit Section
        sem_post(&rw_mutex);

        std::string exitTime = getSysTime();

        sem_wait(&print);
        logfile << i << "th CS Exit by Writer Thread " << thread_ID << " at " << exitTime << std::endl;
        sem_post(&print);

        // Remainder section
        // randRemTime
        // creating a random waiting time using the exponential distribution
        std::exponential_distribution<double> dist2(1 / murem);
        number = dist2(rmGenerator);
        std::chrono::duration<double, std::milli> randRemTime(number);
        std::this_thread::sleep_for(randRemTime); // simulate a thread executing in Remainder Section
    }
}

// variable to showing the thread number or ID
int reader_thread_turn = 1;

void reader()
{
    int thread_ID  = reader_thread_turn++;

    // std::thread::id id = std::this_thread::get_id();

    // Random csSeed, remSeed;

    // every thread goes through the critical section kr times
    for(int i = 0; i < kr; i++)
    {
        // requesting to enter the CS
        std::string reqTime = getSysTime();
        auto start = std::chrono::high_resolution_clock::now();

        sem_wait(&print);
        logfile << i << "th CS Request by Reader Thread " << thread_ID << " at " << reqTime << std::endl;
        sem_post(&print);

        // Entry section
        sem_wait(&mutex);
        read_count++;
        if(read_count == 1)
            sem_wait(&rw_mutex);
        sem_post(&mutex);

        std::string enterTime = getSysTime();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        sem_wait(&print);
        logfile << i << "th CS Entry by Reader Thread " << thread_ID << " at " << enterTime <<std::endl;

        readerWaitingTime[thread_ID] += duration.count();
        if(readerWorstTime < duration.count())
            readerWorstTime = duration.count();

        sem_post(&print);

        // Critical section
        // randCSTime
        std::exponential_distribution<double> dist1(1 / mucs);
        double number = dist1(csGenerator);
        std::chrono::duration<double, std::milli> randCSTime(number);
        std::this_thread::sleep_for(randCSTime); // simulate a thread reading from the CS.

        // Exit section
        sem_wait(&mutex);
        read_count--;
        if(read_count == 0)
            sem_post(&rw_mutex);
        sem_post(&mutex);

        std::string exitTime = getSysTime();

        sem_wait(&print);
        logfile << i << "th CS Exit by Reader Thread " << thread_ID << " at " << exitTime << std::endl;
        sem_post(&print);

        // Remainder section
        // randRemTime
        // creating a random waiting time using the exponential distribution
        std::exponential_distribution<double> dist2(1 / murem);
        number = dist2(rmGenerator);
        std::chrono::duration<double, std::milli> randRemTime(number);
        std::this_thread::sleep_for(randRemTime); // simulate a thread executing in Remainder section
    }
}

int main()
{
    // reading the file and taking the input into global variables
    std::ifstream input;
    input.open("inp-params.txt", std::ios::in);
    if(!input)
    {
        std::cout << "Error in opening input file" << std::endl;
        exit(1);
    }

    input >> nw;
    input >> nr;
    input >> kw;
    input >> kr;
    input >> mucs;
    input >> murem;

    // closing the input file
    input.close();

    // opening the output file to print the log
    logfile.open("RW-log.txt", std::ios::out);
    if(!logfile)
    {
        std::cout << "Error in creating log file" << std::endl;
        exit(1);
    }

    // opening the info file
    std::fstream waitTimeFile;
    waitTimeFile.open("RW-Average_time.txt", std::ios::out);
    if(!waitTimeFile)
    {
        std::cout << "Error in creating average time file" << std::endl;
        exit(1);
    }

    // initialising the semaphores
    read_count = 0;
    sem_init(&mutex, 0, 1);
    sem_init(&rw_mutex, 0, 1);
    sem_init(&print, 0, 1);

    // creating nw writer threads
    std::thread Wthreads[nw];

    // assigning threads the writer() function
    for(unsigned i = 0; i < nw; i++)
    {
        writerWaitingTime.push_back(0);
        Wthreads[i] = std::thread(writer);
    }

    // creating nr reading threads
    std::thread Rthreads[nr];

    // assigning threads the reader() function
    for(unsigned i = 0; i < nr; i++)
    {
        readerWaitingTime.push_back(0);
        Rthreads[i] = std::thread(reader);
    }

    // joining all threads
    for(unsigned i = 0; i < nw; i++)
        Wthreads[i].join();
    for(unsigned i = 0; i < nr; i++)
        Rthreads[i].join();

    // printing the waiting times to the average_time file
    // writer thread
    double totalWriterWaitingTime = 0;
    for(unsigned i = 0; i < nw; i++)
    {
        waitTimeFile << "Average Waiting Time of Writer Thread " << i << ": " << writerWaitingTime[i] / kw << std::endl;
        totalWriterWaitingTime += writerWaitingTime[i] / kw;
    }
    waitTimeFile << std::endl << "Average Waiting Time of All Writer Threads: " << totalWriterWaitingTime / nw << std::endl << std::endl;

    waitTimeFile << "Worst-case Waiting Time of Writer Threads: " << writerWorstTime << std::endl << std::endl;

    // reader thread
    double totalReaderWaitingTime = 0;
    for(unsigned i = 0; i < nr; i++)
    {
        waitTimeFile << "Average Waiting Time of Reader Thread " << i << ": " << readerWaitingTime[i] / kr << std::endl;
        totalReaderWaitingTime += readerWaitingTime[i] / kr;
    }
    waitTimeFile << std::endl << "Average Waiting Time of All Reader Threads: " << totalReaderWaitingTime / nr << std::endl << std::endl;

    waitTimeFile << "Worst-case Waiting Time of Reader Threads: " << readerWorstTime << std::endl << std::endl;

    // total waiting time of all threads
    waitTimeFile << "Total Average Wating Time of All Threads: " << (totalWriterWaitingTime + totalReaderWaitingTime) / (nr + nw) << std::endl;

    // closing the output file
    logfile.close();
    waitTimeFile.close();

    return 0;
}