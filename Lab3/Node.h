/* 
 * File:   Node.h
 * Author: Jordan Mohler, Cedric Smith, Lexie Hermosura
 *
 * Created on January 28, 2018, 10:58 AM
 */

#ifndef NODE_H
#define NODE_H

class Node {
public:
    Node(uint8_t e, Node n);
    virtual ~Node();
    getElement();
    getNext();
    setNext(Node n);
    
    //Rule of 3: prevent use of copy, assign
    Node(const Node&) = delete;
    Node &operator=(const Node&) = delete;
    
    //Rule of 5: prevent use of move and move assign
    Node(const Node&&) = delete;
    Node &operator=(const Node&&) = delete;
private:
    uint8_t element;
    Node next;
};

#endif /* NODE_H */

