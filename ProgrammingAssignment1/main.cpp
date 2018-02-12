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
    uint16_t bd;
    uint16_t ts;
    
    cout << "Block Duration and Time slice: " << std::endl;
    std::cin >> bd >> " ";
    std::cin >> ts >> std::endl;
        
    std::getline(file, s);
    istringstream sstream(s);
    string process_name;
    int process_arrival_time;
    int process_total_time;
    int process_block_time;
    
    std::vector<int> list_of_processes;

    while (std::getline(file, s)) {
        istringstream iss(s);
        int process;
        iss >> std::dec >> process;
        //Reading each input file line (including the first line), then writing the line to output, preceded by the '>" character.
        std::cout << process;

 
    }
}

