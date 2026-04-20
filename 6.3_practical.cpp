#include <iostream>
#include <string>
#include <vector>

// Fuel class: stores fuel type
class Fuel {
protected:
    std::string fuelType;
public:
    explicit Fuel(const std::string& type = "unknown") : fuelType(type) {}
};

// Brand class: stores brand/model name
class Brand {
protected:
    std::string brandName;
public:
    explicit Brand(const std::string& name = "unknown") : brandName(name) {}
};

// Car class: hybrid inheritance from Fuel and Brand
class Car : public Fuel, public Brand {
public:
    Car(const std::string& brand, const std::string& fuel)
        : Fuel(fuel), Brand(brand) {}

    void display() const {
        std::cout << "Brand: " << brandName << ", Fuel: " << fuelType << '\n';
    }
};

// Structured manager: instance-based queue
class Manager {
    std::vector<Car> cars;
public:
    void addCar(const Car& c) { cars.push_back(c); }
    void processAll() {
        std::cout << "[Manager] Processing cars (structured):\n";
        for (const auto& c : cars) c.display();
        cars.clear();
    }
};

// Static manager: class-level queue
class StaticManager {
public:
    static std::vector<Car> s_cars;

    static void addCar(const Car& c) { s_cars.push_back(c); }
    static void processAll() {
        std::cout << "[StaticManager] Processing cars (static):\n";
        for (const auto& c : s_cars) c.display();
        s_cars.clear();
    }
};

// Definition of the static member
std::vector<Car> StaticManager::s_cars;

// Helper to read one car from stdin
Car readCarFromInput(int index) {
    std::string brand, fuel;
    std::cout << "Car " << index << " - Enter brand: ";
    std::getline(std::cin >> std::ws, brand);
    std::cout << "Car " << index << " - Enter fuel: ";
    std::getline(std::cin >> std::ws, fuel);
    return Car(brand, fuel);
}

int main() {
    Manager mgr;

    std::cout << "Choose manager to use (1 = structured, 2 = static, 3 = both): ";
    int choice = 1;
    if (!(std::cin >> choice)) return 1;

    std::cout << "How many cars do you want to enter? ";
    int n = 0;
    if (!(std::cin >> n) || n <= 0) {
        std::cerr << "Invalid number of cars.\n";
        return 1;
    }

    // consume newline before getline in readCarFromInput via std::ws inside
    for (int i = 1; i <= n; ++i) {
        Car c = readCarFromInput(i);
        if (choice == 1) mgr.addCar(c);
        else if (choice == 2) StaticManager::addCar(c);
        else {
            mgr.addCar(c);
            StaticManager::addCar(c);
        }
    }

    if (choice == 1 || choice == 3) mgr.processAll();
    if (choice == 2 || choice == 3) StaticManager::processAll();

    return 0;
}