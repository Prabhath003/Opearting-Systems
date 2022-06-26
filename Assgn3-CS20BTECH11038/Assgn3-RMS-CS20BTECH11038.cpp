// INCLUDING LIBRARIES
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <chrono>

// CONTEXT SWITCH TIME
#define contextSwitch 0.01

// STRUCT TO DETAILS OF THE PROCESS
class Process
{
    public:
        std::string ID;
        int Process_time;
        float remaining_time;
        int next_deadline;
        int period;
        int repetitions;
        float waitingTime;
        float stop_time;

};

// STRUCT TO STORE THE WAITING TIMES OF ITS ITERATIONS
struct waitingTime_t
{
    std::string ID;
    std::vector<float> waitingTime;
};

// CLASS CREATED TO DEFINE THE PRIORITY CRITERION
class Priority
{
    public:
        int operator()(const Process &p1, const Process &p2)
        {
            if(p1.period == p2.period)
            {
                return p1.remaining_time < p2.remaining_time;
            }
            
            return p1.period < p2.period;
        }
};

// FUNCTION TO LOAD PROCESSES DATA FROM A FILE
void loadData(std::vector<Process> *Processes, int *n)
{
    // OPENING THE THE FILE WITH PROCESS DETAILS
    std::ifstream input;
    input.open("inp-params.txt", std::ios::in);

    bool turn = false;

    // READING THE FILE AND STORING THOSE IN VECTOR
    if(input.is_open())
    {
        std::string fileLine;
        while(getline(input, fileLine))
        {
            if(turn == false)
            {
                *n = std::stoi(fileLine);
                turn = true;
                continue;
            }

            // READING THE FILES WITH PROCESS DETAILS
            Process temp;
            std::string fragment;
            unsigned i;

            for(i = 0; fileLine[i] != ' '; i++)
                fragment = fragment + fileLine[i];
            temp.ID = fragment;
            fragment = "\0";

            for(i = i + 1; fileLine[i] != ' '; i++)
                fragment = fragment + fileLine[i];
            temp.Process_time = stoi(fragment);
            temp.remaining_time = temp.Process_time;
            fragment = "\0";

            for(i = i + 1; fileLine[i] != ' '; i++)
                fragment = fragment + fileLine[i];
            temp.period = stoi(fragment);
            temp.next_deadline = temp.period;
            fragment = "\0";

            for(i = i + 1; i < fileLine.length(); i++)
                fragment = fragment + fileLine[i];
            temp.repetitions = stoi(fragment);
            temp.waitingTime = 0;
            temp.stop_time = 0;
            Processes->push_back(temp);
        }
    }
    // CLOSING THE FILE
    input.close();
}

