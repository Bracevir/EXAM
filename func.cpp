#define CURL_STATICLIB
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "func.h"
#include "curl/curl.h" // ���������� �������� �� ������ � ������� � ������� �� url
#pragma comment(lib, "libcurl.lib")
#include "pugixml.hpp" // ���������� �������� xml
#pragma comment(lib, "pugixml.lib") // ����������� ��������� ��� ������ pugixml.hpp ������ � ����� ��������������� ���������� pugixml.lib
#pragma warning(disable : 4996)

using namespace pugi;

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) { // ������ ������ ��� ������������� ���������� curl.h
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}


void showWallet(std::string path) { // ��� ������� ��������� � ������� ������ ������ ������ �� �����
	setlocale(LC_ALL, "russian");
	std::cout << "���� �����:" << std::endl;
	xml_document doc;
	xml_parse_result result = doc.load_file("daily.xml"); // �������� ����� �� ��������� ����
	if (!doc.load_file("daily.xml")) { // �������� �������� �����
		std::cout << "�� ���� ������� ����.";
		throw std::invalid_argument("no file in directory"); // ������ ���������� �� �������� �����
	}
	xml_node Wallet = doc.child("ValCurs"); // ����� ���� �� ���� <ValCurs></ValCurs> � ����� xml
	for (pugi::xml_node tool : Wallet.children("Valute")) // ������� �������� ����� ���
	{
		std::cout << "������: ";
		for (pugi::xml_attribute attr : tool.attributes()) // ������� ��������� ����� (ID = �����)
		{
			std::cout << " " << attr.name() << "=" << attr.value() << "\t";
		}
		for (pugi::xml_node child : tool.children()) // ������� ����� � ����� ��� ������
		{
			std::cout  << child.text().as_string() << "\t\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

/*template <typename T> T convertWallet(T num) { // ��� ������� ������������ ������
	std::ifstream fin;
	fin.open(path);
	if (fin.is_open()) {
		std::cout << "�������, ����� ������ ������ ��������������:" << std::endl;
		std::string str;
		std::cin >> str;
		if (str = 'rub' || str = 'RUB') {

		}
	}
	else
		std::cout << "������ �������� �����." << std::endl;
	fin.close();
}*/

void newWallet() { // ��������� ����� �����
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