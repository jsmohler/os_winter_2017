/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Program1.h
 * Author: Cedric Smith, Jordan Mohler, Lexie Hermosura
 *
 * Created on February 11, 2018, 5:43 PM
 */

#ifndef PROGRAM1_H
#define PROGRAM1_H

#include <cstring>
#include <cstdint>
#include <iostream>

class Scheduler {
    
public:
    Scheduler()
    
    
    
    
    
};



class PageFrameAllocator {
public:
    PageFrameAllocator(uint32_t num_frames);
    virtual ~PageFrameAllocator();
    
    //Rule of 3: prevent use of copy, assign
    PageFrameAllocator(const PageFrameAllocator&) = delete;
    PageFrameAllocator &operator=(const PageFrameAllocator&) = delete;
    
    //Rule of 5: prevent use of move and move assign
    PageFrameAllocator(const PageFrameAllocator&&) = delete;
    PageFrameAllocator &operator=(const PageFrameAllocator&&) = delete;
    
    /*
     * get_page_frames_free() 
     * takes no parameters
     * returns the total number of free page frames
     */
    uint32_t get_page_frames_free() const { return page_frames_free; }
    
    /*
     * get_page_frames_total()
     * takes no parameters
     * returns the total number of page frames in memory
     */
    uint32_t get_page_frames_total() const { return page_frames_total; }
    
    /*
     * get_free_list_head()
     * takes no parameters
     * returns the page frame number of the first page frame in the free list (0xFFFFFFFF if list empty)
     */
    uint32_t get_free_list_head() const { return free_list_head; }
    
    /*
     * get_memory()
     * takes no parameters
     * returns a vector that contains all page frames, including ones that have been deallocated
     */
    std::vector<uint8_t> get_memory() const { return memory; }
    
    /*
     * Allocate(uint32_t count, std::vector<uint32_t> &page_frames)
     * takes a uint32_t called count which is the number of page frames to be allocated in memory
     * takes a reference to a vector of <uint32_t> that pushes count page frames onto page_frames
     * returns true if the page frames were successfully allocated, but false if count > page_frames_free, in which case
     * no page frames are to be allocated
     */
    bool Allocate(uint32_t count, std::vector<uint32_t> &page_frames);
    
    /*
     * Deallocate(uint32_t count, std::vector<uint32_t> &page_frames)
     * takes a uint32_t called count which is the number of page frames to be deallocated from memory
     * takes a reference to a vector of <uint32_t> that pops count page frames from page_frames
     * returns true if the count <= the size of page_frames, but false otherwise, in which case no page frames
     * are freed.
     */
    bool Deallocate(uint32_t count, std::vector<uint32_t> &page_frames);
    
    /*
     * printFromArray(int index)
     * takes an integer index
     * returns the page frame number at index in the free list
     */
    uint32_t printFromArray(int index);
    
    
private:
    //Byte array that will contain the page frames to be managed
    std::vector<uint8_t> memory;
    
    //A count of the total number of page frames in memory (memory size divided by 0x1000)
    uint32_t page_frames_total;
    
    //The current number of free page frames
    uint32_t page_frames_free;
    
    //The page frame number of the first page frame in the free list (0xFFFFFFFF if list empty)
    uint32_t free_list_head;
};


#endif /* PROGRAM1_H */

