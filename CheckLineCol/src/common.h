#ifndef CHECKLINECOL_COMMON_H
#define CHECKLINECOL_COMMON_H

#include <vector>
#include <optional>
struct Point {
    double x, y;
    inline friend bool operator==(const Point& lhs, const Point& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};
struct Line {
    Point start, end;
    inline friend bool operator==(const Line& lhs, const Line& rhs) {
        return lhs.start == rhs.start && lhs.end == rhs.end;
    }
};
struct Collision {
    Line line1, line2;
    inline friend bool operator==(const Collision& lhs, const Collision& rhs) {
        return (lhs.line1 == rhs.line1 && lhs.line2 == rhs.line2)
            || (lhs.line1 == rhs.line2 && lhs.line2 == rhs.line1);
    }
};
typedef std::optional<Collision> OptCol;

bool intersect(const Line&, const Line&);
OptCol check_col_bruteforce(const std::vector<Line>&);
OptCol check_col_sweep(const std::vector<Line>&);
std::vector<Line> generate_testcase(std::size_t nr_points);

#endif //CHECKLINECOL_COMMON_H
