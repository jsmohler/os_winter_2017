/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ProcessTrace.cpp
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 * 
 * Created on January 19, 2018, 4:14 PM
 */

#include "ProcessTrace.h"

ProcessTrace::ProcessTrace(string name) {
    file.open(name);
}


ProcessTrace::~ProcessTrace() {
    file.close();
}

void ProcessTrace::Execute() {
    vector<uint8_t> vec(100);
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

            if (command.compare("alloc") == 0){
                uint32_t size;
                iss >> std::hex >> size;
                cout << std::hex << size << "\n";
                vec.resize(size);
                
            } else if (command.compare("compare") == 0) { 
                uint32_t addr;
                iss >> std::hex >> addr;
                cout << addr << " ";
                vector<uint32_t> orgvals;
                vector<uint32_t> expvals;
                uint32_t val;
                uint32_t oval;
                
                
                
                while (!iss.eof()) {
                    iss >> std::hex >> val;
                    cout << val << " ";
                    expvals.push_back(val);  
                }
                
                for (int i = 0; i < expvals.size(); i++) {
                    if (vec.at(addr+i) != expvals.at(i)) {
                        cout << "error at address " << addr << ", expected " << vec.at(addr+i) << ", actual is " << expvals.at(i) << "\n"; 
                    }
                }
                cout << "\n";
            } else if (command.compare("put") == 0) { 
                uint32_t addr;
                iss >> std::hex >> addr;
                cout << addr << " ";
                uint32_t val;
                while (!iss.eof()) {
                    iss >> std::hex >> val;
                    cout << val << " ";
                    vec.at(addr) = val;
                    addr++;
                }
                cout << "\n"; // to keep it pretty and similar to sample output file
            } else if (command.compare("fill") == 0) {  
                uint32_t addr;
                iss >> std::hex >> addr;
                uint32_t count;
                iss >> std::hex >> count;
                uint32_t val;
                iss >> std::hex >> val;
                cout << addr << " " << count << " " << val;
                for (int i = 0; i < count; i++) {
                    vec.at(addr+1) = val;
                }
                cout << "\n"; // to keep it pretty and similar to sample output file
            } else if (command.compare("copy") == 0) { 
                uint32_t dest_addr;
                iss >> std::hex >> dest_addr;
                uint32_t src_addr;
                iss >> std::hex >> src_addr;
                uint32_t count;
                iss >> std::hex >> count;
                cout << dest_addr << " " << src_addr << " " << count;
                for (int i = 0; i < count; i++) {
                    vec.at(dest_addr + i) = vec.at(src_addr + i);
                }
                cout << "\n";
            } else if (command.compare("dump") == 0) { 
                uint32_t addr;
                iss >> std::hex >> addr;
                uint16_t count;
                iss >> std::hex >> count;
                cout << addr << " " << count << "\n";
                for (int i = 0; i < count; i++) {
                    //cout << std::hex << vec.at(addr+i)++ << " ";
                    printf("%hhx ",vec[addr+i]);
                    if (i%16 == 0) {
                        cout << "\n";
                    }
                }
            }
        }
            
            
    }
        
}


