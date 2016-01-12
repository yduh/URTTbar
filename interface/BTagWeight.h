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
		TH1D* hbeff;
		TH1D* hborig;
		TH1D* hceff;
		TH1D* hcorig;
		TH1D* hleff;
		TH1D* hlorig;

		double scaleb(IDJet* jet);
		double scalec(IDJet* jet);
		double scalel(IDJet* jet);
		vector<double> uncb = {0.031647235155, 0.021615911275, 0.032769639045, 0.024189794436, 0.043655604124, 0.060466367751, 0.064764265418};
		vector<double> uncc = {0.063294470310211182, 0.043231822550296783, 0.065539278090000153, 0.04837958887219429, 0.087311208248138428, 0.12093273550271988, 0.129528530836105347};
		vector<double> uncl = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};

	public:
		BTagWeight();
		void Init(ttbar* an, string filename, double bunc = 0., double lunc = 0.);
		double SF(vector<IDJet*>& jets);

};

#endif
