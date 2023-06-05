#include "common.h"

OptCol check_col_bruteforce(const std::vector<Line>& lines) {
    for(std::size_t x = 0;x<lines.size();x++){
        for(std::size_t y = x + 1;y<lines.size();y++) {
            if(intersect(lines[x], lines[y]))
                return std::make_optional<Collision>({lines[x], lines[y]});
        }
    }
    return std::nullopt;
}
