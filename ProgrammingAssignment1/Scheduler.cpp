/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 * 
 * Author: Cedric Smith, Jordan Mohler, Lexie Hermosura
 */

#include "Scheduler.h"


Scheduler::Scheduler(string name, int bd, int ts){
    file.open(name);
    block_duration = bd;
    time_slice = ts;
    //std::cout << block_duration << " " << time_slice << std::endl; 
    
}

Scheduler::~Scheduler()
{
    file.close();
}

void Scheduler::Execute(){
    
    
    if (file.is_open()) {
    string s;
    string process_name;
    int arrival_time;
    int total_time;
    int block_time;
    
 
    while (std::getline(file, s)) {
        istringstream iss(s);

        iss >> process_name >> arrival_time >> total_time >> block_time;
        //Reading each input file line (including the first line), then writing the line to output, preceded by the '>" character.
       std::cout << process_name << " " << arrival_time << " " << total_time << " " << block_time << std::endl;
 
    }
         
    RoundRobin();
    SJN();
        
        
    }
    
}

void Scheduler::RoundRobin(){
    
    std::cout << "RR " << get_block_duration() << " " << get_time_slice() << std::endl;
}

void Scheduler::SJN()
{
    std::cout << "SJN " << get_block_duration() << std::endl;
}