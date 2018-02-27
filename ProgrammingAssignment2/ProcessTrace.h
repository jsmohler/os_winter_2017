/* 
 * File:   ProcessTrace.h
 * Author: Mike Goss <mikegoss@cs.du.edu>
 *
 */

#ifndef PROCESSTRACE_H
#define PROCESSTRACE_H

#include <MMU.h>
#include "PageFrameAllocator.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class ProcessTrace {
public:
  /*
   * Constructor - open trace file, initialize processing
   * 
   * @param file_name_ source of trace commands
   * @param &input a reference to the number of page frames
   * @param &pfa page a reference to the frames to allocate
   */
  ProcessTrace(string file_name_, mem::MMU &input, PageFrameAllocator &pfa);
  
  /*
   * Destructor - close trace file, clean up processing
   */
  virtual ~ProcessTrace(void);

  // Preventing the use of other constructors, assignment
  ProcessTrace(const ProcessTrace &other) = delete;
  ProcessTrace(ProcessTrace &&other) = delete;
  ProcessTrace operator=(const ProcessTrace &other) = delete;
  ProcessTrace operator=(ProcessTrace &&other) = delete;
  
  /*
   * Execute - read and process commands from trace file
   */
  void Execute(void);
  
private:
  // Trace file
  std::string file_name;
  std::fstream trace;
  long line_number;

  // Memory contents
  mem::MMU& memory;
  
  //Allocated paged
  std::vector<uint32_t> page_frames_allocated;
  
  //Allocator
  PageFrameAllocator& allocator;
  
  //Directory Base
  Addr directory_base;
  
  /*
   * ParseCommand - parse a trace file command. Aborts program if invalid trace file.
   * 
   * @param line return the original command line
   * @param cmd return the command name
   * @param cmdArgs returns a vector of argument bytes
   * @return true if command parsed, false if end of file
   */
  bool ParseCommand(
      std::string &line, std::string &cmd, std::vector<uint32_t> &cmdArgs);
  
  /*
   * CmdAlloc - allocate memory for size bytes, addressed from 0 to size-1. 
   *
   * @param line original text of command line
   * @param cmd command, converted to all lower case
   * @param cmdArgs arguments to command
   */
  void CmdAlloc(const std::string &line, 
                const std::string &cmd, 
                const std::vector<uint32_t> &cmdArgs);
  
  /*
   * CmdCompare - compares bytes starting at addr; expected_values is a list of byte values, 
   * separated by white space. If the actual values of bytes starting at addr 
   * don't match the expected_values, writes an error message to standard error 
   * for each mismatch with the address, the expected value, and the actual value 
   * (all in hexadecimal).
   * 
   * @param line original text of command line
   * @param cmd command, converted to all lower case
   * @param cmdArgs arguments to command
   **/
  void CmdCompare(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  
  /*
   * CmdPut - store values starting at addr; values is a list of byte values, separated 
   * by white space.
   * 
   * @param line original text of command line
   * @param cmd command, converted to all lower case
   * @param cmdArgs arguments to command
   */ 
  void CmdPut(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  
  /*
   * CmdFill - store count copies of value starting at addr.
   * 
   * @param line original text of command line
   * @param cmd command, converted to all lower case
   * @param cmdArgs arguments to command
   */
  void CmdFill(const std::string &line, 
               const std::string &cmd, 
               const std::vector<uint32_t> &cmdArgs);
  /*
   * CmdCopy - copy count bytes from src_addr to dest_addr. The source and destination 
   * ranges should not overlap.
   * 
   * @param line original text of command line
   * @param cmd command, converted to all lower case
   * @param cmdArgs arguments to command
   */
  void CmdCopy(const std::string &line, 
               const std::string &cmd, 
               const std::vector<uint32_t> &cmdArgs);
  /*
   * CmdDump - write a line with addr to standard output, followed on separate lines by 
   * count bytes starting at addr. Write 16 bytes per line, with a space between 
   * adjacent values. 
   * 
   * @param line original text of command line
   * @param cmd command, converted to all lower case
   * @param cmdArgs arguments to command
   */
  void CmdDump(const std::string &line, 
               const std::string &cmd, 
               const std::vector<uint32_t> &cmdArgs);
  
  /* 
   * CmdComment - the # character in the first column means the remainder of the line should be treated as a comment. 
   * The command is echoed to output in the same way as other commands, but should otherwise be ignored.
   * 
   * @param line original text of command line
   * @param cmd command, converted to all lower case
   * @param cmdArgs arguments to command
   */
  void CmdComment(const string &line,
                  const string &cmd,
                  const vector<uint32_t> &cmdArgs);
  
  /* 
   * CmdWritable - changes the writable status of size bytes of memory, starting at virtual address vaddr. 
   * The starting address, vaddr, and the byte count, size are exact multiples of the page size (0x1000). 
   * If status is 0, the Writable bit in the 2nd level page table should be cleared for all Present pages in the range, 
   * otherwise the Writable bit in the 2nd level page table should be set for all Present pages in the range. 
   * Any pages in the range which are not Present should be ignored. The 1st level page table is not changed.
   * 
   * @param line original text of command line
   * @param cmd command, converted to all lower case
   * @param cmdArgs arguments to command
   */
  void CmdWritable(const string &line,
                  const string &cmd,
                  const vector<uint32_t> &cmdArgs);
};

#endif /* PROCESSTRACE_H */

