// 7.3_practical.cpp
// Simple hierarchical banking system with transaction history and undo.
// Compile with: g++ -std=c++17 7.3_practical.cpp -o banking

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <limits>

using std::cin; using std::cout; using std::endl;
using std::string;

// Simple timestamp helper
static string now_str() {
    std::time_t t = std::time(nullptr);
    std::tm tm{};
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

enum class TxType { DEPOSIT, WITHDRAW, INTEREST };

struct Transaction {
    TxType type;
    double amount;
    double prevBalance;
    double newBalance;
    string note;
    string timestamp;
};

// Base Account class
class Account {
protected:
    int accountNumber;
    double balance;
    std::vector<Transaction> history;

public:
    Account(int acctNo, double initBalance = 0.0)
        : accountNumber(acctNo), balance(initBalance) {
        // constructor
    }

    virtual ~Account() {
        // destructor
    }

    int getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    virtual string accountType() const = 0;

    virtual bool deposit(double amount, const string &note = "") {
        if (amount <= 0) return false;
        double prev = balance;
        balance += amount;
        history.push_back({TxType::DEPOSIT, amount, prev, balance, note, now_str()});
        return true;
    }

    // withdraw should be overridden where needed
    virtual bool withdraw(double amount, const string &note = "") {
        if (amount <= 0) return false;
        if (amount > balance) return false;
        double prev = balance;
        balance -= amount;
        history.push_back({TxType::WITHDRAW, amount, prev, balance, note, now_str()});
        return true;
    }

    // Apply interest (savings override to compute interest; base just records)
    virtual void applyInterest(double amount, const string &note = "Interest applied") {
        if (amount == 0) return;
        double prev = balance;
        balance += amount;
        history.push_back({TxType::INTEREST, amount, prev, balance, note, now_str()});
    }

    void display() const {
        cout << "Account #" << accountNumber << " (" << accountType() << ")\n";
        cout << "  Balance: " << std::fixed << std::setprecision(2) << balance << "\n";
    }

    void showHistory() const {
        if (history.empty()) {
            cout << "  No transactions.\n";
            return;
        }
        cout << "  Transaction history (most recent last):\n";
        for (size_t i = 0; i < history.size(); ++i) {
            const Transaction &t = history[i];
            string type;
            if (t.type == TxType::DEPOSIT) type = "Deposit";
            else if (t.type == TxType::WITHDRAW) type = "Withdraw";
            else type = "Interest";
            cout << "   [" << i+1 << "] " << t.timestamp << " | " << type
                 << " | Amount: " << t.amount
                 << " | Prev: " << t.prevBalance
                 << " | New: " << t.newBalance;
            if (!t.note.empty()) cout << " | " << t.note;
            cout << "\n";
        }
    }

    bool undoLastTransaction() {
        if (history.empty()) return false;
        Transaction last = history.back();
        // restore previous balance exactly
        balance = last.prevBalance;
        history.pop_back();
        return true;
    }
};

// SavingsAccount: adds interest rate
class SavingsAccount : public Account {
    double interestRate; // annual rate as percentage (e.g., 3.5 => 3.5%)
public:
    SavingsAccount(int acctNo, double initBalance, double rate)
        : Account(acctNo, initBalance), interestRate(rate) {}

    ~SavingsAccount() override {}

    string accountType() const override { return "Savings"; }

    // apply interest by percentage over current balance (simple one-time)
    void applyAnnualInterest() {
        // interest for one year, simple calculation
        double interest = balance * (interestRate / 100.0);
        applyInterest(interest, "Annual interest");
    }

    // Allow withdraw only when enough balance
    bool withdraw(double amount, const string &note = "") override {
        return Account::withdraw(amount, note);
    }

    double getInterestRate() const { return interestRate; }
};

// CurrentAccount: adds overdraft limit
class CurrentAccount : public Account {
    double overdraftLimit; // positive number representing allowed negative balance
public:
    CurrentAccount(int acctNo, double initBalance, double overdraft)
        : Account(acctNo, initBalance), overdraftLimit(overdraft) {}

    ~CurrentAccount() override {}

    string accountType() const override { return "Current"; }

    bool withdraw(double amount, const string &note = "") override {
        if (amount <= 0) return false;
        double allowedMin = -overdraftLimit;
        if ((balance - amount) < allowedMin) return false;
        double prev = balance;
        balance -= amount;
        history.push_back({TxType::WITHDRAW, amount, prev, balance, note, now_str()});
        return true;
    }

    double getOverdraftLimit() const { return overdraftLimit; }
};

// Simple account manager and user interface
class Bank {
    std::vector<std::unique_ptr<Account>> accounts;
    int nextAccountNumber = 1001;

    Account* findAccount(int acctNo) {
        for (auto &p : accounts) if (p->getAccountNumber() == acctNo) return p.get();
        return nullptr;
    }

public:
    void createSavings(double initBalance, double rate) {
        accounts.push_back(std::make_unique<SavingsAccount>(nextAccountNumber++, initBalance, rate));
        cout << "Created Savings account #" << (nextAccountNumber-1) << "\n";
    }

    void createCurrent(double initBalance, double overdraft) {
        accounts.push_back(std::make_unique<CurrentAccount>(nextAccountNumber++, initBalance, overdraft));
        cout << "Created Current account #" << (nextAccountNumber-1) << "\n";
    }

    void listAccounts() const {
        if (accounts.empty()) {
            cout << "No accounts available.\n";
            return;
        }
        cout << "Accounts:\n";
        for (auto &p : accounts) {
            cout << "  #" << p->getAccountNumber() << " | " << p->accountType()
                 << " | Balance: " << std::fixed << std::setprecision(2) << p->getBalance() << "\n";
        }
    }

    void performMenu();
};

static void ignoreLine() {
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Bank::performMenu() {
    while (true) {
        cout << "\n--- Simple Bank Menu ---\n";
        cout << "1) Create Savings Account\n";
        cout << "2) Create Current Account\n";
        cout << "3) List Accounts\n";
        cout << "4) Operate on Account\n";
        cout << "0) Exit\n";
        cout << "Choose: ";
        int ch;
        if (!(cin >> ch)) { cin.clear(); ignoreLine(); continue; }

        if (ch == 0) break;

        if (ch == 1) {
            double init, rate;
            cout << "Initial balance: "; cin >> init;
            cout << "Interest rate (percent): "; cin >> rate;
            createSavings(init, rate);
        } else if (ch == 2) {
            double init, overd;
            cout << "Initial balance: "; cin >> init;
            cout << "Overdraft limit: "; cin >> overd;
            createCurrent(init, overd);
        } else if (ch == 3) {
            listAccounts();
        } else if (ch == 4) {
            int acctNo;
            cout << "Enter account number: "; cin >> acctNo;
            Account* acct = findAccount(acctNo);
            if (!acct) { cout << "Account not found.\n"; continue; }
            while (true) {
                cout << "\nAccount #" << acct->getAccountNumber() << " (" << acct->accountType() << ")\n";
                cout << " a) Deposit\n b) Withdraw\n c) Display details\n d) Show history\n e) Undo last transaction\n f) Apply interest (savings only)\n q) Back\nChoose: ";
                char op; cin >> op;
                if (op == 'q') break;
                if (op == 'a') {
                    double amt; cout << "Amount to deposit: "; cin >> amt;
                    string note;
                    ignoreLine();
                    cout << "Note (optional): "; std::getline(cin, note);
                    if (acct->deposit(amt, note)) cout << "Deposit successful.\n"; else cout << "Deposit failed.\n";
                } else if (op == 'b') {
                    double amt; cout << "Amount to withdraw: "; cin >> amt;
                    ignoreLine();
                    string note; cout << "Note (optional): "; std::getline(cin, note);
                    if (acct->withdraw(amt, note)) cout << "Withdrawal successful.\n"; else cout << "Withdrawal denied.\n";
                } else if (op == 'c') {
                    acct->display();
                    // show extra details for specialized accounts
                    if (acct->accountType() == "Savings") {
                        SavingsAccount* s = static_cast<SavingsAccount*>(acct);
                        cout << "  Interest rate: " << s->getInterestRate() << "%\n";
                    } else if (acct->accountType() == "Current") {
                        CurrentAccount* c = static_cast<CurrentAccount*>(acct);
                        cout << "  Overdraft limit: " << c->getOverdraftLimit() << "\n";
                    }
                } else if (op == 'd') {
                    acct->showHistory();
                } else if (op == 'e') {
                    if (acct->undoLastTransaction()) cout << "Last transaction undone.\n";
                    else cout << "No transaction to undo.\n";
                } else if (op == 'f') {
                    if (acct->accountType() == "Savings") {
                        SavingsAccount* s = static_cast<SavingsAccount*>(acct);
                        s->applyAnnualInterest();
                        cout << "Interest applied.\n";
                    } else {
                        cout << "Apply interest available only for Savings accounts.\n";
                    }
                } else {
                    cout << "Unknown option.\n";
                }
            }
        } else {
            cout << "Invalid selection.\n";
        }
    }
}

int main() {
    Bank bank;
    cout << "Welcome to the simple banking system.\n";
    bank.performMenu();
    cout << "Goodbye.\n";
    return 0;
}