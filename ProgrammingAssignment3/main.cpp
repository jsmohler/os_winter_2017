/*
 * COMP3361 Winter 2018
 * Programming Assignment 3
 */

/* 
 * File:   main.cpp
 * Author: Jordan Mohler, Lexie Hermosura, Cedric Smith
 *
 */

#include "PageFrameAllocator.h"
#include "ProcessTrace.h"

#include <MMU.h>

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

/*
 * Multiprogram- manages the execution of multiple trace files with 
 * simplified Round-Robin algorithm.
 * @params num_processes: track number of trace files
 * @param memory: MMU to use for memory
 * @param allocator: PageFrameAllocator to allocate pages in trace
 * @param fileinfo: command line arguments containing filenames and time_slice
 */
void Multiprogram(int num_processes, mem::MMU &memory, PageFrameAllocator& allocator, char* fileinfo[]) {
  int time_slice = atoi(fileinfo[2]);
  std::vector<ProcessTrace*> traces;
  std::vector<int> num_process;
    
  for (int i = 0; i < num_processes; i++) {
    ProcessTrace* trace = new ProcessTrace(memory, allocator, fileinfo[i+3]);
    traces.push_back(trace);
    num_process.push_back(i+1);
  }
  
  bool executing = true;
  
  //Round-Robin algorithm
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
  
  if (argc > 2) {
    int num_processes = argc-3;
    Multiprogram(num_processes, memory, allocator, argv);
  } else if (argc == 2) {
    ProcessTrace* process = new ProcessTrace(memory, allocator, argv[1]);
    while (process->get_executing()) {
        process->Execute(1);
    }
  } else {
    std::cerr << "usage: Project3 trace_file\n";
    exit(1);
  }
  
  return 0;
}