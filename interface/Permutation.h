#ifndef PERMUTATION_H
#define PERMUTATION_H
#include <vector>
#include <limits>
#include <iostream>
#include <TLorentzVector.h>
#include <URStreamer.h>
//#include <IDJet.h>

using namespace std;

class TTBarSolver;
class IDMet;

class Permutation
{
	private:
		double prob_ = numeric_limits<double>::max();
		double nu_chisq_          = numeric_limits<double>::max();
		double nu_discriminant_   = numeric_limits<double>::max();
		double btag_discriminant_ = numeric_limits<double>::max();
		double mass_discriminant_ = numeric_limits<double>::max();
		double mt_discriminant_ = numeric_limits<double>::max();
		TLorentzVector* wja_ = 0;
		TLorentzVector* wjb_ = 0;
		TLorentzVector* wj_ = 0;
		TLorentzVector* bjh_ = 0;
		TLorentzVector* bjl_ = 0;
		TLorentzVector* lep_ = 0;
		int lpdgid_ = 0;
		IDMet* met_ = 0;
		bool calculated = false;
		TLorentzVector nu_;
		TLorentzVector whad_;
		TLorentzVector wlep_;
		TLorentzVector thad_;
		TLorentzVector tlep_;
		TLorentzVector tt_;
		TLorentzVector t_cms_;
		bool kinfit_ = false;
		vector<TLorentzVector> improvedobjects;
	public:
		Permutation() : nu_(0.,0.,0.,-1.){}
		//Permutation(int a) {cout << "HALLOllll "<< improvedobjects.size() << endl; }
		//Permutation(IDJet* wja, IDJet* wjb, IDJet* bjh, IDJet* bjl, TLorentzVector* lep, int leppdgid, IDMet* met);
		//Permutation(TLorentzVector* wja, TLorentzVector* wjb, TLorentzVector* bjh, TLorentzVector* bjl, TLorentzVector* lep, int leppdgid, TLorentzVector* nu);
		void Init(TLorentzVector* wja, TLorentzVector* wjb, TLorentzVector* bjh, TLorentzVector* bjl, TLorentzVector* lep, int leppdgid, TLorentzVector* nu)
		{
			Reset();
			wja_ = wja;
			wjb_ = wjb;
			bjh_ = bjh;
			bjl_ = bjl;
			lep_ = lep;
			nu_ = *nu;
			lpdgid_ = leppdgid;
		}
		//Permutation(TLorentzVector* wja, TLorentzVector* wjb, TLorentzVector* bjh, TLorentzVector* bjl, TLorentzVector* lep, int leppdgid, IDMet* met);
		void Init(TLorentzVector* wja, TLorentzVector* wjb, TLorentzVector* bjh, TLorentzVector* bjl, TLorentzVector* lep, int leppdgid, IDMet* met)
		{
			Reset();
			wja_ = wja;
			wjb_ = wjb;
			bjh_ = bjh;
			bjl_ = bjl;
			lep_ = lep;
			met_ = met;
			lpdgid_ = leppdgid;
		}
		void Reset();
		bool IsComplete() const {return(wja_ != 0 && wjb_ != 0 && bjh_ != 0 && bjl_ != 0 && lep_ != 0 && (met_ != 0 || nu_.E() > -0.5)&& wja_ != wjb_ && wja_ != bjh_ && wja_ != bjl_ && wjb_ != bjl_ && wjb_ != bjh_ && bjl_ != bjh_);}
		//bool IsComplete3J() const {return((wja_ != 0 || wjb_!= 0) && bjh_ != 0 && bjl_ != 0 && lep_ != 0 && met_ != 0 && wja_ != wjb_ && (wja_ != bjh_ && wjb_ != bjh_) && (wja_ != bjl_ && wjb_ != bjl_) && bjl_ != bjh_);}
		bool IsComplete3Ja() const {return(wja_ != 0 && wjb_ == 0 && bjh_ != 0 && bjl_ != 0 && lep_ != 0 && met_ != 0 && wja_ != bjh_ && wja_ != bjl_ && bjl_ != bjh_);}
		bool IsComplete3Jb() const {return(wjb_ != 0 && wja_ == 0 && bjh_ != 0 && bjl_ != 0 && lep_ != 0 && met_ != 0 && wjb_ != bjh_ && wjb_ != bjl_ && bjl_ != bjh_);}
		int NumBJets() const {return((bjl_ != 0 ? 1 : 0) + (bjh_ != 0 ? 1 : 0));}
		int NumWJets() const {return((wja_ != 0 ? 1 : 0) + (wjb_ != 0 ? 1 : 0));}
		int NumTTBarJets() const {return(NumBJets() + NumWJets());}
		double Solve(TTBarSolver& ttsolver, bool kinfit = false);
		TLorentzVector* WJa() const {return(wja_);}
		TLorentzVector* WJb() const {return(wjb_);}
		TLorentzVector* BHad() const {return(bjh_);}
		TLorentzVector* BLep() const {return(bjl_);}
		TLorentzVector* L() const {return(lep_);}
		IDMet* MET() const {return(met_);}
		void WJa(TLorentzVector* wja){calculated = false; wja_=wja;}
		void WJb(TLorentzVector* wjb){calculated = false; wjb_=wjb;}
		void BHad(TLorentzVector* bjh){calculated = false; bjh_=bjh;}
		void BLep(TLorentzVector* bjl){calculated = false; bjl_=bjl;}
		void L(TLorentzVector* lep, int leppdgid){calculated = false; lep_=lep; lpdgid_=leppdgid;}
		void Nu(TLorentzVector* nu){calculated = false; nu_=*nu;}
		int LCharge() {return (lpdgid_ < 0 ? 1 : -1);}
		int LPDGId() {return lpdgid_;}
		void MET(IDMet* met){met_=met;}

