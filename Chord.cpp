 //=====================================================
// Author      : Vedang Joshi
// Course      : CMPE 252A
// Description : Code to emulate the Chord protoocol
//=====================================================

#include "Chord.h"

int main() {
	Node* n0 = new Node(13);
	Node* n1 = new Node(44);
	Node* n2 = new Node(18);

	// n0 join
	n0->join(NULL);
	cout << "\n n0 joins the Chord network\n";

	n0->insert_key(3, 3);
	n0->insert_key(4, 4);
	n0->fingertable->printFingerTable(n0->predecessor->id);
	cout << "\n\n";

	// n1 join
	n1->join(n0);

	// stabilize
	n1->stabilize();
	n0->stabilize();
	cout << "\n n1 joins the Chord network\n";

	n1->insert_key(9, 9);
	n1->insert_key(14, 14);
	n1->insert_key(15, 15);
	n1->insert_key(29, 29);
	n0->insert_key(50, 50);
	n0->fingertable->printFingerTable(n0->predecessor->id);
	n1->fingertable->printFingerTable(n1->predecessor->id);
	cout << "\n\n";

	// n2 join
	n2->join(n0);

	// stabilize
	n0->stabilize();
	n1->stabilize();

	cout << "\n n2 joins the Chord network\n";
	n0->insert_key(16, 16);
	n1->insert_key(18, 18);
	n2->insert_key(44, 44);
	n1->insert_key(35, 35);
	n1->insert_key(38, 38);
	n0->insert_key(41, 41);

	n0->fingertable->printFingerTable(n0->predecessor->id);
	n1->fingertable->printFingerTable(n1->predecessor->id);
	n2->fingertable->printFingerTable(n2->predecessor->id);
	cout << "\n\n";

	// Random search for values on non-local nodes i.e nodes that may/may not contain the
	// keys being searched for, locally on them
	cout << n0->find_key(50) << endl;
	cout << n0->find_key(4) << endl;
	cout << n2->find_key(29) << endl;
	cout << n1->find_key(14) << endl;
	cout << n1->find_key(3) << endl;
	cout << n2->find_key(18) << endl;

	return 0;
}

