#include "common.h"
#include <set>
#include <algorithm>
#include <cassert>

namespace {
    const auto sweepCompareLines = [](const Line &a, const Line &b) {
        return a.start.y < b.start.y;
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
        return P1.x < P2.x || (P1.x == P2.x && P1.y < P2.y);
    });
    for(const auto &stop : stops) {
        const auto &line = stop.l;
        if(stop.is_begin()) {
            auto it = sset.insert(line).first;
            assert(it != sset.end());
            auto above = std::next(it), below = it == sset.begin() ? sset.end() : std::prev(it);
            above++;
            if(above != sset.end() && intersect(*above, *it))
                return Collision{*above, *it};
            if(below != sset.end() && intersect(*below, *it))
                return Collision{*below, *it};
        } else {
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
