#include <iostream>
#include <bitset>
#include <map>

// Функция, определяющая вычет (остаток от деления с XOR)
// ОПРЕДЕЛЯЕТ ОБРАЗУЮЩИЙ ПОЛИНОМ
template <std::size_t  N>
int vichet(std::bitset<N> result_vector) {

    std::bitset<15> res, obr_polinom("10011");

    obr_polinom <<= 10;
    std:: cout << "Образующий полином:                " << obr_polinom << std::endl;

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
    std::cout << "Синдром ошибки:                    " << result_vector << std::endl << std::endl;
    return result_vector.to_ulong();
}

template <std::size_t  N>
void ispravlenie(std::bitset<5> sindrom, std::bitset<N> *result_vector) {

}

// Функция, которая портит информационный вектор и исправляет
// Смежные вызовы: vichet, ispravlenie
template <std::size_t N>
void get_info(std::bitset<N> e, int v, std::bitset<N> result_vector, std::bitset<N> code_vector) {
    std::bitset<5> sindrom;

    // Портим информационный вектор
    e = v;
    result_vector = e ^ code_vector;

    std::cout << "Информационная последовательность: " << code_vector << std::endl <<
                 "Вектор ошибки:                     " << e << std::endl <<
                 "Принятая последовательность:       " << result_vector << std::endl;

    // Делим порченный информационный вектор на образующий полином
    sindrom = vichet(result_vector);
    std::cout << "result: " << result_vector << std::endl <<
                  "sindrom: " << sindrom;
}

// Функция заполнения таблички с векторами синдромов s
// На вход: map<разряд с ошибкой в коде, синдром>
void fulfillment(std::map<int, std::bitset<5>> *s) {

    for (int i = 1; i <= 15; i++) {
        s->insert(std::pair<int, std::bitset<5>> (i-1, i));
    }
    //for (auto it = s->begin(); it != s->end(); it++) {
     //   std:: cout << it->first << " : " << it->second << std::endl;
    //}
}

int main() {

    #define size 15

    std::bitset<11> inf_vector("1010011");
    std::bitset<15> result_vector, e, code_vector("000010100110111");
    int N[size], Ck[size], Cim[size];

    std::map<int, std::bitset<5>> s;
    fulfillment(&s);


    for (int i = 1; i <= 1; i++) {
        // Цикл перебора комбинаций ошибок
        // v - вектор, состоящий из 1 по количеству ошибок n
        // t - вектор, смещающий крайнюю единицу
        for (int v = (1 << i) - 1; v <= (1 << size) ; ) {
            unsigned int t = (v | (v - 1)) + 1;
            v = t | ((((t & -t) / (v & -v)) >> 1) - 1); //смещение крайней правой единицы влево пока не получится "(1)"

            if (t == 1 << size) {
                v = (1 << i) - 1;
                get_info(e, v, result_vector, code_vector);
                break;
            }

            //get_info(e, v, result_vector, code_vector);
        }
    }



    return 0;
}


