/* 
 * File:   PageFrameAllocator.cpp
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 * 
 * Created on January 27, 2018, 11:39 PM
 */

#include "PageFrameAllocator.h"
#include "MemoryDefs.h"
#include "PageTable.h"
#include <MMU.h>

using mem::Addr;
using mem::MMU;

PageFrameAllocator::PageFrameAllocator(mem::MMU& memory)
{
    Addr num_frames = memory.get_frame_count();
    
    //address of the free list starts at 0
    Addr next = 0;
    page_frames_free = num_frames;
    page_frames_total = num_frames;

    //build the free list
    for (uint32_t page_number = 1; page_number < num_frames; page_number++) {
        memory.put_byte(next, reinterpret_cast<uint8_t*>(&page_number));
        next+=0x1000;
    }
            
    //the end of the list points to the address 0xFFFFFFFF
    next = 0xFFFFFFFF;
    memory.put_byte((num_frames - 1)*0x1000, reinterpret_cast<uint8_t*>(&next));  
    free_list_head = 0;
}

PageFrameAllocator::~PageFrameAllocator() {}

Addr PageFrameAllocator::Allocate(uint32_t count, 
                                  std::vector<uint32_t> &page_frames, mem::MMU& memory) {
  uint8_t page_number; 
  uint8_t clear = 0; 
  Addr size = 0x1000; 
  
  Addr freed_addr = free_list_head*kPageSize;
  
  if (count <= page_frames_free) {  // if enough to allocate
    while (count-- > 0) {
      // Return next free frame to caller
      page_frames.push_back(free_list_head);
      
      // De-link frame from head of free list
      memory.get_byte(&page_number, freed_addr);
      
      //Clear page
      memory.put_bytes(freed_addr, size, &clear); 
      
      free_list_head = page_number;
      --page_frames_free;
    }
    return freed_addr;
  } else {
    return 0xFFFF;  // do nothing and return error
  }
}


bool PageFrameAllocator::Deallocate(Addr count, std::vector<uint32_t>& page_frames, Addr vAddress, mem::MMU& memory){
    if (count <= page_frames.size()) {
        
        //have a variable to keep track of the current free list head as we deallocate memory
        uint8_t head;
        Addr allocated_page;
        Addr destination;
        
        //take the pages to deallocate, pop them off of page_frames and update the free list head accordingly
        for (int i = 0; i < count; i++) {
            head = free_list_head;
            allocated_page = page_frames[page_frames.size()-1];
            destination = allocated_page * 0x1000;
            page_frames.pop_back();
            memory.put_byte(destination, &head);
            free_list_head = allocated_page;
            page_frames_free++;
        } 
        return true;
    } else {
        return false;
    }
}


//uint32_t PageFrameAllocator::printFromArray(Addr index) {
//    //have a variable to hold page frame number
//    uint8_t page_number;
//    
//    //access the memory at the index and return the page frame number
//    memory->get_byte(&page_number, index);
//    return page_number;
//}

