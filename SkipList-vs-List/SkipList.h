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
        unsigned int skipLen = 0;
        std::unique_ptr<NodePair[]> skips;
        inline ListNode(T const &value, unsigned int skipLen) : value(value), skipLen(skipLen), skips(std::make_unique<NodePair[]>(skipLen)) {}
        inline ListNode(T const &&value, unsigned int skipLen) : value(std::move(value)), skipLen(skipLen), skips(std::make_unique<NodePair[]>(skipLen)) {}
    };
    ListNode *head, *tail;
public:

    class Iterator {
    private:
        ListNode *node = nullptr;
        SkipList<T> *list = nullptr;
        Iterator() = default;
        Iterator(ListNode *node, SkipList<T> *list) : node(node), list(list) {} // only the list makes these
    public:
        [[nodiscard]] Iterator next(unsigned int skips);
        [[nodiscard]] [[maybe_unused]] Iterator prev(unsigned int skips);
        [[nodiscard]] unsigned int getSkips() const;
        [[nodiscard]] const T &value();
        bool operator==(Iterator const &other) const;
        friend class SkipList;
    };

    Iterator find(T const &value);
    void remove(Iterator it);
    void clear();
    void insert(T const &value);

    [[nodiscard]] Iterator begin();
    [[nodiscard]] Iterator end();
    [[nodiscard]] bool empty();

    SkipList();
    ~SkipList();

};

#include "SkipList.tpp"

#endif //SKIPLISTVSLIST_SKIPLIST_H
