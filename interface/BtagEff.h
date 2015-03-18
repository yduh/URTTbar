#ifndef BTAGEFF_H
#define BTAGEFF_H
#include <TTree.h>

class Permutation;

class BtagEff
{
	private:
		TTree* btagtree;
		Float_t jwa[5];
		Float_t jwb[5];
		Float_t jb[5];
		Float_t prob;
		Float_t weight;
		UInt_t typ;
	public:
		BtagEff();
		void Init();
		void Fill(Permutation& per, int filltyp, double theweight);
};

#endif
