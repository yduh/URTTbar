#ifndef IDMET_H
#define IDMET_H
#include "MCMatchable.h"
#include "URStreamer.h"
#include <TMath.h>

//class IDMet : public Nohfmet, public TLorentzVector
//class IDMet : public Met, public TLorentzVector
class IDMet : public Met
{
public:
	IDMet() {}
	//IDMet(const Nohfmet met):
	IDMet(const Met met):
		//Nohfmet(met), TLorentzVector(met.px(), met.py(), 0., TMath::Sqrt(met.px()*met.px() + met.py()*met.py()))
		//Met(met), TLorentzVector(met.px(), met.py(), 0., TMath::Sqrt(met.px()*met.px() + met.py()*met.py()))
		Met(met)
		{
		}
};
#endif
