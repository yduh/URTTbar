#ifndef IDJET_H
#define IDJET_H
#include "URStreamer.h"
#include "IDMuon.h"
#include "IDElectron.h"
#include <TMath.h>

class IDJet : public Jet
{
private:

public:
	IDJet(const Jet jet) : Jet(jet)
	{
	}
	bool ID()
	{
		//to be filled in new tree version
		if(numberOfDaughters() <= 1) {return false;}
		if(neutralHadronEnergyFraction() + HFHadronEnergyFraction() >= 0.99){return false;}
		if(neutralEmEnergyFraction() >= 0.99){return false;}
		if(TMath::Abs(Eta()) < 2.4)
		{
			if(chargedEmEnergyFraction() >= 0.99){return false;}
			if(chargedHadronEnergyFraction() <= 0.){return false;}
			if(chargedMultiplicity() <= 0.){return false;}
		}
		return(true);
	}
	bool Clean(const vector<IDMuon*>& muons, const vector<IDElectron*>& electrons, double distpar = 0.3)
	{
		for(const IDMuon* mu : muons)
		{
			if(DeltaR(*mu) < distpar)
			{
				return false;
			}
		}
		for(const IDElectron* el : electrons)
		{
			if(DeltaR(*el) < distpar)
			{
				return false;
			}
		}
		return true;
	}

};
#endif
