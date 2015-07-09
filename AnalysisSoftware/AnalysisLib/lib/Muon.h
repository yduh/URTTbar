#ifndef CMUONH
#define CMUONH
#include <BASEIO.h> 
#include "TLorentzVector.h" 
#include "TriggerObject.h"

using namespace BASEIO;

class Muon : public IOMuon, public TLorentzVector, public TriggerObject
{
	public:
		Muon(IOMuon mu);
		bool IsGlobal() const {return IOMuon::Info() & 1 << 0;}
		bool IsTracker() const {return IOMuon::Info() & 1 << 1;}
		bool IsStandAlone() const {return IOMuon::Info() & 1 << 2;}
		bool IsCalo() const {return IOMuon::Info() & 1 << 3;}
		bool IsPF() const {return IOMuon::Info() & 1 << 4;}
		int Charge() const {return (IOMuon::Info() & 1 << 5) ? 1 : -1;}

};
#endif
