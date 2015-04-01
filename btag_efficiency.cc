#include <iostream>
#include "AnalyzerBase.h"
#include "URStreamer.h"
#include "URDriver.h"
#include "genSelection.h"
#include <map>
#include "RObject.h"
#include "IDMuon.h"
#include "IDElectron.h"
#include "IDJet.h"
#include <algorithm>
//#include "NeutrinoSolver.h"
#include "TTBarSolver.h"
#include <list>
#include "Logger.h"
#include "URParser.h"
#include "CutFlowTracker.h"
#include <boost/algorithm/string/predicate.hpp>
#include "PDGID.h"
#include "TMath.h"
#include "Permutation.h"

using namespace TMath;

class btag_efficiency : public AnalyzerBase
{
public:
	enum TTNaming {RIGHT, RIGHT_THAD, RIGHT_WHAD, RIGHT_TLEP, WRONG, OTHER};
private:
	map<string, map<string, RObject> > histos_;
	map<TTNaming, string> naming_;
	CutFlowTracker tracker_;
	bool isData_, isTTbar_;

	float (Jet::*btag_id_)() const; // = &Jet::csvIncl;
	float btag_cut_ ;//= 0.941;
	float bjet_pt_cut_ ;//= 30;
	float DR_match_to_gen_ ;//= 0.3;
	map<string, std::function<bool(const Permutation &, const Permutation &)> > ordering_;
	map<string, std::function<bool(const Jet*)> > working_points_;

	TTBarSolver solver_;
  // Nothing by default
  // Add your private variables/methods here

public:
  btag_efficiency(const std::string output_filename):
    AnalyzerBase("btag_efficiency", output_filename), 
		tracker_(),
		ordering_(),
		working_points_(),
		solver_()
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
		//ordering_["full_discriminant"] = [](const Permutation &one, const Permutation &two) {return  one.Prob()  < two.Prob() ;};
		//ordering_["nu_chisq"         ] = [](const Permutation &one, const Permutation &two) {return  one.NuChisq() 	 < two.NuChisq() 	;};
		//ordering_["nu_discriminant"	 ] = [](const Permutation &one, const Permutation &two) {return  one.NuDiscr() 	 < two.NuDiscr() 	;};
		//ordering_["btag_discriminant"] = [](const Permutation &one, const Permutation &two) {return  one.BDiscr()  	 < two.BDiscr()  	;};
		ordering_["mass_discriminant"] = [](const Permutation &one, const Permutation &two) {return  one.MassDiscr() < two.MassDiscr();}; 

		naming_[TTNaming::RIGHT ] = "semilep_visible_right";
		naming_[TTNaming::RIGHT_THAD ] = "semilep_right_thad" 	 ;
		naming_[TTNaming::RIGHT_WHAD ] = "semilep_right_whad" 	 ;
		naming_[TTNaming::RIGHT_TLEP ] = "semilep_right_tlep" 	 ;
		naming_[TTNaming::WRONG ] = "semilep_wrong" 			 ;
		naming_[TTNaming::OTHER ] = 	"other"              ;

		working_points_["notag"]     = [](const Jet* jet) {return false;};
		working_points_["csvTight"]  = [](const Jet* jet) {return jet->csvIncl() > 0.941;};
		working_points_["csvMedium"] = [](const Jet* jet) {return jet->csvIncl() > 0.814;};
		working_points_["csvLoose"]  = [](const Jet* jet) {return jet->csvIncl() > 0.423;};
		// working_points_["ssvHiPur"] = [](const Jet* jet) {return (bool) jet->ssvHiPur()};
		// working_points_["ssvHiEff"] = [](const Jet* jet) {return (bool) jet->ssvHiEff()};
		// working_points_["trkHiPur"] = [](const Jet* jet) {return (bool) jet->trkHiPur()};
		// working_points_["trkHiEff"] = [](const Jet* jet) {return (bool) jet->trkHiEff()};
		// // working_points_[] = [](const Jet* jet) {};
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
		book<TH1F>(folder, "nu_chisq"         , "", 520,  -2., 50.);
		book<TH1F>(folder, "nu_discriminant"	, "", 110,  -1., 10.);
		book<TH1F>(folder, "btag_discriminant", "", 300, -30.,  0.);
		book<TH1F>(folder, "mass_discriminant", "", 100,   0., 10.);
		book<TH1F>(folder, "full_discriminant", "", 200, -10., 10.);
		
