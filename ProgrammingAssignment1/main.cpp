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

#include "Scheduler.h"


using namespace std;


int main(int argc, char** argv) {
 
    Scheduler mySchedule(argv[1], 50, 10);
    
    mySchedule.Execute();
    
    return 0;
    
}

