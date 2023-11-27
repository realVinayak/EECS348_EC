#include <iostream>
#include <string>
#include <utility>
#include <iomanip>

using namespace std;

class Account{
public:
    Account(string accountNumber, string accountHolder, float balance){
        this->accountNumber = std::move(accountNumber);
        this->accountHolder = std::move(accountHolder);
        this->balance = balance;
    }
    void deposit(float amount){
        this->balance += amount;
    }

    virtual int withdraw(float amount){
        float finalBalance = balance - amount;
        if (finalBalance < 0) {
            cout << "Withdraw aborted as not enough balance" << endl;
            return -1;
        }
        this->balance = finalBalance;
        return 0;
    }

    string accountNumber;
    string accountHolder;
    float balance;
};

class SavingsAccount: public Account{
public:
    SavingsAccount(string accountNumber,
                   string accountHolder,
                   float balance,
                   float interestRate): Account(std::move(accountNumber), std::move(accountHolder), balance){
        this->interestRate = interestRate;
    }
    int withdraw(float amount) override{
        float finalBalance = balance - amount;
        if (finalBalance < 5.00) {
            cout << "Withdraw aborted as the final balance will be less than " << 5.00 << endl;
            return -1;
        }
        return Account::withdraw(amount);
    }
    void displayDetails(){
        cout << "Account Details for Savings Account (ID: " << this->accountNumber << "):" << endl;
        cout << "Holder: " << this->accountHolder << endl;
        cout << "Balance: $" << setprecision(2) << fixed << this->balance << endl;
        cout << "Interest Rate: " << setprecision(2) << fixed << this->interestRate * 100<< "%" << endl;
        cout << endl;
    }
    SavingsAccount operator+(SavingsAccount &savingAccount){
        SavingsAccount newAccount = SavingsAccount(this->accountNumber,
                                                   this->accountHolder,
                                                   this->balance,
                                                   this->interestRate);
        float toWithdraw = newAccount.balance;
        if (savingAccount.withdraw(toWithdraw) < 0){
            cout << "Addition Unsuccessful!" << endl;
            exit(1);
        }else{
            newAccount.deposit(toWithdraw);
        }
        return newAccount;
    }
    float interestRate;
};

ostream & operator<<(ostream &os, SavingsAccount &obj){
    os << "Account Details for Savings Account (ID: " << obj.accountNumber << "):" << endl;
    os << "Holder: " << obj.accountHolder << endl;
    os << "Balance: $" << setprecision(2) << fixed << obj.balance << endl;
    os << "Interest Rate: " << setprecision(2) << fixed << obj.interestRate * 100 << endl;
    cout << endl;
    return os;
}


class CurrentAccount: public Account {
public:
    CurrentAccount(string accountNumber,
                   string accountHolder,
                   float balance,
                   float overdraftLimit) : Account(std::move(accountNumber), std::move(accountHolder), balance) {
        this->overdraftLimit = overdraftLimit;
    }

    int withdraw(float amount) override {
        if (amount - balance <= overdraftLimit){
            this->balance -= amount;
            return 0;
        }else{
            return Account::withdraw(amount);
        }
    }
    void displayDetails(){
        cout << "Account Details for Current Account (ID: " << this->accountNumber << "):" << endl;
        cout << "Holder: " << this->accountHolder << endl;
        cout << "Balance: $" << setprecision(2) << fixed << this->balance << endl;
        cout << "Overdraft Limit: $" << setprecision(2) << fixed << this->overdraftLimit << endl;
        cout << endl;
    }
    CurrentAccount operator+(SavingsAccount &savingsAccount){
        CurrentAccount newAccount = CurrentAccount(this->accountNumber, this->accountHolder, this->balance, this->overdraftLimit);
        float savingsBalance = 300;
        if(savingsAccount.withdraw(savingsBalance) < 0){
            cout << "Addition Unsuccessful!" << endl;
            exit(1);
        } else {
            newAccount.deposit(savingsBalance);
        }
        return newAccount;
    }
    CurrentAccount operator+(CurrentAccount &currentAccount){
        CurrentAccount newAccount = CurrentAccount(this->accountNumber, this->accountHolder, this->balance, this->overdraftLimit);
        float balance = 300;
        if(currentAccount.withdraw(balance) < 0){
            cout << "Addition Unsuccessful!" << endl;
            exit(1);
        } else {
            newAccount.deposit(balance);
        }
        return newAccount;
    }
    float overdraftLimit;
};

ostream & operator<<(ostream &os, CurrentAccount &obj){
    os << "Account Details for Current Account (ID: " << obj.accountNumber << "):" << endl;
    os << "Holder: " << obj.accountHolder << endl;
    os << "Balance: $" << setprecision(2) << fixed << obj.balance << endl;
    os << "Overdraft Limit: $" << setprecision(2) << fixed << obj.overdraftLimit << endl;
    os << endl;
    return os;
}

SavingsAccount operator+(SavingsAccount &savingsAccount, CurrentAccount &currentAccount){
    SavingsAccount newAccount = SavingsAccount(savingsAccount.accountNumber,
                                               savingsAccount.accountHolder,
                                               savingsAccount.balance,
                                               savingsAccount.interestRate);
    float toWithdraw = newAccount.balance;
    if (currentAccount.withdraw(toWithdraw) < 0){
        cout << "Addition Unsuccessful!" << endl;
        exit(1);
    }else{
        newAccount.deposit(toWithdraw);
    }
    return newAccount;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    savings.displayDetails();
    current.displayDetails();

    savings.deposit(500);
    current.withdraw(1000);
    
    savings.displayDetails();
    current.displayDetails();

    // Transfer 300 from savings to current
    current = current + savings;

    savings.displayDetails();
    current.displayDetails();

    return 0;
}