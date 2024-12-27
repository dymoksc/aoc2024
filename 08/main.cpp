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
                auto distance = coords[i] - coords[j];
                coord node1 = coords[i] + distance;
                coord node2 = coords[j] - distance;

                if (node1.first >= 0 && node1.first < y && node1.second >= 0 && node1.second < x) {
                    valid_antinodes.insert(node1);
                }
                if (node2.first >= 0 && node2.first < y && node2.second >= 0 && node2.second < x) {
                    valid_antinodes.insert(node2);
                }
            }
        }
    }

    cout << "Valid antinodes:" << valid_antinodes.size() << endl;

    return 0;
}
