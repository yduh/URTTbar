#include <iostream>
#include "AnalyzerBase.h"
#include "URStreamer.h"
#include "URDriver.h"
#include "genSelection.h"
#include <map>
#include "RObject.h"
#include "IDMuon.h"
#include "IDElectron.h"
#include <algorithm>
//#include "NeutrinoSolver.h"
#include "TTBarSolver.h"
#include <list>
#include "Logger.h"
#include "URParser.h"
#include "CutFlowTracker.h"
#include <boost/algorithm/string/predicate.hpp>
#include "PDGID.h"

class btag_efficiency : public AnalyzerBase
{
public:
  btag_efficiency(const std::string output_filename):
    AnalyzerBase("btag_efficiency", output_filename), 
		tracker_(),
		solver_(),
		ordering_()
	{
		solver_.Init("Prob.root");
		opts::variables_map &values = URParser::instance().values();
		string output_file = values["output"].as<std::string>();
		size_t slash = output_file.rfind("/") + 1;
		string basename(output_file, slash, output_file.size() - slash);
		isData_  = boost::starts_with(basename, "data");
		isTTbar_ = boost::starts_with(basename, "ttJets");

		btag_id_ = &Jet::csvIncl;
		btag_cut_ = 0.941;
		bjet_pt_cut_ = 30;
		DR_match_to_gen_ = 0.3;
		ordering_["nu_chisq"         ] = [](const TTbarHypothesis &one, const TTbarHypothesis &two) {return  one.nu_chisq          < two.nu_chisq;};
		ordering_["nu_discriminant"	 ] = [](const TTbarHypothesis &one, const TTbarHypothesis &two) {return  one.nu_discriminant   < two.nu_discriminant;};
		ordering_["btag_discriminant"] = [](const TTbarHypothesis &one, const TTbarHypothesis &two) {return  one.btag_discriminant < two.btag_discriminant;};
		ordering_["mass_discriminant"] = [](const TTbarHypothesis &one, const TTbarHypothesis &two) {return  one.mass_discriminant < two.mass_discriminant;}; 
		ordering_["full_discriminant"] = [](const TTbarHypothesis &one, const TTbarHypothesis &two) {return  one.full_discriminant < two.full_discriminant;};
	}
  
	TDirectory* getDir(string path){
		TDirectory* out = (TDirectory*) outFile_.Get(path.c_str());
		if(out) return out;
		outFile_.mkdir(path.c_str());
		return (TDirectory*) outFile_.Get(path.c_str());
	}

	template <class H, typename ... Args>
	void book(string folder, string name, Args ... args)
	{
		getDir(folder)->cd();
		histos_[folder][name] = RObject::book<H>(name.c_str(), args ...);
	}

  //This method is called once per job at the beginning of the analysis
  //book here your histograms/tree and run every initialization needed
	void book_combo_plots(string folder){
			book<TH1F>(folder, "nu_chisq"         , "", 300,   0., 30.);
			book<TH1F>(folder, "nu_discriminant"	, "", 110,  -1., 10.);
			book<TH1F>(folder, "btag_discriminant", "", 300, -30.,  0.);
			book<TH1F>(folder, "mass_discriminant", "", 100,   0., 10.);
			book<TH1F>(folder, "full_discriminant", "", 200, -10., 10.);
	}

