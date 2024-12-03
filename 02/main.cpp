#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <ranges>
#include <vector>

using namespace std;

enum class ReportDirection {
    Unknown,
    Increase,
    Decrease,
};

typedef vector<int> report_t;

bool is_safe(const report_t & report) {
    ReportDirection direction = ReportDirection::Unknown;
    auto it = ranges::adjacent_find(report, [&](int n1, int n2) {
        if (abs(n1 - n2) < 1 || abs(n1 - n2) > 3 ||
            n1 > n2 && direction == ReportDirection::Increase ||
            n1 < n2 && direction == ReportDirection::Decrease) {
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

    int safe_reports = accumulate(reports.begin(), reports.end(), 0,
                                  [](int sum, const report_t &r) {
                                      return sum + is_safe(r);
                                  });

    cout << "Safe reports: " << safe_reports << endl;

    // 624

    return 0;
}
