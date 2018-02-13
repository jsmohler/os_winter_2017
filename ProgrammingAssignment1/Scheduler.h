/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Program1.h
 * Author: Cedric Smith, Jordan Mohler, Lexie Hermosura
 *
 * Created on February 11, 2018, 5:43 PM
 */

#ifndef PROGRAM1_H
#define PROGRAM1_H

#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

class Scheduler {
    
public:
   
    Scheduler(string name, int block_duration, int time_slice);
    
    virtual ~Scheduler();
    
    void RoundRobin(std::vector<string, std::vector<uint32_t>, std::vector<uint32_t>, std::vector<uint32_t>);
    
    void SPN();
    
    void Execute();
    
    
    int get_block_duration() const {
        return block_duration;
    }
    
    int get_time_slice() const{
        return time_slice;
    }
    
    //Rule of 3: prevent use of copy, assign
    Scheduler(const Scheduler&) = delete;
    Scheduler &operator=(const Scheduler&) = delete;
    
    //Rule of 5: prevent use of move and move assign
    Scheduler(const Scheduler&&) = delete;
    Scheduler &operator=(const Scheduler&&) = delete;
    
private:
ifstream file;   
int block_duration;
int time_slice;
    
    
};


#endif /* PROGRAM1_H */

