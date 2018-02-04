/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ProcessTrace.h
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 *
 * Created on February 3, 2018
 */

#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <MMU.h>

using namespace std;

#ifndef PROCESSTRACE_H
#define PROCESSTRACE_H

class ProcessTrace {
public:
    /*
     *Constructor
     */
    ProcessTrace(string name);
    
    /*
     *Destructor
     */
    virtual ~ProcessTrace();
    
    /*
     * Execute - method allocates an empty array as an std::vector<uint8_t> type. 
     * It then executes the commands in the trace file, 
     * using the vector as the memory referenced by the commands.
     * 
     * @param There are no parameters for this method.
     * 
     * @return the output in hexadecimal format for numeric data from the trace records
     */
    void Execute();
    
    //Rule of 3: prevent use of copy, assign
    ProcessTrace(const ProcessTrace&) = delete;
    ProcessTrace &operator=(const ProcessTrace&) = delete;
    
    //Rule of 5: prevent use of move and move assign
    ProcessTrace(const ProcessTrace&&) = delete;
    ProcessTrace &operator=(const ProcessTrace&&) = delete;
private:
    /*
     * Created a private variable in the ProcessTrace header file 
     * so that we would have some way to access the stream and close it.
     */
    ifstream file;
};

#endif /* PROCESSTRACE_H */

