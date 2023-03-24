#ifndef JOSEPHUS_LINKEDLIST_H
#define JOSEPHUS_LINKEDLIST_H

template <typename T>
class LinkedList {
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
        LinkedList<T> *list;
        Iterator(ListNode *node, LinkedList<T> *list) : node(node), list(list) {} // only the list makes these
    public:
        Iterator next();
        Iterator prev();
        T &value();
        bool operator==(Iterator const &other) const;        friend class LinkedList;
    };

    void remove(Iterator it);
    void clear();
    void insertFront(T const &value);
    void insertAfter(Iterator it, const T &value);
    void insertBack(T const &value);

    Iterator begin();
    Iterator end();
    bool empty();

    LinkedList();
    ~LinkedList();

};

#endif //JOSEPHUS_LINKEDLIST_H
