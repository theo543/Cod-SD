#include <iostream>
#include "SkipList.h"

int main() {
    SkipList<int> testlist;
    testlist.insert(1);
    testlist.insert(2);
    testlist.insert(3);
    testlist.insert(4);
    testlist.remove(testlist.find(2));
    testlist.remove(testlist.find(1));
    std::cout << testlist.begin().value() << std::endl;
}
