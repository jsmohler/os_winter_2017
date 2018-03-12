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
#include <string>

using namespace std;

/*
 * 
 */

void Multiprogram(int num_processes, int time_slice, std::vector<ProcessTrace*> traces, std::vector<int> num_process, mem::MMU &memory, PageFrameAllocator& allocator, char* files[]) {
  for (int i = 0; i < num_processes; i++) {
    ProcessTrace* trace = new ProcessTrace(memory, allocator, files[i+3]);
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
      }
      for (int i = 0; i < traces.size(); i++) {
        if (! traces[i]->get_executing()) {
            traces.erase(traces.begin() + i);
            num_process.erase(num_process.begin() + i);
        } 
        executing |= traces[i]->get_executing();
      }
  }
}

int main(int argc, char* argv[]) {
  mem::MMU memory(1024);
  PageFrameAllocator allocator(memory);
  string first = argv[1];
  
  if (first.compare("program3") == 0) {
    int num_processes = argc-3;
    int time_slice = atoi(argv[2]);
    std::vector<ProcessTrace*> traces;
    std::vector<int> num_process;
    Multiprogram(num_processes, time_slice, traces, num_process, memory, allocator, argv);
  } else {
    ProcessTrace process(memory, allocator, argv[1]);
    while (process.get_executing()) {
        process.Execute(1);
    }
  }
  
  return 0;
}