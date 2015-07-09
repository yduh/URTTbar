#include "OElectron.h"
#include "Analyse.h"
#include "SuperCluster.h"
#include "GenBasicParticle.h"
#include "TMath.h"

using namespace TMath;


OElectron::OElectron(IOElectron el) : Electron(el), genp_(0)
{}

double OElectron::CorPFIsolation2011() const
{
	double eta = Abs(SuperCluster(SC()).Eta());
	double effarea = 0.;
	effarea = 0.13;
	if(eta < 1.){ effarea = 0.10;}
	else if(eta < 1.479){ effarea = 0.12;}
	else if(eta < 2.){ effarea = 0.085;}
	else if(eta < 2.2){ effarea = 0.11;}
	else if(eta < 2.3){ effarea = 0.12;}
	else if(eta < 2.4){ effarea = 0.12;}

	return((PFR3().Charged() + Max(PFR3().Neutral() + PFR3().Photon() - Max(GLAN->AK5PFRho(), 0.f)*effarea, 0.))/Pt());
}

double OElectron::CorPFIsolation2012() const
{
	double eta = Abs(SuperCluster(SC()).Eta());
	double effarea = 0.;
	effarea = 0.14;
	if(eta < 1.){ effarea = 0.13;}
	else if(eta < 1.479){ effarea = 0.14;}
	else if(eta < 2.){ effarea = 0.07;}
	else if(eta < 2.2){ effarea = 0.09;}
	else if(eta < 2.3){ effarea = 0.11;}
	else if(eta < 2.4){ effarea = 0.11;}

	return((PFR3().Charged() + Max(PFR3().Neutral() + PFR3().Photon() - Max(GLAN->AK5PFRho(), 0.f)*effarea, 0.))/Pt());
}

bool OElectron::Clean() const
{
    TVector3 spike;
    TVector3 test(SuperCluster(SC()).Vect());
    spike.SetPtEtaPhi(1., -1.76, 1.37);
    if(spike.DeltaR(test) < 0.05){return(false);}
    spike.SetPtEtaPhi(1., 2.37, 2.69);
    if(spike.DeltaR(test) < 0.05){return(false);}

	if(Abs(test.Eta()) > 2.5) return(false);
	if(Abs(test.Eta()) < 1.566 && Abs(test.Eta()) > 1.4442) return(false);

	return(true);
}

bool OElectron::ID(Int_t idtyp, Int_t isotyp) const
{
	if(!Clean()) return(false);
	if(idtyp == 0) return(true);
	else if(idtyp == 1)
	{
		if(IsEB())
		{
			if(Abs(DeltaEtaSCTrack()) > 0.004){return(false);}
			if(Abs(DeltaPhiSCTrack()) > 0.06){return(false);}
			if(SigmaIEtaIEta() > 0.01){return(false);}
			if(EHCalTowerOverECalD1() + EHCalTowerOverECalD2() > 0.12){return(false);}
			if(Abs(GSFTrack().Dxy()) > 0.02){return(false);}
			if(Abs(GSFTrack().Dz()) > 0.1){return(false);}
			if(Abs((1. - ESCOverETrack())/ECalEnergy()) > 0.05){return(false);}
			if(HasMatchedConversion()){return(false);}
			if(GSFTrack().NMissingInnerHits() > 1){return(false);}
			if(isotyp == 1 && CorPFIsolation2012() > 0.15){return(false);}
			return(true);
		}
		if(IsEE())
		{   
			if(Abs(DeltaEtaSCTrack()) > 0.007){return(false);}
			if(Abs(DeltaPhiSCTrack()) > 0.03){return(false);}
			if(SigmaIEtaIEta() > 0.03){return(false);}
			if(EHCalTowerOverECalD1() + EHCalTowerOverECalD2() > 0.10){return(false);}
			if(Abs(GSFTrack().Dxy()) > 0.02){return(false);}
			if(Abs(GSFTrack().Dz()) > 0.1){return(false);}
			if(Abs((1. - ESCOverETrack())/ECalEnergy()) > 0.05){return(false);}
			if(HasMatchedConversion()){return(false);}
			if(GSFTrack().NMissingInnerHits() > 1){return(false);}
			if(isotyp == 1 && CorPFIsolation2012() > 0.15){return(false);}
			return(true);
		}
	}
	else if(idtyp == 2)
	{
		if(IsEB())
		{
			if(Abs(DeltaEtaSCTrack()) > 0.007){return(false);}
			if(Abs(DeltaPhiSCTrack()) > 0.15){return(false);}
			if(SigmaIEtaIEta() > 0.01){return(false);}
			if(EHCalTowerOverECalD1() + EHCalTowerOverECalD2() > 0.12){return(false);}
			if(Abs(GSFTrack().Dxy()) > 0.02){return(false);}
			if(Abs(GSFTrack().Dz()) > 0.2){return(false);}
			if(Abs((1. - ESCOverETrack())/ECalEnergy()) > 0.05){return(false);}
			if(HasMatchedConversion()){return(false);}
			if(GSFTrack().NMissingInnerHits() > 1){return(false);}
			if(isotyp == 1 && CorPFIsolation2012() > 0.15){return(false);}
			return(true);
		}
		if(IsEE())
		{   
			if(Abs(DeltaEtaSCTrack()) > 0.009){return(false);}
			if(Abs(DeltaPhiSCTrack()) > 0.1){return(false);}
			if(SigmaIEtaIEta() > 0.03){return(false);}
			if(EHCalTowerOverECalD1() + EHCalTowerOverECalD2() > 0.10){return(false);}
			if(Abs(GSFTrack().Dxy()) > 0.02){return(false);}
			if(Abs(GSFTrack().Dz()) > 0.2){return(false);}
			if(Abs((1. - ESCOverETrack())/ECalEnergy()) > 0.05){return(false);}
			if(HasMatchedConversion()){return(false);}
			if(GSFTrack().NMissingInnerHits() > 1){return(false);}
			if(isotyp == 1 && CorPFIsolation2012() > 0.15){return(false);}
			return(true);
		}
	}
	return(false);
}

void OElectron::SetGen(GenBasicParticle* genp)
{
genp_ = genp;
}

