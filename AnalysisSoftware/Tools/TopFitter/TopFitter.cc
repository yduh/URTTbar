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


class Template
{
	private:
		static RooDataSet* data_dummy;
		string name_;
		double uncertainty_;
		int interpolate = 0;
		TFile* file;
		TH2D* hist;
		void commontemplate()
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
		RooDataHist* roohist = 0;
		RooHistPdf* roopdf = 0;
		RooRealVar* N = 0;
		RooRealVar* Nexp = 0;
		RooRealVar* Sexp = 0;
		RooGaussian* gaussconst = 0;
		RooNLLVar* nllconst = 0;


	public:
		RooRealVar& GetN(){return(*N);}
		RooRealVar& GetNExp(){return(*Nexp);}
		RooHistPdf& GetPDF(){return(*roopdf);}
		bool isConstrained() {return(uncertainty_ > 0.);}
		RooNLLVar& GetConstNLL(){return(*nllconst);}
		double Tmin() {return(hist->GetXaxis()->GetXmin());}
		double Tmax() {return(hist->GetXaxis()->GetXmax());}
		double Tbins() {return(hist->GetXaxis()->GetNbins());}
		TH1D* VarHist(string hname) {return(hist->ProjectionY(hname.c_str()));}
		
		Template(string name, string filename, string histname, double scale, double uncertainty = -1, int rebin = 1) : name_(name), uncertainty_(uncertainty)
	{
		file = new TFile(filename.c_str(), "read");
		file->GetObject(histname.c_str(), hist);
		if(hist->Integral() < 100)
		{
			commontemplate();
			cout << "INFO: "<< name << " Use of common template due to low statistics." << endl;
		}
		hist->Scale(scale);
		if(rebin != 1)
		{
			hist->RebinX(rebin);
		}
		cleanzero(hist);
	}

		void LoadBin(RooRealVar& tempvar, int bin)
		{
			if(roohist != 0)
			{
				delete roohist;
				//delete roopdf;
				//delete N;
			}
			stringstream ss;
			ss << "_" << bin;
			TH1D* binhist = hist->ProjectionX((name_+"_hist"+ss.str()).c_str(), bin, bin);
			roohist = new RooDataHist((name_+"_roohist").c_str(), (name_+"_roohist").c_str(), tempvar, binhist);
			roopdf = new RooHistPdf((name_+"_roopdf").c_str(), (name_+"_roopdf").c_str(), tempvar, *roohist, interpolate);
			N = new RooRealVar((name_+"_N").c_str(), (name_+"_N").c_str(), binhist->Integral(), 0., 2.*binhist->Integral());
			if(data_dummy == 0)
			{
				data_dummy = roopdf->generate(tempvar, 1);
			}
			if(isConstrained())
			{
				if(gaussconst != 0)
				{
					delete gaussconst;
					delete nllconst;
					delete Nexp;
					delete Sexp;
				}
				Nexp = new RooRealVar((name_+"_nexp").c_str(), (name_+"_nexp").c_str(), binhist->Integral());
				Sexp = new RooRealVar((name_+"_sexp").c_str(), (name_+"_sexp").c_str(), Sqrt(binhist->Integral() + Power(binhist->Integral()*uncertainty_, 2)));
				gaussconst = new RooGaussian((name_+"_constraint").c_str(), (name_+"_constraint").c_str(), *N, *Nexp, *Sexp);
				nllconst = new RooNLLVar((name_+"_nllconst").c_str(), (name_+"_nllconst").c_str(), *gaussconst, *data_dummy);
			}
		}
};

RooDataSet* Template::data_dummy = 0;

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

class Model
{
	private:
		vector<Template*>& temps_;
		string name_;
		RooArgList pdfs;
		RooArgList vars;
		RooArgSet constraints;
		RooAddPdf* model;
		double norm = 0.;
	public:
		double Norm()
		{
			double norm = 0.;
			for(size_t t = 0 ; t < temps_.size() ; ++t)
			{
				norm += temps_[t]->GetN().getVal();
			}
			return(norm);
		}
		Model(string name, vector<Template*>& temps, RooRealVar& tempvar, int bin) : temps_(temps), name_(name)
		{
			for(size_t t = 0 ; t < temps_.size() ; ++t)
			{
				temps_[t]->LoadBin(tempvar, bin);
				if(temps_[t]->GetN().getVal() > 0.)
				{
					pdfs.add(temps_[t]->GetPDF());
					vars.add(temps_[t]->GetN());
					if(temps_[t]->isConstrained())
					{
						constraints.add(temps_[t]->GetConstNLL());
					}
				}
			}
			model = new RooAddPdf(name_.c_str(), name_.c_str(), pdfs, vars);
		}
		~Model()
		{
			delete model;
		}

