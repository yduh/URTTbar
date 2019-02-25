#include "PDFuncertainty.h"
#include "TDirectory.h"
#include "URStreamer.h"

#include <sstream>

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
	newdir->cd();
	histdir[name] = newdir->mkdir("Iweights");
	histdir[name]->cd();
	Whist1d[name].push_back(new TH1D("weight_1", "weight_1", bins, min, max));	
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
	newdir->cd();
	histdir[name] = newdir->mkdir("Iweights");
	histdir[name]->cd();
	Whist1d[name].push_back(new TH1D("weight_1", "weight_1", bins.size()-1, bins.data()));	
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

	const vector<Mcweight>& ws =  streamer->MCWeights();
	if(Whist1d[name].size() != ws.size())
	{
		cout << "W_ERROR: " << Whist1d[name].size() << " " << ws.size() << endl;
	}
	if(Whist1d[name].size() != ws.size() && Whist1d[name].size() < 2)
	{
		TDirectory* olddir = gDirectory;
		histdir[name]->cd();
		Whist1d[name].reserve(ws.size());
		for(size_t h = 2 ; h <= ws.size() ; ++h)
		{
			stringstream hname;
			hname  << "weight_" << h;
			Whist1d[name].push_back(new TH1D(*Whist1d[name][0]));
			Whist1d[name].back()->SetName(hname.str().c_str());
			Whist1d[name].back()->SetTitle(hname.str().c_str());
		}
		olddir->cd();
	}

	for(size_t h = 0 ; h < ws.size() ; ++h)
	{
		Whist1d[name][h]->Fill(val, weight*ws[h].weights()/ws[0].weights());//IF you are using the NNPDF
		//Whist1d[name][h]->Fill(val, weight*(ws[h].weights()/ws[0].weights())/ (ws[112].weights()/abs(ws[0].weights())) );
		//Besides, 112 is only for the tt PDF unc, if you need it for STt and WnJets, that's different weights
	}

}

URStreamer* PDFuncertainty::streamer = 0;
