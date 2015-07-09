#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>

#include <helper.h>
#include <ConfigParser.h>

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


using namespace std;
using namespace TMath;
using namespace RooFit;

class BTagEffReader
{
	private:
		vector<TFile*> files;
		vector<TTree*> trees;
		vector<double> scales;
		Float_t jwa[5];
		Float_t jwb[5];
		Float_t jb[5];
		Float_t prob;
		UInt_t typ;


	public:
		TH1D* temsigpass = 0;
		TH1D* tembkgpass = 0;
		TH1D* temsigbkgpass = 0;
		TH1D* temsigfail = 0;
		TH1D* tembkgfail = 0;
		TH1D* temsigbkgfail = 0;
		double jptmin = 0.;
		double jptmax = 100000.;
		double jetamin = -10.;
		double jetamax = 10.;
		double btag = 0.5;
		bool bjet = true;

		BTagEffReader(int bins, double min, double max)
		{
			temsigpass = new TH1D("sigpass", "sigpass", bins, min, max);
			tembkgpass = new TH1D("bkgpass", "bkgpass", bins, min, max);
			temsigbkgpass = new TH1D("sigbkgpass", "sigbkgpass", bins, min, max);
			temsigfail = new TH1D("sigfail", "sigfail", bins, min, max);
			tembkgfail = new TH1D("bkgfail", "bkgfail", bins, min, max);
			temsigbkgfail = new TH1D("sigbkgfail", "sigbkgfail", bins, min, max);
		}

		void AddFile(string filename, double scale)
		{
			TFile* f = TFile::Open(filename.c_str());
			TTree* tree = dynamic_cast<TTree*>(f->Get("TRUTH/btagtree"));
			tree->SetBranchAddress("jwa", jwa);	
			tree->SetBranchAddress("jwb", jwb);	
			tree->SetBranchAddress("jb", jb);	
			tree->SetBranchAddress("prob", &prob);	
			tree->SetBranchAddress("typ", &typ);	
			files.push_back(f);
			trees.push_back(tree);
			scales.push_back(scale);
		}

		void Fill()
		{
			temsigpass->Reset();
			tembkgpass->Reset();
			temsigbkgpass->Reset();
			temsigfail->Reset();
			tembkgfail->Reset();
			temsigbkgfail->Reset();
			for(size_t t = 0 ; t < trees.size() ; ++t)
			{
				TTree* tr = trees[t];
				double scale = scales[t];
				for(int i = 0 ; i < tr->GetEntries(); ++i)
				{
					tr->GetEntry(i);
					if(bjet)
					{
						TLorentzVector pb(jb[0], jb[1], jb[2], jb[3]);
						if(pb.Pt() < jptmin || pb.Pt() > jptmax || pb.Eta() < jetamin || pb.Eta() > jetamax) continue;
						if(jb[4] > btag)
						{
							temsigbkgpass->Fill(prob, scale);			
							if(typ) temsigpass->Fill(prob, scale);
							if(!typ) tembkgpass->Fill(prob, scale);
						}
						else
						{
							temsigbkgfail->Fill(prob, scale);			
							if(typ) temsigfail->Fill(prob, scale);
							if(!typ) tembkgfail->Fill(prob, scale);
						}
					}				
					else
					{
						TLorentzVector pwa(jwa[0], jwa[1], jwa[2], jwa[3]);
						if(pwa.Pt() > jptmin && pwa.Pt() < jptmax && pwa.Eta() > jetamin && pwa.Eta() < jetamax)
						{
							if(jwa[4] > btag)
							{
								temsigbkgpass->Fill(prob, scale);			
								if(typ) temsigpass->Fill(prob, scale);
								if(!typ) tembkgpass->Fill(prob, scale);
							}
							else
							{
								temsigbkgfail->Fill(prob, scale);			
								if(typ) temsigfail->Fill(prob, scale);
								if(!typ) tembkgfail->Fill(prob, scale);
							}
						}
						TLorentzVector pwb(jwb[0], jwb[1], jwb[2], jwb[3]);
						if(pwb.Pt() > jptmin && pwb.Pt() < jptmax && pwb.Eta() > jetamin && pwb.Eta() < jetamax)
						{
							if(jwb[4] > btag)
							{
								temsigbkgpass->Fill(prob, scale);			
								if(typ) temsigpass->Fill(prob, scale);
								if(!typ) tembkgpass->Fill(prob, scale);
							}
							else
							{
								temsigbkgfail->Fill(prob, scale);			
								if(typ) temsigfail->Fill(prob, scale);
								if(!typ) tembkgfail->Fill(prob, scale);
							}
						}
					}				
				}
			}
		}	

};


