#include <iostream>
#include <map>
#include <fstream>

struct symbol {     // структура для символа
    char c;         // символ
    int n;          // количество в тексте
    long double p;       // вероятность
    long double a_beg;   // начало отрезка
    long double a_end;   // конец отрезка

    symbol(char c) : c(c) {
        n = 1;
    }
};

std::map<char, symbol> symbols;
std::string str;

void read_from_file(std::string filename) {                                  // считывание с файла
    std::ifstream in(filename);
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

void read_string(std::string filename){
    std::ifstream in(filename);
    if (in.is_open()) {
        getline(in, str);
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

long double code(long double a_beg, long double a_end, char *c) {      // кодирование информации
    auto it = symbols.find(*c);
    long double  new_a_beg = a_beg + (a_end - a_beg) * it->second.a_beg;    // новые границы
    long double new_a_end = a_beg + (a_end - a_beg) * it->second.a_end;
    printf("%c [%.53Lf; %.53Lf) -> [%.53Lf; %.53Lf)\n", *c, a_beg, a_end, new_a_beg, new_a_end);
    c++;
    if (*c) {                                                               // продолжить рекурсивно, пока не найден конец файла
        return code(new_a_beg, new_a_end, c);
    } else {
        return (new_a_beg + new_a_end);
    }
}

std::string decode(long double res, long double a_beg, long double a_end) {

}

int main() {
    std::string filename = "../input/test.txt";
    //std::cin >> filename;
    read_from_file(filename);
    read_string(filename);
    make_segments();
    long double res = code(0, 1, &str[0]);
    std::cout << "Арифметическое кодирование фразы: \"" << str << "\"\n";
    std::cout << "Результат: ";
    printf("%.53Lf\n", res);
    //std::cout << "Коэффициент сжатия: " << (double)sizeof(res)/ sizeof(str);
    return 0;
}