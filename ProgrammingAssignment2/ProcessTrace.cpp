/*
 * ProcessTrace implementation 
 */

/* 
 * File:   ProcessTrace.cpp
 * Author: Mike Goss <mikegoss@cs.du.edu>
 * 
 */

#include "ProcessTrace.h"
#include "PageFrameAllocator.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <iostream>
#include <string>

using mem::Addr;
using mem::MMU;
using std::cin;
using std::cout;
using std::cerr;
using std::getline;
using std::istringstream;
using std::vector;

using namespace std;

ProcessTrace::ProcessTrace(string file_name_, mem::MMU &input, PageFrameAllocator& pfa) 
: file_name(file_name_), line_number(0), page_frames_allocated(0), allocator(pfa), memory(input) {
    

   //get virtual address, free_list_head * mem::kPageSize
    Addr vAddress = pfa.get_free_list_head() * mem::kPageSize;
   //Allocate one page for page frame directory
    allocator.pop(vAddress, 1, memory);
   //new_PMCB with true for virtual mode on
    mem::PMCB new_pmcb(true, vAddress);
   //set memory PMCB to new_PMCB
    memory.set_PMCB(new_pmcb);
    
  // Build directory page table
  // Open the trace file.  Abort program if can't open.
  trace.open(file_name, std::ios_base::in);
  if (!trace.is_open()) {
    cerr << "ERROR: failed to open trace file: " << file_name << "\n";
    exit(2);
  }
}

ProcessTrace::~ProcessTrace() {
  trace.close();
}

void ProcessTrace::Execute(void) {
    // Read and process commands
    string line;                // text line read
    string cmd;                 // command from line
    vector<uint32_t> cmdArgs;   // arguments from line
    
    try {
      // Select the command to execute
      while (ParseCommand(line, cmd, cmdArgs)) {
        if (cmd != "#") {
          mem::PMCB virtual_mode(true, cmdArgs.at(0));
          memory.set_PMCB(virtual_mode); 
        }
         
        
        if (cmd == "alloc" ) {
          CmdAlloc(line, cmd, cmdArgs);    // allocate memory
        } else if (cmd == "compare") {
          CmdCompare(line, cmd, cmdArgs);  // get and compare multiple bytes
        } else if (cmd == "put") {
          CmdPut(line, cmd, cmdArgs);      // put bytes
        } else if (cmd == "fill") {
          CmdFill(line, cmd, cmdArgs);     // fill bytes with value
        } else if (cmd == "copy") {
          CmdCopy(line, cmd, cmdArgs);     // copy bytes to dest from source
        } else if (cmd == "dump") {
          CmdDump(line, cmd, cmdArgs);     // dump byte values to output
        } else if (cmd == "#") {
          CmdComment(line, cmd, cmdArgs);
        } else {
          cerr << "ERROR: invalid command at line " << line_number << ":\n" 
                  << line << "\n";
          exit(2);
        }
      }
    } catch (mem::PageFaultException p) {
        cout << "PageFaultException at " << cmdArgs.at(0) << " " << p.what(); 
    } catch (mem::WritePermissionFaultException w) {
        cout << "WritePermissionFaultException at " << cmdArgs.at(0) << " " << w.what(); 
    }
}

bool ProcessTrace::ParseCommand(
    string &line, string &cmd, vector<uint32_t> &cmdArgs) {
  cmdArgs.clear();
  line.clear();
  
  // Read next line
  if (std::getline(trace, line)) {
    ++line_number;
    cout << std::dec << line_number << ":" << line << "\n";
    
    // Make a string stream from command line
    istringstream lineStream(line);
    
    // Get command
    lineStream >> cmd;
    
    // Get arguments
    uint32_t arg;
    while (lineStream >> std::hex >> arg) {
      cmdArgs.push_back(arg);
    }
    return true;
  } else if (trace.eof()) {
      return false;
  } else {
    cerr << "ERROR: getline failed on trace file: " << file_name 
            << "at line " << line_number << "\n";
    exit(2);
  }
}

void ProcessTrace::CmdAlloc(const string &line, 
                            const string &cmd, 
                            const vector<uint32_t> &cmdArgs) {
  // Allocate the specified memory size
  Addr page_count = (cmdArgs.at(1) + mem::kPageSize - 1) / mem::kPageSize;
  Addr start = cmdArgs.at(0);
  //allocator.Allocate(page_count, page_frames_allocated, start, memory);
}

void ProcessTrace::CmdCompare(const string &line,
                              const string &cmd,
                              const vector<uint32_t> &cmdArgs) {
  uint32_t addr = cmdArgs.at(0);

  // Compare specified byte values
  size_t num_bytes = cmdArgs.size() - 1;
  uint8_t buffer[num_bytes];
  memory.get_bytes(buffer, addr, num_bytes);
  for (int i = 1; i < cmdArgs.size(); ++i) {
    if(buffer[i-1] != cmdArgs.at(i)) {
      cout << "compare error at address " << std::hex << addr
              << ", expected " << static_cast<uint32_t>(cmdArgs.at(i))
              << ", actual is " << static_cast<uint32_t>(buffer[i-1]) << "\n";
    }
    ++addr;
  }
}

void ProcessTrace::CmdPut(const string &line,
                          const string &cmd,
                          const vector<uint32_t> &cmdArgs) {
  // Put multiple bytes starting at specified address
  uint32_t addr = cmdArgs.at(0);
  size_t num_bytes = cmdArgs.size() - 1;
  uint8_t buffer[num_bytes];
  for (int i = 1; i < cmdArgs.size(); ++i) {
     buffer[i-1] = cmdArgs.at(i);
  }
  memory.put_bytes(addr, num_bytes, buffer);
}

void ProcessTrace::CmdCopy(const string &line,
                           const string &cmd,
                           const vector<uint32_t> &cmdArgs) {
  // Copy specified number of bytes to destination from source
  Addr dst = cmdArgs.at(0);
  Addr src = cmdArgs.at(1);
  Addr num_bytes = cmdArgs.at(2);
  uint8_t buffer[num_bytes];
  memory.get_bytes(buffer, src, num_bytes);
  memory.put_bytes(dst, num_bytes, buffer);
}

void ProcessTrace::CmdFill(const string &line,
                          const string &cmd,
                          const vector<uint32_t> &cmdArgs) {
  // Fill a sequence of bytes with the specified value
  Addr addr = cmdArgs.at(0);
  Addr num_bytes = cmdArgs.at(1);
  uint8_t val = cmdArgs.at(2);
  for (int i = 0; i < num_bytes; ++i) {
    memory.put_byte(addr++, &val);
  }
}

void ProcessTrace::CmdDump(const string &line,
                          const string &cmd,
                          const vector<uint32_t> &cmdArgs) {
  uint32_t addr = cmdArgs.at(0);
  uint32_t count = cmdArgs.at(1);

  // Output the address
  cout << std::hex << addr;

  // Output the specified number of bytes starting at the address
  for(int i = 0; i < count; ++i) {
    if((i % 16) == 0) {  // line break every 16 bytes
      cout << "\n";
    }
    uint8_t byte_val;
    memory.get_byte(&byte_val, addr++);
    cout << " " << std::setfill('0') << std::setw(2)
            << static_cast<uint32_t> (byte_val);
  }
  cout << "\n";
}

void ProcessTrace::CmdComment(const string &line,
                          const string &cmd,
                          const vector<uint32_t> &cmdArgs) {
    
}