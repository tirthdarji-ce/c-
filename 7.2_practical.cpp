// File: /Users/tirth/Desktop/CODES/7.2_practical.cpp
// Interactive banking system with BaseAccount, SavingsAccount, CurrentAccount,
// transaction history and undo support. C++17

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <memory>
#include <algorithm>

using namespace std;

struct Transaction {
    string type;        // "deposit", "withdraw", "interest", ...
    double amount;      // positive amount of the transaction
    double prevBalance; // balance before transaction
    double postBalance; // balance after transaction
    string timestamp;
};

static string nowTimestamp() {
    using namespace chrono;
    auto t = system_clock::now();
    time_t tt = system_clock::to_time_t(t);
    tm local_tm;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&local_tm, &tt);
#else
    localtime_r(&tt, &local_tm);
#endif
    ostringstream ss;
    ss << put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

class BaseAccount {
private:
    int accountNumber;
    double balance;

    static int nextAccountNumber;

protected:
    void setBalance(double b) { balance = b; }

public:
    vector<Transaction> transactions;

    BaseAccount(double initBalance = 0.0)
        : accountNumber(++nextAccountNumber), balance(initBalance) {}

    virtual ~BaseAccount() = default;

    int getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    virtual bool canWithdraw(double amount) const {
        return (balance >= amount);
    }

    virtual void deposit(double amount, const string& type = "deposit") {
        if (amount <= 0.0) {
            cerr << "Deposit amount must be positive.\n";
            return;
        }
        double prev = balance;
        balance += amount;
        transactions.push_back({type, amount, prev, balance, nowTimestamp()});
    }

    virtual void withdraw(double amount) {
        if (amount <= 0.0) {
            cerr << "Withdraw amount must be positive.\n";
            return;
        }
        if (!canWithdraw(amount)) {
            cerr << "Insufficient funds (or overdraft) for withdrawal of " << amount << ".\n";
            return;
        }
        double prev = balance;
        balance -= amount;
        transactions.push_back({"withdraw", amount, prev, balance, nowTimestamp()});
    }

    virtual void display() const {
        cout << "Account #" << accountNumber
             << " | Balance: " << fixed << setprecision(2) << balance << "\n";
    }

    void showTransactions() const {
        if (transactions.empty()) {
            cout << "No transactions.\n";
            return;
        }
        cout << "Transactions for Account #" << accountNumber << ":\n";
        for (size_t i = 0; i < transactions.size(); ++i) {
            const auto &t = transactions[i];
            cout << i+1 << ". [" << t.timestamp << "] "
                 << setw(10) << left << t.type << " | amount: " << setw(8) << t.amount
                 << " | prev: " << setw(8) << t.prevBalance
                 << " | post: " << setw(8) << t.postBalance << "\n";
        }
    }

    void undoLastTransaction() {
        if (transactions.empty()) {
            cout << "No transaction to undo.\n";
            return;
        }
        Transaction last = transactions.back();
        transactions.pop_back();
        setBalance(last.prevBalance);
        cout << "Undid last transaction (" << last.type << ", " << last.amount
             << "). Balance restored to " << fixed << setprecision(2) << balance << ".\n";
    }
};

int BaseAccount::nextAccountNumber = 1000;

class SavingsAccount : public BaseAccount {
private:
    double interestRate; // annual percentage

public:
    SavingsAccount(double initBalance = 0.0, double rate = 0.0)
        : BaseAccount(initBalance), interestRate(rate) {}

    ~SavingsAccount() override = default;

    void setInterestRate(double rate) { interestRate = rate; }
    double getInterestRate() const { return interestRate; }

    void applyInterest() {
        if (interestRate <= 0.0) {
            cout << "Interest rate is zero or negative; nothing applied.\n";
            return;
        }
        double current = getBalance();
        double interest = current * (interestRate / 100.0);
        if (interest <= 0.0) {
            cout << "Calculated interest is zero; nothing applied.\n";
            return;
        }
        deposit(interest, "interest");
        cout << "Applied interest: " << fixed << setprecision(2) << interest << "\n";
    }

    void display() const override {
        cout << "Savings Account #" << getAccountNumber()
             << " | Balance: " << fixed << setprecision(2) << getBalance()
             << " | Interest Rate: " << interestRate << "%\n";
    }
};

class CurrentAccount : public BaseAccount {
private:
    double overdraftLimit;

public:
    CurrentAccount(double initBalance = 0.0, double overdraft = 0.0)
        : BaseAccount(initBalance), overdraftLimit(overdraft) {}

    ~CurrentAccount() override = default;

    void setOverdraftLimit(double limit) { overdraftLimit = limit; }
    double getOverdraftLimit() const { return overdraftLimit; }

    bool canWithdraw(double amount) const override {
        double potential = getBalance() - amount;
        return (potential >= -overdraftLimit);
    }

    void display() const override {
        cout << "Current Account #" << getAccountNumber()
             << " | Balance: " << fixed << setprecision(2) << getBalance()
             << " | Overdraft Limit: " << fixed << setprecision(2) << overdraftLimit << "\n";
    }
};

