#include <iostream>
#include <numeric>
#include <string>
#include <regex>

using namespace std;

int main() {
    // Make a regex
    const regex r(R"((don't|do|mul\((\d{1,3}),(\d{1,3})\)))");

    int sum = 0;
    string str;
    bool enabled = true;
    while (getline(cin, str)) {
        sum = accumulate(sregex_iterator(str.begin(), str.end(), r), sregex_iterator(), sum,
                         [&enabled](int acc, const smatch& m) {
                             if (m[0] == "don't") {
                                 enabled = false;
                                 return acc;
                             }

                             if (m[0] == "do") {
                                 enabled = true;
                                 return acc;
                             }

                             if (!enabled) {
                                 return acc;
                             }

                             return acc + stoi(m[2]) * stoi(m[3]);
                         });
    }

    cout << sum << endl;

    return 0;
}
