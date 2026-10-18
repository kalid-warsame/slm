#include <exception>
#include <iostream>

#include "Args.h"

int main(int argc, char* argv[]) {
    try {
        Options options = parse_args(argc, argv);

        // Temporary: show the parsed options until the model is in place.
        std::cout << "n = " << options.n << ", file = " << options.filename
                  << ", length = " << options.length << ", alpha = " << options.alpha << '\n';
    } catch (const std::exception& e) {
        std::cerr << "slm: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