// Helper input functions
static string readLine(const string &prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

template<typename T>
T readValue(const string &prompt) {
    while (true) {
        string s = readLine(prompt);
        stringstream ss(s);
        T val;
        if (ss >> val) return val;
        cout << "Invalid input. Try again.\n";
    }
}

shared_ptr<BaseAccount> findAccount(vector<shared_ptr<BaseAccount>>& accounts, int accNo) {
    for (auto &a : accounts) if (a->getAccountNumber() == accNo) return a;
    return nullptr;
}

void accountMenu(shared_ptr<BaseAccount> acc) {
    while (true) {
        cout << "\nSelected Account #" << acc->getAccountNumber() << "\n";
        cout << "1) Display details\n";
        cout << "2) Deposit\n";
        cout << "3) Withdraw\n";
        cout << "4) Show transactions\n";
        cout << "5) Undo last transaction\n";
        // dynamic options
        if (auto sa = dynamic_pointer_cast<SavingsAccount>(acc)) {
            cout << "6) Apply interest\n";
            cout << "7) Set interest rate\n";
            cout << "8) Back to main menu\n";
            int choice = readValue<int>("Choose: ");
            if (choice == 1) sa->display();
            else if (choice == 2) {
                double amt = readValue<double>("Deposit amount: ");
                sa->deposit(amt);
            } else if (choice == 3) {
                double amt = readValue<double>("Withdraw amount: ");
                sa->withdraw(amt);
            } else if (choice == 4) sa->showTransactions();
            else if (choice == 5) sa->undoLastTransaction();
            else if (choice == 6) sa->applyInterest();
            else if (choice == 7) {
                double r = readValue<double>("New interest rate (%): ");
                sa->setInterestRate(r);
                cout << "Interest rate set to " << r << "%\n";
            } else if (choice == 8) break;
            else cout << "Invalid choice.\n";
        } else if (auto ca = dynamic_pointer_cast<CurrentAccount>(acc)) {
            cout << "6) Set overdraft limit\n";
            cout << "7) Back to main menu\n";
            int choice = readValue<int>("Choose: ");
            if (choice == 1) ca->display();
            else if (choice == 2) {
                double amt = readValue<double>("Deposit amount: ");
                ca->deposit(amt);
            } else if (choice == 3) {
                double amt = readValue<double>("Withdraw amount: ");
                ca->withdraw(amt);
            } else if (choice == 4) ca->showTransactions();
            else if (choice == 5) ca->undoLastTransaction();
            else if (choice == 6) {
                double ov = readValue<double>("New overdraft limit: ");
                ca->setOverdraftLimit(ov);
                cout << "Overdraft limit set to " << fixed << setprecision(2) << ov << "\n";
            } else if (choice == 7) break;
            else cout << "Invalid choice.\n";
        } else {
            // generic account (shouldn't happen in this program)
            cout << "6) Back to main menu\n";
            int choice = readValue<int>("Choose: ");
            if (choice == 1) acc->display();
            else if (choice == 2) { double amt = readValue<double>("Deposit amount: "); acc->deposit(amt); }
            else if (choice == 3) { double amt = readValue<double>("Withdraw amount: "); acc->withdraw(amt); }
            else if (choice == 4) acc->showTransactions();
            else if (choice == 5) acc->undoLastTransaction();
            else if (choice == 6) break;
            else cout << "Invalid choice.\n";
        }
    }
}

int main() {
    cout << "Interactive Banking System\n";
    vector<shared_ptr<BaseAccount>> accounts;

    while (true) {
        cout << "\nMain Menu:\n";
        cout << "1) Create Savings Account\n";
        cout << "2) Create Current Account\n";
        cout << "3) List Accounts\n";
        cout << "4) Select Account by number\n";
        cout << "5) Exit\n";

        int choice = readValue<int>("Choose: ");

        if (choice == 1) {
            double init = readValue<double>("Initial balance: ");
            double rate = readValue<double>("Interest rate (%): ");
            accounts.push_back(make_shared<SavingsAccount>(init, rate));
            cout << "Created Savings Account #" << accounts.back()->getAccountNumber() << "\n";
        } else if (choice == 2) {
            double init = readValue<double>("Initial balance: ");
            double overd = readValue<double>("Overdraft limit: ");
            accounts.push_back(make_shared<CurrentAccount>(init, overd));
            cout << "Created Current Account #" << accounts.back()->getAccountNumber() << "\n";
        } else if (choice == 3) {
            if (accounts.empty()) {
                cout << "No accounts available.\n";
            } else {
                cout << "Accounts:\n";
                for (auto &a : accounts) {
                    a->display();
                }
            }
        } else if (choice == 4) {
            if (accounts.empty()) {
                cout << "No accounts to select.\n";
                continue;
            }
            int accNo = readValue<int>("Enter account number: ");
            auto acc = findAccount(accounts, accNo);
            if (!acc) {
                cout << "Account not found.\n";
            } else {
                accountMenu(acc);
            }
        } else if (choice == 5) {
            cout << "Exiting.\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}