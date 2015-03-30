#include "ttbarxsec.h"
#include "TRandom3.h"

#include "Permutation.h"

using namespace std;

ttbar::ttbar(const std::string output_filename):
	AnalyzerBase("ttbar", output_filename),
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
	ttp_all("all"),
	ttp_right_incl("right_incl"),
	ttp_wrong_incl("wrong_incl"),
	ttp_semi_incl("semi_incl"),
	ttp_other_incl("other_incl"),
	ttp_all_incl("all_incl"),
	ttp_jetspos_right("jetspos_right"),
	ttp_jetspos_wrong("jetspos_wrong"),
	ttp_hadjets_right("hadjets_right"),
	ttp_hadjets_wrong("hadjets_wrong"),
	ttp_jets_right("jets_right"),
	ttp_jets_wrong("jets_wrong"),
	ttp_blep_right("blep_right"),
	ttp_blep_wrong("blep_wrong"),
	ttp_jetspos_incl_right("jetspos_incl_right"),
	ttp_jetspos_incl_wrong("jetspos_incl_wrong"),
	ttp_hadjets_incl_right("hadjets_incl_right"),
	ttp_hadjets_incl_wrong("hadjets_incl_wrong"),
	ttp_jets_incl_right("jets_incl_right"),
	ttp_jets_incl_wrong("jets_incl_wrong"),
	ttp_blep_incl_right("blep_incl_right"),
	ttp_blep_incl_wrong("blep_incl_wrong"),
	BTAGMODE(false), //set true for the b-tag efficiency measurement
	cnbtag(1), //1: one thight b-jet, 2: two medium b-jets
	cnusedjets(10000), //only nused jets, ordered by pT are used for the permutations
	cwjetptsoft(25.), //min pT of softer W-jet
	cwjetpthard(35.), //min pT of harder W-jet 
	cbjetptsoft(25.), //min pT of softer b-jets
	cbjetpthard(35.), //min pT of harder b-jets
	cjetetamax(2.4),//max |eta| for jets
	clptmin(30.), //min pT of lepton (el or mu)
	cletamax(2.4)//max |eta| of leptons (max allowed value is 2.4) 
{

	jetptmin = min(cwjetptsoft, cbjetptsoft);
	topptbins = {0., 40., 55., 65., 75., 85., 95., 105., 115., 125., 135., 145., 155., 170., 185., 200., 220., 240., 265., 300., 350., 400., 1000.};
	topetabins = {0., 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.3, 2.8, 8.0};
	ttmbins = {250., 350., 370., 390., 410., 430., 450., 470., 490., 510., 530., 550., 575., 600., 630., 670., 720., 800., 900, 5000.};
}

