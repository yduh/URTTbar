#ifndef HBTAGWEIGHT
#define HBTAGWEIGHT
#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "helper.h"

using namespace std;

class ttbar;
class IDJet;
class TF1;


class BTagReader
{
	private:
		map<Bin, map<Bin, map<Bin, TF1*> > > data;

	public:
		void Init(const string& filename, const string& measurement, const string& type, int jets, int wp);
		double Get(const IDJet& jet);

};

class BTagWeight
{
	private:
		double btyp, ltyp;
		ttbar* AN;
		TFile* probfile;
		TH1D* hbeffM;
		TH1D* hborigM;
		TH1D* hceffM;
		TH1D* hcorigM;
		TH1D* hleffM;
		TH1D* hlorigM;

		TH1D* hbeffL;
		TH1D* hborigL;
		TH1D* hceffL;
		TH1D* hcorigL;
		TH1D* hleffL;
		TH1D* hlorigL;
		
		vector<BTagReader> BL;
		vector<BTagReader> CL;
		vector<BTagReader> LL;
		vector<BTagReader> BM;
		vector<BTagReader> CM;
		vector<BTagReader> LM;

		double scale(vector<BTagReader>& SF, IDJet* jet, double err);

		double scalebM(IDJet* jet, double err = 0.);
		double scalecM(IDJet* jet, double err = 0.);
		double scalelM(IDJet* jet, double err = 0.);
		double scalebL(IDJet* jet, double err = 0.);
		double scalecL(IDJet* jet, double err = 0.);
		double scalelL(IDJet* jet, double err = 0.);

	public:
		BTagWeight();
		void Init(ttbar* an, const string& csvfilename, const string& efffilename, double bunc = 0., double lunc = 0.);
		double SF(vector<IDJet*>& jets);

};

#endif
