#ifndef BASEPLOTTTBAR
#define BASEPLOTTTBAR
#include <helper.h>
#include <string>
#include <iostream>
#include <TLorentzVector.h>

using namespace std;

class ttbar;
class Permutation;

class TTBarPlotsBase
{
	protected:
		string prefix_;
		TH1DCollection plot1d;	
		TH2DCollection plot2d;	
		ttbar* an;

	public:
		TTBarPlotsBase(string prefix);
		~TTBarPlotsBase();
		void Init(ttbar* analysis);
		//void Fill(TLorentzVector* Hb, TLorentzVector* Hwa, TLorentzVector* Hwb, TLorentzVector* Lb, TLorentzVector* Ll, TLorentzVector* Ln, int lepcharge, double weight);
		void Fill(Permutation& per, double weight);
};

#endif
