#include <iostream>
using namespace std;

class BankAccount {
private:
    int accountNumber;
    string holderName;
    double balance;

public:
    static int totalAccounts;

    BankAccount(int accNum = 0, string name = "Unknown", double bal = 0) {
        accountNumber = accNum;
        holderName = name;
        balance = bal;
        totalAccounts++;
    }
    void deposit(double amount) {
        if (amount > 0)
            balance += amount;
        else
            cout << "Invalid deposit amount.\n";
    }
    void withdraw(double amount) {
        if (amount > 0 && amount <= balance)
            balance -= amount;
        else
            cout << "Insufficient balance or invalid amount.\n";
    }
    void transfer(BankAccount &to, double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            to.balance += amount;
            cout << "Transfer successful.\n";
        } else {
            cout << "Transfer failed.\n";
        }
    }
    void updateName(string newName) {
        holderName = newName;
    }
    void display() {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << holderName << endl;
        cout << "Current Balance: " << balance << endl;
    }
};
int BankAccount::totalAccounts = 0;

int main() {
    int n;
    cout << "Enter number of accounts to create: ";
    cin >> n;

    BankAccount accounts[100];

    for (int i = 0; i < n; i++) {
        int accNum;
        string name;
        double bal;

        cout << "\nEnter details for Account " << i + 1 << ":\n";
        cout << "Account Number: ";
        cin >> accNum;
        cout << "Account Holder Name (no spaces): ";
        cin >> name;
        cout << "Initial Balance: ";
        cin >> bal;

        accounts[i] = BankAccount(accNum, name, bal);
    }
    cout << "\nAccount Summary\n";
    for (int i = 0; i < n; i++)
        accounts[i].display();

    int fromIndex, toIndex;
    double amount;
    cout << "\nEnter index of sender account (0-" << n-1 << "): ";
    cin >> fromIndex;
    cout << "Enter index of receiver account (0-" << n-1 << "): ";
    cin >> toIndex;
    cout << "Enter amount to transfer: ";
    cin >> amount;

    if (fromIndex >= 0 && fromIndex < n && toIndex >= 0 && toIndex < n)
        accounts[fromIndex].transfer(accounts[toIndex], amount);
    else
        cout << "Invalid account indices.\n";

    cout << "\nUpdated Account Summary\n";
    for (int i = 0; i < n; i++)
        accounts[i].display();

    cout << "Total accounts created: " << BankAccount::totalAccounts << endl;
    return 0;
}
