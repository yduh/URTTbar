#include "ttbarxsec.h"
#include "TRandom3.h"

#include "Permutation.h"
#include "PDFuncertainty.h"
#include "NeutrinoSolver.h"

using namespace std;

ttbar::ttbar(const std::string output_filename):
	AnalyzerBase("ttbar", output_filename),
	gen1d("gen"),
	gen2d("gen"),
    ttp_genall("genall"),
    ttp_genacc("genacc"),
	reco1d("reco"),
	reco2d("reco"),
	truth1d("truth"),
	truth2d("truth"),
	ttp_truth("truth"),
	ttp_right("right"),
	ttp_wrong("wrong"),
	ttp_semi("semi"),
	ttp_other("other"),
	ttp_all("all"),
	ttp_jetspos_right("jetspos_right"),
	ttp_jetspos_wrong("jetspos_wrong"),
//	ttp_hadjets_right("hadjets_right"),
//	ttp_hadjets_wrong("hadjets_wrong"),
//	ttp_jets_right("jets_right"),
//	ttp_jets_wrong("jets_wrong"),
//	ttp_blep_right("blep_right"),
//	ttp_blep_wrong("blep_wrong"),
	ttp_whad_right("whad_right"),
	ttp_whad_wrong("whad_wrong"),
	ttp_tlepthad_right("ttp_tlepthad_right"),
	ttp_tlep_right("ttp_tlep_right"),
	ttp_thad_right("ttp_thad_right"),
	ttp_nn_right("ttp_nn_right"),
	ttp_nsemi_right("ttp_nsemi_right"),
	response("response", this),
	DATASIM(false),
	PSEUDOTOP(false),
	BTAGMODE(false), //set true for the b-tag efficiency measurement
	JETSCALEMODE(false), //set true for the b-tag efficiency measurement
	ELECTRONS(true),
	MUONS(true),
	cnbtag(2), //1: one thight b-jet, 2: two medium b-jets
	cnusedjets(100), //only nused jets, ordered by pT are used for the permutations
	cwjetptsoft(25.), //min pT of softer W-jet
	cwjetpthard(35.), //min pT of harder W-jet 
	cbjetptsoft(25.), //min pT of softer b-jets
	cbjetpthard(35.), //min pT of harder b-jets
	cjetetamax(2.4),//max |eta| for jets
	clptmin(30.), //min pT of lepton (el or mu)
	cletamax(2.4)//max |eta| of leptons (max allowed value is 2.4) 
{
	double lumi = 1000;
	cout << output_filename << endl;
	if(output_filename.find("WJets") != string::npos)
	{
		selectionprob = lumi*61524./14113879.;
	}
	else if(output_filename.find("DYJets") != string::npos)
	{
		selectionprob = lumi*6024./2429156.;
	}
	else if(output_filename.find("Wtbar") != string::npos)
	{
		selectionprob = lumi*35.6/499999.;
	}
	else if(output_filename.find("Wt") != string::npos)
	{
		selectionprob = lumi*35.6/500000.;
	}
	else if(output_filename.find("STt") != string::npos)
	{
		selectionprob = lumi*217./860071.;
	}
	else if(output_filename.find("tt_PowhegP8") != string::npos)
	{
		selectionprob = lumi*806./4502964.;
	}
	else if(output_filename.find("tt_aMCatNLO") != string::npos)
	{
		selectionprob = lumi*806./1525140.;
	}

	jetptmin = min(cwjetptsoft, cbjetptsoft);
	if(PSEUDOTOP){cnbtag = 2;}
//	topptbins = {0., 40., 55., 65., 75., 85., 95., 105., 115., 125., 135., 145., 155., 170., 185., 200., 220., 240., 265., 300., 350., 400., 800.};
//	topetabins = {0., 0.2, 0.4, 0.6,  0.8,  1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.3, 2.8, 4.0};
//	ttmbins = {250., 350., 370., 390., 410., 430., 450., 470., 490., 510., 530., 550., 575., 600., 630., 670., 720., 770., 900, 1500.};
//	ttybins = {0., 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 3.};
//	ttptbins = {0., 20., 30., 40., 50., 60., 70., 90., 110., 140., 180., 250., 500.};
//	metbins = {0., 20., 30., 40., 50., 60., 70., 90., 110., 140., 180., 250., 1000.};
	
	topptbins = {0., 66.0, 100.0, 134.0, 174.0, 234.0, 800.};
	topetabins = {0., 0.35, 0.70, 1.05, 1.45, 1.9, 4};
	ttmbins = {280., 394.0, 442.0, 494.0, 562.0, 676.0, 2000.};
	ttybins = {0., 0.2, 0.4, 0.6, 0.8, 1.1, 3.};
	ttptbins = {0., 26.0, 42.0, 60.0, 86.0, 138.0, 800.};
	metbins = {0., 24.0, 36.0, 48.0, 66.0, 98.0, 400.};

	//vector<string> testpdf = {"CT10", "CT10as", "NNPDF30_nnlo_as_0118"};
	vector<string> testpdf = {"CT10nlo", "NNPDF30_nlo_as_0118", "MMHT2014nlo68clas118"};
	pdfunc = new PDFuncertainty("NNPDF30_nlo_as_0118", 0, testpdf);

}

