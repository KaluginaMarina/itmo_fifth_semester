#include<iostream>
#include "Heming.h"
using  namespace std;

#define N 40


int main() {
    cout << "=========Код Хэмминга=========" << endl << endl;
    // (7,4) (15,11) (31,26) -> bez bita parnosti
    // (8,4) (16,11) (32,26) -> sa  bit  parnosti
    // skraceni (x,y) .. x je manje nego sto je obicno ako je 1 to je onda obican kod sa ponavljanjem!



    /***********************USER INSERT*******************************/
    int n;
    cout << "Введите длинну кода (минимум 7)" << endl;
    cin >> n;

    while (n < 7) {
        cout << "Число меньше 7, введите еще раз" << endl;
        cin >> n;
    }

    char yn;
    bool detection;

    cout << "Обнаружение 2 ошибок? (Y/N)" << endl;

    while (true) {
        cin >> yn;

        if (yn == 'y' || yn == 'Y' || yn == '1')
            detection = true;
        else if (yn == 'n' || yn == 'N' || yn == '0')
            detection = false;

        if (yn == 'y' || yn == 'Y' || yn == 'n' || yn == 'N') break;

        cout << "Неверный ввод. Попробуйте еще раз." << endl;

    }

    int k;
    cout << "Введите длину информационного сообщения" << endl;
    cin >> k;

    int uslovK = k + ceil(log2(k + 1)) + detection;
    cout << "Состояние К: " << uslovK << endl;
    while (k < 1 || uslovK > n) {
        cout << "Введите повторно К" << endl;
        cin >> k;
        uslovK = k + ceil(log2(k + 1)) + detection;
    }

    /*********************END USERR INSERT******************************/

    Heming* H = new Heming(n, k, detection);
    H->code();

    return 0;


}

