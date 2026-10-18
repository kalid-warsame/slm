#include <iostream>

int main(int argc, char* argv[]) {
    (void)argv;
    if (argc != 4) {
        std::cerr << "usage: slm n training_file length\n";
        return 1;
    }
    std::cout << "slm: not implemented yet\n";
    return 0;
}
