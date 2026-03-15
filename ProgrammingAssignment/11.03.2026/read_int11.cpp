#include <fstream>
#include <iostream>
#include <cstdint>

class BitReader {
private:
    std::ifstream& is;
    uint8_t buffer = 0;
    int bit_count = 0;

public:
    BitReader(std::ifstream& in_stream) : is(in_stream) {}

    bool read(int num_bits, uint32_t& out_value) {
        uint32_t value = 0;
        for (int i = 0; i < num_bits; ++i) {
            if (bit_count == 0) {
                int c = is.get();
                if (c == EOF) {
                    return false;
                }
                buffer = static_cast<uint8_t>(c);
                bit_count = 8;
            }

            // Estrazione dell'i-esimo bit dal buffer attuale
            uint8_t bit = (buffer >> (bit_count - 1)) & 1;
            value = (value << 1) | bit;
            bit_count--;
        }
        out_value = value;
        return true;
    }
};

// Funzione helper per risistemare il segno se il numero a 11 bit era negativo
int32_t sign_extend_11_bit(uint32_t val) {
    // Il bit numero 10 (l'11esimo) è il bit di segno.
    if (val & 0x400) {  // 0x400 è il bit in posizione 10 (100 0000 0000 binario)
        // Se è negativo in formato 11 bit, estendiamo il segno "1" per tutti i restanti 21 bit a sinistra
        val |= 0xFFFFF800; // 0xFFFFF800 è la maschera di bit 1 fino al bit 11 
    }
    return static_cast<int32_t>(val);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <filein.out> <fileout.txt>\n";
        return 1;
    }

    std::ifstream is(argv[1], std::ios::binary);
    std::ofstream os(argv[2]);
    if (!is || !os) {
        std::cerr << "Could not open files\n";
        return 1;
    }

    BitReader reader(is);
    uint32_t raw_value;
    
    while (reader.read(11, raw_value)) {
        // Ripristino del segno: l'11 bit complemento a due torna a essere un int32 stampabile col meno
        int32_t real_value = sign_extend_11_bit(raw_value);
        os << real_value << "\n";
    }

    std::cout << "Conversion completed successfully.\n";
    return 0;
}