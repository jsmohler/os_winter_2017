/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ProcessTrace.cpp
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 * 
 * Created on February 3, 2018
 */

#include "ProcessTrace.h"
#include <MMU.h>
#include <cmath> 
#include <cstdlib>

/*
 * The constructor takes a single string argument, which is the name of the execution trace file. 
 * The constructor opens the trace file and performs other initialization required by the class.
 */
ProcessTrace::ProcessTrace(string name) {
    file.open(name);
}

/*
 * The destructor for the class should close the trace file.
 */
ProcessTrace::~ProcessTrace() {
    file.close();
}



void ProcessTrace::Execute() {
    
    //vector<uint8_t> vec(100);
    int line = 1;
    
    if (file.is_open()) {
        string s;
        while(getline(file, s)) {
            istringstream iss(s);
            string command;
            iss >> command;
            cout << std::dec << line << ": " << command << " ";
            line++;
            cout << std::hex;

            
            /* 
             * Allocate memory for size bytes, addressed from 0 to size-1. 
             * The first line of the file is an alloc command. 
             * The subsequent alloc commands used change the amount of allocated memory. 
             * All of the contents of the newly-allocated memory are initialized to 0.
             */
            if (command.compare("alloc") == 0){
                uint32_t size;
                iss >> std::hex >> size;
                
                
                uint32_t roundSize = ceil(size/4096.0000) * 0x1000;
                if(roundSize == 0)
                {
                    roundSize = 0x1000;
             
                }
                
                mem::Addr numPageFrames = roundSize/0x1000;
 
                mem::MMU mem(numPageFrames);
                
                cout << std::hex << size << " , " << mem.get_frame_count() << "\n";
                
                //vec.resize(size);
                
             /*
              * Compares bytes starting at addr; expected_values is a list of byte values, separated by white space.
              */
            } else if (command.compare("compare") == 0) { 
                mem::Addr addr;
                iss >> std::hex >> addr;
                cout << addr << " ";
                vector<uint32_t> expvals;
                mem::Addr val;
                
      
                
                while (!iss.eof()) {
                    iss >> std::hex >> val;
                    cout << val << " ";
                    expvals.push_back(val);  
                }
                
                /* 
                 * If the actual values of bytes starting at addr don't match the expected_values, 
                 * write an error message to standard error for each mismatch with the address, the expected value, 
                 * and the actual value (all in hexadecimal). 
                 */
                for (int i = 0; i < expvals.size(); i++) {
//                    if (vec.at(addr+i) != expvals.at(i)) {
//                        if (i == 0) {
//                            cout << std::endl;
//                        }
//                        cout << "error at address ";
//                        printf("%02x", addr+i);
//                        cout << ", expected ";
//                        printf("%hhx", expvals.at(i));
//                        cout << ", actual is ";
//                        printf("%hhx", vec.at(addr+i));
//                        cout << std::endl;
//                    }
                }
                cout << "\n";
                
             /*
             * Store values starting at addr; values is a list of byte values, separated by white space.
             */
            } else if (command.compare("put") == 0) { 
                uint32_t addr;
                iss >> std::hex >> addr;
                cout << addr << " ";
                uint32_t val;
                while (!iss.eof()) {
                    iss >> std::hex >> val;
                    cout << val << " ";
//                    vec.at(addr) = val;
                    addr++;
                }
                cout << "\n";
                
              /*
               * Store count copies of value starting at addr.
               */  
            } else if (command.compare("fill") == 0) {  
                uint32_t addr;
                iss >> std::hex >> addr;
                uint32_t count;
                iss >> std::hex >> count;
                uint32_t val;
                iss >> std::hex >> val;
                cout << addr << " " << count << " " << val;
                for (int i = 0; i < count; i++) {
//                    vec.at(addr+i) = val;
                }
                cout << "\n"; 
                
                /*
                 * Copy count bytes from src_addr to dest_addr. 
                 * The source and destination ranges will not overlap.
                 */
            } else if (command.compare("copy") == 0) { 
                uint32_t dest_addr;
                iss >> std::hex >> dest_addr;
                uint32_t src_addr;
                iss >> std::hex >> src_addr;
                uint32_t count;
                iss >> std::hex >> count;
                cout << dest_addr << " " << src_addr << " " << count;
                for (int i = 0; i < count; i++) {
//                    vec.at(dest_addr + i) = vec.at(src_addr + i);
                }
                cout << "\n";
                
                /*
                 * Writing a line with addr to standard output, followed on separate lines by count bytes starting at addr. 
                 * Writing 16 bytes per line, with a space between adjacent values. 
                 * Print each byte as exactly 2 digits with a leading 0 for values less than 10 (hex).
                 */
            } else if (command.compare("dump") == 0) { 
                uint32_t addr;
                iss >> std::hex >> addr;
                uint16_t count;
                iss >> std::hex >> count;
                cout << addr << " " << count << "\n";
                cout << addr << std::endl;
                for (int i = 0; i < count; i++) {
//                    uint8_t temp = vec[addr+i];
////                    printf("%02x ",temp);
//                    if ((i+1)%16 == 0) {
//                        cout << "\n";
//                    }
                }
            }
        }  
    }   
}


