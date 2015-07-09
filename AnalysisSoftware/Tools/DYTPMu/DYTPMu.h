#ifndef EXAMPLEDY
#define EXAMPLEDY
#include <Analyse.h>
#include <OMuon.h>
#include <OElectron.h>
#include <OPhoton.h>
#include <OJet.h>
#include <Track.h>
#include <helper.h>
#include <GenBasicParticle.h>
#include <GenSelectedParticle.h>
#include <GenSimpleParticle.h>
#include <GenJet.h>
#include <vector>
#include <list>

class TH1D;
class TH2D;
class TTree;
class TFile;

using namespace std;


class DYTPMu : public Analyse
{
	friend class TTBarPlots;
	private:
		TFile* histfile;
		TH1DCollection hists1d;	
		TH2DCollection hists2d;	
		UInt_t testrun;
		double cmuptmin;
		double cmuetamax;
		double cmumumassmin;
		double cmumumassmax;
		vector<double> dzetabins;

		TTree* EffTree;
		Float_t px[2];
		Float_t py[2];
		Float_t pz[2];
		Float_t weight;
		UInt_t efftype[3];

		TTree* TPTree;
		TTree* DZTree;
		Float_t tpx, tpy, tpz, ppx, ppy, ppz, rho;
		UInt_t tpresult, run;
		Int_t numgoodvertices;

		UInt_t TRK = 1<<0;
		UInt_t SAgTRK = 1<<1;
		UInt_t SA = 1<<2;
		UInt_t TRKgSA = 1<<3;
		UInt_t ID = 1<<4;
		UInt_t ISO = 1<<5;
		UInt_t TR_ISOMu27 = 1<<6;
		UInt_t TR_Mu17Mu8_L1 = 1<<7;
		UInt_t TR_Mu17Mu8_L1gt10 = 1<<8;
		UInt_t TR_Mu17Mu8_L2DMF = 1<<9;
		UInt_t TR_Mu17Mu8_L2SMF = 1<<10;
		UInt_t TR_Mu17Mu8_L3DMF = 1<<11;
		UInt_t TR_Mu17Mu8_L3SMF = 1<<12;
		UInt_t DZtest = 1<<13;
		UInt_t DZpos = 1<<14;
		UInt_t TR_ISOMu27_test = 1<<15;
		UInt_t TR_Mu17Mu8_L1_test = 1<<16;
		UInt_t TR_Mu17Mu8_L1gt10_test = 1<<17;
		UInt_t TR_Mu17Mu8_L2DMF_test = 1<<18;
		UInt_t TR_Mu17Mu8_L2SMF_test = 1<<19;
		UInt_t TR_Mu17Mu8_L3DMF_test = 1<<20;
		UInt_t TR_Mu17Mu8_L3SMF_test = 1<<21;
		UInt_t MCTRUTH = 1<<22;

		list<GenSimpleParticle> SBasicParticles; 
		vector<GenSimpleParticle*> GenMus; 
		list<OMuon> SMuons;
		vector<OMuon*> RecMus;

		void SelectGENParticles();
		void SelectRECOParticles();
		
		void FillTruthTree();
		void FillTPTree();
		void PrintTriggerFilter(string trname);
		void DZTest();

		int TriggerTest(const vector<string>& filternames, OMuon& mu);
		vector<string> isomuvec;
		vector<string> dmfvec;
		vector<string> smfvec;
	public:
		DYTPMu(string outfile);
		virtual ~DYTPMu();
		virtual Int_t AnalyseEvent();
		virtual void FileChanged();
		virtual void BeginLoop();
};

#endif
