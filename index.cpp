#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
using namespace std;

// Abstract base class for common functionalities
class AccountBase {
public:
    virtual void createAccount() = 0; // Pure virtual function
    virtual void displayAccount() const = 0; // Pure virtual function
};

// Derived class from abstract base class
class Account : public AccountBase {
private:
    int accountNumber;
    char *name;
    float balance;
    char type; // 'C' for Current, 'S' for Savings
    static int accountCounter; // Static data member for tracking account numbers

public:
    Account() : accountNumber(0), balance(0.0f), type(' ') {
        name = new char[50]; // Dynamic memory allocation
        name[0] = '\0';
    }

    // Constructor overloading
    Account(int accNo, const char* accName, char accType, float initialBalance) {
        accountNumber = accNo;
        name = new char[strlen(accName) + 1];
        strcpy(name, accName);
        type = toupper(accType);
        balance = initialBalance;
    }

    // Inline function
    inline int getAccountNumber() const {
        return accountNumber;
    }

    inline float getBalance() const {
        return balance;
    }

    inline char getAccountType() const {
        return type;
    }

    inline const char* getName() const {
        return name;
    }

    void createAccount() override {
        cout << "\nEnter Account Number: ";
        cin >> accountNumber;
        cout << "Enter Account Holder Name: ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "Enter Account Type (C/S): ";
        cin >> type;
        type = toupper(type);

        // Ensure valid initial deposit based on account type
        while (true) {
            cout << "Enter Initial Deposit Amount (>=500 for Savings, >=1000 for Current): ";
            cin >> balance;
            if ((type == 'S' && balance >= 500) || (type == 'C' && balance >= 1000)) {
                break;
            } else {
                cout << "Invalid amount! Please enter a valid initial deposit." << endl;
            }
        }

        cout << "\nAccount Created Successfully!";
    }

    void displayAccount() const override {
        cout << "\nAccount Number: " << accountNumber;
        cout << "\nAccount Holder Name: " << name;
        cout << "\nAccount Type: " << type;
        cout << "\nBalance: " << balance << endl;
    }

    // Overloaded deposit function (Operator Overloading)
    void operator+(float amount) {
        if (amount > 0) {
            balance += amount;
            cout << "\nAmount Deposited: " << amount;
        } else {
            cout << "\nInvalid Deposit Amount!";
        }
    }

    // Overloaded withdraw function (Operator Overloading)
    void operator-(float amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "\nAmount Withdrawn: " << amount;
        } else {
            cout << "\nInvalid Withdrawal Amount or Insufficient Balance!";
        }
    }

    // Destructor to free allocated memory
    ~Account() {
        delete[] name;
    }

    // Friend class
    friend class AccountManager;
};

// Static data member initialization
int Account::accountCounter = 0;

// AccountManager class to manage multiple accounts using an array
class AccountManager {
private:
    Account* accounts[100]; // Array of pointers to Account objects
    int count;

public:
    AccountManager() : count(0) {}

    // Add a new account to the manager
    void addAccount() {
        accounts[count] = new Account(); // Dynamic allocation for Account object
        accounts[count]->createAccount();
        count++;
    }

    // Display all accounts
    void displayAllAccounts() {
        cout << "\n\n\t\tACCOUNT HOLDER LIST\n";
        cout << "====================================================\n";
        cout << "A/c No.      NAME           Type  Balance\n";
        cout << "====================================================\n";
        for (int i = 0; i < count; i++) {
            cout << setw(10) << accounts[i]->getAccountNumber() << "  ";
            cout << setw(15) << accounts[i]->getName() << "  ";
            cout << setw(5) << accounts[i]->getAccountType() << "  ";
            cout << setw(10) << accounts[i]->getBalance() << endl;
        }
    }

    // Deposit or withdraw amount using arrow operator and operator overloading
    void depositWithdraw(int accountNumber, float amount, bool isDeposit) {
        for (int i = 0; i < count; i++) {
            if (accounts[i]->getAccountNumber() == accountNumber) {
                if (isDeposit) {
                    *accounts[i] + amount; // Using operator overloading for deposit
                } else {
                    *accounts[i] - amount; // Using operator overloading for withdraw
                }
                return;
            }
        }
        cout << "\nAccount number does not exist!";
    }

    // Destructor to free allocated memory
    ~AccountManager() {
        for (int i = 0; i < count; i++) {
            delete accounts[i];
        }
    }
};

// Template class for file handling
template <typename T>
class FileHandler {
public:
    void writeToFile(T& obj, const char* filename) {
        ofstream outFile(filename, ios::binary | ios::app);
        if (!outFile) {
            cout << "\nFile could not be opened!";
            return;
        }
        outFile.write(reinterpret_cast<char*>(&obj), sizeof(T));
        outFile.close();
    }

    void readFromFile(T& obj, const char* filename) {
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            cout << "\nFile could not be opened!";
            return;
        }
        while (inFile.read(reinterpret_cast<char*>(&obj), sizeof(T))) {
            obj.displayAccount();
        }
        inFile.close();
    }
};

// Function prototypes
void writeAccount(AccountManager& manager);
void displayAllAccounts(AccountManager& manager);
void depositWithdraw(AccountManager& manager, bool isDeposit);

int main() {
    AccountManager manager;
    int choice;
    int accountNumber;
    float amount;

    do {
        cout << "\n\n\t\tBANK MANAGEMENT SYSTEM";
        cout << "\n1. Create New Account";
        cout << "\n2. Deposit Amount";
        cout << "\n3. Withdraw Amount";
        cout << "\n4. Display All Accounts";
        cout << "\n5. Exit";
        cout << "\nSelect Your Option (1-5): ";
        cin >> choice;

        switch (choice) {
            case 1:
                writeAccount(manager);  // Create a new account
                break;
            case 2:
                cout << "\nEnter Account Number: ";
                cin >> accountNumber;
                cout << "\nEnter amount to deposit: ";
                cin >> amount;
                manager.depositWithdraw(accountNumber, amount, true);  // Deposit amount
                break;
            case 3:
                cout << "\nEnter Account Number: ";
                cin >> accountNumber;
                cout << "\nEnter amount to withdraw: ";
                cin >> amount;
                manager.depositWithdraw(accountNumber, amount, false); // Withdraw amount
                break;
            case 4:
                displayAllAccounts(manager);  // Display all accounts
                break;
            case 5:
                cout << "\nThank you for using the bank management system.\n";
                break;
            default:
                cout << "\nInvalid option! Please select a number between 1 and 5.";
        }
    } while (choice != 5);  // Continue until the user chooses to exit

    return 0;
}

// Function to write account details to the file using templates
void writeAccount(AccountManager& manager) {
    manager.addAccount();
}

// Function to display all accounts using AccountManager
void displayAllAccounts(AccountManager& manager) {
    manager.displayAllAccounts();
}
