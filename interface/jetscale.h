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
#include "TTBarResponse2D.h"
#include "Permutation.h"
#include "BtagEff.h"
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
		bool isMC = 0;
		map<int, set<int> >  runinfo;
		PDFuncertainty* pdfunc;
		//Collections
		//Gen:
		bool FULLHAD;
		bool SEMILEP;
		bool FULLLEP;
		bool SEMILEPACC;
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


		//configuration
		bool PDFTEST;
		bool PSEUDOTOP;
		bool BTAGMODE;
		bool JETSCALEMODE;
		bool MUONS;
		bool ELECTRONS;
		double B_TIGHT;
		double B_MEDIUM;
		double B_LOOSE;
		string cnbtag;
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
		int cjetres;
		double csigmamet;
		double ctopptweight;
		double ctoprapweight;
		double cttptweight;
		int cfacscale;
		int crenscale;
		int cbtagunc;
		int cltagunc;
		int cpileup;
		bool TTMC;
		bool HERWIGPP;
		bool PYTHIA6;
		bool SCALEUP;
		bool SCALEDOWN;
		//
		double jetptmin;
	
		double weight;
		double mcweight;


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
		void AddGenJetSelection(URStreamer& event);
		void ttanalysis(URStreamer& event);

		static void setOptions() {}
};

#endif
