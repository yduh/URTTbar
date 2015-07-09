#include "TTSemi.h"
#include "NeutrinoSolver.h"
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

TTSemi::TTSemi(string outfilename) :
	gen1d("gen"),
	gen2d("gen"),
	reco1d("reco"),
	reco2d("reco"),
	truth1d("truth"),
	truth2d("truth"),
	ttp_right("right"),
	ttp_wrong("wrong"),
	ttp_semi("semi"),
	ttp_other("other"),
	ttp_truth("truth"),
	ttp_all("all"),
	ttp_right_imp("right_imp"),
	ttp_wrong_imp("wrong_imp"),
	ttp_semi_imp("semi_imp"),
	ttp_other_imp("other_imp"),
	ttp_all_imp("all_imp"),
	cwjetptmin(25),
	cnbtag(2)
{
	//binning_pttoprec = {0., 35};
	//binning_pttopgen = {};
	histfile = new TFile(outfilename.c_str(), "recreate");
	histfile->cd();
	TDirectory* dir_gen = histfile->mkdir("GEN");
	dir_gen->cd();
	gen1d.AddHist("JM1", 1000, 0., 5000., "JM1", "Events");
	gen1d.AddHist("TYP", 3, 0., 3., "Decay TYP", "Events");
	gen2d.AddHist("wjets_eta", 100, 0., 5., 100, 0, 5, "W-jet #eta_{min}", "W-jet #eta_{max}");
	gen2d.AddHist("bjets_eta", 100, 0., 5., 100, 0, 5, "b-jet #eta_{min}", "b-jet #eta_{max}");
	gen2d.AddHist("wjets_pt", 200, 0, 400, 200, 0., 400, "W-jet p_{T,min} (GeV)", "W-jet p_{T,max} (GeV)");
	gen2d.AddHist("bjets_pt", 200, 0, 400, 200, 0., 400, "b-jet p_{T,min} (GeV)", "b-jet p_{T,max} (GeV)");
	gen1d.AddHist("bjets_dr", 100, 0, 5, "b-jets #DeltaR", "Events");
	gen1d.AddHist("wjets_dr", 100, 0, 5, "W-jets #DeltaR", "Events");
	gen1d.AddHist("wJets_M", 200, 0, 200, "gen W-jet mass M_{W} (GeV)", "Events");
	gen1d.AddHist("jetwidth", 200, 0, 2., "jetwidth", "Events");
	gen2d.AddHist("dr_ptfraction", 400, 0, 4., 200, 0., 2., "dR", "ptfraction");


	TDirectory* dir_truth = histfile->mkdir("TRUTH");
	dir_truth->cd();
	jestree = new TTree("jestree", "jestree", 1);
	jestree->Branch("jwa", jwa, "jwa[3]/F");
	jestree->Branch("jwb", jwb, "jwb[3]/F");
	truejestree = new TTree("truejestree", "truejestree", 1);
	truejestree->Branch("jwa", jwa, "jwa[3]/F");
	truejestree->Branch("jwb", jwb, "jwb[3]/F");
	falsejestree = new TTree("falsejestree", "falsejestree", 1);
	falsejestree->Branch("jwa", jwa, "jwa[3]/F");
	falsejestree->Branch("jwb", jwb, "jwb[3]/F");
	truth1d.AddHist("pu", 100, 0., 100., "pu", "Events");
	truth1d.AddHist("pu_weighted", 100, 0., 100., "pu_weighted", "Events");
	truth2d.AddHist("btag2d_true", 100, 0., 1., 100, 0., 1., "btag", "btag");
	truth1d.AddHist("btag_true", 100, 0., 1., "btag", "Events");
	truth1d.AddHist("btag_wrong", 100, 0., 1., "btag", "Events");
	truth1d.AddHist("btagtest", 10, 0., 10., "btagtest", "Events");
	truth2d.AddHist("test_dPhi", 100, 0., 10000., 100, -2., 2., "min chi2", "#Delta#Phi");
	truth2d.AddHist("test_dPhiMET", 100, 0., 10000., 100, -2., 2., "min chi2", "#Delta#Phi");
	truth1d.AddHist("otherjets_pt", 500, 0., 500., "p_{T} (GeV)", "Events");
	truth1d.AddHist("otherjets_sep", 100, 0., 5., "min sep", "Events");
	truth2d.AddHist("wjets_sep", 100, 0., 5., 160, -4., 4., "min sep", "(p_{T}(rec) - p_{T}(gen))/p_{T}(gen)");
	truth2d.AddHist("bjets_sep", 100, 0., 5., 160, -4., 4., "min sep", "(p_{T}(rec) - p_{T}(gen))/p_{T}(gen)");
	truth1d.AddHist("wmass_lowsep", 75, 0., 150, "low separation M(jj) (GeV)", "Events");
	truth1d.AddHist("wmass_highsep", 75, 0., 150, "high separation M(jj) (GeV)", "Events");
	truth2d.AddHist("jettest", 3, 0., 3., 3, 0., 3., "num b-jets", "num w-jets");
	truth2d.AddHist("Jetstt_JetsAll", 5, 0., 5., 20, 0., 20., "jet tt-system", "all jets");
	truth1d.AddHist("dRNu_right", 200, 0., 10., "#DeltaR(#nu_{gen}, #nu_{rec})", "Events");
	truth1d.AddHist("dPtNu_right", 200, -2., 2., "#Deltap_{T}/p_{T}", "Events");
	truth1d.AddHist("dRNuMet_right", 200, 0., 10., "#DeltaR(#nu_{gen}, #nu_{rec})", "Events");
	truth1d.AddHist("dPtNuMet_right", 200, -2., 2., "#Deltap_{T}/p_{T}", "Events");
	truth1d.AddHist("dPzNuNuGen_right", 500, -5., 5., "#Deltap_{z}/p_{z}", "Events");
	truth2d.AddHist("DRngenrec_pt", 50, 0., 500., 100, -2., 2., "p_{T}(#nu) (GeV)", "#Delta#Phi(#nu_{gen}, #nu_{rec})");
	truth2d.AddHist("Dptgenrec_pt", 50, 0., 500., 100, -4., 4., "p_{T}(#nu) (GeV)", "#DeltaP_{t}(#nu_{gen}, #nu_{rec})");
	truth2d.AddHist("Dpzgenrec_pt", 50, 0., 500., 100, -4., 4., "p_{T}(#nu) (GeV)", "#DeltaP_{z}(#nu_{gen}, #nu_{rec})");
	truth2d.AddHist("DRngenrec_pt_old", 500, 0., 500., 800, 0., 8., "p_{T}(#nu) (GeV)", "#DeltaR(#nu_{gen}, #nu_{rec})");
	truth2d.AddHist("Wmasshad_tmasshad_right", 500, 0., 500., 500, 0., 500, "M(W) (GeV)", "M(t) (GeV)");
	truth2d.AddHist("Wmasshad_tmasshad_wrong", 500, 0., 500., 500, 0., 500, "M(W) (GeV)", "M(t) (GeV)");
	truth2d.AddHist("Wmasshad_tmasshad_rightw", 500, 0., 500., 500, 0., 500, "M(W) (GeV)", "M(t) (GeV)");
	truth2d.AddHist("Wmasshad_tmasshad_wrongw", 500, 0., 500., 500, 0., 500, "M(W) (GeV)", "M(t) (GeV)");
	truth2d.AddHist("jetquality_other", 100, 0., 5., 110, 0., 1.1, "p_{T}(jet)/p_{T}(parton)", "1-wrong vertex fraction");
	truth2d.AddHist("jetnconst_other", 100, 0., 5., 100, 0., 100, "p_{T}(jet)/p_{T}(parton)", "const");
	truth2d.AddHist("jetquality_signal", 100, 0., 5., 110, 0., 1.1, "p_{T}(jet)/p_{T}(parton)", "1-wrong vertex fraction");
	truth2d.AddHist("jetnum_highptjets", 20, 0., 20., 20, 0., 20., "num_jets", "num jets higher pt");
	truth1d.AddHist("btagtest_wrong", 1000, -100, 100., "-Log(p) btag-test", "Events");
	truth1d.AddHist("masstest_wrong", 1000, -100, 100., "-Log(p) mass-test", "Events");
	truth1d.AddHist("nstest_wrong", 200, 0, 20., "neutrino-test", "Events");
	truth1d.AddHist("nschi_wrong", 520, -2, 50., "#chi2 neutrino-test", "Events");
	truth1d.AddHist("comtest_wrong", 1000, -100, 100., "-Log(p)", "Events");
	truth1d.AddHist("btagtest_right", 1000, -100, 100., "-Log(p) btag-test", "Events");
	truth1d.AddHist("masstest_right", 1000, -100, 100., "-Log(p) mass-test", "Events");
	truth1d.AddHist("nstest_right", 200, 0, 20., "neutrino-test", "Events");
	truth1d.AddHist("nschi_right", 520, -2, 50., "#chi2 neutrino-test", "Events");
	truth1d.AddHist("comtest_right", 1000, -100, 100., "-Log(p)", "Events");
	truth1d.AddHist("event_loss", 20, 0, 20., "loss", "Events");
	truth2d.AddHist("response_all", 26, -20., 500., 26, -20., 500., "gen", "reco");
	truth2d.AddHist("response_right", 26, -20., 500., 26, -20., 500., "gen", "reco");
	ttp_right.Init();
	ttp_wrong.Init();
	ttp_semi.Init();
	ttp_other.Init();
	ttp_right_imp.Init();
	ttp_wrong_imp.Init();
	ttp_semi_imp.Init();
	ttp_other_imp.Init();
	ttp_truth.Init();

	TDirectory* dir_reco = histfile->mkdir("RECO");
	dir_reco->cd();
	ttp_all.Init();
	ttp_all_imp.Init();

	ttsolver.Init("Prob.root", false);
}

