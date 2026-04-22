#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ifstream f;
    string path, line;
    double num, sum = 0;
    int count = 0, invalid = 0;

    // retry for file
    while (true) {
        cout << "Enter file path (or exit): ";
        cin >> path;
        if (path == "exit") return 0;
        f.open(path);
        if (f) break;
        cout << "File not found! Try again.\n";
    }

    // read line by line
    while (f >> line) {
        try {
            num = stod(line);   // convert to number
            sum += num;
            count++;
        } catch (...) {
            cout << "Invalid: " << line << endl;
            invalid++;
        }
    }

    cout << "Sum = " << sum;
    if (count) cout << "\nAverage = " << sum / count;
    cout << "\nInvalid lines = " << invalid;

    return 0;
}