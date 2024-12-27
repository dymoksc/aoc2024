#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <cassert>
#include <map>
#include <numeric>
#include <set>

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
            *it = '.';
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

pair<int, int> operator-(const pair<int, int>& lhs, const pair<int, int>& at) {
    return {lhs.first - at.first, lhs.second - at.second};
}

pair<int, int> operator+(const pair<int, int>& lhs, const pair<int, int>& at) {
    return {lhs.first + at.first, lhs.second + at.second};
}

bool obstruction_here_will_send_to_obstruction(const field_t & field, guard_t guard) {
    map<char, pair<int, int>> directions = {
        {'^', {-1, 0}},
        {'>', {0, 1}},
        {'v', {1, 0}},
        {'<', {0, -1}}
    };

    auto guard_poisition = guard.first - directions.at(guard.second);
    char new_dir = turn_right(guard.second);

    while (guard_poisition.first >= 0 && guard_poisition.first < field.size() &&
           guard_poisition.second >= 0 && guard_poisition.second < field[0].size()) {
        if (field[guard_poisition.first][guard_poisition.second] == '#') {
            return true;
        }

        guard_poisition = guard_poisition + directions.at(new_dir);
    }

    return false;
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

    if (field[pos.first][pos.second] == dir) {
        throw logic_error("Loop detected");
    }

    field[pos.first][pos.second] = dir;

    return {new_pos, dir};
}

int main() {
    set<pair<int, int>> loop_creating_obstacles = {};

    std::vector<std::vector<char>> initial_field = read_field();
    const guard_t initial_guard = get_guard(initial_field);

    field_t field = initial_field;
    guard_t guard = initial_guard;
    try {
        while (true) {
            guard = move_guard(field, guard);

            if (loop_creating_obstacles.contains(guard.first)) {
                continue;
            }

            if (initial_field[guard.first.first][guard.first.second] != '.') {
                continue;
            }

            field_t obstacle_field = initial_field;
            obstacle_field[guard.first.first][guard.first.second] = '#';
            guard_t obstacle_guard = initial_guard;
            try {
                while (true) {
                    obstacle_guard = move_guard(obstacle_field, obstacle_guard);
                }
            } catch (const out_of_range &) {
                // Ignore
            } catch (const logic_error & e) {
                loop_creating_obstacles.insert(guard.first);
            }
        }
    } catch (const exception & e) {
        cout << e.what() << endl;
    }

    for (auto & obstacle : loop_creating_obstacles) {
        field[obstacle.first][obstacle.second] = 'O';
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

    assert(!loop_creating_obstacles.contains(initial_guard.first));

    cout << "Answer: " << accumulate(res.begin(), res.end(), 0L) << endl;
    cout << "Obstacles: " << loop_creating_obstacles.size() << endl;
}
