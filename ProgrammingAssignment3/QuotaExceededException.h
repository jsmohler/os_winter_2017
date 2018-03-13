/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   QuotaExceededException.h
 * Author: jordanmohler
 *
 * Created on March 10, 2018, 6:59 PM
 */

#ifndef QUOTAEXCEEDEDEXCEPTION_H
#define QUOTAEXCEEDEDEXCEPTION_H

#include <iostream>

class QuotaExceededException : public std::exception {
public:
  /**
   * Exception thrown when trying to allocate more pages than the quota designates as allowed
   */
  QuotaExceededException(void) {std::cout << "ERROR: memory quota a exceeded\n";}
};

#endif /* QUOTAEXCEEDEDEXCEPTION_H */

