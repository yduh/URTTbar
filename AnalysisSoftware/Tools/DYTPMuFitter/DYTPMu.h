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

		UInt_t TRK;
		UInt_t SAgTRK;
		UInt_t SA;
		UInt_t TRKgSA;
		UInt_t ID;
		UInt_t ISO;
		UInt_t TR_ISOMu24;
		UInt_t TR_Mu17Mu8_L1;
		UInt_t TR_Mu17Mu8_L1gt10;
		UInt_t TR_Mu17Mu8_L2DMF;
		UInt_t TR_Mu17Mu8_L2SMF;
		UInt_t TR_Mu17Mu8_L3DMF;
		UInt_t TR_Mu17Mu8_L3SMF;
		UInt_t DZtest;
		UInt_t DZpos;
		UInt_t TR_ISOMu24_test;
		UInt_t TR_Mu17Mu8_L1_test;
		UInt_t TR_Mu17Mu8_L1gt10_test;
		UInt_t TR_Mu17Mu8_L2DMF_test;
		UInt_t TR_Mu17Mu8_L2SMF_test;
		UInt_t TR_Mu17Mu8_L3DMF_test;
		UInt_t TR_Mu17Mu8_L3SMF_test;
		UInt_t MCTRUTH;

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
