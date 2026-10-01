#include "Blockchain.h"

void display() {
	cout << "----------  Menu  ----------\n";
	cout << "1. Register account \n";
	cout << "2. Create transaction(In the data should be 2 users at least)\n";
	cout << "3. Mine of block \n";
	cout << "4. Print all data of users \n";
	cout << "5. Print current blockchain status \n";
	cout << "6. Check the validality of blockchain \n";
	cout << "            0.EXIT       \n";
	cout << "-----------------------------\n\n";
	cout << " Enter the number\n\n";
}




int main() {
	
	BlockChain blockchain(4);
	int choiceOfUser;
	while (true) {
		display();
		if (!(cin >> choiceOfUser)) {
			cout << "Wrong input! Input a number from 0 to 6\n";
			cin.clear();										   // This two lines help to clear buffer from constant output of wrong input phrase
			cin.ignore(numeric_limits<streamsize>::max(), '\n');   // Эти две строчки отвечаю за очистку буфера, чтобы при неверном вводе строка не выводила постоянно "Неверный вывод"
			continue;
		}
		if (choiceOfUser == 0) {
			cout << "Exiting from the program, goodbye!\n";
		}
		try {
			switch (choiceOfUser)
			{
				case 1: {
					string name;
					double balance;  //In the future, I will create a function to top-up the balance of the account, В будущем, создам функцию для пополнения баланса
					cout << "Enter the name of the account (should be longer than 3 letters): ";
					cin >> name;
					cout << "Enter the balance of the account (should be positive): ";
					cin >> balance;
					Account user(name, balance);
					addUser(user);
					break;
				}
				case 2: {
					if (dataUsers.size() < 2) {
						throw exception("The database consists either one or none users! Try to add at least 2 to create a transaction!\n");
					}
					string send, receive;
					double amount;
					cout << "Enter the name of the sender account (should be longer than 3 letters): ";
					cin >> send;
					cout << "Enter the name of the receiver account (should be longer than 3 letters): ";
					cin >> receive;
					cout << "Enter the amount of money you'd like to transfer (should be positive): ";
					cin >> amount;
					Transaction bill(send, receive, amount);
					bill.transferMoney();
					break;
				}
				case 3: {
					cout << "Computing hash.....\n";
					blockchain.createBlock();
					break;
				}
				case 4: {
					printDataUsers();
					break;
				}
				case 5: {
					blockchain.printBlockChain();
					break;
				}
				case 6: {
					if (dataBaseForTransactions.size() == 0) {
						throw exception("There weren't any transactions yet! Try to create a transaction, after that, try this method again");
					}
					cout << "Checking the validality of blockchain....\n";
					cout << blockchain.isHashValid() ? "Yes, it is valid, nothing wrong\n" : "Something feels odd\n";
					break;
				}
				default:
					cout << "Wrong choice, buddy) Try a number in range from 0 to 6!\n";
					break;
			}
		}
		catch (const exception& e) {
			cout << e.what() << '\n';
		}
	}
	return 0;
}