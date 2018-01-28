/* 
 * File:   Node.cpp
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 * 
 * Created on January 28, 2018, 10:58 AM
 */

#include "Node.h"

Node::Node(uint8_t e, Node n) {
    element = e;
    next = n;
}

Node::~Node() {
}

Node::getElement() {
    return element;
}

Node::getNext() {
    return next;
}

Node::setNext(Node n) {
    next = n;
}