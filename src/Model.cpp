#include "Model.h"

#include <algorithm>
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
    transitions_.clear();
    follow_counts_.clear();

    std::set<char> chars(text.begin(), text.end());
    alphabet_.assign(chars.begin(), chars.end());

    total_grams_ = text.size() - n_ + 1;
    for (std::size_t i = 0; i + n_ <= text.size(); ++i) {
        std::string gram = text.substr(i, n_);
        ++gram_counts_[gram];
        if (i + n_ < text.size()) {   // the last n-gram is followed by nothing
            ++transitions_[gram][text[i + n_]];
            ++follow_counts_[gram];
        }
    }
}

double Model::frequency(const std::string& gram) const {
    auto it = gram_counts_.find(gram);
    if (it == gram_counts_.end() || total_grams_ == 0) {
        return 0.0;
    }
    return static_cast<double>(it->second) / total_grams_;
}

double Model::probability(const std::string& gram, char c) const {
    if (!std::binary_search(alphabet_.begin(), alphabet_.end(), c)) {
        return 0.0;
    }
    std::size_t count = 0;
    std::size_t total = 0;
    auto it = transitions_.find(gram);
    if (it != transitions_.end()) {
        auto jt = it->second.find(c);
        if (jt != it->second.end()) {
            count = jt->second;
        }
        total = follow_counts_.at(gram);
    }
    double denominator = total + alpha_ * alphabet_.size();
    if (denominator == 0.0) {
        return 0.0;
    }
    return (count + alpha_) / denominator;
}

std::vector<double> Model::next_weights(const std::string& gram) const {
    const std::map<char, std::size_t>* counts = nullptr;
    auto it = transitions_.find(gram);
    if (it != transitions_.end()) {
        counts = &it->second;
    }
    if (counts == nullptr && alpha_ == 0.0) {
        return {};
    }

    std::vector<double> weights;
    weights.reserve(alphabet_.size());
    for (char c : alphabet_) {
        double count = 0.0;
        if (counts != nullptr) {
            auto jt = counts->find(c);
            if (jt != counts->end()) {
                count = static_cast<double>(jt->second);
            }
        }
        weights.push_back(count + alpha_);
    }
    return weights;
}
