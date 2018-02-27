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
#include "Exceptions.h"
#include <MMU.h>
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

   //Allocate one page for page frame directory
    allocator.Allocate(1, page_frames_allocated, memory);
   //get virtual address, free_list_head * mem::kPageSize
    Addr vAddress = page_frames_allocated.back() * mem::kPageSize;
    directory_base = vAddress;
   //new_PMCB with true for virtual mode on
    mem::PMCB new_pmcb(true, directory_base);
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
    int line_number = 0;
    
  // Select the command to execute
    while (ParseCommand(line, cmd, cmdArgs)) {
        if (cmd != "#") {
          mem::PMCB virtual_mode(true, cmdArgs.at(0));
          memory.set_PMCB(virtual_mode); 
        }

        line_number++;
        
        try {
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
            } else if (cmd == "writable") {
              CmdWritable(line, cmd, cmdArgs);
            }else {
              cerr << "ERROR: invalid command at line " << line_number << ":\n" 
                      << line << "\n";
              exit(2);
            }
        } catch (mem::PageFaultException p) {
            mem::PMCB current;
            memory.get_PMCB(current);
            cout << "Exception type PageFaultException occurred at input line " << std::dec << line_number << " at virtual address 0x" << std::hex << setfill('0') << setw(8) << current.next_vaddress << ": " << p.what() << std::endl; 
            current.operation_state = mem::PMCB::NONE;
            memory.set_PMCB(current);
        } catch (mem::WritePermissionFaultException w) {
            mem::PMCB current;
            memory.get_PMCB(current);
            cout << "Exception type WritePermissionFaultException occurred at input line " << std::dec << line_number << " at virtual address 0x" << std::hex << setfill('0') << setw(8) << current.next_vaddress << ": " << w.what() << std::endl;  
            current.operation_state = mem::PMCB::NONE;
            memory.set_PMCB(current);
        }

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
    
    //Set to physical mode
    mem::PMCB pmcb(false, directory_base);
    memory.set_PMCB(pmcb);
    
    std::vector<uint8_t> found_addr_bytes(4); 
    
    for (int i = 0; i < page_count; i++) {
        // Get entry in top level page table
        Addr l1_offset = (start >> (kPageSizeBits + kPageTableSizeBits)) & kPageTableIndexMask;
        Addr top_level_entry;
        Addr top_level_entry_pa =
                pmcb.page_table_base + l1_offset * sizeof(Addr);
        
        memory.get_bytes(&found_addr_bytes[0], top_level_entry_pa, sizeof(Addr));
        memcpy(&top_level_entry, &found_addr_bytes[0], sizeof(Addr));
       

        //If top level doesn't exist, build one
        if((top_level_entry & kPTE_PresentMask) == 0) {
           top_level_entry = allocator.Allocate(1, page_frames_allocated, memory) | kPTE_PresentMask | kPTE_WritableMask;
           memory.put_bytes(top_level_entry_pa, sizeof(Addr), reinterpret_cast<uint8_t*>(&top_level_entry));
        }

        Addr l2_offset = (start >> kPageSizeBits) & kPageTableIndexMask;
        Addr second_level_entry;
        Addr second_level_address = top_level_entry & kPTE_FrameMask;
        Addr second_level_entry_pa =
                second_level_address + l2_offset * sizeof(Addr);
        memory.get_bytes(&found_addr_bytes[0], second_level_entry_pa, sizeof(Addr)); 
        memcpy(&second_level_entry, &found_addr_bytes[0], sizeof(Addr)); 

        //If second level doesn't exist, build one
        if((second_level_entry & kPTE_PresentMask) == 0) {
           second_level_entry = allocator.Allocate(1, page_frames_allocated, memory) | kPTE_PresentMask | kPTE_WritableMask;
           memory.put_bytes(second_level_entry_pa, sizeof(Addr), reinterpret_cast<uint8_t*>(&second_level_entry));
        }
        
        start+=0x1000;
    }
}

void ProcessTrace::CmdCompare(const string &line,
                              const string &cmd,
                              const vector<uint32_t> &cmdArgs) {
  uint32_t addr = cmdArgs.at(0);

  //Set to virtual mode
  mem::PMCB pmcb(true, directory_base);
  memory.set_PMCB(pmcb);
  
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
  //Set to virtual mode
  mem::PMCB pmcb(true, directory_base);
  memory.set_PMCB(pmcb);
  
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
  //Set to virtual mode
  mem::PMCB pmcb(true, directory_base);
  memory.set_PMCB(pmcb);
  
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
  //Set to virtual mode
  mem::PMCB pmcb(true, directory_base);
  memory.set_PMCB(pmcb);
  
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

  //Set to virtual mode
  mem::PMCB pmcb(true, directory_base);
  memory.set_PMCB(pmcb);
  
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

void ProcessTrace::CmdWritable(const string &line,
                          const string &cmd,
                          const vector<uint32_t> &cmdArgs) {
    Addr page_count = (cmdArgs.at(1) + mem::kPageSize - 1) / mem::kPageSize;
    Addr start = cmdArgs.at(0);
    uint32_t status = cmdArgs.at(2);
    
    //Set to physical mode
    mem::PMCB pmcb(false, directory_base);
    memory.set_PMCB(pmcb);
    
    std::vector<uint8_t> found_addr_bytes(4); 

    for (int i = 0; i < page_count; i++) {
        // Get entry in top level page table
        Addr l1_offset = (start >> (kPageSizeBits + kPageTableSizeBits)) & kPageTableIndexMask;
        Addr top_level_entry;
        Addr top_level_entry_pa =
                pmcb.page_table_base + l1_offset * sizeof(Addr);
        memory.get_bytes(&found_addr_bytes[0], top_level_entry_pa, sizeof(Addr));
        memcpy(&top_level_entry, &found_addr_bytes[0], sizeof(Addr)); 

        //If top level doesn't exist, increment and return
        if((top_level_entry & kPTE_PresentMask) == 0) {
            start += 0x1000;
            return;
        }

        Addr l2_offset = (start >> kPageSizeBits) & kPageTableIndexMask;
        Addr second_level_entry;
        Addr second_level_address = top_level_entry & kPTE_FrameMask;
        Addr second_level_entry_pa =
                second_level_address + l2_offset * sizeof(Addr);
        memory.get_bytes(&found_addr_bytes[0], second_level_entry_pa, sizeof(Addr)); 
        memcpy(&second_level_entry, &found_addr_bytes[0], sizeof(Addr));  

        //If second level does exist, modify all present entries to writable
        if((second_level_entry & kPTE_PresentMask) != 0) {
            //Edit second level to clear writable if status = 0, or set writable if status != 0
            if (status == 0) {
                second_level_entry &= ~(kPTE_WritableMask);
            } else {
                second_level_entry |= kPTE_WritableMask;
            }
            memory.put_bytes(second_level_entry_pa, sizeof(Addr), reinterpret_cast<uint8_t*>(&second_level_entry));
        } 

        start += 0x1000;
    }
    
}
 


