#define CURL_STATICLIB
#include <stdio.h>
#include <iostream>
#include <string>
#include "func.h"
/*���������� ���� ����� �������� �� ��� �� ������� - ���������� �������� NuGet ��� ������� - 
����� - ���������� �� ��������� curl � pugixml
����� *.lib � libcurl-x64.dll �������� � ����� � ��������*/
#include "curl/curl.h" // ���������� �������� �� ������ � ������� � ������� �� url
#pragma comment(lib, "libcurl.lib")
#include "pugixml.hpp" // ���������� �������� xml
#pragma comment(lib, "pugixml.lib") // ����������� ��������� ��� ������ pugixml.hpp ������ � ����� ��������������� ���������� pugixml.lib
#pragma warning(disable : 4996) // ����������� ��� �������� �������� ������������� ��������� � � ������� ��-�� ������ LNK4996

using namespace pugi; // ����������� ������������ ��� pugixml

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) { // ������ ������ ��� ������������� ���������� curl.h
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}


void showWallet() { // ��� ������� ��������� � ������� ������ ������ ������ �� �����
	
	xml_document doc; // �������� ������� ������ xml_document
	xml_parse_result result = doc.load_file("daily.xml"); // �������� ����� �� ��������� ����
	if (!doc.load_file("daily.xml")) { // �������� �������� �����
		std::cout << "�� ���� ������� ����.";
		throw std::invalid_argument("no file in directory"); // ������ ���������� �� �������� �����
	}
	xml_node Wallet = doc.child("ValCurs"); // ����� ���� �� ���� <ValCurs></ValCurs> � ����� xml
	std::cout << "���� ����� " << Wallet.attribute("Date").as_string() << std::endl;
	for (pugi::xml_node tool : Wallet.children("Valute")) // ������� �������� ����� ���
	{
		std::cout << "������: ";
		for (pugi::xml_attribute attr : tool.attributes()) // ������� ��������� ����� (ID = �����)
		{
			std::cout << " " << attr.name() << "=" << attr.value() << "\t";
		}
		for (pugi::xml_node child : tool.children()) // ������� ����� � ����� ��� ������
		{
			std::cout << child.name() << ": " << child.text().as_string() << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void convertWallet() { // ��� ������� ������������ ������
	std::cout << "�������, � ����� ������ ������ �������������� �� ���������� ���� (������ USD):" << std::endl;
	std::string str;
	std::cin >> str;
	std::cout << "������� ������ ������ ��������������?" << std::endl;
	double money, convertmoney=0;
	std::cin >> money;
	if (str == "rub" || str == "RUB") { // �������� �� ���� ������
		std::cout << "��� " << money << " ������" << std::endl;
	}
	xml_document doc; // �������� ������� ������ xml_document
	xml_parse_result result = doc.load_file("daily.xml"); // �������� �����
	if (!doc.load_file("daily.xml")) { // �������� �������� �����
		std::cout << "�� ���� ������� ����.";
		throw std::invalid_argument("no file in directory"); // ������ ���������� �� �������� �����
	}
	else {
		xml_node Wallet = doc.child("ValCurs"); // ����������� ���������� ������� ���� Wallet
		std::string namestr;
		for (pugi::xml_node tool : Wallet.children("Valute")) { // ������� �������� ����� ���
			if (str == tool.child("CharCode").text().as_string()) { // ��������� �������� ���� � �������� �������
				namestr = tool.child("Name").text().as_string(); // ������ ���� Name � ���������� ���� ������
				convertmoney = money * tool.child("Nominal").text().as_double() / tool.child("Value").text().as_double(); // ������� ������� �����������
			}
		}
		if (!convertmoney) std::cout << "�� ����� ������������ ��������� ���.";
		std::cout << money << " ������ " << " = " << convertmoney << " " << namestr << std::endl;
	}
}

void convertWalletIntoRub() { // ����������� ������ � �����
	std::cout << "�������, ����� ������ ������ �������������� �� ���������� ���� (������ USD):" << std::endl;
	std::string str;
	std::cin >> str;
	std::cout << "������� ����� ������ ��������������?" << std::endl;
	double money, convertmoney = 0;
	std::cin >> money;
	if (str == "rub" || str == "RUB") { // �������� �� ���� ������
		std::cout << "��� " << money << " ������" << std::endl;
	}
	xml_document doc; // �������� ������� ������ xml_document
	xml_parse_result result = doc.load_file("daily.xml"); // �������� �����
	if (!doc.load_file("daily.xml")) { // �������� �������� �����
		std::cout << "�� ���� ������� ����.";
		throw std::invalid_argument("no file in directory"); // ������ ���������� �� �������� �����
	}
	else {
		xml_node Wallet = doc.child("ValCurs"); // ����������� ���������� ������� ���� Wallet
		std::string namestr;
		for (pugi::xml_node tool : Wallet.children("Valute")) { // ������� �������� ����� ���
			if (str == tool.child("CharCode").text().as_string()) { // ��������� �������� ���� � �������� �������
				namestr = tool.child("Name").text().as_string(); // ������ ���� Name � ���������� ���� ������
				convertmoney = money / tool.child("Nominal").text().as_double() * tool.child("Value").text().as_double(); // ������� ������� �����������
			}
		}
		if (!convertmoney) std::cout << "�� ����� ������������ ��������� ���.";
		std::cout << money << " " << namestr << " ��� " << convertmoney << " � ������. ";
	}
}

void newWallet() { // ��������� ����� �����
	CURL* curl = curl_easy_init(); // ������������� ������� curl
	FILE* file = fopen("daily.xml", "wb"); // �������� ����� ������� ����� C ��� curl
	//std::string readBuffer;
	curl_easy_setopt(curl, CURLOPT_URL, "https://www.cbr-xml-daily.ru/daily.xml"); // ������ � URL
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // ����� ������� write_data ��� ������ � ����
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, file); // ������ �����
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); // ������ ������
	CURLcode response = curl_easy_perform(curl); // ��� ������������ CURLcode
	curl_easy_cleanup(curl); // ������� ���������� curl ��� �������� ������
	fclose(file); // �������� �����

	//std::cout << readBuffer << std::endl;
}