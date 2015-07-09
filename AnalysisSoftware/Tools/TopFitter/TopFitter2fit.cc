#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>

#include "helper.h"
#include "ConfigParser.h"

#include "TTree.h"
#include "TVector3.h"
#include "TFile.h"
#include "TH1D.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooPoisson.h"
#include "RooBreitWigner.h"
#include "RooVoigtian.h"
#include "RooChebychev.h"
#include "RooPolynomial.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "RooCBShape.h"
#include "RooLandau.h"
#include "RooFFTConvPdf.h"
#include "RooKeysPdf.h"
#include "RooHistPdf.h"
#include "RooExtendPdf.h"
#include "RooExponential.h"
#include "RooCFunction1Binding.h"
#include "RooCFunction3Binding.h"
#include "RooCFunction4Binding.h"
#include "RooTFnBinding.h"
#include "RooGlobalFunc.h"
#include "RooFitResult.h"
#include "RooNLLVar.h"
#include "RooAddition.h"
#include "RooMinuit.h"



using namespace std;
using namespace TMath;
using namespace RooFit;

void cleanzero(TH2D* hist)
{
	for(int x = 1 ; x <= hist->GetNbinsX() ; ++x)
	{
		for(int y = 1 ; y <= hist->GetNbinsY() ; ++y)
		{
			if(hist->GetBinContent(x, y) < 0.) hist->SetBinContent(x, y, 0.);
		}
	}
}

void commontemplate(TH2D* hist)
{
	TH1D* prox = hist->ProjectionX();
	if(prox->Integral() > 0.)
	{	
		prox->Scale(1./prox->Integral());
	}

	TH1D* proy = hist->ProjectionY();

	for(int x = 1 ; x <= hist->GetNbinsX() ; ++x)
	{
		for(int y = 1 ; y <= hist->GetNbinsY() ; ++y)
		{
			hist->SetBinContent(x, y, Max(prox->GetBinContent(x)*proy->GetBinContent(y), 0.));
		}
	}
}


