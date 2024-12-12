#include <iostream>
#include <numeric>
#include <string>
#include <regex>

using namespace std;

int main() {
    unordered_map<string, function<int(const string&, const string&)>> ops = {
        {"mul", [](const string& a, const string& b) { return stoi(a) * stoi(b); }},
    };

    // Make a regex
    regex r(R"(mul\((\d{1,3}),(\d{1,3})\))");

    int sum = 0;
    string str;
    while (getline(cin, str)) {
        sum = accumulate(sregex_iterator(str.begin(), str.end(), r), sregex_iterator(), sum,
                         [&](int acc, const smatch& m) {
                             return acc + ops["mul"](m[1], m[2]);
                         });
    }

    cout << sum << endl;

    return 0;
}
