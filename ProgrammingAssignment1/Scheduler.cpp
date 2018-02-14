/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 * 
 * Author: Cedric Smith, Jordan Mohler, Lexie Hermosura
 */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <queue>
#include "Scheduler.h"

using namespace std;

Scheduler::Scheduler(string name, int bd, int ts) {
    file.open(name);
    block_duration = bd;
    time_slice = ts;
    //std::cout << block_duration << " " << time_slice << std::endl; 

}

Scheduler::~Scheduler() {
    file.close();
}

void Scheduler::Execute() {


    if (file.is_open()) {
        string s;

        char name;
        uint32_t arrival;
        uint32_t total;
        uint32_t block;

        std::map<uint32_t, std::vector<uint32_t>> processes;

        while (std::getline(file, s)) {
            istringstream iss(s);
            std::vector<uint32_t> single_process;
            iss >> name >> arrival >> total >> block;

            single_process.push_back(arrival);
            single_process.push_back(total);
            single_process.push_back(block);
            single_process.push_back(0);
            single_process.push_back(0);
            single_process.push_back(0);
            
            waiting.push_back(name);
            processes.insert(std::map<uint32_t, std::vector<uint32_t>>::value_type(name, single_process));
        }

        
       //RoundRobin(processes);
       SPN(processes);
        
        //RoundRobin(process_name, arrival_time, total_time, block_interval);
        
        
        //single_processSPN(process_name, arrival_time, total_time, block_interval);
    }
}
/* 
 * Structure of processes map:
 *      Key: Name as uint32_t
 *      0:  Arrival time
 *      1: Total time to execute
 *      2: Block interval
 *      3: Time run
 *      4: Time to unblock
 *      5: Termination time
 */
void Scheduler::RoundRobin(std::map<uint32_t, std::vector<uint32_t>> processes) {
    int simulation_time = 0;
    bool finished = false;

    while (!finished) {
         //Add arriving processes
        for (int k = 0; k < waiting.size(); k++) {
            if (processes.at(waiting[k])[0] <= simulation_time) {
                running.push_back(waiting[k]);
                waiting.erase(waiting.cbegin() + k);
                k--;
            }
        }
        
        //unblock
        for (int m = 0; m < blocked.size(); m++) {
            if (processes.at(blocked[m])[4] != 0 && processes.at(blocked[m])[4] <= simulation_time) {
                running.push_back(blocked[m]);
                blocked.erase(blocked.cbegin() + m);  
                m--;
            }
        }

        //move ready to running
        while (ready.size() != 0) {
                running.push_back(ready[0]);
                ready.erase(ready.cbegin());
        }
        
        //if no processes are ready
        if (running.size() == 0 && blocked.size() != 0) {
            int runtime = processes.at(blocked[0])[4] - simulation_time;
            cout << simulation_time << "\t<idle>\t" << runtime << "\tI\n";
            simulation_time +=runtime;
        }
        
        //run and move to ready or block
        while (running.size() != 0) {
            int interval_remain = processes.at(running[0])[2] - processes.at(running[0])[3];
            int runtime = std::min(time_slice, interval_remain);
            int total_remain = processes.at(running[0])[1];
            runtime = std::min(runtime, total_remain);
            
            AdvanceProcess(runtime, simulation_time, processes);
        }
        
        //complete when all processes are in terminated
        finished = (terminated.size() == processes.size());
   }

    //compute average turnaround
    double avg_turnaround = 0;
    for (int i = 0; i < processes.size(); i++) {
        avg_turnaround += (processes.at(terminated[i])[5] - processes.at(terminated[i])[0]);
    }
    avg_turnaround = avg_turnaround / processes.size();
    cout << simulation_time << "\t<done>\t" << avg_turnaround;
}

