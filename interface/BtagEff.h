#ifndef BTAGEFF_H
#define BTAGEFF_H
#include <TTree.h>

class Permutation;

class BtagEff
{
	private:
		TTree* btagtree;
		Float_t j[5];
		Float_t prob;
		Float_t problep;
		Float_t probhad;
		Float_t probnu;
		Float_t met;
		Float_t weight;
		Float_t nvtx;
		Int_t typ;
		Int_t test;
		double btagselection;
	public:
		BtagEff();
		void Init(double btagsel);
		//void Fill(Permutation& per, float thenvtx, bool filltyp, double theweight);
		void Fill(Permutation& per, float thenvtx, bool bhadcorrect, bool blepcorrect, double theweight);
};

#endif
