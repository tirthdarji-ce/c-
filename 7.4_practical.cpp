// Interactive polymorphic Shape area calculator with dynamic and static collections
// Compile: C++17
#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <memory>
#include <cmath>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <limits>

constexpr double PI = 3.14159265358979323846;

class Shape {
public:
    virtual double Area() const = 0;
    virtual std::string Name() const = 0;
    virtual ~Shape() = default;
};

class Rectangle : public Shape {
    double length_;
    double width_;
public:
    Rectangle(double length, double width)
        : length_(length), width_(width)
    {
        if (length_ < 0.0 || width_ < 0.0)
            throw std::invalid_argument("Rectangle dimensions must be non-negative");
    }
    double Area() const override { return length_ * width_; }
    std::string Name() const override { return "Rectangle"; }
};

class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double radius) : radius_(radius)
    {
        if (radius_ < 0.0)
            throw std::invalid_argument("Circle radius must be non-negative");
    }
    double Area() const override { return PI * radius_ * radius_; }
    std::string Name() const override { return "Circle"; }
};

static inline void ClearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static inline std::string LowerTrim(const std::string& s) {
    auto beg = s.find_first_not_of(" \t\r\n");
    if (beg == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    std::string out = s.substr(beg, end - beg + 1);
    std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c){ return std::tolower(c); });
    return out;
}

double TotalAreaDynamic(const std::vector<std::unique_ptr<Shape>>& shapes) {
    double total = 0.0;
    for (const auto& s : shapes) if (s) total += s->Area();
    return total;
}

template <size_t N>
double TotalAreaStatic(const std::array<std::unique_ptr<Shape>, N>& shapes) {
    double total = 0.0;
    for (const auto& s : shapes) if (s) total += s->Area();
    return total;
}

void PrintAreas(const std::vector<std::unique_ptr<Shape>>& shapes) {
    std::cout << std::fixed << std::setprecision(6);
    for (size_t i = 0; i < shapes.size(); ++i) {
        const auto& s = shapes[i];
        if (s) std::cout << "Dynamic[" << i << "] " << s->Name() << " area = " << s->Area() << '\n';
        else std::cout << "Dynamic[" << i << "] <empty>\n";
    }
}

template <size_t N>
void PrintAreas(const std::array<std::unique_ptr<Shape>, N>& shapes) {
    std::cout << std::fixed << std::setprecision(6);
    for (size_t i = 0; i < N; ++i) {
        const auto& s = shapes[i];
        if (s) std::cout << "Static[" << i << "] " << s->Name() << " area = " << s->Area() << '\n';
        else std::cout << "Static[" << i << "] <empty>\n";
    }
}

bool ReadDouble(const std::string& prompt, double& out) {
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> out)) {
            std::cout << "Invalid number. Try again or type 'q' to cancel.\n";
            if (std::cin.eof()) return false;
            ClearInput();
            return false;
        }
        ClearInput();
        return true;
    }
}

