#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>

#include "helper.h"

#include "TTree.h"
#include "TVector3.h"
#include "TFile.h"
#include "TH1D.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "RooGlobalFunc.h"
#include "RooChi2Var.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooFormulaVar.h"
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
		Float_t minp[4];
		Float_t maxp[4];
		//Float_t weight;
		UInt_t type;


	public:
		TH1D* correctiontemplates = 0;
		TH1D* smeartemplates = 0;
		double smear = 0.;
		TH1D* hright = 0;
		TH1D* hwrong = 0;
		TH1D* hmeas = 0;
		double ma = 0.;
		double mb = 0.;
		double papb = 0;
		double mm = 0;
		double pt = 0;
		int n = 0;

		BTagEffReader(int bins, double min, double max, const vector<double>& ptbins)
		{
			hright = new TH1D("right", "right", bins, min, max);
			hright->Sumw2();
			hwrong = new TH1D("wrong", "wrong", bins, min, max);
			hwrong->Sumw2();
			hmeas = new TH1D("meas", "meas", bins, min, max);
			hmeas->Sumw2();
			correctiontemplates = new TH1D("corr", "corr", ptbins.size()-1, ptbins.data());
			smeartemplates = new TH1D("smear", "smear", ptbins.size()-1, ptbins.data());
		}

		void SetCorrection(TMatrixD& corrm)
		{
			for(int i = 1 ; i <= correctiontemplates->GetNbinsX() ; ++i)
			{
				double oldv = correctiontemplates->GetBinContent(i);
				double newv = corrm(i-1, 0);
				correctiontemplates->SetBinContent(i, newv + oldv + oldv*newv);
			}
		}

		void SetSmear(TMatrixD& corrm)
		{
			for(int i = 1 ; i <= smeartemplates->GetNbinsX() ; ++i)
			{
				double oldv = smeartemplates->GetBinContent(i);
				double newv = corrm(i-1, 0);
				smeartemplates->SetBinContent(i, Sqrt(oldv*oldv + newv*newv));
			}
		}

		void AddFile(string filename, double scale)
		{
			TFile* f = TFile::Open(filename.c_str());
			TTree* tree = dynamic_cast<TTree*>(f->Get("TRUTH/jetscale"));
			tree->SetBranchAddress("minp", minp);	
			tree->SetBranchAddress("maxp", maxp);	
			//tree->SetBranchAddress("weight", &weight);	
			tree->SetBranchAddress("typ", &type);	
			files.push_back(f);
			trees.push_back(tree);
			scales.push_back(scale);
		}

		void Fill(double jmindown, double jminup, double jmaxdown, double jmaxup, bool Bmin = true, bool Bmax = true)
		{
			hright->Reset();
			hwrong->Reset();
			hmeas->Reset();
			n = 0;
			ma = 0.;
			mb = 0.;
			papb = 0.;
			mm = 0.;
			pt = 0.;
			TRandom3 rand(1234);
			for(size_t t = 0 ; t < trees.size() ; ++t)
			{
				cout << "tree " << t << endl; 
				TTree* tr = trees[t];
				double scale = scales[t];
				for(int i = 0 ; i < tr->GetEntries(); ++i)
				{
					tr->GetEntry(i);
					TLorentzVector pa(minp[0], minp[1], minp[2], minp[3]);
					TLorentzVector pb(maxp[0], maxp[1], maxp[2], maxp[3]);
					if(rand.Uniform() > 0.5)
					{
						pb.SetPxPyPzE(minp[0], minp[1], minp[2], minp[3]);
						pa.SetPxPyPzE(maxp[0], maxp[1], maxp[2], maxp[3]);
					}
					pa *= (1. + correctiontemplates->GetBinContent(correctiontemplates->FindFixBin(pa.Pt())));
					pb *= (1. + correctiontemplates->GetBinContent(correctiontemplates->FindFixBin(pb.Pt())));
					double smeara = smeartemplates->GetBinContent(smeartemplates->FindFixBin(pa.Pt()));
					if(smeara > 0)
					{
						pa *= rand.Gaus(1, smeara);
					}
					double smearb = smeartemplates->GetBinContent(smeartemplates->FindFixBin(pb.Pt()));
					if(smearb > 0)
					{
						pb *= rand.Gaus(1, smearb);
					}
					if(pa.Pt() > jmindown && pa.Pt() < jminup && pb.Pt() > jmaxdown && pb.Pt() < jmaxup)
					{
						//if((Bmin && Abs(pa.Eta()) < 1.4) || (!Bmin && Abs(pa.Eta()) > 1.4))
						{
							//if((Bmax && Abs(pb.Eta()) < 1.4) || (!Bmax && Abs(pb.Eta()) > 1.4))
							{
								double mass = (pa+pb).M();
								hmeas->Fill(mass, scale);
								if(type == 1)
								{
									papb += pa * pb;
									ma += pa.M();
									mb += pb.M();
									mm+=mass;
									pt+=pa.Pt();
									n++;
									hright->Fill(mass, scale);
								}
								else
								{
									hwrong->Fill(mass, scale);
								}
							}
						}
					}
				}				
			}
		}	

};


