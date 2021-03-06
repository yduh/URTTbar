#ifndef RECOPLOTTTBAR
#define RECOPLOTTTBAR
#include <helper.h>
#include <OJet.h>
#include <string>
#include <iostream>
#include <TLorentzVector.h>

using namespace std;

class TTBarPlots
{
	private:
		string prefix_;
		TH1DCollection plot1d;	
		TH2DCollection plot2d;	

	public:
		TTBarPlots(string prefix);
		~TTBarPlots();
		void Init();
		void Fill(TLorentzVector* Hb, TLorentzVector* Hwa, TLorentzVector* Hwb, TLorentzVector* Lb, TLorentzVector* Ll, TLorentzVector* Ln, int lepcharge, double test = 0., OJet* bjet = 0);
};

#endif
