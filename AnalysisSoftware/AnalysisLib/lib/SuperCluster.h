#ifndef CSCH
#define CSCH
#include <BASEIO.h> 
#include "TLorentzVector.h" 

using namespace BASEIO;

class SuperCluster : public IOSuperCluster, public TLorentzVector
{
	public:
		SuperCluster(IOSuperCluster sc);

};
#endif
