#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <iomanip>
#include <limits>
#include <algorithm>

class BaseAccount {
public:
    struct Transaction {
        std::string type;
        double amount;
        double beforeBalance;
        double afterBalance;
    };

    BaseAccount(long acctNo, double initBalance)
        : accountNumber(acctNo), balance(initBalance) {}

    virtual ~BaseAccount() = default;

    virtual bool deposit(double amount) {
        if (amount <= 0) return false;
        double before = balance;
        balance += amount;
        recordTransaction("Deposit", amount, before, balance);
        return true;
    }

    virtual bool withdraw(double amount) {
        if (amount <= 0) return false;
        if (amount > balance) return false;
        double before = balance;
        balance -= amount;
        recordTransaction("Withdraw", amount, before, balance);
        return true;
    }

    bool undoLastTransaction() {
        if (transactions.empty()) return false;
        Transaction last = transactions.back();
        transactions.pop_back();
        balance = last.beforeBalance;
        return true;
    }

    void printTransactions() const {
        if (transactions.empty()) {
            std::cout << "No transactions.\n";
            return;
        }
        std::cout << "History (most recent last):\n";
        for (const auto &t : transactions) {
            std::cout << "  " << std::setw(8) << t.type
                      << " | Amount: " << std::fixed << std::setprecision(2) << t.amount
                      << " | Before: " << t.beforeBalance
                      << " | After: " << t.afterBalance << '\n';
        }
    }

    void display() const {
        std::cout << getAccountType() << " Account #" << accountNumber
                  << " | Balance: " << std::fixed << std::setprecision(2) << balance << '\n';
    }

    double getBalance() const { return balance; }
    long getAccountNumber() const { return accountNumber; }

protected:
    void adjustBalance(double newBalance, const std::string &type, double amount, double before) {
        double beforeRecorded = before;
        double afterRecorded = newBalance;
        balance = newBalance;
        transactions.push_back({type, amount, beforeRecorded, afterRecorded});
    }

    virtual std::string getAccountType() const { return "Base"; }

private:
    void recordTransaction(const std::string &type, double amount, double before, double after) {
        transactions.push_back({type, amount, before, after});
    }

    long accountNumber;
    double balance;
    std::vector<Transaction> transactions;
};

class SavingsAccount : public BaseAccount {
public:
    SavingsAccount(long acctNo, double initBalance, double rate)
        : BaseAccount(acctNo, initBalance), interestRate(rate) {}

    void applyInterest() {
        double before = getBalance();
        double interest = before * interestRate;
        double after = before + interest;
        adjustBalance(after, "Interest", interest, before);
    }

    std::string getAccountType() const override { return "Savings"; }

private:
    double interestRate;
};

class CurrentAccount : public BaseAccount {
public:
    CurrentAccount(long acctNo, double initBalance, double overdraftLimit)
        : BaseAccount(acctNo, initBalance), overdraft(overdraftLimit) {}

    bool withdraw(double amount) override {
        if (amount <= 0) return false;
        double before = getBalance();
        if (before - amount < -overdraft) return false;
        double after = before - amount;
        adjustBalance(after, "Withdraw", amount, before);
        return true;
    }

    std::string getAccountType() const override { return "Current"; }

private:
    double overdraft;
};

// Utilities for the interactive menu
void ignoreLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

BaseAccount* findAccount(std::vector<std::unique_ptr<BaseAccount>>& accounts, long acctNo) {
    for (auto &a : accounts) {
        if (a->getAccountNumber() == acctNo) return a.get();
    }
    return nullptr;
}

void showMenu() {
    std::cout << "\nMenu:\n"
              << "  1) Create account\n"
              << "  2) List accounts\n"
              << "  3) Deposit\n"
              << "  4) Withdraw\n"
              << "  5) Apply interest (Savings only)\n"
              << "  6) Undo last transaction\n"
              << "  7) Print transactions\n"
              << "  8) Display account\n"
              << "  0) Quit\n"
              << "Choose option: ";
}

int main() {
    std::vector<std::unique_ptr<BaseAccount>> accounts;

    // Optionally seed two accounts (comment out if you prefer starting empty)
    accounts.push_back(std::make_unique<SavingsAccount>(1001, 1000.0, 0.03));
    accounts.push_back(std::make_unique<CurrentAccount>(2001, 500.0, 200.0));

    while (true) {
        showMenu();
        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Exiting.\n";
            break;
        }

        if (choice == 0) break;

        if (choice == 1) {
            std::cout << "Create (s)avings or (c)urrent? ";
            char t; std::cin >> t;
            long acc; double bal;
            std::cout << "Account number: "; std::cin >> acc;
            if (findAccount(accounts, acc)) {
                std::cout << "Account already exists.\n";
                continue;
            }
            std::cout << "Initial balance: "; std::cin >> bal;
            if (t == 's' || t == 'S') {
                double rate;
                std::cout << "Interest rate (e.g. 0.03): "; std::cin >> rate;
                accounts.push_back(std::make_unique<SavingsAccount>(acc, bal, rate));
            } else {
                double od;
                std::cout << "Overdraft limit: "; std::cin >> od;
                accounts.push_back(std::make_unique<CurrentAccount>(acc, bal, od));
            }
            std::cout << "Created.\n";
        }
        else if (choice == 2) {
            if (accounts.empty()) { std::cout << "No accounts.\n"; continue; }
            for (const auto &a : accounts) a->display();
        }
        else if (choice == 3 || choice == 4 || choice == 5 || choice == 6 || choice == 7 || choice == 8) {
            long acc; std::cout << "Account number: "; std::cin >> acc;
            BaseAccount* acct = findAccount(accounts, acc);
            if (!acct) { std::cout << "Account not found.\n"; continue; }

            if (choice == 3) {
                double amt; std::cout << "Amount to deposit: "; std::cin >> amt;
                if (acct->deposit(amt)) std::cout << "Deposit successful.\n";
                else std::cout << "Deposit failed.\n";
            } else if (choice == 4) {
                double amt; std::cout << "Amount to withdraw: "; std::cin >> amt;
                if (acct->withdraw(amt)) std::cout << "Withdraw successful.\n";
                else std::cout << "Withdraw failed.\n";
            } else if (choice == 5) {
                if (auto sa = dynamic_cast<SavingsAccount*>(acct)) {
                    sa->applyInterest();
                    std::cout << "Interest applied.\n";
                } else {
                    std::cout << "Not a savings account.\n";
                }
            } else if (choice == 6) {
                if (acct->undoLastTransaction()) std::cout << "Undo successful.\n";
                else std::cout << "Nothing to undo.\n";
            } else if (choice == 7) {
                acct->printTransactions();
            } else if (choice == 8) {
                acct->display();
            }
        }
        else {
            std::cout << "Unknown option.\n";
        }
    }

    std::cout << "Goodbye.\n";
    return 0;
}