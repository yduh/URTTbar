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
		//Float_t prob2;
		Float_t weight;
		Float_t nvtx;
		UInt_t typ;
		double btagselection;
	public:
		BtagEff();
		void Init(double btagsel);
		void Fill(Permutation& per, float thenvtx, int filltyp, double theweight);
};

#endif
