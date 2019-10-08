#pragma once
#include <iostream>
#include <math.h>
using namespace std;
#define MAX_SECURITY 9
#define MAX_LENGHT 100
#define INIT_ERROR 205
#define BIT_LENGHT 8
#define ERRORS 2

// N -> ukupan broj bita u kodovanoj reci
// K -> broj infromacionih bita 
// M -> broj zastitnih bita
// 2^m >= k + m + 1

class Heming {

private:

    int  n, k, sindrom, sindromLastBit, numOfSecurSecurityBits;
    short errors[ERRORS];
    bool d, infWord[MAX_LENGHT], securyBits[MAX_SECURITY] = { false };
    int codeWord[MAX_LENGHT] = { 0 }, codeWordWithError[MAX_LENGHT] = { 0 };

    void insertCodeWord();
    int calculateNumberOfSecurityBits(int k);
    void initCodeWord();
    void insertSecurityBits();
    bool bitPermutation(int e);
    void invertBit(int e, bool print = false);
    void findSindrom();
    void writeCommenar();


public:
    Heming(int n, int k, bool d);
    void code();
    ~Heming();

};