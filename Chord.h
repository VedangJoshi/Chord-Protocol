
//========================================================
// Author      : Vedang Joshi
// Course      : CMPE 252-A
// Description : Code to simulate the Chord DHT protoocol
//========================================================

#include <iostream>
#include <vector>

using namespace std;

class Node;

class FingerTable {
public:
	vector<Node*> fingerTable;
	Node* local_node;
	int nodeId;

	FingerTable(int id, Node* node) {
		this->nodeId = id;
		this->local_node = node;
	}

	~FingerTable() {
		this->nodeId = -99;
		this->fingerTable.clear();
	}

	void printFingerTable(int);
};

class Node {

public:
	uint64_t id;
	Node* predecessor;
	std::vector<int> local_keys;
	FingerTable *fingertable;

	Node(int id) {
		this->id = (int)id;
		this->predecessor = NULL;
		this->fingertable = new FingerTable(this->id, this);
	}

	~Node() {
		this->id = -99;
		(this->local_keys).clear();
	}

	// Move keys to the newly added node
	void moveKeys(Node* succ, int new_node_id) {
		vector<int> v;
		for (int i = 0; i < succ->local_keys.size(); i++) {
			if(succ->local_keys[i] <= new_node_id &&
					succ->local_keys[i] > succ->predecessor->id) {
				insert_key(succ->local_keys[i]);
			} else {
				v.push_back(succ->local_keys[i]);
			}
		}

		succ->local_keys.clear();
		succ->local_keys = v;
	}

	// Node join operation
	void join(Node* node) {
 		if (node == NULL) {  // First node to join
			for (int i = 0; i < 8; i++) {
				fingertable->fingerTable.push_back(this);
			}
			predecessor = this;
		} else {
			for (int i = 0; i < 8; i++) {
				fingertable->fingerTable.push_back(this);
			}

			// Find successor to attach to
			Node* succ = node->find_successor(id);

			// Update node's successor to point to the successor
			fingertable->fingerTable[0] = succ;

			// Update predecessor's successor to self
			succ->predecessor->fingertable->fingerTable[0] = this;

			// Update predecessor to successor's old predecessor
			predecessor = succ->predecessor;

			// move keys on the successor before changing predecessor
			moveKeys(succ, id);

			// Update successor's predecssor to self
			succ->predecessor = this;
		}
	}

	// Find Successor
	Node* find_successor(int id) {
		if (this->id == id) {
			return this;
		} else if (this->id > id) {
			return this;
		} else {
			return fingertable->fingerTable[0]->
					find_successor(fingertable->fingerTable[0]->id);
		}
	}

	// TODO
	int find(int key);

	void insert(int key) {
		if (key < 0) {
			cerr << "\n *** Error Key is less than 0 *** \n";
			return;
		}

		Node* succ = this->fingertable->fingerTable[0];

		if (succ->id < id && id <= key) {
			succ->insert_key(key);
		} else if (predecessor->id > id && key > predecessor->id) {
			insert_key(key);
		} else {
			while(succ->id < key) {
				succ = succ->fingertable->fingerTable[0];
			}

			succ->insert_key(key);
		}
	}

	// Insert a key on this node
	void insert_key(int key) {
		if (!key) {
			cout << "No key provided to insert!" << endl;
		}

		local_keys.push_back(key);
	}

	// Search a key locally
	int local_lookup(int key) {
		int if_found = 0;

		for (int i = 0; i < local_keys.size(); i++)
			if (local_keys[i] == key)
				if_found = 1;

		return if_found;
	}
};

// Print Finger Table
void FingerTable::printFingerTable(int pred) {
	cout << "\n**** Node ID : " << this->nodeId  << " ****";
	cout << "\nFingerTable\n";

	for (int i = 0; i < fingerTable.size(); i++) {
		cout << i+1 << " : " << fingerTable[i]->fingertable->nodeId << "\n";
	}

	cout << "\nKeys : ";
	for (int i = 0; i < local_node->local_keys.size(); i++) {
		cout << local_node->local_keys[i] << "  ";
	}

	cout << "\nSucc : " << this->fingerTable[0]->id;
	cout << "\nPred : " << pred;

	cout << "\n**********************\n";
}