void ttbar::begin()
{
	outFile_.cd();
	TDirectory* dir_gen = outFile_.mkdir("GEN");
	dir_gen->cd();
	gen1d.AddHist("TYP", 4, 0., 4., "Decay TYP", "Events");
	gen2d.AddHist("wjets_eta", 100, 0., 5., 100, 0, 5, "W-jet #eta_{min}", "W-jet #eta_{max}");
	gen2d.AddHist("bjets_eta", 100, 0., 5., 100, 0, 5, "b-jet #eta_{min}", "b-jet #eta_{max}");
	gen2d.AddHist("wjets_pt", 200, 0, 400, 200, 0., 400, "W-jet p_{T,min} (GeV)", "W-jet p_{T,max} (GeV)");
	gen2d.AddHist("bjets_pt", 200, 0, 400, 200, 0., 400, "b-jet p_{T,min} (GeV)", "b-jet p_{T,max} (GeV)");
	gen1d.AddHist("bjets_dr", 100, 0, 5, "b-jets #DeltaR", "Events");
	gen1d.AddHist("wjets_dr", 100, 0, 5, "W-jets #DeltaR", "Events");

	TDirectory* dir_truth = outFile_.mkdir("TRUTH");
	dir_truth->cd();
	truth1d.AddHist("counter", 20, 0., 20., "counter", "Events");
	truth2d.AddHist("btag2d_true", 100, 0., 1., 100, 0., 1., "btag", "btag");
	truth1d.AddHist("btag_true", 100, 0., 1., "btag", "Events");
	truth1d.AddHist("btag_wrong", 100, 0., 1., "btag", "Events");
	truth2d.AddHist("wjets_sep", 100, 0., 5., 80, -4., 4., "min sep", "resolution");	
	truth2d.AddHist("bjets_sep", 100, 0., 5., 80, -4., 4., "min sep", "resolution");
	truth2d.AddHist("tt_jets", 3, 0., 3., 3, 0., 3., "num b-jets", "num w-jets");
	truth2d.AddHist("Jetstt_JetsAll", 5, 0., 5., 20, 0., 20., "jet tt-system", "all jets");
	truth2d.AddHist("Nmu_Ne", 5, 0., 5., 5, 0., 5., "# #mu", "# e");
	truth2d.AddHist("Ne_Nmu", 5, 0., 5., 5, 0., 5., "# e", "# #mu");
	truth2d.AddHist("dPtJet_right", 50, 0., 250., 200, -2., 2., "p_{T} (GeV)", "#Deltap_{T}/p_{T}");
	truth2d.AddHist("dPtbJet_right", 50, 0., 250., 200, -2., 2., "p_{T} (GeV)", "#Deltap_{T}/p_{T}");
	truth1d.AddHist("dRNu_right", 200, 0., 10., "#DeltaR(#nu_{gen}, #nu_{rec})", "Events");
	truth1d.AddHist("dPtNu_right", 200, -2., 2., "#Deltap_{T}/p_{T}", "Events");
	truth1d.AddHist("dPzNu_right", 200, -2., 2., "#Deltap_{z}/p_{z}", "Events");
	truth1d.AddHist("dRNuMet_right", 200, 0., 10., "#DeltaR(#nu_{gen}, #nu_{rec})", "Events");
	truth1d.AddHist("dPtNuMet_right", 200, -2., 2., "#Deltap_{T}/p_{T}", "Events");
	truth2d.AddHist("Wmasshad_tmasshad_right", 500, 0., 500., 500, 0., 500, "M(W) (GeV)", "M(t) (GeV)");
	truth2d.AddHist("Wmasshad_tmasshad_wrong", 500, 0., 500., 500, 0., 500, "M(W) (GeV)", "M(t) (GeV)");
	truth2d.AddHist("Wmasshad_tmasshad_rightw", 500, 0., 500., 500, 0., 500, "M(W) (GeV)", "M(t) (GeV)");
	truth2d.AddHist("Wmasshad_tmasshad_wrongw", 500, 0., 500., 500, 0., 500, "M(W) (GeV)", "M(t) (GeV)");
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

	truth1d.AddHist("response_toppthad_truth", topptbins, "response_toppthad_truth", "Events");
	truth1d.AddHist("response_toppthad_measured", topptbins, "response_toppthad_measured", "Events");
	truth2d.AddHist("response_toppthad_matrix", topptbins, topptbins, "gen", "reco");

	truth1d.AddHist("response_topptlep_truth", topptbins, "response_topptlep_truth", "Events");
	truth1d.AddHist("response_topptlep_measured", topptbins, "response_topptlep_measured", "Events");
	truth2d.AddHist("response_topptlep_matrix", topptbins, topptbins, "gen", "reco");

	truth1d.AddHist("response_topetahad_truth", topetabins, "response_topetahad_truth", "Events");
	truth1d.AddHist("response_topetahad_measured", topetabins, "response_topetahad_measured", "Events");
	truth2d.AddHist("response_topetahad_matrix", topetabins, topetabins, "gen", "reco");

	truth1d.AddHist("response_topetalep_truth", topetabins, "response_topetalep_truth", "Events");
	truth1d.AddHist("response_topetalep_measured", topetabins, "response_topetalep_measured", "Events");
	truth2d.AddHist("response_topetalep_matrix", topetabins, topetabins, "gen", "reco");

	truth1d.AddHist("response_mtt_truth", ttmbins, "response_mtt_truth", "Events");
	truth1d.AddHist("response_mtt_measured", ttmbins, "response_mtt_measured", "Events");
	truth2d.AddHist("response_mtt_matrix", ttmbins, ttmbins, "gen", "reco");

	ttp_right.Init(this);
	ttp_wrong.Init(this);
	ttp_semi.Init(this);
	ttp_other.Init(this);
	ttp_right_incl.Init(this);
	ttp_wrong_incl.Init(this);
	ttp_semi_incl.Init(this);
	ttp_other_incl.Init(this);

	ttp_hadjets_right.Init(this);
	ttp_hadjets_wrong.Init(this);
	ttp_jets_right.Init(this);
	ttp_jets_wrong.Init(this);
	ttp_blep_right.Init(this);
	ttp_blep_wrong.Init(this);
	ttp_jetspos_right.Init(this);
	ttp_jetspos_wrong.Init(this);
	ttp_hadjets_incl_right.Init(this);
	ttp_hadjets_incl_wrong.Init(this);
	ttp_jets_incl_right.Init(this);
	ttp_jets_incl_wrong.Init(this);
	ttp_blep_incl_right.Init(this);
	ttp_blep_incl_wrong.Init(this);
	ttp_jetspos_incl_right.Init(this);
	ttp_jetspos_incl_wrong.Init(this);

	btageff.Init();

	TDirectory* dir_reco = outFile_.mkdir("RECO");
	dir_reco->cd();
	ttp_all.Init(this);
	ttp_all_incl.Init(this);
	if(BTAGMODE)
	{
		ttsolver.Init("Prob.root", false, true);
	}
	else
	{
		ttsolver.Init("Prob.root", true, true);
	}
}

