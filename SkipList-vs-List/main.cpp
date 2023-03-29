#include <iostream>
#include "SkipList.h"

void print_list(SkipList<int> &list) {
    for(auto iter = list.begin();!iter.isNull();iter = iter.next(0)) {
        std::cout<<iter.value()<<' ';
    }
    std::cout<<std::endl;
}

int main() {
    SkipList<int> testlist;
    testlist.insert(4);
    print_list(testlist);
    testlist.insert(1);
    print_list(testlist);
    testlist.insert(2);
    print_list(testlist);
    testlist.insert(3);
    print_list(testlist);
    testlist.remove(testlist.find(2));
    print_list(testlist);
    testlist.remove(testlist.find(1));
    print_list(testlist);
}
