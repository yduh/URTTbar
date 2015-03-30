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
		void Reset();
		bool IsComplete() const {return(wja_ != 0 && wjb_ != 0 && bjh_ != 0 && bjl_ != 0 && lep_ != 0 && met_ != 0);}
		int NumBJets() const {return((bjl_ != 0 ? 1 : 0) + (bjh_ != 0 ? 1 : 0));}
		int NumWJets() const {return((wja_ != 0 ? 1 : 0) + (wjb_ != 0 ? 1 : 0));}
		int NumTTBarJets() const {return(NumBJets() + NumWJets());}
		double Solve(TTBarSolver& ttsolver, bool kinfit = false);
		Jet* WJa() const {return(wja_);}
		Jet* WJb() const {return(wjb_);}
		Jet* BHad() const {return(bjh_);}
		Jet* BLep() const {return(bjl_);}
		TLorentzVector* L() const {return(lep_);}
		Met* MET() const {return(met_);}
		void WJa(Jet* wja){wja_=wja;}
		void WJb(Jet* wjb){wjb_=wjb;}
		void BHad(Jet* bjh){bjh_=bjh;}
		void BLep(Jet* bjl){bjl_=bjl;}
		void L(TLorentzVector* lep){lep_=lep;}
		void MET(Met* met){met_=met;}

		TLorentzVector Nu() const {return(nu_);}
		TLorentzVector WHad() const {return((*WJa() + *WJb()));}
		TLorentzVector WLep() const {return((*L() + Nu()));}
		TLorentzVector THad() const {return((WHad() + *BHad()));}
		TLorentzVector TLep() const {return((WLep() + *BLep()));}
		double Prob() const {return(prob_);}

		bool AreBsCorrect(const Permutation& other) const //bjets are selected correct, but not necessarily at the right position!!!!!!!!!!
		{
			return((BLep() == other.BLep() && BHad() == other.BHad()) || (BHad() == other.BLep() && BLep() == other.BHad()));
		}
		bool AreJetsCorrect(const Permutation& other) const
		{
			if(BLep() != other.BLep() && BLep() != other.BHad() && BLep() != other.WJa() && BLep() != other.WJb()){return(false);}
			if(BHad() != other.BLep() && BHad() != other.BHad() && BHad() != other.WJa() && BHad() != other.WJb()){return(false);}
			if(WJa() != other.BLep() && WJa() != other.BHad() && WJa() != other.WJa() && WJa() != other.WJb()){return(false);}
			if(WJb() != other.BLep() && WJb() != other.BHad() && WJb() != other.WJa() && WJb() != other.WJb()){return(false);}
			return(true);
		}

		bool AreHadJetsCorrect(const Permutation& other) const
		{
			if(BHad() != other.BHad() && BHad() != other.WJa() && BHad() != other.WJb()){return(false);}
			if(WJa() != other.BHad() && WJa() != other.WJa() && WJa() != other.WJb()){return(false);}
			if(WJb() != other.BHad() && WJb() != other.WJa() && WJb() != other.WJb()){return(false);}
			return(true);
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
		bool IsTHadCorrect(const Permutation& other) const
		{
			return(IsBHadCorrect(other) && IsWHadCorrect(other));
		}
		bool IsCorrect(const Permutation& other) const
		{
			return(IsBLepCorrect(other) && IsTHadCorrect(other));
		}

		
		
		
};

bool operator<(const Permutation& A, const Permutation& B);
bool operator>(const Permutation& A, const Permutation& B);

#endif
