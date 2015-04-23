#ifndef HPDFUNCERTAINTY
#define HPDFUNCERTAINTY
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

#include <TH1D.h>
#include <TH2D.h>

#include <LHAPDF/LHAPDF.h>

using namespace std;
using namespace LHAPDF;

class URStreamer;

class PDFuncertainty
{
	private:
		PDFSet* setorig;
		PDF* pdforig;
		vector<PDFSet*> sets;
		vector< vector<PDF*> > pdfs;
		map< string, vector< vector<TH1D*> > > hist1d;
		vector< vector<double> > weights;
		double oldx1;
		void SetupWeights();

	public:
		static URStreamer* streamer;
		PDFuncertainty(const string setorigname, int memorig, const vector<string>& setnames);
		~PDFuncertainty();
		void Add1dHist(string name, Int_t bins, Double_t min, Double_t max, string xlabel, string ylabel);
		void Add1dHist(string name, const vector<double>& bins, string xlabel, string ylabel);
		void Fill1d(string name, double val, double weight = 1.);
};

#endif
