#include <stdexcept>
#include "LinkedList.h"

template<typename T>
bool LinkedList<T>::Iterator::operator==(LinkedList::Iterator const &other) const {
    return node == other.node && list == other.list;
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::Iterator::next() {
    return LinkedList::Iterator(node->next, list);
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::Iterator::prev() {
    return LinkedList::Iterator(node->prev, list);
}

template<typename T>
T &LinkedList<T>::Iterator::value() {
    return node->value;
}

template<typename T>
LinkedList<T>::LinkedList() {
    head = tail = nullptr;
}

template<typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

template<typename T>
[[nodiscard]] typename LinkedList<T>::Iterator LinkedList<T>::begin() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    return Iterator(head, this);
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::end() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    return LinkedList::Iterator(tail, this);
}

template<typename T>
bool LinkedList<T>::empty() {
    return head == nullptr;
}

template<typename T>
void LinkedList<T>::clear() {
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

template<typename T>
void LinkedList<T>::remove(LinkedList::Iterator it) {
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

template<typename T>
void LinkedList<T>::insertAfter(LinkedList::Iterator it, const T &value) {
    if (it.list != this)
        throw std::invalid_argument("Iterator does not belong to this list");
    auto *newNode = new ListNode{value, it.node->next, it.node};
    newNode->next->prev = newNode;
    newNode->prev->next = newNode;
    if (it.node == tail) {
        tail = newNode;
    }
}

template<typename T>
void LinkedList<T>::insertFront(const T &value) {
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

template<typename T>
void LinkedList<T>::insertBack(const T &value) {
    if (empty()) {
        insertFront(value);
    } else {
        insertAfter(end(), value);
    }
}

template class LinkedList<int>; ///TODO: find some way to avoid this
