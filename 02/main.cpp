#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <ranges>
#include <vector>
#include <range/v3/all.hpp>

using namespace std;

enum class ReportDirection {
    Unknown,
    Increase,
    Decrease,
};

typedef vector<int> report_t;

bool is_safe(auto report) {
    ReportDirection direction = ReportDirection::Unknown;

    auto it = std::ranges::adjacent_find(report, [&](int n1, int n2) {
        auto is_unsafe_pair = [](int a, int b, const ReportDirection dir) {
            return abs(a - b) < 1 || abs(a - b) > 3 ||
                a > b && dir == ReportDirection::Increase ||
                a < b && dir == ReportDirection::Decrease;
        };

        if (is_unsafe_pair(n1, n2, direction)) {
            return true;
        }

        if (direction == ReportDirection::Unknown) {
            direction = n1 < n2 ? ReportDirection::Increase
                                : ReportDirection::Decrease;
        }

        return false;
    });

    return it == report.end();
}

int main() {
    string line;

    vector<vector<int>> reports;
    while (getline(cin, line)) {
        stringstream ss(line);
        reports.emplace_back((istream_iterator<int>(ss)), istream_iterator<int>());
    }
    
    auto [safe_reports_no_skip, safe_reports_skip] = accumulate(
            reports.begin(), reports.end(), make_pair(0, 0),
            [](auto sum, const report_t &r) {
                for (int n : r) cout << n << " ";
                // Output boolalpha to print bool values as "true" or "false"
                const bool is_safe_no_skip = is_safe(r);
                cout << boolalpha << is_safe_no_skip << "\t";

                bool is_safe_w_skip = false;
                if (!is_safe_no_skip) {
                    for (int i = 0; i < r.size(); i++) {
                        is_safe_w_skip = is_safe(::ranges::views::concat(
                                r | ::ranges::views::take(i),
                                r | ::ranges::views::drop(i + 1)));

                        if (is_safe_w_skip) {
                            break;
                        }
                    }
                }

                cout << endl;

                return make_pair(sum.first + is_safe_no_skip, sum.second + (is_safe_w_skip || is_safe_no_skip));
            });

    cout << "Safe reports no skips: " << safe_reports_no_skip << endl;
    cout << "Safe reports skips: " << safe_reports_skip << endl;

    return 0;
}
