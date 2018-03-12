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
  
  int num_processes = argc-3;
  int time_slice = atoi(argv[2]);
  std::vector<ProcessTrace*> traces;
  std::vector<int> num_process;
  
  for (int i = 0; i < argc-3; i++) {
    ProcessTrace* trace = new ProcessTrace(memory, allocator, argv[i+3]);
    traces.push_back(trace);
    num_process.push_back(i+1);
  }
  
  bool executing = true;
  
  
  while (executing) {
      executing = false;
      for (int i = 0; i < traces.size(); i++) {
        for (int m = 0; m < time_slice; m++) {
            if (traces[i]->get_executing())   {
                traces[i]->Execute(num_process[i]);
            }
        }
        if (! traces[i]->get_executing()) {
            traces.erase(traces.begin() + i);
            num_process.erase(num_process.begin() + i);
        }
        executing |= traces[i]->get_executing();
      }
  }
  
  return 0;
}
