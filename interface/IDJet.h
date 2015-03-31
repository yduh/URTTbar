#ifndef IDJET_H
#define IDJET_H
#include "URStreamer.h"
#include "IDMuon.h"
#include "IDElectron.h"

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
