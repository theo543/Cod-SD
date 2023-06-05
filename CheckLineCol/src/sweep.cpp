#include "common.h"
#include <set>
#include <algorithm>
#include <cassert>

namespace {
    const auto sweepCompareLines = [](const Line &a, const Line &b) {
        return a.start.y < b.start.y || (a.start.y == b.start.y && a.start.x < b.start.x);
    };
    typedef std::set<Line, decltype(sweepCompareLines)> SweepSet;
    struct SweepStopPoint {
        Line l;
        const Point Line::*p;
        SweepStopPoint(const Line &l, bool is_begin) : l(l), p(is_begin ? &Line::start : &Line::end) {}
        [[nodiscard]] bool is_end() const {return !is_begin();}
        [[nodiscard]] bool is_begin() const {
            return p == &Line::start;
        }
    };
}
#include <iostream>
#include <fmt/format.h>
OptCol check_col_sweep(const std::vector<Line> &lines) {
    SweepSet sset;
    std::vector<SweepStopPoint> stops;
    for(const auto &l : lines) {
        if(l.start.x > l.end.x) {
            Line tmp = {l.end, l.start};
            stops.emplace_back(tmp, true);
            stops.emplace_back(tmp, false);
        } else {
            stops.emplace_back(l, true);
            stops.emplace_back(l, false);
        }
    }
    std::sort(stops.begin(), stops.end(), [](const SweepStopPoint &p1, const SweepStopPoint &p2) -> bool {
        const auto P1 = (p1.l.*p1.p), P2 = (p2.l.*p2.p);
        if(P1.x < P2.x)
            return true;
        else if (P1.x > P2.x)
            return false;
        else return p1.is_begin();
    });
    for(const auto &stop : stops) {
        const auto &line = stop.l;
        if(stop.is_begin()) {
//            std::cout << fmt::format("point ({}, {}) is begin of line (({}, {}), ({}, {}))", (line.*stop.p).x, (line.*stop.p).y, line.start.x, line.start.y, line.end.x, line.end.y) << std::endl;
            auto it = sset.insert(line).first;
            assert(it != sset.end());
            auto above = std::next(it), below = it == sset.begin() ? sset.end() : std::prev(it);
            if(above != sset.end() && intersect(*above, *it))
                return Collision{*above, *it};
            if(below != sset.end() && intersect(*below, *it))
                return Collision{*below, *it};
        } else {
//1            std::cout << fmt::format("point ({}, {}) is end of line (({}, {}), ({}, {}))", (line.*stop.p).x, (line.*stop.p).y, line.start.x, line.start.y, line.end.x, line.end.y) << std::endl;
            assert(stop.is_end());
            auto it = sset.find(line);
            assert(it != sset.end());
            auto above = std::next(it), below = it == sset.begin() ? sset.end() : std::prev(it);
            if(above != sset.end() && below != sset.end() && intersect(*above, *below))
                return Collision{*above, *below};
            sset.erase(it);
        }
    }
    assert(sset.empty());
    return std::nullopt;
}
