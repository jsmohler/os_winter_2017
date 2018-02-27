/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 *
 * Created on February 20, 2018, 10:29 AM
 */

#include <cstdlib>
#include <MMU.h>
#include "ProcessTrace.h"
#include "PageFrameAllocator.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    mem::MMU memory(256);
    PageFrameAllocator alloc(memory);
    ProcessTrace process(argv[1], memory, alloc);
    process.Execute();
}

