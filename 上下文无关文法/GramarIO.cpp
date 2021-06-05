#include "Gramar.h"
#define _END_SIG "@"
#define _EPSILON_IN '#'

set<production> Gramar::manageLine(const string& line, string& n)
{
	set<production>ans;
	n = "";
	list<string > production;
	string symbol = "";
	int flag = 1;
	for (char c : line) {
		if (c == ' ')continue;
		else if(isalpha(c)) {if(symbol.size()>0) production.insert(production.end(),symbol); symbol = c; }
		else if (isdigit(c) )symbol += c;
		else if (c == _EPSILON_IN && symbol.size() == 0 && production.size() == 0) { ; production.insert(production.end(), epsilon); ans.insert(production); production.clear();  }
		else if (c == '=' && flag == 1) { n = symbol; symbol = ""; flag = 0; }
		else if (flag == 0 && c == '|' &&( production.size() > 0||symbol.size()>0)) {if(symbol.size()>0) production.insert(production.end(), symbol); ans.insert(production); production.clear(); symbol = ""; }
		else { n = ""; return ans; }
	}
	if(symbol.size()>0)production.insert(production.end(), symbol);
	if(production.size()>0)ans.insert(production); 
	return ans;
}

void Gramar::printSet(const set<symbol>& set, ostream& out)
{
	out << "{ ";
	int num = set.size();
	for (string str : set) {
		num--;
		out << str;
		if (num != 0)out << " , ";		
	} out << " }";
}

bool Gramar::belongsN(const symbol& label)
{
	return 'A'<=label[0]&&label[0]<='Z';
}

bool Gramar::belongsT(const symbol& label)
{
	return 'a'<=label[0]&&label[0]<='z';
}

symbol Gramar::getLabel(istream& input,char&c)
{
	string exit = _END_SIG;
	string ans="";
	//char c;
	char flag=2;
	for (input >> c;; ans += c, input >> c) {
		if (c == exit[0]) { input >> ans; if (c + ans == exit)return exit; else return ""; }
		if (c == ' ')continue;
		if ((('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) && ans.size() == 0){
			flag = ('A' <= c && c <= 'Z'); continue;
	}
		if ('0' <= c && c <= '9' && ans.size() > 0)continue;
		if ((c == '|' || c == '\n') && flag == 1)break;
		if (c == '=' && flag == 0)break;
			return "";
	}
	return ans;
}

bool Gramar::canBeN(const symbol& label)
{
	if (label[0] > 'Z' || label[0] < 'A')return false;
	for (int i = 1; i <label.size();i++) {
		if (label[i] > '9' || label[i] < '0')return false;
	}
	return true;
}

bool Gramar::canBeT(const symbol& label)
{
	if (label[0] > 'z' || label[0] < 'a')return false;
	for (int i = 1; i < label.size(); i++) {
		if (label[i] > '9' || label[i] < '0')return false;
	}
	return true;
}

Gramar::Gramar()
{
	epsilon = "ε";
}

Gramar::~Gramar()
{
}

int Gramar::initByInput(istream& input, ostream& cout)
{
	N.clear(); T.clear(); P.clear(); start = "#";

	cout << ">>The G=( N , T , P , S), using \"" << epsilon << "\" to presents 'epsilon' " << endl;
	cout << ">>Please enter elements of N , which can be a capital letter following a subscript number, eg. \"A\", or \"A32\" will be OK.\n";
	cout << ">>You can mark the end of N by type in \"" << _END_SIG << "\".\n";
	for (string n = ""; n != _END_SIG&&!input.eof(); input >> n) {
		if (n == "")continue;
		if (n == _END_SIG)break;
		if (canBeN(n)) {
			if (N.count(n)) { cout << " :: Already a \"" << n << "\" exists in set N.\n"; continue; }
			else N.insert(n);
		}
		else cout << " :: \"" << n << "\" Not match the Format a capital letter or a capital letter following a number !!.\n";
	}

	cout << ">>The N is "; printSet(N, cout); cout << endl;

	cout << ">>Please enter elements of T , which can be a lowercase letter following a subscript number, eg. \"a\", or \"a32\" will be OK.\n";
	cout << ">>You can mark the end of T by type in \"" << _END_SIG << "\".\n";
	for (string t=""; t != _END_SIG; input >> t) {
		if (t == "")continue;
		if (t == _END_SIG)break;
		if (canBeT(t)) {
			if (T.count(t)) { cout << " :: Already a \"" << t << "\" exists in set T.\n"; continue; }
			else if (t == epsilon) { cout << ":: \"" << t << "\" presents 'epsilon' so use another .\n"; continue; }
			else T.insert(t);
		}
		else cout<< " :: \""<<t<<"\" Not match the Format a capital letter or a lowercase letter following a number !!.\n";
	}

	cout << ">>The T is "; printSet(T, cout); cout << endl;

	cout << ">>Please enter the start symbol which should be a element of N.\n";
	input >> start;
	while (!N.count(start)) {
		cout << " :: symbol \"" << start << "\" is not a element of set N. Please give another symbol.\n";
		input >> start;
	}cout << ">>The start is \"" << start<<"\".\n";

	//.....
	cout << ">>Please enter the P by format like \"A=A1A2A3abc , and use '"<<_EPSILON_IN<<"' to presents 'epsilon' here \"\n";
	string line="";
	string n="";
	while (1) {
		getline(input, line);
		if (line=="")continue;
		if (line == _END_SIG)break;
		set<production> productions = manageLine(line, n);

		if (n == "") {
			cout << " :: The input is out of format.\n"; continue;
		}
		else if (!N.count(n))cout << " :: The \"" << n << "\" is not element of set N.\n";
		else {
			for (set<production>::iterator it = productions.begin(); it != productions.end();) {
				bool flag = true;
				for (string symbol : *it) {
					if (!T.count(symbol)&&!N.count(symbol)&&epsilon!=symbol) {
						cout << " :: The \"" << symbol << "\" is not element of set T.\n";
						flag = false;
						break;
					}
				}
				if (!flag) { it = productions.erase(it); flag = true; }
				else it++;
			}
			if (productions.size() != 0)P[n]=productions;
		}
	}
	//.....
	
	cout << ">>So the input Gramar is :\n";
	print(cout);

	return 0;
}

void Gramar::print(ostream& out)
{
	out << ">> G =( "; printSet(N, out); out << " , "; printSet(T, out); cout << " , P ," << start<<" )" << endl<<"> P : \n";
	for (unordered_map<symbol, set<production>>::iterator it = P.begin(); it != P.end();it++) {
		out<<it->first<<" = ";
		int size = it->second.size();
		for (production p : it->second) {
			for (symbol s : p) {
				cout << s;
			}
			size--;
			if (size > 0)cout << "|";
			else cout << endl;			
		}
	}
	cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
}
/*
* 
* S→a|bA|B|ccD
A→abB|ε
B→aA
C→ddC
D→ddd

S A B C D @
a b c d @
S
S=a|bA|B|ccD|A
A=abB|#
B=aA
C=ddC
D=ddd
@
*/
