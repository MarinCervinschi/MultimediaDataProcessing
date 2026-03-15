#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

class BitWriter {
   private:
    std::ofstream& os;
    uint8_t buffer = 0;
    int bit_count = 0;

   public:
    BitWriter(std::ofstream& out_stream) : os(out_stream) {}

    void write(uint32_t value, int num_bits) {
        for (int i = num_bits - 1; i >= 0; --i) {
            // Estrazione dell'i-esimo bit
            uint8_t bit = (value >> i) & 1;

            // Faccio spazio shiftando a sinistra il buffer e aggiungendo il bit
            // a destra
            buffer = (buffer << 1) | bit;
            bit_count++;

            // Quando raggiungo gli 8 bit (un byte completo), lo scrivo e svuoto
            // il buffer
            if (bit_count == 8) {
                os.put(buffer);
                buffer = 0;
                bit_count = 0;  // Azzero il contatore
            }
        }
    }

    // Forza la scrittura dei bit rimanenti (padding con zeri) alla fine
    // dell'inserimento
    ~BitWriter() {
        if (bit_count > 0) {
            // Shifto il buffer a sx per spingere i bit inseriti in coda
            // e riempire in automatico con 0 (zero padding).
            buffer <<= (8 - bit_count);
            os.put(buffer);
        }
    }
};

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

void write_to_file_11(const char* filename, const std::vector<int>& arr) {
    std::ofstream os(filename, std::ios::binary);
    if (!os) throw std::runtime_error("Could not open file for writing");

    BitWriter bw(os);

    for (int val : arr) {
        // Maschero i primi 11 bit (0x7FF) per evitare che bit estranei
        // "sporchi" di un bit negativo (come il 32esimo bit del compl. a 2)
        // entrino nel BitWriter
        uint32_t value_11bit = static_cast<uint32_t>(val) & 0x7FF;

        // Dico al BitWriter di inviare al file esattamente 11 bit
        bw.write(value_11bit, 11);
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <filein.txt> <fileout.bin>\n";
        return 1;
    }

    try {
        std::vector<int> data = read_from_file(argv[1]);
        write_to_file_11(argv[2], data);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}