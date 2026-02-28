#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

class Loan {
private:
    int loanID;
    string applicantName;
    double loanAmount;
    double annualInterestRate;
    int tenureMonths;

public:
    // Default Constructor (No Initialization)
    Loan() {}

    // Parameterized Constructor
    Loan(int id, string name, double amount, double rate, int months)
        : loanID(id), applicantName(name),
          loanAmount(amount), annualInterestRate(rate),
          tenureMonths(months) {}

    // Function to input data
    void inputDetails() {
        cout << "\nEnter Loan ID: ";
        cin >> loanID;

        cout << "Enter Applicant Name: ";
        cin.ignore();
        getline(cin, applicantName);

        cout << "Enter Loan Amount: ";
        cin >> loanAmount;

        cout << "Enter Annual Interest Rate (%): ";
        cin >> annualInterestRate;

        cout << "Enter Loan Tenure (Months): ";
        cin >> tenureMonths;
    }

    // EMI Calculation
    double calculateEMI() const {
        double monthlyRate = annualInterestRate / (12 * 100);

        if (monthlyRate == 0)   // If interest rate is 0
            return loanAmount / tenureMonths;

        double emi = (loanAmount * monthlyRate * pow(1 + monthlyRate, tenureMonths)) /
                     (pow(1 + monthlyRate, tenureMonths) - 1);

        return emi;
    }

    // Display Details
    void displayDetails() const {
        cout << "\n----- Loan Details -----";
        cout << "\nLoan ID: " << loanID;
        cout << "\nApplicant Name: " << applicantName;
        cout << "\nLoan Amount: " << loanAmount;
        cout << "\nAnnual Interest Rate: " << annualInterestRate << "%";
        cout << "\nTenure (Months): " << tenureMonths;
        cout << "\nMonthly EMI: " << calculateEMI();
        cout << "\n------------------------\n";
    }
};

int main() {
    int n;
    cout << "Enter number of loans: ";
    cin >> n;

    vector<Loan> loans(n);

    // Input details
    for (int i = 0; i < n; i++) {
        cout << "\nEntering details for Loan " << i + 1;
        loans[i].inputDetails();
    }

    // Display all loans
    for (const auto &loan : loans) {
        loan.displayDetails();
    }

    return 0;
}