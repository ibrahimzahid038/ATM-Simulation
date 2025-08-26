#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>   

using namespace std;
 
class Account {
private:
    double balance;

public:
    Account(double startBalance = 0.0) : balance(startBalance) {}

    double getBalance() const {
        return balance;
    }
     
    void deposit(double amount) {
        if (amount > 0.0) balance += amount;
    }
     
    bool withdraw(double amount) {
        if (amount <= 0.0) return false;
        if (amount > balance) return false;
        balance -= amount;
        return true;
    }
     
    bool saveToFile(const string& filename) const {
        ofstream out(filename.c_str());
        if (!out) return false;
        out << fixed << setprecision(2) << balance << '\n';
        out.close();
        return true;
    }
     
    bool loadFromFile(const string& filename) {
        ifstream in(filename.c_str());
        if (!in) return false;
        double b;
        if (!(in >> b)) {
            in.close();
            return false;
        }
        balance = b;
        in.close();
        return true;
    }
};
 
class ATM {
private:
    Account account;
    const string storageFile = "balance.txt";
     
    double readAmount(const string& prompt) {
        double value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                cin.ignore(10000, '\n');  
                return value;
            }
            else {
                cout << "Invalid input. Please enter a numeric value.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }
    }
     
    void showMenu() const {
        cout << "\n=== Simple ATM Menu ===\n";
        cout << "1. Check balance\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Exit\n";
        cout << "Choose an option (1-4): ";
    }

public:
    ATM() { 
        if (!account.loadFromFile(storageFile)) {
            account = Account(0.0);
        }
    }
 
    void run() {
        cout << fixed << setprecision(2);
        cout << "Welcome to the ATM Simulation.\n";

        bool running = true;
        while (running) {
            showMenu();
            int choice;
            if (!(cin >> choice)) {
                cout << "Please enter a number between 1 and 4.\n";
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }
            cin.ignore(10000, '\n');  

            switch (choice) {
            case 1: {  
                cout << "Current balance: $" << account.getBalance() << '\n';
                break;
            }
            case 2: {  
                double amount = readAmount("Enter amount to deposit: $");
                if (amount <= 0.0) {
                    cout << "Deposit amount must be positive.\n";
                }
                else {
                    account.deposit(amount);
                    cout << "Deposited $" << amount << ". New balance: $" << account.getBalance() << '\n';
                }
                break;
            }
            case 3: { 
                double amount = readAmount("Enter amount to withdraw: $");
                if (amount <= 0.0) {
                    cout << "Withdrawal amount must be positive.\n";
                }
                else if (account.withdraw(amount)) {
                    cout << "Please take your cash: $" << amount << ". New balance: $" << account.getBalance() << '\n';
                }
                else {
                    cout << "Insufficient funds or invalid amount. Current balance: $" << account.getBalance() << '\n';
                }
                break;
            }
            case 4: {   
                if (account.saveToFile(storageFile)) {
                    cout << "Balance saved. Goodbye!\n";
                }
                else {
                    cout << "Could not save balance to file. Goodbye!\n";
                }
                running = false;
                break;
            }
            default:
                cout << "Invalid choice. Please enter 1-4.\n";
            }
        }
    }
};
 
int main() {
    ATM atm;
    atm.run();
    return 0;
}
