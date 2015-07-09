#ifndef OELECTRONH
#define OELECTRONH
#include <Electron.h> 

class GenBasicParticle;

using namespace std;

class OElectron : public Electron
{
	private:
		GenBasicParticle* genp_;
	public:
		OElectron(IOElectron el);
		//OElectron(){}
		double CorPFIsolation2011() const;
		double CorPFIsolation2012() const;
		bool ID(Int_t idtyp = 1, Int_t isotyp = 1) const;
		bool Clean() const;
		GenBasicParticle* GenParticle() const {return(genp_);}
		void SetGen(GenBasicParticle* genp);
		
};
#endif
