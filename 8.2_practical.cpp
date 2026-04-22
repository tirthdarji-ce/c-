#include <iostream>
#include <cmath>
#include <exception>

class NegativeNumberException : public std::exception {
public:
    const char* what() const noexcept override { return "negative input"; }
};

int main() {
    double x;
    std::cout << "Enter numbers (non-number to quit):\n";
    while (std::cout << "Number: " && std::cin >> x) {
        try {
            if (x < 0) throw NegativeNumberException();
            std::cout << "sqrt = " << std::sqrt(x) << '\n';
        } catch (const NegativeNumberException& e) {
            std::cout << "Error: " << e.what() << '\n';
        }
    }
    return 0;
}