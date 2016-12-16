#include <IDMuon.h>
#include <TMath.h>
#include <iostream>

using namespace TMath;

IDMuon::IDMuon(const Muon mu) : Muon(mu), MCMatchable()
{
}

double IDMuon::PFIsoDb()
{
	return (pfChargedIso04() + TMath::Max(pfNeutralIso04() + pfPhotonIso04() - 0.5*pfPUIso04(), 0.));
}
	
double IDMuon::CorPFIsolation2015()
{
	double eta = Abs(Eta());
	double effarea = 0.;
	if(eta < 0.8){ effarea = 0.0986;}
	else if(eta < 1.5){ effarea = 0.0765;}
	else if(eta < 2.1){ effarea = 0.0537;}
	else if(eta < 2.4){ effarea = 0.0812;}
	//if(eta < 0.8){ effarea = 0.0913;}
	//else if(eta < 1.3){ effarea = 0.0765;}
	//else if(eta < 2.0){ effarea = 0.0546;}
	//else if(eta < 2.2){ effarea = 0.0728;}
	//else if(eta < 2.5){ effarea = 0.1177;}

	if(streamer != 0)
	{
		effarea *= Max(streamer->rho().value(), 0.);
	}
	else {effarea = 0; cerr << "initialize IDElectron::stream = URStreamer object for rho correction" << endl;}
	return(pfChargedIso04() + Max(pfNeutralIso04() + pfPhotonIso04() - effarea, 0.))/Pt();
}

bool IDMuon::ID(IDS idtyp)
{
//cout << "muinfo " << streamer->run << ":" << streamer->lumi << ":" << streamer->evt << " " << Pt() << " " << Eta() <<" " <<  isPF() << " " << isGlobal() << " " << pixelHits() << " " << trackerLayers() << " " << validHits() << " " << numMatchedStations() << " " << TMath::Abs(dB()) << " " << TMath::Abs(dz()) << " " << chi2()/ndof() << " " << PFIsoDb()/Pt() << endl;

	if(idtyp == TIGHT_16)
	{
		if(TMath::Abs(Eta()) > 2.4) return(false);
		if(!isPF()) return(false);
		if(!isGlobal()) return(false);
		if(pixelHits() <= 0) return(false);
		if(trackerLayers() <= 5) return(false);
		if(validHits() <= 0) return(false);
		if(numMatchedStations() <= 1) return(false);
		if(abs(dB()) > 0.2) return(false);
		if(abs(dz()) > 0.5) return(false);
		if(chi2()/ndof() > 10.) return(false);
		if(PFIsoDb()/Pt() > 0.15) return(false);
		return(true);
	}
	else if(idtyp == LOOSE_16)
	{
		if(TMath::Abs(Eta()) > 2.4) return(false);
		if(!isPF()) return(false);
		if(!isGlobal() && !isTracker()) return(false);
		if(PFIsoDb()/Pt() > 0.25) return(false);
		return(true);
	}
	return(false);
}

URStreamer* IDMuon::streamer = 0;
bool IDMuon::USEISO = true;
