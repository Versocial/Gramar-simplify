#include "Gramar.h"
#define _START_SYMBOL "S'"

bool Gramar::eliminateEpsilonInSet(set<production>& allProductions, const set<symbol>&epsilonReachableSymbols, symbol epsilon,symbol LeftSymbol) 
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
				if (epsilonReachableSymbols.count(*s)&& (p.size() > 1)) {
						symbol now = *s;
						s = p.erase(s);
						production temp(p);
						if (!(p.size() == 1 && p.front() == LeftSymbol)) { addition.insert(temp); }
						s=p.insert(s, now);
						num++;
				}
				else if(!epsilonReachableSymbols.count(*s)) epsilonReachableFlag = false;
			}
			if (num >= 2)canReturn = false;
			if (epsilonReachableFlag)startReachEpsilonFlag = true;
			pIt++;
		}
	if (!canReturn) {
		startReachEpsilonFlag |= eliminateEpsilonInSet(addition, epsilonReachableSymbols, epsilon, LeftSymbol);
	}
	allProductions.insert(addition.begin(),addition.end());
	return startReachEpsilonFlag;
}


void Gramar::eliminateEpsilon() 
{
	set<symbol> epsilonReachable = { epsilon };
	for (int size = epsilonReachable.size();; size=epsilonReachable.size()) {
		for (unordered_map<symbol, set<production>>::iterator it = P.begin(); it != P.end(); it++) {
			for (production p : it->second) {
				bool flag = true;//Epsilon Reachable
				for (symbol s : p) {
					if (!epsilonReachable.count(s)) { flag = false; break; }
				}
				if (flag)epsilonReachable.insert(it->first);
			}
		}
		
	if (size == epsilonReachable.size())break;
	}
	cout << ">>The epsilon reachable set is : ";
	printSet(epsilonReachable, cout); cout << endl;
	bool startReachEpsilonFlag = false;
	for (unordered_map<symbol, set<production>>::iterator it = P.begin(); it != P.end(); it++) {
		startReachEpsilonFlag |= eliminateEpsilonInSet(it->second, epsilonReachable, epsilon, it->first);
		}
			
	if ( startReachEpsilonFlag == true) {
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
};