TTSemi::~TTSemi()
{
	histfile->Write();
	histfile->Close();
}




void TTSemi::SelectGENParticles()
{

	int leps = 0;

	if(NumAllGenParticles() != 0)
	{
		vector<GenBasicParticle> finaldecays;
		for(UInt_t i = 0 ; i < NumAllGenParticles() ; i++)
		{
			GenBasicParticle mp(GetAllGenParticle(i));
			if(mp.Status() == 1) {finaldecays.push_back(mp);}

		}
		for(UInt_t i = 0 ; i < NumAllGenParticles() ; i++)
		{
			GenBasicParticle mp(GetAllGenParticle(i));
			//cout << i << " " << mp.Status() << " " << mp.PDGID() << endl;
			//for(int i = 0 ; i < mp.Num_Mother() ; ++i) cout << mp.Mother(i) << " ";
			//cout << endl;
			if(mp.Status() == 22  && Abs(mp.PDGID()) == 24)
			{
				//for(int i = 0 ; i < mp.Num_Daughter() ; ++i) cout << mp.GetDaughter(i).PDGID() << " ";
				//cout << endl;
				vector<GenBasicParticle> decayprods;
				mp.DecayProducts(decayprods);
				for(GenBasicParticle& dp : decayprods)
				{
					if(Abs(dp.PDGID()) < 6)
					{
						SBasicParticles.push_back(dp);
						GenWPartons.push_back(&SBasicParticles.back());
						//vector<GenBasicParticle> finaldecays;
						//dp.StableDecayProducts(finaldecays);
						//cout << finaldecays.size() << endl;
						sort(finaldecays.begin(), finaldecays.end(), [&](GenBasicParticle& A, GenBasicParticle& B) {return(dp.DeltaR(A) < dp.DeltaR(B));});
						double ptsum=0.;
						for(GenBasicParticle& p : finaldecays)
						{
							//cout << p.PDGID() << " " << p.Status() << " " << p.Pt() << endl;
							ptsum+=p.Pt();
							gen2d["dr_ptfraction"]->Fill(dp.DeltaR(p), ptsum/dp.Pt());
							//if(ptsum>=dp.Pt())
							//{
							//	gen1d["jetwidth"]->Fill(dp.DeltaR(p));
							//	break;
							//}
						}
						//cout << ptsum << " " << dp.Pt() << endl;
						continue;
					}
					if(Abs(dp.PDGID()) == 11 || Abs(dp.PDGID()) == 13)
					{
						SBasicParticles.push_back(dp);
						GenCL.push_back(&SBasicParticles.back());
						continue;
					}	
					if(Abs(dp.PDGID()) == 12 || Abs(dp.PDGID()) == 14)
					{
						SBasicParticles.push_back(dp);
						GenN.push_back(&SBasicParticles.back());
						leps++;
						continue;
					}
					if(Abs(dp.PDGID()) == 15)
					{
						leps++;
					}
				}
			}


			if(mp.PDGID() == 5 && mp.HasMother(6))
			{
				SBasicParticles.push_back(mp);
				GenB = &SBasicParticles.back();
			}
			if(mp.PDGID() == -5 && mp.HasMother(-6))
			{
				SBasicParticles.push_back(mp);
				GenBBar = &SBasicParticles.back();
			}

		}
	}
	else if(NumSelectedGenParticles() != 0)
	{
		for(UInt_t i = 0 ; i < NumSelectedGenParticles() ; i++)
		{
			GenSelectedParticle mp(GetSelectedGenParticle(i));
			//cout << i << " " << mp.Status() << " " << mp.PDGID() << " " << mp.Num_Mother() << " M: ";
			//for(int i = 0 ; i < mp.Num_Mother() ; ++i) cout << mp.Mother(i) << " ";
			//cout << "D: ";
			//for(int i = 0 ; i < mp.Num_Daughter() ; ++i) cout << mp.Daughter(i) << " ";
			//cout << endl;
			//		cout << i << " " << mp.Status() << " " << mp.PDGID() << " " << mp.Mother() << " " << mp.IndirectMother() << " " << mp.MoreInfo(GenSelectedParticle::FromWp) << " " <<  mp.MoreInfo(GenSelectedParticle::FromWm) << endl;	

			if(mp.Status() == 1 && Abs(mp.IndirectMother()) == 24)
			{
				if(Abs(mp.PDGID()) == 11 || Abs(mp.PDGID()) == 13)
				{
					SBasicParticles.push_back(mp);
					GenCL.push_back(&SBasicParticles.back());
				}
				if(Abs(mp.PDGID()) == 12 || Abs(mp.PDGID()) == 14)
				{
					SBasicParticles.push_back(mp);
					GenN.push_back(&SBasicParticles.back());
					leps++;
				}
				if(Abs(mp.PDGID()) == 16)
				{
					leps++;
				}
			}

			if(mp.Status() == 23)
			{
				if(mp.PDGID() == 5 && Abs(mp.Mother()) != 24)
				{
					SBasicParticles.push_back(mp);
					GenB = &SBasicParticles.back();
				}
				else if(mp.PDGID() == -5 && Abs(mp.Mother()) != 24)
				{
					SBasicParticles.push_back(mp);
					GenBBar = &SBasicParticles.back();
				}
			}
			if(Abs(mp.PDGID()) < 6 && Abs(mp.Mother()) == 24)
			{
				SBasicParticles.push_back(mp);
				GenWPartons.push_back(&SBasicParticles.back());
			}

		}
	}

	FULLHAD = false;
	SEMILEP = false;
	SEMILEPACC = false;
	FULLLEP = false;
	if(leps == 0 && GenWPartons.size() == 4)
	{
		FULLHAD = true;
		gen1d["TYP"]->Fill(0.5);
	}
	else if(leps == 1 && GenWPartons.size() == 2)
	{
		gen1d["TYP"]->Fill(1.5);
	}
	else if(leps == 2 && GenWPartons.size() == 0) 
	{
		FULLLEP = true;
		gen1d["TYP"]->Fill(2.5);
	}

	if(GenWPartons.size() == 2 && GenCL.size() == 1 && GenN.size() == 1 && GenB != 0 && GenBBar != 0) //no tau
	{
		SEMILEP = true;
		if(GenCL[0]->PDGID() > 0) {GenBl = GenBBar; GenBh = GenB;}
		else if(GenCL[0]->PDGID() < 0) {GenBh = GenBBar; GenBl = GenB;}

		if(Abs(GenCL[0]->Eta()) < 2.4 && GenCL[0]->Pt() > 30. && Abs(GenWPartons[0]->Eta()) < 2.4 && Abs(GenWPartons[1]->Eta()) < 2.4 && Abs(GenB->Eta()) < 2.4 && Abs(GenBBar->Eta()) < 2.4)
		{
			gen2d["wjets_pt"]->Fill(Min(GenWPartons[0]->Pt(), GenWPartons[1]->Pt()), Max(GenWPartons[0]->Pt(), GenWPartons[1]->Pt()));
			gen2d["bjets_pt"]->Fill(Min(GenB->Pt(), GenBBar->Pt()), Max(GenB->Pt(), GenBBar->Pt()));
			gen1d["wjets_dr"]->Fill(GenWPartons[0]->DeltaR(*GenWPartons[1]));
			gen1d["bjets_dr"]->Fill(GenB->DeltaR(*GenBBar));
			if(Min(GenWPartons[0]->Pt(), GenWPartons[1]->Pt()) > cwjetptmin && Max(GenWPartons[0]->Pt(), GenWPartons[1]->Pt()) > 30. && GenB->Pt() > 30. && GenBBar->Pt() > 30.)
			{
				SEMILEPACC = true;
				for(size_t gj = 0 ; gj < NumIOGenAK4Jets() ; ++gj)
				{
					GenJet genjet(GetIOGenAK4Jet(gj));
					if(GenWPartons[0]->DeltaR(genjet) < 0.4)
					{
						SGenJets.push_back(genjet);
						GenWJets.push_back(&(SGenJets.back()));
					}
					if(GenWPartons[1]->DeltaR(genjet) < 0.4)
					{
						SGenJets.push_back(genjet);
						GenWJets.push_back(&(SGenJets.back()));
					}
				}
			if(GenWJets.size() == 2){ gen1d["wJets_M"]->Fill((*GenWJets[0] + *GenWJets[1]).M());}
			}
		}
		gen2d["wjets_eta"]->Fill(Min(Abs(GenWPartons[0]->Eta()), Abs(GenWPartons[1]->Eta())), Max(Abs(GenWPartons[0]->Eta()), Abs(GenWPartons[1]->Eta())));
		gen2d["bjets_eta"]->Fill(Min(Abs(GenB->Eta()), Abs(GenBBar->Eta())), Max(Abs(GenB->Eta()), Abs(GenBBar->Eta())));
	}
	else
	{
		GenB = 0;
		GenBBar = 0;
		GenBl = 0;
		GenBh = 0;
		GenCL.clear();
		GenN.clear();
		GenWPartons.clear();
	}
	//cout << "WPartons: " << GenWPartons.size() << endl;
	//if(GenWPartons.size() == 0)
	//{
	//	for(UInt_t i = 0 ; i < NumAllGenParticles() ; i++)
	//	{
	//		GenSelectedParticle mp(GetAllGenParticle(i));
	//		if(Abs(mp.PDGID()) == 24)
	//		{
	//			cout << i << " " << mp.Status() << " " << mp.PDGID() << " " << mp.Num_Mother() << " M: ";
	//			for(int i = 0 ; i < mp.Num_Mother() ; ++i) cout << mp.GetMother(i).PDGID() << " ";
	//			cout << "D: ";
	//			for(int i = 0 ; i < mp.Num_Daughter() ; ++i) cout << mp.GetDaughter(i).PDGID() << " ";
	//			cout << endl;
	//		}
	//	}
	//}


}

