#include <assert.h>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>

using namespace std;

struct nullbuf : std::streambuf {
    int overflow(int c) override {
        return c;
    }
};

nullbuf null_buffer;
std::ostream cnull(&null_buffer);

auto& cdebug1 = cnull;
auto& cdebug2 = cerr;

struct pattern_matcher_t {
    virtual void reset() = 0;
    virtual bool done() const = 0;
    virtual bool match(char c) const = 0;
    virtual void inc() = 0;
    virtual ostream& print(ostream& os) const = 0;
    friend ostream& operator<<(ostream& os, const pattern_matcher_t& matcher) {
        return matcher.print(os);
    }
};

template <typename it_t>
struct pattern_matcher_impl : pattern_matcher_t {
    const it_t begin_;
    const it_t end_;
    it_t it_;

    pattern_matcher_impl(it_t&& begin, it_t&& end) : begin_(std::move(begin)), end_(std::move(end)),
                                                     it_(begin) {}

    void reset() override {
        cdebug1 << "Resetting matcher " << typeid(begin_).name() << ", now at " << *begin_ << endl;
        it_ = begin_;
    }
    [[nodiscard]] bool done() const override { return it_ == end_; }
    bool match(char c) const override { return *it_ == c; }
    void inc() override {
        cdebug1 << "Incrementing matcher " << typeid(begin_).name() << ", now at " << *it_ << endl;
        ++it_;
    }
    ostream& print(ostream& os) const override { return os << "Matcher " << typeid(begin_).name(); }
};

int main() {
    vector<vector<char>> field;

    string line;
    while (getline(cin, line)) {
        field.emplace_back(line.begin(), line.end());
    }

    assert(!field.empty());
    vector matched(field.size(), vector(field[0].size(), false));
    vector matched2(field.size(), vector(field[0].size(), false));

    const string pattern = "MAS";

    auto build_matchers = [&pattern]() {
        vector<unique_ptr<pattern_matcher_t>> matchers;
        matchers.reserve(2);
        matchers.emplace_back(make_unique<pattern_matcher_impl<string::const_iterator>>(pattern.begin(), pattern.end()));
        matchers.emplace_back(make_unique<pattern_matcher_impl<string::const_reverse_iterator>>(pattern.rbegin(), pattern.rend()));
        return matchers;
    };

    size_t matches = 0;

    vector<vector<unique_ptr<pattern_matcher_t>>> column_matchers(field[0].size());
    for (auto& matchers : column_matchers) {
        matchers = build_matchers();
    }

    auto do_match = [&matches](auto& matchers, char c, auto on_done) {
        for (auto& matcher : matchers) {
            if (!matcher->match(c)) {
                matcher->reset();
            }
            if (matcher->match(c)) {
                matcher->inc();
                if (matcher->done()) {
                    on_done();
                    ++matches;
                    matcher->reset();
                }
            }
        }
    };

    for (size_t i = 0; i < field.size(); ++i) {
        vector<unique_ptr<pattern_matcher_t>> line_matchers = build_matchers();
        for (size_t j = 0; j < field[i].size(); ++j) {
            auto value_to_match = field[i][j];
            cdebug1 << "Matching " << value_to_match << endl;

            // do_match(line_matchers, value_to_match, [&matched, &pattern, i, j] {
            //     for (size_t k = j - pattern.size() + 1; k <= j; ++k) {
            //         matched[i][k] = true;
            //     }
            // });
            //
            // do_match(column_matchers[j], value_to_match, [&matched, &pattern, i, j] {
            //     for (size_t k = i - pattern.size() + 1; k <= i; ++k) {
            //         matched[k][j] = true;
            //     }
            // });

        }
    }

    // Diagonal matching

    auto min_size = static_cast<int>(min(field.size(), field[0].size()));

    vector<vector<unique_ptr<pattern_matcher_t>>> direct_matchers1(min_size - pattern.size() + 1);
    for (auto& matchers : direct_matchers1) {
        matchers = build_matchers();
    }

    vector<vector<unique_ptr<pattern_matcher_t>>> direct_matchers2(min_size - pattern.size() + 1);
    for (auto& matchers : direct_matchers2) {
        matchers = build_matchers();
    }

    vector<vector<unique_ptr<pattern_matcher_t>>> reverse_matchers1(min_size - pattern.size() + 1);
    for (auto& matchers : reverse_matchers1) {
        matchers = build_matchers();
    }

    vector<vector<unique_ptr<pattern_matcher_t>>> reverse_matchers2(min_size - pattern.size() + 1);
    for (auto& matchers : reverse_matchers2) {
        matchers = build_matchers();
    }

    size_t matched_diagonals = 0;

    for (int x_mod = 0; x_mod < min_size - pattern.size() + 1; ++x_mod) {
        for (size_t i = x_mod, j = 0; i < min_size && j < min_size; ++i, ++j) {
            do_match(direct_matchers1[x_mod], field[i][j], [&matched, &pattern, i, j] {
                matched[i - pattern.size() / 2][j - pattern.size() / 2] = true;

                for (int k = 0; k > -static_cast<int>(pattern.size()); --k) {
                    matched[i + k][j + k] = true;
                }
            });
        }
        if (x_mod == 0) {
            continue;
        }
        for (size_t i = 0, j = x_mod; i < min_size && j < min_size; ++i, ++j) {
            do_match(direct_matchers2[x_mod], field[i][j], [&matched, &pattern, i, j] {
                matched[i - pattern.size() / 2][j - pattern.size() / 2] = true;

                for (int k = 0; k > -static_cast<int>(pattern.size()); --k) {
                    matched[i + k][j + k] = true;
                }
            });
        }
    }

    for (int x_mod = 0; x_mod < min_size - pattern.size() + 1; ++x_mod) {
        for (int i = min_size - x_mod - 1, j = 0; i >= 0 && j < min_size; --i, ++j) {
            do_match(reverse_matchers1[x_mod], field[i][j], [&matched, &matched2, &pattern, i, j, &matched_diagonals] {
                if (matched[i + pattern.size() / 2][j - pattern.size() / 2]) {
                    matched_diagonals += true;
                    matched2[i + pattern.size() / 2][j - pattern.size() / 2] = true;
                }

                matched[i + pattern.size() / 2][j - pattern.size() / 2] = true;

                for (int k = 0; k > -static_cast<int>(pattern.size()); --k) {
                    matched[i - k][j + k] = true;
                }
            });
        }
        if (x_mod == 0) {
            continue;
        }
        for (int i = min_size - 1, j = x_mod; i >= 0 && j < min_size; --i, ++j) {
            do_match(reverse_matchers2[x_mod], field[i][j], [&matched, &matched2, &pattern, i, j, &matched_diagonals] {
                if (matched[i + pattern.size() / 2][j - pattern.size() / 2]) {
                    matched_diagonals += true;
                    matched2[i + pattern.size() / 2][j - pattern.size() / 2] = true;
                }

                for (int k = 0; k > -static_cast<int>(pattern.size()); --k) {
                    matched[i - k][j + k] = true;
                }
            });
        }
    }

    // for (size_t i = 0; i < field.size(); ++i) {
    //     for (size_t j = 0; j < field[i].size(); ++j) {
    //         if (!matched2[i][j]) {
    //             cdebug2 << ".";
    //             continue;
    //         }
    //         cdebug2 << field[i][j];
    //     }
    //     cdebug2 << endl;
    // }

    cout << "Pattern matches: " << matches << endl;
    cout << "Diagonal matches: " << matched_diagonals << endl;

    return 0;
}
