#ifndef OJETH
#define OJETH
#include <Jet.h> 
#include <OMuon.h> 
#include <OElectron.h> 
#include <OPhoton.h> 

using namespace std;

class OJet : public Jet
{
		GenBasicParticle* genp_;
		int mcflavour;
		double mcpt;
		int genjet;
		void MatchGenJet();
	public:
		OJet(IOPFJet jet);
		bool Clean(const vector<OMuon*>& muons, const vector<OElectron*>& electrons, const vector<OPhoton*>& photons) const;
		GenBasicParticle* GenParticle() const {return(genp_);}
		void SetGen(GenBasicParticle* genp);
		void CalculateMCFlavour();
		int GetMCFlavour() {CalculateMCFlavour(); return(mcflavour);}
		double GetMCPt() {CalculateMCFlavour(); return(mcpt);}
		int GetGenJet(){MatchGenJet(); return(genjet);}
		
};
#endif
