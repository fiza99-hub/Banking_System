#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

class Transaction {
public:
    string type;
    int amount; 
    Transaction(string t, int a) : type(t), amount(a) {}
};

class Account {
public:
    string accountnumber;
    string name;          
    double balance;      
    double loanAmount;    
    double loanInterestRate; 
    double loanBalance;    
    Account* next;       
    vector<Transaction> t;

    Account(string accnum, string accname, double accbalance) {
        accountnumber = accnum;
        name = accname;
        balance = accbalance;
        loanAmount = 0.0; 
        loanInterestRate = 0.0; 
        loanBalance = 0.0; 
        next = NULL;
    }

    void addTransaction(string type, int amount) { 
        t.push_back(Transaction(type, amount));
    }
    
    void displayTransactions() {
        cout << "Transaction Record for Account Number: " << accountnumber << endl;
        for (int i = 0; i < t.size(); ++i) {
            cout << t[i].type << ": " << t[i].amount << endl;
        }
    }
};

class Bank {
private:
    Account* start;
    Account* current;
public:
    Bank() {
        start = NULL;
        current = NULL;
    }

    void createaccount(string accnum, string accname, double initialdeposit) {
        Account* newAccount = new Account(accnum, accname, initialdeposit);
        if (start == NULL) {
            start = newAccount;
        } else {
            current = start;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newAccount;
        }
        cout << "Account created for " << accname << " with initial balance of " << initialdeposit << endl;
    }

    void deposit(string accnum, int amount) {
        if (amount <= 0) {
            cout << "~~~ Deposit amount must be a positive integer! ~~~" << endl;
            return;
        }

        current = start;
        while (current != NULL) {
            if (current->accountnumber == accnum) {
                current->balance += amount;
                current->addTransaction("Deposit", amount); 
                cout << "|| Deposited " << amount << " into account " << accnum << ". New balance || " << current->balance << endl;
                return;
            }
            current = current->next;
        }
        cout << "~~~ Account not found! ~~~" << endl;
    }

    void withdraw(string accnum, int amount) {
        if (amount <= 0) {
            cout << "~~~ Withdrawal amount must be a positive integer! ~~~" << endl;
            return;
        }

        current = start;
        while (current != NULL) {
            if (current->accountnumber == accnum) {
                if (current->balance >= amount) {
                    current->balance -= amount;
                    current->addTransaction("Withdraw", amount);
                    cout << "|| Withdrew " << amount << " from account " << accnum << ". New balance || " << current->balance << endl;
                } else {
                    cout << "~~~ Insufficient balance! ~~~" << endl;
                }
                return;
            }
            current = current->next;
        }
        cout << "~~~ Account not found! ~~~" << endl;
    }

    void displayAccount(string accnum) {
        current = start;
        while (current != NULL) {
            if (current->accountnumber == accnum) {
                cout << "~> Account Number: " << current->accountnumber << endl;
                cout << "~> Account Holder Name: " << current->name << endl;
                cout << "~> Balance: " << current->balance << endl;
                current->displayTransactions();
                return;
            }
            current = current->next;
        }
        cout << " --- Account not found! ---" << endl;
    }

    void deleteAccount(string accnum) {
        if (start == NULL) {
            cout << "=== No accounts available to delete. ===" << endl;
            return;
        }

        if (start->accountnumber == accnum) {
            Account* temp = start;
            start = start->next;
            delete temp;
            cout << "|| Account " << accnum << " deleted successfully ||" << endl;
            return;
        }

        current = start;
        while (current->next != NULL && current->next->accountnumber != accnum) {
            current = current->next;
        }

        if (current->next == NULL) {
            cout << "=== Account not found! ===" << endl;
        } else {
            Account* del = current->next;
            current->next = current->next->next;
            delete del;
            cout << "|| Account " << accnum << " deleted successfully ||" << endl;
        }
    }

    void displayAllAccounts() {
        if (start == NULL) {
            cout << "??? No accounts available ???" << endl;
            return;
        }

        current = start;
        cout << "=== All Accounts ===" << endl;
        while (current != NULL) {
            cout << "|| Account Number: " << current->accountnumber << " || Name: " << current->name << " || Balance: " << current->balance << endl;
            current = current->next;
        }
    }

    void searchAccount(string accnum) {
        current = start;
        while (current != NULL) {
            if (current->accountnumber == accnum) {
                cout << "|| Account found: ||" << endl;
                cout << "|| Account Number: ||" << current->accountnumber << " || Name: || " << current->name << " || Balance: || " << current->balance << endl;
                return;
            }
            current = current->next;
        }
        cout << ".... Account not found! ...." << endl;
    }

    void addInterest(string accnum, double interestRate) {
        current = start;
        while (current != NULL) {
            if (current->accountnumber == accnum) {
                double interest = (current->balance * interestRate) / 100;
                current->balance += interest;
                current->addTransaction("Interest", static_cast<int>(interest)); 
                cout << "|| Added interest of || " << interest << " to account " << accnum << " || New balance: || " << current->balance << endl;
                return;
            }
            current = current->next;
        }
    }

