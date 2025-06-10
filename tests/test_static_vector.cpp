#include "util/static_vector.hpp"

#include <algorithm>
#include <cstdlib>  // for std::exit
#include <iostream>
#include <source_location>

#include "test.hpp"

using namespace Clockwork;


static isize ctr = 0;

struct TestType {
    isize x;
    TestType() :
        x(0) {
        ctr += 1;
    }

    TestType(const TestType& other) :
        x(other.x) {
        ctr += 1;
    }
    ~TestType() {
        ctr -= 1;
    }

    explicit TestType(isize x) :
        x(x) {
        ctr += 1;
    }

    TestType& operator=(const TestType& other) = default;

    friend bool operator==(const TestType& lhs, const TestType& rhs) = default;
};

struct OnlyMoves {
    OnlyMoves()                                = default;
    OnlyMoves(const OnlyMoves&)                = delete;
    OnlyMoves(OnlyMoves&&) noexcept            = default;
    OnlyMoves& operator=(const OnlyMoves&)     = delete;
    OnlyMoves& operator=(OnlyMoves&&) noexcept = default;
};

void push_pop_back() {
    REQUIRE(ctr == 0);
    StaticVector<TestType, 42> vec;
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 42);
    REQUIRE(ctr == 0);
    vec.push_back(TestType{100});
    REQUIRE(vec.size() == 1);
    REQUIRE(ctr == 1);
    REQUIRE(vec.back().x == 100);
    vec.pop_back();
    REQUIRE(vec.empty());
    REQUIRE(ctr == 0);
}

void special_member_funs() {
    REQUIRE(ctr == 0);
    StaticVector<TestType, 10> vec;
    vec.push_back(TestType(0));
    vec.emplace_back(1);
    vec.push_back(TestType(2));
    REQUIRE(vec.size() == 3);
    REQUIRE(ctr == 3);
    {
        auto copy = vec;
        REQUIRE(ctr == 6);
        REQUIRE(copy == vec);
        copy = copy;             // NOLINT
        copy = std::move(copy);  // NOLINT
        REQUIRE(ctr == 6);
        REQUIRE(copy.end() - copy.begin() == 3);
        auto copy2 = std::move(copy);
        REQUIRE(copy.empty());
        REQUIRE(vec == copy2);
        REQUIRE(ctr == 6);
        copy = std::move(copy2);
        REQUIRE(ctr == 6);
        copy2 = copy;
        REQUIRE(ctr == 9);
        REQUIRE(vec.size() == copy2.size());
        for (usize i = 0; i < vec.size(); ++i) {
            REQUIRE(vec[i] == copy2[i]);
        }
    }
    REQUIRE(ctr == 3);
    auto vec2 = vec;
    vec2.clear();
    vec2.emplace_back(1);
    vec = vec2;
    REQUIRE(vec == vec2);
    vec2.resize(4, TestType{100});
    vec = vec2;
    REQUIRE(vec.size() == 4);
}

void misc() {
    REQUIRE(ctr == 0);
    StaticVector<TestType, 11> vec{TestType(1), TestType(2), TestType(3)};
    REQUIRE(vec.size() == 3);
    vec.emplace_back(4);
    REQUIRE(ctr == 4);
    vec.resize(6);
    REQUIRE(ctr == 6);
    REQUIRE(vec.back().x == 0);
    auto vec2 = vec;
    vec2.pop_back();
    vec.append(std::move(vec2));
    REQUIRE(ctr == 11);
    REQUIRE(vec.back().x == 0);
    vec.resize(10, TestType{-1});
    REQUIRE(vec.back().x == 4);
    REQUIRE(vec[2].x == 3);
    vec.clear();
    REQUIRE(ctr == 0);
}

// this mostly tests that the following doesn't attempt to copy an element.
void no_copies() {
    StaticVector<OnlyMoves, 3> vec;
    vec.push_back(OnlyMoves());
    vec.emplace_back(OnlyMoves());
    vec.emplace_back();
    auto vec2 = std::move(vec);
    vec       = std::move(vec2);
    REQUIRE(vec.size() == 3);
    vec.resize(0);
    REQUIRE(vec.empty());
}

static_assert(std::ranges::contiguous_range<StaticVector<int, 42>>);

int main() {
    push_pop_back();
    special_member_funs();
    misc();
    no_copies();
    REQUIRE(ctr == 0);
    return 0;
}
