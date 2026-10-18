// Unit tests for Model, based on the examples in the lab text.
// Build and run with: make test

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../src/Model.h"

namespace {

int failures = 0;

void check(bool ok, const std::string& what) {
    if (!ok) {
        std::cerr << "FAIL: " << what << '\n';
        ++failures;
    }
}

void check_near(double actual, double expected, const std::string& what) {
    check(std::fabs(actual - expected) < 1e-3,
          what + " (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
}

template <typename Exception, typename F>
void check_throws(F f, const std::string& what) {
    try {
        f();
    } catch (const Exception&) {
        return;
    } catch (...) {
    }
    check(false, what + " should throw");
}

void example1() {
    Model m(2);
    m.train("ababac");
    check_near(m.frequency("ab"), 0.4, "f(ab)");
    check_near(m.frequency("ba"), 0.4, "f(ba)");
    check_near(m.frequency("ac"), 0.2, "f(ac)");
    check_near(m.probability("ab", 'a'), 1.0, "P(a|ab)");
    check_near(m.probability("ba", 'b'), 0.5, "P(b|ba)");
    check_near(m.probability("ba", 'c'), 0.5, "P(c|ba)");
    check(m.next_weights("ac").empty(), "ac has no transitions");
}

void example2() {
    Model m(3);
    m.train("Hubba bubba");
    check(m.gram_counts().size() == 7, "seven distinct trigrams");
    check_near(m.frequency("ubb"), 2.0 / 9, "f(ubb)");
    check_near(m.frequency("Hub"), 1.0 / 9, "f(Hub)");
    check_near(m.probability("bba", ' '), 1.0, "P(space|bba)");
    check_near(m.probability("ba ", 'b'), 1.0, "P(b|'ba ')");
}

void pseudocounts() {
    Model m(2, 1.0);
    m.train("ababac");
    check_near(m.probability("ab", 'a'), 0.6, "alpha=1: P(a|ab)");
    check_near(m.probability("ab", 'b'), 0.2, "alpha=1: P(b|ab)");
    check_near(m.probability("ba", 'b'), 0.4, "alpha=1: P(b|ba)");
    check_near(m.probability("ba", 'a'), 0.2, "alpha=1: P(a|ba)");
    check_near(m.probability("ac", 'c'), 1.0 / 3, "alpha=1: P(c|ac)");
    check_near(m.probability("zz", 'a'), 1.0 / 3, "alpha=1: unseen n-gram is uniform");
    check(m.next_weights("zz").size() == 3, "alpha=1: unseen n-gram has weights");
}

void errors() {
    check_throws<std::invalid_argument>([] { Model m(0); }, "n = 0");
    check_throws<std::invalid_argument>([] { Model m(2, -1.0); }, "negative alpha");
    check_throws<std::runtime_error>([] { Model m(3); m.train("abc"); }, "too short text");
    check_throws<std::runtime_error>([] { Model m(2); m.train_from_file("no_such_file.txt"); },
                                     "missing file");
}

}  // namespace

int main() {
    example1();
    example2();
    pseudocounts();
    errors();
    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    }
    std::cout << failures << " test(s) failed.\n";
    return 1;
}
