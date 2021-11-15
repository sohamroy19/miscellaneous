/**
 * Author   :   @sohamroy19
 * Date     :   14-11-2021
 * 
 * Purpose  :   This is a program that takes an integer n from the user
 *              and prints all subsets of the set {1, 2, 3, ..., n},
 *              without repeating the same subset.
 * 
 * Note     :   Time complexity n * 2^n
 *              Space complexity of O(2^n).
 */

#include <algorithm>
#include <bitset>
#include <iostream>
#include <vector>

int main() {
    std::cout << "This is a program that takes an integer n from the user\n"
                 "and prints all subsets of the set {1, 2, 3, ..., n},\n"
                 "without repeating the same subset.\n";

    std::cout << "\nEnter the integer n: ";
    int n;
    std::cin >> n;

    // supports n < 64, upto a maximum of 2^63 subsets
    std::vector<std::bitset<64>> subsets(1ULL << n);
    for (size_t i = 0; i < subsets.size(); ++i) {
        subsets[i] = i;
    }

    std::sort(subsets.begin(), subsets.end(), [](auto a, auto b) {
        return a.count() < b.count();
    });

    std::cout << "\nSubsets: ";
    for (auto subset : subsets) {
        for (size_t i = 0; i < n; ++i) {
            if (subset[i]) {
                std::cout << i + 1 << ' ';
            }
        }

        std::cout << '\n';
    }

    return 0;
}
