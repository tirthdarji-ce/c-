// 7.6_practical.cpp
// Demonstrates why polymorphic bases need virtual destructors.
// Run and enter: 'n' = non-virtual demo, 'v' = virtual demo, 'b' = both demos.

#include <iostream>
#include <string>
#include <cctype>

class Resource {
public:
    explicit Resource(const std::string& n) : name(n) {
        ++live;
        std::cout << "Construct " << name << " (live=" << live << ")\n";
    }
    ~Resource() {
        --live;
        std::cout << "Destroy  " << name << " (live=" << live << ")\n";
    }
    static int live_count() noexcept { return live; }
private:
    std::string name;
    static int live;
};
int Resource::live = 0;

/* ---------------- Non-virtual base (incorrect) ---------------- */

class BaseNonVirtual {
public:
    BaseNonVirtual() { std::cout << "BaseNonVirtual ctor\n"; }
    ~BaseNonVirtual() { std::cout << "BaseNonVirtual dtor\n"; } // NOT virtual -> may skip derived dtor
};

class DerivedNonVirtual : public BaseNonVirtual {
public:
    DerivedNonVirtual() : r(new Resource("DerivedNonVirtual::r")) {
        std::cout << "DerivedNonVirtual ctor\n";
    }
    ~DerivedNonVirtual() {
        std::cout << "DerivedNonVirtual dtor\n";
        delete r;
    }
private:
    Resource* r; // raw pointer to demonstrate manual resource management and leak when destructor not called
};

void demo_non_virtual() {
    std::cout << "\n--- Non-virtual demo ---\n";
    std::cout << "Before: live=" << Resource::live_count() << '\n';
    BaseNonVirtual* p = new DerivedNonVirtual();
    std::cout << "After create: live=" << Resource::live_count() << '\n';
    std::cout << "Deleting via BaseNonVirtual*...\n";
    delete p; // undefined behavior: DerivedNonVirtual::~DerivedNonVirtual may not run
    std::cout << "After delete: live=" << Resource::live_count() << '\n';
}

/* ---------------- Virtual base (correct) ---------------- */

class BaseVirtual {
public:
    BaseVirtual() { std::cout << "BaseVirtual ctor\n"; }
    virtual ~BaseVirtual() { std::cout << "BaseVirtual dtor\n"; } // virtual -> correct
};

class DerivedVirtual : public BaseVirtual {
public:
    DerivedVirtual() : r(new Resource("DerivedVirtual::r")) {
        std::cout << "DerivedVirtual ctor\n";
    }
    ~DerivedVirtual() override {
        std::cout << "DerivedVirtual dtor\n";
        delete r;
    }
private:
    Resource* r;
};

void demo_virtual() {
    std::cout << "\n--- Virtual demo ---\n";
    std::cout << "Before: live=" << Resource::live_count() << '\n';
    BaseVirtual* p = new DerivedVirtual();
    std::cout << "After create: live=" << Resource::live_count() << '\n';
    std::cout << "Deleting via BaseVirtual*...\n";
    delete p; // correct: DerivedVirtual::~DerivedVirtual will be called
    std::cout << "After delete: live=" << Resource::live_count() << '\n';
}

/* ---------------- main ---------------- */

int main() {
    std::cout << "Enter option: 'n' (non-virtual), 'v' (virtual), 'b' (both): ";
    char c{};
    if (!(std::cin >> c)) return 0;
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

    switch (c) {
        case 'n': demo_non_virtual(); break;
        case 'v': demo_virtual(); break;
        case 'b':
            demo_non_virtual();
            demo_virtual();
            break;
        default:
            std::cout << "Unknown option\n";
    }

    std::cout << "\nFinal live count: " << Resource::live_count() << '\n';
    return 0;
}
