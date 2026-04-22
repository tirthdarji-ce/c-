#include <iostream>
#include <map>
#include <vector>
using namespace std;

int main() {
    map<string, vector<string>> dir;
    int choice;
    string folder, file;

    while (true) {
        cout << "\n1.Add Folder  2.Add File  3.Display  4.Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Folder name: ";
            cin >> folder;
            dir[folder]; // creates folder
        }
        else if (choice == 2) {
            cout << "Folder: "; cin >> folder;
            cout << "File: "; cin >> file;
            dir[folder].push_back(file);
        }
        else if (choice == 3) {
            cout << "\nDirectory:\n";
            for (auto p : dir) {
                cout << p.first << ":\n";
                for (auto f : p.second)
                    cout << "  " << f << endl;
            }
        }
        else break;
    }

    return 0;
}