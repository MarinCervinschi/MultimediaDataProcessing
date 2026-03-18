#include <math.h>

#include <bit>
#include <fstream>
#include <iostream>

class BitWriter {
   private:
    std::ofstream& os;
    uint8_t buffer;
    size_t bit_count;

   public:
    BitWriter(std::ofstream& out_stream)
        : os(out_stream), buffer(0), bit_count(0) {}

    void writeBit(uint8_t curbit) {
        buffer = (buffer << 1) | curbit;
        bit_count++;
        if (bit_count == 8) {
            os.put(buffer);
            buffer = 0;
            bit_count = 0;
        }
    }

    ~BitWriter() {
        if (bit_count > 0) {
            buffer <<= (8 - bit_count);
            os.put(buffer);
        }
    }
};

uint32_t map(int x) {
    int val;
    if (x <= 0) {
        val = 1 - 2 * x;
    } else {
        val = 2 * x;
    }
    return static_cast<uint32_t>(val);
}

int unmap(uint32_t x) {
    int val = static_cast<int>(x);
    if (val % 2 == 0) {
        return val / 2;
    }
    return (1 - val) / 2;
}

void compress(const char* filein, const char* fileout) {
    std::ifstream is(filein);

    if (!is) throw std::runtime_error("Could not open the input file");

    std::vector<int> numbers;
    int val;
    while (is >> val) {
        numbers.push_back(val);
    }

    std::ofstream os(fileout, std::ios::binary);
    if (!os) throw std::runtime_error("Could not open file for writing");

    BitWriter bw(os);

    for (int n : numbers) {
        uint32_t mapped = map(n);
        int L = std::bit_width(mapped);

        for (size_t i = 0; i < L - 1; ++i) {
            bw.writeBit(0);
        }

        for (int i = L - 1; i >= 0; --i) {
            bw.writeBit((mapped >> i) & 1);
        }
    }

    std::cout << "Compression completed successfully.\n";
}

class BitReader {
   private:
    std::ifstream& is;
    uint8_t buffer;
    int bits_left;

   public:
    BitReader(std::ifstream& in_stream)
        : is(in_stream), buffer(0), bits_left(0) {}

    int readBit() {
        if (bits_left == 0) {
            int c = is.get();
            if (c == EOF) {
                return -1;
            }
            buffer = static_cast<uint8_t>(c);
            bits_left = 8;
        }

        --bits_left;
        return (buffer >> bits_left) & 1;
    }
};

void decompress(const char* filein, const char* fileout) {
    std::ifstream is(filein, std::ios::binary);
    if (!is) throw std::runtime_error("Could not open the input file");

    std::ofstream os(fileout);
    if (!os) throw std::runtime_error("Could not open file for writing");

    BitReader br(is);

    int zero_count = 0;
    int c;
    while ((c = br.readBit()) != -1) {
        if (c == 0) {
            ++zero_count;
            continue;
        }

        uint32_t val = 1;
        for (int i = zero_count - 1; i >= 0; --i) {
            val = (val << 1) | br.readBit();
        }

        os << unmap(val) << '\n';
        zero_count = 0;
    }

    std::cout << "Decompression completed successfully.\n";
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << "[c | d] <filein> <fileout>\n";
        return 1;
    }

    try {
        if (std::string(argv[1]) == "c") {
            compress(argv[2], argv[3]);
        } else {
            decompress(argv[2], argv[3]);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error" << e.what() << "\n";
        return 1;
    }

    return 0;
}