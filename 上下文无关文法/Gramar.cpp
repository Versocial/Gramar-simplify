#include "Gramar.h"
#define _END_SIG "@"
#define _EPSILON_IN '#'

set<vector<string>>& Gramar::manageLine(const string& line, string& n)
{
	set<vector<string>>ans;
	n = "";
	vector<string > production;
	string temp = "";
	int flag = 1;
	int num = line.size();
	for (char c : line) {
		num--;
		if (c == ' ')continue;
		else if(isalpha(c)) {if(temp.size()>0) production.insert(production.end(),temp); temp = c; }
		else if (isdigit(c) )temp += c;
		else if (c == _EPSILON_IN && temp.size() == 0 && production.size() == 0) { temp = c; production.insert(production.end(), temp); ans.insert(production); production.clear(); temp = ""; }
		else if (c == '=' && flag == 1) { n = temp; temp = ""; flag = 0; continue; }
		else if (flag == 0 && (c == '|' || num == 0) && production.size() != 0) { ans.insert(production); production.clear(); }
		else { n = ""; return ans; }
	}
	return ans;
}

void Gramar::printSet(const set<string>& set, ostream& out)
{
	out << "{ ";
	int num = set.size();
	for (string str : set) {
		num--;
		out << str;
		if (num != 0)out << " , ";		
	} out << " }";
}

bool Gramar::belongsN(const string& label)
{
	return 'A'<=label[0]&&label[0]<='Z';
}

bool Gramar::belongsT(const string& label)
{
	return 'a'<=label[0]&&label[0]<='z';
}

string Gramar::getLabel(istream& input,char&c)
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

bool Gramar::canBeN(const string& label)
{
	if (label[0] > 'Z' || label[0] < 'A')return false;
	for (int i = 1; i <label.size();i++) {
		if (label[i] > '9' || label[i] < '0')return false;
	}
	return true;
}

bool Gramar::canBeT(const string& label)
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
	for (string n = ""; n != _END_SIG; input >> n) {
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
	string n;
	while (1) {
		getline(input, line);
		if (line=="")continue;
		if (line == _END_SIG)break;
		set<vector<string>> productions = manageLine(line, n);
		if (n == "") {
			cout << " :: The input is out of format.\n"; continue;
		}
		else if (!N.count(n))cout << " :: The \"" << n << "\" is not element of set N.\n";
		else {
			for (vector<string> production : productions) {
				bool flag = true;
				for (string symbol : production) {
					if (!T.count(symbol)) { cout << " :: The \""<<symbol<<"\" is not element of set T.\n"; flag = false; break; }
				}
				if (!flag) { productions.erase(production); }
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
	out << ">> G =( "; printSet(N, out); out << " , "; printSet(T, out); cout << " , P ," << start<<" )" << endl<<" P : \n";


}

