#ifndef HJETSCALE
#define HJETSCALE
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

#include <TTree.h>
#include <TLorentzVector.h>

using namespace std;

class Permutation;

class JetScale
{
	private:
		TTree* tree;
		Float_t minp[4];
		Float_t maxp[4];
		Float_t weight_;
		UInt_t type;

	public:
		JetScale();
		~JetScale();
		void Init(string treename);
		void Fill(const Permutation& per, bool wcorrect, float weight = 1);
};

#endif