    void requestLoan(string accnum, int amount, double interestRate) {
        current = start;
        while (current != NULL) {
            if (current->accountnumber == accnum) {
                current->loanAmount = amount;
                current->loanInterestRate = interestRate;
                current->loanBalance = amount; 
                current->addTransaction("Loan", amount);
                cout << "|| Loan of " << amount << " approved for account " << accnum << " at an interest rate of " << interestRate << "% ||" << endl;
                return;
            }
            current = current->next;
        }
        cout << "~~~ Account not found! ~~~" << endl;
    }

    void repayLoan(string accnum, int amount) {
        current = start;
        while (current != NULL) {
            if (current->accountnumber == accnum) {
                if (amount <= 0) {
                    cout << "~~~ Repayment amount must be a positive integer! ~~~" << endl;
                    return;
                }
                if (current->loanBalance >= amount) {
                    current->loanBalance -= amount;
                    current->addTransaction("Loan Repayment", amount);
                    cout << "|| Repaid " << amount << " towards loan for account " << accnum << ". Remaining loan balance: " << current->loanBalance << " ||" << endl;
                } else {
                    cout << "~~~ Repayment exceeds loan balance! ~~~" << endl;
                }
                return;
            }
            current = current->next;
        }
        cout << "~~~ Account not found! ~~~" << endl;
    }

    void displayLoanInfo(string accnum) {
        current = start;
        while (current != NULL) {
            if (current->accountnumber == accnum) {
                cout << "Loan Information for Account Number: " << current->accountnumber << endl;
                cout << "Loan Amount: " << current->loanAmount << endl;
                cout << "Interest Rate: " << current->loanInterestRate << "%" << endl;
                cout << "Remaining Loan Balance: " << current->loanBalance << endl;
                return;
                
            }
            current = current->next;
        }
        cout << " --- Account not found! ---" << endl;
    }
};

bool isValidName(const string& name) {
    if (name.empty()) return false; 
    for (char c : name) {
        if (!isalpha(c) && c != ' ')
		 return false;
    }
    return true;
}

int main() {
    Bank b;
    int choice;
    string accnum, name;
    int amount; 
    double interestRate;

    while (true) {
        cout << "\nBanking System Menu:\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Display Account\n";
        cout << "5. Delete Account\n";
        cout << "6. Display All Accounts\n";
        cout << "7. Search Account by Number\n";
        cout << "8. Add Interest\n";
        cout << "9. Request Loan\n"; 
        cout << "10. Repay Loan\n"; 
        cout << "11. Display Loan Info\n"; 
        cout << "12. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Account Number: ";
                cin >> accnum;
                cin.ignore();
                cout << "Enter Account Holder's Name: ";
                getline(cin, name);
                if (!isValidName(name)) {
                    cout << "~~~ Invalid name! Only letters and spaces are allowed. ~~~" << endl;
                    break;
                }
                cout << "Enter Initial Deposit: ";
                cin >> amount;
                if (amount <= 0) {
                    cout << "~~~ Initial deposit must be a positive integer! ~~~" << endl;
                    break;
                }
                b.createaccount(accnum, name, amount);
                break;
            case 2:
                cout << "Enter Account Number: ";
                cin >> accnum;
                cout << "Enter Amount to Deposit: ";
                cin >> amount;
                b.deposit(accnum, amount);
                break;
            case 3:
                cout << "Enter Account Number: ";
                cin >> accnum;
                cout << "Enter Amount to Withdraw: ";
                cin >> amount;
                b.withdraw(accnum, amount);
                break;
            case 4:
                cout << "Enter Account Number: ";
                cin >> accnum;
                b.displayAccount(accnum);
                break;
            case 5:
                cout << "Enter Account Number: ";
                cin >> accnum;
                b.deleteAccount(accnum);
                break;
            case 6:
                b.displayAllAccounts();
                break;
            case 7:
                cout << "Enter Account Number: ";
                cin >> accnum;
                b.searchAccount(accnum);
                break;
            case 8:
                cout << "Enter Account Number: ";
                cin >> accnum;
                cout << "Enter Interest Rate: ";
                cin >> interestRate;
                b.addInterest(accnum, interestRate);
                break;
            case 9:
                cout << "Enter Account Number: ";
                cin >> accnum;
                cout << "Enter Loan Amount: ";
                cin >> amount;
                cout << "Enter Interest Rate: ";
                cin >> interestRate;
                b.requestLoan(accnum, amount, interestRate);
                break;
            case 10:
                cout << "Enter Account Number: ";
                cin >> accnum;
                cout << "Enter Repayment Amount: ";
                cin >> amount;
                b.repayLoan(accnum, amount);
                break;
            case 11:
                cout << "Enter Account Number: ";
                cin >> accnum;
                b.displayLoanInfo(accnum);
                break;
            case 12:
                cout << "Exiting the system..." << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}