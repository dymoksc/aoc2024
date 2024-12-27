#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <cassert>
#include <numeric>

using namespace std;

using field_t = vector<vector<char>>;
using guard_t = pair<pair<int, int>, char>;

field_t read_field() {
    string s;
    field_t field;
    while (cin >> s) {
        field.emplace_back(s.begin(), s.end());
    }
    return field;
}

guard_t get_guard(field_t & field) {
    for (int i = 0; i < field.size(); ++i) {
        char direction = 0;
        auto it = ranges::find_if(field[i], [&direction](char c) {
            for (char d : "^>v<") {
                if (c == d) {
                    direction = d;
                    return true;
                }
            }
            return false;
        });
        if (it != field[i].end()) {
            return {{i, it - field[i].begin()}, direction};
        }
    }
    assert(false);
}

char turn_right(char dir) {
    switch (dir) {
        case '^':
            return '>';
        case '>':
            return 'v';
        case 'v':
            return '<';
        case '<':
            return '^';
        default:
            assert(false);
    }
}

guard_t move_guard(field_t & field, guard_t guard) {
    int iterations = 0;
    pair<int, int> new_pos;
    auto [pos, dir] = guard;
    do {
        assert(iterations < 4);

        new_pos = pos;
        switch (dir) {
            case '^':
                new_pos.first--;
                break;
            case '>':
                new_pos.second++;
                break;
            case 'v':
                new_pos.first++;
                break;
            case '<':
                new_pos.second--;
                break;
            default:
                assert(false);
        }

        if (new_pos.first < 0 || new_pos.first >= field.size() || new_pos.second < 0 || new_pos.second >= field[0].size()) {
            field[pos.first][pos.second] = dir;
            throw out_of_range("Guard moved out of the field");
        }

        if (field[new_pos.first][new_pos.second] == '#') {
            dir = turn_right(dir);
            ++iterations;
        }
    } while (field[new_pos.first][new_pos.second] == '#');

    field[pos.first][pos.second] = dir;

    return {new_pos, dir};
}

int main() {
    field_t field = read_field();
    guard_t guard = get_guard(field);
    try {
        while (true) {
            guard = move_guard(field, guard);
        }
    } catch (out_of_range & e) {
        cout << "Guard moved out of the field" << endl;
    }

    for (auto & row : field) {
        for (char c : row) {
            cout << c;
        }
        cout << endl;
    }

    auto res = field | views::transform([](auto & row) {
        return ranges::count_if(row, [](char c) {
            return c == '^' || c == 'v' || c == '<' || c == '>';
        });
    });

    cout << "Answer: " << accumulate(res.begin(), res.end(), 0L) << endl;
}
