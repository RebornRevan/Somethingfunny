#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <ctime>
#include <openssl/sha.h>
#include <iomanip>

using namespace std;

string sha256(const string& str) {
	// Creating array on 32 bytes, массив на 32 байта
	unsigned char hash[SHA256_192_DIGEST_LENGTH];

	// Creating hash, создание хэша
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(hash, &sha256);

	//Converting to 16th NumSystem, перевод в шестнадцатиричную систему
	stringstream str_hex;
	for (int i = 0; i < SHA256_192_DIGEST_LENGTH; i++) {
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
		uint64_t nonce;		//number for PoW, число для PoW(не знаю как это будет на русском)


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