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
    
    if (file.is_open()) {
        string s;
        while(getline(file, s)) {
            istringstream iss(s);
            string command;
            iss >> command;
            cout << command;

            if (command.compare("alloc") == 0){
                uint32_t size;
                iss >> size;
                vec.resize(size);
            } else if (command.compare("compare") == 0) { 
                uint32_t addr;
                iss >> addr;
                vector<uint8_t> expvals;
                uint8_t val;
                while (!iss.eof()) {
                    iss >> val;
                    expvals.push_back(val);  
                }
                for (int i = 0; i < expvals.size(); i++) {
                    if (vec.at(addr+i) != expvals.at(i)) {
                        cout << "error at address " << addr << ", expected " << vec.at(addr+i) << ", actual is " << expvals.at(i) << "\n"; 
                    }
                }
            } else if (command.compare("put") == 0) { 
                uint32_t addr;
                iss >> addr;
                uint8_t val;
                while (!iss.eof()) {
                    iss >> val;
                    vec.at(addr) = val;
                    addr++;
                }
            } else if (command.compare("fill") == 0) {  
                uint32_t addr;
                iss >> addr;
                uint32_t count;
                iss >> count;
                uint8_t val;
                iss >> val;
                for (int i = 0; i < count; i++) {
                    vec.at(addr+1) = val;
                }
            } else if (command.compare("copy") == 0) { 
                uint32_t dest_addr;
                iss >> dest_addr;
                uint32_t src_addr;
                iss >> src_addr;
                uint32_t count;
                iss >> count;
                for (int i = 0; i < count; i++) {
                    vec.at(dest_addr + i) = vec.at(src_addr + i);
                }
            } else if (command.compare("dump") == 0) { 
                uint32_t addr;
                iss >> addr;
                uint32_t count;
                iss >> count;
                cout << addr << "\n";
                for (int i = 0; i < count; i++) {
                    cout << vec.at(addr+i) << " ";
                    if (i%16 == 0) {
                        cout << "\n";
                    }
                }
            }
        }
            
            
    }
        
}


