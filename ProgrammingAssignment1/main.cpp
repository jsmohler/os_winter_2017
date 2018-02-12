/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Cedric Smith, Jordan Mohler, Lexie Hermosura
 *
 * Created on February 11, 2018, 3:49 PM
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <cstdint>


using namespace std;


int main(int argc, char** argv) {
    
    

       //Reads a single hex integer specifying the number of page frames from the first line of the input file.
    string name = argv[1];
    ifstream file;
    file.open(name);
    string s;
    string process_name;
    int arrival_time;
    int total_time;
    int block_time;
    
    int block_duration;
    int time_slice;
    
    
    string scheduling_algorithm = "RR";

    if(scheduling_algorithm == "RR")
    {
        block_duration = 20;
        time_slice = 10;
        std::cout << scheduling_algorithm << " " << block_duration << " " << time_slice << std::endl;
    }else{
        
        block_duration = 20;
        std::cout << scheduling_algorithm << " " << block_duration << std::endl;
    }
    

    while (std::getline(file, s)) {
        istringstream iss(s);

        iss >> process_name >> arrival_time >> total_time >> block_time;
        //Reading each input file line (including the first line), then writing the line to output, preceded by the '>" character.
       std::cout << process_name << " " << arrival_time << " " << total_time << " " << block_time << std::endl;
 
    }
}

