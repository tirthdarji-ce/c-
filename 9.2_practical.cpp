#include <iostream>
#include <map>
#include <sstream>
using namespace std;

int main() {
    string line, word;
    map<string, int> freq;

    cout << "Enter a sentence: ";
    getline(cin, line);

    stringstream ss(line);
    while (ss >> word)
        freq[word]++;

    cout << "\nWord Frequency:\n";
    for (auto p : freq)
        cout << p.first << " : " << p.second << endl;

    return 0;
}