#include <iostream>
#include <vector>
using namespace std;

class Student {
private:
    int roll;
    string name;
    float marks1, marks2, marks3;

public:
    // Default constructor (for testing)
    Student() {
        roll = 0;
        name = "Default";
        marks1 = marks2 = marks3 = 0;
    }

    // Parameterized constructor
    Student(int r, string n, float m1, float m2, float m3) {
        roll = r;
        name = n;
        marks1 = m1;
        marks2 = m2;
        marks3 = m3;
    }

    void input() {
        cout << "Enter Roll Number: ";
        cin >> roll;
        cin.ignore();

        cout << "Enter Name: ";
        getline(cin, name);

        cout << "Enter Marks in 3 Subjects: ";
        cin >> marks1 >> marks2 >> marks3;
    }

    float calculateAverage() {
        return (marks1 + marks2 + marks3) / 3;
    }

    void display() {
        cout << "\nRoll No: " << roll;
        cout << "\nName: " << name;
        cout << "\nMarks: " << marks1 << ", "
             << marks2 << ", "
             << marks3;
        cout << "\nAverage: " << calculateAverage() << endl;
    }
};

int main() {
    vector<Student> students;
    int choice;

    do {
        cout << "\n1. Add Student (User Input)";
        cout << "\n2. Add Default Students";
        cout << "\n3. Display Students";
        cout << "\n4. Exit";
        cout << "\nEnter Choice: ";
        cin >> choice;

        if(choice == 1) {
            Student s;
            s.input();
            students.push_back(s);
        }

        else if(choice == 2) {
            students.push_back(Student(101, "Rahul", 75, 80, 85));
            students.push_back(Student(102, "Priya", 88, 92, 79));
            cout << "Default Students Added!\n";
        }

        else if(choice == 3) {
            for(int i = 0; i < students.size(); i++) {
                students[i].display();
            }
        }

    } while(choice != 4);

    return 0;
}