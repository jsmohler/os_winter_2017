/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ProcessTrace.h
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 *
 * Created on January 19, 2018, 4:14 PM
 */

#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

#ifndef PROCESSTRACE_H
#define PROCESSTRACE_H

class ProcessTrace {
public:
    ProcessTrace(string name);
    virtual ~ProcessTrace();
    void Execute();
    
    //Rule of 3: prevent use of copy, assign
    ProcessTrace(const ProcessTrace&) = delete;
    ProcessTrace &operator=(const ProcessTrace&) = delete;
    
    //Rule of 5: prevent use of move and move assign
    ProcessTrace(const ProcessTrace&&) = delete;
    ProcessTrace &operator=(const ProcessTrace&&) = delete;
private:
    ifstream file;
};

#endif /* PROCESSTRACE_H */