	void book_hyp_plots(string folder){
		book<TH1F>(folder, "njets"    , "", 50, 0., 50.);
		book<TH1F>(folder, "nbjets"   , "", 50, 0., 50.);
		book<TH1F>(folder, "b_pt"     , ";p_{T}(b) (GeV)", 500, 0., 500.);
		book<TH1F>(folder, "bbar_pt"  , ";p_{T}(b) (GeV)", 500, 0., 500.);
		book<TH1F>(folder, "lep_pt"   , ";p_{T}(#ell) (GeV)", 500, 0., 500.);
		book<TH1F>(folder, "Wlep_mass", ";m_{W}(lep) (GeV)", 140, 0., 140.);
		book<TH1F>(folder, "Wlep_char", ";charge_{W}(lep) (GeV)", 2, -1, 1);
		book<TH1F>(folder, "Whad_mass", ";m_{W}(had) (GeV)", 140, 0., 140.);
		book<TH1F>(folder, "tlep_mass", ";m_{t}(lep) (GeV)", 300, 100., 400.);
		book<TH1F>(folder, "thad_mass", ";m_{t}(had) (GeV)", 300, 100., 400.);
		book<TH1F>(folder, "hjet_pt"  , ";m_{t}(lep) (GeV)", 500, 0., 500.);
		book<TH2F>(folder, "hjet_pts" , ";m_{t}(had) (GeV)", 500, 0., 500., 500, 0., 500.);
		book<TH2F>(folder, "hjet_es"  , ";m_{t}(had) (GeV)", 500, 0., 500., 500, 0., 500.);
	}

	void book_jet_plots(string folder){
	}

  virtual void begin()
  {
    outFile_.cd();
		
		string criteria[] = {"nu_chisq", "nu_discriminant", "btag_discriminant", "mass_discriminant", "full_discriminant"};
		string folders[] = {"all", "semilep_visible_right", "semilep_right_thad", 
												"semilep_right_tlep", "semilep_right_whad", "semilep_wrong", "other"};
		string wjet_folders[] = {"leading", "subleading"};
		string subf[] = {"tagged", "all"};

		if(isTTbar_) book_hyp_plots("gen");
		for(auto& genCategory : folders){			
			book_combo_plots(genCategory+"/discriminators");
			for(auto& criterion : criteria){
				string folder = genCategory + "/" + criterion;
				Logger::log().debug() << "creating plots for folder: " << folder << std::endl;

				book_combo_plots(folder);
				book_hyp_plots(folder);

				for(auto& w_folder : wjet_folders){
					for(auto& j_folder : subf){
						string current = folder + "/" + w_folder + "/" + j_folder;
						book<TH1F>(current, "eta"	,"eta"	, 100, -5, 5);
						book<TH1F>(current, "pt" 	,"pt" 	, 500, 0 , 500);
						book<TH1F>(current, "phi"	,"phi"	, 400, -4, 4);
						book<TH1F>(current, "pflav","pflav", 49, -24.5, 24.5);
						book<TH1F>(current, "hflav","hflav", 49, -24.5, 24.5);
						book<TH1F>(current, "energy", ";E_{jet} (GeV)", 500, 0., 500.);
					}
				}

				if(folder == "semilep_visible_right"){
					book<TH1F>(folder, "nu_DR"    , "#DeltaR between gen and reco #nu;#DeltaR;counts", 140, 0., 7.);
					book<TH1F>(folder, "nu_DE"    , "#DeltaE between gen and reco #nu;#DeltaE (GeV);counts", 250, -250, 250.);
				}
			}//*/
			if(!isTTbar_) break;
		}

		string folder = "preselection";
		book<TH1F>(folder, "njets"    , "", 50, 0., 50.);
		book<TH1F>(folder, "nbjets"   , "", 50, 0., 50.);
		book<TH1F>(folder, "lep_pt"   , ";p_{T}(#ell) (GeV)", 500, 0., 500.);
		book<TH1F>(folder, "lep_char" , ";charge_{W}(lep) (GeV)", 2, -1, 1);
		book<TH1F>(folder, "tlep_char", "Tight leptons charge;charge_{#ell}", 2, -1, 1);
		book<TH1F>(folder, "mu_char"  , "Tight muons charge;charge_{#mu}", 2, -1, 1);
		book<TH1F>(folder, "el_char"  , "Tight electrons charge;charge_{e}", 2, -1, 1);
		book<TH1F>(folder, "cjets_pt" , "Charm jet pt;p_{T} (GeV)", 500, 0., 500.);
		book<TH1F>(folder, "cbjets_pt", "B-Tagged charm jet pt;p_{T} (GeV)", 500, 0., 500.);
  }

