#include "GenBasicParticle.h"
#include "TMath.h" 
#include "Analyse.h"

GenBasicParticle::GenBasicParticle(AllGenParticle p) : AllGenParticle(p), TLorentzVector(p.px(), p.py(), p.pz(), TMath::Sqrt(p.px()*p.px() + p.py()*p.py() + p.pz()*p.pz())) {}


GenBasicParticle GenBasicParticle::GetMother(Int_t n)
{
	return GenBasicParticle(GLAN->GetAllGenParticle(Mother(n)));
}

GenBasicParticle GenBasicParticle::GetDaughter(Int_t n)
{
	return GenBasicParticle(GLAN->GetAllGenParticle(Daughter(n)));
}

bool GenBasicParticle::HasMother(Int_t pdgid)
{
	for(UInt_t m = 0 ; m < Num_Mother() ; ++m)
	{
		if(GetMother(m).PDGID() == pdgid){return true;}
	}
	return false;
}

void GenBasicParticle::StableDecayProducts(vector<GenBasicParticle>& decayprods, int level)
{
	level--;
	if(level == 0){cerr << "WARNING StableDecayProducts: reached max number of recursions." << endl; return;}
	if(Status() == 1)
	{
		if(!any_of(decayprods.begin(), decayprods.end(), [&](GenBasicParticle& A){return(*this == A);}))
		{
			decayprods.push_back(*this);
		}
		return;	
	}
	
	for(UInt_t m = 0 ; m < Num_Daughter() ; ++m)
	{
		GetDaughter(m).StableDecayProducts(decayprods, level);
	}
}

void GenBasicParticle::DecayProducts(vector<GenBasicParticle>& decayprods)
{
	if(Status() == 1)
	{
		return;	
	}

	if(Num_Daughter() == 1 && GetDaughter(0).PDGID() == PDGID())
	{
		GetDaughter(0).DecayProducts(decayprods);
		return;
	}
	
	for(UInt_t m = 0 ; m < Num_Daughter() ; ++m)
	{
		decayprods.push_back(GetDaughter(m));
	}
}

bool GenBasicParticle::HasAnyMother(Int_t pdgid, int level)
{
	if(level == 0){return false;}
	level--;
	//cout << "start " << PDGID() << endl;
	if(PDGID() == pdgid){return true;}
	for(UInt_t m = 0 ; m < Num_Mother() ; ++m)
	{
		GenBasicParticle gp(GetMother(m));
		bool found = gp.HasAnyMother(pdgid, level);
		if(found){return true;}
	}
	return false;
}
