#ifndef SKIPLISTVSLIST_SKIPLIST_H
#define SKIPLISTVSLIST_SKIPLIST_H

#include <vector>
#include <optional>
#include <cassert>
#include <random>
#include <algorithm>
#include <iosfwd>
#include <concepts>

/**
 * Skip List invariant:
 * 1. There is always a before-the-start and past-the-end element (head and tail)
 * 2. The elements are sorted
 * 3. The amount of levels is always at least = log2(length)
 * 4. A ListNode has prev and next pointers for each level it is on.
 * 5. The head and tail elements are on all levels
 * 6. The probability of an element being on a level is 1/2^(level_nr)
 * 7. Every node except the head and tail has a value
 * @tparam T The type of the values to store
 */
template <typename T>
requires std::totally_ordered<T>
class SkipList {
public:
    using size_t = std::size_t;
    class Iterator;
private:
    std::mt19937_64 rng;
    std::uniform_int_distribution<size_t> dist;
    struct ListNode;
    struct NodePair {
        ListNode *prev, *next;
        explicit NodePair(ListNode *prev = nullptr, ListNode *next = nullptr) : prev(prev), next(next) {}
    };
    struct ListNode {
        std::optional<T> value;
        std::vector<NodePair> links;
        Iterator iter() {
            return Iterator(this);
        }
    };
    ListNode *head, *tail;
    size_t length, levels;
    void grow() {
        auto *prev_top = head->links[levels - 1].next;
        head->links.emplace_back(NodePair{nullptr, nullptr});
        auto *top = head;
        while(prev_top != tail) {
            if(dist(rng)) {
                prev_top->links.emplace_back(NodePair{top, nullptr});
                top->links.back().next = prev_top;
                top = prev_top;
            }
            prev_top = prev_top->links[levels - 1].next;
        }
        top->links.back().next = tail;
        tail->links.emplace_back(NodePair{top, nullptr});
        levels++;
    }
public:
    class Iterator {
    private:
        ListNode *node;
        explicit Iterator(ListNode* node) : node(node) {}
    public:
        Iterator(const Iterator&) = default;
        Iterator &operator=(const Iterator&) = default;
        [[nodiscard]] Iterator next() const {
            return Iterator(node->links[0].next);
        }
        [[nodiscard]] [[maybe_unused]] Iterator prev() const {
            return Iterator(node->links[0].prev);
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
            node = node->links[0].next;
            return *this;
        }
        friend class SkipList;
    };

    Iterator find(const T& value) {
        ListNode *it = head;
        size_t current_level = levels - 1;
        while(true) {
            // since we arrived here by a path of nodes with level >= current_level, this link should exist
            assert(it->links.size() > current_level);
            ListNode *next = it->links[current_level].next;
            if(next == tail || next->value >= value) {
                if(current_level == 0)
                    break;
                current_level--;
            } else {
                it = next;
            }
        }
        // we will arrive right before the first element >= value
        auto result = it->links[0].next;
        if(result == tail || result->value != value)
            return end();
        return result->iter();
    }

    void erase(Iterator it) {
        if(it.node == tail) throw std::out_of_range("Cannot remove past-the-end element");
        if(it.node == head) throw std::out_of_range("Cannot remove before-the-start element");
        ListNode *node = it.node;
        for(size_t level = 0;level < node->links.size();level++) {
            ListNode *prev = node->links[level].prev;
            ListNode *next = node->links[level].next;
            prev->links[level].next = next;
            next->links[level].prev = prev;
        }
        delete node;
    }

    void insert(const T& value) {
        ListNode *it = head;
        // the links the new element would have if it was on all levels
        std::vector<ListNode*> links_all_levels;
        // the level whose link we are currently finding
        size_t current_level = levels - 1;
        // we do almost the same thing as in find, but we need to find the links of the new element
        while(true) {
            assert(it->links.size() > current_level);
            ListNode *next = it->links[current_level].next;
            if(next == tail || next->value >= value) {
                // we are at the first element of this level >= value
                // the node we insert will be at the first position < value, so it will link to here
                links_all_levels.push_back(next);
                if(current_level == 0)
                    break;
                current_level--;
            } else {
                it = next;
            }
        }
        // since current_level decrements one at a time we must have collected all links
        assert(links_all_levels.size() == levels);
        std::reverse(links_all_levels.begin(), links_all_levels.end());

        auto new_node = new ListNode();
        new_node->value = value;

        size_t new_node_levels = 0;
        while(dist(rng) && new_node_levels != (levels - 1)) {
            new_node_levels++;
        }
        new_node->links.resize(new_node_levels + 1);

        for(size_t x = 0;x<=new_node_levels;x++) {
            auto next_node = links_all_levels[x], prev_node = next_node->links[x].prev;
            new_node->links[x] = {NodePair{prev_node, next_node}};
            prev_node->links[x].next = new_node;
            next_node->links[x].prev = new_node;
        }

        length++;
        size_t minimum_levels = std::log2(length + 1) + 1;
        while(levels < minimum_levels) {
            grow();
        }
    }

    [[nodiscard]] Iterator begin() const {
        return head->links[0].next->iter();
    }
    [[nodiscard]] Iterator end() const {
        return tail->iter();
    }
    [[nodiscard]] [[maybe_unused]] unsigned int getLength() const {
        return length;
    }

    SkipList() : rng(std::random_device()()), dist(0, 1) {
        length = 0;
        levels = 1;
        head = new ListNode();
        tail = new ListNode();
        head->links = {NodePair{nullptr, tail}};
        tail->links = {NodePair{head, nullptr}};
    }

    SkipList(const SkipList&) = delete;
    SkipList &operator=(const SkipList&) = delete;

    ~SkipList() {
        auto *node = head;
        while(node != nullptr) {
            auto *next = node->links[0].next;
            delete node;
            node = next;
        }
    }
};

#endif //SKIPLISTVSLIST_SKIPLIST_H
