#include <stdexcept>
#include "LoopedList.h"

template<typename T>
bool LoopedList<T>::Iterator::operator==(LoopedList::Iterator const &other) const {
    return node == other.node && list == other.list;
}

template<typename T>
typename LoopedList<T>::Iterator LoopedList<T>::Iterator::next() {
    return LoopedList::Iterator(node->next, list);
}

template<typename T>
[[maybe_unused]] typename LoopedList<T>::Iterator LoopedList<T>::Iterator::prev() {
    return LoopedList::Iterator(node->prev, list);
}

template<typename T>
T &LoopedList<T>::Iterator::value() {
    return node->value;
}

template<typename T>
LoopedList<T>::LoopedList() {
    head = tail = nullptr;
}

template<typename T>
LoopedList<T>::~LoopedList() {
    clear();
}

template<typename T>
[[nodiscard]] typename LoopedList<T>::Iterator LoopedList<T>::begin() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    return Iterator(head, this);
}

template<typename T>
typename LoopedList<T>::Iterator LoopedList<T>::end() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    return LoopedList::Iterator(tail, this);
}

template<typename T>
bool LoopedList<T>::empty() {
    return head == nullptr;
}

template<typename T>
void LoopedList<T>::clear() {
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
void LoopedList<T>::remove(LoopedList::Iterator it) {
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
void LoopedList<T>::insertAfter(LoopedList::Iterator it, const T &value) {
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
void LoopedList<T>::insertFront(const T &value) {
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
void LoopedList<T>::insertBack(const T &value) {
    if (empty()) {
        insertFront(value);
    } else {
        insertAfter(end(), value);
    }
}
