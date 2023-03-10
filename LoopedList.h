//
// Created by THEO on 10.03.2023.
//

#ifndef JOSEPHUS_LOOPEDLIST_H
#define JOSEPHUS_LOOPEDLIST_H

class LoopedList;

class Node {
    Node *prev, *next;
    int data;
public:
    int getData() const;

    Node *getPrev();

    Node *getNext();

    explicit Node(int data, Node *prev = nullptr, Node *next = nullptr);

    friend LoopedList;
};

class LoopedList {
    Node *head, *tail;
public:
    Node *getHead();

    Node *getTail();

    void insertTail(int data);

    void remove(Node *to_remove);
};

#endif //JOSEPHUS_LOOPEDLIST_H
