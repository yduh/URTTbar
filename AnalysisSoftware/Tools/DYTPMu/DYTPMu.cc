#include "DYTPMu.h"
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

DYTPMu::DYTPMu(string outfilename) :
	hists1d("gen"),
	hists2d("gen"),
	testrun(0),
	cmuptmin(20),
	cmuetamax(2.4),
	cmumumassmin(55.),
	cmumumassmax(1000.)
{
	dzetabins = {-2.4, -2.1, -1.6, -1.2, -0.9, -0.6, -0.3, -0.2, 0.0, 0.2, 0.3, 0.6, 0.9, 1.2, 1.6, 2.1, 2.4};
	histfile = new TFile(outfilename.c_str(), "recreate");
	histfile->cd();
	TDirectory* dir_gen = histfile->mkdir("TPMU");
	dir_gen->cd();

	hists2d.AddHist("dzall", dzetabins, dzetabins, "#eta{min}", "#eta{max}");
	hists2d.AddHist("dzpass", dzetabins, dzetabins, "#eta{min}", "#eta{max}");
	hists1d.AddHist("drtrsa", 60, 0., 6., "#DeltaR(sa, trk)", "Events");
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

	isomuvec.push_back("HLT_IsoMu27_v1:hltL3crIsoL1sMu25L1f0L2f10QL3f27QL3trkIsoFiltered0p09");
	dmfvec.push_back("HLT_Mu17_Mu8_DZ_v1:hltL3pfL1sDoubleMu103p5L1f0L2pf0L3PreFiltered8");
	smfvec.push_back("HLT_Mu17_Mu8_DZ_v1:hltL3fL1sDoubleMu103p5L1f0L2f10OneMuL3Filtered17");

}

DYTPMu::~DYTPMu()
{
	histfile->Write();
	histfile->Close();
}

void DYTPMu::SelectGENParticles()
{
	for(UInt_t i = 0 ; i < NumSelectedGenParticles() ; i++)
	{
		GenSelectedParticle mp(GetSelectedGenParticle(i));
		//cout << i << " " << mp.Status() << " " << mp.PDGID() << " " << mp.IndirectMother() << endl;
		//for(int i = 0 ; i < mp.Num_Mother() ; ++i) cout << mp.Mother(i) << " ";
		//cout << "D: ";
		//for(int i = 0 ; i < mp.Num_Daughter() ; ++i) cout << mp.Daughter(i) << " ";
		//cout << endl;
		//		cout << i << " " << mp.Status() << " " << mp.PDGID() << " " << mp.Mother() << " " << mp.IndirectMother() << " " << mp.MoreInfo(GenSelectedParticle::FromWp) << " " <<  mp.MoreInfo(GenSelectedParticle::FromWm) << endl;	

		if(Abs(mp.PDGID()) == 13 && mp.Status() == 1 && Abs(mp.IndirectMother()) == 23)
		{
			if(mp.Pt() > cmuptmin && Abs(mp.Eta()) < cmuetamax)
			{
				SBasicParticles.push_back(mp);
				GenMus.push_back(&SBasicParticles.back());
			}
		}
	}
}

void DYTPMu::SelectRECOParticles()
{

	for(UInt_t i = 0 ; i < NumIOMuons() ; i++)
	{   
		OMuon mu(GetIOMuon(i));
		if(mu.Pt() > cmuptmin && Abs(mu.Eta()) < cmuetamax && mu.ID(1))
		{   
			SMuons.push_back(move(mu));
			RecMus.push_back(&SMuons.back());
		}
	}
}

