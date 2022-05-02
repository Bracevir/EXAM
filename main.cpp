#include <iostream>
#include "func.h"
#include <fstream>

using namespace std;

int main()
{
    setlocale(LC_ALL, "russian");
    cout << "Вы запустили конвертер валют." << endl;
    char sym;
    string path = "daily.xml";
    cout << "Выберите, что хотите сделать:" << endl;
    cout << "1. Вывести список курсов валют к рублю" << endl << "2. Конвертировать валюту" << endl;
    cout << "3. Обновить курсы" << endl;
    cin >> sym;
    switch (sym) {
    case '1': showWallet(path); break;
    //case '3': newWallet();
    }



    return 0;
}