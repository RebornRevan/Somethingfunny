#include "Blockchain.h"

int main() {
	BlockChain blockchain(4);

	Account user1("Guest", 0.0);
	Account user2("Dumbass", 10.0);
	Account user3("Genius", 100.0);

	Account clon("Dumbass", 10.0);


	addUser(user1);
	addUser(user2);
	addUser(user3);
	//addUser(clon);

	blockchain.printBlockChain();

	printDataUsers();

	cout << "Blockchain Valid?\n" << (blockchain.isHashValid() ? "Yes" : "No") << endl;

	return 0;

}