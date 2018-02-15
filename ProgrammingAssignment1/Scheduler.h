/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Scheduler.h
 * Author: Cedric Smith, Jordan Mohler, Lexie Hermosura
 *
 * Created on February 11, 2018, 5:43 PM
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>
#include <cstdlib>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <functional>

using namespace std;

class Scheduler {
    
public:
   
    //Constructor
    Scheduler(string name, string block_duration, string time_slice);
    
    //Destructor
    virtual ~Scheduler();
    
    //Rule of 3: prevent use of copy, assign
    Scheduler(const Scheduler&) = delete;
    Scheduler &operator=(const Scheduler&) = delete;
    
    //Rule of 5: prevent use of move and move assign
    Scheduler(const Scheduler&&) = delete;
    Scheduler &operator=(const Scheduler&&) = delete;
    
    /*
     * AdvancedProcess - Updates simulation time, moves processes into ready, blocked, or terminating and prints information for each process
     * 
     * @param int                                       An integer value for the runtime                                       
     * @param int&                                      An integer reference to the simulation time
     * @param std::map<uint32_t, vector<uint32_t>>&     A process map reference with a uint32_t variable as a key
     * 
     * @return                                          This method does not return anything. It simply updates values for the processes
     */
    void AdvanceProcess(int, int&, std::map<uint32_t, vector<uint32_t>>&);
    
    /*
     * SPN - Simulation of the Shortest Process Next scheduling algorithm
     * 
     * @param std::map<uint32_t, std::vector<uint32_t>> returns a process map with the name as a uint32_t key. 
     *                                                  The values are vectors of uint32_t's which are arrival time, 
     *                                                  total time to execute, block interval, time run, time to block 
     *                                                  and termination time.
     * @return                                          A single line with the name SPN, followed by block_duration and values. 
     *                                                  There is one line for each interval during which a process is running or the system is idle.
     *                                                  Simulation time is output for each line along with process name, length of interval, and status code.
     *                                                  After all jobs have terminated, simulation time at which last job was terminated is output, the string <done>,
     *                                                  and the average turnaround time of all of the processes.
     */
    void SPN(std::map<uint32_t, std::vector<uint32_t>>); 
    
    /*
     * RoundRobin - Simulation of the Round Robin scheduling algorithm
     * 
     * @param std::map<uint32_t, std::vector<uint32_t>> a process map with the name as a uint32_t key. 
     *                                                  The values are vectors of uint32_t's which are arrival time, 
     *                                                  total time to execute, block interval, time run, time to block 
     *                                                  and termination time.
     * @return                                          A single line with the name SPN, followed by block_duration, time slice and values. 
     *                                                  There is one line for each interval during which a process is running or the system is idle.
     *                                                  Simulation time is output for each line along with process name, length of interval, and status code.
     *                                                  After all jobs have terminated, simulation time at which last job was terminated, the string <done>,
     *                                                  and the average turnaround time of all of the processes is output.
     */
    void RoundRobin(std::map<uint32_t, std::vector<uint32_t>>);
    
    /*
     * SortToShortest - A modified version of insertion sort for sorting the processes by length
     * 
     * @param std::map<uint32_t, std::vector<uint32_t>> A reference to a process map with the name as a uint32_t key. 
     *                                                  The values are vectors of uint32_t's which are arrival time, 
     *                                                  total time to execute, block interval, time run, time to block and termination time.
     * @return                                          Outputs a sorted list of processes by length
     */
    void SortToShortest(std::map<uint32_t, std::vector<uint32_t>>&);
    
    /*
     * Clear - Clears all private vectors.
     * 
     * @param  There are no parameters for this method
     * @return Cleared vectors for blocked, running, ready, terminated and waiting
     */
    void Clear();
    
    /*
     * Execute - Reads in input from the input file and also executes both the SPN and RoundRobin algorithms
     * 
     * @param  A string which is the name of the algorithm to execute.
     * @return Returns the outputs from both the RoundRobin and SPN scheduling algorithms
     */
    void Execute(string);
    
    //getters
    int get_block_duration() const {
        return block_duration;
    }
   
    int get_time_slice() const{
        return time_slice;
    }
    
private:
    
    ifstream file;   
    int block_duration;
    int time_slice;
    
    //private vectors for keeping track of processes for implementing scheduling algorithms
    std::vector<uint32_t> blocked;
    std::vector<uint32_t> running;
    std::vector<uint32_t> ready;
    std::vector<uint32_t> terminated;
    std::vector<uint32_t> waiting;
    
    
};


#endif /* SCHEDULER_H */

