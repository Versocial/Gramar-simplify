#pragma once
#include<unordered_map>
#include<vector>
#include<set>
#include <iostream>
#include<fstream>
#include<string>

using namespace::std;

typedef string symbol;
typedef vector<symbol> production;

class Gramar {
private:
	set<symbol> N;
	set<symbol>T;
	symbol start;
	symbol epsilon;
	unordered_map<symbol, set<production>> P;

	set<production> manageLine(const string& line, string& n);//

	static void printSet(const set<symbol>& set, ostream& out);
	static bool belongsN(const symbol& label);
	static bool belongsT(const symbol& label);

	static symbol getLabel(istream& input,char& c);
	static bool canBeN(const symbol& label);
	static bool canBeT(const symbol& label);
public:
	Gramar();
	~Gramar();
	int initByInput(istream& input, ostream& out);
	void print(ostream& out);
	void removeEpsilon();
	void removeSingle();
	void removeUseless();
};