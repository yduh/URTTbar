#include "DYTPEl.h"
#include <iostream>
#include <sstream>

#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>

using namespace std;
using namespace TMath;

DYTPEl::DYTPEl(string outfilename) :
	hists1d("gen"),
	hists2d("gen"),
	testrun(0),
	celptmin(20),
	celetamax(2.4),
	celelmassmin(55.),
	celelmassmax(1000.)
{
	histfile = new TFile(outfilename.c_str(), "recreate");
	histfile->cd();
	TDirectory* dir_gen = histfile->mkdir("TPEL");
	dir_gen->cd();

	EffTree = new TTree("EffTree", "EffTree", 1);
	EffTree->Branch("px", px, "px[2]/F");
	EffTree->Branch("py", py, "py[2]/F");
	EffTree->Branch("pz", pz, "pz[2]/F");
	EffTree->Branch("efftype", efftype, "efftype[3]/i");
	EffTree->Branch("weight", &weight, "weight/F");
	EffTree->Branch("numgoodvertices", &numgoodvertices, "numgoodvertices/I");
	EffTree->Branch("rho", &rho, "rho/F");

	TPTree = new TTree("TPTree", "TPTree", 1);
	TPTree->Branch("tpx", &tpx, "tpx/F");
	TPTree->Branch("tpy", &tpy, "tpy/F");
	TPTree->Branch("tpz", &tpz, "tpz/F");
	TPTree->Branch("ppx", &ppx, "ppx/F");
	TPTree->Branch("ppy", &ppy, "ppy/F");
	TPTree->Branch("ppz", &ppz, "ppz/F");
	TPTree->Branch("rho", &rho, "rho/F");
	TPTree->Branch("numgoodvertices", &numgoodvertices, "numgoodvertices/I");
	TPTree->Branch("weight", &weight, "weight/F");
	TPTree->Branch("run", &run, "run/i");
	TPTree->Branch("tpresult", &tpresult, "tpresult/i");

	isoelvec.push_back("HLT_Ele27_WP85_Gsf_v1:hltL1EG25Ele27WP85GsfTrackIsoFilter");
	//dmfvec.push_back("HLT_Mu17_Mu8_DZ_v1:hltL3pfL1sDoubleMu103p5L1f0L2pf0L3PreFiltered8");
	//smfvec.push_back("HLT_Mu17_Mu8_DZ_v1:hltL3fL1sDoubleMu103p5L1f0L2f10OneMuL3Filtered17");

}

DYTPEl::~DYTPEl()
{
	histfile->Write();
	histfile->Close();
}

void DYTPEl::SelectGENParticles()
{
	for(UInt_t i = 0 ; i < NumSelectedGenParticles() ; i++)
	{
		GenSelectedParticle mp(GetSelectedGenParticle(i));

		if(Abs(mp.PDGID()) == 11 && mp.Status() == 1 && Abs(mp.IndirectMother()) == 23)
		{
			if(mp.Pt() > celptmin && Abs(mp.Eta()) < celetamax)
			{
				SBasicParticles.push_back(mp);
				GenEls.push_back(&SBasicParticles.back());
			}
		}
	}
}

void DYTPEl::SelectRECOParticles()
{

	for(UInt_t i = 0 ; i < NumIOElectrons() ; i++)
	{   
		OElectron el(GetIOElectron(i));
		if(el.Pt() > celptmin && Abs(el.Eta()) < celetamax && el.ID(1))
		{   
			SElectrons.push_back(move(el));
			RecEls.push_back(&SElectrons.back());
		}
	}
}

void DYTPEl::FillTruthTree()
{
	if(GenEls.size() != 2) return;

	efftype[2] = 0;
	Int_t triggerresult = GetTriggerSelection("DET")->Result();
	if(triggerresult == 1)
	{
		efftype[2] |= 1;
	}
	for(UInt_t n = 0 ; n < GenEls.size() ; n++)
	{
		px[n] = GenEls[n]->Px();
		py[n] = GenEls[n]->Py();
		pz[n] = GenEls[n]->Pz();
		efftype[n] = 0;
		for(UInt_t i = 0 ; i < NumIOElectrons() ; i++)
		{
			OElectron el(GetIOElectron(i));
			if(el.Pt() < celptmin || Abs(el.Eta()) > celetamax) {continue;}
			if(GenEls[n]->DeltaR(el) < 0.2 && GenEls[n]->DeltaR(el) < GenEls[(n == 0) ? 1 : 0]->DeltaR(el))
			{	
				efftype[n] |= EL;

				if(el.ID(1, 0))
				{
					efftype[n] |= ID;
				}
				if(el.ID(1, 1))
				{
					efftype[n] |= ISO;
					if(TriggerTest(isoelvec, el) == 1)
					{
						efftype[n] |= SET_A;
					}
				}
				break;
			}
		}
		for(UInt_t i = 0 ; i < NumIOPhotons() ; i++)
		{
			OPhoton ph(GetIOPhoton(i));
			if(ph.Pt() < celptmin || Abs(ph.Eta()) > celetamax) {continue;}
			if(GenEls[n]->DeltaR(ph) < 0.2 && GenEls[n]->DeltaR(ph) < GenEls[(n == 0) ? 1 : 0]->DeltaR(ph))
			{
				efftype[n] |= PH;
				break;
			}
		}
		EffTree->Fill();
	}
}

