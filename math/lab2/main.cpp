#include <iostream>
#include <fstream>
#include <map>

struct symbol {      // структура для хранения символа
    char symb;      // что за символ
    int n;          // количество в тексте
    double p;       // вероятность появления
    std::string code_huff;  // код Хаффмана
    std::string code_sh_f;  // код Шеннона-Фано
    symbol(char s) {
        symb = s;
        n = 1;
    }
};


int main() {
    std::map<char, symbol> symbs;                       // char -- символ, symbol -- структура с информацией о символе
    std::ifstream in("../input/1984_500.txt");
    if (in.is_open()) {                                  // чтение посимвольно с файла и сохранение количества каждого типа символов в map
        while (!in.eof()) {
            char c = in.get();
            auto iter = symbs.find(c);
            if (iter != symbs.end()) {                   // если символ уже встречался -- увеличим счетчик
                iter->second.n++;
            } else {
                symbol s = symbol(c);                    // если символ еще не встречался, создадим элемент в мапе
                symbs.insert({c, s});
            }
        }
    } else {
        std::cout << "Файл не найден";
    }
    in.close();

    for (auto it = symbs.begin(); it != symbs.end(); ++it){
        std::cout << it->first << " " << it->second.n << std::endl;
    }
    return 0;
}