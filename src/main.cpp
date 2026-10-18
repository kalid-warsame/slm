#include <exception>
#include <iostream>

#include "Args.h"
#include "Generator.h"
#include "Model.h"

int main(int argc, char* argv[]) {
    try {
        Options options = parse_args(argc, argv);

        Model model(options.n, options.alpha);
        model.train_from_file(options.filename);

        Generator generator(model);
        std::cout << generator.generate(options.length) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "slm: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
