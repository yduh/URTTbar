#include "IDElectron.h"
#include <TMath.h>
#include <iostream>

using namespace TMath;

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
	else {effarea = 0; cerr << "initialize IDElectron::stream = URStreamer object for rho correction" << endl;}
	//return((PFR3().Charged() + Max(PFR3().Neutral() + PFR3().Photon() - Max(GLAN->AK5PFRho(), 0.f)*effarea, 0.))/Pt());
	return(chargedIso() + Max(neutralIso() + photonIso() - effarea, 0.))/Pt();
}

double IDElectron::CorPFIsolation2015() const
{
	//double eta = Abs(TVector3(x(), y(), z()).Eta());
	double eta = Abs(Eta());
	double effarea = 0.;
	//if(eta < 0.8){ effarea = 0.1396;}
	//else if(eta < 1.479){ effarea = 0.1352;}
	//else if(eta < 2.1){ effarea = 0.0816;}
	//else if(eta < 2.5){ effarea = 0.2005;}
	if(eta < 0.8){ effarea = 0.1013;}
	else if(eta < 1.3){ effarea = 0.0988;}
	else if(eta < 2.0){ effarea = 0.0572;}
	else if(eta < 2.2){ effarea = 0.0842;}
	else if(eta < 2.5){ effarea = 0.1530;}

	if(streamer != 0)
	{
		effarea *= Max(streamer->rho().value(), 0.);
	}
	else {effarea = 0; cerr << "initialize IDElectron::stream = URStreamer object for rho correction" << endl;}
	//return((PFR3().Charged() + Max(PFR3().Neutral() + PFR3().Photon() - Max(GLAN->AK5PFRho(), 0.f)*effarea, 0.))/Pt());
	return(chargedIso() + Max(neutralIso() + photonIso() - effarea, 0.))/Pt();
}

double IDElectron::CorPFIsolation2015_25ns() const
{
	//double eta = Abs(TVector3(x(), y(), z()).Eta());
	double eta = Abs(Eta());
	double effarea = 0.1475;
	//25ns
	if(eta < 1.){ effarea = 0.0960;}
	else if(eta < 1.479){ effarea = 0.0947;}
	else if(eta < 2.){ effarea = 0.0580;}
	else if(eta < 2.2){ effarea = 0.0688;}
	else if(eta < 2.3){ effarea = 0.0967;}
	else if(eta < 2.4){ effarea = 0.1195;}

	if(streamer != 0)
	{
		effarea *= Max(streamer->rho().value(), 0.);
	}
	else {effarea = 0; cerr << "initialize IDElectron::stream = URStreamer object for rho correction" << endl;}
	//return((PFR3().Charged() + Max(PFR3().Neutral() + PFR3().Photon() - Max(GLAN->AK5PFRho(), 0.f)*effarea, 0.))/Pt());
	return(chargedIso() + Max(neutralIso() + photonIso() - effarea, 0.))/Pt();
}

double IDElectron::PFIsoDb() const {
	return (chargedIso() + Max(neutralIso() + photonIso() - 0.5*puIso(), 0.));
}

