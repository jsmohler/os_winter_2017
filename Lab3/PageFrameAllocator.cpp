/* 
 * File:   PageFrameAllocator.cpp
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 * 
 * Created on January 27, 2018, 11:39 PM
 */

#include "PageFrameAllocator.h"

PageFrameAllocator::PageFrameAllocator(int num_frames) {
    memory.resize(num_frames*0x1000);
    page_frames_total = num_frames;
    page_frames_free = num_frames;

    uint32_t v32 = 0;

    //build the free list
    for (int i = 0; i < memory.size(); i+=0x1000) {
        v32++;
        memcpy(&memory[i], &v32, sizeof(uint32_t));
    }

    v32 = 0xFFFFFFFF;
    memcpy(&memory[memory.size() - 0x1000], &v32, sizeof(uint32_t));   
    free_list_head = 0;

}

PageFrameAllocator::~PageFrameAllocator() {
        
}


bool PageFrameAllocator::Allocate(uint32_t count, std::vector<uint32_t> &page_frames) {
    return 0;
    
}

bool PageFrameAllocator::Deallocate(uint32_t count, std::vector<uint32_t>& page_frames){
    return 0;
}

