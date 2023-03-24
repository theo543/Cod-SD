#ifndef JOSEPHUS_LOOPEDLIST_HPP
#define JOSEPHUS_LOOPEDLIST_HPP

#include <stdexcept>
#include "LoopedList.h"

#define TTT template<typename T>

TTT bool LoopedList<T>::Iterator::operator==(LoopedList::Iterator const &other) const {
    return node == other.node && list == other.list;
}

TTT LoopedList<T>::Iterator LoopedList<T>::Iterator::next() {
    return LoopedList::Iterator(node->next, list);
}

TTT [[maybe_unused]] LoopedList<T>::Iterator LoopedList<T>::Iterator::prev() {
    return LoopedList::Iterator(node->prev, list);
}

TTT T &LoopedList<T>::Iterator::value() {
    return node->value;
}

TTT LoopedList<T>::LoopedList() {
    head = tail = nullptr;
}

TTT LoopedList<T>::~LoopedList() {
    clear();
}

TTT [[nodiscard]] LoopedList<T>::Iterator LoopedList<T>::begin() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    return Iterator(head, this);
}

TTT LoopedList<T>::Iterator LoopedList<T>::end() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    return LoopedList::Iterator(tail, this);
}

TTT bool LoopedList<T>::empty() {
    return head == nullptr;
}

TTT void LoopedList<T>::clear() {
    if (empty()) {
        return;
    }
    ListNode *current = head;
    while (current != tail) {
        ListNode *next = current->next;
        delete current;
        current = next;
    }
    delete tail;
    head = tail = nullptr;
}

TTT void LoopedList<T>::remove(LoopedList::Iterator it) {
    if (it.list != this)
        throw std::invalid_argument("Iterator does not belong to this list");
    if (it.next().node == it.node) {
        head = tail = nullptr;
        delete it.node;
        return;
    }
    auto *prev = it.node->prev, *next = it.node->next;
    prev->next = next;
    next->prev = prev;
    if (it.node == head) {
        head = next;
    }
    if (it.node == tail) {
        tail = prev;
    }
    delete it.node;
}

TTT void LoopedList<T>::insertAfter(LoopedList::Iterator it, const T &value) {
    if (it.list != this)
        throw std::invalid_argument("Iterator does not belong to this list");
    auto *newNode = new ListNode{value, it.node->next, it.node};
    newNode->next->prev = newNode;
    newNode->prev->next = newNode;
    if (it.node == tail) {
        tail = newNode;
    }
}

TTT void LoopedList<T>::insertFront(const T &value) {
    if (empty()) {
        head = tail = new ListNode{value, nullptr, nullptr};
        head->next = head->prev = head;
    } else {
        auto *newNode = new ListNode{value, head, tail};
        head->prev = newNode;
        tail->next = newNode;
        head = newNode;
    }
}

TTT void LoopedList<T>::insertBack(const T &value) {
    if (empty()) {
        insertFront(value);
    } else {
        insertAfter(end(), value);
    }
}

#undef TTT

#endif //JOSEPHUS_LOOPEDLIST_HPP
