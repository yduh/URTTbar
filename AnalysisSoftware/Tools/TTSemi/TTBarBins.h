#ifndef RECOBINTTBAR
#define RECOBINTTBAR
#include <helper.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class BinInfo
{
	private:
		double lower;
		double upper;
		TH1D* h;
	public:
		BinInfo(string prefix, int bin, vector<double>& bins);
		void Fill(double m, double weight = 1.);
		bool operator<(double val) const {return(lower < val);}

};

class TTBarBins
{
	private:
		vector< vector<BinInfo> > sig;
		vector< vector<BinInfo> > bkg;
		double min;
		double max;
		string prefix_;
		size_t GetBin(double val);
	public:
		TTBarBins(string prefix);
		void Init(vector<double>& bins);
		void Fill(bool SIGNAL, int njets, double val, double m, double weight = 1.);
};

#endif
