/* 
 * File:   SinglyLinkedList.cpp
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 * 
 * Created on January 28, 2018, 10:56 AM
 */

#include "SinglyLinkedList.h"

SinglyLinkedList::SinglyLinkedList() {
    head = 0;
    tail = 0;
    size = 0;
}

SinglyLinkedList::~SinglyLinkedList() {
}

SinglyLinkedList::size() {
    return size;
}

SinglyLinkedList::isEmpty() {
    return size == 0;
}

SinglyLinkedList::first() {
    if (isEmpty()) return 0;
    return head.getElement();
}

SinglyLinkedList::last() {
    if (isEmpty()) return 0;
    return tail.getElement();
}

SinglyLinkedList::getElement(int index) {
    if (index >= 0 && index <= size) {
        if (isEmpty) return 0;
        Node n = head;
        for (int i = 0; i < index; i++) {
            n = n.getNext();
        }
        return n.getElement();
    } else {
        throw std::invalid_argument("Invalid index");
    }
}

SinglyLinkedList::add(int index, uint8_t val) {
    if (index >= 0 && index <=size) {
        if (isEmpty()) {
            head = new Node(val, 0);
        } else if (index == size) {
            tail.setNext(new Node(val, 0));
        } else {
            Node n = head; 
            for (int i = 0; i < index; i++) {
                n = n.getNext();
            }
            n.setNext(new Node(val, n.getNext()));
        }
        size++;
    } else {
        throw std::invalid_argument("Invalid index");
    }
    
}

SinglyLinkedList::remove(int index) {
    if (index >= 0 && index <=size) {
        if (isEmpty()) {
            return 0;
        } else if (size == 1) {
            uint8_t val = head.getElement();
            head = 0;
            size--;
            return val;
        } else {
            Node n = head;
            for (int i = 0; i < index - 1; i++) {
                n = n.getNext();
            }
            Node remove = n.getNext();
            n.setNext(remove.getNext());
            size--;
            return remove.getElement();
        }
    } else {
        throw std::invalid_argument("Invalid index");
    }
}


