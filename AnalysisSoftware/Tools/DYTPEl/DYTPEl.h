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


class DYTPEl : public Analyse
{
	private:
		TFile* histfile;
		TH1DCollection hists1d;	
		TH2DCollection hists2d;	
		UInt_t testrun;
		double celptmin;
		double celetamax;
		double celelmassmin;
		double celelmassmax;

		TTree* EffTree;
		Float_t px[2];
		Float_t py[2];
		Float_t pz[2];
		Float_t weight;
		UInt_t efftype[3];

		TTree* TPTree;
		Float_t tpx, tpy, tpz, ppx, ppy, ppz, rho;
		UInt_t tpresult, run;
		Int_t numgoodvertices;

		UInt_t MCTRUTH = 1<<0;
		UInt_t ELgPH = 1<<1;
		UInt_t ID = 1<<2;
		UInt_t ISO = 1<<3;
		UInt_t SET_A = 1<<4;
		UInt_t EL = 1<<5;
		UInt_t PH = 1<<6;

		list<GenSimpleParticle> SBasicParticles; 
		vector<GenSimpleParticle*> GenEls; 
		list<OElectron> SElectrons;
		vector<OElectron*> RecEls;

		void SelectGENParticles();
		void SelectRECOParticles();
		
		void FillTruthTree();
		void FillTPTree();
		void PrintTriggerFilter(string trname);

		int TriggerTest(const vector<string>& filternames, OElectron& el);
		vector<string> isoelvec;
		vector<string> dmfvec;
		vector<string> smfvec;
	public:
		DYTPEl(string outfile);
		virtual ~DYTPEl();
		virtual Int_t AnalyseEvent();
		virtual void FileChanged();
		virtual void BeginLoop();
};

#endif
