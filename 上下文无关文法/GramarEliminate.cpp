#include "Gramar.h"
#define _START_SYMBOL "S'"

bool Gramar::eliminateEpsilonInSet(set<production>& allProductions, const set<symbol>& epsilonReachableSymbols, symbol epsilon, symbol LeftSymbol)
{
	bool canReturn = true;
	bool startReachEpsilonFlag = false;
	set<production>addition;
	for (set<production>::iterator pIt = allProductions.begin(); pIt != allProductions.end();) {
		production& p = *((production*)(void*)&(*pIt));
		bool epsilonReachableFlag = true;// the production is epsilon reachable
		if (p.size() == 1 && p.front() == epsilon) {//epsilon 
			pIt = allProductions.erase(pIt);
			startReachEpsilonFlag = true;
			continue;
		}
		int num = 0;
		for (production::iterator s = p.begin(); s != p.end(); s++) {
			if (epsilonReachableSymbols.count(*s) && (p.size() > 1)) {
				symbol now = *s;
				s = p.erase(s);
				production temp(p);
				if (!(p.size() == 1 && p.front() == LeftSymbol)) { addition.insert(temp); }
				s = p.insert(s, now);
				num++;
			}
			else if (!epsilonReachableSymbols.count(*s)) epsilonReachableFlag = false;
		}
		if (num >= 2)canReturn = false;
		if (epsilonReachableFlag)startReachEpsilonFlag = true;
		pIt++;
	}
	if (!canReturn) {
		startReachEpsilonFlag |= eliminateEpsilonInSet(addition, epsilonReachableSymbols, epsilon, LeftSymbol);
	}
	allProductions.insert(addition.begin(), addition.end());
	return startReachEpsilonFlag;
}

bool Gramar::isProductionMadeBySymbols(const production& p, const set<symbol>& symbols)
{
	int flag = true;
	for (symbol s:p) {
		if (!symbols.count(s)) { flag = false; break; }
	}
	return flag;
}


void Gramar::eliminateEpsilon()
{
	set<symbol> epsilonReachable = { epsilon };
	for (int lastSize = epsilonReachable.size();; lastSize = epsilonReachable.size()) {
		for (unordered_map<symbol, set<production>>::iterator it = P.begin(); it != P.end(); it++) {
			for (production p : it->second) {
				if (isProductionMadeBySymbols(p,epsilonReachable))epsilonReachable.insert(it->first);
			}
		}

		if (lastSize == epsilonReachable.size())break;
	}
	cout << ">>The epsilon reachable set is : ";
	printSet(epsilonReachable, cout); cout << endl;
	bool startReachEpsilonFlag = false;
	for (unordered_map<symbol, set<production>>::iterator it = P.begin(); it != P.end(); it++) {
		startReachEpsilonFlag |= eliminateEpsilonInSet(it->second, epsilonReachable, epsilon, it->first);
	}

	if (startReachEpsilonFlag == true) {
		production temp = { start };
		production tempE = { epsilon };
		start = _START_SYMBOL;
		P[start] = { temp,tempE };
	}

};



void Gramar::eliminateSingle() {
	bool flag = false;
	while (!flag) {
		flag = true;
		for (unordered_map<symbol, set<production>>::iterator it = P.begin(); it != P.end(); it++)
			for (set<production>::iterator p = it->second.begin(); p != it->second.end();) {
				if (p->size() == 1 && p->front() == it->first) {
					p = it->second.erase(p);
				}
				else if (p->size() == 1 && p->front() != epsilon && belongsN(p->front())) {
					if (P.find(p->front()) == P.end())continue;
					it->second.insert(P[p->front()].begin(), P[p->front()].end());
					p = it->second.erase(p);
					flag = false;
				}
				else p++;
			}
	}
	return;
}
void Gramar::eliminateNotProductionSymbol()
{
	set<symbol> productionSymbol(T);
	productionSymbol.insert(epsilon);
	for (int lastSize = productionSymbol.size();; lastSize = productionSymbol.size()) {
		for (unordered_map<symbol, set<production>>::iterator it = P.begin(); it != P.end(); it++) {
			bool flag = false;//== can  it->first be Production
			for (production p:it->second) {
				if (isProductionMadeBySymbols(p, productionSymbol)) {flag = true; break;	}
			}
			if (flag)productionSymbol.insert(it->first);
		}
		if (lastSize == productionSymbol.size())break;
	}
	cout << ">>The productionSymbols are "; 
	printSet(productionSymbol, cout); cout << endl;

	for (unordered_map<symbol, set<production>>::iterator it = P.begin(); it != P.end(); ) {
		for (set<production>::iterator p = it->second.begin(); p != it->second.end();) {
			bool flag = true;// p is made by production symbol
			for (symbol s : *p) {
				if (!productionSymbol.count(s)) { flag = false; break; }
			}
			if (!flag)p = it->second.erase(p);
			else p++;
		}
		if (it->second.size() == 0)it = P.erase(it);
		else it++;
	}
	return;
}
void Gramar::eliminateNotReachableSymbol()
{
	set<symbol> ReachableSymbol = {start};
	set<symbol> used;
	for (int lastSize = ReachableSymbol.size();; lastSize = ReachableSymbol.size()) {
		for (symbol reachedN : ReachableSymbol) {
			if (used.count(reachedN)||!P.count(reachedN))continue;
			for (production p : P[reachedN])
				for (symbol s : p) {
					if (N.count(s))ReachableSymbol.insert(s);
				}
			used.insert(reachedN);
		}
		if (lastSize == ReachableSymbol.size())break;
	}
	cout << ">>The ReachableSymbols are ";
	printSet(ReachableSymbol, cout); cout << endl;

	for (unordered_map<symbol, set<production>>::iterator it = P.begin(); it != P.end(); ) {
		if (!ReachableSymbol.count(it->first)) {
			it = P.erase(it);
		}
		else it++;
	}
	return;
}
void Gramar::eliminateUseless()
{
	eliminateNotProductionSymbol();
	eliminateNotReachableSymbol();
}
;

