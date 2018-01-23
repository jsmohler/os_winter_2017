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
 * 
 */
int main(int argc, char** argv) {
    ProcessTrace trace(argv[2]);
    trace.Execute();
    
}

