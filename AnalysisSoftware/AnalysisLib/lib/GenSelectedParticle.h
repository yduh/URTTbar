#ifndef CGENSPH
#define CGENSPH
#include <BASEIO.h> 
#include "TLorentzVector.h" 

using namespace BASEIO;

class GenSelectedParticle : public SelectedGenParticle, public TLorentzVector
{
	public:
		GenSelectedParticle(SelectedGenParticle p);
		GenSelectedParticle GetMother(Int_t n);
		GenSelectedParticle GetDaughter(Int_t n);
		enum INFO {FromWp=1<<0, FromWm=1<<1, FromGamma=1<<2, FromZ=1<<3, Fromh=1<<4, FromH=1<<5, FromA=1<<6, FromHp=1<<7, FromHm=1<<8, Fromt=1<<9, Fromtbar=1<<10, Fromb=1<<11, Frombbar=1<<12, FromZprime=1<<13, Fromtprime=1<<14, Fromtprimebar=1<<15, Fromtaup=1<<16, Fromtaum=1<<17};
		UInt_t MoreInfo(INFO test) const {return(SelectedGenParticle::Info() & test);}

};
#endif
