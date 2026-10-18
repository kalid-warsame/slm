#ifndef SLM_MODEL_H
#define SLM_MODEL_H

#include <cstddef>
#include <map>
#include <string>
#include <vector>

// A character-level n-gram language model.
//
// Training records how often each n-gram occurs in a text and which
// characters follow it. From those counts the model gives n-gram
// frequencies and the conditional distribution P(c | w) of the next
// character c after an n-gram w, optionally smoothed with a pseudocount.
class Model {
public:
    // n: n-gram length (> 0). alpha: pseudocount (>= 0).
    // Throws std::invalid_argument on bad values.
    explicit Model(std::size_t n, double alpha = 0.0);

    // Reads the whole file and trains on it.
    // Throws std::runtime_error if the file cannot be opened or read,
    // or if the text is too short (see train).
    void train_from_file(const std::string& filename);

    // Trains on a string, replacing any earlier training.
    // Throws std::runtime_error if the text does not contain at least
    // one n-gram followed by a character, i.e. if it has n or fewer chars.
    void train(const std::string& text);

    std::size_t n() const { return n_; }
    double alpha() const { return alpha_; }

    // The distinct characters of the training text, sorted.
    const std::vector<char>& alphabet() const { return alphabet_; }

    // Number of occurrences of every n-gram in the training text.
    const std::map<std::string, std::size_t>& gram_counts() const { return gram_counts_; }

    // Relative frequency of an n-gram among all n-grams in the text.
    double frequency(const std::string& gram) const;

    // Estimated P(c | gram) = (N(gram, c) + alpha) / (N(gram) + alpha * |A|).
    // Returns 0 if c is not in the alphabet or if the distribution is
    // undefined (alpha == 0 and gram is never followed by a character).
    double probability(const std::string& gram, char c) const;

    // Unnormalised sampling weights N(gram, c) + alpha for the character
    // after gram, one per character in alphabet() and in the same order.
    // Empty if gram has no successor distribution (alpha == 0 and gram is
    // never followed by a character in the training text).
    std::vector<double> next_weights(const std::string& gram) const;

private:
    std::size_t n_;
    double alpha_;
    std::size_t total_grams_ = 0;
    std::vector<char> alphabet_;
    std::map<std::string, std::size_t> gram_counts_;
    // transitions_[w][c] = N(w, c)
    std::map<std::string, std::map<char, std::size_t>> transitions_;
    // follow_counts_[w] = N(w) = sum over c of N(w, c)
    std::map<std::string, std::size_t> follow_counts_;
};

#endif
