#include <iostream>
using namespace std;

class Employee {
private:
    string name;
    double basicSalary;
    double bonus;

public:
    Employee(string n = "Unknown", double basic = 0, double b = 1000) {
        name = n;
        basicSalary = basic;
        bonus = b;
    }
    inline double calculateSalary() {
        return basicSalary + bonus;
    }
    void display() {
        cout << "Name: " << name << endl;
        cout << "Basic Salary: " << basicSalary << endl;
        cout << "Bonus: " << bonus << endl;
        cout << "Total Salary: " << calculateSalary() << endl;
    }
};
int main() {
    int n;
    cout << "Enter number of employees: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        string name;
        double basic, bonus;
        char choice;

        cout << "\nEnter details for Employee " << i + 1 << endl;

        cout << "Name:";
        cin >> name;

        cout << "Basic Salary:";
        cin >> basic;

        cout << "Custom bonus(y/n): ";
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            cout << "Enter Bonus: ";
            cin >> bonus;
            Employee emp(name, basic, bonus);
            emp.display();
        } else {
            Employee emp(name, basic);
            emp.display();
        }
    }
    return 0;
}
