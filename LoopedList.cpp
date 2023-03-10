//
// Created by THEO on 10.03.2023.
//

#include "LoopedList.h"

int Node::getData() const {
    return data;
}

Node *Node::getPrev() {
    return prev;
}

Node *Node::getNext() {
    return next;
}

Node::Node(int data, Node *prev, Node *next) : data(data), prev(prev), next(next) {}

Node *LoopedList::getHead() {
    return head;
}

Node *LoopedList::getTail() {
    return tail;
}

void LoopedList::insertTail(int data) {
    Node *item = new Node(data);
    if(head == nullptr){
        head = tail = item;
        head->next = head->prev = tail->next = tail->prev = item;
    } else {
        tail->next = item;
        item->prev = tail;
        item->next = head;
        tail = item;
    }
}

void LoopedList::remove(Node *to_remove) {
    if(head == tail) {
        head = tail = nullptr;
        delete to_remove;
        return;
    }
    to_remove->prev->next = to_remove->next;
    to_remove->next->prev = to_remove->prev;
    if(to_remove == head){
        head = to_remove->next;
    }
    if(to_remove == tail){
        tail = to_remove->prev;
    }
    delete to_remove;
}
