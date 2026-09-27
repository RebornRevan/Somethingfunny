#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <random>
#include <string>
#include <ctime>
#include <iomanip>
#include <unordered_map>
#include <openssl/sha.h>


using namespace std;


//Library for creating secret keys(consists of 6 key words), Сборник для создания секретного ключа(состоит из 6 слов)
const vector<string> library = { "Crypto", "Newcastle", "Axle", "Conduit", "Bangalore", "Fuse", "Revenant", "MadMaggie", "Ballistic", "Wraith", "Octaine", "Pathfinder", "Horizon",
"Ash", "Alter", "Bloodhound", "Seer", "Valkyrie", "Sparrow", "Vantage", "Gibraltar", "Lifeline", "Mirage", "Loba", "Wattson", "Caustic", "Rampart", "Catalyst" };



string sha256(const string& str) {
	// Creating array on 32 bytes for hash, массив на 32 байта для хэша
	unsigned char hash[SHA256_DIGEST_LENGTH];

	// Creating hash, создание хэша
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(hash, &sha256);

	//Converting to 16th NumSystem, перевод в шестнадцатеричную систему
	stringstream str_hex;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		str_hex << hex << setw(2) << setfill('0') << (int)hash[i];
	}

	return str_hex.str();
}

class Block {
	public:
		int index;		//Index of block, индекс блока
		string prevHash;	//Hash of previous block, хэш прошлого блока
		string data;		//Data of block, данные блока
		string hash;		//Hash of current block, хэш с блоком, с которым мы работаем
		time_t cTime;		//Time of adding block, время, когда добавили блок
		unsigned long long nonce = 0;		//number for PoW, число для PoW(не знаю как это будет на русском)


		// Constructor, конструктор
		Block(int i, string previousHash, string blockData)
			: index(i), prevHash(previousHash), data(blockData) {
			cTime = time(nullptr);
			hash = calculateHash();
		}

		// Realization of Proof of Work(PoW), реализация PoW
		void mineBlock(int length) {
			string target(length, '0');
			
			// Finding hash
			while (hash.substr(0, length) != target) {
				nonce++;
				hash = calculateHash();
			}
			cout << "Block mined, hash = " << hash << endl;
		}

		//Calculating hash using data, index, nonce, previous hash, Считаем хэш исходя от времени, noncа, прошлого хэша, индекса, данных
		string calculateHash() const {
			stringstream result;
			result << index << data << cTime << prevHash << nonce;
			return sha256(result.str());
		}
};


class BlockChain {
	private:
		vector<Block> blockchain;
		int length;		//Count of '0' in the beginning of hash, кол-во нулей в начале хеша

	public:
		BlockChain(int len) : length(len) {
			blockchain.push_back(createGenBlock());
		}

		//Generating genesis block, создание генезис(первого) блока
		Block createGenBlock() {
			return Block(0, "0", "Genesis Block");
		}

		//Get last block from vector, геттер на последний блок
		Block getLastBlock() const {
			return blockchain.back();
		}

		void createBlock(string data) {
			Block newBlock(blockchain.size(), getLastBlock().hash, data);
			newBlock.mineBlock(length);
			blockchain.push_back(newBlock);
		}

		// Checks if hash was calculated right and if it is the same hash as hash of previous one, Проверяет правильно ли посчитан хэш и совпадает ли он с хэшом прошлого блока
		bool isHashValid() const {
			for (size_t i = 1; i < blockchain.size(); i++) {
				Block currentBlock = blockchain[i];
				Block previousBlock = blockchain[i - 1];

				if (currentBlock.calculateHash() != currentBlock.hash) {
					cout << "Error - Incorrect hash of block №" << i << endl;
					return false;
				}
				if (currentBlock.prevHash != previousBlock.hash) {
					cout << "Error - Incorrect previous hash of block №" << i << endl;
					return false;
				}
			}
			return true;
		}


