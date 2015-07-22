#ifndef IDMET_H
#define IDMET_H
#include "MCMatchable.h"
#include "URStreamer.h"
#include <TMath.h>

class IDMet : public Met, public TLorentzVector
{
public:
	IDMet() {}
	IDMet(const Met met):
		Met(met), TLorentzVector(met.px(), met.py(), 0., TMath::Sqrt(met.px()*met.px() + met.py()*met.py()))
		{
		}
	double pxunctot() {return(TMath::Sqrt(pxunc()*pxunc() +pxuncjet()*pxuncjet()));}
	double pyunctot() {return(TMath::Sqrt(pyunc()*pyunc() +pyuncjet()*pyuncjet()));}
};
#endif
