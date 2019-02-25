#ifndef RESPONSE2DVAR
#define RESPONSE2DVAR
#include <helper.h>
#include <vector>
#include <string>

using namespace std;

class ttbar;
class TDirectory;

class TTBarResponse2Dvar
{
	protected:
		string prefix_;
		//ttbar* an_;
		TDirectory* dir;
		TH1DCollection plot1d;	
		TH2DCollection plot2d;

		map<string, map<Bin, map<Bin, double> > > binning;
		map<string, bool> ofx_;
		map<string, bool> ofy_;

	public:
		TTBarResponse2Dvar(string prefix);
		~TTBarResponse2Dvar();
		void AddMatrix(string name, const vector<double>& xbins, const vector<vector<double> >& ybins, bool ofx = false, bool ofy = false);
		void FillTruth(string name, double valx, double valy, double weight);
		void FillAll(string name, double valx, double valy, double weight);
		void FillBKG(string name, double valx, double valy, double weight);
		void FillTruthReco(string name, double tvalx, double tvaly, double rvalx, double rvaly, double weight);
		int GetBin(string name, double valx, double valy);
		int GetNBins(string name) {return plot1d[name+"_all"]->GetNbinsX();}
};
#endif
