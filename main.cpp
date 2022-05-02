#include <iostream>
#include "func.h"
#include <fstream>

using namespace std;

int main()
{
    setlocale(LC_ALL, "russian");
    cout << "Вы запустили конвертер валют." << endl;
    char sym;
    //string path = "daily.xml";
    cout << "Выберите, что хотите сделать:" << endl;
    cout << "1. Вывести список курсов валют к рублю" << endl << "2. Конвертировать рубли в валюту" << endl;
    cout << "3. Конвертировать валюту в рубли" << endl;
    cout << "4. Обновить курсы (ЗАКОММЕНТИРОВАНО)" << endl;
    cout << "5. Выход" << endl;
    cin >> sym;
    try { // вызовы функций в func.h
        switch (sym) {
        case '1': showWallet();
        case '2': convertWallet();
        case '3': convertWalletIntoRub();
        //case '4': newWallet();
        case '5': break;
        }
    }
    catch (const invalid_argument &ex) { // ловит исключения из функций
        cout << "Invalid argument: " << ex.what() << endl;
    }
    return 0;
}