int main(int argc, char** argv)
{
	ConfigParser CP("BTagFitter.cfg");
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

	double xmin = 0.;
	double xmax = 10.;

	BTagEffReader reader(50, xmin, xmax);
	reader.AddFile(CP.Get<string>("TEM_tt").c_str(), TTscale);
	//reader.AddFile(CP.Get<string>("TEM_WJets").c_str(), Wscale);
	//reader.AddFile(CP.Get<string>("TEM_WJets").c_str(), Zscale);
	reader.AddFile(CP.Get<string>("TEM_Wt").c_str(), Wtscale);
	reader.AddFile(CP.Get<string>("TEM_Wtbar").c_str(), Wtbarscale);
	reader.AddFile(CP.Get<string>("TEM_STt").c_str(), STtscale);

	BTagEffReader readerdata(50, xmin, xmax);
	readerdata.AddFile(CP.Get<string>("DATASIM_tt").c_str(), TTscale);
	//readerdata.AddFile(CP.Get<string>("DATASIM_WJets").c_str(), Wscale);
	//readerdata.AddFile(CP.Get<string>("DATASIM_WJets").c_str(), Zscale);
	readerdata.AddFile(CP.Get<string>("DATASIM_Wt").c_str(), Wtscale);
	readerdata.AddFile(CP.Get<string>("DATASIM_Wtbar").c_str(), Wtbarscale);
	readerdata.AddFile(CP.Get<string>("DATASIM_STt").c_str(), STtscale);

	TRandom3 rand;

	TFile* outfile = new TFile("fithists.root", "recreate");
	vector<double> etabins = CP.GetVector<double>("etabins");
	vector<double> ptbins = CP.GetVector<double>("ptbins");
	int mode = CP.Get<int>("MODE");
	double btagcut = CP.Get<double>("btagcut");
	bool misstag = CP.Get<bool>("misstag");
	reader.btag = btagcut;
	reader.bjet = !misstag;
	readerdata.btag = btagcut;
	readerdata.bjet = !misstag;
	vector<double> bins;
	if(mode == 1)
	{
		bins = ptbins;
	}
	else if(mode == 2)
	{
		bins = etabins;
	}
	else if(mode == 3)
	{
		bins.resize(51);
		for(size_t b = 0 ; b < bins.size() ; ++b) bins[b] = 1./(bins.size()-1)*b;
	}

	TH1D* hfail_truth = new TH1D("fail_truth", "fail_truth", bins.size()-1, &(bins.front()));
	TH1D* hpass_truth = new TH1D("pass_truth", "pass_truth", bins.size()-1, &(bins.front()));
	TH1D* heff_truth = new TH1D("eff_truth", "eff_truth", bins.size()-1, &(bins.front()));
	TH1D* hdiff_truth = new TH1D("diff_truth", "diff_truth", bins.size()-1, &(bins.front()));

	TH1D* hfail_mcmes = new TH1D("fail_mcmes", "fail_mcmes", bins.size()-1, &(bins.front()));
	TH1D* hpass_mcmes = new TH1D("pass_mcmes", "pass_mcmes", bins.size()-1, &(bins.front()));
	TH1D* hfail_mcmesq = new TH1D("fail_mcmesq", "fail_mcmesq", bins.size()-1, &(bins.front()));
	TH1D* hpass_mcmesq = new TH1D("pass_mcmesq", "pass_mcmesq", bins.size()-1, &(bins.front()));
	TH1D* hpass_mceff = new TH1D("pass_mceff", "pass_mceff", bins.size()-1, &(bins.front()));
	TH1D* hpass_mcdiff = new TH1D("pass_mcdiff", "pass_mcdiff", bins.size()-1, &(bins.front()));

	RooRealVar x("x","x", 0, xmin, xmax);
	x.setBins(reader.temsigpass->GetNbinsX());
	RooPolynomial constant("const", "const", x, RooArgList());
	RooRealVar small("small", "small", 0.0001);
	//double btagcut = 0.941;
	double Ndata = 0.;
	for(size_t i = 1 ; i < bins.size() ; ++i)
	{
		if(mode == 1)
		{
			reader.jptmin = bins[i-1];
			reader.jptmax = bins[i];

			readerdata.jptmin = bins[i-1];
			readerdata.jptmax = bins[i];
		}
		else if(mode == 2)
		{
			reader.jetamin = bins[i-1];
			reader.jetamax = bins[i];

			readerdata.jetamin = bins[i-1];
			readerdata.jetamax = bins[i];
		}
		else if(mode == 3)
		{
			reader.btag = bins[i-1];
			readerdata.btag = bins[i-1];
		}

		reader.Fill();
		readerdata.Fill();
		//pass
		RooDataHist hsigpass("hsigpass", "hsigpass", x, reader.temsigpass);
		RooDataHist hbkgpass("hbkgpass", "hbkgpass", x, reader.tembkgpass);
		RooDataHist hdatapass("hdatapass", "hdatapass", x, readerdata.temsigbkgpass);
		hpass_truth->SetBinContent(i, reader.temsigpass->Integral());

		RooHistPdf pdf_sigpass("pdf_sigpass", "pdf_sigpass", x, hsigpass, interpolate);
		RooHistPdf pdf_bkgpass("pdf_bkgpass", "pdf_bkgpass", x, hbkgpass, interpolate);
		RooHistPdf pdf_datapass("pdf_sigbkgpass", "pdf_sigbkgpass", x, hdatapass, interpolate);

		Ndata = readerdata.temsigbkgpass->Integral();
		RooRealVar nsigpass("nsigpass", "nsigpass", Ndata*0.25, 0., 2.*Ndata);
		RooRealVar nbkgpass("nbkgpass", "nbkgpass", Ndata*0.25, 0., 2.*Ndata);

		RooAddPdf modelpass("modelpass", "modelpass", RooArgList(constant, pdf_sigpass, pdf_bkgpass), RooArgList(small, nsigpass, nbkgpass));

		for(int exp = 0 ; exp < nexp ; ++exp)
		{
			stringstream ssp;
			ssp << i << "_" << exp;
			int n = rand.Poisson(Ndata);
			RooDataHist* data = pdf_datapass.generateBinned(x, n);

			RooFitResult* fitresult = modelpass.fitTo(*data, Minos(true), PrintLevel(-1), Save());

			cout << "OTTO " << i << " " << exp << ": " << readerdata.temsigpass->Integral() << " " << nsigpass.getVal() <<"+-" << nsigpass.getError() << endl;
			hpass_mcmes->SetBinContent(i, hpass_mcmes->GetBinContent(i) + nsigpass.getVal());
			hpass_mcmesq->SetBinContent(i, hpass_mcmesq->GetBinContent(i) + nsigpass.getVal()*nsigpass.getVal());
			if(exp == 0)
			{
				RooPlot* xframe = x.frame() ;
				xframe->SetTitle("");
				data->plotOn(xframe, MarkerColor(kBlack));
				modelpass.plotOn(xframe, Name("pfit"));
				modelpass.plotOn(xframe, Components(pdf_sigpass), LineColor(kRed), LineStyle(4), Name("psig"));
				modelpass.plotOn(xframe, Components(RooArgSet(pdf_bkgpass)), LineColor(kBlue), LineStyle(4), Name("pbkg"));
				data->plotOn(xframe, MarkerColor(kBlack), MarkerStyle(20), MarkerSize(2), Name("pdata"));

				xframe->Draw();
				xframe->GetXaxis()->SetTitle("#lambda_{b}");
				stringstream title;
				if(mode == 1)title << bins[i-1] << " < p_{T} < " << bins[i] << " GeV";
				if(mode == 2)title << bins[i-1] << " < #eta < " << bins[i];
				if(mode == 3)title << "tag > " << bins[i]; 
				TLegend* lg = new TLegend(0.6, 0.6, 0.95, 0.85, ("Pass: " + title.str()).c_str());
				//lg->SetNColumns(2);
				lg->SetFillColor(0);
				lg->SetFillStyle(0);
				lg->SetLineColor(0);
				lg->SetLineStyle(0);
				lg->SetBorderSize(0);
				lg->SetShadowColor(0);
				lg->SetTextFont(42);
				lg->SetTextSize(0.04);
				lg->AddEntry("psig", "t#bar{t} correct reco", "l");
				lg->AddEntry("pbkg", "backgrounds", "l");
				lg->AddEntry("pfit", "template fit", "l");
				lg->AddEntry("pdata", "pseudo data", "lp");
				lg->Draw();
				can->SaveAs(("pass_"+ssp.str()+".png").c_str());
			}
			delete data;
			delete fitresult;
		}

		//fail
		RooDataHist hsigfail("hsigfail", "hsigfail", x, reader.temsigfail);
		RooDataHist hbkgfail("hbkgfail", "hbkgfail", x, reader.tembkgfail);
		RooDataHist hdatafail("hdatafail", "hdatafail", x, readerdata.temsigbkgfail);
		hfail_truth->SetBinContent(i, reader.temsigfail->Integral());

		RooHistPdf pdf_sigfail("pdf_sigfail", "pdf_sigfail", x, hsigfail, interpolate);
		RooHistPdf pdf_bkgfail("pdf_bkgfail", "pdf_bkgfail", x, hbkgfail, interpolate);
		RooHistPdf pdf_datafail("pdf_sigbkgfail", "pdf_sigbkgfail", x, hdatafail, interpolate);

		Ndata = readerdata.temsigbkgfail->Integral();
		RooRealVar nsigfail("nsigfail", "nsigfail", Ndata*0.25, 0., 2.*Ndata);
		RooRealVar nbkgfail("nbkgfail", "nbkgfail", Ndata*0.25, 0., 2.*Ndata);

		RooAddPdf modelfail("modelfail", "modelfail", RooArgList(constant, pdf_sigfail, pdf_bkgfail), RooArgList(small, nsigfail, nbkgfail));

		for(int exp = 0 ; exp < nexp ; ++exp)
		{
			stringstream ssp;
			ssp << i << "_" << exp;
			int n = rand.Poisson(Ndata);
			RooDataHist* data = pdf_datafail.generateBinned(x, n);

			RooFitResult* fitresult = modelfail.fitTo(*data, Minos(true), PrintLevel(-1), Save());

			cout << "OTTO " << i << " " << exp << ": " << readerdata.temsigfail->Integral() << " " << nsigfail.getVal() <<"+-" << nsigfail.getError() << endl;
			hfail_mcmes->SetBinContent(i, hfail_mcmes->GetBinContent(i) + nsigfail.getVal());
			hfail_mcmesq->SetBinContent(i, hfail_mcmesq->GetBinContent(i) + nsigfail.getVal()*nsigfail.getVal());
			if(exp == 0)
			{
				RooPlot* xframe = x.frame() ;
				xframe->SetTitle("");
				data->plotOn(xframe, MarkerColor(kBlack));
				modelfail.plotOn(xframe, Name("pfit"));
				modelfail.plotOn(xframe, Components(pdf_sigfail), LineColor(kRed), LineStyle(4), Name("psig"));
				modelfail.plotOn(xframe, Components(RooArgSet(pdf_bkgfail)), LineColor(kBlue), LineStyle(4), Name("pbkg"));
				data->plotOn(xframe, MarkerColor(kBlack), MarkerStyle(20), MarkerSize(2), Name("pdata"));

				xframe->Draw();
				xframe->GetXaxis()->SetTitle("#lambda_{b}");
				stringstream title;
				if(mode == 1)title << bins[i-1] << " < p_{T} < " << bins[i] << " GeV";
				if(mode == 2)title << bins[i-1] << " < #eta < " << bins[i];
				if(mode == 3)title << "tag > " << bins[i]; 
				TLegend* lg = new TLegend(0.6, 0.6, 0.95, 0.85, ("Fail: " + title.str()).c_str());
				//lg->SetNColumns(2);
				lg->SetFillColor(0);
				lg->SetFillStyle(0);
				lg->SetLineColor(0);
				lg->SetLineStyle(0);
				lg->SetBorderSize(0);
				lg->SetShadowColor(0);
				lg->SetTextFont(42);
				lg->SetTextSize(0.04);
				lg->AddEntry("psig", "t#bar{t} correct reco", "l");
				lg->AddEntry("pbkg", "backgrounds", "l");
				lg->AddEntry("pfit", "template fit", "l");
				lg->AddEntry("pdata", "pseudo data", "lp");
				lg->Draw();
				can->SaveAs(("fail_"+ssp.str()+".png").c_str());
			}
			delete data;
			delete fitresult;
		}
	}

	for(int b = 1 ; b <= hpass_mcmes->GetNbinsX() ; ++b)
	{
		double mp = hpass_mcmes->GetBinContent(b)/nexp;
		double vp = hpass_mcmesq->GetBinContent(b)/nexp - mp*mp;
		hpass_mcmes->SetBinContent(b, mp);
		hpass_mcmes->SetBinError(b, Sqrt(vp));
		hpass_mcmesq->SetBinContent(b, Sqrt(vp)/mp); 
		double mf = hfail_mcmes->GetBinContent(b)/nexp;
		double vf = hfail_mcmesq->GetBinContent(b)/nexp - mf*mf;
		hfail_mcmes->SetBinContent(b, mf);
		hfail_mcmes->SetBinError(b, Sqrt(vf));
		hfail_mcmesq->SetBinContent(b, Sqrt(vf)/mf);

		hpass_mceff->SetBinContent(b, mp/(mp+mf));
		hpass_mceff->SetBinError(b, Sqrt(vp*mf*mf + vf*mp*mp)/Power(mp+mf, 2));

		heff_truth->SetBinContent(b, hpass_truth->GetBinContent(b)/(hpass_truth->GetBinContent(b) + hfail_truth->GetBinContent(b)));
		if(b >= 2)
		{
			hdiff_truth->SetBinContent(b, heff_truth->GetBinContent(b-1)-heff_truth->GetBinContent(b));
			hpass_mcdiff->SetBinContent(b, hpass_mceff->GetBinContent(b-1)-hpass_mceff->GetBinContent(b));
			hpass_mcdiff->SetBinError(b, Sqrt(hpass_mceff->GetBinError(b-1)*hpass_mceff->GetBinError(b-1)+hpass_mceff->GetBinError(b)*hpass_mceff->GetBinError(b)));
		}
	}

	outfile->Write();
	outfile->Close();

}