void ttbar::begin()
{
	outFile_.cd();
	TDirectory* dir_gen = outFile_.mkdir("GEN");
	dir_gen->cd();
	gen1d.AddHist("TYP", 4, 0., 4., "Decay TYP", "Events");
	//gen2d.AddHist("wjets_eta", 100, 0., 5., 100, 0, 5, "W-jet #eta_{min}", "W-jet #eta_{max}");
	//gen2d.AddHist("bjets_eta", 100, 0., 5., 100, 0, 5, "b-jet #eta_{min}", "b-jet #eta_{max}");
	//gen2d.AddHist("wjets_pt", 200, 0, 400, 200, 0., 400, "W-jet p_{T,min} [GeV]", "W-jet p_{T,max} [GeV]");
	//gen2d.AddHist("bjets_pt", 200, 0, 400, 200, 0., 400, "b-jet p_{T,min} [GeV]", "b-jet p_{T,max} [GeV]");
	//gen2d.AddHist("t_pt", 200, 0, 600, 200, 0., 600, "t p_{T,min} [GeV]", "t p_{T,max} [GeV]");
	//gen1d.AddHist("bjets_dr", 100, 0, 5, "b-jets #DeltaR", "Events");
	//gen1d.AddHist("wjets_dr", 100, 0, 5, "W-jets #DeltaR", "Events");
	//gen2d.AddHist("Wmasshad_tmasshad", 2000, 0., 500., 2000, 0., 500, "M(W) (GeV)", "M(t) (GeV)");
    ttp_genall.Init(this);
    ttp_genacc.Init(this);

	TDirectory* dir_truth = outFile_.mkdir("TRUTH");
	dir_truth->cd();
	truth1d.AddHist("counter", 20, 0., 20., "counter", "Events");
	truth1d.AddHist("npu", 100, 0., 100., "npu", "Events");
	truth1d.AddHist("npuorig", 100, 0., 100., "npuorig", "Events");
	truth1d.AddHist("found", 10, 0., 10., "found", "Events");
	truth2d.AddHist("btag2d_true", 100, 0., 1., 100, 0., 1., "btag", "btag");
	truth1d.AddHist("btag_true", 100, 0., 1., "btag", "Events");
	truth1d.AddHist("btag_wrong", 100, 0., 1., "btag", "Events");
	truth1d.AddHist("wjet_sep", 100, 0., 100, "sep", "Events");	
	truth1d.AddHist("bjet_sep", 100, 0., 100, "sep", "Events");
	truth2d.AddHist("tt_jets", 3, 0., 3., 3, 0., 3., "num b-jets", "num w-jets");
	truth2d.AddHist("Jetstt_JetsAll", 5, 0., 5., 20, 0., 20., "jet tt-system", "all jets");
	truth2d.AddHist("Nmu_Ne", 5, 0., 5., 5, 0., 5., "# #mu", "# e");
	truth2d.AddHist("Ne_Nmu", 5, 0., 5., 5, 0., 5., "# e", "# #mu");
	truth2d.AddHist("dPtJet_right", 50, 0., 250., 200, -2., 2., "p_{T} [GeV]", "#Deltap_{T}/p_{T}");
	truth2d.AddHist("dPtbJet_right", 50, 0., 250., 200, -2., 2., "p_{T} [GeV]", "#Deltap_{T}/p_{T}");
	truth1d.AddHist("dRNu_right", 200, 0., 10., "#DeltaR(#nu_{gen}, #nu_{rec})", "Events");
	truth1d.AddHist("dPtNu_right", 200, -2., 2., "#Deltap_{T}/p_{T}", "Events");
	truth1d.AddHist("dPzNu_right", 200, -2., 2., "#Deltap_{z}/p_{z}", "Events");
	truth2d.AddHist("dPzNu_dPhi_right", 200, -2., 2., 100, 0., 3.15, "#Deltap_{z}/p_{z}", "#Delta#Phi(#nu, met)");
	truth2d.AddHist("dPzNu_chi2_right", 200, -2., 2., 100, 0., 100, "#Deltap_{z}/p_{z}", "#Delta#Phi(#nu, met)");
	truth1d.AddHist("dRNuMet_right", 200, 0., 10., "#DeltaR(#nu_{gen}, #nu_{rec})", "Events");
	truth1d.AddHist("dPtNuMet_right", 200, -2., 2., "#Deltap_{T}/p_{T}", "Events");
	truth2d.AddHist("Wmasshad_tmasshad_right", 500, 0., 500., 500, 0., 500, "M(W) [GeV]", "M(t) [GeV]");
	truth2d.AddHist("Wmasshad_tmasshad_wrong", 500, 0., 500., 500, 0., 500, "M(W) [GeV]", "M(t) [GeV]");
	truth2d.AddHist("Wmasslep_tmasslep_right", 500, 0., 500., 500, 0., 500, "M(W) [GeV]", "M(t) [GeV]");
	truth2d.AddHist("Wmasslep_tmasslep_wrong", 500, 0., 500., 500, 0., 500, "M(W) [GeV]", "M(t) [GeV]");
	truth2d.AddHist("Wmasshad_tmasshad_rightw", 500, 0., 500., 500, 0., 500, "M(W) [GeV]", "M(t) [GeV]");
	truth2d.AddHist("Wmasshad_tmasshad_wrongw", 500, 0., 500., 500, 0., 500, "M(W) [GeV]", "M(t) [GeV]");
	truth1d.AddHist("btagtest_wrong", 1000, -100, 100., "-Log(p) btag-test", "Events");
	truth1d.AddHist("masstest_wrong", 1000, -100, 100., "-Log(p) mass-test", "Events");
	truth1d.AddHist("nstest_wrong", 200, 0, 20., "neutrino-test", "Events");
	truth1d.AddHist("nschi_wrong", 51, -2, 100., "#chi neutrino-test", "Events");
	truth1d.AddHist("comtest_wrong", 1000, -100, 100., "-Log(p)", "Events");
	truth1d.AddHist("btagtest_right", 1000, -100, 100., "-Log(p) btag-test", "Events");
	truth1d.AddHist("masstest_right", 1000, -100, 100., "-Log(p) mass-test", "Events");
	truth1d.AddHist("nstest_right", 200, 0, 20., "neutrino-test", "Events");
	truth1d.AddHist("nschi_right", 51, -2, 100., "#chi neutrino-test", "Events");
	truth1d.AddHist("comtest_right", 1000, -100, 100., "-Log(p)", "Events");

	response.AddMatrix("thadpt", topptbins, topptbins, "p_{T}(t_{h}) [GeV]");
	response.AddMatrix("thadeta", topetabins, topetabins, "#eta(t_{h})");
	response.AddMatrix("tleppt", topptbins, topptbins, "p_{T}(t_{l}) [GeV]");
	response.AddMatrix("tlepeta", topetabins, topetabins, "#eta(t_{l})");
	response.AddMatrix("ttm", ttmbins, ttmbins, "m(t#bar{t}) [GeV]");
	response.AddMatrix("ttpt", ttptbins, ttptbins, "p_{T}(t#bar{t}) [GeV]");
	response.AddMatrix("tty", ttybins, ttybins, "y(t#bar{t})");

	pdfunc->Add1dHist("pdfunc_thadpt", topptbins, "p_{T}(t_{h}) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_tleppt", topptbins, "p_{T}(t_{l}) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_thadeta", topetabins, "#eta(t_{h}) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_tlepeta", topetabins, "#eta(t_{l}) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_ttm", ttmbins, "M(t#bar{t}) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_tty", ttybins, "y(t#bar{t})", "Events");
	pdfunc->Add1dHist("pdfunc_ttpt", ttptbins, "p_{T}(t#bar{t}) [GeV]", "Events");

	ttp_truth.Init(this);
	ttp_right.Init(this);
	ttp_wrong.Init(this);
	ttp_semi.Init(this);
	ttp_other.Init(this);

//	ttp_hadjets_right.Init(this);
//	ttp_hadjets_wrong.Init(this);
//	ttp_jets_right.Init(this);
//	ttp_jets_wrong.Init(this);
//	ttp_blep_right.Init(this);
//	ttp_blep_wrong.Init(this);
	ttp_whad_right.Init(this);
	ttp_whad_wrong.Init(this);
	ttp_jetspos_right.Init(this);
	ttp_jetspos_wrong.Init(this);

	ttp_tlepthad_right.Init(this);
	ttp_tlep_right.Init(this);
	ttp_thad_right.Init(this);
	ttp_nn_right.Init(this);
	ttp_nsemi_right.Init(this);

	btageff.Init();
	jetscale.Init("jetscale");

	TDirectory* dir_reco = outFile_.mkdir("RECO");
	dir_reco->cd();
	reco1d.AddHist("counter", 20, 0., 20., "counter", "Events");
	ttp_all.Init(this);

	//string probfilename("Prob_parton.root");
	string probfilename("Prob_parton_jeccen.root");
	//string probfilename("Prob_parton_jecp2.root");
	//string probfilename("Prob_parton_jecm2.root");
	//string probfilename("Prob_parton_jeccenS5.root");
	if(PSEUDOTOP){probfilename = "Prob_pseudo.root";}
	if(BTAGMODE)
	{
		ttsolver.Init(probfilename, false, true, true);//for btag
	}
	if(JETSCALEMODE)
	{
		ttsolver.Init(probfilename, true, true, false);//don't use mass info
	}
	else
	{
		ttsolver.Init(probfilename, false, true, true);
		//ttsolver.Init(probfilename, false, true);
	}

	TFile* f = TFile::Open("PUweight.root");
	puhist = (TH1D*)f->Get("PUweight");

}

