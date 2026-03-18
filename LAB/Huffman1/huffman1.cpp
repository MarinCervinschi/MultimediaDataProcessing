#include <algorithm>
#include <bit>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <ranges>
#include <string>
#include <vector>

struct Node {
    uint8_t sym;
    uint freq;
    Node* left;
    Node* right;

    Node(uint8_t s, uint f) : sym(s), freq(f), left(nullptr), right(nullptr) {}
    Node(uint f, Node* l, Node* r) : sym(0), freq(f), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) { return a->freq > b->freq; }
};

void get_lengths(Node* root, int depth, std::map<uint8_t, int>& lengths) {
    if (!root) return;
    if (!root->left && !root->right) {
        lengths[root->sym] = depth;
        return;
    }
    get_lengths(root->left, depth + 1, lengths);
    get_lengths(root->right, depth + 1, lengths);
}

void delete_tree(Node* root) {
    if (!root) return;
    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
}

std::map<uint8_t, uint> count_frequencies(const char* filename) {
    std::ifstream is(filename, std::ios::binary);
    std::map<uint8_t, uint> freq;

    if (!is) return freq;

    int c;
    while ((c = is.get()) != EOF) {
        freq[static_cast<uint8_t>(c)]++;
    }

    return freq;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0]
                  << "[c|d] <filein.bin> <fileout.txt>\n";
        return 1;
    }

    if (std::string(argv[1]) == "c") {
        std::map<uint8_t, uint> freq = count_frequencies(argv[2]);

        auto values = std::views::values(freq);
        int totalFreq = std::accumulate(values.begin(), values.end(), 0);

        std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
        for (const auto& [sym, count] : freq) {
            pq.push(new Node(sym, count));
        }

        while (pq.size() > 1) {
            Node* left = pq.top();
            pq.pop();
            Node* right = pq.top();
            pq.pop();
            pq.push(new Node(left->freq + right->freq, left, right));
        }

        std::map<uint8_t, int> lengths;
        Node* root = nullptr;
        if (!pq.empty()) {
            root = pq.top();
            get_lengths(root, 0, lengths);
        }

        std::vector<double> Lov;
        for (const auto& [sym, count] : freq) {
            double Pi = static_cast<double>(count) / totalFreq;
            int Li = lengths[sym];
            Lov.push_back(Pi * Li);
        }

        delete_tree(root);
    }

    return 0;
}