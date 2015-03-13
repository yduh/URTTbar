#include <iostream>
#include <list>
#include "AnalyzerBase.h"
#include "URStreamer.h"
#include "URDriver.h"
#include "IDMuon.h"
#include "IDElectron.h"
#include "TH1F.h"
#include "TRandom3.h"

#include "TTBarPlots.h"
#include "TTBarSolver.h"
#include "Permutation.h"

using namespace std;



class ttbar : public AnalyzerBase
{
	friend class Permutation;
	private:
		//Collections
		//Gen:
		bool FULLHAD;
		bool SEMILEP;
		bool FULLLEP;
		list<Genparticle> sgenparticles;
		vector<Genparticle*> genwpartons;
		vector<Genparticle*> gencls;
		vector<Genparticle*> gennls;
		Genparticle* genb;
		Genparticle* genbbar;
		Genparticle* genbl;
		Genparticle* genbh;

		//matched
		vector<Jet*> recbjets;
		vector<Jet*> recwjets;
		vector<Jet*> recotherjets;
		Jet* recbjet;
		Jet* recbbarjet;
		Jet* recbhjet;
		Jet* recbljet;
		int nttjets;

		//reco
		list<Jet> sjets;
		vector<Jet*> cleanedjets;
		list<IDMuon> smuons;
		vector<IDMuon*> loosemuons;
		vector<IDMuon*> tightmuons;
		list<IDElectron> selectrons;
		vector<IDElectron*> looseelectrons;
		vector<IDElectron*> mediumelectrons;
		Met met;

		//hists
		TH1DCollection gen1d;
		TH2DCollection gen2d;
		TH1DCollection reco1d;
		TH2DCollection reco2d;
		TH1DCollection truth1d;
		TH2DCollection truth2d;
		TTBarPlots ttp_right;
		TTBarPlots ttp_wrong;
		TTBarPlots ttp_semi;
		TTBarPlots ttp_other;
		TTBarPlots ttp_truth;
		TTBarPlots ttp_all;
		TTBarPlots ttp_right_imp;
		TTBarPlots ttp_wrong_imp;
		TTBarPlots ttp_semi_imp;
		TTBarPlots ttp_other_imp;
		TTBarPlots ttp_all_imp;

		//ttbar solver
		TTBarSolver ttsolver;

		//configuration
		int cnbtag;
		double cwjetptsoft;
		double cwjetpthard;
		double cbjetpt;
		double cjetetamax;
		double clptmin;
		//
		double jetptmin;

	public:
		ttbar(const std::string output_filename):
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
			ttp_truth("truth"),
			ttp_all("all"),
			ttp_right_imp("right_imp"),
			ttp_wrong_imp("wrong_imp"),
			ttp_semi_imp("semi_imp"),
			ttp_other_imp("other_imp"),
			ttp_all_imp("all_imp"),
			cnbtag(1),
			cwjetptsoft(30.),
			cwjetpthard(30.),
			cbjetpt(30.),
			cjetetamax(2.4),
			clptmin(30.)
	{
		jetptmin = min(cwjetptsoft, cbjetpt);
	};