void Scheduler::AdvanceProcess(int runtime, int &simulation_time, std::map<uint32_t, vector<uint32_t>> &processes) {
    int i = 0;
    //Increment time run and decrement total time remaining
    processes.at(running[i])[3] += runtime;
    processes.at(running[i])[1] -= runtime;

    //If total time = 0, terminate
    if (processes.at(running[i])[1] == 0) {
        //set termination time
        processes.at(running[i])[5] = simulation_time + runtime;
        
        //Add key to terminated
        terminated.push_back(running[i]);      
        
        //Print results
        cout << simulation_time;
        printf("\t%c\t", running[i]);
        cout << runtime << "\tT\n";
        
        //Remove key from running
        running.erase(running.cbegin());
                
        //If time run equals block interval, then block    
    } else if (processes.at(running[i])[3] == processes.at(running[i])[2]) {
        // Reset time_run
        processes.at(running[i])[3] = 0;

        //Set time unblock
        processes.at(running[i])[4] = simulation_time + block_duration + runtime;

        //Print results
        cout << simulation_time;
        printf("\t%c\t", running[i]);
        cout << runtime << "\tB\n";

        //Remove process from running and place on blocked processes
        blocked.push_back(running[i]);
        running.erase(running.cbegin());
        
    } else {
        //Print results
        cout << simulation_time;
        printf("\t%c\t", running[i]);
        cout << runtime << "\tS\n";
        
        //Remove process from running and place on ready
        ready.push_back(running[i]);
        running.erase(running.cbegin());
    }

    //Increment simulation time
    simulation_time += runtime;
}

void Scheduler::SPN(std::map<uint32_t, std::vector<uint32_t>> processes) 
{
    int simulation_time = 0;
    bool finished = false;

    while (!finished) {
         //Add arriving processes
        for (int k = 0; k < waiting.size(); k++) {
            if (processes.at(waiting[k])[0] <= simulation_time) {
                running.push_back(waiting[k]);
                waiting.erase(waiting.cbegin() + k);
                k--;
            }
        }
        
        //unblock
        for (int m = 0; m < blocked.size(); m++) {
            if (processes.at(blocked[m])[4] != 0 && processes.at(blocked[m])[4] <= simulation_time) {
                running.push_back(blocked[m]);
                blocked.erase(blocked.cbegin() + m);  
                m--;
            }
        }

        //move ready to running
        while (ready.size() != 0) {
            running.push_back(ready[0]);
            ready.erase(ready.cbegin());
        }
        
        //if no processes are ready
        if (running.size() == 0 && blocked.size() != 0) {
            int runtime = processes.at(blocked[0])[4] - simulation_time;
            cout << simulation_time << "\t<idle>\t" << runtime << "\tI\n";
            simulation_time +=runtime;
        }
        
        //run and move to ready or block
        while (running.size() != 0) {
            SortToShortest(running, processes);
            int interval_remain = processes.at(running[0])[2] - processes.at(running[0])[3];
            int total_remain = processes.at(running[0])[1];
            int runtime = std::min(interval_remain, total_remain);
            AdvanceProcess(runtime, simulation_time, processes);
        }
        
        //complete when all processes are in terminated
        finished = (terminated.size() == processes.size());
   }

    //compute average turnaround
    double avg_turnaround = 0;
    for (int i = 0; i < processes.size(); i++) {
        avg_turnaround += (processes.at(terminated[i])[5] - processes.at(terminated[i])[0]);
    }
    avg_turnaround = avg_turnaround / processes.size();
    cout << simulation_time << "\t<done>\t" << avg_turnaround;
}

void Scheduler::SortToShortest(std::vector<uint32_t> &running, std::map<uint32_t, std::vector<uint32_t>> &processes) {
    //sort running vector by process length (modified insertion sort)
    for (int n = 1; n < running.size(); n++) {
        int m = n;
        while (m > 0 && processes.at(running[n-1])[2] > processes.at(running[n])[2]) {
            uint32_t less = running[m];
            running[m] = running[m-1];
            running[m-1] = less;
            m--;
        }
    }
}