void DYTPMu::FillTruthTree()
{

	if(GenMus.size() != 2) return;

	efftype[2] = 0;
	Int_t triggerresult = GetTriggerSelection("DMT")->Result();
	if(triggerresult == 1)
	{
		efftype[2] |= 1;
	}
	for(UInt_t n = 0 ; n < GenMus.size() ; n++)
	{
		px[n] = GenMus[n]->Px();
		py[n] = GenMus[n]->Py();
		pz[n] = GenMus[n]->Pz();
		efftype[n] = 0;
		for(UInt_t i = 0 ; i < NumIOMuons() ; i++)
		{
			OMuon mu(GetIOMuon(i));
			if(mu.Pt() < cmuptmin || Abs(mu.Eta()) > cmuetamax) {continue;}
			if(GenMus[n]->DeltaR(mu) < 0.5 && GenMus[n]->DeltaR(mu) < GenMus[(n == 0) ? 1 : 0]->DeltaR(mu))
			{	
				if(mu.IsGoodSA())
				{
					efftype[n] |= SA;
				}

				if(mu.IsGoodTRK() && mu.IsMatched())
				{
					efftype[n] |= ID;
					if(mu.IsISO())
					{
						efftype[n] |= ISO;
						if(TriggerTest(dmfvec, mu) == 1)
						{
							efftype[n] |= TR_Mu17Mu8_L3DMF;
						}
						if(TriggerTest(smfvec, mu) == 1)
						{
							efftype[n] |= TR_Mu17Mu8_L3SMF;
						}
						if(TriggerTest(smfvec, mu) == 1)
						{
							efftype[n] |= TR_ISOMu27;
						}
					}
				}
				break;
			}
		}
		for(UInt_t i = 0 ; i < NumIOTracks() ; i++)
		{
			Track tr(GetIOTrack(i));
			if(tr.Pt() < cmuptmin || Abs(tr.Eta()) > cmuetamax || !TestMuonInnerTrack(tr)) {continue;}
			if(GenMus[n]->DeltaR(tr) < 0.2 && GenMus[n]->DeltaR(tr) < GenMus[(n == 0) ? 1 : 0]->DeltaR(tr))
			{
				efftype[n] |= TRK;
				break;
			}
		}
		EffTree->Fill();
	}
}

void DYTPMu::DZTest()
{
	if(GetTriggerSelection("ISOSMT")->Result() <= 0 || RecMus.size() != 2){return;}
	if((*RecMus[0] + *RecMus[1]).M() < 50.){return;}
	Int_t dmfa = TriggerTest(dmfvec, *RecMus[0]);
	Int_t smfa = TriggerTest(smfvec, *RecMus[0]);
	Int_t dmfb = TriggerTest(dmfvec, *RecMus[1]);
	Int_t smfb = TriggerTest(smfvec, *RecMus[1]);
	sort(RecMus.begin(), RecMus.end(), [](OMuon* A, OMuon* B){return(A->Eta() < B->Eta());});
	if(dmfa == 1 && dmfb == 1 && (smfa == 1 || smfb == 1))
	{
		hists2d["dzall"]->Fill(RecMus[0]->Eta(), RecMus[1]->Eta(), weight);
	}
	if(GetTriggerSelection("DMT")->Result() == 1)
	{
		hists2d["dzpass"]->Fill(RecMus[0]->Eta(), RecMus[1]->Eta(), weight);
	}
}

