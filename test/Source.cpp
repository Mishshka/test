#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <windows.h>

using namespace std;

// Функция перекодирования строки из CP1251 в UTF-8
string cp1251_to_utf8(const string& str1251)
{
	if (str1251.empty()) return "";

	//CP1251 -> UTF-16 |wchar|
	int wsize = MultiByteToWideChar(1251, 0, str1251.c_str(), -1, 0, 0);
	wstring wstr(wsize, 0);
	MultiByteToWideChar(1251, 0, str1251.c_str(), -1, &wstr[0], wsize);

	//UTF-16 -> UTF-8
	int u8size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, 0, 0, 0, 0);
	string utf8str(u8size, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8str[0], u8size, 0, 0);

	if (!utf8str.empty() && utf8str.back() == '\0')
		utf8str.pop_back();

	return utf8str;
}

// Принимает вектор CP1251 строк, возвращает вектор IPA-транскрипций (!!!!!!!!!!!!!!!!!!!!в UTF-8)
vector <string> get_transcript(vector<pair<string, int>> words)
{
	vector <string> transcriptions;

	if (words.empty()) return transcriptions;

	const string input = "tmp_words.txt";
	const string output = "tmp_transcript";

	ofstream outFile(input);
	for (const auto& word : words)
		outFile << cp1251_to_utf8(word.first) << endl;
	outFile.close();

	string exePath = "eSpeak NG\\espeak-ng.exe";
	string com = "\"\"" + exePath + "\" -v ru --ipa --path=\"eSpeak NG\" -f " + input + " -q > " + output + "\"";
	system(com.c_str());

	ifstream inFile(output);
	string token;
	while (inFile >> token)
	{
		transcriptions.push_back(token);
	}
	inFile.close();

	remove(input.c_str());
	remove(output.c_str());

	return transcriptions;
}

int main() 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	
	vector<pair<string, int>> words = { {"Лес",13},{"рюмка",15},{"глава",19},{"слякоть",21} };

	vector<string> results = get_transcript(words);

	SetConsoleOutputCP(CP_UTF8);

	for (size_t i = 0; i < words.size() && i < results.size(); ++i) {
		cout << " [" << results[i] << "]\n";
	}

	return 0;
}