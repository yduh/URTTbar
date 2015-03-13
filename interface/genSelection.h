#ifndef genSelection_h
#define genSelection_h

#include "Hypotheses.h"
#include "URStreamer.h"
#include <vector>
#include "TLorentzVector.h"

int Collapse(int root, std::vector<const Genparticle*> &particles);
TTbarHypothesis SelectGenParticles(URStreamer& event);

template <class T>
const T* gen_match(const TLorentzVector* gen, std::vector<const T*> &recos, float dr_max){
	const T* best = NULL;
	float dpt = 1e100;
	for(auto reco : recos){
		if(reco->DeltaR(*gen) > dr_max)	continue;

		// CLOSEST IN DPT
		// if(fabs(reco->Pt() - gen->Pt()) < dpt){
		// 	dpt = fabs(reco->Pt() - gen->Pt());
		// 	best = reco;
		// }

		//HIGHEST PT
		/* if(!best || reco->Pt() > best->Pt()){ */
		/* 	best = reco; */
		/* } */

		// CLOSEST IN DR
		if(reco->DeltaR(*gen) < dpt){
			dpt  = reco->DeltaR(*gen);
			best = reco;
		}
	}
	return best;
}

TTbarHypothesis match_to_gen(
	TTbarHypothesis &gen_hyp, 
	std::vector<const Jet*> &jets,	
	std::vector<const Electron*> &electrons,
	std::vector<const Muon*> &muons,
  float dr_max);

#endif
