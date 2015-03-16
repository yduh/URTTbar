#ifndef TTBARAN_H
#define TTBARAN_H
#include <iostream>
#include <list>
#include "AnalyzerBase.h"
#include "URStreamer.h"
#include "URDriver.h"
#include "IDMuon.h"
#include "IDElectron.h"
#include "TTBarPlots.h"
#include "TTBarSolver.h"
#include "Permutation.h"

using namespace std;

class ttbar : public AnalyzerBase
{
	private:
		//Collections
		//Gen:
		bool FULLHAD;
		bool SEMILEP;
		bool FULLLEP;
		bool SEMILEPACC;
		list<Genparticle> sgenparticles;
		vector<Genparticle*> genwpartons;
		vector<Genparticle*> gencls;
		vector<Genparticle*> gennls;
		Genparticle* genb;
		Genparticle* genbbar;
		Genparticle* genbl;
		Genparticle* genbh;

		//matched
		//vector<Jet*> recbjets;
		//vector<Jet*> recwjets;
		//Jet* recbjet;
		//Jet* recbbarjet;
		//Jet* recbhjet;
		//Jet* recbljet;
		//int nttjets;
		Permutation rightper;
		vector<Jet*> recotherjets;

		//reco
		list<Jet> sjets;
		vector<Jet*> cleanedjets;
		vector<Jet*> reducedjets;
		list<IDMuon> smuons;
		vector<IDMuon*> loosemuons;
		vector<IDMuon*> tightmuons;
		list<IDElectron> selectrons;
		vector<IDElectron*> looseelectrons;
		vector<IDElectron*> mediumelectrons;
		Met met;

		//hists
		TH1DCollection gen1d;
		TH2DCollection gen2d;
		TH1DCollection reco1d;
		TH2DCollection reco2d;
		TH1DCollection truth1d;
		TH2DCollection truth2d;
		TTBarPlots ttp_right;
		TTBarPlots ttp_wrong;
		TTBarPlots ttp_semi;
		TTBarPlots ttp_other;
		TTBarPlots ttp_truth;
		TTBarPlots ttp_all;
		TTBarPlots ttp_right_imp;
		TTBarPlots ttp_wrong_imp;
		TTBarPlots ttp_semi_imp;
		TTBarPlots ttp_other_imp;
		TTBarPlots ttp_all_imp;

		//ttbar solver
		TTBarSolver ttsolver;

		//configuration
		int cnbtag;
		size_t cnusedjets;
		double cwjetptsoft;
		double cwjetpthard;
		double cbjetpt;
		double cjetetamax;
		double clptmin;
		double cletamax;
		//
		double jetptmin;

	public:
		//binning vectors
		vector<double> topptbins;
		vector<double> ttmbins;

		ttbar(const std::string output_filename);

		//This method is called once per job at the beginning of the analysis
		//book here your histograms/tree and run every initialization needed
		virtual void begin();
		virtual void analyze();

		void SelectGenParticles(URStreamer& event);
		void SelectRecoParticles(URStreamer& event);
		void ttanalysis();

		static void setOptions() {}
};

#endif
