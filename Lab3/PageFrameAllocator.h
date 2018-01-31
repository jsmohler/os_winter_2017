/* 
 * File:   PageFrameAllocator.h
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 *
 * Created on January 27, 2018, 11:39 PM
 */



#ifndef PAGEFRAMEALLOCATOR_H
#define PAGEFRAMEALLOCATOR_H

#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <iostream>

class PageFrameAllocator {
public:
    PageFrameAllocator(int num_frames);
    virtual ~PageFrameAllocator();
    
    //Rule of 3: prevent use of copy, assign
    PageFrameAllocator(const PageFrameAllocator&) = delete;
    PageFrameAllocator &operator=(const PageFrameAllocator&) = delete;
    
    //Rule of 5: prevent use of move and move assign
    PageFrameAllocator(const PageFrameAllocator&&) = delete;
    PageFrameAllocator &operator=(const PageFrameAllocator&&) = delete;
    
    
    uint32_t get_page_frames_free() const { return page_frames_free; }
    uint32_t get_page_frames_total() const { return page_frames_total; }
    uint32_t get_free_list_head() const { return free_list_head; }
    std::vector<uint8_t> get_memory() const { return memory; }
    
    bool Allocate(uint32_t count, std::vector<uint32_t> &page_frames);
    
    bool Deallocate(uint32_t count, std::vector<uint32_t> &page_frames);
    
    
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

#endif /* PAGEFRAMEALLOCATOR_H */

