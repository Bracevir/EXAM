#define CURL_STATICLIB
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "func.h"
#include "curl/curl.h" // библиотека отвечает за работу с данными и файлами по url
#pragma comment(lib, "libcurl.lib")
#include "pugixml.hpp" // библиотека разборки xml
#pragma comment(lib, "pugixml.lib") // комментарий уточнения для поиска pugixml.hpp средой в файле компилированной библиотеки pugixml.lib
#pragma warning(disable : 4996)

using namespace pugi;

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) { // объект создан для использования библиотеки curl.h
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}


void showWallet(std::string path) { // эта функция считывает и выводит список курсов валдют из файла
	setlocale(LC_ALL, "russian");
	std::cout << "Курс валют:" << std::endl;
	xml_document doc;
	xml_parse_result result = doc.load_file("daily.xml"); // открытие файла по заданному пути
	if (!doc.load_file("daily.xml")) { // проверка открытия файла
		std::cout << "Не могу открыть файл.";
		throw std::invalid_argument("no file in directory"); // бросок исключения по открытию файла
	}
	xml_node Wallet = doc.child("ValCurs"); // выбор ноды из тега <ValCurs></ValCurs> в файле xml
	for (pugi::xml_node tool : Wallet.children("Valute")) // перебор дочерних тегов нод
	{
		std::cout << "Валюта: ";
		for (pugi::xml_attribute attr : tool.attributes()) // перебор атрибутов тегов (ID = число)
		{
			std::cout << " " << attr.name() << "=" << attr.value() << "\t";
		}
		for (pugi::xml_node child : tool.children()) // перебор строк в нодах для вывода
		{
			std::cout  << child.text().as_string() << "\t\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

/*template <typename T> T convertWallet(T num) { // Эта функция конвертирует валюту
	std::ifstream fin;
	fin.open(path);
	if (fin.is_open()) {
		std::cout << "Введите, какую валюту хотите конвертировать:" << std::endl;
		std::string str;
		std::cin >> str;
		if (str = 'rub' || str = 'RUB') {

		}
	}
	else
		std::cout << "Ошибка открытия файла." << std::endl;
	fin.close();
}*/

void newWallet() { // обновляет курсы валют
	CURL* curl = curl_easy_init();
	FILE* file = fopen("daily.xml", "wb");
	std::string readBuffer;
	curl_easy_setopt(curl, CURLOPT_URL, "https://www.cbr-xml-daily.ru/daily.xml");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	CURLcode response = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	fclose(file);

	std::cout << readBuffer << std::endl;
}