		//This method is called once per job at the beginning of the analysis
		//book here your histograms/tree and run every initialization needed
		virtual void begin()
		{
			outFile_.cd();
			TDirectory* dir_gen = outFile_.mkdir("GEN");
			dir_gen->cd();
			gen1d.AddHist("JM1", 1000, 0., 5000., "JM1", "Events");
			gen1d.AddHist("TYP", 4, 0., 4., "Decay TYP", "Events");
			gen2d.AddHist("wjets_eta", 100, 0., 5., 100, 0, 5, "W-jet #eta_{min}", "W-jet #eta_{max}");
			gen2d.AddHist("bjets_eta", 100, 0., 5., 100, 0, 5, "b-jet #eta_{min}", "b-jet #eta_{max}");
			gen2d.AddHist("wjets_pt", 200, 0, 400, 200, 0., 400, "W-jet p_{T,min} (GeV)", "W-jet p_{T,max} (GeV)");
			gen2d.AddHist("bjets_pt", 200, 0, 400, 200, 0., 400, "b-jet p_{T,min} (GeV)", "b-jet p_{T,max} (GeV)");
			gen1d.AddHist("bjets_dr", 100, 0, 5, "b-jets #DeltaR", "Events");
			gen1d.AddHist("wjets_dr", 100, 0, 5, "W-jets #DeltaR", "Events");

			TDirectory* dir_truth = outFile_.mkdir("TRUTH");
			dir_truth->cd();
			truth2d.AddHist("btag2d_true", 100, 0., 1., 100, 0., 1., "btag", "btag");
			truth1d.AddHist("btag_true", 100, 0., 1., "btag", "Events");
			truth1d.AddHist("counter", 10, 0., 10., "counter", "Events");
			truth1d.AddHist("btag_wrong", 100, 0., 1., "btag", "Events");
			truth1d.AddHist("btagtest", 10, 0., 10., "btagtest", "Events");
			truth2d.AddHist("test_dPhi", 100, 0., 10000., 100, -2., 2., "min chi2", "#Delta#Phi");
			truth2d.AddHist("test_dPhiMET", 100, 0., 10000., 100, -2., 2., "min chi2", "#Delta#Phi");
			truth1d.AddHist("otherjets_pt", 500, 0., 500., "p_{T} (GeV)", "Events");
			truth1d.AddHist("otherjets_sep", 100, 0., 5., "min sep", "Events");
			truth2d.AddHist("wjets_sep", 100, 0., 5., 80, -4., 4., "min sep", "resolution");	
			truth2d.AddHist("bjets_sep", 100, 0., 5., 80, -4., 4., "min sep", "resolution");
			truth1d.AddHist("wmass_lowsep", 75, 0., 150, "low separation M(jj) (GeV)", "Events");
			truth1d.AddHist("wmass_highsep", 75, 0., 150, "high separation M(jj) (GeV)", "Events");
			truth2d.AddHist("tt_jets", 3, 0., 3., 3, 0., 3., "num b-jets", "num w-jets");
			truth2d.AddHist("Jetstt_JetsAll", 5, 0., 5., 20, 0., 20., "jet tt-system", "all jets");
			truth1d.AddHist("dRNu_right", 200, 0., 10., "#DeltaR(#nu_{gen}, #nu_{rec})", "Events");
			truth1d.AddHist("dPtNu_right", 200, -2., 2., "#Deltap_{T}/p_{T}", "Events");
			truth1d.AddHist("dRNuMet_right", 200, 0., 10., "#DeltaR(#nu_{gen}, #nu_{rec})", "Events");
			truth1d.AddHist("dPtNuMet_right", 200, -2., 2., "#Deltap_{T}/p_{T}", "Events");
			truth1d.AddHist("dRNu_wrong", 200, 0., 10., "#DeltaR(#nu_{gen}, #nu_{rec})", "Events");
			truth1d.AddHist("dPtNu_wrong", 200, -2., 2., "#p_{T} (GeV)", "Events");
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
			truth1d.AddHist("nschi_wrong", 200, 0, 20., "#chi2 neutrino-test", "Events");
			truth1d.AddHist("comtest_wrong", 1000, -100, 100., "-Log(p)", "Events");
			truth1d.AddHist("btagtest_right", 1000, -100, 100., "-Log(p) btag-test", "Events");
			truth1d.AddHist("masstest_right", 1000, -100, 100., "-Log(p) mass-test", "Events");
			truth1d.AddHist("nstest_right", 200, 0, 20., "neutrino-test", "Events");
			truth1d.AddHist("nschi_right", 200, 0, 20., "#chi2 neutrino-test", "Events");
			truth1d.AddHist("comtest_right", 1000, -100, 100., "-Log(p)", "Events");

			ttp_right.Init();
			ttp_wrong.Init();
			ttp_semi.Init();
			ttp_other.Init();
			ttp_right_imp.Init();
			ttp_wrong_imp.Init();
			ttp_semi_imp.Init();
			ttp_other_imp.Init();
			ttp_truth.Init();

			TDirectory* dir_reco = outFile_.mkdir("RECO");
			dir_reco->cd();
			ttp_all.Init();
			ttp_all_imp.Init();

			ttsolver.Init("Prob.root");
		}

