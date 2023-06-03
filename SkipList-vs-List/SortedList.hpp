#ifndef COD_SD_SORTEDLIST_HPP
#define COD_SD_SORTEDLIST_HPP

#include <concepts>
#include <optional>
#include <stdexcept>

template <typename T>
requires std::totally_ordered<T>
class SortedList {
public:
    using size_t = std::size_t;
    class Iterator;
private:
    struct ListNode {
        ListNode *prev, *next;
        std::optional<T> value;
        Iterator iter() {
            return Iterator(this);
        }
    };
    ListNode *head, *tail;
    size_t length;
public:
    class Iterator {
    private:
        ListNode *node;
        explicit Iterator(ListNode* node) : node(node) {}
    public:
        Iterator(const Iterator&) = default;
        Iterator &operator=(const Iterator&) = default;
        [[nodiscard]] Iterator next() const {
            return Iterator(node->next);
        }
        [[nodiscard]] [[maybe_unused]] Iterator prev() const {
            return Iterator(node->prev);
        }
        friend bool operator==(const Iterator&, const Iterator&) = default;
        friend bool operator!=(const Iterator&, const Iterator&) = default;
        const T &operator*() const {
            return node->value.value();
        }
        const T &operator->() const {
            return node->value.value();
        }
        Iterator &operator++() {
            node = node->next;
            return *this;
        }
        friend class SortedList;
    };

    Iterator find(const T& value) {
        ListNode *it = head->next;
        while(it != tail && it->value < value) {
            it = it->next;
        }
        if(it == tail || it->value != value) return end();
        return it->iter();
    }

    void erase(Iterator it) {
        if(it.node == tail) throw std::out_of_range("Cannot remove past-the-end element");
        if(it.node == head) throw std::out_of_range("Cannot remove before-the-start element");
        ListNode *node = it.node;
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
        length--;
    }

    void insert(const T& value) {
        ListNode *ins_before = head->next;
        while(ins_before != tail && ins_before->value < value) {
            ins_before = ins_before->next;
        }
        auto *node = new ListNode{ins_before->prev, ins_before, value};
        auto prev = ins_before->prev, next = ins_before;
        prev->next = node;
        next->prev = node;
        length++;
    }

    [[nodiscard]] Iterator begin() const {
        return head->next->iter();
    }
    [[nodiscard]] Iterator end() const {
        return tail->iter();
    }
    [[nodiscard]] [[maybe_unused]] unsigned int getLength() const {
        return length;
    }

    SortedList() : head(new ListNode()), tail(new ListNode()), length(0) {
        head->next = tail;
        tail->prev = head;
    }

    SortedList(const SortedList&) = delete;
    SortedList &operator=(const SortedList&) = delete;

    ~SortedList() {
        ListNode *it = head;
        while(it) {
            ListNode *next = it->next;
            delete it;
            it = next;
        }
    }
};

#endif //COD_SD_SORTEDLIST_HPP
