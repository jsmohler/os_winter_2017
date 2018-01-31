/* 
 * File:   PageFrameAllocator.cpp
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 * 
 * Created on January 27, 2018, 11:39 PM
 */

#include "PageFrameAllocator.h"

PageFrameAllocator::PageFrameAllocator(uint32_t num_frames)
{
    //resize memory to have enough space for num_frames page frames
    memory.resize(num_frames*0x1000);
    page_frames_total = num_frames;
    page_frames_free = num_frames;

    //address of the free list starts at 0
    uint32_t v32 = 0;

    //build the free list
    for (int i = 0; i < memory.size(); i+=0x1000) {
        v32++;
        memcpy(&memory[i], &v32, sizeof(uint32_t));
    }

    //the end of the list points to the address 0xFFFFFFFF
    v32 = 0xFFFFFFFF;
    memcpy(&memory[memory.size() - 0x1000], &v32, sizeof(uint32_t));   
    free_list_head = 0;

}

PageFrameAllocator::~PageFrameAllocator() {
        
}


bool PageFrameAllocator::Allocate(uint32_t count, std::vector<uint32_t> &page_frames) {
    if (count > page_frames_free) {
        return false;
    } else {
        //have a variable to hold the next address in the free list
        uint32_t next;
        
        //go through the free list and push page frames that we have allocated, adjust page_frames_free accordingly
        for (uint32_t i = 0; i < count; i++) {
            page_frames.push_back(free_list_head);
            memcpy(&next, &memory[free_list_head*0x1000], sizeof(uint32_t));
            free_list_head = next;
            page_frames_free--;
        }
        return true;
    }
    
}

bool PageFrameAllocator::Deallocate(uint32_t count, std::vector<uint32_t>& page_frames){
    if (count <= page_frames.size()) {
        
        //have a variable to keep track of the current free list head as we deallocate memory
        uint32_t v32;
        
        //take the pages to deallocate, pop them off of page_frames and update the free list head accordingly
        for (int i = 0; i < count; i++) {
            v32 = free_list_head;
            uint32_t allocated_page = page_frames[page_frames.size()-1];
            page_frames.pop_back();
            memcpy(&memory[allocated_page * 0x1000], &v32, sizeof(uint32_t));
            free_list_head = allocated_page;
            page_frames_free++;
        } 
        return true;
    } else {
        return false;
    }
}


uint32_t PageFrameAllocator::printFromArray(int index) {
    //have a variable to hold page frame number
    uint32_t v32;
    
    //access the memory at the index and return the page frame number
    memcpy(&v32, &memory[index*0x1000], sizeof(uint32_t));
    return v32;
}

