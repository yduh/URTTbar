#ifndef CGENBPH
#define CGENBPH
#include <BASEIO.h> 
#include "TLorentzVector.h" 

using namespace BASEIO;

class GenBasicParticle : public AllGenParticle, public TLorentzVector
{
	public:
		GenBasicParticle(AllGenParticle p);
		GenBasicParticle GetMother(Int_t n);
		GenBasicParticle GetDaughter(Int_t n);
		bool HasMother(Int_t pdgid);
		bool HasAnyMother(Int_t pdgid, int level = 50);
		void DecayProducts(vector<GenBasicParticle>& decayprods);
		void StableDecayProducts(vector<GenBasicParticle>& decayprods, int level = 50);

};
#endif