int main() {
    try {
        std::cout << "Interactive Shape Area Calculator\n";
        std::cout << "Choose storage mode: (d)ynamic or (s)tatic: ";
        char mode;
        if (!(std::cin >> mode)) return 0;
        mode = static_cast<char>(std::tolower(mode));
        ClearInput();

        if (mode == 'd') {
            std::vector<std::unique_ptr<Shape>> dyn;
            dyn.reserve(8);

            std::cout << "Entering dynamic mode. Type 'r' for rectangle, 'c' for circle, 'done' to finish.\n";
            while (true) {
                std::string cmd;
                std::cout << "Enter shape (r/c) or done: ";
                if (!std::getline(std::cin, cmd)) break;
                cmd = LowerTrim(cmd);
                if (cmd.empty()) continue;
                if (cmd == "done" || cmd == "d" || cmd == "q") break;

                try {
                    if (cmd == "r" || cmd == "rectangle") {
                        double l, w;
                        std::cout << "Enter length: ";
                        if (!(std::cin >> l)) { ClearInput(); std::cout << "Invalid input.\n"; continue; }
                        std::cout << "Enter width: ";
                        if (!(std::cin >> w)) { ClearInput(); std::cout << "Invalid input.\n"; continue; }
                        ClearInput();
                        dyn.push_back(std::make_unique<Rectangle>(l, w));
                        std::cout << "Rectangle added.\n";
                    } else if (cmd == "c" || cmd == "circle") {
                        double r;
                        std::cout << "Enter radius: ";
                        if (!(std::cin >> r)) { ClearInput(); std::cout << "Invalid input.\n"; continue; }
                        ClearInput();
                        dyn.push_back(std::make_unique<Circle>(r));
                        std::cout << "Circle added.\n";
                    } else {
                        std::cout << "Unknown command. Use 'r', 'c', or 'done'.\n";
                    }
                } catch (const std::exception& ex) {
                    std::cout << "Error: " << ex.what() << '\n';
                }
            }

            std::cout << "\n=== Dynamic collection ===\n";
            PrintAreas(dyn);
            std::cout << "Total dynamic area = " << TotalAreaDynamic(dyn) << "\n";

        } else if (mode == 's') {
            std::cout << "Enter static capacity (positive integer, max 1000): ";
            int cap = 0;
            if (!(std::cin >> cap) || cap <= 0) {
                std::cout << "Invalid capacity.\n";
                return 0;
            }
            ClearInput();
            if (cap > 1000) cap = 1000;

            // Use dynamic allocation of std::unique_ptr array of size cap via vector then convert to array-like behavior.
            std::vector<std::unique_ptr<Shape>> temp(static_cast<size_t>(cap));
            size_t nextIndex = 0;

            std::cout << "Entering static mode with capacity " << cap << ". Type 'r' for rectangle, 'c' for circle, 'done' to finish.\n";
            while (nextIndex < temp.size()) {
                std::string cmd;
                std::cout << "Enter shape (r/c) or done: ";
                if (!std::getline(std::cin, cmd)) break;
                cmd = LowerTrim(cmd);
                if (cmd.empty()) continue;
                if (cmd == "done" || cmd == "d" || cmd == "q") break;

                try {
                    if (cmd == "r" || cmd == "rectangle") {
                        double l, w;
                        std::cout << "Enter length: ";
                        if (!(std::cin >> l)) { ClearInput(); std::cout << "Invalid input.\n"; continue; }
                        std::cout << "Enter width: ";
                        if (!(std::cin >> w)) { ClearInput(); std::cout << "Invalid input.\n"; continue; }
                        ClearInput();
                        temp[nextIndex++] = std::make_unique<Rectangle>(l, w);
                        std::cout << "Rectangle added at slot " << (nextIndex-1) << ".\n";
                    } else if (cmd == "c" || cmd == "circle") {
                        double r;
                        std::cout << "Enter radius: ";
                        if (!(std::cin >> r)) { ClearInput(); std::cout << "Invalid input.\n"; continue; }
                        ClearInput();
                        temp[nextIndex++] = std::make_unique<Circle>(r);
                        std::cout << "Circle added at slot " << (nextIndex-1) << ".\n";
                    } else {
                        std::cout << "Unknown command. Use 'r', 'c', or 'done'.\n";
                    }
                } catch (const std::exception& ex) {
                    std::cout << "Error: " << ex.what() << '\n';
                }
            }

            // Convert vector to std::array-like view for printing/totaling
            std::cout << "\n=== Static collection ===\n";
            std::cout << std::fixed << std::setprecision(6);
            for (size_t i = 0; i < temp.size(); ++i) {
                const auto& s = temp[i];
                if (s) std::cout << "Static[" << i << "] " << s->Name() << " area = " << s->Area() << '\n';
                else std::cout << "Static[" << i << "] <empty>\n";
            }
            double total = 0.0;
            for (const auto& s : temp) if (s) total += s->Area();
            std::cout << "Total static area = " << total << "\n";

        } else {
            std::cout << "Unknown mode. Please run again and choose 'd' or 's'.\n";
        }

    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}