int main()
{
    // VARIABLE TO STORE THE NUMBER OF PROCESSES
    int num_of_processes;

    // VECTOR TO STORE THE STRUCTS WITH PROCESS DETAILS
    std::vector<Process> processes;

    // CALLING FUNCTION TO LOAD DATA INTO THE ABOVE VETOR
    loadData(&processes, &num_of_processes);

    // CREATING A RECORD TO STORE THE WAITING TIMES
    std::vector<waitingTime_t> record;
    for(auto itr : processes)
    {
        waitingTime_t temp;
        temp.ID = itr.ID;
        record.push_back(temp);
    }

    // CREATING THE NAMES OF LOG FILE AND STATISTICS FILE
    std::string log = "RMS-Log.txt";
    std::string stat = "RM-Stats.txt";

    // VARIABLE TO STORE THE STATISTICS
    int num_of_processes_came_into_system = 0;
    int num_of_processes_succesfully_completed = 0;
    int num_of_processes_missed = 0;

    // VARIABLE TO CREATE LOG AND STATISTICS FILE
    std::fstream logFile, statFile;

    // CREATING THE LOG FILE
    logFile.open(log, std::ios::out);
    if(!logFile)
    {
        std::cout << "Error in creating log file" << std::endl;
        exit(1);
    }

    // PRINTING THE PROCESS DETAILS TO LOG FILE
    for(auto itr : processes)
    {
        logFile << "Process " << itr.ID << " : ";
        logFile << "processing time=" << itr.Process_time << "; ";
        logFile << "deadline=" << itr.period << "; ";
        logFile << "period=" << itr.period << " ";
        logFile << "joined the system at time 0" << std::endl;
    }

    // CREATING THE CHECKPOINTS OF DEADLINES
    std::set<int, std::less<int>> deadline_check_points;
    for(auto temp : processes)
    {
        int repetitions = temp.repetitions;
        int period = temp.period;
        for(unsigned i = 1; i <= repetitions + 1; i++)
        {
            deadline_check_points.insert(period * i);
        }
    }
    deadline_check_points.insert(0);

    // PARAMETERS TO SEND A PROCESS TO RUNNING STATE
    Process currentProcess;
    float currentProcessTime;
    float currentTime = 0;
    bool free = true, error = false;
    bool end = false;

    // CREATING A WAITING QUEUE
    std::vector<Process> waitingQueue;

    // RUNNING THE FOR LOOP WHICH CHECKS THE STATUS AT EVERY POINT OF DEADLINE
    for(std::set<int>::iterator it = deadline_check_points.begin(); it != --deadline_check_points.end() && !end; it++)
    {
        end = true;
        for(unsigned i = 0; i < processes.size(); i++)
        {
            if(processes[i].repetitions > 0)
            {
                end = false;
            }
        }
        if(!waitingQueue.empty() || free == false) end = false;
        if(end) break;

        // IF PROCESSOR IS FREE
        if(free == true)
        {
            // CHECKING THE TIME FOR WHICH THE CPU IS IDLE
            if(*it - currentTime > 0)
            {
                logFile << "CPU is idle till time " << *it << std::endl;
            }
            // UPDATING THE CURRENT TIME
            currentTime = *it;

            for(int itr = 0; itr < num_of_processes; itr++)
            {
                // CHECKING WHETHER A PROCESS SHOULD BE SENT INTO WAITING QUEUE
                if(*it % processes[itr].period == 0 && processes[itr].repetitions > 0)
                {
                    // CHECKING FOR THE DEADLINE OF ANY PROCESSES TO TERMINATE
                    for(unsigned i = 0; i < waitingQueue.size(); i++)
                    {
                        if(processes[itr].ID == waitingQueue[i].ID || waitingQueue[i].next_deadline <= currentTime)
                        {
                            logFile << "Process " << waitingQueue[i].ID << " missed deadline at time " << currentTime << std::endl;
                            logFile << "Terminating Process " << waitingQueue[i].ID << std::endl;

                            waitingQueue[i].waitingTime += currentTime - waitingQueue[i].stop_time;

                            // TAKING THE NOTE OF THE WAITING TIME OF THE TERMINATING PROCESS
                            for(unsigned j = 0; j < record.size(); j++)
                            {
                                if(record[j].ID == waitingQueue[i].ID)
                                {
                                    record[j].waitingTime.push_back(waitingQueue[i].waitingTime);
                                    break; 
                                }
                            }

                            // TERMINATING A PROCESS BY REMOVING IT FROMT THE WAITING QUEUE
                            waitingQueue.erase(waitingQueue.begin() + i);
                            num_of_processes_missed++;

                            // VARIABLE TO KEEP A NOTE OF TERMINATION
                            error = true;
                            break;
                        }
                    }

                    // PUSHING THE CURRENT ENTERING PROCESS INTO WAITING QUEUE
                    processes[itr].stop_time = currentTime;
                    waitingQueue.push_back(processes[itr]);

                    // UPDATING THE DATA OF THE PROCESSES
                    processes[itr].next_deadline += processes[itr].period;
                    processes[itr].repetitions--;

                    // UPDATING THE STATISTIC VARIABLE
                    num_of_processes_came_into_system++;
                }
            }
            // MAINTAINING THE PRORITY ORDER
            std::sort(waitingQueue.begin(), waitingQueue.end(), Priority());

            // RUNNING THE WAITING QUEUE
            while(!waitingQueue.empty())
            {
                // SELECTING FROM THE WAITING QUEUE
                auto start = std::chrono::high_resolution_clock::now();
                currentProcess = waitingQueue[0];
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration<float, std::chrono::milliseconds::period>(stop - start);

                // ADDING THE SELECTION TIME
                currentTime += duration.count();

                if(currentProcess.remaining_time == currentProcess.Process_time)
                    logFile << "Process " << currentProcess.ID << " starts execution at time " << currentTime << std::endl;
                else
                    logFile << "Process " << currentProcess.ID << " resumes execution at time " << currentTime << std::endl;

                // UPDATING THE WAITING TIME
                currentProcess.waitingTime += currentTime - currentProcess.stop_time;

                error = false;

                // REMOVING THE RUNNING PROCESS FROM THE WAITING QUEUE
                waitingQueue.erase(waitingQueue.begin());
                currentProcessTime = currentProcess.remaining_time;

                // UPDATING THE STATUS OF PROCESSOR
                // IF CURRENT PROCESS REMAINING TIME MORE THAN NEXT CHECK POINT
                if(currentProcessTime + currentTime > *(++it))
                {
                    it--;
                    free = false;
                    break;
                }
                // IF LESS THAN THE NEXT CHECK POINT
                else
                {
                    it--;
                    free = true;
                    // FREEING THE PROCESSOR AND FINISHING THE CURRENT PROCESS
                    currentTime += currentProcessTime;
                    logFile << "Process " << currentProcess.ID << " finishes execution at time " << currentTime << std::endl;

                    currentTime += contextSwitch;

                    // UPDAITING THE STATISTIC VARIABLES
                    num_of_processes_succesfully_completed++;
                    for(unsigned i = 0; i < record.size(); i++)
                    {
                        if(record[i].ID == currentProcess.ID)
                        {
                            record[i].waitingTime.push_back(currentProcess.waitingTime);
                            break; 
                        }
                    }
                }
            }
        }

        // IF PROCESSOR IS NOT FREE
        else
        {
            // SAVING THE STATE TO MANAGE IF A PREEMPTION OCCURS
            currentProcess.remaining_time -= *it - currentTime;

            currentTime = *it;

            currentProcess.stop_time = currentTime;
            waitingQueue.push_back(currentProcess);

            std::string currentProcess_ID = currentProcess.ID;
            bool Continue = false;

            for(int itr = 0; itr < num_of_processes; itr++)
            {
                // CHECKING WHETHER A PROCESS SHOULD BE SENT INTO WAITING QUEUE
                if(*it % processes[itr].period == 0 && processes[itr].repetitions > 0)
                {
                    // CHECKING FOR THE DEADLINE OF ANY PROCESSES TO TERMINATE
                    for(unsigned i = 0; i < waitingQueue.size(); i++)
                    {
                        if(processes[itr].ID == waitingQueue[i].ID || waitingQueue[i].next_deadline <= currentTime)
                        {
                            logFile << "Process " << waitingQueue[i].ID << " missed deadline at time " << currentTime << std::endl;
                            logFile << "Terminating Process " << waitingQueue[i].ID << std::endl;

                            waitingQueue[i].waitingTime += currentTime - waitingQueue[i].stop_time;

                            // TAKING THE NOTE OF THE WAITING TIME OF THE TERMINATING PROCESS
                            for(unsigned j = 0; j < record.size(); j++)
                            {
                                if(record[j].ID == waitingQueue[i].ID)
                                {
                                    record[j].waitingTime.push_back(waitingQueue[i].waitingTime);
                                    break; 
                                }
                            }

                            // TERMINATING A PROCESS BY REMOVING IT FROMT THE WAITING QUEUE
                            waitingQueue.erase(waitingQueue.begin() + i);
                            num_of_processes_missed++;

                            // VARIABLE TO KEEP A NOTE OF TERMINATION
                            if(currentProcess.ID == processes[itr].ID)
                            {
                                error = true;
                            }
                            break;
                        }
                    }

                    // PUSHING THE CURRENT ENTERING PROCESS INTO WAITING QUEUE
                    processes[itr].stop_time = currentTime;
                    waitingQueue.push_back(processes[itr]);

                    // UPDATING THE DATA OF THE PROCESSES
                    processes[itr].next_deadline += processes[itr].period;
                    processes[itr].repetitions--;

                    // UPDATING THE STATISTIC VARIABLE
                    num_of_processes_came_into_system++;
                }
            }
            // MAINTANING THE PROPORITY ORDER
            std::sort(waitingQueue.begin(), waitingQueue.end(), Priority());

            // CHECKING FOR ANY PREEMPTION
            if(!error && waitingQueue[0].ID != currentProcess.ID)
            {
                logFile << "Process " << currentProcess.ID;
                logFile << " is Preempted by Process " << waitingQueue[0].ID;
                logFile << " at time " << currentTime << ". ";
                logFile << "Remaining processing time: " << currentProcess.remaining_time << std::endl;
            }
            // IF NO PREEMPTION OR TERMINATION 
            else if(!error && waitingQueue[0].ID == currentProcess.ID) Continue = true;

            while(!waitingQueue.empty())
            {
                // SELECTING FROM THE WAITING QUEUE
                auto start = std::chrono::high_resolution_clock::now();
                currentProcess = waitingQueue[0];
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration<float, std::chrono::milliseconds::period>(stop - start);

                if(!Continue)
                {
                    currentTime += contextSwitch;

                    // ADDING THE SELECTION TIME
                    currentTime += duration.count();

                    if(currentProcess.remaining_time == currentProcess.Process_time)
                        logFile << "Process " << currentProcess.ID << " starts execution at time " << currentTime << std::endl;
                    else
                        logFile << "Process " << currentProcess.ID << " resumes execution at time " << currentTime << std::endl;
                }
                Continue = false;
                error = false;

                // UPDATING THE WAITING TIME
                currentProcess.waitingTime += currentTime - currentProcess.stop_time;

                // REMOVING THE RUNNING PROCESS FROM THE WAITING QUEUE
                waitingQueue.erase(waitingQueue.begin());
                currentProcessTime = currentProcess.remaining_time;

                // UPDATING THE STATUS OF PROCESSOR
                // IF CURRENT PROCESS REMAINING TIME MORE THAN NEXT CHECK POINT
                if(currentProcessTime + currentTime > *(++it))
                {
                    it--;
                    free = false;
                    break;
                }
                // IF LESS THAN THE NEXT CHECK POINT
                else
                {
                    it--;
                    free = true;
                    // FREEING THE PROCESSOR AND FINISHING THE CURRENT PROCESS
                    currentTime += currentProcessTime;
                    logFile << "Process " << currentProcess.ID << " finishes execution at time " << currentTime << std::endl;

                    // UPDAITING THE STATISTIC VARIABLES
                    num_of_processes_succesfully_completed++;
                    for(unsigned i = 0; i < record.size(); i++)
                    {
                        if(record[i].ID == currentProcess.ID)
                        {
                            record[i].waitingTime.push_back(currentProcess.waitingTime);
                            break; 
                        }
                    }
                }
            }
        }
    }

    // CREATING THE STATISTICS FILE
    statFile.open(stat, std::ios::out);
    if(!statFile)
    {
        std::cout << "Error in creating Statistics file" << std::endl;
        exit(1);
    }

    // PRINTING THE STATISTICS TO THE STATISTICS FILE
    statFile << "Number of processes that came into the system: " << num_of_processes_came_into_system << std::endl;
    statFile << "Number of Processes that successfully completed: " << num_of_processes_succesfully_completed << std::endl;
    statFile << "Number of Processes that missed their deadlines: " << num_of_processes_missed << std::endl;
    
    float total = 0;
    for(unsigned i = 0; i < record.size(); i++)
    {
        float sum = 0;
        for(auto itr : record[i].waitingTime)
        {
            sum += itr;
        }

        total += sum;

        statFile << "Process " << record[i].ID << ": Average waiting time: " << sum / (float)record[i].waitingTime.size() << std::endl;
    }
    statFile << "Overall Average waiting time: " << total / num_of_processes_came_into_system << std::endl;

    // CLOSING THE STATISTICS FILE
    statFile.close();
    // CLOSING THE LOG FILE
    logFile.close();

    return 0;
}