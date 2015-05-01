#ifndef GENPLOTTTBAR
#define GENPLOTTTBAR
#include <helper.h>
#include <string>
#include <iostream>
#include <TLorentzVector.h>
//#include <TTBarPlots.h>
#include <TTBarPlotsBase.h>

using namespace std;

class Jet;
class ttbar;
class Permutation;

class TTBarGenPlots : public TTBarPlotsBase
{
	private:

	public:
		TTBarGenPlots(string prefix);
		~TTBarGenPlots();
		void Init(ttbar* analysis);
		//void Fill(TLorentzVector* Hb, TLorentzVector* Ln, int lepcharge, double weight);
		void Fill(TLorentzVector* Hb, TLorentzVector* Hwa, TLorentzVector* Hwb, TLorentzVector* Lb, TLorentzVector* Ll, TLorentzVector* Ln, int leppdgid, double weight);
};

#endif
