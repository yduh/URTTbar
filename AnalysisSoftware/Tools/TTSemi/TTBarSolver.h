#ifndef HTTBSOLVER
#define HTTBSOLVER
#include <TMatrixD.h>
#include <TVector3.h>
#include <TMinuit.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <iostream>
#include <OJet.h>
#include <BASEIO.h>

using namespace std;
using namespace TMath;

void myfuncln(Int_t& npar, Double_t* deriv, Double_t& val, Double_t* par, Int_t flag);

class TTBarSolver
{
	private:
		TMinuit minuit;
		bool FULLMASS;
		TFile* probfile;
		TH2D* WTmass_right;
		TH1D* Tmass_right;
		TH1D* BTag_right; 
		TH1D* N_right; 
		TH2D* WTmass_wrong;
		TH1D* Tmass_wrong;
		TH1D* BTag_wrong; 
		TH1D* N_wrong; 

		TLorentzVector* bhad_;
		TLorentzVector* j1had_;
		TLorentzVector* j2had_;
		TLorentzVector* blep_;
		TLorentzVector* llep_;
		IOMET* met_;

		TLorentzVector bhadT_;
		TLorentzVector j1hadT_;
		TLorentzVector j2hadT_;
		TLorentzVector blepT_;
		TLorentzVector llepT_;
		TLorentzVector metT_;
		double ubhad_;
		double uj1had_;
		double uj2had_;
		double ublep_;
		double ullep_;
		double umetx_;
		double umety_;
		double rhomet_;

		double nschi;

		double btagtest;
		double nstest;
		double masstest;
		double res;
	public:
		static TTBarSolver* TTBS; 
		TTBarSolver();
		~TTBarSolver();
		void Init(string filename, bool WTMASS = true);
		void Solve(OJet* bhad, OJet* j1had, OJet* j2had, OJet* blep, TLorentzVector* llep, IOMET* met);

		double Test(double* par);
		double Res() const {return res;}
		double NSRes() const {return nstest;}
		double BTagRes() const {return btagtest;}
		double MassRes() const {return masstest;}
		double NSChi2() const {return nschi;}
		TLorentzVector BHad() const {return bhadT_;}
		TLorentzVector Wja() const {return j1hadT_;}
		TLorentzVector Wjb() const {return j2hadT_;}
		TLorentzVector BLep() const {return blepT_;}
		TLorentzVector L() const {return llepT_;}
		TLorentzVector Nu() const {return metT_;}
};

#endif
