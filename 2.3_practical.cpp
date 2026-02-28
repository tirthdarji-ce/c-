#include <iostream>
#include <vector>
using namespace std;

class DigitalAccount {
private:
    string customerName;
    int accountID;
    double balance;

public:
    // Constructor with zero balance
    DigitalAccount(string name, int id) {
        customerName = name;
        accountID = id;
        balance = 0.0;
    }

    // Constructor with custom balance
    DigitalAccount(string name, int id, double initialBalance) {
        customerName = name;
        accountID = id;

        if(initialBalance >= 0)
            balance = initialBalance;
        else {
            cout << "Invalid Initial Balance! Setting to 0.\n";
            balance = 0.0;
        }
    }

    void deposit(double amount) {
        if(amount > 0) {
            balance += amount;
            cout << "Deposit Successful!\n";
        } else {
            cout << "Invalid Deposit Amount!\n";
        }
    }

    void withdraw(double amount) {
        if(amount <= 0) {
            cout << "Invalid Withdrawal Amount!\n";
        }
        else if(amount > balance) {
            cout << "Withdrawal Failed! Insufficient Balance.\n";
        }
        else {
            balance -= amount;
            cout << "Withdrawal Successful!\n";
        }
    }

    void displaySummary() const {
        cout << "\n----- Account Summary -----\n";
        cout << "Customer Name : " << customerName << endl;
        cout << "Account ID    : " << accountID << endl;
        cout << "Current Balance: ₹" << balance << endl;
    }

    int getAccountID() const {
        return accountID;
    }
};

int main() {
    vector<DigitalAccount> accounts;
    int choice;

    do {
        cout << "\n1. Create Account (Zero Balance)";
        cout << "\n2. Create Account (Custom Balance)";
        cout << "\n3. Deposit";
        cout << "\n4. Withdraw";
        cout << "\n5. Display Account Summary";
        cout << "\n6. Exit";
        cout << "\nEnter Choice: ";
        cin >> choice;

        if(choice == 1 || choice == 2) {
            string name;
            int id;
            double balance = 0;

            cout << "Enter Customer Name: ";
            cin.ignore();
            getline(cin, name);

            cout << "Enter Account ID: ";
            cin >> id;

            if(choice == 2) {
                cout << "Enter Initial Balance: ";
                cin >> balance;
                accounts.push_back(DigitalAccount(name, id, balance));
            } else {
                accounts.push_back(DigitalAccount(name, id));
            }

            cout << "Account Created Successfully!\n";
        }

        else if(choice >= 3 && choice <= 5) {
            int id;
            cout << "Enter Account ID: ";
            cin >> id;

            bool found = false;
            for(auto &acc : accounts) {
                if(acc.getAccountID() == id) {
                    found = true;

                    if(choice == 3) {
                        double amt;
                        cout << "Enter Deposit Amount: ";
                        cin >> amt;
                        acc.deposit(amt);
                    }
                    else if(choice == 4) {
                        double amt;
                        cout << "Enter Withdrawal Amount: ";
                        cin >> amt;
                        acc.withdraw(amt);
                    }
                    else if(choice == 5) {
                        acc.displaySummary();
                    }

                    break;
                }
            }

            if(!found)
                cout << "Account Not Found!\n";
        }

    } while(choice != 6);

    return 0;
}