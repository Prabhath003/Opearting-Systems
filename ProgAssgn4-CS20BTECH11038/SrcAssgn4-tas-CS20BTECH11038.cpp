// including the libraries
#include <iostream>
#include <fstream>
#include <thread>
#include <ctime>
#include <chrono>
#include <atomic>
#include <cstring>
#include <random>

// declaring the global variables
int n, k;
double lamda1, lamda2;

// declaring the atomic lock
std::atomic_flag lock = ATOMIC_FLAG_INIT;

// declaring a random generator
std::default_random_engine *generator;

// variable to create ouput file
std::fstream output;

// function to get the current date and time
time_t getSysTime()
{
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

// variable to showing the thread number or ID
int thread_turn = 0;

// function to test the implementation of the TAS algorithm
void testCS()
{
    int thread_ID = thread_turn++;

    // std::thread::id id = std::this_thread::get_id();

    // every thread goes through the critical section k times
    for(unsigned i = 0; i < k; i++)
    {
        // requesting to enter the CS
        time_t reqEnterTime = getSysTime();

        // busy waiting by applying the atomic TAS function on lock
        while(std::atomic_flag_test_and_set_explicit(&lock, std::memory_order_acquire));    // do nothing

        output << i << "th CS Request at " << std::strtok(ctime(&reqEnterTime), "\n") << " by thread " << thread_ID << std::endl;

            // Entry Section;

        time_t actEnterTime = getSysTime();
        output << i << "th CS Entry at " << std::strtok(ctime(&actEnterTime), "\n") << " by thread " << thread_ID << std::endl;

        // output << "waiting time: " << difftime(actEnterTime, reqEnterTime) << std::endl;

        // Critical section
        // creating a random waiting time using exponential distribution
        std::exponential_distribution<double> dist1(1 / lamda1);
        double number = dist1(*generator);
        std::chrono::duration<double> t1(number);
        std::this_thread::sleep_for(t1);  // Simulation of critical-section

            // Exit Section

        time_t exitTime = getSysTime();
        output << i << "th CS Exit at " << std::strtok(ctime(&exitTime), "\n") << " by thread " << thread_ID << std::endl;

        // releasing the lock using atomic_flag functions
        std::atomic_flag_clear_explicit(&lock, std::memory_order_release);

        // Remainder section
        // creating a random waiting time using the exponential distribution
        std::exponential_distribution<double> dist2(1 / lamda2);
        number = dist2(*generator);
        std::chrono::duration<double> t2(number);
        std::this_thread::sleep_for(t2);  // Simulation of Remainder Section
    }
}

int main()
{
    // using seed
    int seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator = new std::default_random_engine(seed);

    // reading the file and taking the input into global variables
    std::ifstream input;
    input.open("inp-params.txt", std::ios::in);
    if(!input)
    {
        std::cout << "Error in opening input file" << std::endl;
        exit(1);
    }

    input >> n;
    input >> k;
    input >> lamda1;
    input >> lamda2;

    // closing the input the file
    input.close();

    // opening the ouput file to print the log
    output.open("TAS-ME.txt", std::ios::out);
    if(!output)
    {
        std::cout << "Error in creating log file" << std::endl;
        exit(1);
    }

    // creating n threads
    std::thread threads[n];

    // std::cout << n << " " << k << " " << lamda1 << " " << lamda2 << std::endl;

    // assigning threads the testCS function
    for(unsigned i = 0; i < n; i++)
    {
        threads[i] = std::thread(testCS);
    }

    // joining all the threads 
    for(unsigned i = 0; i < n; i++)
    {
        threads[i].join();
    }

    // closing the output file
    output.close();

    return 0;
}