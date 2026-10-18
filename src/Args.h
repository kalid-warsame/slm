#ifndef SLM_ARGS_H
#define SLM_ARGS_H

#include <cstddef>
#include <string>

// Command-line options for slm.
struct Options {
    double alpha = 0.0;        // pseudocount, set with -a
    std::size_t n = 0;         // n-gram length
    std::string filename;      // training text
    std::size_t length = 0;    // number of characters to output
};

// Parses: slm [-a alpha] n training_file length
// Throws std::invalid_argument with a descriptive message on any error.
Options parse_args(int argc, char* argv[]);

// One-line usage text.
std::string usage();

#endif
