#include <utility>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>

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

void make_codes_huffman(std::shared_ptr<node> node, char code) {
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


int main() {
    std::ifstream in("../input/1984_500.txt");
    if (in.is_open()) {                                  // чтение посимвольно с файла и сохранение количества каждого типа символов в map
        int sum = 0;                                     // кол-во символов
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

    for (auto &s : symbols) {
        std::cout << s.first << " " << s.second.code_huff << "\n";
    }
    return 0;
}