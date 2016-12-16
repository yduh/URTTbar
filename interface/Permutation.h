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
		TLorentzVector* owja_ = 0;
		TLorentzVector* owjb_ = 0;
		TLorentzVector* objh_ = 0;
		TLorentzVector* objl_ = 0;
		TLorentzVector* olep_ = 0;
		TLorentzVector* wja_ = 0;
		TLorentzVector* wjb_ = 0;
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
		void SetImproved(bool improved)
		{
			if(improved && improvedobjects.size() == 6)
			{
				wja_ = &improvedobjects[0]; 
				wjb_ = &improvedobjects[1]; 
				bjh_ = &improvedobjects[2]; 
				bjl_ = &improvedobjects[3]; 
				lep_ = &improvedobjects[4]; 
				nu_ = improvedobjects[5]; 
			}
			else
			{
				wja_ = owja_; 
				wjb_ = owjb_;
				bjh_ = objh_;
				bjl_ = objl_;
				lep_ = olep_;
			}
			calculated = false;
		}
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
			owja_ = wja;
			owjb_ = wjb;
			objh_ = bjh;
			objl_ = bjl;
			olep_ = lep;
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
			owja_ = wja;
			owjb_ = wjb;
			objh_ = bjh;
			objl_ = bjl;
			olep_ = lep;
			met_ = met;
			lpdgid_ = leppdgid;
		}
		void Reset();
		bool IsComplete() const {return(wja_ != 0 && wjb_ != 0 && bjh_ != 0 && bjl_ != 0 && lep_ != 0 && (met_ != 0 || nu_.E() > -0.5)&& wja_ != wjb_ && wja_ != bjh_ && wja_ != bjl_ && wjb_ != bjl_ && wjb_ != bjh_ && bjl_ != bjh_);}
 		bool IsComplete3Ja() const {return(wja_ != 0 && wjb_ == 0 && bjh_ != 0 && bjl_ != 0 && lep_ != 0 && (met_ != 0 || nu_.E() > -0.5) && wja_ != bjh_ && wja_ != bjl_ && bjl_ != bjh_);}
 		bool IsComplete3Jb() const {return(wjb_ != 0 && wja_ == 0 && bjh_ != 0 && bjl_ != 0 && lep_ != 0 && (met_ != 0 || nu_.E() > -0.5) && wjb_ != bjh_ && wjb_ != bjl_ && bjl_ != bjh_);}

		int NumBJets() const {return((bjl_ != 0 ? 1 : 0) + (bjh_ != 0 ? 1 : 0));}
		int NumWJets() const {return((wja_ != 0 ? 1 : 0) + (wjb_ != 0 ? 1 : 0));}
		int NumTTBarJets() const {return(NumBJets() + NumWJets());}
		double Solve(TTBarSolver& ttsolver, bool kinfit = false);
		TLorentzVector* WJa() const {return(wja_);}
		TLorentzVector* WJb() const {return(wjb_);}
		TLorentzVector* WJPtmin() const
		{
			if(wja_ == nullptr || wjb_ == nullptr){return nullptr;}
			return(wja_->Pt() < wjb_->Pt() ? wja_ : wjb_);
		}
		TLorentzVector* WJPtmax() const 
		{

			if(wja_ == nullptr && wjb_ != nullptr){return wjb_;}
			if(wja_ != nullptr && wjb_ == nullptr){return wja_;}
			if(wja_ == nullptr && wjb_ == nullptr){return nullptr;}
			return(wja_->Pt() >= wjb_->Pt() ? wja_ : wjb_);
		}
		TLorentzVector* BHad() const {return(bjh_);}
		TLorentzVector* BLep() const {return(bjl_);}
		TLorentzVector* L() const {return(lep_);}
		IDMet* MET() const {return(met_);}
		void WJa(TLorentzVector* wja){calculated = false; wja_=wja; owja_=wja;}
		void WJb(TLorentzVector* wjb){calculated = false; wjb_=wjb; owjb_=wjb;}
		void BHad(TLorentzVector* bjh){calculated = false; bjh_=bjh; objh_=bjh;}
		void BLep(TLorentzVector* bjl){calculated = false; bjl_=bjl; objl_=bjl;}
		void L(TLorentzVector* lep, int leppdgid){calculated = false; lep_=lep; olep_=lep; lpdgid_=leppdgid;}
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

		bool IsValid() const
		{
			if(owja_ != 0 && owja_ == owjb_) {return(false);}
			if(objh_ != 0 && (objh_ == owja_ || objh_ == owjb_)) {return(false);}
			if(objl_ != 0 && (objl_ == objh_ || objl_ == owja_ || objl_ == owjb_)) {return(false);}
			return(true);
		}

		bool AreBsCorrect(const Permutation& other) const //bjets are selected correct, but not necessarily at the right position!!!!!!!!!!
		{
			return((objl_ == other.objl_ && objh_ == other.objh_) || (objh_ == other.objl_ && objl_ == other.objh_));
		}
		bool AreJetsCorrect(const Permutation& other) const
		{
			if(objl_ != other.objl_ && objl_ != other.objh_ && objl_ != other.owja_ && objl_ != other.owjb_){return(false);}
			if(objh_ != other.objl_ && objh_ != other.objh_ && objh_ != other.owja_ && objh_ != other.owjb_){return(false);}
			if(owja_ != other.objl_ && owja_ != other.objh_ && owja_ != other.owja_ && owja_ != other.owjb_){return(false);}
			if(owjb_ != other.objl_ && owjb_ != other.objh_ && owjb_ != other.owja_ && owjb_ != other.owjb_){return(false);}
			return(true);
		}

		bool AreHadJetsCorrect(const Permutation& other) const
		{
			if(objh_ != other.objh_ && objh_ != other.owja_ && objh_ != other.owjb_){return(false);}
			if(owja_ != other.objh_ && owja_ != other.owja_ && owja_ != other.owjb_){return(false);}
			if(owjb_ != other.objh_ && owjb_ != other.owja_ && owjb_ != other.owjb_){return(false);}
			return(true);
		}

		bool IsBLepCorrect(const Permutation& other) const
		{
			//return(BLep() == other.BLep());
			return(objl_ == other.objl_);
		}
		bool IsBHadCorrect(const Permutation& other) const
		{
			//return(BHad() == other.BHad());
			return(objh_ == other.objh_);
		}
		bool IsWHadCorrect(const Permutation& other) const
		{
			//return((WJa() == other.WJa() && WJb() == other.WJb()) || (WJa() == other.WJb() && WJb() == other.WJa()));
			return((owja_ == other.owja_ && owjb_ == other.owjb_) || (owja_ == other.owjb_ && owjb_ == other.owja_));
		}
		bool IsTHadCorrect(const Permutation& other) const
		{
			return(IsBHadCorrect(other) && IsWHadCorrect(other));
		}
		bool IsCorrect(const Permutation& other) const
		{
			return(IsBLepCorrect(other) && IsTHadCorrect(other));
		}
		int IsJetIn(TLorentzVector* jet)
		{
			if(jet == objl_) return 1;
			if(jet == objh_) return 2;
			if(jet == WJPtmax()) return 3;
			if(jet == WJPtmin()) return 4;

			return 0;
		}
		TLorentzVector* GetJet(int n)
		{
			if(n == 1) return objl_;
			if(n == 2) return objh_;
			if(n == 3) return WJPtmax();
			if(n == 4) return WJPtmin();

			return nullptr;
		}
		
};

bool operator<(const Permutation& A, const Permutation& B);
bool operator>(const Permutation& A, const Permutation& B);

#endif
