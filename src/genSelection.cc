#include "genSelection.h"
#include "Logger.h"
#include "PDGID.h"

int Collapse(int root, std::vector<const Genparticle*> &particles)
{
	bool found = true;
	while(found){
		found = false;
		for(auto particle : particles){
			if(particle->momIdx().size() == 0) continue;
			if(particle->momIdx()[0] == root){
				root = particle->idx();
				found = true;
				break;
			}
		}
	}
	return root;
}


TTbarHypothesis SelectGenParticles(URStreamer& event)
{
	const std::vector<Genparticle>& gps = event.genParticles();
	TTbarHypothesis ret;

	//find top and tbar (multiple due to radiation
	std::vector<const Genparticle*> top_ids;
	std::vector<const Genparticle*> tbar_ids;
	std::vector<int> root_t, root_tbar;
	for(auto gp = gps.begin(); gp != gps.end(); ++gp){
		if(gp->pdgId() == ura::PDGID::t){
			top_ids.push_back(&(*gp));
			if(gp->momIdx().size() == 0 || gps[gp->momIdx()[0]].pdgId() != ura::PDGID::t){
				root_t.push_back(gp->idx());
			}
		} 
		else if(gp->pdgId() == ura::PDGID::tbar) {
			tbar_ids.push_back(&(*gp));
			if(gp->momIdx().size() == 0 || gps[gp->momIdx()[0]].pdgId() != ura::PDGID::tbar){
				root_tbar.push_back(gp->idx());
			}
		}
	}
	std::vector<int> collapsed_t, collapsed_tbar;
	for(auto idx : root_t){
		collapsed_t.push_back(
			Collapse(idx, top_ids)
			);
	}
	for(auto idx : root_tbar){
		collapsed_tbar.push_back(
			Collapse(idx, tbar_ids)
			);
	}
	//the std::vectors should be already sorted
	if(!is_sorted(collapsed_t.begin(), collapsed_t.end())) sort(collapsed_t.begin(), collapsed_t.end());
	if(!is_sorted(collapsed_tbar.begin(), collapsed_tbar.end())) sort(collapsed_tbar.begin(), collapsed_tbar.end());
		
	//look for top decay products
	int nbs = 0;
	int nbbars = 0;
	int nwp = 0, nwm = 0;
	int wpIdx = -1, wmIdx = -1;
	std::vector<const Genparticle*> wplus, wminus;
	for(auto gp = gps.begin(); gp != gps.end(); ++gp){
		if(gp->momIdx().size() == 0) continue;
		if(gp->pdgId() == ura::PDGID::b && binary_search(collapsed_t.begin(), collapsed_t.end(), gp->momIdx()[0])) {
			nbs++; 
			ret.b = &(*gp);
		}
		else if(gp->pdgId() == ura::PDGID::bbar && binary_search(collapsed_tbar.begin(), collapsed_tbar.end(), gp->momIdx()[0])) {
			nbbars++;
			ret.bbar = &(*gp);
		}
		else if(gp->pdgId() == ura::PDGID::Wminus){
			wminus.push_back(&(*gp));
			if(binary_search(collapsed_tbar.begin(), collapsed_tbar.end(), gp->momIdx()[0])) {nwm++; wmIdx = gp->idx();}
		}
		else if(gp->pdgId() == ura::PDGID::Wplus){
			wplus.push_back(&(*gp));
			if(binary_search(collapsed_t.begin(), collapsed_t.end(), gp->momIdx()[0])) {nwp++; wpIdx = gp->idx();}
		}
	}
	if(nbs != 1 && nbbars != 1)	Logger::log().error() << event.run<<":"<< event.lumi << ":" << event.evt << 
																" Found " << nbs << " b's and " << nbbars << " bbar's" << endl;
	if(nwp != 1 && nwm != 1) Logger::log().error() << event.run<<":"<< event.lumi << ":" << event.evt << 
														 " Found " << nwp << " W+'s and " << nwm << " W-'s" << endl;

	//collapse W+
	wpIdx = Collapse(wpIdx, wplus);
	wmIdx = Collapse(wmIdx, wminus);

	int wp_nprods = 0, wm_nprods = 0;
	const Genparticle *wplus_prods[2], *wminus_prods[2];		
	for(auto gp = gps.begin(); gp != gps.end(); ++gp){
		if(gp->momIdx().size() == 0) continue;
		if(gp->momIdx()[0] == wpIdx) {
			if(wp_nprods < 2) wplus_prods[wp_nprods] = &(*gp);
			wp_nprods++; 
		}
		else if(gp->momIdx()[0] == wmIdx) {
			if(wm_nprods < 2) wminus_prods[wm_nprods] = &(*gp);
			wm_nprods++; 
		}
	}
	if(wp_nprods != 2 && wm_nprods != 2){ 
		Logger::log().error() << event.run<<":"<< event.lumi << ":" << event.evt << 
			" Found " << wp_nprods <<
			" W+ products and " << wm_nprods << " W- products" << endl;
		ret.decay = INVALID;
	} else {
		int lep_decays = 0;
		//w products are quarks
		bool wp1_isquark = fabs(wplus_prods[0]->pdgId()) <= 6;
		bool wp2_isquark = fabs(wplus_prods[1]->pdgId()) <= 6;
		//w products are leptons or quarks
		bool wp1_isqlep = fabs(wplus_prods[0]->pdgId()) <= 16;
		bool wp2_isqlep = fabs(wplus_prods[1]->pdgId()) <= 16;
		if(wp1_isquark && wp2_isquark) {
			ret.wplus.first  = wplus_prods[0];
			ret.wplus.second = wplus_prods[1];
			ret.wplus.isLeptonic = false;
		} else if((wp1_isqlep && !wp1_isquark) && (wp2_isqlep && !wp2_isquark)) {
			//assign the charged lepton as first
			int id0 = wplus_prods[0]->pdgId();
			ret.wplus.first  = (id0 % 2 == 0) ? wplus_prods[1] : wplus_prods[0];
			ret.wplus.second = (id0 % 2 == 0) ? wplus_prods[0] : wplus_prods[1];
			lep_decays++;
			ret.wplus.isLeptonic = true;
		} else {
			Logger::log().error() << event.run<<":"<< event.lumi << ":" << event.evt << 
				" W+ decays to lepton and quark! (" <<
				wplus_prods[0]->pdgId() << ", " << wplus_prods[1]->pdgId() << endl;
			// ") 1:" << wp1_isquark << "," << wp1_isqlep << " 2:" <<
			// wp1_isquark << "," << wp2_isqlep << endl;
			ret.decay = INVALID;
		}

		//FIXME: this should go in a separate function
		//w products are quarks
		bool wm1_isquark = fabs(wminus_prods[0]->pdgId()) <= 6;
		bool wm2_isquark = fabs(wminus_prods[1]->pdgId()) <= 6;
		//w products are leptons or quarks
		bool wm1_isqlep = fabs(wminus_prods[0]->pdgId()) <= 16;
		bool wm2_isqlep = fabs(wminus_prods[1]->pdgId()) <= 16;
		if(wm1_isquark && wm2_isquark) {
			ret.wminus.first  = wminus_prods[0];
			ret.wminus.second = wminus_prods[1];
			ret.wminus.isLeptonic = false;
		} else if((wm1_isqlep && !wm1_isquark) && (wm2_isqlep && !wm2_isquark)) {
			//assign the charged lepton as first
			int id0 = wminus_prods[0]->pdgId();
			ret.wminus.first  = (id0 % 2 == 0) ? wminus_prods[1] : wminus_prods[0];
			ret.wminus.second = (id0 % 2 == 0) ? wminus_prods[0] : wminus_prods[1];
			lep_decays++;
			ret.wminus.isLeptonic = true;
		} else {
			Logger::log().error() << event.run<<":"<< event.lumi << ":" << event.evt << 
				" W- decays to lepton and quark! (" <<
				wminus_prods[0]->pdgId() << ", " << wminus_prods[1]->pdgId() <<
				")" << endl;
			ret.decay = INVALID;
		}
		if(ret.decay != INVALID){
			switch(lep_decays){
			case 0: ret.decay = FULLHAD; break;
			case 1: ret.decay = SEMILEP; break;
			case 2: ret.decay = FULLLEP; break;
			}
		}
	}
	return ret;

}

