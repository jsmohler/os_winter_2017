/* 
 * File:   PageFrameAllocator.cpp
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 * 
 * Created on January 27, 2018, 11:39 PM
 */

#include <vector>

#include "PageFrameAllocator.h"

PageFrameAllocator::PageFrameAllocator(int num_frames) {
    memory.resize(num_frames*0x1000);
    page_frames_total = num_frames;
    page_frames_free = 0;
    free_list_head = 0xFFFFFFFF;
}

PageFrameAllocator::~PageFrameAllocator() {
}

