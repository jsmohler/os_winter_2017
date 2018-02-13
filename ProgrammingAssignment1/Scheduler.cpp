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

        std::vector<string> process_name;
        std::vector<uint32_t> arrival_time;
        std::vector<uint32_t> total_time;
        std::vector<uint32_t> block_interval;

        string name;
        uint32_t arrival;
        uint32_t total;
        uint32_t block;

        int num_processes = 0;

        while (std::getline(file, s)) {
            istringstream iss(s);

            iss >> name >> arrival >> total >> block;

            process_name.push_back(name);
            arrival_time.push_back(arrival);
            total_time.push_back(total);
            block_interval.push_back(block);
        }

        RoundRobin(process_name, arrival_time, total_time, block_interval);
        SPN(process_name, arrival_time, total_time, block_interval);
    }
}

void Scheduler::RoundRobin(std::vector<string> process_name, std::vector<uint32_t> arrival_time, std::vector<uint32_t> total_time, std::vector<uint32_t> block_interval) {
    int simulation_time = 0;
    int num_processes = process_name.size();
    int running_process[num_processes];
    std::fill_n(running_process, num_processes, -1);
    std::vector<int> time_run(num_processes);
    std::vector<int> time_unblocked(num_processes);

    for (int i = 0; i < arrival_time.size(); i++) {
        time_unblocked.push_back(arrival_time[i]);
    }

    while (!std::all_of(total_time.begin(), total_time.end(), [](uint32_t j) {return j == 0;})) {
        int index = 0;
        for (int i : running_process) {
            if (i == -1) {
                for (int k = 0; k < time_unblocked.size(); k++) {
                    if (time_unblocked[k] != 0 && time_unblocked[k] >= simulation_time) {
                        running_process[k] = k;
                        time_unblocked[k] = 0;
                    }
                }
            } else {
                int runtime = block_interval[i] - time_run[i];

                if (total_time[i] < runtime) {
                    runtime = total_time[i];
                }

                if (runtime > 0 && runtime >= time_slice) {
                    total_time[i] -= time_slice;
                    time_run[i] += time_slice;

                    //Output Run
                    if (total_time[i] == 0) {
                        cout << simulation_time << " " << process_name[i] << " " << time_slice << " " << "T\n";
                    } else if (time_run[i] == block_interval[i]) {
                        running_process[i] = 0xFFFFFFFF;
                        //cout << "Blocked " << process_name[i] << ": " << running_process[i] << std::endl;
                        time_unblocked[i] = simulation_time + block_duration;
                        time_run[i] = 0;
                        cout << simulation_time << " " << process_name[i] << " " << time_slice << " " << "B\n";
                    } else {
                        cout << simulation_time << " " << process_name[i] << " " << time_slice << " " << "S\n";
                    }

                    simulation_time += time_slice;

                } else if (runtime > 0 && runtime < time_slice) {
                    total_time[i] -= runtime;
                    time_run[i] += runtime;

                    //Output Run
                    if (total_time[i] == 0) {
                        cout << simulation_time << " " << process_name[i] << " " << runtime << " " << "T\n";
                    } else if (time_run[i] == block_interval[i]) {
                        running_process[i] = 0xFFFFFFFF;
                        time_unblocked[i] = simulation_time + block_duration;
                        //cout << "Blocked " << process_name[i] << ": " << running_process[i] << std::endl;
                        time_run[i] = 0;
                        cout << simulation_time << " " << process_name[i] << " " << runtime << " " << "B\n";
                    }

                    simulation_time += runtime;
                }
            }

            for (int k = 0; k < arrival_time.size(); k++) {
                if (arrival_time[k] <= simulation_time) {
                    running_process[k] = k;
                }
            }

            std::cout << "RR " << get_block_duration() << " " << get_time_slice() << std::endl;
        }
    }
}

void Scheduler::SPN(std::vector<string> p, std::vector<uint32_t> a, std::vector<uint32_t> t, std::vector<uint32_t> b) 
{
    int simulation_time = 0;
    std::priority_queue<vector> ready_list;
    std::priority_queue<vector> blocked_list;
    string current_process;

    

    std::cout << "SPN " << get_block_duration() << std::endl;
    //std::cout << simulation_time << " " << p.at(current_process) << std::endl;
}