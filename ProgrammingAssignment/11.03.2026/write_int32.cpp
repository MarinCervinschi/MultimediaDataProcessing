#include <fstream>
#include <iostream>
#include <vector>

std::vector<int> read_from_file(const char* filename) {
    std::vector<int> v;
    std::ifstream is(filename);
    if (!is) throw std::runtime_error("Could not open file for reading");

    int val;
    while (is >> val) {
        v.push_back(val);
    }
    return v;
}

void write_to_file(const char* filename, const std::vector<int>& arr) {
    std::ofstream os(filename, std::ios::binary);
    if (!os) {
        throw std::runtime_error("Could not open file for writing");
    }
    for (size_t i = 0; i < arr.size(); i++) {
        os.write(reinterpret_cast<const char*>(&arr[i]), sizeof(int));
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <filein.txt> <fileout.bin>\n";
        return 1;
    }

    try {
        std::vector<int> data = read_from_file(argv[1]);
        write_to_file(argv[2], data);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}