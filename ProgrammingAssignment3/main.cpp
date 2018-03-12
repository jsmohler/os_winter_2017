/*
 * COMP3361 Winter 2018
 * Programming Assignment 2 Sample Solution
 */

/* 
 * File:   main.cpp
 * Author: Mike Goss <mikegoss@cs.du.edu>
 *
 * Created on October 28, 2017, 10:08 PM
 */

#include "PageFrameAllocator.h"
#include "ProcessTrace.h"

#include <MMU.h>

#include <cstdlib>
#include <iostream>

/*
 * 
 */
int main(int argc, char* argv[]) {
  mem::MMU memory(1024);
  PageFrameAllocator allocator(memory);
  ProcessTrace trace(memory, allocator, argv[8]);
  trace.Execute(1);
  
//  int num_processes = argc - 3;
//  int time_slice = atoi(argv[2]);
//  std::vector<ProcessTrace*> traces;
//  
//  for (int i = 0; i < num_processes; i++) {
//    ProcessTrace* trace = new ProcessTrace(memory, allocator, argv[i+3]);
//    traces.push_back(trace);
//  }
//  
//  std::vector<mem::Addr> allocated;
//  
//  for (int i = 0; i < num_processes; i++) {
//      for (int m = 0; m < time_slice; m++) {
//          traces[i]->Execute(i+1, allocated);
//      }
//  }
  
  return 0;
}
