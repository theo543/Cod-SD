#ifndef JOSEPHUS_LOOPEDLIST_H
#define JOSEPHUS_LOOPEDLIST_H

template <typename T>
class LoopedList {
private:
    struct ListNode {
        T value;
        ListNode *next, *prev;
    };
    ListNode *head, *tail;
public:

    class Iterator {
    private:
        ListNode *node;
        LoopedList<T> *list;
        Iterator(ListNode *node, LoopedList<T> *list) : node(node), list(list) {} // only the list makes these
    public:
        [[nodiscard]] Iterator next();
        [[nodiscard]] [[maybe_unused]] Iterator prev();
        [[nodiscard]] T &value();
        bool operator==(Iterator const &other) const;
        friend class LoopedList;
    };

    void remove(Iterator it);
    void clear();
    void insertFront(T const &value);
    void insertAfter(Iterator it, const T &value);
    void insertBack(T const &value);

    [[nodiscard]] Iterator begin();
    [[nodiscard]] Iterator end();
    [[nodiscard]] bool empty();

    LoopedList();
    ~LoopedList();

};

#include "LoopedList.tpp"

#endif //JOSEPHUS_LOOPEDLIST_H