void ttbar::SelectGenParticles(URStreamer& event)
{
	const vector<Genparticle>& gps = event.genParticles();
	//cout <<gps.size() << endl;
	//loop over muons (by reference)
	int lepdecays = 0;
	int topcounter = 0;
	for(vector<Genparticle>::const_iterator gp = gps.begin(); gp != gps.end(); ++gp)
	{
		//	if(Abs(gp->pdgId()) > 10 && Abs(gp->pdgId()) < 18)
		//	{
		//		cout << "P:"  << " " << gp->status() << " " << gp->pdgId() << ":";
		//		for(size_t m = 0 ; m < gp->momIdx().size() ; ++m) cout << " " << gps[gp->momIdx()[m]].pdgId();
		//		cout << endl;
		//	}
			//if(gp->pdgId() == 6)
			//{
			//	weight = 1.-(gp->Pt()-200.)/2000.;
			//}
		if(gp->status() > 21 && gp->status() < 30 && gp->momIdx().size() != 0)
		{
			if(Abs(gp->pdgId()) == 6)
			{
				topcounter++;
			}
			if(gp->pdgId() == 5 && gps[gp->momIdx()[0]].pdgId() != 24)
			{
				sgenparticles.push_back(*gp);
				genb = &(sgenparticles.back());
			}

			if(gp->pdgId() == -5 && gps[gp->momIdx()[0]].pdgId() != -24)
			{
				sgenparticles.push_back(*gp);
				genbbar = &(sgenparticles.back());
			}
			if(Abs(gp->pdgId()) < 6 && Abs(gps[gp->momIdx()[0]].pdgId()) == 24)
			{
				sgenparticles.push_back(*gp);
				genwpartons.push_back(&(sgenparticles.back()));
			}
		}

		if(gp->momIdx().size() != 0 && Abs(gps[gp->momIdx()[0]].pdgId()) == 24)
		{	
			if(Abs(gp->pdgId()) == 11 || Abs(gp->pdgId()) == 13)
			{
				sgenparticles.push_back(*gp);
				gencls.push_back(&(sgenparticles.back()));
			}
			if(Abs(gp->pdgId()) == 12 || Abs(gp->pdgId()) == 14)
			{
				sgenparticles.push_back(*gp);
				gennls.push_back(&(sgenparticles.back()));	
				lepdecays++;
			}
			if(Abs(gp->pdgId()) == 16)
			{
				lepdecays++;
			}
		}

		if(gp->status() == 1 && gp->momIdx().size() != 0 && (Abs(gps[gp->momIdx()[0]].pdgId()) == 24 || gp->pdgId() == gps[gp->momIdx()[0]].pdgId()))
		{
			if(Abs(gp->pdgId()) == 11 || Abs(gp->pdgId()) == 13)
			{
				sgenparticles.push_back(*gp);
				genfincls.push_back(&(sgenparticles.back()));	
			}
		}
	}

	FULLHAD = false;
	SEMILEP = false;
	FULLLEP = false;
	SEMILEPACC = false;
	if(topcounter == 2 && genb != 0 && genbbar != 0)
	{
		if(lepdecays == 2 && genwpartons.size() == 0)
		{ 
			FULLLEP = true; gen1d["TYP"]->Fill(0.5, weight);
		}
		else if(lepdecays == 1 && genwpartons.size() == 2)
		{
			gen1d["TYP"]->Fill(1.5, weight);
		}
		else if(lepdecays == 0 && genwpartons.size() == 4)
		{
			FULLHAD = true; gen1d["TYP"]->Fill(2.5, weight);
		}
	}
	else
	{
		gen1d["TYP"]->Fill(3.5, weight);
	}

	if(topcounter == 2 && genwpartons.size() == 2 && gencls.size() == 1 && gennls.size() == 1 && genb != 0 && genbbar != 0)//no tau
	{
		SEMILEP = true;
		if(gencls[0]->pdgId() > 0){genbl = genbbar; genbh = genb;}
		else {genbh = genbbar; genbl = genb;}
		gentophad = (*genwpartons[0] + *genwpartons[1] + *genbh);
		gentoplep = (*gencls[0] + *gennls[0] + *genbl);

		gen2d["wjets_eta"]->Fill(Min(Abs(genwpartons[0]->Eta()), Abs(genwpartons[1]->Eta())), Max(Abs(genwpartons[0]->Eta()), Abs(genwpartons[1]->Eta())), weight);
		gen2d["bjets_eta"]->Fill(Min(Abs(genb->Eta()), Abs(genbbar->Eta())), Max(Abs(genb->Eta()), Abs(genbbar->Eta())), weight);
		if(Abs(gencls[0]->Eta()) < cletamax && gencls[0]->Pt() > clptmin && Abs(genwpartons[0]->Eta()) < cjetetamax && Abs(genwpartons[1]->Eta()) < cjetetamax && Abs(genb->Eta()) < cjetetamax && Abs(genbbar->Eta()) < cjetetamax)
		{
			gen2d["wjets_pt"]->Fill(Min(genwpartons[0]->Pt(), genwpartons[1]->Pt()), Max(genwpartons[0]->Pt(), genwpartons[1]->Pt()), weight);
			gen2d["bjets_pt"]->Fill(Min(genb->Pt(), genbbar->Pt()), Max(genb->Pt(), genbbar->Pt()), weight);
			gen1d["wjets_dr"]->Fill(genwpartons[0]->DeltaR(*genwpartons[1]), weight);
			gen1d["bjets_dr"]->Fill(genb->DeltaR(*genbbar), weight);
			if(Min(genwpartons[0]->Pt(), genwpartons[1]->Pt()) > cwjetptsoft && Max(genwpartons[0]->Pt(), genwpartons[1]->Pt()) > cwjetpthard && Min(genb->Pt(), genbbar->Pt()) > cbjetptsoft && Max(genb->Pt(), genbbar->Pt()) > cbjetpthard)
			{
				SEMILEPACC = true;
			}
		}

	}
	else
	{
		genwpartons.clear();
		gencls.clear();
		gennls.clear();
		genb = 0;
		genbbar = 0;
	}

}

