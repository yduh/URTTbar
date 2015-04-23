#include "PDFuncertainty.h"
#include "TDirectory.h"
#include "URStreamer.h"



PDFuncertainty::PDFuncertainty(const string setorigname, int memorig, const vector<string>& setnames) : oldx1(0.)
{
	setorig = new PDFSet(setorigname);
	pdforig = setorig->mkPDF(memorig);
	for(const string& setname : setnames)
	{
		sets.push_back(new PDFSet(setname));
		pdfs.push_back(sets.back()->mkPDFs());
		weights.push_back(vector<double>(pdfs.back().size()));
	}
}

PDFuncertainty::~PDFuncertainty() 
{

}

void PDFuncertainty::Add1dHist(string name, Int_t bins, Double_t min, Double_t max, string xlabel, string ylabel)
{
	hist1d[name].resize(sets.size()); 		
	TDirectory* olddir = gDirectory;
	TDirectory* newdir = olddir->mkdir(name.c_str());
	for(size_t s = 0 ; s < sets.size() ; ++s)
	{
		newdir->cd();
		TDirectory* setdir = newdir->mkdir((sets[s]->name()).c_str());
		setdir->cd();
		for(size_t p = 0 ; p < pdfs[s].size() ; ++p)
		{
			stringstream hname;
			hname  <<  "pdf_" << p;
			hist1d[name][s].push_back(new TH1D(hname.str().c_str(), hname.str().c_str(), bins, min, max));
			hist1d[name][s][p]->Sumw2();
			hist1d[name][s][p]->GetXaxis()->SetTitle(xlabel.c_str());
			hist1d[name][s][p]->GetYaxis()->SetTitle(ylabel.c_str());
		}
	}
	olddir->cd();
}

void PDFuncertainty::Add1dHist(string name, const vector<double>& bins, string xlabel, string ylabel)
{
	hist1d[name].resize(sets.size()); 		
	TDirectory* olddir = gDirectory;
	TDirectory* newdir = olddir->mkdir(name.c_str());
	for(size_t s = 0 ; s < sets.size() ; ++s)
	{
		newdir->cd();
		TDirectory* setdir = newdir->mkdir((sets[s]->name()).c_str());
		setdir->cd();
		for(size_t p = 0 ; p < pdfs[s].size() ; ++p)
		{
			stringstream hname;
			hname << "pdf_" << p;
			hist1d[name][s].push_back(new TH1D(hname.str().c_str(), hname.str().c_str(), bins.size()-1, bins.data()));
			hist1d[name][s][p]->Sumw2();
			hist1d[name][s][p]->GetXaxis()->SetTitle(xlabel.c_str());
			hist1d[name][s][p]->GetYaxis()->SetTitle(ylabel.c_str());
		}
	}
	olddir->cd();
}

void PDFuncertainty::SetupWeights()
{
	const Geninfo& info = streamer->genInfo();
	double x1 = info.x1();	
	if(oldx1 == x1) return;
	oldx1 = x1;
	double x2 = info.x2();
	double Q = info.renScale();
	int id1 = info.pdfid1();	
	int id2 = info.pdfid2();	
	for(size_t s = 0 ; s < sets.size() ; ++s)
	{
		for(size_t p = 0 ; p < pdfs[s].size() ; ++p)
		{
			weights[s][p] = pdfs[s][p]->xfxQ(id1,x1,Q)/pdforig->xfxQ(id1,x1,Q) * pdfs[s][p]->xfxQ(id2,x2,Q)/pdforig->xfxQ(id2,x2,Q);
		}
	}
}

void PDFuncertainty::Fill1d(string name, double val, double weight)
{
	SetupWeights();
	for(size_t s = 0 ; s < sets.size() ; ++s)
	{
		for(size_t p = 0 ; p < pdfs[s].size() ; ++p)
		{
			hist1d[name][s][p]->Fill(val, weight*weights[s][p]);
		}
	}
}

URStreamer* PDFuncertainty::streamer = 0;
