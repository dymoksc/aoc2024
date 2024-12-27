#include <functional>
#include <iostream>
#include <string>
#include <range/v3/all.hpp>

using namespace std;
namespace rr = ::ranges;
namespace rv = ::ranges::views;

typedef unsigned long int_t;
auto stoit = [](const string & s) { return stoul(s); };

struct problem_t {
    int_t result;
    vector<int_t> operands;
};

vector<problem_t> read_problems(istream & is) {
    vector<problem_t> problems;
    string s;
    while (getline(is, s)) {
        vector<string> res = s | rv::split(':') | rr::to<vector<string>>();
        assert(res.size() == 2);
        problems.emplace_back(
            stoit(res[0]),
            res[1] |
                rv::split(' ') |
                rv::filter([](auto&& i) { return !i.empty(); }) |
                rv::transform([](auto&& i) { return stoit(rr::to<string>(i)); }) |
                rr::to<vector<int_t>>());
    }
    return problems;
}

int_t concatenates(int_t a, int_t b) {
    return stoit(to_string(a) + to_string(b));
}

const vector<function<int_t(int_t, int_t)>> ops = {plus<>(), multiplies<>(), concatenates};

bool solve(auto numbers, int_t result_so_far, int_t required_result) {
    if (rr::distance(numbers) == 0) {
        return result_so_far == required_result;
    }

    if (result_so_far > required_result) {
        return false;
    }

    for (auto & op : ops) {
        if (solve(numbers | rv::drop(1), op(result_so_far, *numbers.begin()), required_result)) {
            return true;
        }
    }

    return false;
}

int main() {
    vector<problem_t> problems = read_problems(cin);
    int_t sum_of_results = 0;
    for (problem_t & problem : problems) {
        bool can_solve = solve(problem.operands | rv::drop(1), problem.operands[0], problem.result);
        if (can_solve) {
            sum_of_results += problem.result;
        }

        // cout << "Required result: " << problem.result << endl;
        // cout << "Operands: ";
        // for (int_t operand : problem.operands) {
            // cout << operand << " ";
        // }
        // cout << endl;
        // cout << "Can solve: " << (can_solve ? "yes" : "no") << endl;
    }

    cout << "Result: " << sum_of_results << endl;
}
