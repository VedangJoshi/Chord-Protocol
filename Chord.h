//========================================================
// Author      : Vedang Joshi
// Course      : CMPE 252-A
// Description : Code to simulate the Chord DHT protoocol
//========================================================

#include <iostream>
#include <vector>
#include <map>
#include <math.h>

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
	std::map<int, int> local_keys;
	FingerTable *fingertable;

	Node(int id) {
		this->id = (int) id;
		this->predecessor = NULL;
		this->fingertable = new FingerTable(this->id, this);
	}

	~Node() {
		this->id = INT64_MIN;
		(this->local_keys).clear();
	}

	// Move keys (if any) to the newly added node
	void moveKeys(Node* succ, int new_node_id) {
		map<int, int> m;
		map<int, int>::iterator iter;

		for (map<int, int>::iterator iter = succ->local_keys.begin();
				iter != succ->local_keys.end(); iter++) {
			if (iter->first <= new_node_id
					&& iter->first > succ->predecessor->id) {
				insert_key_local(iter->first, iter->second);
			} else {
				m.insert(pair<int, int>(iter->first, iter->second));
			}
		}

		succ->local_keys.clear();
		succ->local_keys = m;
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

			// update finger table
			// fingerTable[0] is always the successor
			createFingerTable();
		}
	}

	// creates the finger table
	void createFingerTable() {
		for (int i = 1; i < fingertable->fingerTable.size(); i++) {
			Node* ptr = this;
			int flag = 0;

			for (int j = 0; j < pow(2, i); j++) {
				ptr = ptr->fingertable->fingerTable[0];

				if (ptr == this) {
					flag = 1;
					break;
				}
			}

			if (flag == 0) {
				fingertable->fingerTable[i] = ptr;
			}
		}
	}

	// stabilize the finger tables
	void stabilize() {
		for (int i = 1; i < fingertable->fingerTable.size(); i++) {
			fingertable->fingerTable[i]->createFingerTable();
		}
	}

	// Find Successor
	Node* find_successor(int id) {
		if (this->id == id) {
			return this;
		} else if (this->id > id) {
			return this;
		} else {
			return fingertable->fingerTable[0]->find_successor(
					fingertable->fingerTable[0]->id);
		}
	}

	// Search a key value pair
	string find_key(int key) {
		int node_id = 0;
		string ret_val;

		cout << "\n Searching Key " << key << " on node " << id << endl;
		node_id = local_key_lookup(key);
		if (node_id >= 0) {
			ret_val = " Found value - " + to_string(node_id) + " on Node - "
					+ to_string(id) + "\n";
		} else {
			for (int i = 0; i < fingertable->fingerTable.size(); i++) {
				node_id = fingertable->fingerTable[i]->local_key_lookup(key);
				if (node_id >= 0) {
					ret_val =  " Found value - " + to_string(node_id) + " on Node - "
							+ to_string(fingertable->fingerTable[i]->id) + "\n";
					break;
				}
			}
		}

		return ret_val;
	}

	// Insert key
	void insert_key(int key, int value) {
		if (key < 0) {
			cerr << "\n *** Error Key is less than 0 *** \n";
			return;
		}

		Node* succ = this->fingertable->fingerTable[0];

		if (succ->id < id && id <= key) {
			succ->insert_key_local(key, value);
		} else if (predecessor->id > id && key > predecessor->id) {
			insert_key_local(key, value);
		} else {
			while (succ->id < key) {
				succ = succ->fingertable->fingerTable[0];
			}
			succ->insert_key_local(key, value);
		}
	}

	// Insert a key on this node
	void insert_key_local(int key, int value) {
		if (!key) {
			cout << "No key provided to insert_key!" << endl;
		}

		local_keys.insert(pair<int, int>(key, value));
	}

	// Search a key locally
	int local_key_lookup(int key) {
		cout << " Node " << this->id << " searched" << endl;
		int node = -1;

		for (int i = 0; i < local_keys.size(); i++)
			if (local_keys.find(key)->first == key)
				node =  local_keys.find(key)->second;

		return node;
	}
};

// Print Finger Table
void FingerTable::printFingerTable(int pred) {
	cout << "\n**** Node ID : " << this->nodeId << " ****";
	cout << "\nFingerTable\n";

	for (int i = 0; i < fingerTable.size(); i++) {
		if (i == 0 || (nodeId != fingerTable[i]->fingertable->nodeId)) {
			cout << i + 1 << " : " << fingerTable[i]->fingertable->nodeId
					<< "\n";
		}
	}

	cout << "\nKeys : ";
	for (map<int, int>::iterator iter = local_node->local_keys.begin();
			iter != local_node->local_keys.end(); iter++) {
		cout << iter->second << "  ";
	}

	cout << "\n**********************\n";
}