void DYTPEl::FillTPTree()
{
	vector<OElectron*> tms;
	Int_t tmCharge = (gRandom->Integer(2)*2)-1;
	for(OElectron* el : RecEls)
	{
		if(el->Charge() == tmCharge && Abs(el->Eta()) < celetamax && el->Pt() > celptmin && TriggerTest(isoelvec, *el) == 1)
		{
			tms.push_back(el);
		}
	}

	if(tms.size() != 1) {return;}
	OElectron* tm = tms[0];
	tpx = tm->Px();
	tpy = tm->Py();
	tpz = tm->Pz();
	tpresult = 0;

	bool probemc = false;
	for(size_t p = 0 ; p < GenEls.size() ; ++p)
	{
		if(tm->DeltaR(*GenEls[p]) < 0.2) probemc = true;
	}

	vector<OPhoton> CutPhotons;
	vector<OElectron> CutOElectrons;

	for(UInt_t i = 0 ; i < NumIOPhotons() ; i++)
	{
		OPhoton ph(GetIOPhoton(i));
		if(ph.Pt() > celptmin && ph.Clean() && (ph + *tm).M() > celelmassmin && (ph + *tm).M() < celelmassmax)
		{
			CutPhotons.push_back(ph);
		}
	}

	for(UInt_t i = 0 ; i < NumIOElectrons() ; i++)
	{
		OElectron el(GetIOElectron(i));
		if(el.Charge() != tm->Charge() && (el + *tm).M() > celelmassmin && (el + *tm).M() < celelmassmax)
		{	
		}
	}

}


void DYTPEl::PrintTriggerFilter(string trname)
{
	cout << "FilterINFO: " << trname << endl;
	for(size_t i = 0 ; i < RecEls[0]->TriggerNames().size() ; i++)
	{
		if(RecEls[0]->TriggerNames()[i].find(trname) != string::npos)
		{	
			cout  << " " << RecEls[0]->TriggerNames()[i];
		}
	}
	cout << endl;
}


Int_t DYTPEl::AnalyseEvent()
{
	if(CheckDuplicate() != 0) {cout << "Duplicated Event" << endl; return(1);}

	GenEls.clear();
	SBasicParticles.clear();
	RecEls.clear();
	SElectrons.clear();

	numgoodvertices = NumPrimaryVertexs();
	run = Run();
	rho = AK5PFRho();
	weight = 1.;
	if(IsMC())
	{
		if(NumGenInfos() == 1)
		{
			weight = GetGenInfo(0).Weight()/Abs(GetGenInfo(0).Weight());
		}
		SelectGENParticles();
		FillTruthTree();
	}
	SelectRECOParticles();
	if(testrun != Run() && RecEls.size() == 2)
	{
		testrun = Run();
		if(TriggerTest(isoelvec, *RecEls[0]) == 0) cout << Run() <<" ISOMU ERROR" << endl;
		if(TriggerTest(dmfvec, *RecEls[0]) == 0) cout << Run() <<" DMF ERROR" << endl;
		if(TriggerTest(smfvec, *RecEls[0]) == 0) cout << Run() <<" SMF ERROR" << endl;
		//for(int i = 0 ; i < GetNumHLTriggers() ; ++i)
		//{
		//	cout << i << ": " << GetHLTNames(i) << " " << GetHLTPrescale(i) << endl;
		//}
		PrintTriggerFilter("HLT_IsoMu24_eta2p1_v");
		PrintTriggerFilter("HLT_IsoMu27_v");
		PrintTriggerFilter("HLT_Mu17_Mu8_DZ_v");
	}
	FillTPTree();
	return(1);
}

void DYTPEl::FileChanged()
{

	cout << "File Changed" << endl;
	//LoadPrimaryVertex(false);
	//LoadIOEventInfo(false);
	//LoadIOPFJet(false);
	//LoadGenInfo(false);
	//LoadIOMET(false);
	//LoadIOGenAK4Jet(false);
	//LoadSelectedGenParticle(false);
	//LoadAllGenParticle(false);
	//LoadIOElectron(false);
	//LoadIOTrack(false);
	//LoadIOBeamSpot(false);
	//LoadIOPhoton(false);
}


void DYTPEl::BeginLoop()
{
	//SetupPileupWeighting("pu.root", "pu", false);
}

int DYTPEl::TriggerTest(const vector<string>& filternames, OElectron& el)
{
	int defined = 0;
	for(const string& fn : filternames)
	{
		int result = el.Trigger(fn);
		if(result == 1) {return(1);}
		if(result == -1) {defined = -1;}
	}
	return(defined);
}