void ttbar::SelectRecoParticles(URStreamer& event)
{
	const vector<Muon>& muons = event.muons();
	for(vector<Muon>::const_iterator muon = muons.begin(); muon != muons.end(); ++muon)
	{
		IDMuon mu(*muon);
		if(mu.ID(IDMuon::TIGHT_12) && mu.Pt() > 15.)
		{
			smuons.push_back(mu);
			loosemuons.push_back(&(smuons.back()));
			if(mu.ID(IDMuon::TIGHT_12) && mu.Pt() > clptmin && Abs(mu.Eta()) < cletamax)
			{
				tightmuons.push_back(&(smuons.back()));
			}
		}
	}

	const vector<Electron>& electrons = event.electrons();
	for(vector<Electron>::const_iterator electron = electrons.begin(); electron != electrons.end(); ++electron)
	{
		IDElectron el(*electron);
		if(el.ID(IDElectron::MEDIUM_12) && el.Pt() > 15.)
		{
			selectrons.push_back(el);
			looseelectrons.push_back(&(selectrons.back()));
			if(el.ID(IDElectron::MEDIUM_12) && el.Pt() > clptmin && Abs(el.Eta()) < cletamax)
			{
				mediumelectrons.push_back(&(selectrons.back()));
			}
		}
	}

	vector<Jet> jets = event.jets();
	for(vector<Jet>::iterator jet = jets.begin(); jet != jets.end(); ++jet)
	{
		double sf = 1.0;
		jet->SetPxPyPzE(jet->Px()*sf, jet->Py()*sf, jet->Pz()*sf, jet->E()*sf);
		if(jet->Pt() < jetptmin || Abs(jet->Eta()) > cjetetamax) {continue;}

		for(IDMuon* mu : loosemuons)
		{
			if(mu->DeltaR(*jet) < 0.3)
			{
				goto nextjetA;
			}
		}

		for(IDElectron* el : looseelectrons)
		{
			if(el->DeltaR(*jet) < 0.3)
			{
				goto nextjetA;
			}
		}

		sjets.push_back(*jet);
		cleanedjets.push_back(&(sjets.back()));
nextjetA: continue;
	}

	const vector<Met>& mets = event.METs();
	if(mets.size() == 1)
	{
		met = mets[0];
	}

	//Gen-Reco matching
	if(SEMILEP)
	{

		rightper.MET(&met);
		for(IDElectron* el : mediumelectrons)
		{
			if(el->DeltaR(*genfincls[0]) < 0.2)
			{
				rightper.L(el);
			}
		}

		for(IDMuon* mu : tightmuons)
		{
			if(mu->DeltaR(*genfincls[0]) < 0.2)
			{
				rightper.L(mu);
			}
		}

		double ptbmax = 0.;
		double ptbbarmax = 0.;
		vector<double> wjptmax(genwpartons.size(), 0.);
		for(size_t j = 0 ; j < cleanedjets.size() ; ++j)
		{
			Jet* jet = cleanedjets[j];
			if(jet->DeltaR(*genb) < 0.3 && jet->Pt() > ptbmax)
			{
				ptbmax = jet->Pt();
				if(gencls[0]->pdgId() < 0)
				{
					rightper.BLep(jet);
				}
				else if(gencls[0]->pdgId() > 0)
				{
					rightper.BHad(jet);
				}
				continue;
			}
			if(jet->DeltaR(*genbbar) < 0.3 && jet->Pt() > ptbbarmax)
			{
				ptbbarmax = jet->Pt();
				if(gencls[0]->pdgId() < 0)
				{
					rightper.BHad(jet);
				}
				else if(gencls[0]->pdgId() > 0)
				{
					rightper.BLep(jet);
				}
				continue;
			}
			if(jet->DeltaR(*genwpartons[0]) < 0.3 && jet->Pt() > wjptmax[0])
			{
				wjptmax[0] = jet->Pt();
				rightper.WJa(jet);
				continue;
			}
			if(jet->DeltaR(*genwpartons[1]) < 0.3 && jet->Pt() > wjptmax[1])
			{
				wjptmax[1] = jet->Pt();
				rightper.WJb(jet);
				continue;
			}
			recotherjets.push_back(jet);
		}
	}
}