void TTSemi::SelectRECOParticles()
{

	for(UInt_t i = 0 ; i < NumIOMuons() ; i++)
	{   
		OMuon mu(GetIOMuon(i));
		if(mu.Pt() > 15. && Abs(mu.Eta()) < 2.4 && mu.ID(1))
		{   
			SMuons.push_back(move(mu));
			LooseMuons.push_back(&SMuons.back());
			if(mu.Pt() > 30. && Abs(mu.Eta()) < 2.4 && mu.ID(1))
			{   
				AllMuons.push_back(&SMuons.back());
			}
		}
	}

	for(UInt_t i = 0 ; i < NumIOElectrons() ; i++)
	{   
		OElectron el(GetIOElectron(i));
		if(el.Pt() > 15. && el.ID(1))
		{   
			SElectrons.push_back(move(el));
			LooseElectrons.push_back(&SElectrons.back());
			if(el.Pt() > 30. && el.ID(1))
			{
				AllElectrons.push_back(&SElectrons.back());
			}
		}
	}

	for(UInt_t i = 0 ; i < NumIOPhotons() ; i++)
	{   
		OPhoton ph(GetIOPhoton(i));
		if(ph.Pt() > 15. && ph.ID(1))
		{   
			SPhotons.push_back(move(ph));
			AllPhotons.push_back(&SPhotons.back());
		}
	}

	
	for(UInt_t i = 0 ; i < NumIOPFJets() ; i++)
	{   
		OJet jet(GetIOPFJet(i));
		double smear = 1.;//gRandom->Gaus(1.,0.1);
		double shift = 1.;
		jet.SetPxPyPzE(jet.Px()*smear*shift, jet.Py()*smear*shift, jet.Pz()*smear*shift, jet.E()*smear*shift);
		if(jet.Pt() > cwjetptmin && jet.Pt() < 100000. && Abs(jet.Eta()) < 2.4 && jet.Clean(LooseMuons, LooseElectrons, AllPhotons))// && jet.PtFractionWrongPrimaryVertex() > 0.5)
		{   
			SJets.push_back(move(jet));
			AllJets.push_back(&SJets.back());
		}
	}
	
	if(SEMILEP)
	{
		sort(AllJets.begin(), AllJets.end(), [&](OJet* A, OJet* B){return(A->Pt() > B->Pt());});
		double ptb = 0.;
		double ptbbar = 0.;
		vector<double> ptmax(4, 0.);
		for(UInt_t i = 0 ; i < AllJets.size() ; i++)
		{
			if(AllJets[i]->DeltaR(*GenB) < 0.4 && AllJets[i]->Pt() > ptb)
			{
				ptb = AllJets[i]->Pt();
				BJet = AllJets[i];	
				if(GenCL[0]->PDGID() < 0)
				{
					BlJet = AllJets[i];
				}
				else if(GenCL[0]->PDGID() > 0)
				{
					BhJet = AllJets[i];
				}
				goto nextjet;
			}
			if(AllJets[i]->DeltaR(*GenBBar) < 0.4 && AllJets[i]->Pt() > ptbbar)
			{
				ptbbar = AllJets[i]->Pt();
				BBarJet = AllJets[i];	
				if(GenCL[0]->PDGID() < 0)
				{
					BhJet = AllJets[i];
				}
				else if(GenCL[0]->PDGID() > 0)
				{
					BlJet = AllJets[i];
				}
				goto nextjet;
			}
			for(UInt_t j = 0 ; j < GenWPartons.size() ; j++)
			{
				if(AllJets[i]->DeltaR(*GenWPartons[j]) < 0.4 && ptmax[j] < AllJets[i]->Pt())
				{
					ptmax[j] = AllJets[i]->Pt();
					RecWJets.push_back(AllJets[i]);
					goto nextjet;
				}
			}
			RecOtherJets.push_back(AllJets[i]);
nextjet: continue;
		}
	}
	//cout << NumIOElectrons() << endl;
	//cout << NumIOTracks() << endl;
	//cout << NumIOPFJets() << endl;
	//cout << NumAllGenParticles() << endl;
	//cout << NumPrimaryVertexs() << endl;
	//cout << GetGenInfo(0).PDGID1() << endl;

}