	void fill_gen_info(string folder, const TTbarHypothesis &reco, const TTbarHypothesis &gen){
		auto dir = histos_.find(folder);
    if(dir == histos_.end()) {
			Logger::log().error() << "histogram folder: " << folder <<
        " does not exists!" << endl;
      return;
    }
		const WHypothesis &reco_wlep = reco.wplus.isLeptonic ? reco.wplus : reco.wminus;
    const WHypothesis &gen_wlep  = reco.wplus.isLeptonic ? gen.wplus : gen.wminus;
		dir->second["nu_DR"].fill(reco_wlep.second->DeltaR(*gen_wlep.second));
		dir->second["nu_DE"].fill(reco_wlep.second->E() - gen_wlep.second->E());
	}

	void fill_jet_info(string folder, const Jet* jet){
		auto dir = histos_.find(folder);
		dir->second["eta"	 ].fill(jet->Eta());
		dir->second["pt" 	 ].fill(jet->Pt() );
		dir->second["phi"	 ].fill(jet->Phi());
		dir->second["pflav"].fill(jet->partonFlavour());
		dir->second["hflav"].fill(jet->hadronFlavour());
		dir->second["energy"].fill(jet->E());
	}

	void fill_discriminator_info(string folder, const TTbarHypothesis &hyp){
		//Logger::log().warning() << "filling " << hyp.btag_discriminant << endl;
		auto dir = histos_.find(folder);
		dir->second["nu_chisq"         ].fill(hyp.nu_chisq        );
		dir->second["nu_discriminant"	 ].fill(hyp.nu_discriminant	);
		dir->second["btag_discriminant"].fill(hyp.btag_discriminant);
		dir->second["mass_discriminant"].fill(hyp.mass_discriminant);
		dir->second["full_discriminant"].fill(hyp.full_discriminant);
	}

	void fill(string folder, const TTbarHypothesis &hyp, size_t njets, size_t nbjets) {
		auto dir = histos_.find(folder);
		if(dir == histos_.end()) {
			Logger::log().error() << "histogram folder: " << folder <<
				" does not exists!" << endl;
			return;
		}

		fill_discriminator_info(folder, hyp);
		
		const WHypothesis &wlep = hyp.wplus.isLeptonic ? hyp.wplus : hyp.wminus;
		const WHypothesis &whad = hyp.wplus.isLeptonic ? hyp.wminus : hyp.wplus;

		double tlep_m = hyp.wplus.isLeptonic ? hyp.top_mass() : hyp.tbar_mass();
		double thad_m = hyp.wplus.isLeptonic ? hyp.tbar_mass() : hyp.top_mass();
		double charge = hyp.wplus.isLeptonic ? 0.5 : -0.5;

		dir->second["Wlep_char"].fill(charge);
		dir->second["njets"    ].fill(njets );
		dir->second["nbjets"   ].fill(nbjets);
		dir->second["b_pt"     ].fill(hyp.b->Pt());
		dir->second["bbar_pt"  ].fill(hyp.bbar->Pt());
		dir->second["lep_pt"   ].fill(wlep.first->Pt());
		dir->second["Wlep_mass"].fill(wlep.mass());
		dir->second["Whad_mass"].fill(whad.mass());
		dir->second["tlep_mass"].fill(tlep_m);
		dir->second["thad_mass"].fill(thad_m);
		dir->second["hjet_pt"  ].fill(whad.first->Pt());
		dir->second["hjet_pt"  ].fill(whad.second->Pt());
		float lead = (whad.first->Pt() > whad.second->Pt()) ? whad.first->Pt() : whad.second->Pt();
		float sub  = (whad.first->Pt() > whad.second->Pt()) ? whad.second->Pt() : whad.first->Pt();
		dir->second["hjet_pts" ].fill(lead, sub);//*/

		const Jet *leading    = (const Jet*)((whad.first->E() > whad.second->E()) ? whad.first  : whad.second);
		const Jet *subleading = (const Jet*)((whad.first->E() > whad.second->E()) ? whad.second : whad.first );
		
		//FILL JET INFORMATION
		fill_jet_info(folder + "/leading/all", leading);
		if((leading->*btag_id_)() > btag_cut_) fill_jet_info(folder + "/leading/tagged", leading);
		fill_jet_info(folder + "/subleading/all", subleading);
		if((subleading->*btag_id_)() > btag_cut_) fill_jet_info(folder + "/subleading/tagged", subleading);

		dir->second["hjet_es"  ].fill(leading->E(), subleading->E());
	}

