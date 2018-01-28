/* 
 * File:   SinglyLinkedList.h
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 *
 * Created on January 28, 2018, 10:56 AM
 */

#include "Node.h"

#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

class SinglyLinkedList {
public:
    SinglyLinkedList();
    virtual ~SinglyLinkedList();
    int size();
    bool isEmpty();
    uint8_t first();
    uint8_t last();
    uint8_t getElement(int index);
    add(int index, uint8_t);
    uint8_t remove(int index);
       
    //Rule of 3: prevent use of copy, assign
    SinglyLinkedList(const SinglyLinkedList&) = delete;
    SinglyLinkedList &operator=(const SinglyLinkedList&) = delete;
    
    //Rule of 5: prevent use of move and move assign
    SinglyLinkedList(const SinglyLinkedList&&) = delete;
    SinglyLinkedList &operator=(const SinglyLinkedList&&) = delete;
private:
    int size;
    Node head;
    Node tail;
};

#endif /* SINGLYLINKEDLIST_H */