TTbarHypothesis match_to_gen(
	TTbarHypothesis& gen_hyp, 
	std::vector<const Jet*>& jets, 
	std::vector<const Electron*>& electrons,
	std::vector<const Muon*>& muons,
	float dr_max){
	TTbarHypothesis ret;
	ret.decay = gen_hyp.decay;
	ret.wplus.isLeptonic  = gen_hyp.wplus.isLeptonic;
	ret.wminus.isLeptonic = gen_hyp.wminus.isLeptonic;
	// cout << ret.b << " " << ret.bbar << " " << ret.wplus.isLeptonic << " " <<
	// 	ret.wminus.isLeptonic << endl;

	if(gen_hyp.decay != SEMILEP) return ret;
	ret.b    = gen_match(gen_hyp.b   , jets, dr_max);
	ret.bbar = gen_match(gen_hyp.bbar, jets, dr_max);
	
	if(gen_hyp.wplus.isLeptonic) {
		// cout << "matching wplus lep" << endl;
		ret.wminus.first  = gen_match(gen_hyp.wminus.first , jets, dr_max);
		ret.wminus.second = gen_match(gen_hyp.wminus.second, jets, dr_max);
		if(fabs(((const Genparticle*) gen_hyp.wplus.first)->pdgId()) == ura::PDGID::e){
			// cout << "matching to electron" << endl;
			ret.wplus.first = gen_match(gen_hyp.wplus.first, electrons, dr_max);
		}else{
			// cout << "matching to muon" << endl;
			ret.wplus.first = gen_match(gen_hyp.wplus.first, muons, dr_max);
		}
		ret.wplus.second = gen_hyp.wplus.second;
	} else {
		// cout << "Matching wminus lep" << endl;
		ret.wplus.first  = gen_match(gen_hyp.wplus.first , jets, dr_max);
		ret.wplus.second = gen_match(gen_hyp.wplus.second, jets, dr_max);

		if(fabs(((const Genparticle*) gen_hyp.wminus.first)->pdgId()) == ura::PDGID::e){
			// cout << "Matching to electron" << endl;
			ret.wminus.first = gen_match(gen_hyp.wminus.first, electrons, dr_max);
		}else{
			// cout << "Matching to muon" << endl;
			ret.wminus.first = gen_match(gen_hyp.wminus.first, muons, dr_max);
		}
		ret.wminus.second = gen_hyp.wminus.second;
	}	
	return ret;
}
