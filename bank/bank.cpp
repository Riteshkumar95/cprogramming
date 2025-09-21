#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace std;

class Transaction {
private:
    string type;
    double amount;
    string date;
    
public:
    Transaction(string t, double amt) : type(t), amount(amt) {
        time_t now = time(0);
        char* dt = ctime(&now);
        date = string(dt);
        date.pop_back(); // Remove newline
    }
    
    void display() const {
        cout << setw(12) << type << setw(12) << fixed << setprecision(2) 
             << amount << "  " << date << endl;
    }
    
    string getTransactionData() const {
        return type + "," + to_string(amount) + "," + date;
    }
};

class Account {
private:
    int accountNumber;
    string accountHolder;
    double balance;
    string accountType;
    vector<Transaction> transactions;
    
public:
    // Constructor
    Account(int accNum, string holder, double bal, string type) 
        : accountNumber(accNum), accountHolder(holder), balance(bal), accountType(type) {}
    
    // Getters
    int getAccountNumber() const { return accountNumber; }
    string getAccountHolder() const { return accountHolder; }
    double getBalance() const { return balance; }
    string getAccountType() const { return accountType; }
    
    // Banking operations
    bool deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount!" << endl;
            return false;
        }
        balance += amount;
        transactions.push_back(Transaction("DEPOSIT", amount));
        cout << "Deposit successful! New balance: $" << fixed << setprecision(2) << balance << endl;
        return true;
    }
    
    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount!" << endl;
            return false;
        }
        if (amount > balance) {
            cout << "Insufficient funds! Available balance: $" << fixed << setprecision(2) << balance << endl;
            return false;
        }
        balance -= amount;
        transactions.push_back(Transaction("WITHDRAW", amount));
        cout << "Withdrawal successful! New balance: $" << fixed << setprecision(2) << balance << endl;
        return true;
    }
    
    bool transfer(Account& toAccount, double amount) {
        if (amount <= 0) {
            cout << "Invalid transfer amount!" << endl;
            return false;
        }
        if (amount > balance) {
            cout << "Insufficient funds for transfer!" << endl;
            return false;
        }
        
        balance -= amount;
        toAccount.balance += amount;
        
        transactions.push_back(Transaction("TRANSFER_OUT", amount));
        toAccount.transactions.push_back(Transaction("TRANSFER_IN", amount));
        
        cout << "Transfer successful!" << endl;
        cout << "From Account " << accountNumber << " to Account " << toAccount.accountNumber << endl;
        cout << "Amount: $" << fixed << setprecision(2) << amount << endl;
        return true;
    }
    
    void displayAccountInfo() const {
        cout << "\n==================== ACCOUNT INFO ====================" << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolder << endl;
        cout << "Account Type: " << accountType << endl;
        cout << "Current Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << "=====================================================" << endl;
    }
    
    void displayTransactionHistory() const {
        cout << "\n================= TRANSACTION HISTORY =================" << endl;
        cout << "Account Number: " << accountNumber << " - " << accountHolder << endl;
        cout << "-------------------------------------------------------" << endl;
        
        if (transactions.empty()) {
            cout << "No transactions found." << endl;
        } else {
            cout << setw(12) << "TYPE" << setw(12) << "AMOUNT" << "  DATE" << endl;
            cout << "-------------------------------------------------------" << endl;
            for (const auto& transaction : transactions) {
                transaction.display();
            }
        }
        cout << "=======================================================" << endl;
    }
    
    // File operations
    string getAccountData() const {
        stringstream ss;
        ss << accountNumber << "," << accountHolder << "," << balance << "," << accountType;
        return ss.str();
    }
    
    void updateBalance(double newBalance) {
        balance = newBalance;
    }
};

class BankManagementSystem {
private:
    vector<Account> accounts;
    int nextAccountNumber;
    const string filename = "bank_data.txt";
    
public:
    BankManagementSystem() : nextAccountNumber(1001) {
        loadAccountsFromFile();
    }
    
    ~BankManagementSystem() {
        saveAccountsToFile();
    }
    
    void createAccount() {
        string name, type;
        double initialDeposit;
        
        cout << "\n============ CREATE NEW ACCOUNT ============" << endl;
        cout << "Enter account holder name: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "Enter account type (Savings/Current/Fixed): ";
        getline(cin, type);
        
        cout << "Enter initial deposit amount: $";
        cin >> initialDeposit;
        
        if (initialDeposit < 0) {
            cout << "Invalid initial deposit amount!" << endl;
            return;
        }
        
        Account newAccount(nextAccountNumber, name, initialDeposit, type);
        accounts.push_back(newAccount);
        
        cout << "\nAccount created successfully!" << endl;
        cout << "Account Number: " << nextAccountNumber << endl;
        cout << "Account Holder: " << name << endl;
        cout << "Initial Balance: $" << fixed << setprecision(2) << initialDeposit << endl;
        
        nextAccountNumber++;
    }
    
    Account* findAccount(int accNum) {
        for (auto& account : accounts) {
            if (account.getAccountNumber() == accNum) {
                return &account;
            }
        }
        return nullptr;
    }
    
    void depositMoney() {
        int accNum;
        double amount;
        
        cout << "\n============ DEPOSIT MONEY ============" << endl;
        cout << "Enter account number: ";
        cin >> accNum;
        
        Account* account = findAccount(accNum);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }
        
        cout << "Enter deposit amount: $";
        cin >> amount;
        
