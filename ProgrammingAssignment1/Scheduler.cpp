/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 * 
 * Author: Cedric Smith, Jordan Mohler, Lexie Hermosura
 */

#include "Scheduler.h"
#include <iostream>
#include <vector>

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
        //SJN(process_name, arrival_time, total_time, block_interval);
    }
}

void Scheduler::RoundRobin(std::vector<string> process_name, std::vector<uint32_t> arrival_time, std::vector<uint32_t> total_time, std::vector<uint32_t> block_interval) {
    int simulation_time = 0;
    int num_processes = process_name.size();
    std::vector<int> running_process;
    std::vector<int> order;
    std::vector<int> time_run(num_processes);
    std::vector<int> time_unblocked(num_processes);

    for (int i = 0; i < arrival_time.size(); i++) {
        time_unblocked.push_back(arrival_time[i]);
        running_process.push_back(-1);
    }
    
    int run = 0;

    while (!std::all_of(total_time.begin(), total_time.end(), [](uint32_t j) { return j == 0; })) {
        run++;
        if (run > 35) {
            break;
        }
        
        int index = 0;
        int count = 0;
        for (int i : running_process) {
            cout << "I: " << i << std::endl;
            cout <<"Count: " << count << std::endl;
            if (i == -1) {
                for (int l = 0; l < running_process.size(); l++) {
                    cout << running_process[l] << " ";
                }
                cout << std::endl;
                for (int k = 0; k < time_unblocked.size(); k++) {
                    if (time_unblocked[k] != 0 && time_unblocked[k] >= simulation_time) {
                        running_process[count] = k;
                        time_unblocked[k] = 0;
                    }
                }
                for (int l = 0; l < running_process.size(); l++) {
                    cout << running_process[l] << " ";
                }
                cout << std::endl;
            } else {
                for (int l = 0; l < running_process.size(); l++) {
                    cout << running_process[l] << " ";
                }
                cout << std::endl;
                int runtime = block_interval[i] - time_run[i];

                if (total_time[i] < runtime) {
                    runtime = total_time[i];
                }

                if (runtime > 0 && runtime >= time_slice) {
                    int current_time = simulation_time;
                    total_time[i] -= time_slice;
                    time_run[i] += time_slice;
                    simulation_time += time_slice;

                    //Output Run
                    if (total_time[i] == 0) {
                        cout << current_time << " " << process_name[i] << " " << time_slice << " " << "T\n";
                    } else if (time_run[i] == block_interval[i]) {
                        running_process.erase(running_process.begin() + count);
                        int next_process = running_process[count-1];
                        running_process.erase(running_process.begin());
                        running_process.push_back(next_process);
                        running_process.push_back(0xFFFFFFFF);
                        

                        time_unblocked[count] = current_time + block_duration;
                        time_run[count] = 0;
                        cout << current_time << " " << process_name[i] << " " << time_slice << " " << "B\n";
                        std::cout << std::endl;
                        break;
                    } else {
                        cout << current_time << " " << process_name[i] << " " << time_slice << " " << "S\n";
                    }


                } else if (runtime > 0 && runtime < time_slice) {
                    int current_time = simulation_time;
                    total_time[i] -= runtime;
                    time_run[i] += runtime;
                    simulation_time += runtime;

                    //Output Run
                    if (total_time[i] == 0) {
                        cout << current_time << " " << process_name[i] << " " << runtime << " " << "T\n";
                    } else if (time_run[i] == block_interval[i]) {
                        running_process.erase(running_process.begin() + count);
                        int next_process = running_process[count-1];
                        running_process.erase(running_process.begin());
                        running_process.push_back(next_process);
                        running_process.push_back(0xFFFFFFFF);
                        
                        time_unblocked[count] = current_time + block_duration;
                        time_run[count] = 0;
                        cout << current_time << " " << process_name[i] << " " << runtime << " " << "B\n";
                        cout << std::endl;
                        break;
                    }

                }

            }

//            for (int k = 0; k < time_unblocked.size(); k++) {
//                if (time_unblocked[k] != 0 && time_unblocked[k] >= simulation_time) {
//                    running_process[count] = k;
//                    time_unblocked[k] = 0;
//                }
//            }

            for (int k = 0; k < arrival_time.size(); k++) {
                if (arrival_time[k] <= simulation_time) { 
                    running_process[k] = k;
                    arrival_time[k] = 0xFFFF;
                }
            }
            
//            for (int l = 0; l < running_process.size(); l++) {
//                cout << running_process[l] << " ";
//            }
//            cout << std::endl;
            cout << std::endl;

            count++;
        }
    }
    std::cout << "RR " << get_block_duration() << " " << get_time_slice() << std::endl;
}