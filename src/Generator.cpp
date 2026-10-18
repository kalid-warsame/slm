#include "Generator.h"

#include <stdexcept>
#include <vector>

Generator::Generator(const Model& model) : model_(model), rng_(std::random_device{}()) {}

Generator::Generator(const Model& model, unsigned seed) : model_(model), rng_(seed) {}

std::string Generator::random_start() {
    const auto& counts = model_.gram_counts();
    if (counts.empty()) {
        throw std::logic_error("the model has not been trained");
    }
    std::vector<std::string> grams;
    std::vector<double> weights;
    grams.reserve(counts.size());
    weights.reserve(counts.size());
    for (const auto& [gram, count] : counts) {
        grams.push_back(gram);
        weights.push_back(static_cast<double>(count));
    }
    std::discrete_distribution<std::size_t> pick(weights.begin(), weights.end());
    return grams[pick(rng_)];
}

std::string Generator::generate(std::size_t length) {
    if (length < model_.n()) {
        throw std::invalid_argument("output length must be at least n");
    }

    std::string current = random_start();
    std::string output = current;
    const std::vector<char>& alphabet = model_.alphabet();

    while (output.size() < length) {
        std::vector<double> weights = model_.next_weights(current);
        if (weights.empty()) {
            break;   // stuck: n-gram only occurs at the end of the training text
        }
        std::discrete_distribution<std::size_t> pick(weights.begin(), weights.end());
        char next = alphabet[pick(rng_)];
        output.push_back(next);
        current.erase(0, 1);
        current.push_back(next);
    }
    return output;
}
