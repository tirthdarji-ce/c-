#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>

using namespace std;

// simple currency manipulator object and insertion operator
struct Currency { string sym; };
inline Currency currency(const string &s) { return Currency{s}; }
inline ostream& operator<<(ostream &os, const Currency &c) { return os << c.sym; }

struct Student {
    int roll = 0;
    string name;
    double m1 = 0, m2 = 0, m3 = 0;
    double fees = 0;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cout << "Enter number of students: ";
    if (!(cin >> n) || n <= 0) return 0;

    vector<Student> students;
    students.reserve(n);

    for (int i = 0; i < n; ++i) {
        Student s;
        cout << "\nStudent #" << (i + 1) << '\n';
        cout << " Roll number: ";        cin >> s.roll;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << " Name: ";              getline(cin, s.name);
        cout << " Marks (subject 1): "; cin >> s.m1;
        cout << " Marks (subject 2): "; cin >> s.m2;
        cout << " Marks (subject 3): "; cin >> s.m3;
        cout << " Fees: ";              cin >> s.fees;
        students.push_back(move(s));
    }

    const int wRoll = 6, wName = 24, wTotal = 8, wAvg = 10, wFees = 14;
    const int totalWidth = wRoll + wName + wTotal + wAvg + wFees;

    cout << '\n'
         << left << setw(wRoll) << "Roll"
         << setw(wName) << "Name"
         << right << setw(wTotal) << "Total"
         << setw(wAvg) << "Average"
         << setw(wFees) << "Fees" << '\n';

    cout << string(totalWidth, '-') << '\n';

    for (const auto &s : students) {
        double total = s.m1 + s.m2 + s.m3;
        double avg = total / 3.0;

        cout << left << setw(wRoll) << s.roll
             << setw(wName) << s.name;

        cout << right << setw(wTotal) << fixed << setprecision(1) << total
             << setw(wAvg) << fixed << setprecision(2) << avg;

        // demonstrate user-defined manipulator: symbol then aligned amount
        cout << currency("₹") << setw(wFees - 1) << fixed << setprecision(2) << s.fees
             << '\n';
    }

    cout << '\n';
    return 0;
}
