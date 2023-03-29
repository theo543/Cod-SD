#ifndef SKIPLISTVSLIST_SKIPLIST_TPP
#define SKIPLISTVSLIST_SKIPLIST_TPP

#define TTT template <typename T>
#define SL SkipList<T>
#define SLI SL::Iterator

#include <limits>

TTT SLI SL::lower_bound(T const &value) {
    if(!length)
        return {nullptr, this};
    if(value < head->value)
        return {head, this};
    if(tail->value < value)
        return {tail, this};
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

TTT SLI SL::find(T const &value) {
    auto iter = lower_bound(value);
    if(iter.isNull() || iter.value() != value)
        return {nullptr, this};
    return iter;
}

TTT void SL::remove(SLI it) {
    if(it.list != this)
        throw std::invalid_argument("Iterator does come from this list");
    if(it.node == nullptr)
        throw std::invalid_argument("Cannot remove a null pointer");
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
    //TODO placeholder
    auto node = new ListNode(value, 1);
    if(length == 0) {
        head = tail = node;
        length = 1;
        return;
    }
    auto ins_before = lower_bound(value).node;
    if(ins_before && ins_before->value < value)
        ins_before = ins_before->skips[0].next;
    if(ins_before == nullptr) {
        tail->skips[0].next = node;
        node->skips[0].prev = tail;
        tail = node;
    } else {
        auto before_node = ins_before->skips[0].prev;
        auto after_node = ins_before;
        if(before_node)
            before_node->skips[0].next = node;
        after_node->skips[0].prev = node;
        node->skips[0].prev = before_node;
        node->skips[0].next = after_node;
        if(after_node == head)
            head = node;
    }
    length++;
}

TTT SLI SL::begin() {return {head, this};}
TTT SLI SL::end() {return {tail, this};}
TTT [[maybe_unused]] unsigned int SL::getLength() {return length;}

TTT SL::SkipList() : head(nullptr), tail(nullptr), length(0) {}
TTT SL::~SkipList() {
    clear();
}

#undef TTT
#undef SL
#undef SLI

#endif //SKIPLISTVSLIST_SKIPLIST_TPP
