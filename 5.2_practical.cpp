#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    ifstream file("input.txt");

    // Error handling
    if (!file) {
        cout << "Error: Cannot open file!" << endl;
        return 1;
    }

    string line;
    int lineCount = 0, wordCount = 0, charCount = 0;

    // Read file line-by-line
    while (getline(file, line)) {
        lineCount++;

        bool inWord = false;

        // Scan each character manually
        for (int i = 0; i < line.length(); i++) {
            charCount++;  // count characters

            if (line[i] != ' ' && line[i] != '\t') {
                if (!inWord) {
                    wordCount++;   // new word detected
                    inWord = true;
                }
            } else {
                inWord = false;
            }
        }
    }

    file.close();

    // Output results
    cout << "Total Lines: " << lineCount << endl;
    cout << "Total Words: " << wordCount << endl;
    cout << "Total Characters: " << charCount << endl;

    return 0;
}