#ifndef CGENJETH
#define CGENJETH
#include <BASEIO.h> 
#include "TLorentzVector.h" 

using namespace BASEIO;

class GenJet : public IOGenAK4Jet, public TLorentzVector
{
	public:
		GenJet(IOGenAK4Jet p);
};
#endif
