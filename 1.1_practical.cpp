#include <iostream>
using namespace std;

class Wallet {
private:
    string id;
    string name;
    double balance;

public:
    // Default Constructor
    Wallet() {
        id = "";
        name = "";
        balance = 0;
    }

    // Create Wallet
    void createWallet() {
        cout << "Enter Wallet ID: ";
        cin >> id;
        cout << "Enter User Name: ";
        cin >> name;
        cout << "Enter Initial Balance: ";
        cin >> balance;
    }

    void load(double amount) {
        if (amount > 0)
            balance += amount;
        else
            cout << "Invalid amount!\n";
    }

    void transfer(Wallet &other, double amount) {
        if (balance >= amount) {
            balance -= amount;
            other.balance += amount;
            cout << "Transfer successful!\n";
        } else {
            cout << "Insufficient balance!\n";
        }
    }

    void display() {
        cout << "\nWallet ID: " << id;
        cout << "\nName: " << name;
        cout << "\nBalance: " << balance << endl;
    }
};

int main() {
    Wallet w1, w2;
    int choice;
    double amount;

    do {
        cout << "\n---- Wallet Menu ----";
        cout << "\n1. Create Wallet 1";
        cout << "\n2. Create Wallet 2";
        cout << "\n3. Load Money to Wallet 1";
        cout << "\n4. Transfer Wallet1 -> Wallet2";
        cout << "\n5. Display Wallets";
        cout << "\n6. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                w1.createWallet();
                break;

            case 2:
                w2.createWallet();
                break;

            case 3:
                cout << "Enter amount: ";
                cin >> amount;
                w1.load(amount);
                break;

            case 4:
                cout << "Enter amount: ";
                cin >> amount;
                w1.transfer(w2, amount);
                break;

            case 5:
                w1.display();
                w2.display();
                break;

            case 6:
                cout << "Exiting...";
                break;

            default:
                cout << "Invalid choice!";
        }

    } while (choice != 6);

    return 0;
}