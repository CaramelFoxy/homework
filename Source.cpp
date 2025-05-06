#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <Windows.h>

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    ifstream file("input.txt");
    if (!file) {
        cerr << "Не вдалося відкрити файл.\n";
        return 1;
    }

    map<string, int> wordCount;
    string line, word;

    while (getline(file, line)) {
        stringstream ss(line);
        while (ss >> word) {
            // Видаляємо символи крапки та коми з кожного слова
            string cleaned;
            for (char c : word) {
                if (c != '.' && c != ',') {
                    cleaned.push_back(c);
                }
            }

            // Перетворюємо всі символи на нижній регістр
            for (auto& ch : cleaned) {
                if (ch >= 'А' && ch <= 'Я') {
                    ch = ch - 'А' + 'а';
                }
            }

            if (!cleaned.empty()) {
                ++wordCount[cleaned];
            }
        }
    }
    file.close();

    // Вивід результату
    for (const auto& entry : wordCount) {
        cout << entry.first << ": " << entry.second << "\n";
    }

    return 0;
}