void TTSemi::PlotGEN()
{
}



Int_t TTSemi::AnalyseEvent()
{

	if(CheckDuplicate() != 0) {cout << "Duplicated Event" << endl; return(1);}
	//double weight =  GetPileupWeight();
	//truth1d["pu"]->Fill(GetGenInfo(0).NumPUInteractions());
	//truth1d["pu_weighted"]->Fill(GetGenInfo(0).NumPUInteractions(), weight);
	SGenJets.clear(); 
	GenWJets.clear(); 
	GenBJets.clear(); 

	GenWPartons.clear();
	GenCL.clear();
	GenN.clear();
	GenB = 0;
	GenBBar = 0;
	GenBl = 0;
	GenBh = 0;
	SBasicParticles.clear();

	SMuons.clear();
	LooseMuons.clear();
	AllMuons.clear();
	SElectrons.clear();
	LooseElectrons.clear();
	AllElectrons.clear();
	SPhotons.clear();
	AllPhotons.clear();
	SJets.clear();
	AllJets.clear();
	BJets.clear();
	RecWJets.clear();
	RecOtherJets.clear();
	BJet = 0;
	BBarJet = 0;
	BlJet = 0;
	BhJet = 0;

	SelectGENParticles();
	truth1d["event_loss"]->Fill(0.5);
	if(SEMILEP)
	{
		truth1d["event_loss"]->Fill(1.5);
		Genth = (*GenWPartons[0] + *GenWPartons[1] + *GenBh);
	}
	if(SEMILEPACC)
	{
		truth1d["event_loss"]->Fill(2.5);
	}
	//if(GenN.size() != 1) {return(1);}
	//PlotGEN();
	//cout << Run() << " " << LumiBlock() << " " << (int)Number() << endl;
	//cout << GetTriggerSelection("DMT")->Result() << " " << GetTriggerSelection("DET")->Result() << endl;
	//if(GetTriggerSelection("DMT")->Result() == 1)// && GetTriggerSelection("DET")->Result() == -1)
	{
		SelectRECOParticles();
	}
	//if(LooseElectrons.size() != 1 || LooseMuons.size() != 1){return(1);}
	int ntjets = (BJet != 0 ? 1 : 0) + (BBarJet != 0 ? 1 : 0) + RecWJets.size();
	//int ntjets2 = (BhJet != 0 ? 1 : 0) + (BlJet != 0 ? 1 : 0) + RecWJets.size();
	//if(ntjets != ntjets2) cout << "ALARM " << GenCL.size() << " " << RecWJets.size() << endl;
	//if(SEMILEPACC && ntjets != 4){cout << "MISSING JET " << ntjets << endl;}

	if(AllJets.size() < 4 || AllJets.size() > 4000)
	{
		if(SEMILEPACC){ truth2d["response_all"]->Fill(Genth.Pt(), -1);}
		if(SEMILEPACC){ truth2d["response_right"]->Fill(Genth.Pt(), -1);}
		return(1);
	}
	if(SEMILEPACC && ntjets == 4) truth1d["event_loss"]->Fill(3.5);

	TLorentzVector* lep = 0;
	int lepcharge = 0;
	if(AllMuons.size() == 1 && LooseMuons.size() == 1 && LooseElectrons.size() == 0) 
	{
		lep = dynamic_cast<TLorentzVector*>(AllMuons[0]);
		lepcharge = AllMuons[0]->Charge();
	}
	if(AllElectrons.size() == 1 && LooseElectrons.size() == 1 && LooseMuons.size() == 0)
	{
		lep = dynamic_cast<TLorentzVector*>(AllElectrons[0]);
		lepcharge = AllElectrons[0]->Charge();
	} 
	if(lep == 0)
	{
		if(SEMILEPACC){ truth2d["response_all"]->Fill(Genth.Pt(), -1);}
		if(SEMILEPACC){ truth2d["response_right"]->Fill(Genth.Pt(), -1);}
		return(1);
	}
	if(SEMILEPACC && ntjets == 4) truth1d["event_loss"]->Fill(4.5);
	if(SEMILEP && lep->DeltaR(*GenCL[0]) > 0.2) {cout << "Wrong Lep" << endl;}
	//cout << GenCL.size() << " " << (GenCL.size() == 1 ? GenCL[0]->DeltaR(*lep) : -1) << endl;
	sort(AllJets.begin(), AllJets.end(), [](OJet* A, OJet* B){return(A->Pt() > B->Pt());});
	int reducedsize = Min(AllJets.size(), (size_t)50000);
	vector<OJet*> ReducedJets(reducedsize);
	copy(AllJets.begin(), AllJets.begin()+reducedsize, ReducedJets.begin());

	//sort(AllJets.begin(), AllJets.end(), [](OJet* A, OJet* B){return(A->BTagCSVv2() > B->BTagCSVv2());});
	sort(ReducedJets.begin(), ReducedJets.end(), [](OJet* A, OJet* B){return(A->BTagCSVv2() > B->BTagCSVv2());});
	//if(ReducedJets[0]->BTagCSVv2() < 0.941){return(1);}

	//copy_if(AllJets.begin(), AllJets.end(), back_inserter(BJets), [](OJet* j){return(j->BTagCSVv2() > 0.814 && j->Pt() > 30.);});
	//if(AllJets[0]->BTagCSVv2() < 0.941){return(1);}
	//if(AllJets[1]->BTagCSVv2() < 0.814){return(1);}
	//sort(BJets.begin(), BJets.end(), [](OJet* A, OJet* B){return(A->BTagCSVv2() > B->BTagCSVv2());});
	//if(BJets.size() < 2) {return(1);}
	//BJets.resize(2);

	//cout << AllJets[0]->BTagCSVv2() << " " << AllJets[1]->BTagCSVv2() << endl;
	
	IOMET iomet(GetIOMET(2));
	TLorentzVector tlvmet(iomet.px(), iomet.py(), 0., 0.);
	//iomet.px(GenN[0]->Px());
	//iomet.py(GenN[0]->Py());
	if(SEMILEPACC)
	{
		if(RecWJets.size() > 2) cout << "WJets: " << RecWJets.size() << endl;
		truth2d["Jetstt_JetsAll"]->Fill(ntjets+0.5, AllJets.size()+0.5);
		int nb = 0;
		if(BlJet != 0){nb++;}
		if(BhJet != 0){nb++;}
		truth2d["jettest"]->Fill(nb+0.5, RecWJets.size()+0.5);
	}
	if(ntjets == 4)
	{
		ttp_truth.Fill(BhJet, RecWJets[0], RecWJets[1], BlJet, lep, GenN[0], lepcharge);
		for(size_t oj = 0 ; oj < RecOtherJets.size() ; ++oj)
		{
			truth1d["otherjets_pt"]->Fill(RecOtherJets[oj]->Pt());
			truth2d["jetquality_other"]->Fill(RecOtherJets[oj]->GetMCPt()/RecOtherJets[oj]->Pt(), RecOtherJets[oj]->PtFractionWrongPrimaryVertex());
			truth2d["jetnconst_other"]->Fill(RecOtherJets[oj]->GetMCPt()/RecOtherJets[oj]->Pt(), RecOtherJets[oj]->NumChargedHadrons());
			double iso = 100.;
			for_each(AllJets.begin(), AllJets.end(), [&](OJet* j){double dr = RecOtherJets[oj]->DeltaR(*j); if(dr > 0.05 && dr < iso){iso = dr;}});	
			truth1d["otherjets_sep"]->Fill(iso);	
		}
		double isoa = 100.;
		for_each(AllJets.begin(), AllJets.end(), [&](OJet* j){double dr = RecWJets[0]->DeltaR(*j); if(dr > 0.05 && dr < isoa){isoa = dr;}});	
		truth2d["wjets_sep"]->Fill(isoa, (RecWJets[0]->Pt()-RecWJets[0]->GetMCPt())/RecWJets[0]->GetMCPt());	
		double isob = 100.;
		for_each(AllJets.begin(), AllJets.end(), [&](OJet* j){double dr = RecWJets[1]->DeltaR(*j); if(dr > 0.05 && dr < isob){isob = dr;}});	
		truth2d["wjets_sep"]->Fill(isob, (RecWJets[1]->Pt()-RecWJets[1]->GetMCPt())/RecWJets[1]->GetMCPt());	
		if(isoa < 1. && isob < 1) {truth1d["wmass_lowsep"]->Fill((*RecWJets[1] + *RecWJets[0]).M());}	
		if(isoa > 1. && isob > 1) {truth1d["wmass_highsep"]->Fill((*RecWJets[1] + *RecWJets[0]).M());}	
		truth2d["jetquality_signal"]->Fill(BJet->GetMCPt()/BJet->Pt(), BJet->PtFractionWrongPrimaryVertex());
		truth2d["jetquality_signal"]->Fill(BBarJet->GetMCPt()/BBarJet->Pt(), BBarJet->PtFractionWrongPrimaryVertex());
		truth2d["jetquality_signal"]->Fill(RecWJets[0]->GetMCPt()/RecWJets[0]->Pt(), RecWJets[0]->PtFractionWrongPrimaryVertex());
		truth2d["jetquality_signal"]->Fill(RecWJets[1]->GetMCPt()/RecWJets[1]->Pt(), RecWJets[1]->PtFractionWrongPrimaryVertex());

		vector<OJet*> ttjets = {BJet, BBarJet, RecWJets[0], RecWJets[1]};
		OJet* ttjmin = *min_element(ttjets.begin(), ttjets.end(), [](OJet* a, OJet* b){return(a->Pt() < b->Pt());});
		int numhigherpt = count_if(AllJets.begin(), AllJets.end(), [&](OJet* a){return(a->Pt() > ttjmin->Pt());});
		truth2d["jetnum_highptjets"]->Fill(AllJets.size(), numhigherpt);

		truth2d["btag2d_true"]->Fill(Min(BJet->BTagCSVv2(), BBarJet->BTagCSVv2()), Max(BJet->BTagCSVv2(), BBarJet->BTagCSVv2()));
		truth1d["btag_true"]->Fill(BJet->BTagCSVv2());
		truth1d["btag_true"]->Fill(BBarJet->BTagCSVv2());
		truth1d["btag_wrong"]->Fill(RecWJets[0]->BTagCSVv2());
		truth1d["btag_wrong"]->Fill(RecWJets[1]->BTagCSVv2());
	}

	if((cnbtag == 1 && ReducedJets[0]->BTagCSVv2() < 0.941) || (cnbtag == 2 && ReducedJets[1]->BTagCSVv2() < 0.814))
	{
		if(SEMILEPACC){ truth2d["response_all"]->Fill(Genth.Pt(), -1);}
		if(SEMILEPACC){ truth2d["response_right"]->Fill(Genth.Pt(), -1);}
		return(1);
	}
	if(SEMILEPACC && ntjets == 4) truth1d["event_loss"]->Fill(5.5);

	if(SEMILEPACC && ntjets == 4 && ((ReducedJets[0] == BJet && ReducedJets[1] == BBarJet) || ((ReducedJets[1] == BJet && ReducedJets[0] == BBarJet)))){truth1d["event_loss"]->Fill(9.5);}
	int permutcount = 0;

	OJet* rwja = 0;
	OJet* rwjb = 0;
	OJet* rbjh = 0;
	OJet* rbjl = 0;
	TLorentzVector ImpBHad;
	TLorentzVector ImpWja;
	TLorentzVector ImpWjb;
	TLorentzVector ImpBLep;
	TLorentzVector ImpL;
	TLorentzVector ImpNu;
	double rtest = 1E10;
	for(size_t i = cnbtag ; i < ReducedJets.size() ; ++i)
	{
		for(size_t j = cnbtag ; j < i ; ++j)
		{
			for(size_t k = 0 ; k < (cnbtag == 2 ? 2 : ReducedJets.size()) ; ++k)
			{
				if(i == k || j == k) continue;
				for(size_t l = 0 ; l < (cnbtag == 2 ? 2 : ReducedJets.size()) ; ++l)
				{
					if(i == l || j == l || k == l) continue;
					if(k != 0 && l != 0) continue;
					OJet* wja = ReducedJets[i]; 
					OJet* wjb = ReducedJets[j]; 
					OJet* bjh = ReducedJets[k]; 
					OJet* bjl = ReducedJets[l]; 
					//wma > (wmi-20)*-1 + 40
					//if(Max(wja->Pt(), wjb->Pt()) < (20.- Min(wja->Pt(), wjb->Pt())) + 40.) continue;
					if(wja->Pt() < 30. && wjb->Pt() < 30.) continue;
					if(bjh->Pt() < 30. || bjl->Pt() < 30.) continue;
					//double iso = 100.;
					//for_each(AllJets.begin(), AllJets.end(), [&](OJet* j){double dr = wja->DeltaR(*j); if(dr > 0.05 && dr < iso){iso = dr;}});	
					//if(iso < 0.8) continue;
					//iso = 100.;
					//for_each(AllJets.begin(), AllJets.end(), [&](OJet* j){double dr = wjb->DeltaR(*j); if(dr > 0.05 && dr < iso){iso = dr;}});	
					//if(iso < 0.8) continue;
					//if(all_of(AllJets.begin(), AllJets.end(), [&](OJet*j){if(j!=wjb && j!=wja && j->Pt() < 30.){return false;} return true;})) continue; 
permutcount++;
					ttsolver.Solve(bjh, wjb, wja, bjl, lep, &iomet);
					TLorentzVector whad = (*wja + *wjb);
					TLorentzVector thad = (whad + *bjh);

					if(ntjets == 4)
					{
						if(bjl == BlJet)
						{
							truth1d["nstest_right"]->Fill(ttsolver.NSRes());
							truth1d["nschi_right"]->Fill(ttsolver.NSChi2());
						}
						else
						{
							truth1d["nstest_wrong"]->Fill(ttsolver.NSRes());
							truth1d["nschi_wrong"]->Fill(ttsolver.NSChi2());
						}

						if((BJet == bjh && BBarJet == bjl) || (BJet == bjl && BBarJet == bjh))
						{
							truth1d["btagtest_right"]->Fill(ttsolver.BTagRes());
						}
						else
						{
							truth1d["btagtest_wrong"]->Fill(ttsolver.BTagRes());
						}

						if(bjh == BhJet && bjl == BlJet && ((wja == RecWJets[0] && wjb == RecWJets[1]) || (wja == RecWJets[1] && wjb == RecWJets[0])))
						{									
//cout << "m " << bjh << " " << bjl << " " << wja << " " << wjb << endl;
//cout << "t " << BhJet << " " << BlJet << " " << RecWJets[0] << " " << RecWJets[1] << endl;
							truth2d["Wmasshad_tmasshad_right"]->Fill(whad.M(), thad.M());
							truth1d["masstest_right"]->Fill(ttsolver.MassRes());
							truth1d["comtest_right"]->Fill(ttsolver.Res());
							//cout << "right " << permutcount << " " << ttsolver.Res() << endl;
						}
						else
						{
							//cout << "wrong " << permutcount << " " << ttsolver.Res() << endl;
							if(((wja == RecWJets[0] && wjb == RecWJets[1]) || (wja == RecWJets[1] && wjb == RecWJets[0])))
							{
								truth2d["Wmasshad_tmasshad_rightw"]->Fill(whad.M(), thad.M());
							}
							else
							{
								truth2d["Wmasshad_tmasshad_wrongw"]->Fill(whad.M(), thad.M());
							}
							truth2d["Wmasshad_tmasshad_wrong"]->Fill(whad.M(), thad.M());
							truth1d["masstest_wrong"]->Fill(ttsolver.MassRes());
							truth1d["comtest_wrong"]->Fill(ttsolver.Res());
						}
					}
					double res = ttsolver.Res();
					//cout << count++ << " " <<  res << endl;
					if(res < rtest && ttsolver.Nu().Pt() > 0.5)
					{
						//cout << "changed " << res << endl;
						rtest = res;
						rwja = wja; 
						rwjb = wjb; 
						rbjh = bjh; 
						rbjl = bjl; 
						ImpBHad = ttsolver.BHad();
						ImpWja = ttsolver.Wja();
						ImpWjb = ttsolver.Wjb();
						ImpBLep = ttsolver.BLep();
						ImpL = ttsolver.L();
						ImpNu = ttsolver.Nu();
					}	
				}
			}
		}
	}

	//cout << ImpBHad.Pt() << " " << ImpWja.Pt() << " " << ImpWjb.Pt() << " " << ImpBLep.Pt() << " " << ImpL.Pt() << " " << ImpNu.Pt() << endl;
	if(rtest > 1E9)
	{
		if(SEMILEPACC){ truth2d["response_all"]->Fill(Genth.Pt(), -1);}
		if(SEMILEPACC){ truth2d["response_right"]->Fill(Genth.Pt(), -1);}
		return(1);
	}

	if(ReducedJets.size() < 7 &&  Abs((*rwja + *rwjb + *rbjh).M()-170.) < 30.)
	{
		jwa[0] = rwja->Px(); jwa[1] = rwja->Py(); jwa[2] = rwja->Pz();
		jwb[0] = rwjb->Px(); jwb[1] = rwjb->Py(); jwb[2] = rwjb->Pz();

		jestree->Fill();
		if((rwja == RecWJets[0] && rwjb == RecWJets[1]) || (rwja == RecWJets[1] && rwjb == RecWJets[0]))
		{
			truejestree->Fill();
		}
		else
		{
			falsejestree->Fill();
		}
	}
//cout << "rtest " << rtest << endl;
//cout << "M" << rbjh << " " << rbjl << " " << rwja << " " << rwjb << endl;
//cout << "T" << BhJet << " " << BlJet << " " << RecWJets[0] << " " << RecWJets[1] << endl;
	bool coin = (gRandom->Uniform() < 0.5);
	OJet* bjet = 0;
	if(coin)
	{
		if(rbjl->BTagCSVv2() > 0.941) bjet = rbjh;
	}
	else
	{
		if(rbjh->BTagCSVv2() > 0.941) bjet = rbjl;

	}

	if(SEMILEPACC && ntjets == 4) truth1d["event_loss"]->Fill(6.5);
	ttp_all_imp.Fill(&ImpBHad, &ImpWja, &ImpWjb, &ImpBLep, &ImpL, &ImpNu, lepcharge, rtest, bjet);
	if(SEMILEPACC){truth2d["response_all"]->Fill(Genth.Pt(), (ImpWja + ImpWjb + ImpBHad).Pt());}
	else{truth2d["response_all"]->Fill(-1, (ImpWja + ImpWjb + ImpBHad).Pt());}
	if(SEMILEPACC && ntjets == 4 && rbjl == BlJet && rbjh == BhJet && ((rwja == RecWJets[0] && rwjb == RecWJets[1]) || (rwja == RecWJets[1] && rwjb == RecWJets[0]))){truth2d["response_right"]->Fill(Genth.Pt(), (ImpWja + ImpWjb + ImpBHad).Pt());}
	else{truth2d["response_right"]->Fill(-1, (ImpWja + ImpWjb + ImpBHad).Pt());}

	//Truth Information
	if(ntjets == 4 && rbjl == BlJet && rbjh == BhJet && ((rwja == RecWJets[0] && rwjb == RecWJets[1]) || (rwja == RecWJets[1] && rwjb == RecWJets[0])))	
	{
		if(SEMILEPACC && ntjets == 4) truth1d["event_loss"]->Fill(7.5);
		ttp_right.Fill(rbjh, rwja, rwjb, rbjl, lep, &ImpNu, lepcharge, rtest, bjet);
		ttp_right_imp.Fill(&ImpBHad, &ImpWja, &ImpWjb, &ImpBLep, &ImpL, &ImpNu, lepcharge, rtest, bjet);
		truth1d["dRNu_right"]->Fill(ImpNu.DeltaR(*GenN[0]));
		truth1d["dPtNu_right"]->Fill((ImpNu.Pt() - GenN[0]->Pt())/GenN[0]->Pt());
		truth1d["dRNuMet_right"]->Fill(tlvmet.DeltaR(*GenN[0]));
		truth1d["dPtNuMet_right"]->Fill((tlvmet.Pt() - GenN[0]->Pt())/GenN[0]->Pt());
		truth1d["dPzNuNuGen_right"]->Fill((ImpNu.Pz() - GenN[0]->Pz())/GenN[0]->Pz());
		//cout << "right" << endl;
	}
	else if(ntjets == 4)
	{
		ttp_wrong.Fill(rbjh, rwja, rwjb, rbjl, lep, &ImpNu, lepcharge, rtest, bjet);
		ttp_wrong_imp.Fill(&ImpBHad, &ImpWja, &ImpWjb, &ImpBLep, &ImpL, &ImpNu, lepcharge, rtest, bjet);
		//cout << "wrong" << endl;
	}
	else if(SEMILEP)
	{
		ttp_semi.Fill(rbjh, rwja, rwjb, rbjl, lep, &ImpNu, lepcharge, rtest, bjet);
		ttp_semi_imp.Fill(&ImpBHad, &ImpWja, &ImpWjb, &ImpBLep, &ImpL, &ImpNu, lepcharge, rtest, bjet);
	}
	else
	{
		ttp_other.Fill(rbjh, rwja, rwjb, rbjl, lep, &ImpNu, lepcharge, rtest, bjet);
		ttp_other_imp.Fill(&ImpBHad, &ImpWja, &ImpWjb, &ImpBLep, &ImpL, &ImpNu, lepcharge, rtest, bjet);
	}

	return(1);
}

void TTSemi::FileChanged()
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


void TTSemi::BeginLoop()
{
	//SetupPileupWeighting("pu.root", "pu", false);
}
