/* 
 * File:   main.cpp
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 *
 * Created on January 19, 2018, 4:09 PM
 */

#include <cstdlib>
#include "ProcessTrace.h"

using namespace std;

/*
 * The main program takes in a single command line argument which is the name of the trace file.
 * It creates an instance of the ProcessTrace class using this trace file, 
 * and then calls the Execute method of the ProcessTrace instance.
 */
int main(int argc, char** argv) {
    ProcessTrace trace(argv[2]);
    trace.Execute();
    
}

