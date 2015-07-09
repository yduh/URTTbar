#ifndef EXAMPLEDY
#define EXAMPLEDY
#include <Analyse.h>
#include <OMuon.h>
#include <OElectron.h>
#include <OPhoton.h>
#include <OJet.h>
#include <helper.h>
#include <GenBasicParticle.h>
#include <GenSelectedParticle.h>
#include <GenSimpleParticle.h>
#include <GenJet.h>
#include <vector>
#include <list>
#include "TTBarPlots.h"
#include "TTBarBins.h"
#include "TTBarSolver.h"

class TH1D;
class TH2D;
class TTree;
class TFile;

using namespace std;

class TTSemi : public Analyse
{
	friend class TTBarPlots;
	private:
		TFile* histfile;
		TTree* jestree;
		TTree* truejestree;
		TTree* falsejestree;
		Float_t jwa[3];
		Float_t jwb[3];
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
		double cwjetptmin;
		int cnbtag;

		list<GenJet> SGenJets; 
		vector<GenJet*> GenWJets; 
		vector<GenJet*> GenBJets; 

		list<GenSimpleParticle> SBasicParticles; 
		vector<GenSimpleParticle*> GenWPartons; 
		vector<GenSimpleParticle*> GenCL; 
		vector<GenSimpleParticle*> GenN; 
		GenSimpleParticle* GenB;
		GenSimpleParticle* GenBBar;
		GenSimpleParticle* GenBl;
		GenSimpleParticle* GenBh;
		TLorentzVector Genth;

		list<OMuon> SMuons;
		list<OElectron> SElectrons;
		list<OPhoton> SPhotons;
		list<OJet> SJets;

		vector<OMuon*> AllMuons;
		vector<OElectron*> AllElectrons;
		vector<OMuon*> LooseMuons;
		vector<OElectron*> LooseElectrons;
		vector<OPhoton*> AllPhotons;
		vector<OJet*> AllJets;
		vector<OJet*> BJets;
		vector<OJet*> RecWJets;
		vector<OJet*> RecOtherJets;
		OJet* BJet;
		OJet* BBarJet;
		OJet* BlJet;
		OJet* BhJet;

		bool FULLHAD;
		bool SEMILEP;
		bool SEMILEPACC;
		bool FULLLEP;

		void SelectGENParticles();
		void PlotGEN();
		void SelectRECOParticles();
		
		TTBarSolver ttsolver;

		vector<double> binning_pttop;

		//TTBarBins ttbar_T_ptt;
	public:
		TTSemi(string outfile);
		virtual ~TTSemi();
		virtual Int_t AnalyseEvent();
		virtual void FileChanged();
		virtual void BeginLoop();
};

#endif
