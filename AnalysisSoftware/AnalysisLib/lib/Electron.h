#ifndef CELECTRONH
#define CELECTRONH
#include <BASEIO.h> 
#include "TLorentzVector.h" 
#include "TriggerObject.h"

using namespace BASEIO;

class Electron : public IOElectron, public TLorentzVector, public TriggerObject
{
	public:
		Electron(IOElectron el);
		bool IsEB() const {return IOElectron::Info() & 1<<0;}
		bool IsEE() const {return IOElectron::Info() & 1<<1;}
		bool HasMatchedConversion() const {return IOElectron::Info() & 1<<10;}
		int Charge() const {return GSFTrack().Charge();}

};
#endif
