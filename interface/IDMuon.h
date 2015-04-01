#ifndef IDMUON_H
#define IDMUON_H
#include "URStreamer.h"
#include "MCMatchable.h"
#include <TMath.h>


class IDMuon : public Muon, public MCMatchable
{
private:

public:
	IDMuon(const Muon mu) : 
		Muon(mu),
		MCMatchable()
	{
	}
	enum IDS {TIGHT_12, LOOSE_12};
	bool ID(IDS idtyp)
	{
		if(idtyp == TIGHT_12)
		{
			if(TMath::Abs(Eta()) > 2.4) return(false);
			if(!isPF()) return(false);
			if(!isGlobal()) return(false);
			if(pixelHits() <= 0) return(false);
			if(trackerLayers() <= 5) return(false);
			if(validHits() <= 0) return(false);
			if(numMatchedStations() <= 1) return(false);
			if(TMath::Abs(dB()) > 0.2) return(false);
			if(TMath::Abs(dz()) > 0.5) return(false);
			if(chi2()/ndof() > 10.) return(false);
			//if((chargedIso() + neutralIso() + photonIso())/Pt() > 0.12) return(false);
			if((trackiso())/Pt() > 0.05) return(false);
			return(true);
		}
		else if(idtyp == LOOSE_12)
		{
			if(TMath::Abs(Eta()) > 2.4) return(false);
			if(!isPF()) return(false);
			if(!isGlobal() && !isTracker()) return(false);
			//if((chargedIso() + neutralIso() + photonIso())/Pt() > 0.2) return(false);
			if((trackiso())/Pt() > 0.1) return(false);
			return(true);
		}
		return(false);
	}

};
#endif
