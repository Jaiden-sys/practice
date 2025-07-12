//Самое длинное слово
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    string word;
    ifstream text("input.txt");
    ofstream text2("1.txt");
    char ch;
    vector<string> words;
    if (text.is_open())
    {
        while (text >> word)
        {

            words.push_back(word);
            text2 << word << "\n";
        }
    }

    sort(words.begin(), words.end(), [](const string& a, const string& b) {
        return a.length() > b.length();
        });
    for (int i = 0; i < 5; i++)
    {
        cout << words[i] << "\n";
    }

}
