#ifndef HELPER_H
#define HELPER_H
#include <string>
#include <map>
#include <vector>

#include "TH1D.h"
#include "TH2D.h"
#include "TLorentzVector.h"
#include "TMath.h"

using namespace std;

//Path and Filenames
string FNbody(string filename);
string FNfilename(string filename);
string FNpath(string filename);
string FNnoext(string filename);

//Stat
double ProbToSigma(double prob);
double SigmaToProb(double sigma);

//Histgramm Collections

class TH1DCollection
{
	private:
		string prefix;
		map<string, TH1D*> hmap;
	public:
		TH1DCollection(string _prefix);
		TH1D* AddHist(string name, Int_t bins, Double_t min, Double_t max, string xlabel, string ylabel);
		TH1D* AddHist(string name, vector<Double_t>& bins, string xlabel, string ylabel);

		TH1D* Get(string name);
		TH1D* operator[](string name);
};

class TH2DCollection
{
	private:
		string prefix;
		map<string, TH2D*> hmap;
	public:
		TH2DCollection(string _prefix);
		TH2D* AddHist(string name, Int_t xbins, Double_t xmin, Double_t xmax, Int_t ybins, Double_t ymin, Double_t ymax, string xlabel, string ylabel);
		TH2D* AddHist(string name, Int_t xbins, Double_t xmin, Double_t xmax, const vector<Double_t>& ybins, string xlabel, string ylabel);
		TH2D* AddHist(string name, const vector<Double_t>& xbins, Int_t ybins, Double_t ymin, Double_t ymax, string xlabel, string ylabel);
		TH2D* AddHist(string name, const vector<Double_t>& xbins, const vector<Double_t>& ybins, string xlabel, string ylabel);

		TH2D* Get(string name);
		TH2D* operator[](string name);
};

//Sort
double getThetaStar(TLorentzVector Z1, TLorentzVector Z2);

bool tlvsortpt(const TLorentzVector& JA, const TLorentzVector& JB);
bool tlvsortetaabs(const TLorentzVector& JA, const TLorentzVector& JB);

bool ptlvsortpt(const TLorentzVector* JA, const TLorentzVector* JB);
bool ptlvsortetaabs(const TLorentzVector* JA, const TLorentzVector* JB);
bool ptlvsorteta(const TLorentzVector* JA, const TLorentzVector* JB);

#endif
