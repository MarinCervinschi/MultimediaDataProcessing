
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <filein.bin> <fileout.txt>\n";
        return 1;
    }

    std::ifstream is(argv[1], std::ios::binary);
    std::ofstream os(argv[2]);
    if (!is || !os) throw std::runtime_error("Could not open files");

    int c;
    while (is.read(reinterpret_cast<char*>(&c), sizeof(int))) {
        os << c << "\n";
    }

    std::cout << "Conversion completed successfully.\n";

    return 0;
}