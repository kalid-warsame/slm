#ifndef SLM_GENERATOR_H
#define SLM_GENERATOR_H

#include <cstddef>
#include <random>
#include <string>

#include "Model.h"

// Generates random text from a trained Model.
// The Generator keeps a reference to the model, so the model must
// outlive the generator.
class Generator {
public:
    // Seeds the random number generator from std::random_device.
    explicit Generator(const Model& model);

    // Fixed seed, for reproducible output (e.g. in tests).
    Generator(const Model& model, unsigned seed);

    // Generates at most `length` characters, including the starting n-gram.
    // The output is shorter if generation gets stuck on an n-gram without
    // successors (only possible with pseudocount 0).
    // Throws std::invalid_argument if length < n.
    std::string generate(std::size_t length);

private:
    // Picks a starting n-gram according to the n-gram frequencies.
    std::string random_start();

    const Model& model_;
    std::mt19937 rng_;
};

#endif
