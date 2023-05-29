#include <iostream>
#include <fmt/format.h>
#include "common.h"
#include "color.hpp"

std::string col_string(OptCol opt) {
    if(opt.has_value()) {
        auto v = opt.value();
        return fmt::format("Collision at lines ({}, {})->({}, {}) and ({}, {})->({}, {})",
                   v.line1.start.x, v.line1.start.y, v.line1.end.x, v.line1.end.y,
                   v.line2.start.x, v.line2.start.y, v.line2.end.x, v.line2.end.y);
    } else {
        return "No collision";
    }
}

int main() {
    while(true) {
        color("blue", "How many points? (0 = exit)", true);
        std::size_t points;
        std::cin >> points;
        if(!points) break;
        std::vector<Line> testcase = generate_testcase(points);
        auto naive = check_col_bruteforce(testcase);
        auto smart = check_col_sweep(testcase);
        if(naive.has_value() != smart.has_value()) {
            color("red", "Algorithms disagree!", true);
            color("red", fmt::format("Naive says:{}\nSmart says:[]\n",
                  col_string(naive), col_string(smart)), false);
        } else {
            color("green", "Algorithms agree.");
            if(!naive.has_value())
                std::cout << "No collision.\n";
            else if(naive.value() == smart.value()) {
                std::cout << "Algorithms returned same collision.\n";
            } else {
                std::cout << "Algorithms returned different collisions.\n";
            }
        }
    }
    return 0;
}
