#include "Permutation.h"
#include "TTBarSolver.h"
#include "IDMet.h"

using namespace std;


//Permutation::Permutation(TLorentzVector* wja, TLorentzVector* wjb, TLorentzVector* bjh, TLorentzVector* bjl, TLorentzVector* lep, int leppdgid, IDMet* met) :
//	wja_(wja),
//	wjb_(wjb),
//	bjh_(bjh),
//	bjl_(bjl),
//	lep_(lep),
//	lpdgid_(leppdgid),
//	met_(met)
//{
//}
//
//Permutation::Permutation(TLorentzVector* wja, TLorentzVector* wjb, TLorentzVector* bjh, TLorentzVector* bjl, TLorentzVector* lep, int leppdgid, TLorentzVector* nu):
//	wja_(wja),
//	wjb_(wjb),
//	bjh_(bjh),
//	bjl_(bjl),
//	lep_(lep),
//	lpdgid_(leppdgid)
//{
//nu_ = *nu;
//}

void Permutation::Reset()
{
	calculated = false;
	prob_ = numeric_limits<double>::max();
	nu_chisq_          = numeric_limits<double>::max();
	nu_discriminant_   = numeric_limits<double>::max();
	btag_discriminant_ = numeric_limits<double>::max();
	mass_discriminant_ = numeric_limits<double>::max();
	wja_ = nullptr;
	wjb_ = nullptr;
	bjh_ = nullptr;
	bjl_ = nullptr;
	lep_ = nullptr;
	met_ = nullptr;
	owja_ = nullptr;
	owjb_ = nullptr;
	objh_ = nullptr;
	objl_ = nullptr;
	olep_ = nullptr;
	kinfit_ = false;
	improvedobjects.clear();
}

double Permutation::Solve(TTBarSolver& ttsolver, bool kinfit)
{
	kinfit_ = kinfit;
	ttsolver.Solve(bjh_, wjb_, wja_, bjl_, lep_, met_, kinfit_);
	nu_ = ttsolver.Nu();
	prob_ = ttsolver.Res();
	nu_chisq_          = ttsolver.NSChi2();
	nu_discriminant_   = ttsolver.NSRes();
	btag_discriminant_ = ttsolver.BTagRes();
	mass_discriminant_ = ttsolver.MassRes();
	//mt_discriminant_ = ttsolver.MTRes();
	if(kinfit_)
	{
		
		improvedobjects.push_back(ttsolver.Wja());
		improvedobjects.push_back(ttsolver.Wjb());
		improvedobjects.push_back(ttsolver.BHad());
		improvedobjects.push_back(ttsolver.BLep());
		improvedobjects.push_back(ttsolver.L());
		improvedobjects.push_back(ttsolver.Nu());
		SetImproved(true);
	}
	return(prob_);
}

double Permutation::MtWLep() {return(Sqrt(Power(MET()->Pt() + L()->Pt(),2) - Power(MET()->Px() + L()->Px(),2) - Power(MET()->Py() + L()->Py(),2)));}

double Permutation::MttLep() {return(Sqrt(Power(MET()->Pt() + L()->Pt() + Sqrt(BLep()->M()*BLep()->M() + BLep()->Pt()*BLep()->Pt()),2) - Power(MET()->Px() + L()->Px() + BLep()->Px(),2) - Power(MET()->Py() + L()->Py() + BLep()->Py(),2)));}


bool operator<(const Permutation& A, const Permutation& B)
{
	return(A.Prob() < B.Prob());
}

bool operator>(const Permutation& A, const Permutation& B)
{
	return(A.Prob() > B.Prob());
}