		RooAddPdf& GetModel()
		{
			return(*model);
		}

		RooArgSet& GetConstraints(){return(constraints);}
};

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
	TCanvas* can = new TCanvas("can", "can", 2000, 1600);
	can->SetRightMargin(0.05);
	can->SetTopMargin(0.05);

	vector<Template*> templates;
	string histnamereco("RECO/all_test_" + histname);
	templates.push_back(new Template("ttsig", CP.Get<string>("TEM_tt").c_str(), "TRUTH/"+histtyp+"_right_test_" + histname, TTscale));
	templates.push_back(new Template("ttbkg", CP.Get<string>("TEM_tt").c_str(), "TRUTH/"+histtyp+"_wrong_test_" + histname, TTscale));
	templates.push_back(new Template("Wt", CP.Get<string>("TEM_Wt").c_str(), histnamereco, Wtscale, CP.Get<double>("UNC_Wt"), 5));
	templates.push_back(new Template("Wtbar", CP.Get<string>("TEM_Wtbar").c_str(), histnamereco, Wtbarscale, CP.Get<double>("UNC_Wtbar"), 5));
	templates.push_back(new Template("STt", CP.Get<string>("TEM_STt").c_str(), histnamereco, STtscale, CP.Get<double>("UNC_STt"), 5));
	templates.push_back(new Template("W", CP.Get<string>("TEM_WJets").c_str(), histnamereco, Wscale, CP.Get<double>("UNC_WJets"), 5));
	templates.push_back(new Template("DY", CP.Get<string>("TEM_DYJets").c_str(), histnamereco, Zscale, CP.Get<double>("UNC_DYJets"), 5));

	vector<Template*> data;
	bool isDATA = CP.Get<bool>("ISDATA");
	if(isDATA)
	{
	//realdata
		data.push_back(new Template("data", CP.Get<string>("DATAFILE").c_str(), "RECO/all_test_" + histname, 1.));
	}
	else
	{
	//simdata
		string tttestfile(CP.Get<string>("DATASIM_tt"));
		double TTscale2 = lumi*CPscale.Get<double>("tt_xsec")/CPscale.Get<double>(FNbody(tttestfile)+"_W");
		data.push_back(new Template("ttsig", tttestfile, "TRUTH/"+histtyp+"_right_test_" + histname, TTscale2));
		data.push_back(new Template("ttbkg", tttestfile, "TRUTH/"+histtyp+"_wrong_test_" + histname, TTscale2));
		data.push_back(new Template("Wt", CP.Get<string>("DATASIM_Wt").c_str(), histnamereco, Wtscale, -1, 5));
		data.push_back(new Template("Wtbar", CP.Get<string>("DATASIM_Wtbar").c_str(), histnamereco, Wtbarscale, -1, 5));
		data.push_back(new Template("STt", CP.Get<string>("DATASIM_STt").c_str(), histnamereco, STtscale, -1, 5));
		data.push_back(new Template("W", CP.Get<string>("DATASIM_WJets").c_str(), histnamereco, Wscale, -1, 5));
		data.push_back(new Template("DY", CP.Get<string>("DATASIM_DYJets").c_str(), histnamereco, Zscale, -1, 5));
	}

	TRandom3 rand;

	TFile* outfile = new TFile(("fit_"+histtyp+"_"+histname+".root").c_str(), "recreate");

	TH1D* htrue = data[0]->VarHist("Truth");
	TH1D* hfitsum = new TH1D(*htrue);
	hfitsum->SetName("fitsum");
	hfitsum->Reset();
	TH1D* hfitsumq = new TH1D(*htrue);
	hfitsumq->SetName("fitsumq");
	hfitsumq->Reset();

	RooRealVar x("x","x", 0, data[0]->Tmin(), data[0]->Tmax());

	for(int i = 1 ; i <= htrue->GetNbinsX() ; ++i)
	{
		stringstream ss;
		ss << i;
		TH1D* hL= new TH1D(("L_"+ss.str()).c_str(), ("L_"+ss.str()).c_str(), 10000, -100000, 0);
		TH1D* hLMC= new TH1D(("LMC_"+ss.str()).c_str(), ("LMC_"+ss.str()).c_str(), 10000, -100000, 0);

		x.setBins(data[0]->Tbins());
		Model Mdata("data", data, x, i);
		x.setBins(data[0]->Tbins());
		Model Mfit("fit", templates, x, i);
		double Ndata = Mdata.Norm();
		cout << "Ndata: " << Ndata<< endl;
		if(Ndata == 0) continue;
		
		double Nright = 0.;
		double Nwrong = 0.;
		double Nbkg = 0.;
		for(int exp = 0 ; exp < nexp ; ++exp)
		{
			stringstream ssp;
			ssp << i << "_" << exp;
			int n = rand.Poisson(Ndata);
			x.setBins(data[0]->Tbins());
			RooDataHist* data_x = Mdata.GetModel().generateBinned(x, n);

			RooNLLVar nll_x("nll_x","nll_x", Mfit.GetModel(), *data_x, Extended(kTRUE));
			RooAddition nll("nll", "nll", RooArgSet(nll_x, Mfit.GetConstraints()));
			RooMinuit m(nll);
			m.migrad();
			m.hesse();
			hL->Fill(nll.getVal());

			double nsig = templates[0]->GetN().getVal();
			double nsigerr = templates[0]->GetN().getError();
			double nttbkg = templates[1]->GetN().getVal();
			double nttbkgerr = templates[1]->GetN().getError();
			cout << "MYINFO: " << nsig << " " << i << endl; 

			hfitsum->SetBinContent(i, hfitsum->GetBinContent(i) + nsig);
			hfitsumq->SetBinContent(i, hfitsumq->GetBinContent(i) + nsig*nsig);
			Nright += nsig;
			Nwrong += nttbkg;
			if(exp == 0)
			{
				stringstream title;
				title << htrue->GetXaxis()->GetBinLowEdge(i) << " < p_{T} < " << htrue->GetXaxis()->GetBinUpEdge(i) << " GeV" << endl;

				RooPlot* xframe_test = x.frame();
				xframe_test->SetTitle("");
				data_x->plotOn(xframe_test, MarkerColor(kBlack));
				Mfit.GetModel().plotOn(xframe_test, Name("pfit"));
				Mfit.GetModel().plotOn(xframe_test, Components(templates[0]->GetPDF()), LineColor(kRed), LineStyle(4), Name("psig_test"));
				Mfit.GetModel().plotOn(xframe_test, Components(templates[1]->GetPDF()), LineColor(kBlue), LineStyle(4), Name("pwrong_test"));
				Mfit.GetModel().plotOn(xframe_test, Components(RooArgSet(templates[2]->GetPDF(), templates[3]->GetPDF(), templates[4]->GetPDF(), templates[5]->GetPDF(), templates[6]->GetPDF())), LineColor(kGreen), LineStyle(4), Name("pbkg_test"));
				data_x->plotOn(xframe_test, MarkerColor(kBlack), MarkerStyle(20), MarkerSize(2), Name("pdata_test"));

				xframe_test->Draw();
				xframe_test->GetXaxis()->SetTitle("-Log(#lambda_{m})");

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
				num << round(nsig) << "#pm" << round(nsigerr);
				lg_test->AddEntry("psig_test", ("t#bar{t} sig.: " + num.str()).c_str(), "l");
				num.str("");
				num << round(nttbkg) << "#pm" << round(nttbkgerr);
				lg_test->AddEntry("pwrong_test", ("t#bar{t} bkg.: " +num.str()).c_str(), "l");
				num.str("");
				//num << round(nbkg.getVal()) << "#pm" << round(nbkg.getError());
				//lg_test->AddEntry("pbkg_test", ("other bkg.: " + num.str()).c_str(), "l");
				lg_test->AddEntry("pbkg_test", "other bkg", "l");
				lg_test->AddEntry("pfit_test", "template fit", "l");
				lg_test->AddEntry("pdata_test", "pseudo data", "lp");
				lg_test->Draw();

				can->SaveAs(("pic_"+histname+"_test_"+ssp.str()+".png").c_str());

		//		RooPlot* xframe_testb = y.frame();
		//		xframe_testb->SetTitle("");
		//		data_testb->plotOn(xframe_testb, MarkerColor(kBlack));
		//		model_testb.plotOn(xframe_testb, Name("pfit"));
		//		model_testb.plotOn(xframe_testb, Components(pdf_right_testb), LineColor(kRed), LineStyle(4), Name("psig_testb"));
		//		model_testb.plotOn(xframe_testb, Components(RooArgSet(pdf_wrong_testb)), LineColor(kBlue), LineStyle(4), Name("pwrong_testb"));
		//		model_testb.plotOn(xframe_testb, Components(RooArgSet(pdf_bkg_testb)), LineColor(kGreen), LineStyle(4), Name("pbkg_testb"));
		//		data_testb->plotOn(xframe_testb, MarkerColor(kBlack), MarkerStyle(20), MarkerSize(2), Name("pdata_testb"));

		//		xframe_testb->Draw();
		//		xframe_testb->GetXaxis()->SetTitle("-Log(#lambda)");

		//		TLegend* lg_testb = new TLegend(0.6, 0.6, 0.95, 0.85, title.str().c_str());
		//		//lg_testb->SetNColumns(2);
		//		lg_testb->SetFillColor(0);
		//		lg_testb->SetFillStyle(0);
		//		lg_testb->SetLineColor(0);
		//		lg_testb->SetLineStyle(0);
		//		lg_testb->SetBorderSize(0);
		//		lg_testb->SetShadowColor(0);
		//		lg_testb->SetTextFont(42);
		//		lg_testb->SetTextSize(0.04);
		//		lg_testb->AddEntry("psig_testb", "t#bar{t} correct reco", "l");
		//		lg_testb->AddEntry("pwrong_testb", "t#bar{t} other", "l");
		//		lg_testb->AddEntry("pbkg_testb", "backgrounds", "l");
		//		lg_testb->AddEntry("pfit_testb", "template fit", "l");
		//		lg_testb->AddEntry("pdata_testb", "pseudo data", "lp");
		//		lg_testb->Draw();

		//		can->SaveAs(("pic_"+histname+"_testb_"+ssp.str()+".png").c_str());
			}
			delete data_x;
		}
		//Nright /= nexp;
		//Nwrong /= nexp;
		//Nbkg /= nexp;
		//RooRealVar nrightfix("nrightfix", "nrightfix", Nright);
		//RooRealVar nwrongfix("nwrongfix", "nwrongfix", Nwrong);
		//RooRealVar nbkgfix("nbkgfix", "nbkgfix", Nbkg);

		//RooAddPdf model_testfix("model_testfix", "model_testfix", RooArgList(constant_test, pdf_right_test, pdf_wrong_test, pdf_bkg_test), RooArgList(small, nrightfix, nwrongfix, nbkgfix));
		//RooAddPdf model_testbfix("model_testbfix", "model_testbfix", RooArgList(constant_testb, pdf_right_testb, pdf_wrong_testb, pdf_bkg_testb), RooArgList(small, nrightfix, nwrongfix, nbkgfix));
		//for(int exp = 0 ; exp < nexp ; ++exp)
		//{
		//	int n = rand.Poisson(Ndata);
		//	RooDataHist* data_test = model_testfix.generateBinned(x, n);
		//	RooDataHist* data_testb = model_testbfix.generateBinned(y, n);

		//	RooNLLVar nll_test("nll_test","nll_test", model_test, *data_test, Extended(kTRUE));
		//	RooNLLVar nll_testb("nll_testb","nll_testb", model_testb, *data_testb, Extended(kTRUE));
		//	RooNLLVar nll_nuicance("nll_nuicance","nll_nuicance", nbkg_pdf, *data_dummy);
		//	RooAddition nll("nll", "nll", RooArgSet(nll_test, nll_nuicance));
		//	//RooAddition nll("nll", "nll", RooArgSet(nll_testb, nll_nuicance));
		//	//RooAddition nll("nll", "nll", RooArgSet(nll_test, nll_testb, nll_nuicance));
		//	RooMinuit m(nll);
		//	m.migrad();
		//	m.hesse();
		//	hLMC->Fill(nll.getVal());
		//	delete data_test;
		//	delete data_testb;
		//}
	}

	for(int b = 1 ; b <= hfitsum->GetNbinsX() ; ++b)
	{
		double m = hfitsum->GetBinContent(b)/nexp;
		double v = hfitsumq->GetBinContent(b)/nexp - m*m;
		hfitsum->SetBinContent(b, m);
		hfitsum->SetBinError(b, Sqrt(v));
		hfitsumq->SetBinContent(b, Sqrt(v)/m); 
	}

	outfile->Write();
	outfile->Close();

}
