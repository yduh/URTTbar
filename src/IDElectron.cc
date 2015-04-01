#include "IDElectron.h"
#include <TMath.h>
#include <iostream>

IDElectron::IDElectron(const Electron el) : Electron(el)
{
}

double IDElectron::CorPFIsolation2012(double eta) const
{
	double effarea = 0.;
	effarea = 0.14;
	if(eta < 1.){ effarea = 0.13;}
	else if(eta < 1.479){ effarea = 0.14;}
	else if(eta < 2.){ effarea = 0.07;}
	else if(eta < 2.2){ effarea = 0.09;}
	else if(eta < 2.3){ effarea = 0.11;}
	else if(eta < 2.4){ effarea = 0.11;}

	if(streamer != 0)
	{
		effarea *= Max(streamer->rho().value(), 0.);
	}
	else {effarea = 0; cerr << "inittialize IDElectron::stream = URStreamer object for rho correction" << endl;}
	//return((PFR3().Charged() + Max(PFR3().Neutral() + PFR3().Photon() - Max(GLAN->AK5PFRho(), 0.f)*effarea, 0.))/Pt());
	return(chargedIso() + Max(neutralIso() + photonIso() - effarea, 0.))/Pt();
}

bool IDElectron::ID(IDS idtyp)
{
	double sceta = Abs(TVector3(x(), y(), z()).Eta());
	if(sceta > 2.5) return(false);
	if(sceta < 1.566 && sceta > 1.4442) return(false);

	if(idtyp == MEDIUM_12)
	{
		if(isEB())
		{
			if(Abs(DEtaSCTrk()) > 0.004){return(false);}
			if(Abs(DPhiSCTrk()) > 0.06){return(false);}
			if(sigmaIEtaIEta() > 0.01){return(false);}
			if(hcalDepth1OverEcalBc() + hcalDepth2OverEcalBc() > 0.12){return(false);}
			if(Abs(dB()) > 0.02){return(false);}
			if(Abs(dz()) > 0.1){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.05){return(false);}
			if(CorPFIsolation2012(sceta) > 0.15){return(false);}
			if(!passConversionVeto()){return(false);}
			return(true);
		}
		if(isEE())
		{
			if(Abs(DEtaSCTrk()) > 0.007){return(false);}
			if(Abs(DPhiSCTrk()) > 0.03){return(false);}
			if(sigmaIEtaIEta() > 0.03){return(false);}
			if(hcalDepth1OverEcalBc() + hcalDepth2OverEcalBc() > 0.10){return(false);}
			if(Abs(dB()) > 0.02){return(false);}
			if(Abs(dz()) > 0.1){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.05){return(false);}
			if(CorPFIsolation2012(sceta) > 0.15){return(false);}
			if(!passConversionVeto()){return(false);}
			return(true);
		}
	}
	else if(idtyp == LOOSE_12)
	{
		if(isEB())
		{
			if(Abs(DEtaSCTrk()) > 0.007){return(false);}
			if(Abs(DPhiSCTrk()) > 0.15){return(false);}
			if(sigmaIEtaIEta() > 0.01){return(false);}
			if(hcalDepth1OverEcalBc() + hcalDepth2OverEcalBc() > 0.12){return(false);}
			if(Abs(dB()) > 0.02){return(false);}
			if(Abs(dz()) > 0.2){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.05){return(false);}
			if(CorPFIsolation2012(sceta) > 0.15){return(false);}
			if(!passConversionVeto()){return(false);}
			return(true);
		}
		if(isEE())
		{
			if(Abs(DEtaSCTrk()) > 0.009){return(false);}
			if(Abs(DPhiSCTrk()) > 0.1){return(false);}
			if(sigmaIEtaIEta() > 0.03){return(false);}
			if(hcalDepth1OverEcalBc() + hcalDepth2OverEcalBc() > 0.10){return(false);}
			if(Abs(dB()) > 0.02){return(false);}
			if(Abs(dz()) > 0.2){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.05){return(false);}
			if(CorPFIsolation2012(sceta) > 0.15){return(false);}
			if(!passConversionVeto()){return(false);}
			return(true);
		}
	}
	return(false);
}

URStreamer* IDElectron::streamer = 0;

