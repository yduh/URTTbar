#ifndef CTRACKH
#define CTRACKH
#include <BASEIO.h> 
#include "TLorentzVector.h" 

using namespace BASEIO;

class Track : public IOTrack, public TLorentzVector
{
	public:
		Track(IOTrack trk);

};
#endif