		void Calculate()
		{
			if(!calculated)
			{
				calculated = true;
				whad_ = *WJa() + *WJb();
				wlep_ = *L() + Nu();
				thad_ = whad_ + *BHad();
				tlep_ = wlep_ + *BLep();
				tt_ = thad_ + tlep_;
				t_cms_ = T();
				t_cms_.Boost(-1.*TT().BoostVector());
			}
		}

		TLorentzVector& Nu() {return(nu_);}
		TLorentzVector& WHad() {Calculate(); return whad_;}
		TLorentzVector& WLep() {Calculate(); return wlep_;}
		TLorentzVector& THad() {Calculate(); return thad_;}
		TLorentzVector& TLep() {Calculate(); return tlep_;}
		TLorentzVector& TT() {Calculate(); return(tt_);}
		TLorentzVector& T() {return (LCharge() < 0 ? THad() : TLep());}
		TLorentzVector& Tb() {return (LCharge() > 0 ? THad() : TLep());}
		TLorentzVector& THard() {return (THad().Pt() > TLep().Pt() ? THad() : TLep());}
		TLorentzVector& TSoft() {return (THad().Pt() < TLep().Pt() ? THad() : TLep());}
		TLorentzVector& T_CMS() {Calculate(); return t_cms_;}

		double MtWLep();
		double MttLep();

		double Prob() const {return(prob_);}
		double NuChisq() 	 const {return nu_chisq_         ;}
		double NuDiscr() 	 const {return nu_discriminant_  ;}
		double BDiscr()  	 const {return btag_discriminant_;}
		double MassDiscr() const {return mass_discriminant_;}
		double MTDiscr() const {return mt_discriminant_;}

	//	bool IsValid() const
	//	{
	//		if(WJa() != 0 && WJa() == WJb()) {return(false);}
	//		if(BHad() != 0 && (BHad() == WJa() || BHad() == WJb())) {return(false);}
	//		if(BLep() != 0 && (BLep() == BHad() || BLep() == WJa() || BLep() == WJb())) {return(false);}
	//		return(true);
	//	}

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
		bool IsJetIn(TLorentzVector* jet)
		{
			return(jet == WJa() || jet == WJb() || jet == BHad() || jet == BLep());
		}
		
};

bool operator<(const Permutation& A, const Permutation& B);
bool operator>(const Permutation& A, const Permutation& B);

#endif
