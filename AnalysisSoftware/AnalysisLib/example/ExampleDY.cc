#include "ExampleDY.h"
#include <iostream>
#include <sstream>

#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TFile.h>

using namespace std;
using namespace TMath;

ExampleDY::ExampleDY(string outfilename) : recoplots1d("reco")
{
	histfile = new TFile(outfilename.c_str(), "recreate");
	histfile->cd();
	recoplots1d.AddHist("Mmumu", 1000, 40, 1040, "M_{#mu#mu} (GeV)", "Events"); 
}

ExampleDY::~ExampleDY()
{
	histfile->Write();
	histfile->Close();
}

void ExampleDY::SelectRECOParticles()
{
	for(UInt_t i = 0 ; i < NumIOMuons() ; i++)
	{   
		OMuon mu(GetIOMuon(i));
		if(mu.Pt() > 20. && Abs(mu.Eta()) < 2.4 && mu.ID(1))
		{   
			SMuons.push_back(move(mu));
			AllMuons.push_back(&SMuons.back());
		}
	}
	//cout << NumIOElectrons() << endl;
	//cout << NumIOTracks() << endl;
	//cout << NumIOPFJets() << endl;
	//cout << NumAllGenParticles() << endl;
	//cout << NumPrimaryVertexs() << endl;
	//cout << GetGenInfo(0).PDGID1() << endl;
}

Int_t ExampleDY::AnalyseEvent()
{
	//LoadIOPhoton(false);
	//LoadIOMuon(false);
	SMuons.clear();

	AllMuons.clear();

	//cout << Run() << " " << LumiBlock() << " " << (int)Number() << endl;
	//cout << GetTriggerSelection("DMT")->Result() << " " << GetTriggerSelection("DET")->Result() << endl;
	if(GetTriggerSelection("DMT")->Result() == 1 && GetTriggerSelection("DET")->Result() == -1)
	{
		SelectRECOParticles();
		//cout << AllMuons.size() << endl;
		if(AllMuons.size() == 2)
		{
			TLorentzVector Z(*AllMuons[0] + *AllMuons[1]);
			recoplots1d["Mmumu"]->Fill(Z.M());
		}
	}

	return(1);
}

