#ifndef RESPONSE2D
#define RESPONSE2D
#include <helper.h>
#include <vector>
#include <string>

using namespace std;

class ttbar;
class TDirectory;

class TTBarResponse2D
{
	protected:
		string prefix_;
		//ttbar* an_;
		TDirectory* dir;
		TH1DCollection plot1d;	
		TH2DCollection plot2d;	

	public:
		TTBarResponse2D(string prefix);
		~TTBarResponse2D();
		void AddMatrix(string name, const vector<double>& Mbinsx, const vector<double>& Mbinsy, const vector<double>& Tbinsx, const vector<double>& Tbinsy);
		void FillTruth(string name, double valx, double valy, double weight);
		void FillAll(string name, double valx, double valy, double weight);
		void FillBKG(string name, double valx, double valy, double weight);
		void FillTruthReco(string name, double tvalx, double tvaly, double rvalx, double rvaly, double weight);
		int GetBin(string name, double valx, double valy, bool RECO = false);
		int GetNBins(string name, bool RECO = false) {return RECO ? plot1d[name+"_all"]->GetNbinsX() : plot1d[name+"_truth"]->GetNbinsX();}
};
#endif
