#pragma once
#include<unordered_map>
#include<vector>
#include<set>
#include <iostream>
#include<string>

using namespace::std;
class Gramar {
private:
	set<string> N;
	set<string>T;
	string start;
	string epsilon;
	unordered_map<string, set<vector<string>>> P;

	set<vector<string>>& manageLine(const string& line, string& n);//

	static void printSet(const set<string>& set, ostream& out);
	static bool belongsN(const string& label);
	static bool belongsT(const string& label);

	static string getLabel(istream& input,char& c);
	static bool canBeN(const string& label);
	static bool canBeT(const string& label);
public:
	Gramar();
	~Gramar();
	int initByInput(istream& input, ostream& out);
	void print(ostream& out);
	void removeEpsilon();
	void removeSingle();
	void removeUseless();
};