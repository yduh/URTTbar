#ifndef OPHOTONH
#define OPHOTONH
#include <Photon.h> 

using namespace std;

class GenBasicParticle;

class OPhoton : public Photon
{
	private:
		GenBasicParticle* genp_;
		double EAcharged() const;
		double EAneutral() const;
		double EAphoton() const;
	public:
		OPhoton(Photon ph);
		bool ID(Int_t idtyp = 1) const;
		bool Clean() const;
		GenBasicParticle* GenParticle() const {return(genp_);}
		void SetGen(GenBasicParticle* genp);
		
};
#endif