	string get_ttdir_name(TTbarHypothesis &gen, TTbarHypothesis &reco){
		// Logger::log().warning() << "fcn called: " << gen.decay << " (SEMI = "<< DecayType::SEMILEP <<")" << endl;
		if(gen.decay == DecayType::SEMILEP){
			//if(!gen.hasMissingProng()){
			Logger::log().debug() << "--------------------------------------" << endl;
			Logger::log().debug() << "RECO: wh " << reco.whad()->first << " " << reco.whad()->second << " wl " <<
				reco.wlep()->first << " b " << reco.b << " bb " << reco.bbar << endl;
			Logger::log().debug() << "GEN: wh " << gen.whad()->first << " " << gen.whad()->second << " wl " <<
				gen.wlep()->first << " b " << gen.b << " bb " << gen.bbar << endl;
				//}
			bool whad_matches = (reco.whad()->first == gen.whad()->first && reco.whad()->second == gen.whad()->second) ||
				(reco.whad()->first == gen.whad()->second && reco.whad()->second == gen.whad()->first);
			bool b_matches    = (reco.bhad() == gen.bhad());
			bool lepb_matches = (reco.blep() == gen.blep());
			bool lep_matches  = (reco.wlep()->first == gen.wlep()->first);
			if(lepb_matches && lep_matches && whad_matches && b_matches) return "semilep_visible_right";
			else if(whad_matches && b_matches) return "semilep_right_thad";
			else if(whad_matches) return "semilep_right_whad";
			else if(lep_matches && lepb_matches) return "semilep_right_tlep";
			else return "semilep_wrong";
		}
		else {
			return "other";
		}
		// if(gen.decay == DecayType::SEMILEP){
		// 	bool whad_matches = reco.wplus.isLeptonic ? reco.wminus.matches(gen.wminus, DR_match_to_gen_) : reco.wplus.matches(gen.wplus, DR_match_to_gen_);
		// 	bool b_matches    = reco.wplus.isLeptonic ? (reco.bbar->DeltaR(*gen.bbar) < DR_match_to_gen_) : (reco.b->DeltaR(*gen.b) < DR_match_to_gen_);
		// 	bool lepb_matches = reco.wplus.isLeptonic ? (reco.b->DeltaR(*gen.b) < DR_match_to_gen_) : (reco.bbar->DeltaR(*gen.bbar) < DR_match_to_gen_);
		// 	bool lep_matches  = reco.wplus.isLeptonic ? (reco.wplus.first->DeltaR(*gen.wplus.first) < DR_match_to_gen_) : (reco.wminus.first->DeltaR(*gen.wminus.first) < DR_match_to_gen_);
		// 	if(lepb_matches && lep_matches && whad_matches && b_matches) return "semilep_visible_right";
		// 	else if(whad_matches && b_matches) return "semilep_right_thad";
		// 	else if(whad_matches) return "semilep_right_whad";
		// 	else return "semilep_wrong";
		// }
		// else return "other";
	}
		

