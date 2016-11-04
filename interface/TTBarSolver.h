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

#include "URStreamer.h"

using namespace std;
using namespace TMath;

class IDMet;
class IDJet;

void myfuncln(Int_t& npar, Double_t* deriv, Double_t& val, Double_t* par, Int_t flag);

class TTBarSolver
{
	private:
		TMinuit minuit;
		TFile* probfile;
		TH2D* WTmass_right;
		TH2D* WTmt_right;
		TH1D* BTag_right; 
		TH1D* N_right; 
		//TH2D* WTmass_wrong;
		TH1D* BTag_wrong; 
		//TH1D* N_wrong; 
		double masscutoff;

		IDJet* bhad_;
		IDJet* j1had_;
		IDJet* j2had_;
		IDJet* blep_;
		TLorentzVector* llep_;
		IDMet* met_;

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

		bool USEBTAG;
		bool USENS;
		bool USEMASS;
		bool kinfit_;
		double c_mt;
		double c_mw;
		double c_rt;
		double c_rw;
		double c_rwt;
		double norm;
		double masstestmax;

	public:
		double Test(double* par);
		static TTBarSolver* TTBS; 
		TTBarSolver();
		~TTBarSolver();
		void Init(bool pseudo, string filename, bool usebtag = true, bool usens = true, bool usemass = true);//provide root file with probability distribution, switches if btag and neutrino solver information should be used for final discriminant Res()
		void Solve(TLorentzVector* bhad, TLorentzVector* j1had, TLorentzVector* j2had, TLorentzVector* blep, TLorentzVector* llep, IDMet* met, bool kinfit = false);

		//extrem unlikely hypothesis will return a value >= 1E10
		double Res() const {return res;}//final discriminant
		double NSRes() const {return nstest;}//-log(l) of neutriosolver 
		double BTagRes() const {return btagtest;} //-log(l) of btagging
		double MassRes() const {return masstest;} //-log(l) of 2d-mass test

		double NSChi2() const {return nschi;}//chi2 of neutrinosolver

		//improved objects: currently only usefull for neutrino
		TLorentzVector BHad() const {return bhadT_;}
		TLorentzVector Wja() const {return j1hadT_;}
		TLorentzVector Wjb() const {return j2hadT_;}
		TLorentzVector BLep() const {return blepT_;}
		TLorentzVector L() const {return llepT_;}
		TLorentzVector Nu() const {return metT_;}
};

#endif
