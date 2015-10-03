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
		ttbar* AN;
		TFile* probfile;
		TH1D* hbpass;
		TH1D* hball;
		TH1D* hlpass;
		TH1D* hlall;

		//double scale = 0.965254;
		//vector<double> uncl = {0.16037511825561523, 0.14245779812335968, 0.10478173941373825, 0.1455094963312149, 0.23630200326442719, 0.22251765429973602, 0.34035113453865051};
		//vector<double> uncb = {0.080187559127807617, 0.07122889906167984, 0.052390869706869125, 0.072754748165607452, 0.11815100163221359, 0.11125882714986801, 0.17017556726932526};	

		double scale = 0.968546;

		vector<double> uncl = {0.093810759484767914, 0.10516738146543503, 0.095603741705417633, 0.11572366207838058, 0.19687847793102264, 0.16733628511428833, 0.25004029273986816};

		vector<double> uncb = {0.046905379742383957, 0.052583690732717514, 0.047801870852708817, 0.057861831039190292, 0.098439238965511322, 0.083668142557144165, 0.12502014636993408};

	public:
		BTagWeight();
		void Init(ttbar* an, string filename);
		double SF(vector<IDJet*>& jets, int btyp = 0, int ltyp = 0);

};

#endif
