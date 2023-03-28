#ifndef SKIPLISTVSLIST_SKIPLIST_TPP
#define SKIPLISTVSLIST_SKIPLIST_TPP

#define TTT template <typename T>
#define SL SkipList<T>
#define SLI SL::Iterator

#include <limits>

TTT SLI SL::find(T const &value) {
    if(!length)
        throw std::out_of_range("List is empty");
    if(head->value > value)
        return {head, this};
    unsigned int max_skip = std::numeric_limits<unsigned int>::max();
    auto ptr = head;
    while(max_skip && ptr->value != value) {
        auto next_skip = std::min(max_skip, ptr->levels);
        auto next_ptr = ptr->skips[next_skip - 1].next;
        if(next_ptr->value > value)
            max_skip = next_skip - 1;
        else ptr = next_ptr;
    }
    return {ptr, this};
}

TTT void SL::remove(SLI it) {
    if(it.list != this)
        throw std::invalid_argument("Iterator does come from this list");
    auto node = it.node;
    if(node->skips[0].prev != nullptr) {
        for(int i = 0;i<node->levels;i++)
            node->skips[i].prev->skips[i].next = node->skips[i].next;
    }
    if(node->skips[0].next != nullptr) {
        for(int i = 0;i<node->levels;i++)
            node->skips[i].next->skips[i].prev = node->skips[i].prev;
    }
    if(node == head)
        head = head->skips[0].next;
    if(node == tail)
        tail = tail->skips[0].prev;
    delete node;
    length--;
}

TTT void SL::clear() {
    auto node = head;
    while(node) {
        auto next = node->skips[0].next;
        delete node;
        node = next;
    }
    length = 0;
}

TTT void SL::insert(T const &value) {
    ///TODO
}

TTT SLI SL::begin() {return {head, this};}
TTT SLI SL::end() {return {tail, this};}
TTT unsigned int SL::getLength() {return length;}

TTT SL::SkipList() : head(nullptr), tail(nullptr), length(0) {}
TTT SL::~SkipList() {
    clear();
}

#undef TTT
#undef SL
#undef SLI

#endif //SKIPLISTVSLIST_SKIPLIST_TPP
