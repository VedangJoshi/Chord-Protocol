//=====================================================
// Author      : Vedang Joshi
// Course      : CMPE 252A
// Description : Code to emulate Chord protoocol
//=====================================================

#include "Chord.h"

int main() {
	Node *n0 = new Node(13);
	Node *n1 = new Node(44);
	Node *n2 = new Node(18);

	n0->join(NULL);
	cout << "\n n0 joins the Chord network\n";

	n0->insert(3);
	n0->insert(4);
	n0->fingertable->printFingerTable(n0->predecessor->id);
	cout << "\n\n";

	n1->join(n0);
	cout << "\n n1 joins the Chord network\n";

	n1->insert(9);
	n1->insert(14);
	n1->insert(15);
	n1->insert(29);
	n0->insert(50);
	n0->fingertable->printFingerTable(n0->predecessor->id);
	n1->fingertable->printFingerTable(n1->predecessor->id);
	cout << "\n\n";

	n2->join(n0);
	cout << "\n n2 joins the Chord network\n";
//	n0->insert(16);
//	n1->insert(18);
//	n2->insert(44);
//	n1->insert(35);

	n0->fingertable->printFingerTable(n0->predecessor->id);
	n1->fingertable->printFingerTable(n1->predecessor->id);
	n2->fingertable->printFingerTable(n2->predecessor->id);
	cout << "\n\n";

	return 0;
}


