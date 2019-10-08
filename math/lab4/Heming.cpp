#include <iostream>
#include <math.h>
#include <bitset>
#include "Heming.h"
using namespace std;



int Heming::calculateNumberOfSecurityBits(int k) {
    if (k >= 2 && k <= 4) {
        return 3;
    }
    else if (k >= 5 && k <= 11) {
        return 4;
    }
    else if (k >= 12 && k <= 26) {
        return 5;

    }
    else if (k >= 27 && k <= 57) {
        return 6;
    }
    else if (k == 1) {
        return 2;
    }
}

void Heming::insertCodeWord() {
    cout << "Введите код по битам:" << endl;
    for (int i = 0; i < k; ) {
        char temp;
        cin >> temp;

        if (temp == '0')      infWord[i++] = false;
        else if (temp == '1') infWord[i++] = true;
        else                  cout << "Введите " << i+1 << "-тый бий информационного слова" << endl;
    }

    cout << endl << "=======Решение=======" << endl;

    this->numOfSecurSecurityBits = calculateNumberOfSecurityBits(k);

    for (int i = 1, maska = 1, j = 1, p = 1; i <= n; i++) {
        string binary = bitset<BIT_LENGHT>(i).to_string();
        cout << i << "\t" << binary<< "\t";
        if (i & maska) {
            if (j > numOfSecurSecurityBits)
                cout << "---Z" << endl;
            else
                cout << "Z" << j << endl;
            maska <<= 1;
            j++;
        }
        else {
            if(p > k)
                cout << "---" << endl;
            else
                cout << "I" << p << endl;
            p++;
        }
    }

    n = k + numOfSecurSecurityBits;
}

void Heming::initCodeWord() {
    int maska = 1, j = 0;

    for (int i = 0; i < n; i++) {
        if ((i + 1) & maska) {
            codeWord[i] = -1;
            maska <<= 1;
        }
        else {
            codeWord[i] = infWord[j++];
        }
    }

    cout << "Кодовое слова без бит: " << endl;

    for (int i = 0, j = 0, maska = 1; i < n; i++) {
        if ((i + 1) & maska) {
            cout << "Z" << ++j << " ";
            maska <<= 1;
        }
        else
        if (codeWord[i] == INIT_ERROR)
            cout << "X" << "  ";
        else
            cout << codeWord[i] << "  ";
    }
    if (d)
        cout << "ZP";

    cout << endl;
}

void Heming::insertSecurityBits() {
    for (int i = 0, maska = 1; i < numOfSecurSecurityBits; i++, maska <<=1) {
        for (int j = 0; j < n; j++) {
            if (codeWord[j] != -1 && (j+1)&maska ) {
                if (codeWord[j] == INIT_ERROR) continue;
                securyBits[i] ^= codeWord[j];
            }
        }
    }
    for (int i = 0, j = 0; i < n; i++) {
        if (codeWord[i] == -1) {
            if(codeWord[i] != INIT_ERROR)
                codeWord[i] = securyBits[j++];
        }
    }

    if (d) {
        for (int i = 0; i < n; i++)
            if(codeWord[i] != INIT_ERROR)
                securyBits[numOfSecurSecurityBits] ^= codeWord[i];

        codeWord[n] = securyBits[numOfSecurSecurityBits];
    }

    cout << "Защитный бит: " << endl;
    for (int i = 0; i < numOfSecurSecurityBits + d; i++)
        cout << "Z" << i + 1 << " = " << securyBits[i] << endl;
    cout << endl;

    //Ispis poruke koju trebamo da prenesemo (jos se nije desila greska)!
    n += d;
    cout << "Кодовое слово из N = " << n << " бит:"  << endl;

    for (int i = 0; i < n; i++) {
        cout << codeWord[i] << "  ";
    }
    cout << endl;
}

void Heming::invertBit(int e, bool print) {
    for (int i = 0; i < n; i++) {
        if (i + 1 == e) {
            if (codeWordWithError[i] == 0)
                codeWordWithError[i] = 1;
            else
                codeWordWithError[i] = 0;
        }
    }

    if (print) {
        cout << "Код после исправления ошибки: " << endl;
        for (int i = 0; i < n; i++)
            cout << codeWordWithError[i] << "  ";
        cout << endl;
    }
}
bool Heming::bitPermutation(int e) {

    for (int i = 0;i < n + d;i++)
        codeWordWithError[i] = codeWord[i];

    if (e == -1) {
        short numOfError = 0;
        cout << "Введите кол-во ошибок(1 или 2)" << endl;
        cin >> numOfError;

        if (numOfError == 0) {
            cout << "Нет ошибки" << endl;
            return false;
        }
        for (short i = 0; i < numOfError; i++) {

            cout << "Введите позицию " << i+1 << ". ошибки: " << endl;
            cin >> errors[i];

            while (errors[i] < 0 || errors[i] > n + d) {
                cout << "Введено неверное значение для позиции ошибки" << endl;
                cin >> errors[i];
            }
            if(errors[i] != 0)
                invertBit(errors[i],false);
        }
    }


    cout << "Код ошибки: " << endl;
    for (int i = 0; i < n; i++)
        cout << codeWordWithError[i] << "  ";
    cout << endl;

    return true;
}

void Heming::findSindrom() {

    int lengthSyndrom = ceil(log2(n));

    sindrom = 0;

    for (int i = 0, s = 0, maska = 1; i < lengthSyndrom; i++, s = 0, maska <<= 1) {
        for (int j = 0; j < n; j++) {
            //cout << codeWordWithError[j] << " + ";
            if (((j + 1)&maska) && (codeWordWithError[j] != INIT_ERROR)) {
                s ^= codeWordWithError[j];
            }
        }
        //cout << endl;
        sindrom |= (s << i);

    }
    cout << "Синдром: " << sindrom << endl;

    if (d) {
        sindromLastBit = 0;
        for (int i = 0;i < n + d;i++)
            sindromLastBit ^= codeWordWithError[i];
        cout << "Синдром последнего бита: " << sindromLastBit << endl; //TEST
    }
}

void Heming::writeCommenar() {
    if (d) {
        cout << "Последний бит: :" << codeWordWithError[n - 1] << endl;
        if (sindrom == 0) {
            if (sindromLastBit == 0)
                cout << "Не было ошибки!" << endl;
            else
                cout << "Ошибка на бит четности!" << endl;
        }
        else {
            if (sindromLastBit == 0)
                cout << "Четное кол-во ошибок. Исправленный код недействителен, синдром не показывает позицию ошибки." << endl;
            else
                cout << "Странное количество ошибок. Синдром показывает на ошибку. Ошибка была на " << sindrom << "-бите" << endl;
        }

    }
    else {
        if (sindrom == 0) {
            cout << "Там не было ошибки." << endl;
        }
        else cout << "Проихошли ошибки в бите " << sindrom  << endl;
    }
}

Heming::Heming(int n, int k, bool d) {
    this->n = n;
    this->k = k;
    this->d = d;
}

void Heming::code() {

    insertCodeWord();
    initCodeWord();
    insertSecurityBits();

    if (bitPermutation(-1)) {
        findSindrom();
        invertBit(sindrom, true);
        writeCommenar();
    }
}

Heming::~Heming() {
    n = k = d = 0;

    delete infWord;
    delete securyBits;
    delete codeWord;
    delete codeWordWithError;
}

