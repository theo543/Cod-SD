#include <iostream>
#include <random>
#include "SkipList.hpp"

void print_list(SkipList<int> &list) {
    for(auto iter = list.begin();iter != list.end();iter = iter.next()) {
        std::cout<<*iter<<' ';
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
