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
		vector<int> jetbins = {-1, 0, 1, 2, 3};

	public:
		TTBarPlots(string prefix);
		~TTBarPlots();
		void Init(ttbar* analysis);
		void Fill(Permutation& per, double weight);
};

#endif
