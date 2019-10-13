#include <iostream>
#include <bitset>
#include <cmath>
#include <iomanip>

// Факториал
double factorial(int n)
{
    if (n != 0) {
        if (n == 1)
            return 1;
        else
            return n * factorial(n - 1);
    }
}
// Сочетания из n по i (n - 15-значный код, i - кратность ошибки)
void fulfillment(int *Cin, int i) {
    Cin[i] = ceil(factorial(15) / (factorial(15-i) * factorial(i)));
}


// Функция, вычисляющая вычет (остаток от деления с XOR)
// ОПРЕДЕЛЯЕТ ОБРАЗУЮЩИЙ ПОЛИНОМ
template <std::size_t  N>
int vichet(std::bitset<N> result_vector) {

    std::bitset<15> res, obr_polinom("10011");

    obr_polinom <<= 10;
    //std:: cout << "Образующий полином:                " << obr_polinom << std::endl;

    for (int i = 14; i >= 4; i--) {
        if (result_vector[i] == 1) {
            result_vector ^= obr_polinom;
            res <<= 1;
            res.set(0);
            obr_polinom >>= 1;
        }
        else {
            obr_polinom >>= 1;
            res <<= 1;
        }
    }
    //std::cout << "Синдром ошибки:                    " << result_vector << std::endl << std::endl;
    return result_vector.to_ulong();
}


// Инвертирует бит по синдрому
// Исправляет одну ошибку
template <std::size_t  N>
void ispravlenie(std::bitset<5> sindrom, std::bitset<N> *result_vector) {
    result_vector->flip(sindrom.to_ulong() - 1);
}


// Функция, которая портит информационный вектор и исправляет его
template <std::size_t N>
void get_info(std::bitset<N> e, int v, std::bitset<N> result_vector, std::bitset<N> code_vector, int i, int *Nk) {
    std::bitset<5> sindrom;

    // Портим информационный вектор
    e = v;
    result_vector = e ^ code_vector;

    //std::cout << "Информационная последовательность: " << code_vector << std::endl <<
    //            "Вектор ошибки:                     " << e << std::endl <<
    //             "Принятая последовательность:       " << result_vector << std::endl;

    // Делим порченный информационный вектор на образующий полином и получаем синдром
    // столько раз, сколько ошибок в векторе е(х)
    // если синдром = 0, то считаем что ошибок больше нет и исправлять нечего - выходим
    for (int j = i; j >= 1; j--) {
        sindrom = 0;
        sindrom = vichet(result_vector);
        //std::cout << "result: " << result_vector << std::endl <<
        //          "sindrom: " << sindrom << std::endl;
        if (sindrom != 0) {
            ispravlenie(sindrom, &result_vector);
            //std::cout << "result: " << result_vector << std::endl;
        }
        else break;
    }

    if (result_vector.to_ulong() == code_vector.to_ulong()) {
        Nk[i]++;
    }
}



int main() {

    #define size 16

    std::bitset<11> inf_vector("1010011");
    std::bitset<15> result_vector, e, code_vector("000010100110111");
    static int N[size], Cin[size];
    double Ck[size];

    for (int i = 1; i <= 15; i++) {
        // Цикл перебора комбинаций ошибок
        // v - вектор, состоящий из 1 по количеству ошибок n
        // t - вектор, смещающий крайнюю единицу
        for (int v = (1 << i) - 1; v <= (1 << size) ; ) {
            unsigned int t = (v | (v - 1)) + 1;
            v = t | ((((t & -t) / (v & -v)) >> 1) - 1); //смещение крайней правой единицы влево пока не получится "(1)"

            if (t == 1 << size) {
                v = (1 << i) - 1;
                get_info(e, v, result_vector, code_vector, i, N);
                break;
            }
            get_info(e, v, result_vector, code_vector, i, N);
        }

        fulfillment(Cin, i);

        Ck[i] = double(N[i]) / double(Cin[i]);
    }


    std::cout << std::setw(8) << "Кратность" << std::setw(8) << "Nk" << std::setw(8)
              <<  "Cik" << std::setw(12) <<  "Ck" << std::endl;
    for (int i = 1; i <= 15; i++) {
        std::cout << std::setw(8) << i << std::setw(8) << N[i] << std::setw(8) <<  Cin[i]
                  << std::setw(12) << std::setprecision(5)<< Ck[i] << std::endl;
    }

    return 0;
}


