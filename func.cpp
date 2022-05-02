#define CURL_STATICLIB
#include <stdio.h>
#include <iostream>
#include <string>
#include "func.h"
/*библиотеки ниже можно добавить по ПКМ по проекту - Управление пакетами NuGet лоя решения - 
Поиск - установить по названиям curl и pugixml
файлы *.lib и libcurl-x64.dll положить в папку с проектом*/
#include "curl/curl.h" // библиотека отвечает за работу с данными и файлами по url
#pragma comment(lib, "libcurl.lib")
#include "pugixml.hpp" // библиотека разборки xml
#pragma comment(lib, "pugixml.lib") // комментарий уточнения для поиска pugixml.hpp средой в файле компилированной библиотеки pugixml.lib
#pragma warning(disable : 4996) // комментарий для пропуска проверки совместимости библиотек С в проекте из-за ошибки LNK4996

using namespace pugi; // подключение пространства имён pugixml

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) { // объект создан для использования библиотеки curl.h
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}


void showWallet() { // эта функция считывает и выводит список курсов валдют из файла
	
	xml_document doc; // создание объекта класса xml_document
	xml_parse_result result = doc.load_file("daily.xml"); // открытие файла по заданному пути
	if (!doc.load_file("daily.xml")) { // проверка открытия файла
		std::cout << "Не могу открыть файл.";
		throw std::invalid_argument("no file in directory"); // бросок исключения по открытию файла
	}
	xml_node Wallet = doc.child("ValCurs"); // выбор ноды из тега <ValCurs></ValCurs> в файле xml
	std::cout << "Курс валют " << Wallet.attribute("Date").as_string() << std::endl;
	for (pugi::xml_node tool : Wallet.children("Valute")) // перебор дочерних тегов нод
	{
		std::cout << "Валюта: ";
		for (pugi::xml_attribute attr : tool.attributes()) // перебор атрибутов тегов (ID = число)
		{
			std::cout << " " << attr.name() << "=" << attr.value() << "\t";
		}
		for (pugi::xml_node child : tool.children()) // перебор строк в нодах для вывода
		{
			std::cout << child.name() << ": " << child.text().as_string() << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void convertWallet() { // Эта функция конвертирует валюту
	std::cout << "Введите, в какую валюту хотите конвертировать по буквенному коду (пример USD):" << std::endl;
	std::string str;
	std::cin >> str;
	std::cout << "Сколько рублей хотите конвертировать?" << std::endl;
	double money, convertmoney=0;
	std::cin >> money;
	if (str == "rub" || str == "RUB") { // проверка на ввод рублей
		std::cout << "Это " << money << " рублей" << std::endl;
	}
	xml_document doc; // создание объекта класса xml_document
	xml_parse_result result = doc.load_file("daily.xml"); // открытие файла
	if (!doc.load_file("daily.xml")) { // проверка открытия файла
		std::cout << "Не могу открыть файл.";
		throw std::invalid_argument("no file in directory"); // бросок исключения по открытию файла
	}
	else {
		xml_node Wallet = doc.child("ValCurs"); // определение параметров искомой ноды Wallet
		std::string namestr;
		for (pugi::xml_node tool : Wallet.children("Valute")) { // перебор дочерних тегов нод
			if (str == tool.child("CharCode").text().as_string()) { // сравнение значения ноды с введённой строкой
				namestr = tool.child("Name").text().as_string(); // запись ноды Name в переменную типа строка
				convertmoney = money * tool.child("Nominal").text().as_double() / tool.child("Value").text().as_double(); // формула расчёта конвертации
			}
		}
		if (!convertmoney) std::cout << "Вы ввели неправильный буквенный код.";
		std::cout << money << " рублей " << " = " << convertmoney << " " << namestr << std::endl;
	}
}

void convertWalletIntoRub() { // конвертация валюты в рубли
	std::cout << "Введите, какую валюту хотите конвертировать по буквенному коду (пример USD):" << std::endl;
	std::string str;
	std::cin >> str;
	std::cout << "Сколько денег хотите конвертировать?" << std::endl;
	double money, convertmoney = 0;
	std::cin >> money;
	if (str == "rub" || str == "RUB") { // проверка на ввод рублей
		std::cout << "Это " << money << " рублей" << std::endl;
	}
	xml_document doc; // создание объекта класса xml_document
	xml_parse_result result = doc.load_file("daily.xml"); // открытие файла
	if (!doc.load_file("daily.xml")) { // проверка открытия файла
		std::cout << "Не могу открыть файл.";
		throw std::invalid_argument("no file in directory"); // бросок исключения по открытию файла
	}
	else {
		xml_node Wallet = doc.child("ValCurs"); // определение параметров искомой ноды Wallet
		std::string namestr;
		for (pugi::xml_node tool : Wallet.children("Valute")) { // перебор дочерних тегов нод
			if (str == tool.child("CharCode").text().as_string()) { // сравнение значения ноды с введённой строкой
				namestr = tool.child("Name").text().as_string(); // запись ноды Name в переменную типа строка
				convertmoney = money / tool.child("Nominal").text().as_double() * tool.child("Value").text().as_double(); // формула расчёта конвертации
			}
		}
		if (!convertmoney) std::cout << "Вы ввели неправильный буквенный код.";
		std::cout << money << " " << namestr << " это " << convertmoney << " в рублях. ";
	}
}

void newWallet() { // обновляет курсы валют
	CURL* curl = curl_easy_init(); // инициализация объекта curl
	FILE* file = fopen("daily.xml", "wb"); // открытие файла методом языка C для curl
	//std::string readBuffer;
	curl_easy_setopt(curl, CURLOPT_URL, "https://www.cbr-xml-daily.ru/daily.xml"); // запрос к URL
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // вызов объекта write_data для записи в файл
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, file); // запись файла
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); // запись строки
	CURLcode response = curl_easy_perform(curl); // тип перечисления CURLcode
	curl_easy_cleanup(curl); // очистка параметров curl для экономии памяти
	fclose(file); // закрытие файла

	//std::cout << readBuffer << std::endl;
}