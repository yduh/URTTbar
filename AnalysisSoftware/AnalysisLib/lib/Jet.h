#ifndef CJETH
#define CJETH
#include <BASEIO.h> 
#include "TLorentzVector.h" 
#include "TriggerObject.h"

using namespace BASEIO;

class Jet : public IOPFJet, public TLorentzVector, public TriggerObject
{
	public:
		Jet(IOPFJet jet);

};
#endif
