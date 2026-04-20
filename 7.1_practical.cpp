// File: banking.cpp
// Build: g++ -std=c++17 banking.cpp -o banking

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include <algorithm>

using namespace std;

enum class TxType { Deposit, Withdraw, Interest };

struct Transaction {
    TxType type;
    double amount;
    string note;
    time_t when;
};

class Account {
protected:
    string accNumber;
    double balance = 0.0;
    vector<Transaction> history;

    void record(TxType t, double amt, string note = "") {
        history.push_back({t, amt, std::move(note), std::time(nullptr)});
    }

public:
    Account(string accNo, double init = 0.0) : accNumber(std::move(accNo)), balance(init) {}
    virtual ~Account() = default;

    virtual bool withdraw(double amt) {
        if (amt <= 0 || amt > balance) return false;
        balance -= amt;
        record(TxType::Withdraw, amt, "Withdraw");
        return true;
    }

    virtual void deposit(double amt) {
        if (amt <= 0) return;
        balance += amt;
        record(TxType::Deposit, amt, "Deposit");
    }

    bool undoLast() {
        if (history.empty()) return false;
        auto tx = history.back();
        switch (tx.type) {
            case TxType::Deposit:  balance -= tx.amount; break;
            case TxType::Withdraw: balance += tx.amount; break;
            case TxType::Interest: balance -= tx.amount; break;
        }
        history.pop_back();
        return true;
    }

    void printHistory() const {
        if (history.empty()) { cout << "No transactions.\n"; return; }
        cout << "Transactions:\n";
        for (size_t i = 0; i < history.size(); ++i) {
            const auto &t = history[i];
            char buf[20];
            std::strftime(buf, sizeof(buf), "%F %T", std::localtime(&t.when));
            string type = (t.type == TxType::Deposit ? "Deposit" :
                           t.type == TxType::Withdraw ? "Withdraw" : "Interest");
            cout << i+1 << ". [" << buf << "] " << type << " " << fixed << setprecision(2) << t.amount;
            if (!t.note.empty()) cout << " (" << t.note << ")";
            cout << '\n';
        }
    }

    virtual void display() const {
        cout << "Account: " << accNumber << "\nBalance: " << fixed << setprecision(2) << balance << '\n';
    }

    const string& number() const { return accNumber; }
    double getBalance() const { return balance; }
    virtual string typeName() const = 0;
};

class SavingsAccount : public Account {
    double interestRate = 0.0; // percent
public:
    SavingsAccount(string accNo, double init, double rate)
        : Account(std::move(accNo), init), interestRate(rate) {}

    void applyInterest() {
        if (interestRate <= 0.0) return;
        double interest = balance * (interestRate / 100.0);
        if (interest != 0.0) {
            balance += interest;
            record(TxType::Interest, interest, "Interest applied");
        }
    }

    void display() const override {
        cout << "[Savings]\n";
        Account::display();
        cout << "Interest Rate: " << fixed << setprecision(2) << interestRate << "%\n";
    }

    string typeName() const override { return "Savings"; }
};

class CurrentAccount : public Account {
    double overdraft = 0.0;
public:
    CurrentAccount(string accNo, double init, double od)
        : Account(std::move(accNo), init), overdraft(od) {}

    bool withdraw(double amt) override {
        if (amt <= 0 || balance - amt < -overdraft) return false;
        balance -= amt;
        record(TxType::Withdraw, amt, "Withdraw");
        return true;
    }

    void display() const override {
        cout << "[Current]\n";
        Account::display();
        cout << "Overdraft: " << fixed << setprecision(2) << overdraft << '\n';
    }

    string typeName() const override { return "Current"; }
};

// helpers
Account* findAccount(const vector<unique_ptr<Account>>& accounts, const string& no) {
    for (auto &p : accounts) if (p->number() == no) return p.get();
    return nullptr;
}

int readInt() {
    int v; while (!(cin >> v)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Invalid, try again: "; }
    return v;
}

double readDouble() {
    double v; while (!(cin >> v)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Invalid, try again: "; }
    return v;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<unique_ptr<Account>> accounts;

    while (true) {
        cout << "\n1.Create Savings  2.Create Current  3.List  4.Select  5.Exit\nChoice: ";
        int ch = readInt();
        if (ch == 1) {
            cout << "Account No: "; string no; cin >> no;
            if (findAccount(accounts, no)) { cout << "Exists.\n"; continue; }
            cout << "Init balance: "; double b = readDouble();
            cout << "Interest %: "; double r = readDouble();
            accounts.push_back(make_unique<SavingsAccount>(no, b, r));
            cout << "Saved.\n";
        } else if (ch == 2) {
            cout << "Account No: "; string no; cin >> no;
            if (findAccount(accounts, no)) { cout << "Exists.\n"; continue; }
            cout << "Init balance: "; double b = readDouble();
            cout << "Overdraft: "; double od = readDouble();
            accounts.push_back(make_unique<CurrentAccount>(no, b, od));
            cout << "Created.\n";
        } else if (ch == 3) {
            if (accounts.empty()) { cout << "No accounts.\n"; continue; }
            for (size_t i = 0; i < accounts.size(); ++i)
                cout << i+1 << ". " << accounts[i]->typeName() << " " << accounts[i]->number()
                     << " (Bal: " << fixed << setprecision(2) << accounts[i]->getBalance() << ")\n";
        } else if (ch == 4) {
            cout << "Account No: "; string no; cin >> no;
            Account* acc = findAccount(accounts, no);
            if (!acc) { cout << "Not found.\n"; continue; }
            while (true) {
                cout << "\n1.Display 2.Deposit 3.Withdraw 4.History 5.Undo 6.ApplyInterest 7.Back\nChoice: ";
                int a = readInt();
                if (a == 1) acc->display();
                else if (a == 2) { cout << "Amount: "; acc->deposit(readDouble()); cout << "Done.\n"; }
                else if (a == 3) { cout << "Amount: "; if (acc->withdraw(readDouble())) cout << "OK\n"; else cout << "Failed\n"; }
                else if (a == 4) acc->printHistory();
                else if (a == 5) { if (acc->undoLast()) cout << "Undone\n"; else cout << "None\n"; }
                else if (a == 6) {
                    if (auto s = dynamic_cast<SavingsAccount*>(acc)) { s->applyInterest(); cout << "Interest applied\n"; }
                    else cout << "Not a savings account\n";
                } else break;
            }
        } else break;
    }

    cout << "Bye.\n";
    return 0;
}