		void printBlockChain() const {
			cout << "-----   Information about BlockChain   -----\n";
			for (auto& block : blockchain) {
				cout << "-----   Block   -----\n";
				cout << "Index: " << block.index << endl;
				cout << "Data: " << block.data << endl;
				cout << "Previous Hash: " << block.prevHash << endl;
				cout << "Hash: " << block.hash << endl;
				cout << "Nonce: " << block.nonce << endl << endl;
			}
		}
};



class Account {
	private:
		string name;		//Name of account, имя аккаунта
		string password;		//
		double balance;		//Balance of account, баланс аккаунта
		vector<string> secretKeys;	//Vector of secret keys, вектор секретных ключей
	public:
		Account(string name, double bal) : name(name), balance(bal) {
			secretKeys = generateSecretKeys();
		}

		// Getters for name and balance, геттеры для имени и баланса
		string getName() const { return name; }
		double getBalance() const { return balance; }

		// Simple adding money to the account(in the future will be divided by whom: account user itself or by other user),
		// Добавляет деньги на счет(потом будет разделено на пополнение счета и на перевод с другого счета)
		void deposit(double amount) { balance += amount; }

		// Withdraw money from account(same as deposit plans), Вывод денег с аккаунта(планы такие же, как и у deposit)
		void withdraw(double amount) {
			if (amount > balance) {
				throw exception("Error - Insufficient funds for withdrawal.\n");
			}
			else if (amount < 0) {
				throw exception("Error - Withdrawal amount cannot be negative.\n");
			}
			else {
				balance -= amount;
			}
		}

		//Generate secret keys for an account using std::randomdevice, Генерация ключей для аккаунта через функцию std::randomdevice
		vector<string> generateSecretKeys() const {

			//Generating random numbers by requesting data from Windows, Генерация рандомных чисел через запросы в Windows	Idea of this method: https://www.youtube.com/watch?v=1NXPf6Loshc 
			random_device rd;
			mt19937 rand(rd());
			uniform_int_distribution<int> distance(0, library.size() - 1);

			vector<string> dict;

			while (dict.size() < 6) {
				string word = library[distance(rand)];

				if (find(dict.begin(), dict.end(), word) == dict.end()) {
					dict.push_back(word);
				}
			}

			return dict;
		}

};

//All information about users, Вся информация о пользователях
unordered_map<string, Account> dataUsers;


//Function to add user(also checks if username is not already in the dataUsers(in the future need to add checking the secret key combination),
//Функция для добавления пользователя в dataUsers + проверка на наличие имени пользователя в dataUsers(в будщем надо добавить проверку на комбинацию ключей)
void addUser(const Account& user) {
	if (dataUsers.count(user.getName()) == 0) {
		dataUsers.insert({ user.getName(), user });
		cout << "You successfully registered your account!\n";
	}
	else {
		cout << "You can't use this username - choose another one\n";
	}
}

class Transaction {
	private:
		string sender;		//Name of sender of transaction, имя отправителя транзакции
		string receiver;	//Name of receiver of transaction, имя получателя транзакции
		double amount;		//Amount of transaction, сумма транзакции
		string message;		//Information about transaction, информация о переводе
	public:
		Transaction(string s, string r, double a) : sender(s), receiver(r), amount(a) {}

		string getSender() const { return sender; }

		string getReceiver() const { return receiver; }

		void transferMoney() {
			if (sender == receiver) {
				throw invalid_argument("The receiver can't be you!\n");
			}
			if (dataUsers.count(sender) == 0) {
				throw exception("The sender doesn't exist in the system\n");
			}
			if (dataUsers.count(receiver) == 0) {
				throw exception("The receiver doesn't exist in the system\n");
			}

			Account& from = dataUsers.at(sender);
			Account& to = dataUsers.at(receiver);

			from.withdraw(amount);
			to.deposit(amount);

			message = "The sender <" + sender + "> had sent " + to_string(amount) + "$ to the receiver<" + receiver + ">\n";

			dataBaseForTransactions.push_back(*this);
		}

		
};


vector<Transaction> dataBaseForTransactions;