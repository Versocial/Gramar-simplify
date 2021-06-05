#include "Gramar.h"

int main() {
	ifstream input("../res/Gramar.txt");
	if (!input.is_open()) { cout << "open file error.\n"; };
	Gramar G;
	G.initByInput(input, cout);
	G.eliminateEpsilon();
	cout << ">>The Gramar without epsilon : \n";
	G.print(cout);
	G.eliminateSingle();
	cout << ">>The Grammar without single : \n";
	G.print(cout);
}