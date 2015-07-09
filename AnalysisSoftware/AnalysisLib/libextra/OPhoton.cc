#include "OPhoton.h"
#include "Analyse.h"
#include "SuperCluster.h"

OPhoton::OPhoton(Photon ph) : Photon(ph), genp_(0)
{}

double OPhoton::EAcharged() const
{
	double sceta = Abs(SuperCluster(SC()).Eta());
	if(sceta < 1.) return(0.012);
	if(sceta < 1.479) return(0.010);
	if(sceta < 2.0) return(0.014);
	if(sceta < 2.2) return(0.012);
	if(sceta < 2.3) return(0.016);
	if(sceta < 2.4) return(0.020);
	return(0.012);
}

double OPhoton::EAneutral() const
{
	double sceta = Abs(SuperCluster(SC()).Eta());
	if(sceta < 1.) return(0.030);
	if(sceta < 1.479) return(0.057);
	if(sceta < 2.0) return(0.039);
	if(sceta < 2.2) return(0.015);
	if(sceta < 2.3) return(0.024);
	if(sceta < 2.4) return(0.039);
	return(0.072);
}

double OPhoton::EAphoton() const
{
	double sceta = Abs(SuperCluster(SC()).Eta());
	if(sceta < 1.) return(0.148);
	if(sceta < 1.479) return(0.130);
	if(sceta < 2.0) return(0.112);
	if(sceta < 2.2) return(0.216);
	if(sceta < 2.3) return(0.262);
	if(sceta < 2.4) return(0.260);
	return(0.266);
}

bool OPhoton::Clean() const
{
	SuperCluster sc(SC());
	if(sc.Eta() > 2.5) return(false);
	if(sc.Eta() > 1.4442 && sc.Eta() < 1.566) return(false);
	if(R9() > 1.) return(false);
	if(R9() < 0.003) return(false);
	TVector3 spike;
	spike.SetPtEtaPhi(1., -1.76, 1.37);
	if(spike.DeltaR(sc.Vect()) < 0.05){return(false);}
	spike.SetPtEtaPhi(1., 2.37, 2.69);
	if(spike.DeltaR(sc.Vect()) < 0.05){return(false);}
	return(true);
}

bool OPhoton::ID(Int_t idtyp) const
{
	if(idtyp == 0) return(true);
	if(idtyp == 1)
	{
		if(!Clean()) return(false);
		if(EHCalTowerOverECalD1() + EHCalTowerOverECalD1() > 0.05) return(false);
		if(HasPromptElectron()) return(false);
		if(IsEB())
		{
			if(SigmaIEtaIEta() > 0.011) return(false);
			if(Max(PFR3().Photon() - GLAN->AK5PFRho()*EAphoton(), 0.) - 0.7 - 0.005*Pt() > 0.) return(false);
			if(Max(PFR3().Charged() - GLAN->AK5PFRho()*EAcharged(), 0.) - 1.5 > 0.) return(false);
			if(Max(PFR3().Neutral() - GLAN->AK5PFRho()*EAneutral(), 0.) - 1.0 - 0.04*Pt() > 0.) return(false);
		}
		else if(IsEE())
		{
			if(SigmaIEtaIEta() > 0.033) return(false);
			if(Max(PFR3().Photon() - GLAN->AK5PFRho()*EAphoton(), 0.) - 1.0 - 0.005*Pt() > 0.) return(false);
			if(Max(PFR3().Charged() - GLAN->AK5PFRho()*EAcharged(), 0.) - 1.2 > 0.) return(false);
			if(Max(PFR3().Neutral() - GLAN->AK5PFRho()*EAneutral(), 0.) - 1.5 - 0.04*Pt() > 0.) return(false);
		}
		else
		{
			return(false);
		}
		return(true);
	}
	return(false);
}

void OPhoton::SetGen(GenBasicParticle* genp) {genp_ = genp;}
