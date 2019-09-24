#include <utility>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include <array>
#include <cmath>


struct symbol {     // структура для хранения символа
    char symb;      // что за символ
    int n;          // количество в тексте
    double p;       // вероятность появления
    std::string code_huff;  // код Хаффмана
    std::string code_sh_f;  // код Шеннона-Фано
    symbol(char s) {
        symb = s;
        n = 1;
    }

    symbol() {}

    bool operator==(const symbol &rhs) const {
        return symb == rhs.symb &&
               n == rhs.n &&
               p == rhs.p &&
               code_huff == rhs.code_huff &&
               code_sh_f == rhs.code_sh_f;
    }

    bool operator!=(const symbol &rhs) const {
        return !(rhs == *this);
    }
};

struct node {                   // узел дерева
    std::vector<symbol> symbs;  // символы в узле
    double p;                   // суммарная вероятность
    std::shared_ptr<node> child1;               // указатели на детей
    std::shared_ptr<node> child2;

    node() {}

    node(std::vector<symbol> s, double p, std::shared_ptr<node> child1, std::shared_ptr<node> child2) {
        symbs = std::move(s);
        this->p = p;
        this->child1 = child1;
        this->child2 = child2;
    };

    bool operator==(const node &rhs) const {
        return symbs == rhs.symbs &&
               p == rhs.p &&
               child1 == rhs.child1 &&
               child2 == rhs.child2;
    }

    bool operator!=(const node &rhs) const {
        return !(rhs == *this);
    }
};

std::map<char, symbol> symbols;                       // char -- символ, symbol -- структура с информацией о символе

void make_codes_huffman(std::shared_ptr<node> node, char code) {    // для создания кодов Хаффмана
    for (auto &s : node->symbs) {
        auto iter = symbols.find(s.symb);
        iter->second.code_huff += code;
    }
    if (!node->child1) {
        return;
    }
    make_codes_huffman(node->child1, '0');
    if (!node->child2) {
        return;
    }
    make_codes_huffman(node->child2, '1');
}


void huffman(std::map<char, symbol> &symbs) {        // Функция, высчитывающаяя код Хаффмана
    std::vector<std::shared_ptr<node>> nodes;
    for (auto &symb : symbs) {
        std::vector<symbol> s;
        s.push_back(symb.second);
        nodes.insert(nodes.end(),
                     std::make_shared<node>(s, symb.second.p, std::shared_ptr<node>(), std::shared_ptr<node>()));
    }
    // создание дерева
    for (int i = 0; i < symbs.size() - 2; ++i) {
        // найдем 1 минимальную, сохраним:
        double min = 2;
        std::shared_ptr<node> min_node;
        for (auto &node : nodes) {
            if (node->p <= min) {
                min = node->p;
                min_node = node;
            }
        }
        // удалим из вектора:
        nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [&](std::shared_ptr<node> const &n) {
                        return n == min_node;
                    }),
                    nodes.end());
        // найдем 2 минимальную, сохраним:
        min = 2;
        std::shared_ptr<node> min2_node;
        for (auto &node : nodes) {
            if (node->p <= min) {
                min = node->p;
                min2_node = node;
            }
        }
        // удалим из вектора:
        nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [&](std::shared_ptr<node> const &n) {
                        return n == min2_node;
                    }),
                    nodes.end());
        // создадим узел:
        std::vector<symbol> concat(min2_node->symbs.begin(), min2_node->symbs.end());
        concat.insert(concat.end(), min_node->symbs.begin(), min_node->symbs.end());
        nodes.insert(nodes.end(),
                     std::make_shared<node>(std::move(concat), min2_node->p + min_node->p, min_node, min2_node));
    }
    char code = '0';
    for (auto &node : nodes) {
        make_codes_huffman(node, code);
        code = (code == '0') ? '1' : '0';
    }
}


bool cmp(std::pair<char, double> p1, std::pair<char, double> p2) { // для сортировки массива с символами
    return p1.second > p2.second;
}

void shen_fan_splt(std::vector<std::pair<char, double>> smbs, char code) {
    double sum = 0;
    double sum1 = 0;
    std::vector<std::pair<char, double>> v1;
    std::vector<std::pair<char, double>> v2;
    for (auto &s : smbs) {
        sum += s.second;
        auto iter = symbols.find(s.first);
        iter->second.code_sh_f += code;             // добавляем букву в код
    }
    double sum2 = sum;
    if (smbs.size() == 1) {
        return;
    }
    for (auto &s : smbs) {               // разделяем вектор на 2
        if (sum1 < sum2) {
            v1.push_back(s);
            sum1 += s.second;
            sum2 -= sum1;
        } else {
            v2.push_back(s);
        }
    }
    shen_fan_splt(v1, '0');
    shen_fan_splt(v2, '1');
}

void shen_fan() {        // функция для расчета кодов Шеннона-Фано
    std::vector<std::pair<char, double>> smbs;
    for (auto &s : symbols) {
        smbs.emplace_back(std::pair<char, double>(s.first, s.second.p));
    }
    std::sort(smbs.begin(), smbs.end(), cmp);
    shen_fan_splt(smbs, ' ');
}


int main() {
    std::string filename;
    std::cin >> filename;
    //std::ifstream in("../input/1984_500.txt");
    std::ifstream in(filename);
    int sum = 0;                                     // кол-во символов
    if (in.is_open()) {                                  // чтение посимвольно с файла и сохранение количества каждого типа символов в map
        while (!in.eof()) {
            char c = in.get();
            auto iter = symbols.find(c);
            if (iter != symbols.end()) {                   // если символ уже встречался -- увеличим счетчик
                iter->second.n++;
            } else {
                symbol s = symbol(c);                    // если символ еще не встречался, создадим элемент в мапе
                symbols.insert({c, s});
            }
            sum++;
        }
        for (auto iter = symbols.begin(); iter != symbols.end(); iter++) {   // расчет вероятностей
            iter->second.p = (double) iter->second.n / (double) sum;
        }
    } else {
        std::cout << "Файл не найден";
    }
    in.close();

    huffman(symbols);


    std::cout << "Коды Хаффмана:\n";
    for (auto &s : symbols) {                                               // вывод для задания 1
        std::cout << "\"" << s.first << "\"" << "\t\t" << s.second.p << "\t\t" << s.second.code_huff << "\t\t\t"
                  << s.second.code_huff.size() << "\n";
    }
    std::cout << "Срeдняя длина кодов Хаффмана:\n";
    double sr = 0;                                                          // подсчет средней длины
    for (auto &s : symbols) {
        sr += s.second.p * s.second.code_huff.size();
    }
    std::cout << sr << "\n\n\n";

    shen_fan();

    std::cout << "Коды Шеннона-Фано:\n";                                    // вывод задания 2
    for (auto &s : symbols) {
        std::cout << "\"" << s.first << "\"" << "\t\t" << s.second.p << "\t\t" << s.second.code_sh_f << "\t\t\t"
                  << s.second.code_sh_f.size() - 1 << "\n";
    }
    sr = 0;
    std::cout << "Срeдняя длина кодов Шеннона-Фано:\n";
    for (auto &s : symbols) {
        sr += s.second.p * s.second.code_sh_f.size();
    }
    std::cout << sr << "\n\n\n";

    return 0;
}