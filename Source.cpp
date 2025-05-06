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
        cerr << "�� ������� ������� ����.\n";
        return 1;
    }

    map<string, int> wordCount;
    string line, word;

    while (getline(file, line)) {
        stringstream ss(line);
        while (ss >> word) {
            // ��������� ������� ������ �� ���� � ������� �����
            string cleaned;
            for (char c : word) {
                if (c != '.' && c != ',') {
                    cleaned.push_back(c);
                }
            }

            // ������������ �� ������� �� ����� ������
            for (auto& ch : cleaned) {
                if (ch >= '�' && ch <= '�') {
                    ch = ch - '�' + '�';
                }
            }

            if (!cleaned.empty()) {
                ++wordCount[cleaned];
            }
        }
    }
    file.close();

    // ���� ����������
    for (const auto& entry : wordCount) {
        cout << entry.first << ": " << entry.second << "\n";
    }

    return 0;
}
