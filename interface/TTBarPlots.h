#ifndef RECOPLOTTTBAR
#define RECOPLOTTTBAR
#include <helper.h>
#include <string>
#include <iostream>
#include <TLorentzVector.h>
#include <TTBarPlotsBase.h>

using namespace std;

class Jet;
class ttbar;
class Permutation;

class TTBarPlots : public TTBarPlotsBase
{
	private:

	public:
		TTBarPlots(string prefix);
		~TTBarPlots();
		void Init(ttbar* analysis);
		void Fill(Permutation& per, int lepcharge, double weight);
};

#endif
