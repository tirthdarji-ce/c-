// interactive_circles.cpp
// Simple, easy-to-understand C++ program that accepts user input to manage circles.

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>

using namespace std;

constexpr double PI = 3.14159265358979323846;

struct Circle {
    int id;
    double radius;

    double area() const { return PI * radius * radius; }
    double circumference() const { return 2.0 * PI * radius; }
};

class CircleManager {
    vector<Circle> circles;
    int next_id = 1;

public:
    int add(double radius) {
        if (radius < 0.0) radius = 0.0;
        circles.push_back({next_id, radius});
        return next_id++;
    }

    bool removeById(int id) {
        auto oldSize = circles.size();
        circles.erase(remove_if(circles.begin(), circles.end(),
                                [id](const Circle& c){ return c.id == id; }),
                      circles.end());
        return circles.size() != oldSize;
    }

    Circle* findById(int id) {
        for (auto &c : circles) if (c.id == id) return &c;
        return nullptr;
    }

    double totalArea() const {
        return accumulate(circles.begin(), circles.end(), 0.0,
                          [](double acc, const Circle& c){ return acc + c.area(); });
    }

    double averageArea() const {
        if (circles.empty()) return 0.0;
        return totalArea() / static_cast<double>(circles.size());
    }

    void scaleAll(double factor) {
        for (auto &c : circles) c.radius *= factor;
    }

    void sortByArea(bool ascending = true) {
        if (ascending)
            sort(circles.begin(), circles.end(),
                 [](const Circle& a, const Circle& b){ return a.area() < b.area(); });
        else
            sort(circles.begin(), circles.end(),
                 [](const Circle& a, const Circle& b){ return a.area() > b.area(); });
    }

    void printAll() const {
        if (circles.empty()) {
            cout << "(no circles)\n";
            return;
        }
        for (const auto &c : circles) {
            cout << "Circle{id=" << c.id
                 << ", r=" << fixed << setprecision(4) << c.radius
                 << ", area=" << fixed << setprecision(4) << c.area()
                 << "}\n";
        }
    }

    size_t size() const { return circles.size(); }
};

int main() {
    CircleManager mgr;
    cout << "Simple Circle Manager\n";
    cout << "Commands:\n"
         << " 1: Add circle (enter radius)\n"
         << " 2: Remove circle by id\n"
         << " 3: List all circles\n"
         << " 4: Show total and average area\n"
         << " 5: Scale all radii (enter factor)\n"
         << " 6: Sort by area (1=asc, 2=desc)\n"
         << " 7: Find circle by id\n"
         << " 0: Quit\n";

    while (true) {
        cout << "\nEnter command: ";
        int cmd;
        if (!(cin >> cmd)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        if (cmd == 0) break;

        switch (cmd) {
            case 1: {
                cout << "Enter radius: ";
                double r;
                if (!(cin >> r)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid radius.\n"; break; }
                int id = mgr.add(r);
                cout << "Added circle with id " << id << ".\n";
                break;
            }
            case 2: {
                cout << "Enter id to remove: ";
                int id;
                if (!(cin >> id)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid id.\n"; break; }
                if (mgr.removeById(id)) cout << "Removed circle " << id << ".\n";
                else cout << "No circle with id " << id << " found.\n";
                break;
            }
            case 3:
                cout << "All circles:\n";
                mgr.printAll();
                break;
            case 4:
                cout << fixed << setprecision(4)
                     << "Total area: " << mgr.totalArea() << "\n"
                     << "Average area: " << mgr.averageArea() << "\n";
                break;
            case 5: {
                cout << "Enter scale factor: ";
                double f;
                if (!(cin >> f)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid factor.\n"; break; }
                mgr.scaleAll(f);
                cout << "Scaled all radii by " << f << ".\n";
                break;
            }
            case 6: {
                cout << "1 = ascending, 2 = descending: ";
                int d;
                if (!(cin >> d)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid choice.\n"; break; }
                mgr.sortByArea(d != 2);
                cout << "Sorted.\n";
                break;
            }
            case 7: {
                cout << "Enter id to find: ";
                int id;
                if (!(cin >> id)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid id.\n"; break; }
                Circle* c = mgr.findById(id);
                if (c) {
                    cout << "Found: id=" << c->id
                         << ", r=" << fixed << setprecision(4) << c->radius
                         << ", area=" << fixed << setprecision(4) << c->area() << "\n";
                } else {
                    cout << "No circle with id " << id << ".\n";
                }
                break;
            }
            default:
                cout << "Unknown command.\n";
        }
    }

    cout << "Goodbye.\n";
    return 0;
}
