#include "Blockchain.h"

int main() {
	BlockChain blockchain(4);

	blockchain.createBlock("Transaction: I transfered 10 USD to Bob");
	blockchain.createBlock("Withdrawl: Bob withdraw 5 USD");

	blockchain.printBlockChain();

	cout << "Blockchain Valid?\n" << (blockchain.isHashValid() ? "Yes" : "No") << endl;

	return 0;

}