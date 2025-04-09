#include <iostream>
 #include <unordered_map> // For efficient farmer lookups
 #include <stack> // For retrieving recent transactions (account statements)
 #include <list> // For storing all deposit transactions
 #include <ctime> // For timestamps
 #include <iomanip> // For formatting output
 #include <limits> // For clearing input buffer


 using namespace std;


 // --- Data Structures ---


 // Structure to hold details of a single transaction
 struct Transaction {
  int farmerId;
  double amount;
  time_t timestamp;
  string type; // "Deposit" or "Withdrawal"


  // Constructor to initialize a Transaction object
  Transaction(int id, double amt, time_t time, string t) : farmerId(id), amount(amt), timestamp(time), type(t) {}


  // Method to format the timestamp into a readable string
  string getFormattedTimestamp() const {
  tm *localTime = localtime(&timestamp); // Convert timestamp to local time
  char buffer[80];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime); // Format the time
  return string(buffer);
  }
 };


 // Structure to hold a farmer's account information
 struct FarmerAccount {
  double balance;
  stack<Transaction> transactions; // Stack to store transaction history


  // Constructor to initialize a FarmerAccount object
  FarmerAccount() : balance(0) {}
 };


 // --- SACCO Class ---


 // Class to manage the SACCO system
 class SACCO {
 private:
  unordered_map<int, FarmerAccount> farmerAccounts; // Hash map for farmer accounts (key: farmerId)
  list<Transaction> allDeposits; // Linked list to store all deposit transactions


 public:
  // Method to deposit money into a farmer's account
  void deposit(int farmerId, double amount) {
  farmerAccounts[farmerId].balance += amount; // Update balance
  time_t now = time(0); // Get current timestamp
  Transaction newDeposit(farmerId, amount, now, "Deposit"); // Create a new Transaction object
  farmerAccounts[farmerId].transactions.push(newDeposit); // Add to farmer's transaction history
  allDeposits.push_back(newDeposit); // Add to the list of all deposits
  cout << "Deposit of " << amount << " successful for Farmer " << farmerId << endl;
  }


  // Method to withdraw money from a farmer's account
  bool withdraw(int farmerId, double amount) {
  // Check if the farmer exists
  if (farmerAccounts.find(farmerId) == farmerAccounts.end()) {
  cout << "Farmer " << farmerId << " not found." << endl;
  return false;
  }


  // Check if the farmer has sufficient balance
  if (farmerAccounts[farmerId].balance >= amount) {
  farmerAccounts[farmerId].balance -= amount; // Update balance
  time_t now = time(0); // Get current timestamp
  Transaction newWithdrawal(farmerId, amount, now, "Withdrawal"); // Create a new Transaction object
  farmerAccounts[farmerId].transactions.push(newWithdrawal); // Add to farmer's transaction history
  cout << "Withdrawal of " << amount << " successful for Farmer " << farmerId << endl;
  return true;
  } else {
  cout << "Insufficient balance for Farmer " << farmerId << endl;
  return false;
  }
  }


  // Method to retrieve a farmer's account statement
  void getAccountStatement(int farmerId, int numTransactions) {
  // Check if the farmer exists
  if (farmerAccounts.find(farmerId) == farmerAccounts.end()) {
  cout << "Farmer " << farmerId << " not found." << endl;
  return;
  }


  cout << "Account Statement for Farmer " << farmerId << ":" << endl;
  stack<Transaction> tempStack = farmerAccounts[farmerId].transactions; // Create a temporary stack (copy)
  int count = 0;
  // Retrieve and display the last 'numTransactions' transactions
  while (!tempStack.empty() && count < numTransactions) {
  Transaction transaction = tempStack.top(); // Get the most recent transaction
  cout << transaction.type << ": " << transaction.amount
  << " on " << transaction.getFormattedTimestamp() << endl; // Display transaction details
  tempStack.pop(); // Remove the transaction from the temporary stack
  count++;
  }
  }


  // Method to check a farmer's account balance
  void checkBalance(int farmerId) {
  // Check if the farmer exists
  if (farmerAccounts.find(farmerId) == farmerAccounts.end()) {
  cout << "Farmer " << farmerId << " not found." << endl;
  return;
  }
  cout << "Current Balance for Farmer " << farmerId << ": "
  << farmerAccounts[farmerId].balance << endl;
  }
 };


 // --- Main Function (User Interface) ---


 int main() {
  SACCO sacco; // Create a SACCO object
  int choice;
  int farmerId;
  double amount;
  int numTransactions;


  // Main loop to display the menu and get user input
  do {
  cout << "\n--- SACCO Management System ---" << endl;
  cout << "1. Deposit Money" << endl;
  cout << "2. Withdraw Money" << endl;
  cout << "3. Check Balance" << endl;
  cout << "4. Get Account Statement" << endl;
  cout << "0. Exit" << endl;
  cout << "Enter your choice: ";
  cin >> choice;


  // Input validation: Check if the input is an integer
  if (cin.fail()) {
  cout << "Invalid input. Please enter a number." << endl;
  cin.clear(); // Clear error flags
  cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the line
  choice = -1; // Set choice to an invalid value to repeat the loop
  continue;
  }


  switch (choice) {
  case 1: // Deposit
  cout << "Enter Farmer ID: ";
  cin >> farmerId;
  cout << "Enter Amount to Deposit: ";
  cin >> amount;
  sacco.deposit(farmerId, amount);
  break;
  case 2: // Withdraw
  cout << "Enter Farmer ID: ";
  cin >> farmerId;
  cout << "Enter Amount to Withdraw: ";
  cin >> amount;
  sacco.withdraw(farmerId, amount);
  break;
  case 3: // Check Balance
  cout << "Enter Farmer ID: ";
  cin >> farmerId;
  sacco.checkBalance(farmerId);
  break;
  case 4: // Get Account Statement
  cout << "Enter Farmer ID: ";
  cin >> farmerId;
  cout << "Enter Number of Transactions to Display: ";
  cin >> numTransactions;
  sacco.getAccountStatement(farmerId, numTransactions);
  break;
  case 0: // Exit
  cout << "Exiting..." << endl;
  break;
  default: // Invalid choice
  cout << "Invalid choice. Please try again." << endl;
  }
  } while (choice != 0); // Continue until the user chooses to exit


  return 0;
 }
