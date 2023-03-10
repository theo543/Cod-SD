#include <iostream>
#include "LoopedList.h"

int main() {
    int len, counter, counter_max;
    std::cin >> len >> counter >> counter_max;
    LoopedList list{};
    for (int k = 1; k <= len; ++k) {
        list.insertTail(k);
    }
    Node *iter = list.getHead();
    while (list.getHead() != list.getTail()) {
        std::cout << "Current:" << iter->getData() << std::endl;
        if (counter == counter_max) {
            Node *to_delete = iter;
            iter = iter->getNext();
            list.remove(to_delete);
            counter = 1;
        } else {
            counter++;
            iter = iter->getNext();
        }
    }
    std::cout << iter->getData() << std::endl;
    return 0;
}
