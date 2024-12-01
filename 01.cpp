#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

int main() {
    vector<int> list1, list2;

    int n1, n2;
    while (cin >> n1 >> n2) {
        list1.push_back(n1);
        list2.push_back(n2);
    }

    assert(size(list1) == size(list2));

    ranges::sort(list1);
    ranges::sort(list2);

    int total_distance = transform_reduce(list1.begin(), list1.end(),
                                          list2.begin(), 0, plus<>{},
                                          [](int n1, int n2) {
                                              return abs(n1 - n2);
                                          });

    cout << "Total distance: " << total_distance << endl;

    size_t sim_score = 0;

    auto it1 = list1.begin();
    auto it2 = list2.begin();

    auto get_reps = [](auto & it1, const auto & end) {
        const auto begin = it1;
        it1 = ranges::adjacent_find(it1, end, not_equal_to{});
        it1 = it1 == end ? it1 : it1 + 1;

        return ranges::distance(begin, it1);
    };

    while (true) {
        // Fast-forwarding until one of lists is depleted or members are the same
        while (it1 != list1.end() && it2 != list2.end() && *it1 != *it2) {
            if (*it1 < *it2) it1 = find_if(it1, list1.end(), [&](int n1) { return n1 >= *it2; });
            else it2 = find_if(it2, list2.end(), [&](int n2) { return n2 >= *it1; });
        }
        if (it1 == list1.end() || it2 == list2.end()) break;

        int number = *it1;
        sim_score += number * get_reps(it1, list1.end()) *
                     get_reps(it2, list2.end());

        if (it1 == list1.end() || it2 == list2.end()) break;
    }

    cout << "Similarity score: " << sim_score << endl;

    return 0;
}
