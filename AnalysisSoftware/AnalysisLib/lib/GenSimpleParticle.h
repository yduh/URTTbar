#ifndef CGENSIMPLEPH
#define CGENSIMPLEPH
#include <BASEIO.h> 
#include "GenBasicParticle.h"
#include "GenSelectedParticle.h"
#include "TLorentzVector.h" 

using namespace BASEIO;

class GenSimpleParticle : public TLorentzVector
{
	private:
		int status;
		int pdgid;
	public:
		GenSimpleParticle(GenSelectedParticle p);
		GenSimpleParticle(GenBasicParticle p);
		int PDGID() const {return pdgid;}
		int Status() const {return status;}


};
#endif
