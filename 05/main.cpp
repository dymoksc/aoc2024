#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <range/v3/all.hpp>

using namespace std;

int main() {
    map<int, vector<int>> precedence_list;

    // Filling adjacency list
    {
        string s;
        while (getline(cin, s) && !s.empty()) {
            ::ranges::transform_view res = s | ::ranges::views::split('|') |
                ::ranges::views::transform([](auto &&i) {
                    return stoi(::ranges::to<string>(i));
                });

            assert(std::ranges::distance(res) == 2);

            int from = *res.begin();
            int to = *(res | ::ranges::views::drop(1)).begin();

            if (!precedence_list.contains(from)) precedence_list[from] = {};
            if (!precedence_list.contains(to)) precedence_list[to] = {};
            precedence_list[from].push_back(to);
        }
    }

    // Reading numbers
    {
        string s;
        int correct_middle_sum = 0;
        int corrected_middle_sum = 0;
        while (getline(cin, s)) {
            vector<int> visited;
            optional<int> last;
            auto res = s |
                ::ranges::views::split(',') |
                ::ranges::views::transform([](auto &&i) {
                    return stoi(::ranges::to<string>(i));
                });

            assert(::ranges::distance(res) % 2 == 1);

            auto vector1 = ::ranges::to<vector<int>>(res);

            bool found_incorrect = false;
            for (int i = 0; i < vector1.size() - 1; i++) {
                for (int j = i + 1; j < vector1.size(); j++) {
                    if (::ranges::count(precedence_list[vector1[j]], vector1[i]) == 1) {
                        found_incorrect = true;
                        break;
                    }
                }
                if (found_incorrect) break;
            }

            if (!found_incorrect) {
                int middle_num = *(res | ::ranges::views::drop(::ranges::distance(res) / 2) | ::ranges::views::take(1)).begin();
                correct_middle_sum += middle_num;
            } else {
                ::ranges::sort(vector1, [&](int a, int b) {
                    bool a_should_precede = ::ranges::contains(precedence_list[a], b);
                    bool b_should_precede = ::ranges::contains(precedence_list[b], a);

                    assert(!(a_should_precede || b_should_precede) || a_should_precede != b_should_precede);

                    if (a_should_precede) return true;
                    return false;
                });

                corrected_middle_sum += vector1[vector1.size() / 2];
            }
        }

        cout << "Correct middle sum: " << correct_middle_sum << endl;
        cout << "Corrected middle sum: " << corrected_middle_sum << endl;
    }


    return 0;
}
