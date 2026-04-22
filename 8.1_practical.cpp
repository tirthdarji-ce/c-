#include <iostream>
#include <stdexcept>

struct Div { double value{}; };

std::istream& operator>>(std::istream& is, Div& d) {
    int n, m;
    if (!(is >> n >> m)) throw std::invalid_argument("invalid integer input");
    if (m == 0) throw std::runtime_error("division by zero");
    d.value = static_cast<double>(n) / m;
    return is;
}

int main() {
    try {
        std::cout << "Enter numerator and denominator: ";
        Div r;
        std::cin >> r;
        std::cout << "Result: " << r.value << '\n';
    } catch (const std::invalid_argument& e) {
        std::cerr << "Input error: " << e.what() << '\n';
        return 1;
    } catch (const std::runtime_error& e) {
        std::cerr << "Math error: " << e.what() << '\n';
        return 2;
    }
}
