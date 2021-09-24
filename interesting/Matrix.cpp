#include <iostream>
#include <vector>

/**
 * Simple matrix class that supports basic arithmetic (+, -, *), equality (==, !=) and
 * exponentiation (pow).
 */
class Mat {
public:
    int n, m;
    std::vector<std::vector<int>> a;

    Mat() {}
    Mat(int nInit, int mInit)
        : n(nInit), m(mInit),
          a(std::vector<std::vector<int>>(n, std::vector<int>(m))) {}
    Mat(std::vector<std::vector<int>> &v)
        : n(v.size()), m(n ? v[0].size() : 0), a(v) {}

    inline void makeIdentity() { // logically should have n == m
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) a[i][j] = (i == j);
    }
    inline Mat operator+(const Mat &b) { // needs n == b.n && m == b.m
        Mat ans = Mat(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                ans.a[i][j] = (a[i][j] + b.a[i][j]); // % MOD;
        return ans;
    }
    inline Mat operator-(const Mat &b) { // needs n == b.n && m == b.m
        Mat ans = Mat(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                ans.a[i][j] = (a[i][j] - b.a[i][j]); // + MOD) % MOD;
        return ans;
    }
    inline Mat operator*(const Mat &b) { // needs m == b.n
        Mat ans = Mat(n, b.m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                for (int k = 0; k < b.m; k++)
                    ans.a[i][k] += a[i][j] * b.a[j][k];
        // ans.a[i][k] = (ans.a[i][k] + 1LL * a[i][j] * b.a[j][k] % MOD) % MOD;
        return ans;
    }
    inline Mat &operator+=(const Mat &b) { return *this = (*this) + b; }
    inline Mat &operator-=(const Mat &b) { return *this = (*this) - b; }
    inline Mat &operator*=(const Mat &b) { return *this = (*this) * b; }
    inline bool operator==(const Mat &b) { return a == b.a; }
    inline bool operator!=(const Mat &b) { return a != b.a; }
    inline Mat pow(long long k) { // needs n == m
        Mat ans(n, n), t(a);
        ans.makeIdentity();
        while (k) {
            if (k % 2 != 0) ans *= t;
            t *= t;
            k /= 2;
        }
        return ans;
    }
};

/**
 * finds nth number of a recurrence relation defined by initial state i
 * and the square matrix f, by evaluating f^(n-1) * i
 */
int recurrer(std::vector<std::vector<int>> f, std::vector<std::vector<int>> i, int n) {
    Mat func(f), init(i);
    return (func.pow(n - 1) * init).a[1][0];
}

int main() {
    int n;
    std::cin >> n;

    /**
     * Fibonacci: f(n + 1) = f(n) + f(n - 1)
     * ⌈1 1⌉ * ⌈ f(n) ⌉ = ⌈f(n+1)⌉
     * ⌊1 0⌋   ⌊f(n-1)⌋   ⌊ f(n) ⌋
     * where ⌈f(2)⌉ = ⌈1⌉
     *       ⌊f(1)⌋   ⌊1⌋
     */
    std::cout << "F(" << n << ") = " << recurrer({{1, 1}, {1, 0}}, {{1}, {1}}, n) << "\n";

    return 0;
}