  //This method is called once every file, contains the event loop
  //run your proper analysis here
  virtual void analyze()
  {
		unsigned long evt_idx = 0;
    URStreamer event(tree_);

    opts::variables_map &values = URParser::instance().values();
		int limit = values["limit"].as<int>();

		//CUTS
		// auto btag_id = &Jet::csvIncl;
		// float btag_cut = 0.941;
		// float bjet_pt_cut = 30;
		// float DR_match_to_gen = 0.3;

		float mu_charge=0, e_charge=0;
    while(event.next())
    {
			// if(evt_idx % 100 == 0) Logger::log().warning() << "Beginning event: " <<
			// 												 evt_idx << endl;
			if(limit > 0 && evt_idx > limit) {
				return;
			}
			evt_idx++;

			tracker_.track("start");
			Logger::log().debug() << "Beginning event" << endl;

			const vector<Muon>& muons = event.muons();
			vector<const Muon*> tight_muons; tight_muons.reserve(muons.size());
			vector<const Muon*> loose_muons; loose_muons.reserve(muons.size());
			for(vector<Muon>::const_iterator muon = muons.begin(); muon != muons.end(); ++muon){
				IDMuon mu(*muon);
				if(mu.ID(IDMuon::LOOSE_12) && mu.Pt() > 15.){
					loose_muons.push_back(&(*muon));
					if(mu.ID(IDMuon::TIGHT_12) && mu.Pt() > 30.){
						histos_["preselection"]["tlep_char"].fill(muon->charge()*0.5);
						histos_["preselection"]["mu_char"].fill(muon->charge()*0.5);
						mu_charge += muon->charge();
						tight_muons.push_back(&(*muon));
					}
				}
			}
			//Apply vetoes
			tracker_.track("selections");
			if(loose_muons.size() > 1) continue;
			tracker_.track("muon veto");
			
			const vector<Electron>& electrons = event.electrons();
			vector<const Electron*> tight_electrons; tight_electrons.reserve(electrons.size());
			vector<const Electron*> loose_electrons; loose_electrons.reserve(electrons.size());
			for(vector<Electron>::const_iterator electron = electrons.begin(); electron != electrons.end(); ++electron){
				IDElectron el(*electron);
				if(el.ID(IDElectron::LOOSE_12) && el.Pt() > 15.){
					loose_electrons.push_back(&(*electron));
					if(el.ID(IDElectron::MEDIUM_12) && el.Pt() > 30.){
						histos_["preselection"]["tlep_char"].fill(electron->charge()*0.5);
						histos_["preselection"]["el_char"].fill(electron->charge()*0.5);
						e_charge += electron->charge();
						tight_electrons.push_back(&(*electron));
					}
				}
			}
			if(loose_electrons.size() > 1) continue;
			tracker_.track("electron veto");
			if(tight_muons.size() + tight_electrons.size() != 1) continue;
			tracker_.track("One lepton");

			const vector<Jet>& jets = event.jets();
			vector<const Jet*> selected_jets; selected_jets.reserve(jets.size());
			for(vector<Jet>::const_iterator jet = jets.begin(); jet != jets.end(); ++jet){
				if(jet->Pt() < 20 || Abs(jet->Eta()) > 2.4) {continue;}
				//if()

				bool overlaps = false;
				for(auto mu : loose_muons){
					if(mu->DeltaR(*jet) < 0.4){
						overlaps = true;
						break;
					}
				}

				for(auto el : loose_electrons){
					if(el->DeltaR(*jet) < 0.4){
						overlaps = true;
						break;
					}
				}

				if(overlaps) continue;
				if(fabs(jet->partonFlavour()) == ura::PDGID::c){
					histos_["preselection"]["cjets_pt" ].fill(jet->Pt());
					if(((*jet).*btag_id_)() > btag_cut_) {
						histos_["preselection"]["cbjets_pt"].fill(jet->Pt());
					}
				}
				selected_jets.push_back(&(*jet));
			}
			if(selected_jets.size() < 4) continue;
			tracker_.track("4 jets");

			vector<const Jet*> bjets; bjets.reserve(selected_jets.size());
			for(auto jet : selected_jets){
				if((jet->*btag_id_)() > btag_cut_) bjets.push_back(jet);
			}
			
			Met met = event.METs()[0];

			if(bjets.size() < 2) continue;
			tracker_.track("2 bjets");

			TLorentzVector* lepton = tight_muons.size() ? (TLorentzVector*) tight_muons[0] : (TLorentzVector*) tight_electrons[0];
			int lep_charge = tight_muons.size() ? tight_muons[0]->charge() : tight_electrons[0]->charge();
			//sort jets by pt
			sort(selected_jets.begin(), selected_jets.end(), [](const Jet* one, const Jet* two) {return  one->Pt() > two->Pt();});
			size_t pick = selected_jets.size() == 4 ? 4 : 5; //avoid vector out of bounds
			vector<const Jet*> leading_jets(selected_jets.begin(), selected_jets.begin()+pick); //pick first 4-5 
			//at least two jets with pt > 30
			if(leading_jets[1]->Pt() < bjet_pt_cut_) continue;
			tracker_.track("bjets pt cut");

			histos_["preselection"]["njets"   ].fill(selected_jets.size());
			histos_["preselection"]["nbjets"  ].fill(bjets.size());
			histos_["preselection"]["lep_pt"  ].fill(lepton->Pt());
			histos_["preselection"]["lep_char"].fill(lep_charge*0.5);

			vector< TTbarHypothesis > combinations;
			list< TLorentzVector > i_wish_it_was_python;
			do {
				if((leading_jets[0]->*btag_id_)() < btag_cut_) continue;
				if((leading_jets[1]->*btag_id_)() < btag_cut_) continue;
				if(leading_jets[0]->Pt() < bjet_pt_cut_) continue;
				if(leading_jets[1]->Pt() < bjet_pt_cut_) continue;

				TTbarHypothesis hyp;
				hyp.b    = leading_jets[0];
				hyp.bbar = leading_jets[1];
				//remove W had ambiguity
				if(leading_jets[2]->Pt() < leading_jets[3]->Pt()) continue;

				if(lep_charge == 1){
					hyp.wplus.isLeptonic = true;
					hyp.wminus.isLeptonic = false;
					hyp.wplus.first   = lepton;
					hyp.wminus.first  = leading_jets[2];
					hyp.wminus.second = leading_jets[3];
					// TLorentzVector b;
					// b.SetPtEtaPhiM(
					// 	hyp.b->Pt(), 
					// 	hyp.b->Eta(),
					// 	hyp.b->Phi(),
					// 	0.);
					// NeutrinoSolver solver(lepton, hyp.b);
					// TLorentzVector nu = solver.GetBest(
					// 	met.Px(),
					// 	met.Py(),
					// 	met.Px() * 0.1, //fake error to 10% until met unc matrix is available
					// 	met.Py() * 0.1,
					// 	0.,
					// 	hyp.chisq);
					// i_wish_it_was_python.push_back(nu);
					// hyp.wplus.second = &(i_wish_it_was_python.back());
				} else {
					hyp.wplus.isLeptonic  = false;
					hyp.wminus.isLeptonic = true;
					hyp.wminus.first = lepton;
					hyp.wplus.first  = leading_jets[2];
					hyp.wplus.second = leading_jets[3];
					// TLorentzVector b;
					// b.SetPtEtaPhiM(
					// 	hyp.bbar->Pt(), 
					// 	hyp.bbar->Eta(),
					// 	hyp.bbar->Phi(),
					// 	0.);
				}

				hyp.solve(solver_, met);
				//Hypothesis selection (rough cuts so far)
				if(fabs(hyp.wplus.mass() - 80) > 40) continue;
				if(fabs(hyp.wminus.mass() - 80) > 40) continue;
				if(fabs(hyp.top_mass() - 173) > 50) continue;
				if(fabs(hyp.tbar_mass() - 173) > 50) continue;
				if(hyp.nu_chisq < 0) continue;

				combinations.push_back(hyp);
			} while(std::next_permutation(leading_jets.begin(), leading_jets.end()));

			if(combinations.size() == 0) continue;
			tracker_.track("one ttbar hypothesis");

			/*
				FETCH TTBAR GEN HYPOTHESIS (IF IT IS TT MC)
			 */
			TTbarHypothesis gen_hyp;
			TTbarHypothesis matched_hyp;
			if(isTTbar_){
				gen_hyp = SelectGenParticles(event);			
				fill("gen", gen_hyp, selected_jets.size(), bjets.size());
				matched_hyp = match_to_gen(
					gen_hyp, 
					//leading_jets,
					selected_jets, 
					tight_electrons,
					tight_muons,
					DR_match_to_gen_
					);
				
				//Logger::log().debug() << "lepton: "<< gen_hyp.wlep()->second->Pt() <<" Jets: ";
				for(const Jet * jet : leading_jets) Logger::log().debug() << jet << " ";
				Logger::log().debug() << endl;
				for(auto& combo : combinations){
					Logger::log().debug() << "#Combo: " << combinations.size() << endl;
					fill_discriminator_info(get_ttdir_name(matched_hyp, combo)+"/discriminators", combo);
				}
			}
			
			for(auto item = ordering_.begin(); item != ordering_.end(); ++item){
				sort(combinations.begin(), combinations.end(), item->second);
				TTbarHypothesis best = *combinations.begin();
				if(!isTTbar_){
					fill("all/"+item->first, best, selected_jets.size(), bjets.size());
				} else {
					//define which subdir we fall in
					string ttsubdir = get_ttdir_name(matched_hyp, best);
					fill(ttsubdir+"/"+item->first, best, selected_jets.size(), bjets.size());
					if(ttsubdir == "semilep_visible_right") fill_gen_info("semilep_visible_right/"+item->first, best, gen_hyp);
				} //if(!isTTbar_)
			} // for(auto item = ordering_.begin(); item != ordering_.end(); ++item)
		} //while(event.next())
   }