		book<TH1F>(folder, "btag_value", "", 100, 0., 1.);
		book<TH2F>(folder, "Wmasshad_tmasshad", "", 500, 0., 500., 500, 0., 500);			
		book<TH2F>(folder, "WtMass_special", "", 500, 0., 500., 500, 0., 500);			
		book<TH2F>(folder, "WtMass_rphi", "", 500, 0., 500., 500,  0, 5);
	}

	void book_hyp_plots(string folder){
		book<TH1F>(folder, "njets"    , "", 50, 0., 50.);
		book<TH1F>(folder, "nbjets"   , "", 50, 0., 50.);
		book<TH1F>(folder, "lep_b_pt" , ";p_{T}(b) (GeV)", 500, 0., 500.);
		book<TH1F>(folder, "had_b_pt" , ";p_{T}(b) (GeV)", 500, 0., 500.);
		book<TH1F>(folder, "lep_pt"   , ";p_{T}(#ell) (GeV)", 500, 0., 500.);
		book<TH1F>(folder, "Wlep_mass", ";m_{W}(lep) (GeV)", 140, 0., 140.);
		book<TH1F>(folder, "Wlep_char", ";charge_{W}(lep) (GeV)", 2, -1, 1);
		book<TH1F>(folder, "Whad_mass", ";m_{W}(had) (GeV)", 140, 0., 140.);
		book<TH1F>(folder, "Whad_DR"  , ";m_{W}(had) (GeV)", 100, 0., 10.);
		book<TH1F>(folder, "Whad_pt"  , ";m_{W}(had) (GeV)", 500, 0., 500.);
		book<TH1F>(folder, "Whad_leading_DR", "", 100, 0., 10.);
		book<TH1F>(folder, "Whad_sublead_DR", "", 100, 0., 10.);
		book<TH2F>(folder, "Whad_lead_sub_DR", ";#DeltaR(leading jet, WHad); #DeltaR(subleading jet, WHad)", 100, 0., 10., 100, 0., 10.);
		book<TH1F>(folder, "tlep_mass", ";m_{t}(lep) (GeV)", 300, 100., 400.);
		book<TH1F>(folder, "thad_mass", ";m_{t}(had) (GeV)", 300, 100., 400.);
		book<TH1F>(folder, "hjet_pt"  , ";m_{t}(lep) (GeV)", 500, 0., 500.);
		book<TH2F>(folder, "hjet_pts" , ";m_{t}(had) (GeV)", 500, 0., 500., 500, 0., 500.);
		book<TH2F>(folder, "Whad_pt_lead_pt" , "", 500, 0., 500., 500, 0., 500.);
		book<TH2F>(folder, "hjet_es"  , ";m_{t}(had) (GeV)", 500, 0., 500., 500, 0., 500.);
	}

	void book_jet_plots(string folder){
	}

  virtual void begin()
  {
    outFile_.cd();
		
		string folders[] = {"all", "semilep_visible_right", "semilep_right_thad", 
												"semilep_right_tlep", "semilep_right_whad", "semilep_wrong", "other"};
		string wjet_folders[] = {"leading", "subleading"};
		string tagging[] = {"lead_tagged", "sublead_tagged", "both_tagged", "both_untagged"};

		//if(isTTbar_) book_hyp_plots("gen");
		for(auto& genCategory : folders){			
			if(isTTbar_ && genCategory == "all") continue;
			book_combo_plots(genCategory+"/discriminators");
			for(auto& item : ordering_) {
				string criterion = item.first;
				for(auto& wp_item : working_points_) {
					string working_point = wp_item.first;
					for(auto& tag : tagging){
						string folder = genCategory + "/" + criterion + "/" + working_point + "/" + tag;
						Logger::log().debug() << "creating plots for folder: " << folder << std::endl;
						
						book_combo_plots(folder);
						book_hyp_plots(folder);

						for(auto& w_folder : wjet_folders){
							string current = folder + "/" + w_folder;
							book<TH1F>(current, "eta"	,"eta"	, 100, -5, 5);
							book<TH1F>(current, "pt" 	,"pt" 	, 500, 0 , 500);
							book<TH1F>(current, "phi"	,"phi"	, 400, -4, 4);
							book<TH1F>(current, "pflav","pflav", 55, -27.5, 27.5);
							book<TH1F>(current, "pflav_smart","pflav", 55, -27.5, 27.5);
							book<TH1F>(current, "hflav","hflav", 55, -27.5, 27.5);
							book<TH1F>(current, "energy", ";E_{jet} (GeV)", 500, 0., 500.);
							book<TH1F>(current, "ncharged", "", 50, 0., 50.);						
							book<TH1F>(current, "nneutral", "", 50, 0., 50.);						
							book<TH1F>(current, "ntotal"  , "", 50, 0., 50.);						
						}

						if(genCategory == "semilep_visible_right"){
							book<TH1F>(folder, "nu_DR"    , "#DeltaR between gen and reco #nu;#DeltaR;counts", 140, 0., 7.);
							book<TH1F>(folder, "nu_DE"    , "#DeltaE between gen and reco #nu;#DeltaE (GeV);counts", 250, -250, 250.);
						}
					}//for(auto& tag : tagging)
				}//for(auto& wp_item : working_points_)
			}//for(auto& criterion : criteria)
			if(!isTTbar_) break;
		}//for(auto& genCategory : folders)

		string folder = "preselection";
		book<TH1F>(folder, "njets"    , "", 50, 0., 50.);
		book<TH1F>(folder, "nleadjets", "", 10, 0., 10.);
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

	void fill_jet_info(string folder, const Jet* jet){//, const Genparticle* genp=0){
		Logger::log().debug() << folder << endl;
		auto dir = histos_.find(folder);
		dir->second["eta"	 ].fill(jet->Eta());
		dir->second["pt" 	 ].fill(jet->Pt() );
		dir->second["phi"	 ].fill(jet->Phi());
		dir->second["pflav"].fill(jet->partonFlavour());
		dir->second["hflav"].fill(jet->hadronFlavour());
		dir->second["energy"].fill(jet->E());

		dir->second["ncharged"].fill(jet->numChargedHadrons());
		dir->second["nneutral"].fill(jet->numNeutralHadrons());
		dir->second["ntotal"  ].fill(jet->numChargedHadrons()+jet->numNeutralHadrons());

		const IDJet* idjet = (const IDJet*) jet;
		Logger::log().debug() << "fill_jet_info " << jet->partonFlavour() << " " << idjet->flavor() << endl;
		dir->second["pflav_smart"].fill(idjet->flavor());
	}

	void fill_discriminator_info(string folder, const Permutation &hyp){
		//Logger::log().warning() << "filling " << hyp.btag_discriminant << endl;
		auto dir = histos_.find(folder);
		dir->second["full_discriminant"].fill(hyp.Prob() );
		dir->second["nu_chisq"         ].fill(hyp.NuChisq() );
		dir->second["nu_discriminant"	 ].fill(hyp.NuDiscr()	);
		dir->second["btag_discriminant"].fill(hyp.BDiscr() );
		dir->second["mass_discriminant"].fill(hyp.MassDiscr());

		double whad_mass = hyp.WHad().M();
		double thad_mass = hyp.THad().M();
		dir->second["btag_value"       ].fill((hyp.BHad()->*btag_id_)());
		dir->second["btag_value"       ].fill((hyp.BLep()->*btag_id_)());
		dir->second["Wmasshad_tmasshad"].fill(whad_mass, thad_mass);
		dir->second["WtMass_special"   ].fill(whad_mass + thad_mass, thad_mass - whad_mass);
		double r = Sqrt(pow(whad_mass,2) + pow(thad_mass,2));
		double phi = ATan(thad_mass/whad_mass);
		dir->second["WtMass_rphi"      ].fill(r, phi);
	}

	void fill(string folder, Permutation &hyp, size_t njets, size_t nbjets){//, TTbarHypothesis *genHyp=0) {
		auto dir = histos_.find(folder);
		if(dir == histos_.end()) {
			Logger::log().error() << "histogram folder: " << folder <<
				" does not exists!" << endl;
			return;
		}

		fill_discriminator_info(folder, hyp);
		
		//dir->second["Wlep_char"].fill(charge);
		dir->second["njets"    ].fill(njets );
		dir->second["nbjets"   ].fill(nbjets);
		dir->second["lep_b_pt" ].fill(hyp.BLep()->Pt());
		dir->second["had_b_pt" ].fill(hyp.BHad()->Pt());
		dir->second["lep_pt"   ].fill(hyp.L()->Pt());
		dir->second["Wlep_mass"].fill(hyp.WLep().M());
		dir->second["Whad_mass"].fill(hyp.WHad().M());
		dir->second["Whad_DR"  ].fill(hyp.WJa()->DeltaR(*hyp.WJb()));
		dir->second["Whad_pt"  ].fill(hyp.WHad().Pt());
		dir->second["tlep_mass"].fill(hyp.TLep().M());
		dir->second["thad_mass"].fill(hyp.THad().M());
		dir->second["hjet_pt"  ].fill(hyp.WJa()->Pt());
		dir->second["hjet_pt"  ].fill(hyp.WJb()->Pt());
		float lead = (hyp.WJa()->Pt() > hyp.WJb()->Pt()) ? hyp.WJa()->Pt() : hyp.WJb()->Pt();
		float sub  = (hyp.WJa()->Pt() > hyp.WJb()->Pt()) ? hyp.WJb()->Pt() : hyp.WJa()->Pt();
		dir->second["hjet_pts" ].fill(lead, sub);//*/

		//if(folder == "gen") return;
		Logger::log().debug() <<"analyzer::fill " << folder << endl;
		const Jet *leading    = (hyp.WJa()->E() > hyp.WJb()->E()) ? hyp.WJa() : hyp.WJb();
		const Jet *subleading = (hyp.WJa()->E() > hyp.WJb()->E()) ? hyp.WJb() : hyp.WJa();

		dir->second["Whad_leading_DR" ].fill(hyp.WHad().DeltaR(*leading));
		dir->second["Whad_sublead_DR" ].fill(hyp.WHad().DeltaR(*subleading));
		dir->second["Whad_lead_sub_DR"].fill(hyp.WHad().DeltaR(*leading), hyp.WHad().DeltaR(*subleading));
		dir->second["Whad_pt_lead_pt" ].fill(hyp.WHad().Pt(), leading->Pt());		
		dir->second["hjet_es"  ].fill(leading->E(), subleading->E());

		//FILL JET INFORMATION
		fill_jet_info(folder + "/leading", leading);
		fill_jet_info(folder + "/subleading", subleading);
		//if((subleading->*btag_id_)() > btag_cut_) fill_jet_info(folder + "/subleading/tagged", subleading, gen_subleading);
	}

	string get_wjet_category(Permutation &hyp, std::function<bool(const Jet*)>& fcn) {
		const Jet *leading    = (hyp.WJa()->E() > hyp.WJb()->E()) ? hyp.WJa() : hyp.WJb();
		const Jet *subleading = (hyp.WJa()->E() > hyp.WJb()->E()) ? hyp.WJb() : hyp.WJa();
		bool lead_tag = fcn(leading   );
		bool sub_tag  = fcn(subleading);
		if(lead_tag && sub_tag) return "/both_tagged";
		else if(lead_tag) return "/lead_tagged";
		else if(sub_tag)  return "/sublead_tagged";
		return "/both_untagged";
	}

	TTNaming get_ttdir_name(Permutation &gen, Permutation &reco, DecayType decay_type){
		// Logger::log().warning() << "fcn called: " << gen.decay << " (SEMI = "<< DecayType::SEMILEP <<")" << endl;
		if(decay_type == DecayType::SEMILEP){
			//if(!gen.hasMissingProng()){
			// Logger::log().debug() << "--------------------------------------" << endl;
			// Logger::log().debug() << "RECO: wh " << reco.whad()->first << " " << reco.whad()->second << " wl " <<
			// 	reco.wlep()->first << " b " << reco.b << " bb " << reco.bbar << endl;
			// Logger::log().debug() << "GEN: wh " << gen.whad()->first << " " << gen.whad()->second << " wl " <<
			// 	gen.wlep()->first << " b " << gen.b << " bb " << gen.bbar << endl;
				//}
			bool whad_matches = reco.IsWHadCorrect(gen);
			bool b_matches    = reco.IsBHadCorrect(gen);
			bool lepb_matches = reco.IsBLepCorrect(gen);
			bool lep_matches  = reco.L() == gen.L();
			if(lepb_matches && lep_matches && whad_matches && b_matches) return TTNaming::RIGHT;//"semilep_visible_right";
			else if(whad_matches && b_matches) return TTNaming::RIGHT_THAD; //"semilep_right_thad";
			else if(whad_matches) return TTNaming::RIGHT_WHAD; //"semilep_right_whad";
			else if(lep_matches && lepb_matches) return TTNaming::RIGHT_TLEP; //"semilep_right_tlep";
			else return TTNaming::WRONG; //"semilep_wrong";
		}
		else {
			return TTNaming::OTHER; //"other";
		}
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
			list<  IDMuon> keep_muons;
			vector<IDMuon*> tight_muons; tight_muons.reserve(muons.size());
			vector<IDMuon*> loose_muons; loose_muons.reserve(muons.size());
			for(vector<Muon>::const_iterator muon = muons.begin(); muon != muons.end(); ++muon){
				IDMuon mu(*muon);
				if(mu.ID(IDMuon::LOOSE_12) && mu.Pt() > 15.){
					keep_muons.push_back(mu);
					loose_muons.push_back(&keep_muons.back());
					if(mu.ID(IDMuon::TIGHT_12) && mu.Pt() > 30.){
						histos_["preselection"]["tlep_char"].fill(muon->charge()*0.5);
						histos_["preselection"]["mu_char"].fill(muon->charge()*0.5);
						mu_charge += muon->charge();
						tight_muons.push_back(&keep_muons.back());
					}
				}
			}
			//Apply vetoes
			tracker_.track("selections");
			if(loose_muons.size() > 1) continue;
			tracker_.track("muon veto");
			
			const vector<Electron>& electrons = event.electrons();
			list<  IDElectron> keep_electrons;
			vector<IDElectron*> tight_electrons; tight_electrons.reserve(electrons.size());
			vector<IDElectron*> loose_electrons; loose_electrons.reserve(electrons.size());
			for(vector<Electron>::const_iterator electron = electrons.begin(); electron != electrons.end(); ++electron){
				IDElectron el(*electron);
				if(el.ID(IDElectron::LOOSE_12) && el.Pt() > 15.){
					keep_electrons.push_back(el);
					loose_electrons.push_back(&keep_electrons.back());
					if(el.ID(IDElectron::MEDIUM_12) && el.Pt() > 30.){
						histos_["preselection"]["tlep_char"].fill(electron->charge()*0.5);
						histos_["preselection"]["el_char"].fill(electron->charge()*0.5);
						e_charge += electron->charge();
						tight_electrons.push_back(&keep_electrons.back());
					}
				}
			}
			if(loose_electrons.size() > 1) continue;
			tracker_.track("electron veto");
			if(tight_muons.size() + tight_electrons.size() != 1) continue;
			tracker_.track("One lepton");

			const vector<Jet>& jets = event.jets();
			list<  IDJet> keep_jets;
			vector<IDJet*> selected_jets; selected_jets.reserve(jets.size());
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
				keep_jets.push_back(*jet);
				selected_jets.push_back(&keep_jets.back());
			}
			if(selected_jets.size() < 4) continue;
			tracker_.track("4 jets");

			vector<Jet*> bjets; bjets.reserve(selected_jets.size());
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
			vector<Jet*> leading_jets(selected_jets.begin(), selected_jets.begin()+pick); //pick first 4-5 
			//at least two jets with pt > 30
			if(leading_jets[1]->Pt() < bjet_pt_cut_) continue;
			tracker_.track("bjets pt cut");

			histos_["preselection"]["nleadjets"].fill(leading_jets.size());
			histos_["preselection"]["njets"   ].fill(selected_jets.size());
			histos_["preselection"]["nbjets"  ].fill(bjets.size());
			histos_["preselection"]["lep_pt"  ].fill(lepton->Pt());
			histos_["preselection"]["lep_char"].fill(lep_charge*0.5);
			// for(auto& jet : leading_jets) Logger::log().debug() << jet << " ";
			// Logger::log().debug() << " " << endl;

			// for(auto& jet : leading_jets) Logger::log().debug() << jet->Pt() << " ";
			// Logger::log().debug() << " " << endl;
		
			//sort leading jets to make next_permutation to work properly
			sort(leading_jets.begin(), leading_jets.end());

			vector< Permutation > combinations;
			list< TLorentzVector > i_wish_it_was_python;
			size_t ncombos=0;
			do {
				ncombos++;
				tracker_.track("Combination start");
				if((leading_jets[0]->*btag_id_)() < btag_cut_) continue;
				if((leading_jets[1]->*btag_id_)() < btag_cut_) continue;
				tracker_.track("Bjet Tag cuts");
				if(leading_jets[0]->Pt() < bjet_pt_cut_) continue;
				if(leading_jets[1]->Pt() < bjet_pt_cut_) continue;
				tracker_.track("Bjet PT cuts");
				//remove W had ambiguity
				if(leading_jets[2]->Pt() < leading_jets[3]->Pt()) continue;
				tracker_.track("Ambiguity cut");

				Permutation hyp(
					leading_jets[2], 
					leading_jets[3], 
					leading_jets[0], 
					leading_jets[1], 
					lepton, 
					&met
					);
				hyp.Solve(solver_);

				//Hypothesis selection (rough cuts so far)
				//if(fabs(hyp.wplus.mass()  - 80) > 40) continue;
				if(fabs(hyp.WHad().M() - 80) > 40) continue;
				tracker_.track("W mass cut");
				//if(fabs(hyp.top_mass() - 173) > 50) continue;
				if(fabs(hyp.THad().M() - 173) > 50) continue;
				tracker_.track("Top mass cut");
				if(hyp.NuChisq() < 0) continue;
				tracker_.track("Neutrino chisquare cut");
				//if(hyp.mass_discriminant > 4) continue;

				combinations.push_back(hyp);
			} while(std::next_permutation(leading_jets.begin(), leading_jets.end()));
			//check that we have the right number of combinations
			if((leading_jets.size() == 4 && ncombos != 24) || (leading_jets.size() == 5 && ncombos != 120)) throw 20;
			// Logger::log().debug() << "HOOK: event: " << event.run << " " << event.lumi << " " << 
			// 	event.evt << endl << "n total cmbs: " << ncombos << ", passing: " << combinations.size() << endl <<
			// 	" nleadjets: " << leading_jets.size() << " njets: " <<  selected_jets.size() <<
			// 	" nbjets: "  << bjets.size() << endl;

			if(combinations.size() == 0) continue;
			tracker_.track("one ttbar hypothesis");

			/*
				FETCH TTBAR GEN HYPOTHESIS (IF IT IS TT MC)
			 */
			TTbarHypothesis gen_hyp;
			Permutation matched_hyp;
			if(isTTbar_){
				gen_hyp = SelectGenParticles(event);			
				//fill("gen", gen_hyp, selected_jets.size(), bjets.size());
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
					TTNaming dir_id = get_ttdir_name(matched_hyp, combo, gen_hyp.decay);
					string dirname  = naming_[dir_id];
					fill_discriminator_info(dirname+"/discriminators", combo);
				}
			}
			
			for(auto item = ordering_.begin(); item != ordering_.end(); ++item){
				//sort(combinations.begin(), combinations.end(), item->second);
				Permutation best = *min_element(combinations.begin(), combinations.end(), item->second);// *combinations.begin();
				if(!isTTbar_){
					//fill("all/"+item->first+"/all", best, selected_jets.size(), bjets.size());
					for(auto& wpoint : working_points_){
						string jet_category = get_wjet_category(best, wpoint.second);
						fill("all/"+item->first+"/"+wpoint.first+jet_category, best, selected_jets.size(), bjets.size());
					}
				} else {
					//define which subdir we fall in
					TTNaming dir_id = get_ttdir_name(matched_hyp, best, gen_hyp.decay);
					string ttsubdir = naming_[dir_id];
					TTbarHypothesis *gen = 0;

					Logger::log().debug() << ttsubdir << " " << dir_id << " " << TTNaming::RIGHT_WHAD << endl;
					if(dir_id <= TTNaming::RIGHT_WHAD) gen = &gen_hyp;

					//fill(ttsubdir+"/"+item->first+"/all", best, selected_jets.size(), bjets.size(), gen);
					for(auto& wpoint : working_points_){
						string jet_category = get_wjet_category(best, wpoint.second);
						fill(ttsubdir+"/"+item->first+"/"+wpoint.first+jet_category, best, selected_jets.size(), bjets.size());
						//if(dir_id == TTNaming::RIGHT) fill_gen_info("semilep_visible_right/"+item->first+"/all", best, gen_hyp);
					}
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
};

//make it executable
int main(int argc, char *argv[])
{
  URParser &parser = URParser::instance(argc, argv);
  URDriver<btag_efficiency> test;
  return test.run();
}