void ttbar::ttanalysis()
{
	truth1d["counter"]->Fill(19.5, weight);
	if(SEMILEP) truth1d["counter"]->Fill(1.5, weight);
	if(SEMILEPACC)
	{
		truth1d["counter"]->Fill(2.5, weight);
		truth1d["response_toppthad_truth"]->Fill(gentophad.Pt(), weight);
		truth1d["response_topptlep_truth"]->Fill(gentoplep.Pt(), weight);
		truth1d["response_topetahad_truth"]->Fill(Abs(gentophad.Eta()), weight);
		truth1d["response_topetalep_truth"]->Fill(Abs(gentoplep.Eta()), weight);
		truth1d["response_mtt_truth"]->Fill((gentophad + gentoplep).M(), weight);
		if(Abs(gencls[0]->pdgId()) == 11) {truth2d["Ne_Nmu"]->Fill(mediumelectrons.size()+0.5, tightmuons.size()+0.5, weight);}
		if(Abs(gencls[0]->pdgId()) == 13) {truth2d["Nmu_Ne"]->Fill(tightmuons.size()+0.5, mediumelectrons.size()+0.5, weight);}
	}
	//check for lepton:
	TLorentzVector* lep = 0;
	int lepcharge = 0;
	if(tightmuons.size() == 1 && mediumelectrons.size() == 0)
	{
		lep = dynamic_cast<TLorentzVector*>(tightmuons[0]);
		lepcharge = tightmuons[0]->charge();
	}
	if(tightmuons.size() == 0 && mediumelectrons.size() == 1)
	{
		lep = dynamic_cast<TLorentzVector*>(mediumelectrons[0]);
		lepcharge = mediumelectrons[0]->charge();
	}
	if(lep == 0){return;}
	if(SEMILEPACC)
	{
		truth1d["counter"]->Fill(3.5);
	}
	if(rightper.NumTTBarJets() == 4)
	{
		truth2d["dPtJet_right"]->Fill(genwpartons[0]->Pt(), (rightper.WJa()->Pt() - genwpartons[0]->Pt())/genwpartons[0]->Pt(), weight);
		truth2d["dPtJet_right"]->Fill(genwpartons[1]->Pt(), (rightper.WJb()->Pt() - genwpartons[1]->Pt())/genwpartons[1]->Pt(), weight);
		truth2d["dPtbJet_right"]->Fill(genbh->Pt(), (rightper.BHad()->Pt() - genbh->Pt())/genbh->Pt(), weight);
		truth2d["dPtbJet_right"]->Fill(genbl->Pt(), (rightper.BLep()->Pt() - genbl->Pt())/genbl->Pt(), weight);
	}

	//jet number plots
	if(SEMILEP)
	{
		if(lep != rightper.L()) {cout << "Wrong Lep" << endl;}
		truth2d["Jetstt_JetsAll"]->Fill(rightper.NumTTBarJets()+0.5, cleanedjets.size()+0.5, weight);
	}
	//plot b-tag distribution
	if(rightper.IsComplete())
	{
		truth2d["btag2d_true"]->Fill(Min(rightper.BHad()->csvIncl(), rightper.BLep()->csvIncl()), Max(rightper.BHad()->csvIncl(), rightper.BLep()->csvIncl()), weight);
		truth1d["btag_true"]->Fill(rightper.BHad()->csvIncl(), weight);
		truth1d["btag_true"]->Fill(rightper.BLep()->csvIncl(), weight);
		truth1d["btag_wrong"]->Fill(rightper.WJa()->csvIncl(), weight);
		truth1d["btag_wrong"]->Fill(rightper.WJb()->csvIncl(), weight);
	}

	//cut on number of jets
	if(cleanedjets.size() < 4){return;}
	if(BTAGMODE && cleanedjets.size() > 5){return;}
	if(SEMILEPACC) truth1d["counter"]->Fill(4.5, weight);

	//keeping only the n leading jets. 
	sort(cleanedjets.begin(), cleanedjets.end(), [](Jet* A, Jet* B){return(A->Pt() > B->Pt());});
	int reducedsize = Min(cleanedjets.size(), cnusedjets);
	reducedjets.resize(reducedsize);
	copy(cleanedjets.begin(), cleanedjets.begin()+reducedsize, reducedjets.begin());

	//check for b-jets
	sort(reducedjets.begin(), reducedjets.end(), [](Jet* A, Jet* B){return(A->csvIncl() > B->csvIncl());});
	if((cnbtag == 1 && reducedjets[0]->csvIncl() < 0.941) || (cnbtag == 2 && reducedjets[1]->csvIncl() < 0.814)){return;}
	if(SEMILEPACC) truth1d["counter"]->Fill(5.5, weight);

	//check what we have reconstructed
	if(SEMILEP)
	{
		truth2d["tt_jets"]->Fill(rightper.NumBJets()+0.5, rightper.NumWJets()+0.5, weight);
	}

	//reconstruction
	Permutation bestper;
	int permutation = 0;

	if(SEMILEPACC && rightper.IsComplete()) truth1d["counter"]->Fill(6.5, weight);
	for(size_t i = cnbtag ; i < reducedjets.size() ; ++i)
	{
		for(size_t j = cnbtag ; j < i ; ++j)
		{
			for(size_t k = 0 ; k < (cnbtag == 2 ? 2 : reducedjets.size()) ; ++k)
			{
				if(i == k || j == k) continue;
				for(size_t l = 0 ; l < (cnbtag == 2 ? 2 : reducedjets.size()) ; ++l)
				{
					if(i == l || j == l || k == l) continue;
					Permutation testper(reducedjets[i], reducedjets[j], reducedjets[k], reducedjets[l], lep, &met);
					if(testper.WJa()->Pt() < cwjetpthard && testper.WJb()->Pt() < cwjetpthard) continue;
					if(testper.WJa()->Pt() < cwjetptsoft || testper.WJb()->Pt() < cwjetptsoft) continue;
					if(testper.BHad()->Pt() < cbjetpthard && testper.BLep()->Pt() < cbjetpthard) continue;
					if(testper.BHad()->Pt() < cbjetptsoft || testper.BLep()->Pt() < cbjetptsoft) continue;
					testper.Solve(ttsolver);

					TLorentzVector whad(testper.WHad());
					TLorentzVector thad(testper.THad());

					if(rightper.IsComplete())
					{
						if(rightper.IsBLepCorrect(testper))
						{
							truth1d["nstest_right"]->Fill(ttsolver.NSRes(), weight);
							truth1d["nschi_right"]->Fill(ttsolver.NSChi2(), weight);
						}
						else
						{
							truth1d["nstest_wrong"]->Fill(ttsolver.NSRes(), weight);
							truth1d["nschi_wrong"]->Fill(ttsolver.NSChi2(), weight);
						}

						if(rightper.AreBsCorrect(testper))
						{
							truth1d["btagtest_right"]->Fill(ttsolver.BTagRes(), weight);
						}
						else
						{
							truth1d["btagtest_wrong"]->Fill(ttsolver.BTagRes(), weight);
						}

						if(rightper.IsCorrect(testper))
						{
							truth2d["Wmasshad_tmasshad_right"]->Fill(whad.M(), thad.M(), weight);
							truth1d["masstest_right"]->Fill(ttsolver.MassRes(), weight);
							truth1d["comtest_right"]->Fill(ttsolver.Res(), weight);
						}
						else
						{
							if(rightper.IsWHadCorrect(testper))
							{
								truth2d["Wmasshad_tmasshad_rightw"]->Fill(whad.M(), thad.M(), weight);
							}
							else
							{
								truth2d["Wmasshad_tmasshad_wrongw"]->Fill(whad.M(), thad.M(), weight);
							}
							truth2d["Wmasshad_tmasshad_wrong"]->Fill(whad.M(), thad.M(), weight);
							truth1d["masstest_wrong"]->Fill(ttsolver.MassRes(), weight);
							truth1d["comtest_wrong"]->Fill(ttsolver.Res(), weight);
						}

					}

					if(testper < bestper)
					{
						bestper = testper;
					}
				}
			}
		}
	}
	if(bestper.Prob() > 1E9){return;}
	if(SEMILEPACC && rightper.IsComplete()) truth1d["counter"]->Fill(7.5, weight);
	//Fill reconstructed hists
	ttp_all.Fill(bestper, lepcharge, weight);
	if(cleanedjets.size() == 4) ttp_all_incl.Fill(bestper, lepcharge, weight);


	if(BTAGMODE){btageff.Fill(bestper, rightper.IsCorrect(bestper), weight);}

	//Fill reconstructed hists with matching information
	if(rightper.IsCorrect(bestper))
	{
		ttp_right.Fill(bestper, lepcharge, weight);
		if(cleanedjets.size() == 4) ttp_right_incl.Fill(bestper, lepcharge, weight);
	}
	else if(rightper.IsComplete())
	{
		ttp_wrong.Fill(bestper, lepcharge, weight);
		if(cleanedjets.size() == 4) ttp_wrong_incl.Fill(bestper, lepcharge, weight);
		if(SEMILEPACC) truth1d["counter"]->Fill(9.5, weight);
	}
	else if(SEMILEP)
	{
		ttp_semi.Fill(bestper, lepcharge, weight);
		if(cleanedjets.size() == 4) ttp_semi_incl.Fill(bestper, lepcharge, weight);
	}
	else
	{
		ttp_other.Fill(bestper, lepcharge, weight);
		if(cleanedjets.size() == 4) ttp_other_incl.Fill(bestper, lepcharge, weight);
	}

	if(bestper.IsCorrect(rightper))
	{
		ttp_jetspos_right.Fill(bestper, lepcharge, weight);
		if(SEMILEPACC)
		{
			truth1d["counter"]->Fill(10.5, weight);
		}
	}
	else
	{
		ttp_jetspos_wrong.Fill(bestper, lepcharge, weight);
	}

	if(bestper.AreHadJetsCorrect(rightper))
	{
		ttp_hadjets_right.Fill(bestper, lepcharge, weight);
		truth1d["response_toppthad_measured"]->Fill(bestper.THad().Pt(), weight);
		truth1d["response_topetahad_measured"]->Fill(Abs(bestper.THad().Eta()), weight);
		if(SEMILEPACC)
		{
			truth1d["counter"]->Fill(11.5, weight);
			truth2d["response_toppthad_matrix"]->Fill(gentophad.Pt(), bestper.THad().Pt(), weight);
			truth2d["response_topetahad_matrix"]->Fill(Abs(gentophad.Eta()), Abs(bestper.THad().Eta()), weight);
		}
	}
	else
	{
		ttp_hadjets_wrong.Fill(bestper, lepcharge, weight);
	}

	if(bestper.AreJetsCorrect(rightper))
	{
		ttp_jets_right.Fill(bestper, lepcharge, weight);
		truth1d["response_mtt_measured"]->Fill((bestper.THad() + bestper.TLep()).M(), weight);
		if(SEMILEPACC)
		{
			truth1d["counter"]->Fill(12.5, weight);
			truth2d["response_mtt_matrix"]->Fill((*genbl + *gencls[0] + *gennls[0] + *genbh + *genwpartons[0] + *genwpartons[1]).M(), (bestper.THad() + bestper.TLep()).M(), weight);
		}
	}
	else
	{
		ttp_jets_wrong.Fill(bestper, lepcharge, weight);
	}

	if(bestper.IsBLepCorrect(rightper))
	{
		ttp_blep_right.Fill(bestper, lepcharge, weight);
		truth1d["response_topptlep_measured"]->Fill(bestper.TLep().Pt(), weight);
		truth1d["response_topetalep_measured"]->Fill(Abs(bestper.TLep().Eta()), weight);
		if(SEMILEPACC)
		{
			truth1d["counter"]->Fill(13.5, weight);
			truth2d["response_topptlep_matrix"]->Fill(gentoplep.Pt(), bestper.TLep().Pt(), weight);
			truth2d["response_topetalep_matrix"]->Fill(Abs(gentoplep.Eta()), Abs(bestper.TLep().Eta()), weight);
		}
		//Neutrino reconstruction plots
		truth1d["dRNu_right"]->Fill(bestper.Nu().DeltaR(*gennls[0]), weight);
		truth1d["dPtNu_right"]->Fill((bestper.Nu().Pt() - gennls[0]->Pt())/gennls[0]->Pt(), weight);
		truth1d["dPzNu_right"]->Fill((bestper.Nu().Pz() - gennls[0]->Pz())/gennls[0]->Pz(), weight);
		truth1d["dRNuMet_right"]->Fill(met.DeltaR(*gennls[0]), weight);
		truth1d["dPtNuMet_right"]->Fill((met.Pt() - gennls[0]->Pt())/gennls[0]->Pt(), weight);
	}
	else
	{
		ttp_blep_wrong.Fill(bestper, lepcharge, weight);
	}

}

