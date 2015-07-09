#ifndef OMUONH
#define OMUONH
#include <Muon.h> 

class GenBasicParticle;

bool TestMuonInnerTrack(const IOTrack& trk);

using namespace std;

class OMuon : public Muon
{
		GenBasicParticle* genp_;
	public:
		OMuon(IOMuon mu);
		bool IsGoodSA() const;
		bool IsGoodTRK() const;
		bool IsMatched() const;
		bool IsISO() const;
		bool ID(Int_t idtyp = 1) const;
		GenBasicParticle* GenParticle() const {return(genp_);}
		void SetGen(GenBasicParticle* genp);
		
};
#endif
