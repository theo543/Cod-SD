#ifndef SKIPLISTVSLIST_SKIPLIST_H
#define SKIPLISTVSLIST_SKIPLIST_H

#include <memory>

template <typename T>
class SkipList {
private:
    struct ListNode {
        struct NodePair {
            ListNode *prev, *next;
        };
        T value;
        unsigned int levels = 0;
        std::unique_ptr<NodePair[]> skips;
        inline explicit ListNode(T const &value, unsigned int levels) : value(value), levels(levels), skips(std::make_unique<NodePair[]>(levels)) {}
        inline explicit ListNode(T const &&value, unsigned int levels) : value(std::move(value)), levels(levels), skips(std::make_unique<NodePair[]>(levels)) {}
    };
    ListNode *head, *tail;
    unsigned int length = 0;
public:

    class Iterator {
    private:
        ListNode *node = nullptr;
        SkipList<T> *list = nullptr;
        Iterator(ListNode *node, SkipList<T> *list) : node(node), list(list) {} // only the list makes these
    public:
        [[nodiscard]] inline Iterator next(unsigned int level){return Iterator{this->node->skips[level].next, this->list};}
        [[nodiscard]] [[maybe_unused]] inline Iterator prev(unsigned int level){return {this->node->skips[level].prev, this->list};}
        [[nodiscard]] inline unsigned int getLevels() const{return this->node->levels;}
        [[nodiscard]] inline const T &value(){return this->node->value;}
        bool operator==(Iterator const &other) const{return this->node == other->node;}
        bool isNull() {return node == nullptr;}
        friend class SkipList;
    };

    Iterator lower_bound(T const &value);
    Iterator find(T const &value);

    void remove(Iterator it);
    void clear();
    void insert(T const &value);

    [[nodiscard]] Iterator begin();
    [[nodiscard]] Iterator end();
    [[nodiscard]] [[maybe_unused]] unsigned int getLength();

    SkipList();
    ~SkipList();

};

#include "SkipList.tpp"

#endif //SKIPLISTVSLIST_SKIPLIST_H
