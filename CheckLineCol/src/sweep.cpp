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
        const Line *l;
        const Point *p;
        [[nodiscard]] bool is_end() const {return &l->end == p;}
        [[nodiscard]] bool is_begin() const {return &l->start == p;}
    };
}

OptCol check_col_sweep(const std::vector<Line> &lines) {
    SweepSet sset;
    std::vector<SweepStopPoint> stops;
    for(const auto &l : lines) {
        stops.push_back({&l, &l.start});
        stops.push_back({&l, &l.end});
    }
    std::sort(stops.begin(), stops.end(), [](const SweepStopPoint &p1, const SweepStopPoint &p2) -> bool {
        return p1.p->x < p2.p->x;
    });
    for(const auto &stop : stops) {
        const auto &line = *stop.l;
        const auto &point = *stop.p;
        if(sset.contains(*stop.l)) {
            // that line has same start-y
            const auto &other = *sset.find(*stop.l);
            return std::make_optional<Collision>({other, *stop.l});
        }
        if(stop.is_begin()) {

        } else {
            assert(stop.is_end());

        }
    }
    throw "!!!";
}
