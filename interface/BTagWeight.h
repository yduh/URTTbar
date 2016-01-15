#ifndef HBTAGWEIGHT
#define HBTAGWEIGHT
#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

class ttbar;
class IDJet;

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

		double scalebM(IDJet* jet);
		double scalecM(IDJet* jet);
		double scalelM(IDJet* jet);
		double scalebL(IDJet* jet);
		double scalecL(IDJet* jet);
		double scalelL(IDJet* jet);
		double UnclL(IDJet* jet);
		vector<double> uncbM = {0.031647235155, 0.021615911275, 0.032769639045, 0.024189794436, 0.043655604124, 0.060466367751, 0.064764265418};//MEDIUM
		vector<double> uncbL = {0.0223276, 0.0153304, 0.0244939, 0.0209332, 0.0292196, 0.0395714, 0.0473297};
		vector<double> unccM = {0.063294470310211182, 0.043231822550296783, 0.065539278090000153, 0.04837958887219429, 0.087311208248138428, 0.12093273550271988, 0.129528530836105347};//MEDIUM
		vector<double> unccL = {0.0446552, 0.0306609, 0.0489879, 0.0418664, 0.0584392, 0.0791429, 0.0946595};
		vector<double> unclM = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};//MEDIUM

	public:
		BTagWeight();
		void Init(ttbar* an, string filename, double bunc = 0., double lunc = 0.);
		double SF(vector<IDJet*>& jets);

};

#endif