int main(int argc, char** argv)
{
	string histtyp(argv[1]);
	string histname(argv[2]);
	ConfigParser CP("TopFitter.cfg");
	ConfigParser CPscale("SCALE.cfg");
	double lumi = CPscale.Get<double>("lumi"); 
	double TTscale = lumi*CPscale.Get<double>("tt_xsec")/CPscale.Get<double>(FNbody(CP.Get<string>("TEM_tt"))+"_W");
	double Wscale = lumi*CPscale.Get<double>("WJets_xsec")/CPscale.Get<double>("WJets_W");
	double Zscale = lumi*CPscale.Get<double>("DYJets_xsec")/CPscale.Get<double>("DYJets_W");
	double Wtscale = lumi*CPscale.Get<double>("Wt_xsec")/CPscale.Get<double>("Wt_W");
	double Wtbarscale = lumi*CPscale.Get<double>("Wtbar_xsec")/CPscale.Get<double>("Wtbar_W");
	double STtscale = lumi*CPscale.Get<double>("STt_xsec")/CPscale.Get<double>("STt_W");
	int nexp = CP.Get<int>("nexp");
	int interpolate = 0;
	TCanvas* can = new TCanvas("can", "can", 2000, 1600);
	can->SetRightMargin(0.05);
	can->SetTopMargin(0.05);

	TFile* infile = TFile::Open(CP.Get<string>("TEM_tt").c_str());
	TFile* Wfile = TFile::Open(CP.Get<string>("TEM_WJets").c_str());
	TFile* DYfile = TFile::Open(CP.Get<string>("TEM_DYJets").c_str());
	TFile* Tfile = TFile::Open(CP.Get<string>("TEM_Wt").c_str());
	TFile* Tbarfile = TFile::Open(CP.Get<string>("TEM_Wtbar").c_str());
	TFile* STtfile = TFile::Open(CP.Get<string>("TEM_STt").c_str());


	//string histname("ptbmedium_pass");

	//string histname("ptthad");
	TH2D* h2d_test_right = static_cast<TH2D*>(infile->Get(("TRUTH/"+histtyp+"_right_test_" + histname).c_str()));
	cleanzero(h2d_test_right);
	h2d_test_right->Scale(TTscale);
	TH2D* h2d_test_wrong = static_cast<TH2D*>(infile->Get(("TRUTH/"+histtyp+"_wrong_test_" + histname).c_str()));
	cleanzero(h2d_test_wrong);
	h2d_test_wrong->Scale(TTscale);

	TH2D* h2d_test_bkg = static_cast<TH2D*>(Wfile->Get(("TRUTH/"+histtyp+"_wrong_test_" + histname).c_str()));
	commontemplate(h2d_test_bkg);
	TH2D* h2d_test_bkgDY = static_cast<TH2D*>(DYfile->Get(("TRUTH/"+histtyp+"_wrong_test_" + histname).c_str()));
	commontemplate(h2d_test_bkgDY);
	TH2D* h2d_test_bkgT = static_cast<TH2D*>(Tfile->Get(("TRUTH/"+histtyp+"_wrong_test_" + histname).c_str()));
	cleanzero(h2d_test_bkgT);
	TH2D* h2d_test_bkgTbar = static_cast<TH2D*>(Tbarfile->Get(("TRUTH/"+histtyp+"_wrong_test_" + histname).c_str()));
	cleanzero(h2d_test_bkgTbar);
	TH2D* h2d_test_bkgSTt = static_cast<TH2D*>(STtfile->Get(("TRUTH/"+histtyp+"_wrong_test_" + histname).c_str()));
	commontemplate(h2d_test_bkgSTt);
	h2d_test_bkg->Scale(Wscale);
	h2d_test_bkgDY->Scale(Zscale);
	h2d_test_bkgT->Scale(Wtscale);
	h2d_test_bkgTbar->Scale(Wtbarscale);
	h2d_test_bkgSTt->Scale(STtscale);
	h2d_test_bkg->Add(h2d_test_bkgDY);	
	h2d_test_bkg->Add(h2d_test_bkgT);	
	h2d_test_bkg->Add(h2d_test_bkgTbar);	
	h2d_test_bkg->Add(h2d_test_bkgSTt);	

	TH2D* h2d_testb_right = static_cast<TH2D*>(infile->Get(("TRUTH/"+histtyp+"_right_testb_" + histname).c_str()));
	cleanzero(h2d_testb_right);
	h2d_testb_right->Scale(TTscale);
	TH2D* h2d_testb_wrong = static_cast<TH2D*>(infile->Get(("TRUTH/"+histtyp+"_wrong_testb_" + histname).c_str()));
	cleanzero(h2d_testb_wrong);
	h2d_testb_wrong->Scale(TTscale);

	TH2D* h2d_testb_bkg = static_cast<TH2D*>(Wfile->Get(("TRUTH/"+histtyp+"_wrong_testb_" + histname).c_str()));
	commontemplate(h2d_testb_bkg);
	TH2D* h2d_testb_bkgDY = static_cast<TH2D*>(DYfile->Get(("TRUTH/"+histtyp+"_wrong_testb_" + histname).c_str()));
	commontemplate(h2d_testb_bkgDY);
	TH2D* h2d_testb_bkgT = static_cast<TH2D*>(Tfile->Get(("TRUTH/"+histtyp+"_wrong_testb_" + histname).c_str()));
	cleanzero(h2d_testb_bkgT);
	TH2D* h2d_testb_bkgTbar = static_cast<TH2D*>(Tbarfile->Get(("TRUTH/"+histtyp+"_wrong_testb_" + histname).c_str()));
	cleanzero(h2d_testb_bkgTbar);
	TH2D* h2d_testb_bkgSTt = static_cast<TH2D*>(STtfile->Get(("TRUTH/"+histtyp+"_wrong_testb_" + histname).c_str()));
	commontemplate(h2d_testb_bkgSTt);
	h2d_testb_bkg->Scale(Wscale);
	h2d_testb_bkgDY->Scale(Zscale);
	h2d_testb_bkgT->Scale(Wtscale);
	h2d_testb_bkgTbar->Scale(Wtbarscale);
	h2d_testb_bkgSTt->Scale(STtscale);
	h2d_testb_bkg->Add(h2d_testb_bkgDY);	
	h2d_testb_bkg->Add(h2d_testb_bkgT);	
	h2d_testb_bkg->Add(h2d_testb_bkgTbar);	
	h2d_testb_bkg->Add(h2d_testb_bkgSTt);	


	string tttestfile(CP.Get<string>("DATASIM_tt"));
	double TTscale2 = lumi*CPscale.Get<double>("tt_xsec")/CPscale.Get<double>(FNbody(tttestfile)+"_W");
	TFile* infile_data = TFile::Open(tttestfile.c_str());
	TFile* Wfile_data = TFile::Open(CP.Get<string>("DATASIM_WJets").c_str());
	TFile* DYfile_data = TFile::Open(CP.Get<string>("DATASIM_DYJets").c_str());
	TFile* Tfile_data = TFile::Open(CP.Get<string>("DATASIM_Wt").c_str());
	TFile* Tbarfile_data = TFile::Open(CP.Get<string>("DATASIM_Wtbar").c_str());
	TFile* STtfile_data = TFile::Open(CP.Get<string>("DATASIM_STt").c_str());

	TH2D* h2d_test_data_truth = static_cast<TH2D*>(infile_data->Get(("TRUTH/"+histtyp+"_right_test_" + histname).c_str()));
	cleanzero(h2d_test_data_truth);
	TH1D* data_truth = h2d_test_data_truth->ProjectionY("datatruth");
	data_truth->Scale(TTscale2);
	TH2D* h2d_test_data_all = 0;
	TH2D* h2d_testb_data_all = 0;
	//realdata
	bool isDATA = CP.Get<bool>("ISDATA");
	if(isDATA)
	{
		TFile* infile_realdata = TFile::Open(CP.Get<string>("DATAFILE").c_str());
		h2d_test_data_all = static_cast<TH2D*>(infile_realdata->Get(("RECO/all_test_" + histname).c_str()));
		cleanzero(h2d_test_data_all);
		h2d_testb_data_all = static_cast<TH2D*>(infile_realdata->Get(("RECO/all_testb_" + histname).c_str()));
		cleanzero(h2d_testb_data_all);
	}
	else
	{
		h2d_test_data_all = static_cast<TH2D*>(infile_data->Get(("RECO/all_test_" + histname).c_str()));
		cleanzero(h2d_test_data_all);
		TH2D* h2d_test_data_bkgW = static_cast<TH2D*>(Wfile_data->Get(("RECO/all_test_" + histname).c_str()));
		commontemplate(h2d_test_data_bkgW);
		TH2D* h2d_test_data_bkgDY = static_cast<TH2D*>(DYfile_data->Get(("RECO/all_test_" + histname).c_str()));
		commontemplate(h2d_test_data_bkgDY);
		TH2D* h2d_test_data_bkgT = static_cast<TH2D*>(Tfile_data->Get(("RECO/all_test_" + histname).c_str()));
		cleanzero(h2d_test_data_bkgT);
		TH2D* h2d_test_data_bkgTbar = static_cast<TH2D*>(Tbarfile_data->Get(("RECO/all_test_" + histname).c_str()));
		cleanzero(h2d_test_data_bkgTbar);
		TH2D* h2d_test_data_bkgSTt = static_cast<TH2D*>(STtfile_data->Get(("RECO/all_test_" + histname).c_str()));
		commontemplate(h2d_test_data_bkgSTt);
		h2d_test_data_all->Scale(TTscale2);
		h2d_test_data_bkgW->Scale(Wscale);
		h2d_test_data_bkgDY->Scale(Zscale);
		h2d_test_data_bkgT->Scale(Wtscale);
		h2d_test_data_bkgTbar->Scale(Wtbarscale);
		h2d_test_data_bkgSTt->Scale(STtscale);
		h2d_test_data_all->Add(h2d_test_data_bkgW);	
		h2d_test_data_all->Add(h2d_test_data_bkgDY);	
		h2d_test_data_all->Add(h2d_test_data_bkgT);	
		h2d_test_data_all->Add(h2d_test_data_bkgTbar);	
		h2d_test_data_all->Add(h2d_test_data_bkgSTt);	

		h2d_testb_data_all = static_cast<TH2D*>(infile_data->Get(("RECO/all_testb_" + histname).c_str()));
		cleanzero(h2d_testb_data_all);
		TH2D* h2d_testb_data_bkgW = static_cast<TH2D*>(Wfile_data->Get(("RECO/all_testb_" + histname).c_str()));
		commontemplate(h2d_testb_data_bkgW);
		TH2D* h2d_testb_data_bkgDY = static_cast<TH2D*>(DYfile_data->Get(("RECO/all_testb_" + histname).c_str()));
		commontemplate(h2d_testb_data_bkgDY);
		TH2D* h2d_testb_data_bkgT = static_cast<TH2D*>(Tfile_data->Get(("RECO/all_testb_" + histname).c_str()));
		cleanzero(h2d_testb_data_bkgT);
		TH2D* h2d_testb_data_bkgTbar = static_cast<TH2D*>(Tbarfile_data->Get(("RECO/all_testb_" + histname).c_str()));
		cleanzero(h2d_testb_data_bkgTbar);
		TH2D* h2d_testb_data_bkgSTt = static_cast<TH2D*>(STtfile_data->Get(("RECO/all_testb_" + histname).c_str()));
		commontemplate(h2d_testb_data_bkgSTt);
		h2d_testb_data_all->Scale(TTscale2);
		h2d_testb_data_bkgW->Scale(Wscale);
		h2d_testb_data_bkgDY->Scale(Zscale);
		h2d_testb_data_bkgT->Scale(Wtscale);
		h2d_testb_data_bkgTbar->Scale(Wtbarscale);
		h2d_testb_data_bkgSTt->Scale(STtscale);
		h2d_testb_data_all->Add(h2d_testb_data_bkgW);	
		h2d_testb_data_all->Add(h2d_testb_data_bkgDY);	
		h2d_testb_data_all->Add(h2d_testb_data_bkgT);	
		h2d_testb_data_all->Add(h2d_testb_data_bkgTbar);	
		h2d_testb_data_all->Add(h2d_testb_data_bkgSTt);	

	}
	h2d_test_data_all->SetName("data2d_test");
	h2d_testb_data_all->SetName("data2d_testb");

	TRandom3 rand;

	TFile* outfile = new TFile(("fit_"+histtyp+"_"+histname+".root").c_str(), "recreate");

	data_truth->Write();
	TH1D* htrue = h2d_test_right->ProjectionY("truth");
	TH1D* hfitsum = new TH1D(*htrue);
	hfitsum->SetName("fitsum");
	hfitsum->Reset();
	TH1D* hfitsumq = new TH1D(*htrue);
	hfitsumq->SetName("fitsumq");
	hfitsumq->Reset();
	TH1D* hdata = h2d_test_right->ProjectionX("datatest");
	hdata->Reset();

	RooRealVar x("x","x", 0, h2d_test_data_all->GetXaxis()->GetXmin(), h2d_test_data_all->GetXaxis()->GetXmax());
	x.setBins(h2d_test_data_all->GetNbinsX());
	RooRealVar y("y","y", 0, h2d_testb_data_all->GetXaxis()->GetXmin(), h2d_testb_data_all->GetXaxis()->GetXmax());
	y.setBins(h2d_testb_data_all->GetNbinsX());

	for(int i = 1 ; i <= htrue->GetNbinsX() ; ++i)
	{
		stringstream ss;
		ss << i;
		TH1D* hL= new TH1D(("L_"+ss.str()).c_str(), ("L_"+ss.str()).c_str(), 10000, -100000, 0);
		TH1D* hLMC= new TH1D(("LMC_"+ss.str()).c_str(), ("LMC_"+ss.str()).c_str(), 10000, -100000, 0);
		TH1D* hdataorig_test = h2d_test_data_all->ProjectionX(("dataorig_test_" +ss.str()).c_str(), i, i);
		double Ndata = hdataorig_test->Integral();
		if(Ndata == 0) continue;
		RooDataHist dataorig_test("histdataorig_test", "histdataorig_test", x, hdataorig_test);
		RooHistPdf pdf_dataorig_test("pdf_dataorig_test", "pdf_dataorig_test", x, dataorig_test, interpolate);
		TH1D* hdataorig_testb = h2d_testb_data_all->ProjectionX(("dataorig_testb_" +ss.str()).c_str(), i, i);
		RooDataHist dataorig_testb("histdataorig_testb", "histdataorig_testb", y, hdataorig_testb);
		RooHistPdf pdf_dataorig_testb("pdf_dataorig_testb", "pdf_dataorig_testb", y, dataorig_testb, interpolate);

		TH1D* hright_test = h2d_test_right->ProjectionX(("right_test_" +ss.str()).c_str(), i, i);
		TH1D* hwrong_test = h2d_test_wrong->ProjectionX(("wrong_test_" +ss.str()).c_str(), i, i);
		TH1D* hbkg_test = h2d_test_bkg->ProjectionX(("bkg_test_" +ss.str()).c_str(), i, i);
		hbkg_test->Rebin(4);

		RooDataHist right_test("histright_test", "histright_test", x, hright_test);
		RooDataHist wrong_test("histwrong_test", "histwrong_test", x, hwrong_test);
		RooDataHist bkg_test("histbkg_test", "histbkg_test", x, hbkg_test);
		RooHistPdf pdf_right_test("pdf_right_test", "pdf_right_test", x, right_test, interpolate);
		RooHistPdf pdf_wrong_test("pdf_wrong_test", "pdf_wrong_test", x, wrong_test, interpolate);
		RooHistPdf pdf_bkg_test("pdf_bkg_test", "pdf_bkg_test", x, bkg_test, interpolate);

		TH1D* hright_testb = h2d_testb_right->ProjectionX(("right_testb_" +ss.str()).c_str(), i, i);
		TH1D* hwrong_testb = h2d_testb_wrong->ProjectionX(("wrong_testb_" +ss.str()).c_str(), i, i);
		TH1D* hbkg_testb = h2d_testb_bkg->ProjectionX(("bkg_testb_" +ss.str()).c_str(), i, i);

		RooDataHist right_testb("histright_testb", "histright_testb", y, hright_testb);
		RooDataHist wrong_testb("histwrong_testb", "histwrong_testb", y, hwrong_testb);
		RooDataHist bkg_testb("histbkg_testb", "histbkg_testb", y, hbkg_testb);
		RooHistPdf pdf_right_testb("pdf_right_testb", "pdf_right_testb", y, right_testb, interpolate);
		RooHistPdf pdf_wrong_testb("pdf_wrong_testb", "pdf_wrong_testb", y, wrong_testb, interpolate);
		RooHistPdf pdf_bkg_testb("pdf_bkg_testb", "pdf_bkg_testb", y, bkg_testb, interpolate);

		//RooRealVar nright("nright", "nright", Ndata*0.3, 0., 2.*Ndata);
		//RooRealVar nwrong("nwrong", "nwrong", Ndata*0.5, 0., 2.*Ndata);
		//RooRealVar nbkg("nbkg", "nbkg", Ndata*0.2, 0., 2.*Ndata);
		//RooPolynomial constant("const", "const", x, RooArgList());
		//RooRealVar small("small", "small", 0.00001);

		//RooAddPdf model_test("model_test", "model_test", RooArgList(constant, pdf_right_test, pdf_wrong_test, pdf_bkg_test), RooArgList(small, nright, nwrong, nbkg));
		//RooAddPdf model_testb("model_testb", "model_testb", RooArgList(constant, pdf_right_testb, pdf_wrong_testb, pdf_bkg_testb), RooArgList(small, nright, nwrong, nbkg));
		//double nexpright = hright_test->Integral(); 
		//double nexpwrong = hwrong_test->Integral(); 
		//double nexpbkg = hbkg_test->Integral(); 
		//double nexpected = nexpright + nexpwrong + nexpbkg;
		double nexpbkg = hbkg_test->Integral(); 
		RooRealVar nright("nright", "nright", 0.3*Ndata, 0., 2.*Ndata);
		RooRealVar nwrong("nwrong", "nwrong", 0.3*Ndata, 0., 2.*Ndata);
		RooRealVar nbkg("nbkg", "nbkg", nexpbkg, 0., 2.*Ndata);
		RooPolynomial constant_test("const_test", "const_test", x, RooArgList());
		RooPolynomial constant_testb("const_testb", "const_testb", y, RooArgList());
		RooRealVar small("small", "small", 0.00001);

		RooAddPdf model_test("model_test", "model_test", RooArgList(constant_test, pdf_right_test, pdf_wrong_test, pdf_bkg_test), RooArgList(small, nright, nwrong, nbkg));
		RooAddPdf model_testb("model_testb", "model_testb", RooArgList(constant_testb, pdf_right_testb, pdf_wrong_testb, pdf_bkg_testb), RooArgList(small, nright, nwrong, nbkg));
		
		RooRealVar expbkg("expbkg", "expbkg", nexpbkg);
		RooPoisson nbkg_pdf("nbkg_pdf", "nbkg_pdf", nbkg, expbkg);
		RooDataSet* data_dummy = pdf_dataorig_testb.generate(y, 1);
		
		double Nright = 0.;
		double Nwrong = 0.;
		double Nbkg = 0.;
		for(int exp = 0 ; exp < nexp ; ++exp)
		{
			stringstream ssp;
			ssp << i << "_" << exp;
			int n = rand.Poisson(Ndata);
			RooDataHist* data_test = pdf_dataorig_test.generateBinned(x, n);
			RooDataHist* data_testb = pdf_dataorig_testb.generateBinned(y, n);
			//RooDataHist* data = pdf_dataorig.generateBinned(x, Ndata);
			//int n_right = rand.Poisson(Ndata);
			//RooDataHist* data_test = pdf_right_test.generateBinned(x, n_right);
			//RooDataHist* data_testb = pdf_right_testb.generateBinned(y, n_right);
			//int n_wrong = rand.Poisson(nexpwrong);
			//data_test->add(*pdf_wrong_test.generateBinned(x, n_wrong));
			//data_testb->add(*pdf_wrong_testb.generateBinned(y, n_wrong));
			//int n_bkg = rand.Poisson(nexpbkg);
			//data_test->add(*pdf_bkg_test.generateBinned(x, n_bkg));
			//data_testb->add(*pdf_bkg_testb.generateBinned(y, n_bkg));

			RooNLLVar nll_test("nll_test","nll_test", model_test, *data_test, Extended(kTRUE));
			RooNLLVar nll_testb("nll_testb","nll_testb", model_testb, *data_testb, Extended(kTRUE));
			RooNLLVar nll_nuicance("nll_nuicance","nll_nuicance", nbkg_pdf, *data_dummy);
			//cout << "TESTOTTO: " << nll_nuicance.getVal() << " " << nexpbkg << " " << nbkg.getVal() << endl;
			RooAddition nll("nll", "nll", RooArgSet(nll_test, nll_nuicance));
			//RooAddition nll("nll", "nll", RooArgSet(nll_testb, nll_nuicance));
			//RooAddition nll("nll", "nll", RooArgSet(nll_test, nll_testb, nll_nuicance));
			RooMinuit m(nll);
			m.migrad();
			m.hesse();
			hL->Fill(nll.getVal());

			//model_testb.fitTo(*data_testb, Minos(true), PrintLevel(-1), Save());
	cout << "OTTO " << nbkg.getVal() << " " << hbkg_test->Integral() << " " << nright.getVal() << " " << hright_test->Integral() << " " << nwrong.getVal() << " " << hwrong_test->Integral()<< endl;

			//cout << "OTTO "  << i << " " << exp << ": " << hright_test->Integral() << " " << Ndata << " " << data_truth->GetBinContent(i) << " " << nright.getVal() <<"+-" << nright.getError() << " " << nright.getError()/nright.getVal()*100. << " " << nll.getVal() << endl;
			hfitsum->SetBinContent(i, hfitsum->GetBinContent(i) + nright.getVal());
			hfitsumq->SetBinContent(i, hfitsumq->GetBinContent(i) + nright.getVal()*nright.getVal());
			Nright += nright.getVal();
			Nwrong += nwrong.getVal();
			Nbkg += nbkg.getVal();
			if(exp == 0)
			{

				stringstream title;
				title << data_truth->GetXaxis()->GetBinLowEdge(i) << " < p_{T} < " << data_truth->GetXaxis()->GetBinUpEdge(i) << " GeV" << endl;

				RooPlot* xframe_test = x.frame();
				xframe_test->SetTitle("");
				data_test->plotOn(xframe_test, MarkerColor(kBlack));
				model_test.plotOn(xframe_test, Name("pfit"));
				model_test.plotOn(xframe_test, Components(pdf_right_test), LineColor(kRed), LineStyle(4), Name("psig_test"));
				model_test.plotOn(xframe_test, Components(RooArgSet(pdf_wrong_test)), LineColor(kBlue), LineStyle(4), Name("pwrong_test"));
				model_test.plotOn(xframe_test, Components(RooArgSet(pdf_bkg_test)), LineColor(kGreen), LineStyle(4), Name("pbkg_test"));
				data_test->plotOn(xframe_test, MarkerColor(kBlack), MarkerStyle(20), MarkerSize(2), Name("pdata_test"));

				xframe_test->Draw();
				xframe_test->GetXaxis()->SetTitle("-Log(#lambda)");

				TLegend* lg_test = new TLegend(0.6, 0.6, 0.95, 0.85, title.str().c_str());
				//lg_test->SetNColumns(2);
				lg_test->SetFillColor(0);
				lg_test->SetFillStyle(0);
				lg_test->SetLineColor(0);
				lg_test->SetLineStyle(0);
				lg_test->SetBorderSize(0);
				lg_test->SetShadowColor(0);
				lg_test->SetTextFont(42);
				lg_test->SetTextSize(0.04);
				stringstream num;
				num << round(nright.getVal()) << "#pm" << round(nright.getError());
				lg_test->AddEntry("psig_test", ("t#bar{t} sig.: " + num.str()).c_str(), "l");
				num.str("");
				num << round(nwrong.getVal()) << "#pm" << round(nwrong.getError());
				lg_test->AddEntry("pwrong_test", ("t#bar{t} bkg.: " +num.str()).c_str(), "l");
				num.str("");
				num << round(nbkg.getVal()) << "#pm" << round(nbkg.getError());
				lg_test->AddEntry("pbkg_test", ("other bkg.: " + num.str()).c_str(), "l");
				lg_test->AddEntry("pfit_test", "template fit", "l");
				lg_test->AddEntry("pdata_test", "pseudo data", "lp");
				lg_test->Draw();

				can->SaveAs(("pic_"+histname+"_test_"+ssp.str()+".png").c_str());

				RooPlot* xframe_testb = y.frame();
				xframe_testb->SetTitle("");
				data_testb->plotOn(xframe_testb, MarkerColor(kBlack));
				model_testb.plotOn(xframe_testb, Name("pfit"));
				model_testb.plotOn(xframe_testb, Components(pdf_right_testb), LineColor(kRed), LineStyle(4), Name("psig_testb"));
				model_testb.plotOn(xframe_testb, Components(RooArgSet(pdf_wrong_testb)), LineColor(kBlue), LineStyle(4), Name("pwrong_testb"));
				model_testb.plotOn(xframe_testb, Components(RooArgSet(pdf_bkg_testb)), LineColor(kGreen), LineStyle(4), Name("pbkg_testb"));
				data_testb->plotOn(xframe_testb, MarkerColor(kBlack), MarkerStyle(20), MarkerSize(2), Name("pdata_testb"));

				xframe_testb->Draw();
				xframe_testb->GetXaxis()->SetTitle("-Log(#lambda)");

				TLegend* lg_testb = new TLegend(0.6, 0.6, 0.95, 0.85, title.str().c_str());
				//lg_testb->SetNColumns(2);
				lg_testb->SetFillColor(0);
				lg_testb->SetFillStyle(0);
				lg_testb->SetLineColor(0);
				lg_testb->SetLineStyle(0);
				lg_testb->SetBorderSize(0);
				lg_testb->SetShadowColor(0);
				lg_testb->SetTextFont(42);
				lg_testb->SetTextSize(0.04);
				lg_testb->AddEntry("psig_testb", "t#bar{t} correct reco", "l");
				lg_testb->AddEntry("pwrong_testb", "t#bar{t} other", "l");
				lg_testb->AddEntry("pbkg_testb", "backgrounds", "l");
				lg_testb->AddEntry("pfit_testb", "template fit", "l");
				lg_testb->AddEntry("pdata_testb", "pseudo data", "lp");
				lg_testb->Draw();

				can->SaveAs(("pic_"+histname+"_testb_"+ssp.str()+".png").c_str());
			}
			delete data_test;
			delete data_testb;
		}
		Nright /= nexp;
		Nwrong /= nexp;
		Nbkg /= nexp;
		RooRealVar nrightfix("nrightfix", "nrightfix", Nright);
		RooRealVar nwrongfix("nwrongfix", "nwrongfix", Nwrong);
		RooRealVar nbkgfix("nbkgfix", "nbkgfix", Nbkg);

		RooAddPdf model_testfix("model_testfix", "model_testfix", RooArgList(constant_test, pdf_right_test, pdf_wrong_test, pdf_bkg_test), RooArgList(small, nrightfix, nwrongfix, nbkgfix));
		RooAddPdf model_testbfix("model_testbfix", "model_testbfix", RooArgList(constant_testb, pdf_right_testb, pdf_wrong_testb, pdf_bkg_testb), RooArgList(small, nrightfix, nwrongfix, nbkgfix));
		for(int exp = 0 ; exp < nexp ; ++exp)
		{
			int n = rand.Poisson(Ndata);
			RooDataHist* data_test = model_testfix.generateBinned(x, n);
			RooDataHist* data_testb = model_testbfix.generateBinned(y, n);

			RooNLLVar nll_test("nll_test","nll_test", model_test, *data_test, Extended(kTRUE));
			RooNLLVar nll_testb("nll_testb","nll_testb", model_testb, *data_testb, Extended(kTRUE));
			RooNLLVar nll_nuicance("nll_nuicance","nll_nuicance", nbkg_pdf, *data_dummy);
			RooAddition nll("nll", "nll", RooArgSet(nll_test, nll_nuicance));
			//RooAddition nll("nll", "nll", RooArgSet(nll_testb, nll_nuicance));
			//RooAddition nll("nll", "nll", RooArgSet(nll_test, nll_testb, nll_nuicance));
			RooMinuit m(nll);
			m.migrad();
			m.hesse();
			hLMC->Fill(nll.getVal());
			delete data_test;
			delete data_testb;
		}
	}

	for(int b = 1 ; b <= hfitsum->GetNbinsX() ; ++b)
	{
		double m = hfitsum->GetBinContent(b)/nexp;
		double v = hfitsumq->GetBinContent(b)/nexp - m*m;
		hfitsum->SetBinContent(b, m);
		hfitsum->SetBinError(b, Sqrt(v));
		hfitsumq->SetBinContent(b, Sqrt(v)/m); 
	}

	infile->Close();

	outfile->Write();
	outfile->Close();

}