int main(int argc, char** argv)
{
	string datafilename(argv[1]); 
	double lumi = 5000; 
	double TTscale = lumi*806./25246993.;
	double Wscale = lumi*61524./10017462.;
	double Zscale = lumi*6024./2499142.;
	double Tscale = lumi*35.6/986100.;
	double Tbarscale = lumi*35.6/971800.;

	int interpolate = 0;
	int nexp = 50;
	TCanvas* can = new TCanvas("can", "can", 2000, 1600);

	int bins = 40;
	double xmin = 40.;
	double xmax = 120.;
	//vector<double> wjbin = {28., 40.,70, 100., 250.};
	vector<double> wjbin = {28., 45, 65, 90., 250.};
	//vector<double> wjbin = {28., 40, 50, 60, 90., 250.};

	BTagEffReader reader(bins, xmin, xmax, wjbin);
	reader.AddFile("ttJets.root", TTscale);
	//reader.AddFile("WJets.root", Wscale);
	//reader.AddFile("ZJets.root", Zscale);
	//reader.AddFile("singlet.root", Tscale);
	//reader.AddFile("singletbar.root", Tbarscale);

	BTagEffReader readerdata(bins, xmin, xmax, wjbin);
	readerdata.AddFile(datafilename, 1.);
	TMatrixD test(wjbin.size()-1, 1);
	for(size_t i = 0 ; i < wjbin.size()-1 ; ++i)
	{
		test(i, 0) = -0.03;
	}
	//readerdata.smear = 0.05;
	//test(0, 0) = -0.04;
	//test(1, 0) = 0.04;
	//test(2, 0) = -0.04;
	//test(3, 0) = 0.04;
	//test(4, 0) = 0.00;
	readerdata.SetCorrection(test);
	TMatrixD rtest(wjbin.size()-1, 1);
	for(size_t i = 0 ; i < wjbin.size()-1 ; ++i)
	{
		rtest(i, 0) = 0.1;
	}
	readerdata.SetSmear(rtest);

	TRandom3 rand;

	TFile* outfile = new TFile(("jetscalefit_" + datafilename).c_str(), "recreate");

	RooRealVar x("x","x", 0, xmin, xmax);
	x.setBins(bins);
	//x.setBins(10000);
	RooPolynomial constant("const", "const", x, RooArgList());
	RooRealVar small("small", "small", 0.0001);
	//double btagcut = 0.941;

	vector<TH2D*> chi2s;
	vector<TH2D*> shifts;
	vector<TH2D*> ress;
	vector<TH2D*> mas;
	vector<TH2D*> mbs;
	vector<TH2D*> mms;
	vector<TH2D*> papbs;
	vector<TH2D*> pts;
	vector<TH1D*> corrections;

	for(size_t iter = 0 ; iter < 2 ; ++iter)
	{
		stringstream ss;
		ss << iter;
		chi2s.push_back(new TH2D((ss.str()+"_chi2").c_str(), (ss.str()+"_chi2").c_str(), wjbin.size()-1, 0, wjbin.size()-1, wjbin.size()-1, 0, wjbin.size()-1));
		shifts.push_back(new TH2D((ss.str()+"_mean").c_str(), (ss.str()+"_mean").c_str(), wjbin.size()-1, 0, wjbin.size()-1, wjbin.size()-1, 0, wjbin.size()-1));
		ress.push_back(new TH2D((ss.str()+"_width").c_str(), (ss.str()+"_mean").c_str(), wjbin.size()-1, 0, wjbin.size()-1, wjbin.size()-1, 0, wjbin.size()-1));
		mas.push_back(new TH2D((ss.str()+"_ma").c_str(), (ss.str()+"_mean").c_str(), wjbin.size()-1, 0, wjbin.size()-1, wjbin.size()-1, 0, wjbin.size()-1));
		mbs.push_back(new TH2D((ss.str()+"_mb").c_str(), (ss.str()+"_mean").c_str(), wjbin.size()-1, 0, wjbin.size()-1, wjbin.size()-1, 0, wjbin.size()-1));
		mms.push_back(new TH2D((ss.str()+"_mm").c_str(), (ss.str()+"_mean").c_str(), wjbin.size()-1, 0, wjbin.size()-1, wjbin.size()-1, 0, wjbin.size()-1));
		papbs.push_back(new TH2D((ss.str()+"_papb").c_str(), (ss.str()+"_mean").c_str(), wjbin.size()-1, 0, wjbin.size()-1, wjbin.size()-1, 0, wjbin.size()-1));
		pts.push_back(new TH2D((ss.str()+"_pt").c_str(), (ss.str()+"_mean").c_str(), wjbin.size()-1, 0, wjbin.size()-1, wjbin.size()-1, 0, wjbin.size()-1));
		corrections.push_back(new TH1D((ss.str()+"_correction").c_str(), (ss.str()+"_correction").c_str(), wjbin.size()-1, 0, wjbin.size()-1));

		for(size_t i = 1 ; i < wjbin.size() ; ++i)
		{
			for(size_t j = 1 ; j < wjbin.size() ; ++j)
			{
				reader.Fill(wjbin[i-1], wjbin[i], wjbin[j-1], wjbin[j]);
				readerdata.Fill(wjbin[i-1], wjbin[i], wjbin[j-1], wjbin[j]);
				RooDataHist hright("hright", "hright", x, reader.hright);
				RooDataHist hwrong("hwrong", "hwrong", x, reader.hwrong);
				TH1D* thdata = readerdata.hmeas;
				if(iter % 2 == 0)
				{
					thdata = reader.hmeas;
				}
				RooDataHist hdata("hdata", "hdata", x, thdata);
				double Ndata = readerdata.hmeas->Integral();
				//if(Ndata < 500) continue;

				RooRealVar a("a","a",0,-10,10) ;
				RooFormulaVar xf("xf","x-a",RooArgSet(x,a)) ; 		
				RooHistPdf pdf_right("pdf_right", "pdf_right", xf, x, hright, interpolate);
				RooHistPdf pdf_wrong("pdf_wrong", "pdf_wrong", x, hwrong, interpolate);
				RooHistPdf pdf_data("pdf_data", "pdf_data", x, hdata, interpolate);

				RooRealVar nsigpass("nsigpass", "nsigpass", Ndata*0.25, 0., 2.*Ndata);
				RooRealVar nbkgpass("nbkgpass", "nbkgpass", Ndata*0.25, 0., 2.*Ndata);
				RooRealVar wres("wres", "wres", 5, 1, 100.);
				RooRealVar wshift("wshift", "wshift", 80, 50., 110.);
				RooGaussian pdf_sig("gauss","gauss", x, wshift, wres);
				//RooRealVar wres("wres", "wres", 0.5, 0.1, 20.);
				//RooRealVar null("null", "null", 0);
				//RooGaussian gauss("gauss","gauss", x, null, wres);

				//x.setBins(10000, "fft");
				//RooFFTConvPdf pdf_sig("sig", "sig",x, pdf_right, gauss);
				RooAddPdf model("model", "model", RooArgList(constant, pdf_sig, pdf_wrong), RooArgList(small, nsigpass, nbkgpass));

				stringstream ssp;
				ssp << iter << "_" << i << "_" << j;
				//TH1D* hshift = new TH1D((ssp.str()+"_shift").c_str(), (ssp.str()+"_shift").c_str(), 2000, -10., 10);
				//TH1D* hres = new TH1D((ssp.str()+"_res").c_str(), (ssp.str()+"_res").c_str(), 400, 0., 5.);
				TH1D* hshift = new TH1D((ssp.str()+"_shift").c_str(), (ssp.str()+"_shift").c_str(), 400, 60, 100);
				TH1D* hres = new TH1D((ssp.str()+"_res").c_str(), (ssp.str()+"_res").c_str(), 400, 0., 40);
				cout << "OTTO " << i << " " << j << endl;
				//x.setRange("RFit",50,110);
				//x.setBins(30, "RFit"); 

				RooFitResult* fitresult = model.fitTo(hdata, Minos(true), SumW2Error(kTRUE), PrintLevel(-1), Save());
				hshift->Fill(wshift.getVal());
				hres->Fill(wres.getVal());
				RooChi2Var thechi2("chi2", "chi2", model, hdata);
				chi2s.back()->SetBinContent(i, j, thechi2.getVal());
			
				if(false)
				{
					RooPlot* xframe_test = x.frame();
					xframe_test->SetTitle("");
					hdata.plotOn(xframe_test, MarkerColor(kBlack));
					model.plotOn(xframe_test, Name("pfit"));
					model.plotOn(xframe_test, Components(pdf_sig), LineColor(kRed), LineStyle(4), Name("psig_test"));
					model.plotOn(xframe_test, Components(RooArgSet(pdf_wrong)), LineColor(kBlue), LineStyle(4), Name("pwrong_test"));
					hdata.plotOn(xframe_test, MarkerColor(kBlack), MarkerStyle(20), MarkerSize(2), Name("pdata_test"));

					xframe_test->Draw();
					can->SaveAs(("pic_"+ssp.str()+".png").c_str());
				}
				//for(int exp = 0 ; exp < nexp ; ++exp)
				//{
				//	int n = rand.Poisson(Ndata);
				//	RooDataHist* data = pdf_data.generateBinned(x, n);

				//	//RooFitResult* fitresult = model.fitTo(*data, Minos(true), PrintLevel(-1), Save(), Range("RFit"));
				//	RooFitResult* fitresult = model.fitTo(*data, Minos(true), PrintLevel(-1), Save());
				//	hshift->Fill(wshift.getVal());
				//	//hshift->Fill(a.getVal());
				//	hres->Fill(wres.getVal());

				//	if(exp == 0)
				//	{
				//		RooPlot* xframe_test = x.frame();
				//		xframe_test->SetTitle("");
				//		data->plotOn(xframe_test, MarkerColor(kBlack));
				//		model.plotOn(xframe_test, Name("pfit"));
				//		model.plotOn(xframe_test, Components(pdf_sig), LineColor(kRed), LineStyle(4), Name("psig_test"));
				//		model.plotOn(xframe_test, Components(RooArgSet(pdf_wrong)), LineColor(kBlue), LineStyle(4), Name("pwrong_test"));
				//		data->plotOn(xframe_test, MarkerColor(kBlack), MarkerStyle(20), MarkerSize(2), Name("pdata_test"));

				//		xframe_test->Draw();
				//		stringstream ssc;
				//		ssc << i << "_" << j << "_" << exp;
				//		can->SaveAs(("pic_"+ssc.str()+".png").c_str());

				//	}

				//}
				//shifts.back()->SetBinContent(i, j, hshift->GetMean());
				//shifts.back()->SetBinError(i, j, hshift->GetRMS());
				shifts.back()->SetBinContent(i, j, wshift.getVal());
				shifts.back()->SetBinError(i, j, wshift.getError());
				ress.back()->SetBinContent(i, j, wres.getVal());
				ress.back()->SetBinError(i, j, wres.getError());
				mas.back()->SetBinContent(i, j, reader.ma/reader.n);
				mbs.back()->SetBinContent(i, j, reader.mb/reader.n);
				papbs.back()->SetBinContent(i, j, reader.papb/reader.n);
				mms.back()->SetBinContent(i, j, reader.mm/reader.n);
				pts.back()->SetBinContent(i, j, reader.pt/reader.n);
			}
		}

		if(iter % 2 == 1)
		{

			TMatrix M((wjbin.size()-1)*(wjbin.size()-1), wjbin.size()-1);
			TMatrix E((wjbin.size()-1)*(wjbin.size()-1), (wjbin.size()-1)*(wjbin.size()-1));
			TMatrix S((wjbin.size()-1)*(wjbin.size()-1), 1);

			TMatrix rM((wjbin.size()-1)*(wjbin.size()-1), wjbin.size()-1);
			TMatrix rE((wjbin.size()-1)*(wjbin.size()-1), (wjbin.size()-1)*(wjbin.size()-1));
			TMatrix rS((wjbin.size()-1)*(wjbin.size()-1), 1);
			
			int nmes = 0;
			for(size_t i = 1 ; i < wjbin.size() ; ++i)
			{
				for(size_t j = 1 ; j < wjbin.size() ; ++j)
				{
					if(i == 1 && j == 1) continue;
					if(i == 1 && j == 2) continue;
					if(i == 1 && j == 3) continue;
					if(i == 2 && j == 1) continue;
					if(i == 2 && j == 2) continue;
					if(i == 3 && j == 1) continue;
					double ma = mas.back()->GetBinContent(i, j);
					double mb = mbs.back()->GetBinContent(i, j);
					double papb = papbs.back()->GetBinContent(i, j);
					double mnew = shifts.back()->GetBinContent(i, j);
					double mnewerr = shifts.back()->GetBinError(i, j);
					double mold = shifts[shifts.size()-2]->GetBinContent(i, j);
					double molderr = shifts[shifts.size()-2]->GetBinError(i, j);
					//int nmes = (i-1)*(wjbin.size()-1)+(j-1);
					M(nmes, i-1)+=2.*(ma*ma+papb);
					M(nmes, j-1)+=2.*(mb*mb+papb);
					S(nmes, 0) = mnew*mnew - mold*mold;
					//cout << mnew << "+-" << mnewerr << " " << mold << "+-" << molderr << " " << (mnew*mnew/mold/mold-1.)/2. << endl;
					E(nmes, nmes) = mnewerr*mnewerr;

					double rnew = ress.back()->GetBinContent(i, j);
					double rnewerr = ress.back()->GetBinError(i, j);
					double rold = ress[0]->GetBinContent(i, j);
					double rolderr = ress[0]->GetBinError(i, j);
					rM(nmes, i-1)+=1;
					rM(nmes, j-1)+=1;
					rS(nmes, 0) = rnew*rnew - rold*rold;
					cout << i << " " << j << " " << rnew << " " << rold << " " << rnew*rnew - rold*rold << endl;
					rE(nmes, nmes) = rnewerr*rnewerr;
					nmes++;
				}
			}
			M.ResizeTo(nmes, wjbin.size()-1);
			E.ResizeTo(nmes, nmes);
			S.ResizeTo(nmes, 1);
			rM.ResizeTo(nmes, wjbin.size()-1);
			rE.ResizeTo(nmes, nmes);
			rS.ResizeTo(nmes, 1);
			stringstream it;
			it << iter;
			//Resolution
			rM.Print();
			rS.Print();
			rE.Print();
			TMatrixD rEM(rE, TMatrixD::kMult, rM);
			TMatrixD rES(rE, TMatrixD::kMult, rS);
			TMatrixD rL(rM, TMatrixD::kTransposeMult, rEM);
			TMatrixD rR(rM, TMatrixD::kTransposeMult, rES);
			rL.Invert();
			TMatrixD rF(rL, TMatrixD::kMult, rR);
			rF.Print();
			for(size_t b = 1 ; b < wjbin.size() ; ++b)
			{
				cout << rF(b-1, 0) << " " << pts[shifts.size()-2]->GetBinContent(b,b) << " " << rF(b-1, 0)/pts[shifts.size()-2]->GetBinContent(b,b) << endl;
				rF(b-1, 0) = Sqrt(Max(0.,rF(b-1, 0)))/pts[shifts.size()-2]->GetBinContent(b,b);
			}
			reader.SetSmear(rF);
			TH1D* rescor = new TH1D(*reader.smeartemplates);
			rescor->SetName(("Res_"+it.str()).c_str());

			//Scale
			M.Print();
			S.Print();
			E.Print();
			TMatrixD EM(E, TMatrixD::kMult, M);
			TMatrixD ES(E, TMatrixD::kMult, S);
			TMatrixD L(M, TMatrixD::kTransposeMult, EM);
			TMatrixD R(M, TMatrixD::kTransposeMult, ES);
			L.Invert();
			TMatrixD F(L, TMatrixD::kMult, R);
			F.Print();
			if(iter >= 3)
			{
				cout << iter << " " << chi2s[shifts.size()-1]->Integral() << " " << chi2s[shifts.size()-3]->Integral() << endl;
			}
			reader.SetCorrection(F);
			TH1D* scalecor = new TH1D(*reader.correctiontemplates);
			scalecor->SetName(("Scale_"+it.str()).c_str());

		}

	}

	reader.correctiontemplates->Print("all");
	outfile->Write();
	outfile->Close();

}
