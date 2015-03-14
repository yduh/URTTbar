#include "Permutation.h"
#include "TTBarSolver.h"

using namespace std;

Permutation::Permutation(Jet* wja, Jet* wjb, Jet* bjh, Jet* bjl, TLorentzVector* lep, Met* met) :
	wja_(wja),
	wjb_(wjb),
	bjh_(bjh),
	bjl_(bjl),
	lep_(lep),
	met_(met)
{
}

void Permutation::Reset()
{
	prob_ = numeric_limits<double>::max();
	wja_ = 0;
	wjb_ = 0;
	bjh_ = 0;
	bjl_ = 0;
	lep_ = 0;
	met_ = 0;
	kinfit_ = false;
	improvedobjects.clear();
}

double Permutation::Solve(TTBarSolver& ttsolver, bool kinfit)
{
	kinfit_ = kinfit;
	ttsolver.Solve(bjh_, wjb_, wja_, bjl_, lep_, met_);
	prob_ = ttsolver.Res();
	nu_ = ttsolver.Nu();
	if(kinfit_)
	{
		improvedobjects.push_back(ttsolver.Wja());
		improvedobjects.push_back(ttsolver.Wjb());
		improvedobjects.push_back(ttsolver.BHad());
		improvedobjects.push_back(ttsolver.BLep());
		improvedobjects.push_back(ttsolver.L());
		improvedobjects.push_back(ttsolver.Nu());
	}
	return(prob_);
}

bool operator<(const Permutation& A, const Permutation& B)
{
	return(A.Prob() < B.Prob());
}

bool operator>(const Permutation& A, const Permutation& B)
{
	return(A.Prob() > B.Prob());
}

