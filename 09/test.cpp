#include <iostream>
#include "ThreadPool.h"

#define TESTS_BEGIN(c) \
    int N_TESTS = 0; \
    int N_FAILED_TESTS = 0; \
    c; \
    std::cout << "Tests " << #c << " started." << std::endl;

#define TEST(c)  do { \
    N_TESTS++; \
    if (!(c)) { \
    N_FAILED_TESTS++; \
        std::cout << "Test " << #c << " failed in " << __FILE__ \
                  << "(line " << __LINE__ << ") : " << std::endl; \
    } \
    else { \
        std::cout << "Test " << #c << " done successfully" << std::endl; \
    } \
} while(0);

#define TESTS_END \
    std::cout << "Failed " << N_FAILED_TESTS << " from " << N_TESTS << " tests." << std::endl << std::endl;


size_t fibb(size_t n) {
    switch (n) {
        case 0:
            return 1;
        case 1:
            return 1;
        default:
            return fibb(n - 2) + fibb(n - 1);
    }
}

size_t factorial(size_t n) {
    if (n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}

int main() {
    ThreadPool pool(8);

    TESTS_BEGIN("ThreadPool tests")
    size_t N = 20;
    std::vector<std::future<void>> futures(N);
    for (size_t n = 0; n < N; n++) {
        futures[n] = pool.exec([n]() {
            std::string s = "Thread:" + std::to_string(n) + " iteration:";
            for (size_t i = 0; i < 1000; i++) {
                std::cout << s + std::to_string(i) + '\n';
            }
        });
    }

    size_t count = 0;
    for (auto &future : futures) {
        future.get();
        count++;
    }
    TEST(count == N)

    std::vector<std::future<size_t>> f(N + 1);
    f[0] = pool.exec([]() { return size_t(1); });
    for (size_t n = 1; n <= N; n++) {
        f[n] = pool.exec([&f, n]() { return f[n - 1].get() * n; });
    }
    auto NFactorial = f[N].get();
    TEST(NFactorial == factorial(N))
    std::cout << N << "! = " << NFactorial << std::endl;

    N = 41;
    f.resize(N);
    for (size_t n = 0; n <= N; n++) {
        f[n] = pool.exec(fibb, n);
    }

    for (size_t n = 0; n <= N; n++) {
        auto nFibb = f[n].get();
        TEST(nFibb == fibb(n))
        std::cout << "Fibonacci(" + std::to_string(n) + ") = " << nFibb << std::endl;
    }

    TESTS_END
    return 0;
}
