#include "Model.h"

#include <fstream>
#include <set>
#include <sstream>
#include <stdexcept>

Model::Model(std::size_t n, double alpha) : n_(n), alpha_(alpha) {
    if (n == 0) {
        throw std::invalid_argument("n must be a positive integer");
    }
    if (!(alpha >= 0.0)) {
        throw std::invalid_argument("pseudocount must be a non-negative number");
    }
}

void Model::train_from_file(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("cannot open file '" + filename + "'");
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    if (in.bad()) {
        throw std::runtime_error("error while reading file '" + filename + "'");
    }
    train(buffer.str());
}

void Model::train(const std::string& text) {
    if (text.size() <= n_) {
        throw std::runtime_error("training text is too short: it must contain more than "
                                 + std::to_string(n_) + " characters");
    }

    gram_counts_.clear();

    std::set<char> chars(text.begin(), text.end());
    alphabet_.assign(chars.begin(), chars.end());

    total_grams_ = text.size() - n_ + 1;
    for (std::size_t i = 0; i + n_ <= text.size(); ++i) {
        ++gram_counts_[text.substr(i, n_)];
    }
}

double Model::frequency(const std::string& gram) const {
    auto it = gram_counts_.find(gram);
    if (it == gram_counts_.end() || total_grams_ == 0) {
        return 0.0;
    }
    return static_cast<double>(it->second) / total_grams_;
}
