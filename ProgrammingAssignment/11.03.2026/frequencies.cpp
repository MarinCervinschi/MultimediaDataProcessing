#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

std::map<uint8_t, int> count_frequencies(const char* filename) {
    std::ifstream is(filename, std::ios::binary);
    std::map<uint8_t, int> freq;

    if (!is) return freq;

    int c;
    while ((c = is.get()) != EOF) {
        freq[static_cast<uint8_t>(c)]++;
    }

    return freq;
}

void write_frequencies(const char* filename, const std::map<uint8_t, int>& freq) {
    std::ofstream os(filename);
    if (!os) return;

    for (const auto& [byte, count] : freq) {
        os << std::format("{:02x}\t{}\n", byte, count);
    }

    std::cout << "Frequencies written to " << filename << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    std::map<uint8_t, int> freq = count_frequencies(argv[1]);
    write_frequencies(argv[2], freq);

    return 0;
}