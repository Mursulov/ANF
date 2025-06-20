#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <bitset>
#include <ctime>



std::vector<int> random_truth_table(int n = 6) {
    int N = 1 << n;
    std::vector<int> tt(N);
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(0, 1);
    for (int i = 0; i < N; ++i) {
        tt[i] = dist(rng);
    }
    return tt;
}

std::vector<int> anf_from_truth_table(const std::vector<int>& tt) {
    std::vector<int> anf = tt;
    int n = 6;
    int N = 1 << n;
    for (int i = 0; i < n; ++i) {
        int step = 1 << i;
        for (int j = 0; j < N; ++j) {
            if (j & step) {
                anf[j] ^= anf[j ^ step];
            }
        }
    }
    return anf;
}

std::string monomial_to_str(int idx, int n = 6) {
    if (idx == 0) return "1";
    std::string s;
    bool first = true;
    for (int i = 0; i < n; ++i) {
        if (idx & (1 << i)) {
            if (!first) s += "*";
            s += "x" + std::to_string(i + 1);
            first = false;
        }
    }
    return s;
}

void print_anf(const std::vector<int>& anf) {
    std::vector<std::string> terms;
    for (int i = 0; i < anf.size(); ++i) {
        if (anf[i]) terms.push_back(monomial_to_str(i));
    }
    if (terms.empty()) {
        std::cout << "0" << std::endl;
        return;
    }
    for (size_t i = 0; i < terms.size(); ++i) {
        if (i) std::cout << " xor ";
        std::cout << terms[i];
    }
    std::cout << std::endl;
}

void print_truth_table(const std::vector<int>& tt) {
    std::cout << "\nТаблица истинности (x1 x2 x3 x4 x5 x6 | f):\n";
    std::cout << "-----------------------------------------\n";
    for (int i = 0; i < 64; ++i) {
        std::bitset<6> bits(i);
        // x1 = младший бит, x6 = старший бит
        for (int j = 0; j < 6; ++j) {
            std::cout << bits[j] << " ";
        }
        std::cout << "| " << tt[i] << "\n";
    }
}

int main() {
 


    std::vector<int> tt = random_truth_table(6);

 
    std::cout << "Случайный вектор значений (64):" << std::endl;
    for (int i = 0; i < tt.size(); ++i) {
        std::cout << tt[i];
        if ((i + 1) % 8 == 0) std::cout << " ";
    }
    std::cout << std::endl << std::endl;

 
    print_truth_table(tt);


    std::vector<int> anf = anf_from_truth_table(tt);

    std::cout << "\nПолином Жегалкина (АНФ):" << std::endl;
    print_anf(anf);

    return 0;
}