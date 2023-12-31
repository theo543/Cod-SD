#include <random>
#include "common.h"

namespace {
    Point vector(const Point &a, const Point &b) {
        return {b.x - a.x, b.y - a.y};
    }
    Point vector(const Line &l) {
        return vector(l.start, l.end);
    }
    double vec_prod(const Point &a, const Point &b) {
        return a.x * b.y - a.y * b.x;
    }
    bool different_sides(const Line &line, const Point &p3, const Point &p4) {
        const auto &p1 = line.start, &p2 = line.end;
        auto turn_p3 = vec_prod(vector(p1, p2), vector(p1, p3));
        auto turn_p4 = vec_prod(vector(p1, p2), vector(p1, p4));
        return (turn_p3 > 0 && turn_p4 < 0) || (turn_p3 < 0 && turn_p4 > 0);
    }
    struct AABB {
        Point start, end;
    };
    AABB calc_AABB(const Line &l) {
        Point tl = {std::min(l.start.x, l.end.x), std::max(l.start.y, l.end.y)};
        Point br = {std::max(l.start.x, l.end.x), std::max(l.start.y, l.end.y)};
        return {tl, br};
    }
    bool AABB_collide(const AABB &a, const AABB &b) {
        bool xAxis = a.start.x <= b.end.x && b.start.x <= a.end.x;
        bool yAxis = a.start.y <= b.end.y && b.start.y <= a.end.y;
        return xAxis && yAxis;
    }
}

bool intersect(const Line &a, const Line &b) {
    if(!AABB_collide(calc_AABB(a), calc_AABB(b)))
        return false;
    // check if any endpoints touch
    if(a.start == b.start || a.start == b.end || a.end == b.start || a.end == b.end)
        return true;
    return different_sides(a, b.start, b.end) || different_sides(b, a.start, a.end);
}

std::vector<Line> generate_testcase(std::size_t nr_lines) {
    std::mt19937 rand {std::random_device{}()};
    std::uniform_real_distribution<double> dist {0, 100};
    std::vector<Line> lines;
    for(std::size_t i = 0;i<nr_lines;i++) {
        lines.push_back({{dist(rand), dist(rand)}, {dist(rand), dist(rand)}});
    }
    auto &last = lines.back();
    lines.push_back({{last.end.x, last.start.y}, {last.start.x, last.end.y}});
    return lines;
}
