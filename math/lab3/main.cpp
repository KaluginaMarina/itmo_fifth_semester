#include <iostream>
#include <map>
#include <fstream>

struct symbol {     // структура для символа
    char c;         // символ
    int n;          // количество в тексте
    double p;       // вероятность
    double a_beg;   // начало отрезка
    double a_end;   // конец отрезка

    symbol(char c) : c(c) {
        n = 1;
    }
};

std::map<char, symbol> symbols;
//std::string str = "Pack my box with five dozen liquor jugs";
std::string str = "abcda";

void read_from_file() {                                  // считывание с файла
    std::string filename;
    //std::cin >> filename;
    //std::ifstream in(filename);
    std::ifstream in("../input/test.txt");
    int sum = 0;                                         // кол-во символов
    if (in.is_open()) {                                  // чтение посимвольно с файла и сохранение количества каждого типа символов в map
        while (!in.eof()) {
            char c = in.get();
            auto iter = symbols.find(c);
            if (iter != symbols.end()) {                  // если символ уже встречался -- увеличим счетчик
                iter->second.n++;
            } else {
                symbol s = symbol(c);                     // если символ еще не встречался, создадим элемент в мапе
                symbols.insert({c, s});
            }
            sum++;
        }
        for (auto &symbol : symbols) {                   // расчет вероятностей
            symbol.second.p = (double) symbol.second.n / (double) sum;
        }
    } else {
        std::cout << "Файл не найден";
    }
    in.close();
}

void make_segments() {                                   // создание границ отрезков
    double courser = 0;
    for (auto &s : symbols) {
        s.second.a_beg = courser;
        courser += s.second.p;
        s.second.a_end = courser;
    }
}

double code(double a_beg, double a_end, char *c) {      // кодирование информации
    auto it = symbols.find(*c);
    double new_a_beg = it->second.a_beg / (a_end - a_beg);
    double new_a_end = (it->second.a_end - it->second.a_beg) / (a_end - a_beg) + new_a_beg;
    c++;
    std::cout << *c << " a_beg " << a_beg << " a_end " << a_end << " new_a_beg " << new_a_beg << " new_a_end " << new_a_end << "\n";
    if (*c) {
        return code(new_a_beg, new_a_end, c);
    } else {
        return (new_a_beg + new_a_end) / 2;
    }
}

int main() {
    read_from_file();
    make_segments();
    code(0, 1, &str[0]);
    for (auto &s : symbols) {
        std::cout << s.first << " " << s.second.p << "\n";
    }
    return 0;
}