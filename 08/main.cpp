#include <functional>
#include <iostream>
#include <set>
#include <range/v3/all.hpp>

using namespace std;
namespace rr = ::ranges;
namespace rv = ::ranges::views;

typedef pair<int, int> coord;

map<char, vector<coord>> read_coord_by_freq(istream& is, int & i, int & max_j) {
    map<char, vector<coord>> coord_by_freq;
    int j = 0;
    char c;
    while (is.get(c)) {
        if (c == '\n') {
            ++i;
            max_j = j;
            j = 0;
            continue;
        }

        if (c != '.') {
            assert(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9');
            if (!coord_by_freq.contains(c)) coord_by_freq[c] = {};
            coord_by_freq[c].emplace_back(i, j);
        }

        ++j;
    }

    return coord_by_freq;
}

coord operator-(const coord& lhs, const coord& rhs) {
    return {lhs.first - rhs.first, lhs.second - rhs.second};
}

coord operator+(const coord& lhs, const coord& rhs) {
    return {lhs.first + rhs.first, lhs.second + rhs.second};
}

int main() {
    int x = 0, y = 0;
    map<char, vector<coord>> coord_by_freq = read_coord_by_freq(cin, y, x);
    set<coord> valid_antinodes;

    for (const auto& [c, coords] : coord_by_freq) {
        for (int i = 0; i < coords.size(); ++i) {
            for (int j = i + 1; j < coords.size(); ++j) {
                auto vect = coords[i] - coords[j];
                auto anti_vect = coord{-vect.first, -vect.second};

                for (auto init_coord : {coords[i], coords[j]}) {
                    for (auto v : {vect, anti_vect}) {
                        coord node = init_coord + v;
                        while (node.first >= 0 && node.first < y && node.second >= 0 && node.second < x) {
                            valid_antinodes.insert(node);
                            node = node + v;
                        }
                    }
                }
            }
        }
    }

    cout << "Valid antinodes: " << valid_antinodes.size() << endl;

    return 0;
}