//This method is called once every file, contains the event loop
//run your proper analysis here
void ttbar::analyze()
{

	int nevent = 0;
	URStreamer event(tree_);
	IDElectron::streamer = &event;
	while(event.next())
	{
		nevent++;
		if(nevent % 1000 == 0)cout << "Event:" << nevent << endl;
		truth1d["counter"]->Fill(0.5);	
		weight = 1.;
		//TLorentzVector tl(20., 10., 10., Sqrt(600));
		//TLorentzVector tb(10., -30., -10., Sqrt(1200));
		//NeutrinoSolver NS(&tl, &tb, 80., 173.);
		//double chi;
		//TLorentzVector met(NS.GetBest(-20., 10., 1.,1.,0, chi));
		//cout << chi << " " << (tl+met).M() << " " << (tl+tb+met).M() << endl; 


		sgenparticles.clear();
		genwpartons.clear();
		gencls.clear();
		genfincls.clear();
		gennls.clear();
		genb = 0;
		genbbar = 0;
		genbl = 0;
		genbh = 0;

		rightper.Reset();

		//recbjets.clear();
		//recwjets.clear();
		recotherjets.clear();
		//recbjet = 0;
		//recbbarjet = 0;
		//recbhjet = 0;
		//recbljet = 0;

		sjets.clear();
		cleanedjets.clear();
		reducedjets.clear();
		smuons.clear();
		tightmuons.clear();
		loosemuons.clear();
		selectrons.clear();
		mediumelectrons.clear();
		looseelectrons.clear();

		SelectGenParticles(event);
		SelectRecoParticles(event);

		ttanalysis();

	}
}


//make it executable
int main(int argc, char *argv[])
{
	URParser &parser = URParser::instance(argc, argv);
	URDriver<ttbar> test;
	return test.run();
}
