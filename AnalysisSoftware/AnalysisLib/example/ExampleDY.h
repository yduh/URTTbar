#ifndef EXAMPLEDY
#define EXAMPLEDY
#include <Analyse.h>
#include <OMuon.h>
#include <OJet.h>
#include <helper.h>
#include <vector>
#include <list>

class TFile;

using namespace std;

class ExampleDY : public Analyse
{
	private:
		TFile* histfile;
		TH1DCollection recoplots1d;
		list<OMuon> SMuons;
		list<OJet> SJets;

		vector<OMuon*> AllMuons;
		vector<OJet*> AllJets;

		void SelectRECOParticles();
	public:
		ExampleDY(string outfile);
		virtual ~ExampleDY();
		virtual Int_t AnalyseEvent();
};

#endif
