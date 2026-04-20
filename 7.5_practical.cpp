// File: 7.5_practical.cpp
// C++17 - Simple object-oriented grading system demonstrating abstraction and runtime polymorphism.
// Compile: g++ -std=c++17 7.5_practical.cpp -o grading

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <iomanip>

class Student {
protected:
    std::string name;
    int id;
    double marks; // single numeric mark (percentage)
public:
    Student(std::string name_, int id_, double marks_)
        : name(std::move(name_)), id(id_), marks(marks_) {}
    virtual ~Student() = default;

    int getId() const { return id; }
    const std::string& getName() const { return name; }
    double getMarks() const { return marks; }
    void setMarks(double m) { marks = m; }

    // pure virtual: every derived class must implement its own grading policy
    virtual std::string computeGrade() const = 0;
    virtual std::string level() const = 0;

    // helper to print a concise record line
    void printRecord() const {
        std::cout << std::left << std::setw(4) << id << " | "
                  << std::setw(15) << name << " | "
                  << std::setw(12) << level() << " | "
                  << std::fixed << std::setprecision(2) << std::setw(7) << marks << "% | "
                  << computeGrade() << '\n';
    }
};

// Undergraduate grading policy
class Undergraduate : public Student {
public:
    Undergraduate(std::string name_, int id_, double marks_)
        : Student(std::move(name_), id_, marks_) {}

    std::string level() const override { return "Undergrad"; }

    // Thresholds (example):
    // A: >= 70, B: 60-69, C: 50-59, D: 40-49, F: < 40
    std::string computeGrade() const override {
        if (marks >= 70.0) return "A";
        if (marks >= 60.0) return "B";
        if (marks >= 50.0) return "C";
        if (marks >= 40.0) return "D";
        return "F";
    }
};

// Postgraduate grading policy (more rigorous)
class Postgraduate : public Student {
public:
    Postgraduate(std::string name_, int id_, double marks_)
        : Student(std::move(name_), id_, marks_) {}

    std::string level() const override { return "Postgrad"; }

    // Thresholds (example):
    // A: >= 75, B: 65-74, C: 55-64, F: < 55
    std::string computeGrade() const override {
        if (marks >= 75.0) return "A";
        if (marks >= 65.0) return "B";
        if (marks >= 55.0) return "C";
        return "F";
    }
}

;

int main() {
    std::vector<std::unique_ptr<Student>> records;
    int nextId = 1;

    while (true) {
        std::cout << "\nMenu:\n"
                  << "1) Add Undergraduate student\n"
                  << "2) Add Postgraduate student\n"
                  << "3) List all students\n"
                  << "4) Show grade for a student (by ID)\n"
                  << "5) Update marks (by ID)\n"
                  << "6) Exit\n"
                  << "Choose an option: ";
        int option;
        if (!(std::cin >> option)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input.\n";
            continue;
        }

        if (option == 6) break;

        if (option == 1 || option == 2) {
            std::cin.ignore(10000, '\n');
            std::string name;
            double marks;
            std::cout << "Enter student name: ";
            std::getline(std::cin, name);
            std::cout << "Enter marks (0-100): ";
            if (!(std::cin >> marks) || marks < 0.0 || marks > 100.0) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid marks. Operation cancelled.\n";
                continue;
            }
            if (option == 1) {
                records.push_back(std::make_unique<Undergraduate>(name, nextId++, marks));
            } else {
                records.push_back(std::make_unique<Postgraduate>(name, nextId++, marks));
            }
            std::cout << "Student added with ID " << (nextId - 1) << ".\n";
            continue;
        }

        if (option == 3) {
            if (records.empty()) {
                std::cout << "No records available.\n";
                continue;
            }
            std::cout << "\nID   | Name            | Level        | Marks  | Grade\n";
            std::cout << "-----------------------------------------------------\n";
            for (const auto& s : records) s->printRecord();
            continue;
        }

        if (option == 4 || option == 5) {
            std::cout << "Enter student ID: ";
            int id;
            if (!(std::cin >> id)) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid ID.\n";
                continue;
            }
            auto it = std::find_if(records.begin(), records.end(),
                                   [id](const std::unique_ptr<Student>& p){ return p->getId() == id; });
            if (it == records.end()) {
                std::cout << "Student ID not found.\n";
                continue;
            }
            if (option == 4) {
                const auto& s = *it;
                std::cout << "\nRecord:\n";
                std::cout << "ID: " << s->getId() << "\n";
                std::cout << "Name: " << s->getName() << "\n";
                std::cout << "Level: " << s->level() << "\n";
                std::cout << "Marks: " << std::fixed << std::setprecision(2) << s->getMarks() << "%\n";
                std::cout << "Computed Grade: " << s->computeGrade() << "\n";
            } else {
                double newMarks;
                std::cout << "Enter new marks (0-100): ";
                if (!(std::cin >> newMarks) || newMarks < 0.0 || newMarks > 100.0) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "Invalid marks.\n";
                    continue;
                }
                (*it)->setMarks(newMarks);
                std::cout << "Marks updated. New grade: " << (*it)->computeGrade() << "\n";
            }
            continue;
        }

        std::cout << "Unknown option.\n";
    }

    std::cout << "Exiting.\n";
    return 0;
}