  //this method is called at the end of the job, by default saves
  //every histogram/tree produced, override it if you need something more
  virtual void end(){
		outFile_.Write();
		tracker_.writeTo(outFile_);
	}

  //do you need command-line or cfg options? If so implement this 
  //method to book the options you need. CLI parsing is provided
  //by AnalysisFW/interface/URParser.h and uses boost::program_options
  //look here for a quickstart tutorial: 
  //http://www.boost.org/doc/libs/1_51_0/doc/html/program_options/tutorial.html
  static void setOptions() {
		URParser &parser = URParser::instance();
		opts::options_description &opts = parser.optionGroup("analyzer", "CLI and CFG options that modify the analysis");
		opts.add_options()
      ("limit,l", opts::value<int>()->default_value(-1), "limit the number of events processed per file");
	}
private:
	map<string, map<string, RObject> > histos_;
	CutFlowTracker tracker_;
	bool isData_, isTTbar_;

	float (Jet::*btag_id_)() const; // = &Jet::csvIncl;
	float btag_cut_ ;//= 0.941;
	float bjet_pt_cut_ ;//= 30;
	float DR_match_to_gen_ ;//= 0.3;
	map<string, std::function<bool(const TTbarHypothesis &, const TTbarHypothesis &)> > ordering_;

	TTBarSolver solver_;
  // Nothing by default
  // Add your private variables/methods here
};

//make it executable
int main(int argc, char *argv[])
{
  URParser &parser = URParser::instance(argc, argv);
  URDriver<btag_efficiency> test;
  return test.run();
}
