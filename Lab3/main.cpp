/* 
 * File:   main.cpp
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 *
 * Created on January 27, 2018, 11:38 PM
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include "PageFrameAllocator.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    ifstream file(argv[1]);
    string s;
    std::getline(file, s);
    int num_page_frames = s;
    std::cout << '> ' << num_page_frames << std::endl; 
    
    std::vector<uint32_t> page_frames;
    
    PageFrameAllocator pfa(num_page_frames);
    
    while(std::getline(file, s)) {
        istringstream iss(s);
        int command;
        iss >> command;
        std::cout << '> ' << command;
        
        if (command == 0) {
           int amount;
           iss >> amount;
           std::cout << '> ' << amount << std::endl;
           bool b = pfa.Deallocate(amount, &page_frames); 
           
           if (b) {
               std::cout << " T " << pfa.get_page_frames_total() << std::endl;
           } else {
               std::cout << " F " << pfa.get_page_frames_total() << std::endl;
           }
           
        } else if (command == 1) {
           int amount;
           iss >> amount;
           std::cout << '> ' << amount << std::endl;
           bool b = pfa.Allocate(amount, &page_frames); 
           
           if (b) {
               std::cout << " T " << pfa.get_page_frames_free() << std::endl;
           } else {
               std::cout << " F " << pfa.get_page_frames_free() << std::endl;
           }
           
        } else {
            std::cout << std::endl;
            std::cout << " ";
            for (int i = 0; i < pfa.get_page_frames_free(); i++) {
                //insert line to access page numbers of free list
            }
        }
    }
    
    return 0;
}