void ttbar::SelectGenParticles(URStreamer& event)
{
	const Geninfo& info = event.genInfo();
	weight *= info.weight()/Abs(info.weight());
	int lepdecays = 0;
	int topcounter = 0;
	
	if(PSEUDOTOP)
	{
		const vector<Pst>& pseudotops = event.PSTs();
		if(pseudotops.size() == 10)
		{
			topcounter = 2;
			//for(int t = 0 ; t < 10 ; ++t) cout << t << " " << pseudotops[t].pdgId() << ", ";
			//cout << endl;

			if(Abs(pseudotops[8].pdgId()) == 11 || Abs(pseudotops[8].pdgId()) == 13)
			{
				sgenparticles.push_back(pseudotops[8]);
				gencls.push_back(&(sgenparticles.back()));
				genfincls.push_back(&(sgenparticles.back()));
				sgenparticles.push_back(pseudotops[9]);
				gennls.push_back(&(sgenparticles.back()));
				lepdecays++;
			}
			else
			{
				sgenparticles.push_back(pseudotops[8]);
				genwpartons.push_back(&(sgenparticles.back()));
				sgenparticles.push_back(pseudotops[9]);
				genwpartons.push_back(&(sgenparticles.back()));
			}

			if(Abs(pseudotops[3].pdgId()) == 11 || Abs(pseudotops[3].pdgId()) == 13)
			{
				sgenparticles.push_back(pseudotops[3]);
				gencls.push_back(&(sgenparticles.back()));
				genfincls.push_back(&(sgenparticles.back()));
				sgenparticles.push_back(pseudotops[4]);
				gennls.push_back(&(sgenparticles.back()));
				lepdecays++;
			}
			else
			{
				sgenparticles.push_back(pseudotops[3]);
				genwpartons.push_back(&(sgenparticles.back()));
				sgenparticles.push_back(pseudotops[4]);
				genwpartons.push_back(&(sgenparticles.back()));
			}

			if(pseudotops[2].pdgId() == 5)
			{
				sgenparticles.push_back(pseudotops[2]);
				genb = &(sgenparticles.back());
				sgenparticles.push_back(pseudotops[7]);
				genbbar = &(sgenparticles.back());
			}
			else
			{
				sgenparticles.push_back(pseudotops[2]);
				genbbar = &(sgenparticles.back());
				sgenparticles.push_back(pseudotops[7]);
				genb = &(sgenparticles.back());
			}
		}
		//else {cout << "no pseudotop" << endl;}
	}
	else
	{
		const vector<Genparticle>& gps = event.genParticles();
		for(vector<Genparticle>::const_iterator gp = gps.begin(); gp != gps.end(); ++gp)
		{
			//cout << gp-gps.begin() << " " << gp->pdgId() << " " << gp->status() << " " << (gp->momIdx().size() != 0 ? gps[gp->momIdx()[0]].pdgId():0) << endl;
			if(gp->status() > 21 && gp->status() < 30 && gp->momIdx().size() != 0)
			{
				//if(gp->pdgId() == 6)
				//{
				//	weight *= 1.+(gp->Pt()-200.)/1000.;
				//}
				if(Abs(gp->pdgId()) == 6)
				{
					topcounter++;
				}
				else if(gp->pdgId() == 5 && gps[gp->momIdx()[0]].pdgId() != 24)
				{
					sgenparticles.push_back(*gp);
					genb = &(sgenparticles.back());
				}
				else if(gp->pdgId() == -5 && gps[gp->momIdx()[0]].pdgId() != -24)
				{
					sgenparticles.push_back(*gp);
					genbbar = &(sgenparticles.back());
				}
				else if(Abs(gp->pdgId()) < 6 && Abs(gps[gp->momIdx()[0]].pdgId()) == 24)
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
		sort(genwpartons.begin(), genwpartons.end(), [](GenObject* A, GenObject* B){return(A->Pt() > B->Pt());});
		if(gencls[0]->pdgId() > 0){genbl = genbbar; genbh = genb;}
		else {genbh = genbbar; genbl = genb;}
		gentophad = (*genwpartons[0] + *genwpartons[1] + *genbh);
		//if(gentophad.M() > 150. && gentophad.M() < 194.) {weight /= reweighthist->Interpolate(gentophad.M());}
		//if(gentophad.M() < 150.) {weight = 0.;}
		gentoplep = (*gencls[0] + *gennls[0] + *genbl);
		if(Abs(gencls[0]->Eta()) < cletamax && gencls[0]->Pt() > clptmin && Abs(genwpartons[0]->Eta()) < cjetetamax && Abs(genwpartons[1]->Eta()) < cjetetamax && Abs(genb->Eta()) < cjetetamax && Abs(genbbar->Eta()) < cjetetamax)
		{
			if(Min(genwpartons[0]->Pt(), genwpartons[1]->Pt()) > cwjetptsoft && Max(genwpartons[0]->Pt(), genwpartons[1]->Pt()) > cwjetpthard && Min(genb->Pt(), genbbar->Pt()) > cbjetptsoft && Max(genb->Pt(), genbbar->Pt()) > cbjetpthard)
			{
				if(genwpartons[0]->DeltaR(*genwpartons[1]) > 0.4 && genwpartons[0]->DeltaR(*genb) > 0.4 && genwpartons[0]->DeltaR(*genbbar) > 0.4 && genwpartons[1]->DeltaR(*genb) > 0.4 && genwpartons[1]->DeltaR(*genbbar) > 0.4 && genb->DeltaR(*genbbar) > 0.4)
				{
					SEMILEPACC = true;
				}
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
	if(MUONS)
	{
		const vector<Muon>& muons = event.muons();
		for(vector<Muon>::const_iterator muon = muons.begin(); muon != muons.end(); ++muon)
		{
			IDMuon mu(*muon);
			if(mu.ID(IDMuon::TIGHT_12) && mu.Pt() > 10.)
			{
				smuons.push_back(mu);
				loosemuons.push_back(&(smuons.back()));
				if(mu.ID(IDMuon::TIGHT_12) && mu.Pt() > clptmin && Abs(mu.Eta()) < cletamax)
				{
					tightmuons.push_back(&(smuons.back()));
				}
			}
		}
	}

	if(ELECTRONS)
	{
		const vector<Electron>& electrons = event.electrons();
		for(vector<Electron>::const_iterator electron = electrons.begin(); electron != electrons.end(); ++electron)
		{
			IDElectron el(*electron);
			if(el.ID(IDElectron::MEDIUM_15) && el.Pt() > 10.)
			{
				selectrons.push_back(el);
				looseelectrons.push_back(&(selectrons.back()));
				if(el.ID(IDElectron::MEDIUM_15) && el.Pt() > clptmin && Abs(el.Eta()) < cletamax)
				{
					mediumelectrons.push_back(&(selectrons.back()));
				}
			}
		}
	}

	const vector<Jet>& jets = event.jets();
	for(vector<Jet>::const_iterator jetit = jets.begin(); jetit != jets.end(); ++jetit)
	{
		IDJet jet(*jetit);
		//double sf = gRandom->Gaus(1., 0.05);
		double sf = 1.0;
		jet.SetPxPyPzE(jet.Px()*sf, jet.Py()*sf, jet.Pz()*sf, jet.E()*sf);
		if(jet.Pt() < jetptmin || Abs(jet.Eta()) > cjetetamax) {continue;}
		if(!jet.ID() || !jet.Clean(loosemuons, looseelectrons)) {continue;}

		sjets.push_back(jet);
		cleanedjets.push_back(&(sjets.back()));
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
				if(SEMILEPACC){truth1d["found"]->Fill(4.5, weight);}
		}

		for(IDMuon* mu : tightmuons)
		{
			if(mu->DeltaR(*genfincls[0]) < 0.2)
			{
				rightper.L(mu);
				if(SEMILEPACC){truth1d["found"]->Fill(5.5, weight);}
			}
		}

		double ptbmax = 0.;
		double ptbbarmax = 0.;
		vector<double> wjptmax(genwpartons.size(), 0.);
		for(size_t j = 0 ; j < cleanedjets.size() ; ++j)
		{
			IDJet* jet = cleanedjets[j];
		//	double jetsep = 0;
		//	for(size_t k = 0 ; k < cleanedjets.size() ; ++k)
		//	{
		//		if(k != j && jet->DeltaR(*cleanedjets[k]) < 0.8)
		//		{
		//			jetsep += cleanedjets[k]->Pt();
		//		}
		//	}
			
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
				if(SEMILEPACC){truth1d["found"]->Fill(0.5, weight);}
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
				if(SEMILEPACC){truth1d["found"]->Fill(1.5, weight);}
			}
			if(jet->DeltaR(*genwpartons[0]) < 0.3 && jet->Pt() > wjptmax[0])
			{
				wjptmax[0] = jet->Pt();
				rightper.WJa(jet);
				if(SEMILEPACC){truth1d["found"]->Fill(2.5, weight);}
			}
			if(jet->DeltaR(*genwpartons[1]) < 0.3 && jet->Pt() > wjptmax[1])
			{
				wjptmax[1] = jet->Pt();
				rightper.WJb(jet);
				if(SEMILEPACC){truth1d["found"]->Fill(3.5, weight);}
			}
			recotherjets.push_back(jet);
		}
		//cout << rightper.IsValid() << endl;
		//if(!rightper.IsValid()) {rightper.Reset();}
	}
}

void ttbar::ttanalysis()
{
	truth1d["counter"]->Fill(19.5, weight);
	reco1d["counter"]->Fill(0.5, weight);
	if(SEMILEP) 
	{
		truth1d["counter"]->Fill(1.5, weight);
		ttp_genall.Fill(genbh, genwpartons[0], genwpartons[1], genbl, gencls[0], gennls[0], gencls[0]->pdgId(), weight);
	}
	if(SEMILEPACC)
	{
		ttp_genacc.Fill(genbh, genwpartons[0], genwpartons[1], genbl, gencls[0], gennls[0], gencls[0]->pdgId(), weight);
		truth1d["counter"]->Fill(2.5, weight);

		response.FillTruth("thadpt", gentophad.Pt(), weight);
		response.FillTruth("thadeta", Abs(gentophad.Eta()), weight);
		response.FillTruth("tleppt", gentoplep.Pt(), weight);
		response.FillTruth("tlepeta", Abs(gentoplep.Eta()), weight);
		response.FillTruth("ttm", (gentophad + gentoplep).M(), weight);
		response.FillTruth("ttpt", (gentophad + gentoplep).Pt(), weight);
		response.FillTruth("tty", Abs((gentophad + gentoplep).Rapidity()), weight);

		pdfunc->Fill1d("pdfunc_thadpt", gentophad.Pt(), weight);
		pdfunc->Fill1d("pdfunc_tleppt", gentoplep.Pt(), weight);
		pdfunc->Fill1d("pdfunc_thadeta", Abs(gentophad.Eta()), weight);
		pdfunc->Fill1d("pdfunc_tlepeta", Abs(gentoplep.Eta()), weight);
		pdfunc->Fill1d("pdfunc_ttm", (gentophad + gentoplep).M(), weight);
		pdfunc->Fill1d("pdfunc_tty", Abs((gentophad + gentoplep).Rapidity()), weight);
		pdfunc->Fill1d("pdfunc_ttpt", (gentophad + gentoplep).Pt(), weight);

		if(Abs(gencls[0]->pdgId()) == 11) {truth2d["Ne_Nmu"]->Fill(mediumelectrons.size()+0.5, tightmuons.size()+0.5, weight);}
		if(Abs(gencls[0]->pdgId()) == 13) {truth2d["Nmu_Ne"]->Fill(tightmuons.size()+0.5, mediumelectrons.size()+0.5, weight);}
	}

	//keeping only the n leading jets. 
	sort(cleanedjets.begin(), cleanedjets.end(), [](IDJet* A, IDJet* B){return(A->Pt() > B->Pt());});
	int reducedsize = Min(cleanedjets.size(), cnusedjets);
	reducedjets.resize(reducedsize);
	copy(cleanedjets.begin(), cleanedjets.begin()+reducedsize, reducedjets.begin());

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
	if(SEMILEPACC && rightper.IsComplete()){ttp_truth.Fill(rightper, lepcharge, weight);}
	if(lep == 0){return;}
	reco1d["counter"]->Fill(1.5, weight);
	if(SEMILEPACC)
	{
		truth1d["counter"]->Fill(3.5, weight);
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
	reco1d["counter"]->Fill(2.5, weight);
	if(SEMILEPACC) truth1d["counter"]->Fill(4.5, weight);

	//check for b-jets
	sort(reducedjets.begin(), reducedjets.end(), [](IDJet* A, IDJet* B){return(A->csvIncl() > B->csvIncl());});
	if((cnbtag == 1 && reducedjets[0]->csvIncl() < 0.941) || (cnbtag == 2 && reducedjets[1]->csvIncl() < 0.814)){return;}
	//int nbjets = count_if(reducedjets.begin(), reducedjets.end(), [](IDJet* A){if(A->csvIncl() > 0.814){return true;} return false;});
	//cout << nbjets << endl;
	//if((cnbtag == 1 && reducedjets[0]->csvIncl() < 0.941) || (cnbtag == 2 && (reducedjets[0]->csvIncl() < 0.941 || reducedjets[1]->csvIncl() < 0.814))){return;}
	//if(reducedjets[0]->csvIncl() > 0.814){return;}
	//if(reducedjets[0]->csvIncl() < 0.941) {return;}
	//if(reducedjets[1]->csvIncl() > 0.814 && reducedjets[2]->csvIncl() < 0.814) {cnbtag = 2;} else{cnbtag = 1;}

	reco1d["counter"]->Fill(3.5, weight);
	if(SEMILEPACC) truth1d["counter"]->Fill(5.5, weight);

	//check what we have reconstructed
	if(SEMILEP)
	{
		truth2d["tt_jets"]->Fill(rightper.NumBJets()+0.5, rightper.NumWJets()+0.5, weight);
	}

	//reconstruction
	Permutation bestper;

	if(SEMILEPACC && rightper.IsComplete()) truth1d["counter"]->Fill(6.5, weight);

	if(PSEUDOTOP)
	{
		vector<IDJet*> bjets;
		vector<IDJet*> lightjets;

		for(IDJet* j : reducedjets)
		{
			if(j->csvIncl() < 0.814) {lightjets.push_back(j);} else {bjets.push_back(j);}
		} 
		//sort(bjets.begin(), bjets.end(), [](IDJet* A, IDJet* B){return(A->Pt() > B->Pt());});
		bestper.L(lep);
		bestper.MET(&met);
		Permutation testper = bestper;
		double bestmassdiff = 100000.;
		for(int wja = 0 ; wja < lightjets.size() ; ++wja)
		{
			for(int wjb = 0 ; wjb < wja ; ++wjb)
			{
				testper.WJa(lightjets[wja]);
				testper.WJb(lightjets[wjb]);
				double testmassdiff = Abs(testper.WHad().M() - 80.);
				if(testmassdiff < bestmassdiff)
				{
					bestmassdiff = testmassdiff;
					bestper = testper;
				}
			}
		}
		bestmassdiff = 100000.;
		testper = bestper;
		for(int bja = 0 ; bja < bjets.size() ; ++bja)
		{
			for(int bjb = 0 ; bjb < bjets.size() ; ++bjb)
			{
				if(bja == bjb) continue;
				testper.BLep(bjets[bja]);
				testper.BHad(bjets[bjb]);
				if(!testper.IsComplete()) {return;}
				testper.Solve(ttsolver);
				double testmassdiff = Abs(testper.THad().M()-172.) + Abs(testper.TLep().M()-172.);
				if(testmassdiff < bestmassdiff)
				{
					bestmassdiff = testmassdiff;
					bestper = testper;
				}
			}
		}
	}
	else
	{
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
						TLorentzVector wlepmiss(*lep + met);
						TLorentzVector tlepmiss(wlepmiss + *testper.BLep());
						//cout << per++ << " " << ttsolver.NSRes() << " " << ttsolver.BTagRes() << " " << ttsolver.MassRes() << endl;
						if(rightper.IsComplete())
						{
							if(rightper.IsBLepCorrect(testper))
							{
								truth1d["nstest_right"]->Fill(ttsolver.NSRes(), weight);
								truth1d["nschi_right"]->Fill(ttsolver.NSChi2()/Sqrt(Abs(ttsolver.NSChi2())), weight);
							}
							else
							{
								truth1d["nstest_wrong"]->Fill(ttsolver.NSRes(), weight);
								truth1d["nschi_wrong"]->Fill(ttsolver.NSChi2()/Sqrt(Abs(ttsolver.NSChi2())), weight);
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
								truth2d["Wmasslep_tmasslep_right"]->Fill(wlepmiss.Mt(), tlepmiss.Mt(), weight);
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
								truth2d["Wmasslep_tmasslep_wrong"]->Fill(wlepmiss.Mt(), tlepmiss.Mt(), weight);
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
	}
	if(bestper.Prob() > 1E9){return;}
	reco1d["counter"]->Fill(4.5, weight);
	if(SEMILEPACC && rightper.IsComplete()) truth1d["counter"]->Fill(7.5, weight);
	//Fill reconstructed hists
	ttp_all.Fill(bestper, lepcharge, weight);



	//Fill reconstructed hists with matching information
	if(rightper.IsCorrect(bestper))
	{
		ttp_right.Fill(bestper, lepcharge, weight);
		if(SEMILEPACC) truth1d["counter"]->Fill(8.5, weight);
	}
	else if(rightper.IsComplete())
	{
		ttp_wrong.Fill(bestper, lepcharge, weight);
		if(SEMILEPACC) truth1d["counter"]->Fill(9.5, weight);
	}
	else if(SEMILEP)
	{
		ttp_semi.Fill(bestper, lepcharge, weight);
	}
	else
	{
		ttp_other.Fill(bestper, lepcharge, weight);
	}

	if(bestper.IsCorrect(rightper))
	{
		ttp_jetspos_right.Fill(bestper, lepcharge, weight);
		response.FillReco("thadpt", bestper.THad().Pt(), weight);
		response.FillReco("thadeta", bestper.THad().Eta(), weight);
		response.FillReco("tleppt", bestper.TLep().Pt(), weight);
		response.FillReco("tlepeta", bestper.TLep().Pt(), weight);
		response.FillReco("ttm", (bestper.THad() + bestper.TLep()).M(), weight);
		response.FillReco("ttpt", (bestper.THad() + bestper.TLep()).Pt(), weight);
		response.FillReco("tty", Abs((bestper.THad() + bestper.TLep()).Rapidity()), weight);
		if(SEMILEPACC)
		{
			truth1d["counter"]->Fill(10.5, weight);
			response.FillTruthReco("thadpt", gentophad.Pt(), bestper.THad().Pt(), weight);
			response.FillTruthReco("thadeta", Abs(gentophad.Eta()), Abs(bestper.THad().Eta()), weight);
			response.FillTruthReco("tleppt", gentoplep.Pt(), bestper.TLep().Pt(), weight);
			response.FillTruthReco("tlepeta", Abs(gentoplep.Eta()), Abs(bestper.TLep().Eta()), weight);
			response.FillTruthReco("ttm", (gentophad + gentoplep).M(), (bestper.THad() + bestper.TLep()).M(), weight);
			response.FillTruthReco("ttpt", (gentophad + gentoplep).Pt(), (bestper.THad() + bestper.TLep()).Pt(), weight);
			response.FillTruthReco("tty", Abs((gentophad + gentoplep).Rapidity()), Abs((bestper.THad() + bestper.TLep()).Rapidity()), weight);
			truth1d["dRNu_right"]->Fill(bestper.Nu().DeltaR(*gennls[0]), weight);
			truth1d["dPtNu_right"]->Fill((bestper.Nu().Pt() - gennls[0]->Pt())/gennls[0]->Pt(), weight);
			truth1d["dPzNu_right"]->Fill((bestper.Nu().Pz() - gennls[0]->Pz())/gennls[0]->Pz(), weight);
			truth2d["dPzNu_dPhi_right"]->Fill((bestper.Nu().Pz() - gennls[0]->Pz())/gennls[0]->Pz(), Abs(bestper.Nu().DeltaPhi(met)), weight);
			truth2d["dPzNu_chi2_right"]->Fill((bestper.Nu().Pz() - gennls[0]->Pz())/gennls[0]->Pz(), Sqrt(bestper.NuChisq()), weight);
			//if(Abs((bestper.Nu().Pz() - gennls[0]->Pz())/gennls[0]->Pz()) < 0.2)
			//{
			//	cout << (bestper.Nu() + *(bestper.BLep()) + *(bestper.L())).M() << " " << bestper.Nu().Pz() << " " << gennls[0]->Pz() << " "  << bestper.Nu().Pt() << " " << met.Pt() << " " << met.DeltaPhi(bestper.Nu()) << " " << bestper.NuChisq() << endl;
			//	cout << met.Px() << " " << met.Py() << endl;
			//	cout << gennls[0]->Px() << " " << gennls[0]->Py() << " " <<gennls[0]->Pz() << endl;
			//	cout << bestper.Nu().Px() << " " << bestper.Nu().Py() << " " <<bestper.Nu().Pz() << endl;
			//	NeutrinoSolver NS(bestper.L(), bestper.BLep(), 80, 173);
			//	double tchi;
			//	TLorentzVector tv(NS.GetBest(met.Px(), met.Py(), Sqrt(met.pxunc()*met.pxunc() + met.pxuncjet()*met.pxuncjet()), Sqrt(met.pyunc()*met.pyunc() + met.pyuncjet()*met.pyuncjet()), 0., tchi, true));
			//	cout << tchi << " " << tv.Pz() << " " << tv.Pt() << endl;
			//}
			truth1d["dRNuMet_right"]->Fill(met.DeltaR(*gennls[0]), weight);
			truth1d["dPtNuMet_right"]->Fill((met.Pt() - gennls[0]->Pt())/gennls[0]->Pt(), weight);
		}
	}
	else
	{
		ttp_jetspos_wrong.Fill(bestper, lepcharge, weight);
	}

	if(bestper.IsCorrect(rightper))
	{
		ttp_tlepthad_right.Fill(bestper, lepcharge, weight);
	}
	else if(bestper.IsTHadCorrect(rightper))
	{
		ttp_thad_right.Fill(bestper, lepcharge, weight);
	}
	else if(bestper.IsBLepCorrect(rightper))
	{
		ttp_tlep_right.Fill(bestper, lepcharge, weight);
	}
	else if(SEMILEP)
	{
		ttp_nn_right.Fill(bestper, lepcharge, weight);
	}
	else
	{
		ttp_nsemi_right.Fill(bestper, lepcharge, weight);
	}

	if(JETSCALEMODE)
	{
		if(bestper.IsWHadCorrect(rightper))
		{
			ttp_whad_right.Fill(bestper, lepcharge, weight);
			jetscale.Fill(bestper, true, weight);
		}
		else
		{
			ttp_whad_wrong.Fill(bestper, lepcharge, weight);
			jetscale.Fill(bestper, false, weight);
		}
	}

	if(BTAGMODE){btageff.Fill(bestper, rightper.IsCorrect(bestper), weight);}
}

//This method is called once every file, contains the event loop
//run your proper analysis here
void ttbar::analyze()
{

	int nevent = 0;
	URStreamer event(tree_);
	IDElectron::streamer = &event;
	IDMuon::streamer = &event;
	PDFuncertainty::streamer = &event;
	TRandom3 rand(12311);
	while(event.next())
	{
		nevent++;
		if(nevent % 1000 == 0)cout << "Event:" << nevent << endl;
		truth1d["counter"]->Fill(0.5);
		weight = 1.;	

		double npu = event.PUInfos()[0].nPU();
		truth1d["npuorig"]->Fill(npu, weight);
		if(npu > 0)
		{
			weight *= puhist->GetBinContent(puhist->FindFixBin(npu));
		}
		else
		{
			weight = 0.;
		}
		truth1d["npu"]->Fill(npu, weight);
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
		if(DATASIM && selectionprob > rand.Uniform())
		{
			if(selectionprob > 1.){weight *= selectionprob;}
			SelectGenParticles(event);
			SelectRecoParticles(event);
			ttanalysis();
		}

		else if(!DATASIM)
		{
			SelectGenParticles(event);
			SelectRecoParticles(event);
			ttanalysis();
		}

	}
}


//make it executable
int main(int argc, char *argv[])
{
	URParser &parser = URParser::instance(argc, argv);
	URDriver<ttbar> test;
	return test.run();
}
