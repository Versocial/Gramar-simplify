#include "Gramar.h"

int main() {
	ifstream input("../res/Gramar.txt");
	if (!input.is_open()) { cout << "open file error.\n"; };
	Gramar G;
	G.initByInput(input, cout);
}