bool IDElectron::ID(IDS idtyp)
{
	double sceta = Abs(TVector3(x(), y(), z()).Eta());
	if(sceta > 2.5) return(false);
	if(sceta < 1.566 && sceta > 1.4442) return(false);

	if(idtyp == MEDIUM_12 || idtyp == MEDIUM_12Db)
	{
		if(isEB())
		{
			if(Abs(DEtaSCTrk()) > 0.004){return(false);}
			if(Abs(DPhiSCTrk()) > 0.06){return(false);}
			if(sigmaIEtaIEta() > 0.01){return(false);}
			if(hadronicOverEM() > 0.12){return(false);}
			if(Abs(dB()) > 0.02){return(false);}
			if(Abs(dz()) > 0.1){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.05){return(false);}
			if(idtyp == MEDIUM_12Db && PFIsoDb()/Pt() > 0.15){return(false);}
			if(idtyp == MEDIUM_12 && CorPFIsolation2012(sceta) > 0.15){return(false);}
			if(!passConversionVeto()){return(false);}
			return(true);
		}
		if(isEE())
		{
			if(Abs(DEtaSCTrk()) > 0.007){return(false);}
			if(Abs(DPhiSCTrk()) > 0.03){return(false);}
			if(sigmaIEtaIEta() > 0.03){return(false);}
			if(hadronicOverEM() > 0.10){return(false);}
			if(Abs(dB()) > 0.02){return(false);}
			if(Abs(dz()) > 0.1){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.05){return(false);}
			if(idtyp == MEDIUM_12Db && PFIsoDb()/Pt()> 0.15){return(false);}
			if(idtyp == MEDIUM_12 && CorPFIsolation2012(sceta) > 0.15){return(false);}
			if(!passConversionVeto()){return(false);}
			return(true);
		}
	}
	else if(idtyp == LOOSE_12 || idtyp == LOOSE_12Db)
	{
		if(isEB())
		{
			if(Abs(DEtaSCTrk()) > 0.007){return(false);}
			if(Abs(DPhiSCTrk()) > 0.15){return(false);}
			if(sigmaIEtaIEta() > 0.01){return(false);}
			if(hadronicOverEM() > 0.12){return(false);}
			if(Abs(dB()) > 0.02){return(false);}
			if(Abs(dz()) > 0.2){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.05){return(false);}
			if(idtyp == LOOSE_12Db && PFIsoDb()/Pt()> 0.15){return(false);}
			if(idtyp == LOOSE_12 && CorPFIsolation2012(sceta) > 0.15){return(false);}
			if(!passConversionVeto()){return(false);}
			return(true);
		}
		if(isEE())
		{
			if(Abs(DEtaSCTrk()) > 0.009){return(false);}
			if(Abs(DPhiSCTrk()) > 0.1){return(false);}
			if(sigmaIEtaIEta() > 0.03){return(false);}
			if(hadronicOverEM() > 0.10){return(false);}
			if(Abs(dB()) > 0.02){return(false);}
			if(Abs(dz()) > 0.2){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.05){return(false);}
			if(idtyp == LOOSE_12Db && PFIsoDb()/Pt()> 0.15){return(false);}
			if(idtyp == LOOSE_12 && CorPFIsolation2012(sceta) > 0.15){return(false);}
			if(!passConversionVeto()){return(false);}
			return(true);
		}
	}
	else if(idtyp == MEDIUM_15)
	{
		//cout << dB() << " " << ipDXY() << endl;
		if(isEB())
		{
			if(Abs(DEtaSCTrk()) > 0.008925){return(false);}
			if(Abs(DPhiSCTrk()) > 0.035973){return(false);}
			if(full5x5_sigmaIEtaIEta() > 0.009996){return(false);}
			if(hadronicOverEM() > 0.050537){return(false);}
			if(Abs(dB()) > 0.012235){return(false);}
			if(Abs(dz()) > 0.042020){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.091942){return(false);}
			if(USEISO && CorPFIsolation2015() > 0.107587){return(false);}
			//if(chargedIso()/Pt() > 0.05){return(false);}
			if(!passConversionVeto()){return(false);}
			if(nMissingInnerHits() > 1){return(false);}
			return(true);
		}
		if(isEE())
		{
			if(Abs(DEtaSCTrk()) > 0.007429){return(false);}
			if(Abs(DPhiSCTrk()) > 0.067879){return(false);}
			if(full5x5_sigmaIEtaIEta() > 0.030135){return(false);}
			if(hadronicOverEM() > 0.086782){return(false);}
			if(Abs(dB()) > 0.036719){return(false);}
			if(Abs(dz()) > 0.138142){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.100683){return(false);}
			if(USEISO && CorPFIsolation2015() > 0.113254){return(false);}
			//if(chargedIso()/Pt() > 0.05){return(false);}
			if(!passConversionVeto()){return(false);}
			if(nMissingInnerHits() > 1){return(false);}
			return(true);
		}
	}
	else if(idtyp == LOOSE_15)
	{
		if(isEB())
		{
			if(Abs(DEtaSCTrk()) > 0.009277){return(false);}
			if(Abs(DPhiSCTrk()) > 0.094739){return(false);}
			if(sigmaIEtaIEta() > 0.010331){return(false);}
			if(hadronicOverEM() > 0.093068){return(false);}
			if(Abs(dB()) > 0.035904){return(false);}
			if(Abs(dz()) > 0.075496){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.189968){return(false);}
			if(USEISO && CorPFIsolation2015() > 0.130136){return(false);}
			//if(chargedIso()/Pt() > 0.1){return(false);}
			if(!passConversionVeto()){return(false);}
			if(nMissingInnerHits() > 1){return(false);}
			return(true);
		}
		if(isEE())
		{
			if(Abs(DEtaSCTrk()) > 0.009833){return(false);}
			if(Abs(DPhiSCTrk()) > 0.149934){return(false);}
			if(sigmaIEtaIEta() > 0.031838){return(false);}
			if(hadronicOverEM() > 0.115754){return(false);}
			if(Abs(dB()) > 0.099266){return(false);}
			if(Abs(dz()) > 0.197897){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.140662){return(false);}
			if(USEISO && CorPFIsolation2015() > 0.163368){return(false);}
			//if(chargedIso()/Pt() > 0.1){return(false);}
			if(!passConversionVeto()){return(false);}
			if(nMissingInnerHits() > 1){return(false);}
			return(true);
		}
	}
	else if(idtyp == MEDIUM_15_25ns)
	{
		//cout << dB() << " " << ipDXY() << endl;
		if(isEB())
		{
			if(Abs(DEtaSCTrk()) > 0.0103){return(false);}
			if(Abs(DPhiSCTrk()) > 0.0336){return(false);}
			if(full5x5_sigmaIEtaIEta() > 0.0101){return(false);}
			if(hadronicOverEM() > 0.0876){return(false);}
			if(Abs(dB()) > 0.0118){return(false);}
			if(Abs(dz()) > 0.373){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.0174){return(false);}
			if(USEISO && CorPFIsolation2015_25ns() > 0.0766){return(false);}
			//if(chargedIso()/Pt() > 0.05){return(false);}
			if(!passConversionVeto()){return(false);}
			if(nMissingInnerHits() > 2){return(false);}
			return(true);
		}
		if(isEE())
		{
			if(Abs(DEtaSCTrk()) > 0.00733){return(false);}
			if(Abs(DPhiSCTrk()) > 0.114){return(false);}
			if(full5x5_sigmaIEtaIEta() > 0.0283){return(false);}
			if(hadronicOverEM() > 0.0678){return(false);}
			if(Abs(dB()) > 0.0739){return(false);}
			if(Abs(dz()) > 0.602){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.0898){return(false);}
			if(USEISO && CorPFIsolation2015_25ns() > 0.0678){return(false);}
			//if(chargedIso()/Pt() > 0.05){return(false);}
			if(!passConversionVeto()){return(false);}
			if(nMissingInnerHits() > 1){return(false);}
			return(true);
		}
	}
	else if(idtyp == LOOSE_15_25ns)
	{
		if(isEB())
		{
			if(Abs(DEtaSCTrk()) > 0.0105){return(false);}
			if(Abs(DPhiSCTrk()) > 0.115){return(false);}
			if(sigmaIEtaIEta() > 0.0103){return(false);}
			if(hadronicOverEM() > 0.104){return(false);}
			if(Abs(dB()) > 0.0261){return(false);}
			if(Abs(dz()) > 0.41){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.102){return(false);}
			if(USEISO && CorPFIsolation2015_25ns() > 0.0893){return(false);}
			//if(chargedIso()/Pt() > 0.1){return(false);}
			if(!passConversionVeto()){return(false);}
			if(nMissingInnerHits() > 2){return(false);}
			return(true);
		}
		if(isEE())
		{
			if(Abs(DEtaSCTrk()) > 0.00814){return(false);}
			if(Abs(DPhiSCTrk()) > 0.182){return(false);}
			if(sigmaIEtaIEta() > 0.0301){return(false);}
			if(hadronicOverEM() > 0.0897){return(false);}
			if(Abs(dB()) > 0.118){return(false);}
			if(Abs(dz()) > 0.822){return(false);}
			if(Abs((1. - ESCOverETrack())/energy()) > 0.126){return(false);}
			if(USEISO && CorPFIsolation2015_25ns() > 0.121){return(false);}
			//if(chargedIso()/Pt() > 0.1){return(false);}
			if(!passConversionVeto()){return(false);}
			if(nMissingInnerHits() > 1){return(false);}
			return(true);
		}
	}
	return(false);
}

URStreamer* IDElectron::streamer = 0;
bool IDElectron::USEISO = true;