		void SelectGenParticles(URStreamer& event)
		{
			const vector<Genparticle>& gps = event.genParticles();
			//cout <<gps.size() << endl;
			//loop over muons (by reference)
			int lepdecays = 0;
			for(vector<Genparticle>::const_iterator gp = gps.begin(); gp != gps.end(); ++gp)
			{
				//	if(Abs(gp->pdgId()) > 10 && Abs(gp->pdgId()) < 18)
				//	{
				//		cout << "P:"  << " " << gp->status() << " " << gp->pdgId() << ":";
				//		for(size_t m = 0 ; m < gp->momIdx().size() ; ++m) cout << " " << gps[gp->momIdx()[m]].pdgId();
				//		cout << endl;
				//	}
				if(gp->status() == 23 && gp->momIdx().size() != 0)
				{
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
				if(gp->status() == 1 && gp->momIdx().size() != 0 && (Abs(gps[gp->momIdx()[0]].pdgId()) == 24 || gp->pdgId() == gps[gp->momIdx()[0]].pdgId()))
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
			}
			FULLHAD = false;
			SEMILEP = false;
			FULLLEP = false;
			if(lepdecays == 2 && genwpartons.size() == 0)
			{ 
				FULLLEP = true; gen1d["TYP"]->Fill(0.5);
			}
			else if(lepdecays == 1 && genwpartons.size() == 2)
			{
				gen1d["TYP"]->Fill(1.5);
			}
			else if(lepdecays == 0 && genwpartons.size() == 4)
			{
				FULLHAD = true; gen1d["TYP"]->Fill(2.5);
			}
			else
			{
				gen1d["TYP"]->Fill(3.5);
			}

			if(genwpartons.size() == 2 && gencls.size() == 1 && gennls.size() == 1 && genb != 0 && genbbar != 0)//no tau
			{
				SEMILEP = true;
				if(gencls[0]->pdgId() > 0){genbl = genbbar; genbh = genb;}
				else {genbh = genbbar; genbl = genb;}
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

		void SelectRecoParticles(URStreamer& event)
		{
			const vector<Muon>& muons = event.muons();
			for(vector<Muon>::const_iterator muon = muons.begin(); muon != muons.end(); ++muon)
			{
				IDMuon mu(*muon);
				if(mu.ID(IDMuon::TIGHT_12) && mu.Pt() > 15.)
				{
					smuons.push_back(mu);
					loosemuons.push_back(&(smuons.back()));
					if(mu.ID(IDMuon::TIGHT_12) && mu.Pt() > clptmin)
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
					if(el.ID(IDElectron::MEDIUM_12) && el.Pt() > clptmin)
					{
						mediumelectrons.push_back(&(selectrons.back()));
					}
				}
			}

			const vector<Jet>& jets = event.jets();
			for(vector<Jet>::const_iterator jet = jets.begin(); jet != jets.end(); ++jet)
			{
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

			//Jet-Parton matching
			if(SEMILEP)
			{
				double ptbmax = 0.;
				double ptbbarmax = 0.;
				vector<double> wjptmax(genwpartons.size(), 0.);
				for(size_t j = 0 ; j < cleanedjets.size() ; ++j)
				{
					Jet* jet = cleanedjets[j];
					if(jet->DeltaR(*genb) < 0.3 && jet->Pt() > ptbmax)
					{
						ptbmax = jet->Pt();
						recbjet = jet;
						recbjets.push_back(jet);
						if(gencls[0]->pdgId() < 0)
						{
							recbljet = jet;
						}
						else if(gencls[0]->pdgId() > 0)
						{
							recbhjet = jet;
						}
						continue;
					}
					if(jet->DeltaR(*genbbar) < 0.3 && jet->Pt() > ptbbarmax)
					{
						ptbbarmax = jet->Pt();
						recbbarjet = jet;
						recbjets.push_back(jet);
						if(gencls[0]->pdgId() < 0)
						{
							recbhjet = jet;
						}
						else if(gencls[0]->pdgId() > 0)
						{
							recbljet = jet;
						}
						continue;
					}
					for(size_t wp = 0 ; wp < genwpartons.size() ; ++wp)
					{
						if(jet->DeltaR(*genwpartons[wp]) < 0.3 && jet->Pt() > wjptmax[wp])
						{
							wjptmax[wp] = jet->Pt();
							recwjets.push_back(jet);
							break;
						}
					}
					recotherjets.push_back(jet);
				}
				nttjets = (recbjet != 0 ? 1 : 0) + (recbbarjet != 0 ? 1 : 0) + recwjets.size();
			}


		}

		void ttanalysis()
		{
			if(SEMILEP) truth1d["counter"]->Fill(1.5);
			if(cleanedjets.size() < 4){return;}
			if(SEMILEP) truth1d["counter"]->Fill(2.5);

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
			if(SEMILEP) truth1d["counter"]->Fill(3.5);
			if(SEMILEP && lep->DeltaR(*gencls[0]) > 0.2) {cout << "Wrong Lep" << endl;}

			//keeping only the 5 leading jets
			sort(cleanedjets.begin(), cleanedjets.end(), [](Jet* A, Jet* B){return(A->Pt() > B->Pt());});
			int reducedsize = Min(cleanedjets.size(), (size_t)5000);
			vector<Jet*> reducedjets(reducedsize);
			copy(cleanedjets.begin(), cleanedjets.begin()+reducedsize, reducedjets.begin());

			//check for 2 b-jets
			sort(reducedjets.begin(), reducedjets.end(), [](Jet* A, Jet* B){return(A->csvIncl() > B->csvIncl());});
			if((cnbtag == 1 && reducedjets[0]->csvIncl() < 0.941) || (cnbtag == 2 && reducedjets[1]->csvIncl() < 0.814)){return;}
			if(SEMILEP) truth1d["counter"]->Fill(4.5);

			//check what we have reconstructed
			if(SEMILEP)
			{
				truth2d["Jetstt_JetsAll"]->Fill(nttjets+0.5, cleanedjets.size()+0.5);
				int nb = 0;
				if(recbljet != 0){nb++;}
				if(recbhjet != 0){nb++;}
				truth2d["tt_jets"]->Fill(nb+0.5, recwjets.size()+0.5);
			}

			//reconstruction
			Permutation rightper;
			if(nttjets == 4)
			{
				rightper = Permutation(recwjets[0], recwjets[1], recbhjet, recbljet, lep, &met);
			}
			Permutation bestper;

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
							Jet* wja = reducedjets[i];
							Jet* wjb = reducedjets[j];
							Jet* bjh = reducedjets[k];
							Jet* bjl = reducedjets[l];
							if(wja->Pt() < cwjetpthard && wjb->Pt() < cwjetpthard) continue;
							if(wja->Pt() < cwjetptsoft || wjb->Pt() < cwjetptsoft) continue;
							if(bjh->Pt() < cbjetpt || bjl->Pt() < cbjetpt) continue;
							Permutation testper(wja, wjb, bjh, bjl, lep, &met);
							testper.Solve(ttsolver);

							TLorentzVector whad(testper.WHad());
							TLorentzVector thad(testper.THad());

							if(nttjets == 4)
							{
								if(rightper.IsBLepCorrect(testper))
								{
									truth1d["nstest_right"]->Fill(ttsolver.NSRes());
									truth1d["nschi_right"]->Fill(ttsolver.NSChi2());
								}
								else
								{
									truth1d["nstest_wrong"]->Fill(ttsolver.NSRes());
									truth1d["nschi_wrong"]->Fill(ttsolver.NSChi2());
								}

								if(rightper.IsBCorrect(testper))
								{
									truth1d["btagtest_right"]->Fill(ttsolver.BTagRes());
								}
								else
								{
									truth1d["btagtest_wrong"]->Fill(ttsolver.BTagRes());
								}

								if(rightper.IsCorrect(testper))
								{
									truth2d["Wmasshad_tmasshad_right"]->Fill(whad.M(), thad.M());
									truth1d["masstest_right"]->Fill(ttsolver.MassRes());
									truth1d["comtest_right"]->Fill(ttsolver.Res());
								}
								else
								{
									if(rightper.IsWHadCorrect(testper))
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
							if(bestper.BLep() == 0) {bestper = testper; continue;}

							if(testper < bestper)
							{
								bestper = testper;
							}
						}
					}
				}
			}
			if(bestper.Prob() > 1E9){return;}
			if(SEMILEP) truth1d["counter"]->Fill(5.5);
			//Fill reconstructed hists
			ttp_all.Fill(bestper, lepcharge);
			//ttp_all_imp.Fill(&ImpBHad, &ImpWja, &ImpWjb, &ImpBLep, &ImpL, &ImpNu, lepcharge);
			//
		
			bool coin = (gRandom->Uniform() < 0.5);
			Jet* bjet = 0;
			if(coin)
			{
				if(bestper.BLep()->csvIncl() > 0.941) bjet = bestper.BHad();
			}
			else
			{
				if(bestper.BHad()->csvIncl() > 0.941) bjet = bestper.BLep();
			}


			//Fill reconstructed hists with matching information
			if(rightper.IsCorrect(bestper))
			{
				ttp_right.Fill(bestper, lepcharge, bjet);
				if(SEMILEP) truth1d["counter"]->Fill(6.5);
				//ttp_right_imp.Fill(&ImpBHad, &ImpWja, &ImpWjb, &ImpBLep, &ImpL, &ImpNu, lepcharge);
				//Neutrino reconstruction plots
				//truth1d["dRNu_right"]->Fill(ImpNu.DeltaR(*gennls[0]));
				//truth1d["dPtNu_right"]->Fill((ImpNu.Pt() - gennls[0]->Pt())/gennls[0]->Pt());
				//truth1d["dRNuMet_right"]->Fill(met.DeltaR(*gennls[0]));
				//truth1d["dPtNuMet_right"]->Fill((met.Pt() - gennls[0]->Pt())/gennls[0]->Pt());
			}
			else if(nttjets == 4)
			{
				ttp_wrong.Fill(bestper, lepcharge, bjet);
				if(SEMILEP) truth1d["counter"]->Fill(7.5);
				//ttp_wrong_imp.Fill(&ImpBHad, &ImpWja, &ImpWjb, &ImpBLep, &ImpL, &ImpNu, lepcharge);
				//Neutrino reconstruction plots
				//truth1d["dRNu_wrong"]->Fill(ImpNu.DeltaR(*gennls[0]));
				//truth1d["dPtNu_wrong"]->Fill((ImpNu.Pt() - gennls[0]->Pt())/gennls[0]->Pt());
			}
			else if(SEMILEP)
			{
				ttp_semi.Fill(bestper, lepcharge, bjet);
				//ttp_semi_imp.Fill(&ImpBHad, &ImpWja, &ImpWjb, &ImpBLep, &ImpL, &ImpNu, lepcharge);
			}
			else
			{
				ttp_other.Fill(bestper, lepcharge, bjet);
				//ttp_other_imp.Fill(&ImpBHad, &ImpWja, &ImpWjb, &ImpBLep, &ImpL, &ImpNu, lepcharge);
			}

			//plot true tt 
	//		if(nttjets != 4){return;}
	//		ttp_truth.Fill(recbhjet, recwjets[0], recwjets[1], recbljet, lep, gennls[0], lepcharge);
	//		for(size_t oj = 0 ; oj < recotherjets.size() ; ++oj)
	//		{
	//			truth1d["otherjets_pt"]->Fill(recotherjets[oj]->Pt());
	//		}

	//		vector<Jet*> ttjets = {recbjet, recbbarjet, recwjets[0], recwjets[1]};
	//		Jet* ttjmin = *min_element(ttjets.begin(), ttjets.end(), [](Jet* a, Jet* b){return(a->Pt() < b->Pt());});
	//		int numhigherpt = count_if(cleanedjets.begin(), cleanedjets.end(), [&](Jet* a){return(a->Pt() > ttjmin->Pt());});
	//		truth2d["jetnum_highptjets"]->Fill(cleanedjets.size(), numhigherpt);

	//		truth2d["btag2d_true"]->Fill(Min(recbjet->csvIncl(), recbbarjet->csvIncl()), Max(recbjet->csvIncl(), recbbarjet->csvIncl()));
	//		truth1d["btag_true"]->Fill(recbjet->csvIncl());
	//		truth1d["btag_true"]->Fill(recbbarjet->csvIncl());
	//		truth1d["btag_wrong"]->Fill(recwjets[0]->csvIncl());
	//		truth1d["btag_wrong"]->Fill(recwjets[1]->csvIncl());


		}

		//This method is called once every file, contains the event loop
		//run your proper analysis here
		virtual void analyze()
		{

			int nevent = 0;
			URStreamer event(tree_);
			while(event.next())
			{
				nevent++;
				if(nevent % 1000 == 0)cout << "Event:" << nevent << endl;
				truth1d["counter"]->Fill(0.5);	
				//TLorentzVector tl(20., 10., 10., Sqrt(600));
				//TLorentzVector tb(10., -30., -10., Sqrt(1200));
				//NeutrinoSolver NS(&tl, &tb, 80., 173.);
				//double chi;
				//TLorentzVector met(NS.GetBest(-20., 10., 1.,1.,0, chi));
				//cout << chi << " " << (tl+met).M() << " " << (tl+tb+met).M() << endl; 


				sgenparticles.clear();
				genwpartons.clear();
				gencls.clear();
				gennls.clear();
				genb = 0;
				genbbar = 0;
				genbl = 0;
				genbh = 0;

				recbjets.clear();
				recwjets.clear();
				recotherjets.clear();
				recbjet = 0;
				recbbarjet = 0;
				recbhjet = 0;
				recbljet = 0;

				sjets.clear();
				cleanedjets.clear();
				smuons.clear();
				tightmuons.clear();
				loosemuons.clear();
				selectrons.clear();
				mediumelectrons.clear();
				looseelectrons.clear();

				nttjets = 0;

				SelectGenParticles(event);
				SelectRecoParticles(event);

				ttanalysis();

			}
		}

		//this method is called at the end of the job, by default saves
		//every histogram/tree produced, override it if you need something more
		//virtual void end();

		//do you need command-line or cfg options? If so implement this 
		//method to book the options you need. CLI parsing is provided
		//by AnalysisFW/interface/URParser.h and uses boost::program_options
		//look here for a quickstart tutorial: 
		//http://www.boost.org/doc/libs/1_51_0/doc/html/program_options/tutorial.html
		static void setOptions() {}
	private:
		// Nothing by default
		// Add your private variables/methods here

};

//make it executable
int main(int argc, char *argv[])
{
	URParser &parser = URParser::instance(argc, argv);
	URDriver<ttbar> test;
	return test.run();
}
