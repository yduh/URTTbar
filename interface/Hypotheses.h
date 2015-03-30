#ifndef Hypotheses_h
#define Hypotheses_h

#include "TLorentzVector.h"
#include "TTBarSolver.h"
#include "URStreamer.h"

struct WHypothesis {
	WHypothesis():
		first(0), 
		second(0),
		isLeptonic(false)
	{}
	const TLorentzVector *first, *second;
	bool isLeptonic;
	bool hasMissingProng() const { return (!first || !second);	}

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
	const TLorentzVector *b, *bbar;
	TLorentzVector nu_;
	WHypothesis wplus, wminus;
	DecayType decay;
	double nu_chisq;
	double full_discriminant;
	double nu_discriminant;
	double btag_discriminant;
	double mass_discriminant;

	TTbarHypothesis():
		b(0), 
		bbar(0),
		nu_(),
		wplus(), 
		wminus(),
		decay(NOTSET),
		nu_chisq(-1),
		full_discriminant(-1),
		nu_discriminant(-1),
		btag_discriminant(-1),
		mass_discriminant(-1)
	{}

	void solve(TTBarSolver &solver, Met &met) {
		Jet *bhad = const_cast<Jet*>((const Jet*) (wplus.isLeptonic ? bbar : b));
		Jet *blep = const_cast<Jet*>((const Jet*) (wplus.isLeptonic ? b : bbar));
		Jet *wh1  = const_cast<Jet*>((const Jet*) whad()->first );
		Jet *wh2  = const_cast<Jet*>((const Jet*) whad()->second);
		TLorentzVector *l = const_cast<TLorentzVector*>(wlep()->first);
		solver.Solve(bhad, wh1, wh2, blep, l, &met);

		nu_chisq           = solver.NSChi2() ;
		full_discriminant  = solver.Res()		 ;
		nu_discriminant    = solver.NSRes()	 ;
		btag_discriminant  = solver.BTagRes();
		mass_discriminant  = solver.MassRes();
		nu_ = solver.Nu();
		wlep()->second = &nu_;
	}

	WHypothesis * whad() {
		if(!wplus.isLeptonic) return &wplus;
		else if(!wminus.isLeptonic) return &wminus;
		return NULL;
	}

	WHypothesis * wlep() {
		if(wplus.isLeptonic) return &wplus;
		else if(wminus.isLeptonic) return &wminus;
		return NULL;
	}

	const TLorentzVector *blep() {
		if(wplus.isLeptonic) return b;
		else if(wminus.isLeptonic) return bbar;
		return NULL;
	}

	const TLorentzVector *bhad() {
		if(!wplus.isLeptonic) return b;
		else if(!wminus.isLeptonic) return bbar;
		return NULL;
	}

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

	double thad_mass() const {
		if(!wplus.isLeptonic) return top_mass();
		else if(!wminus.isLeptonic) return tbar_mass();
		else return -1;
	}
};

#endif
