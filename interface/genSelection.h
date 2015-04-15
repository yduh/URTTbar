#ifndef genSelection_h
#define genSelection_h

#include "Hypotheses.h"
#include "URStreamer.h"
#include <vector>
#include "TLorentzVector.h"
#include "IDMuon.h"
#include "IDElectron.h"
#include "IDJet.h"
#include "Permutation.h"

int Collapse(int root, std::vector<const Genparticle*> &particles);
TTbarHypothesis SelectGenParticles(URStreamer& event);

template <class T>
T* gen_match(const Genparticle* gen, std::vector<T*> &recos, float dr_max){
	T* best = NULL;
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
	if(best) best->addMatch(gen);
	return best;
}

Permutation match_to_gen(
	TTbarHypothesis &gen_hyp, 
	std::vector<IDJet*> &jets,	
	std::vector<IDElectron*> &electrons,
	std::vector<IDMuon*> &muons,
  float dr_max);

#endif
