#ifndef RECOPLOTTTBAR
#define RECOPLOTTTBAR
#include <helper.h>
#include <string>
#include <iostream>
#include <TLorentzVector.h>

using namespace std;

class Jet;
class ttbar;
class Permutation;

class TTBarPlots
{
	private:
		string prefix_;
		TH1DCollection plot1d;	
		TH2DCollection plot2d;	

	public:
		TTBarPlots(string prefix);
		~TTBarPlots();
		void Init(ttbar* analysis);
		void Fill(TLorentzVector* Hb, TLorentzVector* Hwa, TLorentzVector* Hwb, TLorentzVector* Lb, TLorentzVector* Ll, TLorentzVector* Ln, int lepcharge, double test, double weight);
		void Fill(Permutation& per, int lepcharge, double weight);
};

#endif
