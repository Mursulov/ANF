#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <ctime>
#include <random>
#include <termios.h>
#include <unistd.h>

std::vector<int> live_truth_table_input(int n = 6) {
    int N = 1 << n;
    std::vector<int> tt;
    std::cout << "Введите поочерёдно 64 значения функции (0 или 1):\n";
    std::cout << "Backspace — удалить, Enter — завершить ввод (при 64 значениях).\n";
    std::cout.flush();

    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (true) {
        char ch = getchar();
        if ((ch == '\n' || ch == '\r') && tt.size() == N) {
            std::cout << std::endl;
            break;
        }
        if ((ch == 127 || ch == 8) && !tt.empty()) {
            tt.pop_back();
        } else if ((ch == '0' || ch == '1') && (int)tt.size() < N) {
            tt.push_back(ch - '0');
        }
        std::cout << "\r";
        for (auto v : tt) std::cout << v;
        std::cout << std::string(N - tt.size(), '_') << "  [" << tt.size() << "/" << N << "]";
        std::cout.flush();
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << std::endl;
    return tt;
}

std::vector<int> random_truth_table(int n = 6) {
    int N = 1 << n;
    std::vector<int> tt(N);
    std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
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
    for (int i = n - 1; i >= 0; --i) {
        if (idx & (1 << i)) {
            if (!first) s += "*";
            s += "x" + std::to_string(n - i);
            first = false;
        }
    }
    return s;
}

void print_anf(const std::vector<int>& anf) {
    std::cout << "\nПолином Жегалкина:\n";
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
    std::cout << "\nТаблица истинности:\n";
    std::cout << "x1\tx2\tx3\tx4\tx5\tx6\tF\n";
    for (int i = 0; i < 64; ++i) {
        std::bitset<6> bits(i);
        for (int j = 5; j >= 0; --j) {
            std::cout << bits[j] << "\t";
        }
        std::cout << tt[i] << std::endl;
    }
}

int main() {
    std::vector<int> tt;

    std::cout << "Выберите способ задания вектора значений:\n";
    std::cout << "1) Ввести вручную (по одному символу)\n";
    std::cout << "2) Сгенерировать случайный\n";
    std::cout << "Ваш выбор: ";
    int mode = 0;
    std::cin >> mode;
    std::cin.ignore(1000, '\n');

    if (mode == 1) {
        tt = live_truth_table_input(6);
    } else {
        tt = random_truth_table(6);
        std::cout << "Случайный вектор значений (64):" << std::endl;
        for (int i = 0; i < tt.size(); ++i) {
            std::cout << tt[i];
            if ((i + 1) % 8 == 0) std::cout << " ";
        }
        std::cout << std::endl << std::endl;
    }

    std::cout << "Введённый вектор: ";
    for (int v : tt) std::cout << v;
    std::cout << std::endl;

    print_truth_table(tt);
    std::vector<int> anf = anf_from_truth_table(tt);
    print_anf(anf);

    return 0;
}