void DYTPMu::FillTPTree()
{
	vector<OMuon*> tms;
	Int_t tmCharge = (gRandom->Integer(2)*2)-1;
	for(OMuon* mu : RecMus)
	{
		if(mu->Charge() == tmCharge && Abs(mu->Eta()) < cmuetamax && mu->Pt() > cmuptmin && TriggerTest(isomuvec, *mu) == 1 && TriggerTest(smfvec, *mu) == 1)
		{
			tms.push_back(mu);
		}
	}

	if(tms.size() != 1) {return;}
	OMuon* tm = tms[0];
	tpx = tm->Px();
	tpy = tm->Py();
	tpz = tm->Pz();
	tpresult = 0;

	bool probemc = false;
	for(size_t p = 0 ; p < GenMus.size() ; ++p)
	{
		if(tm->DeltaR(*GenMus[p]) < 0.3) probemc = true;
	}

	vector<Track> CutTracks;
	vector<OMuon> CutMuonTracks;

	for(UInt_t i = 0 ; i < NumIOTracks() ; i++)
	{
		Track tr(GetIOTrack(i));
		if(tr.Charge() != tm->Charge() && TestMuonInnerTrack(tr) && (tr + *tm).M() > cmumumassmin && (tr + *tm).M() < cmumumassmax)
		{
			CutTracks.push_back(tr);
		}
	}

	for(UInt_t i = 0 ; i < NumIOMuons() ; i++)
	{
		OMuon mu(GetIOMuon(i));
		if(mu.Charge() != tm->Charge() && (mu + *tm).M() > cmumumassmin && (mu + *tm).M() < cmumumassmax)
		{	
			if(mu.IsGoodSA())
			{
				CutMuonTracks.push_back(mu);
			}
		}
	}

	//TR
	for(size_t i = 0 ; i < CutMuonTracks.size() ; i++)
	{
		tpresult = SA;
		OMuon& samu = CutMuonTracks[i];
		ppx = samu.Px();
		ppy = samu.Py();
		ppz = samu.Pz();

		for(size_t p = 0 ; p < GenMus.size() ; ++p)
		{
			if(probemc && GenMus[p]->DeltaR(samu) < 0.3){tpresult |= MCTRUTH;}
		}
		for(size_t j = 0 ; j < CutTracks.size() ; j++)
		{
			if(samu.DeltaR(CutTracks[j]) < 0.2)
			{
				ppx = CutTracks[j].Px();
				ppy = CutTracks[j].Py();
				ppz = CutTracks[j].Pz();
				tpresult |= TRKgSA;
			}
		}
		TPTree->Fill();
	}

	//SA
	for(size_t i = 0 ; i < CutTracks.size() ; i++)
	{
		tpresult = TRK;
		ppx = CutTracks[i].Px();
		ppy = CutTracks[i].Py();
		ppz = CutTracks[i].Pz();

		for(size_t p = 0 ; p < GenMus.size() ; ++p)
		{
			if(probemc && GenMus[p]->DeltaR(CutTracks[i]) < 0.3) tpresult |= MCTRUTH;
		}

		for(size_t j = 0 ; j < CutMuonTracks.size() ; j++)
		{
			OMuon& sa = CutMuonTracks[j];
			hists1d["drtrsa"]->Fill(CutTracks[i].DeltaR(sa), weight);
			if(CutTracks[i].DeltaR(sa) > 0.2) continue;
			tpresult |= SAgTRK;

			//ID	
			if(sa.IsGoodTRK() && sa.IsMatched())
			{
				tpresult |= ID;
				ppx = sa.Px();
				ppy = sa.Py();
				ppz = sa.Pz();
			}

			//Isolation
			if(sa.IsISO())
			{
				tpresult |= ISO;
			}

			//TRIG
			Int_t dmfresult = TriggerTest(dmfvec, sa);
			if(Abs(dmfresult) == 1)
			{
				tpresult |= TR_Mu17Mu8_L3DMF_test;
			}
			if(dmfresult == 1)
			{
				tpresult |= TR_Mu17Mu8_L3DMF;
			}

			Int_t smfresult = TriggerTest(smfvec, sa);
			if(Abs(smfresult) == 1)
			{
				tpresult |= TR_Mu17Mu8_L3SMF_test;
			}
			if(smfresult == 1)
			{
				tpresult |= TR_Mu17Mu8_L3SMF;
			}

			Int_t isomuresult = TriggerTest(isomuvec, sa);
			if(Abs(isomuresult) == 1)
			{
				tpresult |= TR_ISOMu27_test;
			}
			if(isomuresult == 1)
			{
				tpresult |= TR_ISOMu27;
			}

		}
		TPTree->Fill();
	}

}


void DYTPMu::PrintTriggerFilter(string trname)
{
	cout << "FilterINFO: " << trname << endl;
	for(size_t i = 0 ; i < RecMus[0]->TriggerNames().size() ; i++)
	{
		if(RecMus[0]->TriggerNames()[i].find(trname) != string::npos)
		{	
			cout  << " " << RecMus[0]->TriggerNames()[i];
		}
	}
	cout << endl;
}


Int_t DYTPMu::AnalyseEvent()
{
	if(CheckDuplicate() != 0) {cout << "Duplicated Event" << endl; return(1);}

	GenMus.clear();
	SBasicParticles.clear();
	RecMus.clear();
	SMuons.clear();

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
	if(testrun != Run() && RecMus.size() == 2)
	{
		testrun = Run();
		if(TriggerTest(isomuvec, *RecMus[0]) == 0) cout << Run() <<" ISOMU ERROR" << endl;
		if(TriggerTest(dmfvec, *RecMus[0]) == 0) cout << Run() <<" DMF ERROR" << endl;
		if(TriggerTest(smfvec, *RecMus[0]) == 0) cout << Run() <<" SMF ERROR" << endl;
		//for(int i = 0 ; i < GetNumHLTriggers() ; ++i)
		//{
		//	cout << i << ": " << GetHLTNames(i) << " " << GetHLTPrescale(i) << endl;
		//}
		//PrintTriggerFilter("HLT_IsoMu24_eta2p1_v");
		PrintTriggerFilter("HLT_IsoMu27_v");
		PrintTriggerFilter("HLT_Mu17_Mu8_DZ_v");
	}
	FillTPTree();
	DZTest();
	return(1);
}

void DYTPMu::FileChanged()
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


void DYTPMu::BeginLoop()
{
	//SetupPileupWeighting("pu.root", "pu", false);
}

int DYTPMu::TriggerTest(const vector<string>& filternames, OMuon& mu)
{
	int defined = 0;
	for(const string& fn : filternames)
	{
		int result = mu.Trigger(fn);
		if(result == 1) {return(1);}
		if(result == -1) {defined = -1;}
	}
	return(defined);
}
