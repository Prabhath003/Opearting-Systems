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
std::atomic<int> lock{0};

// declaring the random generator
std::default_random_engine *generator;

// creating waiting array to implement the bounded-waitingw
std::atomic<bool> *waiting; 

// variable to create output file
std::fstream output;

// entry section to implement the busy waiting using the atomic CAS and bounded-waiting
void entry_section(int i)
{
    waiting[i].store(true, std::memory_order_relaxed);
    int key = 1;
    // busy waiting by using the CAS on lock along with bounded-waiting
    while(waiting[i].load(std::memory_order_relaxed) && key == 1)
    {
        int expected_value = 0;
        key = !lock.compare_exchange_strong(expected_value, 1);
    }
    waiting[i].store(false, std::memory_order_relaxed);
}

// function to implement the exit section
void exit_section(int i)
{
    // applying the bounded-waiting statements to select the next thread
    int j = (i + 1) % n;
    while((j != i) && !waiting[j].load(std::memory_order_relaxed))
        j = (j + 1) % n;

    // giving signal to next process updating the lock or waiting[]
    if(j == i)
        lock.store(0, std::memory_order_relaxed);
    else
        waiting[j].store(false, std::memory_order_relaxed);
}

// function to get the current date and time
time_t getSysTime()
{
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

// variable to show the thread number or ID
int thread_turn = 0;

// function to test the implementation of the TAS algorithm
void testCS()
{
    int thread_ID = thread_turn++;

    // std::thread::id id = std::this_thread::get_id();

    // every thread goes through the critical section k times
    for(unsigned i = 0; i < k; i++)
    {
        // requesting the enter the CS
        time_t reqEnterTime = getSysTime();

        // calling the entry section by requesting
        entry_section(thread_ID);

        output << i << "th CS Request at " << std::strtok(ctime(&reqEnterTime), "\n") << " by thread " << thread_ID << std::endl;

            //Entry Section;

        time_t actEnterTime = getSysTime();
        output << i << "th CS Entry at " << std::strtok(ctime(&actEnterTime), "\n") << " by thread " << thread_ID << std::endl;
        
        // output << "waiting time: " << difftime(actEnterTime, reqEnterTime) << std::endl;

        // creating a random waiting time using the exponent distribution
        std::exponential_distribution<double> dist1(1 / lamda1);
        double number = dist1(*generator);
        std::chrono::duration<double> t1(number);
        std::this_thread::sleep_for(t1);  // Simulation of critical-section

            // Exit Section

        time_t exitTime = getSysTime();
        output << i << "th CS Exit at " << std::strtok(ctime(&exitTime), "\n") << " by thread " << thread_ID << std::endl;

        exit_section(thread_ID);

        // remainder section
        // creating a random waiting time using the exponent distribution
        std::exponential_distribution<double> dist2(1 / lamda2);
        number = dist2(*generator);
        std::chrono::duration<double> t2(number);
        std::this_thread::sleep_for(t2);  // Simulation of Reminder Section
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

    // opening the output file to print the log
    output.open("Bounded-CAS-ME.txt", std::ios::out);
    if(!output)
    {
        std::cout << "Error in creating log file" << std::endl;
        exit(1);
    }

    // cerating n threads
    waiting = (std::atomic<bool>*)std::malloc(n * sizeof(std::atomic<bool>));
    for(unsigned i = 0; i < n; i++)
    {
        waiting[i].store(false, std::memory_order_relaxed);
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