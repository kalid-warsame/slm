#include "Args.h"

#include <cmath>
#include <stdexcept>
#include <vector>

namespace {

std::size_t parse_positive_int(const std::string& s, const std::string& what) {
    if (s.empty() || s.find_first_not_of("0123456789") != std::string::npos) {
        throw std::invalid_argument(what + " must be a positive integer, got '" + s + "'");
    }
    unsigned long long value = 0;
    try {
        value = std::stoull(s);
    } catch (const std::out_of_range&) {
        throw std::invalid_argument(what + " is too large: '" + s + "'");
    }
    if (value == 0) {
        throw std::invalid_argument(what + " must be a positive integer, got '" + s + "'");
    }
    return static_cast<std::size_t>(value);
}

double parse_pseudocount(const std::string& s) {
    const std::string message =
        "option -a must be followed by a non-negative number, got '" + s + "'";
    std::size_t pos = 0;
    double value = 0.0;
    try {
        value = std::stod(s, &pos);
    } catch (const std::exception&) {
        throw std::invalid_argument(message);
    }
    if (pos != s.size() || !std::isfinite(value) || value < 0.0) {
        throw std::invalid_argument(message);
    }
    return value;
}

}  // namespace

std::string usage() {
    return "usage: slm [-a pseudocount] n training_file length";
}

Options parse_args(int argc, char* argv[]) {
    Options options;
    int i = 1;

    // Options come before the positional arguments.
    while (i < argc) {
        std::string arg = argv[i];
        if (arg == "-a") {
            if (i + 1 >= argc) {
                throw std::invalid_argument("option -a must be followed by a non-negative number");
            }
            options.alpha = parse_pseudocount(argv[i + 1]);
            i += 2;
        } else if (arg.size() > 1 && arg[0] == '-') {
            throw std::invalid_argument("unknown option '" + arg + "'\n" + usage());
        } else {
            break;
        }
    }

    std::vector<std::string> positional(argv + i, argv + argc);
    if (positional.size() != 3) {
        throw std::invalid_argument("wrong number of arguments\n" + usage());
    }

    options.n = parse_positive_int(positional[0], "n");
    options.filename = positional[1];
    options.length = parse_positive_int(positional[2], "output length");
    if (options.length < options.n) {
        throw std::invalid_argument("output length (" + std::to_string(options.length)
                                    + ") must be at least n (" + std::to_string(options.n) + ")");
    }
    return options;
}
