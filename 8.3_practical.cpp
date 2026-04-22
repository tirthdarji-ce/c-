#include <iostream>
using namespace std;

// Log array
string logs[50];
int idx = 0;

// Function to store logs
void addLog(string msg) {
    logs[idx++] = msg;
}

// BankAccount class
class BankAccount {
    double balance;

public:
    BankAccount(double b) {
        balance = b;
    }

    void deposit(double amt) {
        addLog("Enter deposit()");
        if (amt <= 0)
            throw "Invalid deposit amount!";
        balance += amt;
        addLog("Deposit successful");
        addLog("Exit deposit()");
    }

    void withdraw(double amt) {
        addLog("Enter withdraw()");
        if (amt <= 0)
            throw "Invalid withdraw amount!";
        if (amt > balance)
            throw "Insufficient balance!";
        balance -= amt;
        addLog("Withdraw successful");
        addLog("Exit withdraw()");
    }
};

// Transaction function (to show stack unwinding)
void transaction(BankAccount &acc) {
    addLog("Enter transaction()");
    
    double d, w;

    cout << "Enter deposit amount: ";
    cin >> d;
    acc.deposit(d);

    cout << "Enter withdraw amount: ";
    cin >> w;
    acc.withdraw(w);

    addLog("Exit transaction()");
}

// Main function
int main() {
    addLog("Enter main()");
    
    double initialBalance;
    cout << "Enter initial balance: ";
    cin >> initialBalance;

    BankAccount acc(initialBalance);

    try {
        transaction(acc);
    }
    catch (const char* error) {
        addLog("Exception caught in main()");
        cout << "Error: " << error << endl;
    }

    addLog("Exit main()");

    // Display logs
    cout << "\n--- Transaction Logs ---\n";
    for (int i = 0; i < idx; i++) {
        cout << logs[i] << endl;
    }

    return 0;
}