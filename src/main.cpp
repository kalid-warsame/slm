#include <exception>
#include <iostream>

#include "Args.h"
#include "Model.h"

int main(int argc, char* argv[]) {
    try {
        Options options = parse_args(argc, argv);

        Model model(options.n, options.alpha);
        model.train_from_file(options.filename);

        // Temporary: show some statistics until text generation is in place.
        std::cout << "distinct " << options.n << "-grams: " << model.gram_counts().size()
                  << ", alphabet size: " << model.alphabet().size() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "slm: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
