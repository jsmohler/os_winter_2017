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


//The main program reads input from the file specified by the first command line argument. 
//Output is written to standard output.
//Reads the input file a line at a time, and processes the allocation or deallocation specified by each line in sequence.

int main(int argc, char** argv) {

    //Reads a single hex integer specifying the number of page frames from the first line of the input file.
    ifstream file(argv[1]);
    string s;
    std::getline(file, s);
    int num_page_frames = s;
    std::cout << '> ' << num_page_frames << std::endl;

    std::vector<uint32_t> page_frames;

    PageFrameAllocator pfa(num_page_frames);

    while (std::getline(file, s)) {
        istringstream iss(s);
        int command;
        iss >> command;
        //Reading each input file line (including the first line), then writing the line to output, preceded by the '>" character.
        std::cout << '> ' << command;


        //to deallocate page frames
        if (command == 0) {
            //number of page frames to deallocate
            int amount;
            iss >> amount;
            //Reading each input file line (including the first line), then writing the line to output, preceded by the '>" character.
            std::cout << '> ' << amount << std::endl;
            bool b = pfa.Deallocate(amount, &page_frames);

            if (b) {
                //The first character of the output line is blank. The S value is T if Allocate returned true
                std::cout << " T " << pfa.get_page_frames_total() << std::endl;
            } else {
                //The S value is F if Allocate returned false
                std::cout << " F " << pfa.get_page_frames_total() << std::endl;
            }
            //to allocate page frames
        } else if (command == 1) {
            //number of page frames to deallocate
            int amount;
            iss >> amount;
            //Reading each input file line (including the first line), then writing the line to output, preceded by the '>" character.
            std::cout << '> ' << amount << std::endl;
            bool b = pfa.Allocate(amount, &page_frames);

            if (b) {
                //The first character of the output line is blank. The S value is T if Allocate returned true
                std::cout << " T " << pfa.get_page_frames_free() << std::endl;
            } else {
                 //The S value is F if Allocate returned false
                std::cout << " F " << pfa.get_page_frames_free() << std::endl;
            }
            //print the contents of the free list
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

