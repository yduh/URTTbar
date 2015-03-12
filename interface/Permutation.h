#ifndef PERMUTATION_H
#define PERMUTATION_H
#include <vector>
#include <limits>
#include <TLorentzVector.h>
#include <URStreamer.h>

using namespace std;

class TTBarSolver;

class Permutation
{
	private:
		double prob_ = numeric_limits<double>::max();
		Jet* wja_ = 0;
		Jet* wjb_ = 0;
		Jet* bjh_ = 0;
		Jet* bjl_ = 0;
		TLorentzVector* lep_ = 0;
		Met* met_ = 0;
		TLorentzVector nu_;
		bool kinfit_ = false;
		vector<TLorentzVector> improvedobjects;
	public:
		Permutation() {}
		Permutation(Jet* wja, Jet* wjb, Jet* bjh, Jet* bjl, TLorentzVector* lep, Met* met);
		double Solve(TTBarSolver& ttsolver, bool kinfit = false);
		Jet* WJa() const {return(wja_);}
		Jet* WJb() const {return(wjb_);}
		Jet* BHad() const {return(bjh_);}
		Jet* BLep() const {return(bjl_);}
		TLorentzVector* L() const {return(lep_);}
		TLorentzVector Nu() {return(nu_);}
		TLorentzVector WHad() const {return((*wja_ + *wjb_));}
		TLorentzVector THad() const {return((WHad() + *bjh_));}
		double Prob() const {return(prob_);}

		bool IsBCorrect(const Permutation& other) const //bjets are selected correct, but not necessarily at the right position!!!!!!!!!!
		{
			return((BLep() == other.BLep() && BHad() == other.BHad()) || (BHad() == other.BLep() && BLep() == other.BHad()));
		}

		bool IsBLepCorrect(const Permutation& other) const
		{
			return(BLep() == other.BLep());
		}
		bool IsBHadCorrect(const Permutation& other) const
		{
			return(BHad() == other.BHad());
		}
		bool IsWHadCorrect(const Permutation& other) const
		{
			return((WJa() == other.WJa() && WJb() == other.WJb()) || (WJa() == other.WJb() && WJb() == other.WJa()));
		}
		bool IsCorrect(const Permutation& other) const
		{
			return(IsBLepCorrect(other) && IsBHadCorrect(other) && IsWHadCorrect(other));
		}
		
};

bool operator<(const Permutation& A, const Permutation& B);
bool operator>(const Permutation& A, const Permutation& B);

#endif
