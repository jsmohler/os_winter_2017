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
#include <MMU.h>

using mem::Addr;

// Bit masks for other flags
const uint32_t kPTE_Present = 0;            // page present in memory
const uint32_t kPTE_PresentMask = (1 << kPTE_Present);
const uint32_t kPTE_Writable = 1;           // page is writable
const uint32_t kPTE_WritableMask = (1 << kPTE_Writable);
const uint32_t kPTE_Accessed = 5;           // set when page is accessed
const uint32_t kPTE_AccessedMask = (1 << kPTE_Accessed);
const uint32_t kPTE_Modified = 6;           // set when page is modified
const uint32_t kPTE_ModifiedMask = (1 << kPTE_Modified);

const int  kPageSizeBits = 12;  // shift count for page size
const Addr kPageSize = (1 << kPageSizeBits);
const Addr kPageOffsetMask = (kPageSize - 1);
const Addr kPageNumberMask = ~kPageOffsetMask;

// Page table size
const int  kPageTableSizeBits = 10;
const Addr kPageTableEntries = (1 << kPageTableSizeBits);
static_assert(kPageTableEntries == kPageSize / sizeof(Addr),
              "Page size and page table size mismatch");
const Addr kPageTableSizeBytes = kPageSize;
const Addr kPageTableIndexMask = kPageTableEntries - 1;

// The page frame number is stored in the upper 20 bits
const uint32_t kPTE_FrameMask = kPageNumberMask;

class PageFrameAllocator {
public:
    PageFrameAllocator(mem::MMU& input);
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
    Addr get_page_frames_free() const { return page_frames_free; }
    
    /*
     * get_page_frames_total()
     * takes no parameters
     * returns the total number of page frames in memory
     */
    Addr get_page_frames_total() const { return page_frames_total; }
    
    /*
     * get_free_list_head()
     * takes no parameters
     * returns the page frame number of the first page frame in the free list (0xFFFFFFFF if list empty)
     */
    Addr get_free_list_head() const { return free_list_head; }
    
    /*
     * get_memory()
     * takes no parameters
     * returns a vector that contains all page frames, including ones that have been deallocated
     */
    //std::unique_ptr<mem::MMU> get_memory() const { return memory; }
    
    
    /*
     * Allocate(uint32_t count, std::vector<uint32_t> &page_frames)
     * takes a uint32_t called count which is the number of page frames to be allocated in memory
     * takes a reference to a vector of <uint32_t> that pushes count page frames onto page_frames
     * returns true if the page frames were successfully allocated, but false if count > page_frames_free, in which case
     * no page frames are to be allocated
     */
    Addr Allocate(uint32_t count, std::vector<uint32_t> &page_frames, mem::MMU& memory);
    
    /*
     * Deallocate(uint32_t count, std::vector<uint32_t> &page_frames)
     * takes a uint32_t called count which is the number of page frames to be deallocated from memory
     * takes a reference to a vector of <uint32_t> that pops count page frames from page_frames
     * returns true if the count <= the size of page_frames, but false otherwise, in which case no page frames
     * are freed.
     */
    bool Deallocate(Addr count, std::vector<uint32_t> &page_frames, Addr start, mem::MMU& memory);
    
    
    
private:
    //Memory
    std::unique_ptr<mem::MMU> memory;
   
    //A count of the total number of page frames in memory (memory size divided by 0x1000)
    uint32_t page_frames_total;
    
    //The current number of free page frames
    uint32_t page_frames_free;
    
    //The page frame number of the first page frame in the free list (0xFFFFFFFF if list empty)
    uint32_t free_list_head;
};

#endif /* PAGEFRAMEALLOCATOR_H */

