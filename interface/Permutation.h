#ifndef PERMUTATIO_H
#define PERMUTATIO_H
#include <string>
#include <map>
#include <vector>

using namespace std;

class Permutation
{
	private:
		double prob_;
		Jet* wja_ = 0;
		Jet* wjb_ = 0;
		Jet* bjh_ = 0;
		Jet* bjl_ = 0;
		TLorentzVector* lep_;
		Met* met_;
		TLorentzVector nu_;
		bool kinfit_;
		vector<TLorentzVector> improvedobjects;
	public:
		Permutation(Jet* wja, Jet* wjb, Jet* bjh, Jet* bjl, TLorentzVector* lep, Met* met) :
		wja_(wja),
		wjb_(wjb),
		wjh_(wjh),
		wjl_(wjl),
		lep_(lep),
		met_(met)
		{

		}
		double Solve(TTBarSolver& ttsolver, bool kinfit = false)
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
		Jet* WJa() const {return(wja_);}
		Jet* WJb() const {return(wjb_);}
		Jet* BHad() const {return(bjh_);}
		Jet* BLep() const {return(bjl_);}
		TLorentzVector* L() const {return(lep_);}
		TLorentzVector Nu() const {return(nu_);}
		TLorentzVector WHad() const {return((*wja_ + *wjb_));}
		TLorentzVector THad() const {return((GetWHad() + *bjh_));}
		double Prob() const {return(prob_);}

		bool IsCorrect(const Permutation* other) const //bjets are selected correct, but not necessarily at the right position!!!!!!!!!!
		{
			return((BLep() == other->BLep() && BHad() == other->BHad()) || (BHad() == other->BLep() && BLep() == other->BHad()));
		}

		bool IsBLepCorrect(const Permutation* other) const
		{
			return(BLep() == other->BLep());
		}
		bool IsBHadCorrect(const Permutation* other) const
		{
			return(BHad() == other->BHad());
		}
		bool IsWHadCorrect(const Permutation* other) const
		{
			return((WJa() == other->WJa() && WJb() == other->WJb()) || (WJa() == other->WJb() && WJb() == other->WJa()));
		}
		bool IsCorrect() const
		{
			return(IsBLepCorrect() && IsBHadCorrect() && IsWHadCorrect());
		}
		
};

bool operator<(const Permutation& A, const Permutation& B)
{
return(A.Prob() < B.Prob());
}
bool operator>(const Permutation& A, const Permutation& B)
{
return(A.Prob() > B.Prob());
}

#endif
