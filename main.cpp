#include <iostream>
#include "LoopedList.h"

inline auto josephus_formula(unsigned int n) {
    unsigned int highestBit = 1;
    while (highestBit <= n) {
        highestBit <<= 1;
    }
    highestBit >>= 1;
    return 2 * (n - highestBit) + 1;
}

int main() {
    std::cout << "Enter non-unsigned int value to exit" << std::endl;
    while(true) {
        LoopedList<unsigned int> testlist;
        unsigned int nr;
        {
            std::string input;
            getline(std::cin, input);
            try {
                if(input.find('-') != std::string::npos)
                    throw std::invalid_argument("Minus sign found");
                nr = std::stoul(input);
            } catch (std::invalid_argument &e) {
                break;
            } catch (std::out_of_range &e) {
                std::cout<<"Number out of range, try again"<<std::endl;
                continue;
            }
        }
        for (int i = 1; i <= nr; ++i) {
            testlist.insertBack(i);
        }
        auto it = testlist.begin();
        bool delNext = false;
        while (true) {
            if (delNext) {
                auto tmp = it.next();
                testlist.remove(it);
                it = tmp;
            } else {
                it = it.next();
            }
            delNext = !delNext;
            if (it.next() == it)
                break;
        }
        std::cout << "Looped List solution is: " << it.value() << std::endl;
        std::cout << "Formula solution is: " << josephus_formula(nr) << std::endl;
    }
    std::cout << "Exiting..." << std::endl;
    return 0;
}
