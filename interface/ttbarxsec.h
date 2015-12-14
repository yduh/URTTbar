#ifndef TTBARAN_H
#define TTBARAN_H
#include <iostream>
#include <list>
#include <set>
#include "AnalyzerBase.h"
#include "URStreamer.h"
#include "URDriver.h"
#include "IDMuon.h"
#include "IDElectron.h"
#include "IDJet.h"
#include "IDMet.h"
#include "GenObject.h"
#include "TTBarGenPlots.h"
#include "TTBarPlots.h"
#include "TTBarSolver.h"
#include "TTBarResponse.h"
#include "Permutation.h"
#include "BtagEff.h"
#include "JetScale.h"
#include "JetScaler.h"
#include "BTagWeight.h"

using namespace std;
class PDFuncertainty;

class ttbar : public AnalyzerBase
{
	friend class TTBarGenPlots;
    friend class TTBarPlots;
    friend class TTBarResponse;
    friend class BTagWeight;

	private:
		bool isMC = false;
		map<int, set<int> >  runinfo;
		double selectionprob;
		PDFuncertainty* pdfunc;
		//Collections
		//Gen:
		bool FULLHAD;
		bool SEMILEP;
		bool FULLLEP;
		bool SEMILEPACC;
		list<GenObject> sgenparticles;
		vector<GenObject*> genfincls;
	  	vector<GenObject*> genbpartons;
		GenObject* gent;
		GenObject* gentbar;
		//GenObject* genbl;
		//GenObject* genbh;
		//TLorentzVector gentoplep;
		//TLorentzVector gentophad;

		list<Genjet> sgenjets;
		vector<Genjet*> genaddjets;

		Permutation genallper;
		Permutation psper;
		Permutation* genper;
		Permutation rightper;
		Permutation testper;
		Permutation bestper;

		//reco
		list<IDJet> sjets;
		vector<IDJet*> cleanedjets;
		vector<IDJet*> reducedjets;
		list<IDMuon> smuons;
		vector<IDMuon*> loosemuons;
		vector<IDMuon*> tightmuons;
		list<IDElectron> selectrons;
		vector<IDElectron*> looseelectrons;
		vector<IDElectron*> mediumelectrons;
		IDMet met;

		//hists
		TH1DCollection gen1d;
		TH2DCollection gen2d;
		TH1DCollection reco1d;
		TH2DCollection reco2d;
		TH1DCollection truth1d;
		TH2DCollection truth2d;

        TTBarGenPlots ttp_genall;
        TTBarGenPlots ttp_genacc;
        TTBarGenPlots ttp_genjet;

		TTBarPlots ttp_truth;
		TTBarPlots ttp_right;
		TTBarPlots ttp_wrong;
		TTBarPlots ttp_semi;
		TTBarPlots ttp_other;
		TTBarPlots ttp_all;

		TTBarPlots ttp_jetspos_right;
		TTBarPlots ttp_jetspos_wrong;
	//	TTBarPlots ttp_hadjets_right;
	//	TTBarPlots ttp_hadjets_wrong;
	//	TTBarPlots ttp_jets_right;
	//	TTBarPlots ttp_jets_wrong;
	//	TTBarPlots ttp_blep_right;
	//	TTBarPlots ttp_blep_wrong;
		TTBarPlots ttp_whad_right;
		TTBarPlots ttp_whad_wrong;

		TTBarPlots ttp_tlepthad_right;
		TTBarPlots ttp_tlep_right;
		TTBarPlots ttp_thad_right;
		TTBarPlots ttp_nn_right;
		TTBarPlots ttp_nsemi_right;

		TTBarResponse response;
		TTBarResponse responseall;

		BtagEff btageff;
		BTagWeight btagweight;

		//ttbar solver
		TTBarSolver ttsolver;

		JetScaler jetscaler;

		//configuration
		bool DATASIM;
		bool PDFTEST;
		bool PSEUDOTOP;
		bool BTAGMODE;
		bool JETSCALEMODE;
		bool MUONS;
		bool ELECTRONS;
		double B_TIGHT;
		double B_MEDIUM;
		int cnbtag;
		size_t cnusedjets;
		double clikelihoodcut;
		double cwjetptsoft;
		double cwjetpthard;
		double cbjetptsoft;
		double cbjetpthard;
		double cjetetamax;
		double clptmin;
		double cletamax;
		double cpwjetptsoft;
		double cpwjetpthard;
		double cpbjetptsoft;
		double cpbjetpthard;
		double cpjetetamax;
		double cplptmin;
		double cpletamax;
		double cpjetsep;
		double csigmajet;
		double cjetres;
		double csigmamet;
		double ctopptweight;
		double cttptweight;
		int cfacscale;
		int crenscale;
		int cbtagunc;
		int cltagunc;
		int cpileup;
		int crandomseed;
		bool HERWIGPP;
		//
		double jetptmin;
	
		double weight;
		double mcweight;

		//binning vectors
		vector<double> topptbins;
		vector<double> topybins;
		vector<double> ttmbins;
		vector<double> ttybins;
		vector<double> ttptbins;
		vector<double> metbins;
		vector<double> jetbins;
		vector<double> nobins;
		vector<double> btagpt;
		vector<double> runbins;

//		JetScale jetscale;

		TH1D* puhist;
		TH2D* musfhist;
		TH2D* elsfhist;
		TH2D* mutrgsfhist;
		TH2D* eltrgsfhist;
	public:

		ttbar(const std::string output_filename);
		~ttbar();

		//This method is called once per job at the beginning of the analysis
		//book here your histograms/tree and run every initialization needed
		virtual void begin();
		//virtual void end();
		virtual void analyze();

		void SelectGenParticles(URStreamer& event);
		void SelectPseudoTop(URStreamer& event);
		void SelectRecoParticles(URStreamer& event);
		void ttanalysis(URStreamer& event);

		static void setOptions() {}
};

#endif
