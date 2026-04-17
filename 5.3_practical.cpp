#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>

const std::string DB_FILENAME = "inventory.txt";

struct Item {
    std::string name;
    int quantity = 0;
    double price = 0.0;
};

static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    return s;
}

// Parse line format: name|quantity|price
bool parseLine(const std::string& line, Item &it) {
    auto p1 = line.find('|');
    if (p1 == std::string::npos) return false;
    auto p2 = line.find('|', p1 + 1);
    if (p2 == std::string::npos) return false;
    it.name = line.substr(0, p1);
    try {
        it.quantity = std::stoi(line.substr(p1 + 1, p2 - p1 - 1));
        it.price = std::stod(line.substr(p2 + 1));
    } catch (...) {
        return false;
    }
    return true;
}

void addItem() {
    std::string name, qtys, prs;
    std::cout << "Enter item name: ";
    std::getline(std::cin, name);
    if (name.empty()) { std::cout << "Name cannot be empty.\n"; return; }
    for (char &c : name) if (c == '|') c = '/';

    std::cout << "Enter quantity: ";
    std::getline(std::cin, qtys);
    std::cout << "Enter price: ";
    std::getline(std::cin, prs);

    int qty; double price;
    try {
        qty = std::stoi(qtys);
        price = std::stod(prs);
    } catch (...) {
        std::cout << "Invalid quantity or price.\n";
        return;
    }
    if (qty < 0 || price < 0.0) { std::cout << "Values must be non-negative.\n"; return; }

    std::ofstream ofs(DB_FILENAME, std::ios::app);
    if (!ofs) { std::cout << "Cannot open database file.\n"; return; }
    ofs << name << '|' << qty << '|' << std::fixed << std::setprecision(2) << price << '\n';
    std::cout << "Item added.\n";
}

void viewItems() {
    std::ifstream ifs(DB_FILENAME);
    if (!ifs) { std::cout << "No items (database file not found).\n"; return; }
    std::string line; Item it;
    bool any = false;
    std::cout << std::left << std::setw(30) << "Name" << std::setw(10) << "Quantity" << "Price\n";
    std::cout << std::string(50, '-') << '\n';
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        if (!parseLine(line, it)) continue;
        std::cout << std::left << std::setw(30) << it.name
                  << std::setw(10) << it.quantity
                  << "$" << std::fixed << std::setprecision(2) << it.price << '\n';
        any = true;
    }
    if (!any) std::cout << "No items in inventory.\n";
}

void searchItem() {
    std::cout << "Enter name to search: ";
    std::string q; std::getline(std::cin, q);
    if (q.empty()) { std::cout << "Search query cannot be empty.\n"; return; }
    auto qlow = toLower(q);

    std::ifstream ifs(DB_FILENAME);
    if (!ifs) { std::cout << "No items (database file not found).\n"; return; }
    std::string line; Item it; bool found = false;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        if (!parseLine(line, it)) continue;
        if (toLower(it.name).find(qlow) != std::string::npos) {
            if (!found) {
                std::cout << std::left << std::setw(30) << "Name" << std::setw(10) << "Quantity" << "Price\n";
                std::cout << std::string(50, '-') << '\n';
            }
            std::cout << std::left << std::setw(30) << it.name
                      << std::setw(10) << it.quantity
                      << "$" << std::fixed << std::setprecision(2) << it.price << '\n';
            found = true;
        }
    }
    if (!found) std::cout << "No matching item found.\n";
}

int main() {
    while (true) {
        std::cout << "\nInventory Management\n1. Add item\n2. View all items\n3. Search by name\n4. Exit\nChoose an option: ";
        std::string opt; std::getline(std::cin, opt);
        if (opt.empty()) continue;
        if (opt == "1") addItem();
        else if (opt == "2") viewItems();
        else if (opt == "3") searchItem();
        else if (opt == "4") { std::cout << "Goodbye.\n"; break; }
        else std::cout << "Invalid option.\n";
    }
    return 0;
}
