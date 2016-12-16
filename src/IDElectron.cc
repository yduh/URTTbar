#include "IDElectron.h"
#include <TMath.h>
#include <iostream>

using namespace TMath;

double IDElectron::CorPFIsolation() const
{
	double eta = Abs(Eta());
	double effarea = 0.2393;
	if(eta < 1.){ effarea = 0.1703;}
	else if(eta < 1.479){ effarea = 0.1715;}
	else if(eta < 2.){ effarea = 0.1213;}
	else if(eta < 2.2){ effarea = 0.1230;}
	else if(eta < 2.3){ effarea = 0.1635;}
	else if(eta < 2.4){ effarea = 0.1937;}

	if(streamer != 0)
	{
		effarea *= Max(streamer->rho().value(), 0.);
	}
	else {effarea = 0; cerr << "initialize IDElectron::stream = URStreamer object for rho correction" << endl;}
	return(pfChargedIso() + Max(pfNeutralIso() + pfPhotonIso() - effarea, 0.))/Pt();
}

bool IDElectron::ID(IDS idtyp)
{
	if(Abs(Eta()) > 2.4) return(false);
	double sceta = abs(SCeta());
	if(sceta > 2.5) return(false);
	if(sceta < 1.566 && sceta > 1.4442) return(false);
	else if(idtyp == TIGHT_16)
	{
		if(isEB())
		{
			if(full5x5_sigmaIEtaIEta() > 0.00998){return(false);}
			if(Abs(DEtaSCTrk()) > 0.00308){return(false);}
			if(Abs(DPhiSCTrk()) > 0.0816){return(false);}
			if(hadronicOverEM() > 0.0414){return(false);}
			if(USEISO && CorPFIsolation() > 0.0588){return(false);}
			if(Abs((1. - ESCOverETrack())/ecalEnergy()) > 0.0129){return(false);}
			if(nMissingInnerHits() > 1){return(false);}
			if(conversion()){return(false);}
			if(Abs(dB()) > 0.05){return(false);}
			if(Abs(dz()) > 0.1){return(false);}
			return(true);
		}
		if(isEE())
		{
			if(full5x5_sigmaIEtaIEta() > 0.0292){return(false);}
			if(Abs(DEtaSCTrk()) > 0.00605){return(false);}
			if(Abs(DPhiSCTrk()) > 0.0394){return(false);}
			if(hadronicOverEM() > 0.0641){return(false);}
			if(USEISO && CorPFIsolation() > 0.0571){return(false);}
			if(Abs((1. - ESCOverETrack())/ecalEnergy()) > 0.0129){return(false);}
			if(nMissingInnerHits() > 1){return(false);}
			if(conversion()){return(false);}
			if(Abs(dB()) > 0.1){return(false);}
			if(Abs(dz()) > 0.2){return(false);}
			return(true);
		}
	}
	else if(idtyp == LOOSE_16)
	{
		if(isEB())
		{
			if(full5x5_sigmaIEtaIEta() > 0.011){return(false);}
			if(Abs(DEtaSCTrk()) > 0.00477){return(false);}
			if(Abs(DPhiSCTrk()) > 0.222){return(false);}
			if(hadronicOverEM() > 0.298){return(false);}
			if(USEISO && CorPFIsolation() > 0.0994){return(false);}
			if(Abs((1. - ESCOverETrack())/ecalEnergy()) > 0.241){return(false);}
			if(nMissingInnerHits() > 1){return(false);}
			if(conversion()){return(false);}
			if(Abs(dB()) > 0.05){return(false);}
			if(Abs(dz()) > 0.1){return(false);}
			return(true);
		}
		if(isEE())
		{
			if(full5x5_sigmaIEtaIEta() > 0.0314){return(false);}
			if(Abs(DEtaSCTrk()) > 0.00868){return(false);}
			if(Abs(DPhiSCTrk()) > 0.213){return(false);}
			if(hadronicOverEM() > 0.101){return(false);}
			if(USEISO && CorPFIsolation() > 0.107){return(false);}
			if(Abs((1. - ESCOverETrack())/ecalEnergy()) > 0.14){return(false);}
			if(nMissingInnerHits() > 1){return(false);}
			if(conversion()){return(false);}
			if(Abs(dB()) > 0.1){return(false);}
			if(Abs(dz()) > 0.2){return(false);}
			return(true);
		}
	}
	return(false);
}

URStreamer* IDElectron::streamer = 0;
bool IDElectron::USEISO = true;

