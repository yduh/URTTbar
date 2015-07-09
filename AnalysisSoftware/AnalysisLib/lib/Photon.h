#ifndef CPHOTONH
#define CPHOTONH
#include <BASEIO.h> 
#include "TLorentzVector.h" 
#include "TriggerObject.h"

using namespace BASEIO;

class SuperCluster;

class Photon : public IOPhoton, public TLorentzVector, public TriggerObject
{
	public:
		Photon(IOPhoton ph);
		bool IsEB() const {return IOPhoton::Info() & 1<<0;}
		bool IsEE() const {return IOPhoton::Info() & 1<<1;}
		bool IsPFlowPhoton() const {return IOPhoton::Info() & 1<<2;}
		bool HasConversionTracks() const {return IOPhoton::Info() & 1<<3;}
		bool HasPixelSeed() const {return IOPhoton::Info() & 1<<4;}
		bool HasPromptElectron() const {return IOPhoton::Info() & 1<<5;}
		bool R9() const {return E3x3()/SC().RawEnergy();}
		//SuperCluster SC();

};
#endif
