#ifndef Hypotheses_h
#define Hypotheses_h

#include "TLorentzVector.h"

struct WHypothesis {
WHypothesis():
	first(0), 
		second(0){}
	const TLorentzVector *first, *second;
	bool isLeptonic;
	bool hasMissingProng() const { return (!first || !second);	}
	//bool hasNeutrino() const {return second && (s)}
	bool matches(const WHypothesis &other, float threshold=0.3) const {
		bool have_first = first && other.first;
		bool have_second = second && other.second;
		if(isLeptonic){
			if(!have_first || first->DeltaR(*other.first) > threshold) return false;
			return true;
		} else {
			if(!have_first || !have_second) return false;
			bool f_matches_f = first->DeltaR(*other.first) < threshold;
			bool s_matches_s = second->DeltaR(*other.second) < threshold;
			bool f_matches_s = first->DeltaR(*other.second) < threshold;
			bool s_matches_f = second->DeltaR(*other.first) < threshold;
			if(f_matches_f && s_matches_s) return true;
			if(f_matches_s && s_matches_f) return true;
			return false;
		}
	}

	TLorentzVector p4() const {
		if(hasMissingProng()) return TLorentzVector();
		return (*first + *second);
	}

	double mass() const {
		if(hasMissingProng()) return -1;
		return p4().M();
	}
};

enum DecayType {NOTSET, INVALID, FULLHAD, SEMILEP, FULLLEP};
struct TTbarHypothesis {
	TTbarHypothesis():
		b(0), 
		chisq(-1),
		bbar(0),
		wplus(), 
		wminus(),
		decay(NOTSET)
	{}
	const TLorentzVector *b, *bbar;
	WHypothesis wplus, wminus;
	DecayType decay;
	double chisq;
	bool hasMissingProng() const {
		return (!b || !bbar || wplus.hasMissingProng() || wminus.hasMissingProng());
	}

	bool matches(const TTbarHypothesis& other, float threshold=0.3){
		if(!b || !other.b || b->DeltaR(*other.b) > threshold) return false;
		if(!bbar || !other.bbar || bbar->DeltaR(*other.bbar) > threshold) return false;
		if(!wplus.matches( other.wplus , threshold)) return false;
		if(!wminus.matches(other.wminus, threshold)) return false;
		return true;
	}

	double top_mass() const {
		if(wplus.hasMissingProng()) return -1;
		return (wplus.p4() + *b).M();
	}
	double tbar_mass() const {
		if(wminus.hasMissingProng()) return -1;
		return (wminus.p4() + *bbar).M();
	}
};

#endif
