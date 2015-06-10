#include "helper.h"
#include <iostream>
#include <TMath.h>

using namespace TMath;

string FNbody(string filename)
{
	string file(FNfilename(filename));
	return file.substr(0, file.find_first_of("."));
}

string FNfilename(string filename)
{
	return filename.substr(filename.find_last_of("/")+1);
}

string FNpath(string filename)
{
	return filename.substr(0, filename.find_last_of("/"));
}

string FNnoext(string filename)
{
	return filename.substr(0, filename.find_first_of("."));
}

double ProbToSigma(double prob)
{
	return(TMath::Sqrt(2.) * TMath::ErfInverse(prob));
}

double SigmaToProb(double sigma)
{
	return(TMath::Erf(sigma/TMath::Sqrt(2.)));
}

TH1DCollection::TH1DCollection(string _prefix) : prefix(_prefix)
{
	if(prefix.size() != 0) {prefix += "_";}
}

TH1D* TH1DCollection::AddHist(string name, Int_t bins, Double_t min, Double_t max, string xlabel, string ylabel)
{
	hmap[name] = new TH1D((prefix+name).c_str(), (prefix+name).c_str(), bins, min, max);
	hmap[name]->Sumw2();
	hmap[name]->GetXaxis()->SetTitle(xlabel.c_str());
	hmap[name]->GetYaxis()->SetTitle(ylabel.c_str());
	return hmap[name];
}

TH1D* TH1DCollection::AddHist(string name, const vector<Double_t>& bins, string xlabel, string ylabel)
{
	hmap[name] = new TH1D((prefix+name).c_str(), (prefix+name).c_str(), bins.size()-1, bins.data());
	hmap[name]->Sumw2();
	hmap[name]->GetXaxis()->SetTitle(xlabel.c_str());
	hmap[name]->GetYaxis()->SetTitle(ylabel.c_str());
	return hmap[name];
}

TH1D* TH1DCollection::Get(string name)
{
	map<string, TH1D*>::iterator it_pl = hmap.find(name);
	if(it_pl != hmap.end()) return it_pl->second; 
	cerr << "TH1DCollection: Request for unknown hist: " << prefix << " " << name << endl;
	return 0;
}

TH1D* TH1DCollection::operator[](string name)
{
	return Get(name) ;
}



TH2DCollection::TH2DCollection(string _prefix) : prefix(_prefix) 
{
	if(prefix.size() != 0) {prefix += "_";}
}

TH2D* TH2DCollection::AddHist(string name, Int_t xbins, Double_t xmin, Double_t xmax, Int_t ybins, Double_t ymin, Double_t ymax, string xlabel, string ylabel)
{
	hmap[name] = new TH2D((prefix+name).c_str(), (prefix+name).c_str(), xbins, xmin, xmax, ybins, ymin, ymax);
	hmap[name]->Sumw2();
	hmap[name]->GetXaxis()->SetTitle(xlabel.c_str());
	hmap[name]->GetYaxis()->SetTitle(ylabel.c_str());
	return hmap[name];
}

TH2D* TH2DCollection::Get(string name)
{
	map<string, TH2D*>::iterator it_pl = hmap.find(name);
	if(it_pl != hmap.end()) return it_pl->second; 
	cerr << "TH2DCollection: Request for unknown hist: " << prefix << " " << name << endl;
	return 0;
}
TH2D* TH2DCollection::AddHist(string name, Int_t xbins, Double_t xmin, Double_t xmax, const vector<Double_t>& ybins, string xlabel, string ylabel)
{
    hmap[name] = new TH2D((prefix+name).c_str(), (prefix+name).c_str(), xbins, xmin, xmax, ybins.size()-1, ybins.data());
    hmap[name]->Sumw2();
    hmap[name]->GetXaxis()->SetTitle(xlabel.c_str());
    hmap[name]->GetYaxis()->SetTitle(ylabel.c_str());
    return hmap[name];
}
TH2D* TH2DCollection::AddHist(string name, const vector<Double_t>& xbins, Int_t ybins, Double_t ymin, Double_t ymax, string xlabel, string ylabel)
{
    hmap[name] = new TH2D((prefix+name).c_str(), (prefix+name).c_str(), xbins.size()-1, xbins.data(), ybins, ymin, ymax);
    hmap[name]->Sumw2();
    hmap[name]->GetXaxis()->SetTitle(xlabel.c_str());
    hmap[name]->GetYaxis()->SetTitle(ylabel.c_str());
    return hmap[name];
}
TH2D* TH2DCollection::AddHist(string name, const vector<Double_t>& xbins, const vector<Double_t>& ybins, string xlabel, string ylabel)
{
    hmap[name] = new TH2D((prefix+name).c_str(), (prefix+name).c_str(), xbins.size()-1, xbins.data(), ybins.size()-1, ybins.data());
    hmap[name]->Sumw2();
    hmap[name]->GetXaxis()->SetTitle(xlabel.c_str());
    hmap[name]->GetYaxis()->SetTitle(ylabel.c_str());
    return hmap[name];
}

TH2D* TH2DCollection::operator[](string name)
{
	return Get(name);
}

//sorting

double getThetaStar(TLorentzVector Z1, TLorentzVector Z2)
{
	TLorentzVector fm(Z1 + Z2);
	TVector3 bvec = fm.BoostVector();
	Z1.Boost(-1.*bvec);

	TVector3 z(0., 0., 1.);
	return(z*Z1.Vect()/Z1.Vect().Mag());
}

bool tlvsortpt(const TLorentzVector& JA, const TLorentzVector& JB)
{
        if(JA.Pt() > JB.Pt()) return(true);
        return(false);
}

bool tlvsortetaabs(const TLorentzVector& JA, const TLorentzVector& JB)
{
        if(Abs(JA.Eta()) > Abs(JB.Eta())) return(true);
        return(false);
}

bool ptlvsortpt(const TLorentzVector* JA, const TLorentzVector* JB)
{
        if(JA->Pt() > JB->Pt()) return(true);
        return(false);
}

bool ptlvsortetaabs(const TLorentzVector* JA, const TLorentzVector* JB)
{
        if(Abs(JA->Eta()) > Abs(JB->Eta())) return(true);
        return(false);
}

bool ptlvsorteta(const TLorentzVector* JA, const TLorentzVector* JB)
{
        if(JA->Eta() > JB->Eta()) return(true);
        return(false);
}

