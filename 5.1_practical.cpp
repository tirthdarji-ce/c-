#include <iostream>
#include <string>
using namespace std;

int main() {
    string paragraph;
    cout << "Enter paragraph:\n";
    getline(cin, paragraph);

    string words[100];
    int freq[100] = {0};
    int count = 0;

    string temp = "";

    for (int i = 0; i <= paragraph.length(); i++) {
        if (paragraph[i] == ' ' || paragraph[i] == '\0') {
            if (temp != "") {

                // Convert to lowercase
                for (int k = 0; k < temp.length(); k++) {
                    if (temp[k] >= 'A' && temp[k] <= 'Z') {
                        temp[k] = temp[k] + 32;
                    }
                }

                // Check if word exists
                int found = -1;
                for (int j = 0; j < count; j++) {
                    if (words[j] == temp) {
                        found = j;
                        break;
                    }
                }

                if (found != -1) {
                    freq[found]++;
                } else {
                    words[count] = temp;
                    freq[count] = 1;
                    count++;
                }

                temp = "";
            }
        } else {
            temp += paragraph[i];
        }
    }

    cout << "\nWord Frequency:\n";
    for (int i = 0; i < count; i++) {
        cout << words[i] << " : " << freq[i] << endl;
    }

    return 0;
}