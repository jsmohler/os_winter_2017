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
    uint8_t next = 0;
    page_frames_free = num_frames;
    page_frames_total = num_frames;

    //build the free list
    for (Addr page_number = 0; page_number < num_frames-1; page_number+=0x1000) {
        next++;
        memory.put_byte(page_number, &next);
    }
            
    //the end of the list points to the address 0xFFFFFFFF
    next = 0xFF;
    memory.put_byte((num_frames - 1)*0x1000, &next);  
    free_list_head = 0;
}

PageFrameAllocator::~PageFrameAllocator() {}

void PageFrameAllocator::Alloc(Addr vaddress, Addr count, mem::MMU& memory) {
    std::vector<uint8_t> found_addr_bytes(4);
    uint32_t kPTE_FrameMask = 0xFFC00000;
    
    mem::PMCB pmcb(false, vaddress);
    memory.set_PMCB(pmcb);

    // Get entry in top level page table
    Addr top_level_index = (vaddress >> (kPageSizeBits + kPageTableSizeBits))
            & kPageTableIndexMask;
    Addr top_level_entry;
    Addr top_level_entry_pa =
            pmcb.page_table_base + top_level_index * sizeof(Addr);
    
    memory.get_bytes(&found_addr_bytes[0], top_level_entry_pa, 4);
    memcpy(&top_level_entry, &found_addr_bytes[0], sizeof(Addr));
    std::cout << "Entry exists (not 0): ";
    std::cout << (top_level_entry & 0xFFC00000) << std::endl;
    
    //If entry doesn't exist, build it
    if((top_level_entry & 0xFFC00000) == 0) {
      //Create a page table
      Addr next_free_addr = free_list_head * 0x1000;
      std::cout << "NFA: " << next_free_addr << std::endl;
      bool add_page = pop(next_free_addr, 1, memory);
      
      //Put page table address in directory (convert address to uint8_t)
      next_free_addr = next_free_addr << 13;
      memcpy(&found_addr_bytes[0], &next_free_addr, sizeof(Addr));
      std::cout << "NFA: " << next_free_addr << std::endl;
      memory.put_bytes(top_level_entry_pa, 4, &found_addr_bytes[0]);  
        
      //Reset top_level_entry
      memory.get_bytes(&found_addr_bytes[0], top_level_entry_pa, 4);
      memcpy(&top_level_entry, &found_addr_bytes[0], sizeof(Addr));
    }
    
    // Get 2nd level page table entry
    Addr second_level_entry = 0;
    Addr second_level_address = top_level_entry & kPTE_FrameMask;
    Addr second_level_index = (vaddress >> kPageSizeBits) & kPageTableIndexMask;
    Addr second_level_entry_pa =
            second_level_address + second_level_index * sizeof(Addr);
    memory.get_bytes(&found_addr_bytes[0], second_level_entry_pa, 4);
    memcpy(&second_level_entry, &found_addr_bytes[0], sizeof(Addr));
    
    //If second entry doesn't exist, build it
    if((second_level_entry & kPTE_PresentMask) == 0) {
      //Create a page
      Addr next_free_page = free_list_head * 0x1000;
      pop(next_free_page, 1, memory);
      
      //Put page address in second-level page table (convert address to uint8_t) 
      memcpy(&found_addr_bytes[0], &next_free_page, sizeof(Addr));
      memory.put_bytes(second_level_entry_pa, 4, &found_addr_bytes[0]);  
        
      //Reset second_level_entry
      memory.get_bytes(&found_addr_bytes[0], second_level_entry_pa, 4);
      memcpy(&second_level_entry, &found_addr_bytes[0], sizeof(Addr));
    }
}

bool PageFrameAllocator::pop(Addr vaddress, Addr count, mem::MMU& memory) {
    if (count > memory.get_frame_count()) {
        return false;
    } else {
        //have a variable to hold the next address in the free list
        uint8_t next;
        uint8_t clear = 0;
        Addr size = 0x1000;
        
        //go through the free list and push page frames that we have allocated, adjust page_frames_free accordingly
        for (uint32_t i = 0; i < count; i++) {
            memory.get_byte(&next, vaddress);
            memory.put_bytes(vaddress, size, &clear);
            free_list_head = next;
        }
        return true;
    }
}


//
//bool PageFrameAllocator::Allocate(Addr count, std::vector<uint32_t> &page_frames, Addr vAddress, mem::MMU& memory) {
//    mem::PMCB physical_mode(false, vAddress);
//    memory.set_PMCB(physical_mode);
//    
//    if (count > page_frames_free) {
//        return false;
//    } else {
//        //have a variable to hold the next address in the free list
//        uint8_t next;
//        Addr current;
//        uint8_t clear = 0;
//        Addr size = 0x1000;
//        
//        //go through the free list and push page frames that we have allocated, adjust page_frames_free accordingly
//        for (uint32_t i = 0; i < count; i++) {
//            std::cout << "i: " << i << std::endl;
//            current = vAddress / 0x1000;
//            page_frames.push_back(current);
//            memory.get_byte(&next, vAddress);
//            memory.put_bytes(vAddress, size, &clear);
//            free_list_head = next;
//            page_frames_free--;
//            std::cout << "FLH: " << free_list_head << std::endl;
//        }
//        return true;
//    }
//    
//}
//
//bool PageFrameAllocator::Deallocate(Addr count, std::vector<uint32_t>& page_frames, Addr vAddress, mem::MMU& memory){
//    if (count <= page_frames.size()) {
//        
//        //have a variable to keep track of the current free list head as we deallocate memory
//        uint8_t head;
//        Addr allocated_page;
//        Addr destination;
//        
//        //take the pages to deallocate, pop them off of page_frames and update the free list head accordingly
//        for (int i = 0; i < count; i++) {
//            head = free_list_head;
//            allocated_page = page_frames[page_frames.size()-1];
//            destination = allocated_page * 0x1000;
//            page_frames.pop_back();
//            memory.put_byte(destination, &head);
//            free_list_head = allocated_page;
//            page_frames_free++;
//        } 
//        return true;
//    } else {
//        return false;
//    }
//}
//
//
//uint32_t PageFrameAllocator::printFromArray(Addr index) {
//    //have a variable to hold page frame number
//    uint8_t page_number;
//    
//    //access the memory at the index and return the page frame number
//    memory->get_byte(&page_number, index);
//    return page_number;
//}