        account->deposit(amount);
    }
    
    void withdrawMoney() {
        int accNum;
        double amount;
        
        cout << "\n============ WITHDRAW MONEY ============" << endl;
        cout << "Enter account number: ";
        cin >> accNum;
        
        Account* account = findAccount(accNum);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }
        
        cout << "Current balance: $" << fixed << setprecision(2) << account->getBalance() << endl;
        cout << "Enter withdrawal amount: $";
        cin >> amount;
        
        account->withdraw(amount);
    }
    
    void transferMoney() {
        int fromAcc, toAcc;
        double amount;
        
        cout << "\n============ TRANSFER MONEY ============" << endl;
        cout << "Enter source account number: ";
        cin >> fromAcc;
        
        Account* fromAccount = findAccount(fromAcc);
        if (!fromAccount) {
            cout << "Source account not found!" << endl;
            return;
        }
        
        cout << "Enter destination account number: ";
        cin >> toAcc;
        
        Account* toAccount = findAccount(toAcc);
        if (!toAccount) {
            cout << "Destination account not found!" << endl;
            return;
        }
        
        if (fromAcc == toAcc) {
            cout << "Cannot transfer to the same account!" << endl;
            return;
        }
        
        cout << "Available balance: $" << fixed << setprecision(2) << fromAccount->getBalance() << endl;
        cout << "Enter transfer amount: $";
        cin >> amount;
        
        fromAccount->transfer(*toAccount, amount);
    }
    
    void checkBalance() {
        int accNum;
        
        cout << "\n============ CHECK BALANCE ============" << endl;
        cout << "Enter account number: ";
        cin >> accNum;
        
        Account* account = findAccount(accNum);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }
        
        account->displayAccountInfo();
    }
    
    void viewTransactionHistory() {
        int accNum;
        
        cout << "\n========== TRANSACTION HISTORY ==========" << endl;
        cout << "Enter account number: ";
        cin >> accNum;
        
        Account* account = findAccount(accNum);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }
        
        account->displayTransactionHistory();
    }
    
    void displayAllAccounts() {
        cout << "\n============= ALL ACCOUNTS =============" << endl;
        if (accounts.empty()) {
            cout << "No accounts found in the system." << endl;
            return;
        }
        
        cout << setw(8) << "ACC NO" << setw(20) << "HOLDER NAME" 
             << setw(15) << "TYPE" << setw(12) << "BALANCE" << endl;
        cout << "-------------------------------------------------------" << endl;
        
        for (const auto& account : accounts) {
            cout << setw(8) << account.getAccountNumber() 
                 << setw(20) << account.getAccountHolder()
                 << setw(15) << account.getAccountType()
                 << setw(12) << fixed << setprecision(2) << account.getBalance() << endl;
        }
    }
    
    void deleteAccount() {
        int accNum;
        
        cout << "\n============ DELETE ACCOUNT ============" << endl;
        cout << "Enter account number to delete: ";
        cin >> accNum;
        
        auto it = find_if(accounts.begin(), accounts.end(),
            [accNum](const Account& acc) { return acc.getAccountNumber() == accNum; });
        
        if (it == accounts.end()) {
            cout << "Account not found!" << endl;
            return;
        }
        
        cout << "Account found:" << endl;
        it->displayAccountInfo();
        
        char confirm;
        cout << "\nAre you sure you want to delete this account? (y/n): ";
        cin >> confirm;
        
        if (confirm == 'y' || confirm == 'Y') {
            accounts.erase(it);
            cout << "Account deleted successfully!" << endl;
        } else {
            cout << "Account deletion cancelled." << endl;
        }
    }
    
    void saveAccountsToFile() {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& account : accounts) {
                file << account.getAccountData() << endl;
            }
            file.close();
        }
    }
    
    void loadAccountsFromFile() {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string item;
                vector<string> data;
                
                while (getline(ss, item, ',')) {
                    data.push_back(item);
                }
                
                if (data.size() == 4) {
                    int accNum = stoi(data[0]);
                    string holder = data[1];
                    double balance = stod(data[2]);
                    string type = data[3];
                    
                    Account account(accNum, holder, balance, type);
                    accounts.push_back(account);
                    
                    if (accNum >= nextAccountNumber) {
                        nextAccountNumber = accNum + 1;
                    }
                }
            }
            file.close();
        }
    }
    
    void displayMenu() {
        cout << "\n================= BANK MANAGEMENT SYSTEM =================" << endl;
        cout << "1. Create New Account" << endl;
        cout << "2. Deposit Money" << endl;
        cout << "3. Withdraw Money" << endl;
        cout << "4. Transfer Money" << endl;
        cout << "5. Check Balance" << endl;
        cout << "6. View Transaction History" << endl;
        cout << "7. Display All Accounts" << endl;
        cout << "8. Delete Account" << endl;
        cout << "9. Exit" << endl;
        cout << "==========================================================" << endl;
        cout << "Enter your choice: ";
    }
    
    void run() {
        int choice;
        
        cout << "Welcome to Bank Management System!" << endl;
        
        while (true) {
            displayMenu();
            cin >> choice;
            
            switch (choice) {
                case 1:
                    createAccount();
                    break;
                case 2:
                    depositMoney();
                    break;
                case 3:
                    withdrawMoney();
                    break;
                case 4:
                    transferMoney();
                    break;
                case 5:
                    checkBalance();
                    break;
                case 6:
                    viewTransactionHistory();
                    break;
                case 7:
                    displayAllAccounts();
                    break;
                case 8:
                    deleteAccount();
                    break;
                case 9:
                    cout << "Thank you for using Bank Management System!" << endl;
                    return;
                default:
                    cout << "Invalid choice! Please try again." << endl;
            }
            
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
};

int main() {
    BankManagementSystem bank;
    bank.run();
    return 0;
}