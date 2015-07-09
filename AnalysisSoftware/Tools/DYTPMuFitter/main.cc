#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TTree.h>
#include <TRandom3.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TLatex.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <TGraphAsymmErrors.h>
#include <TApplication.h>
#include <Math/DistFunc.h>

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooBreitWigner.h"
#include "RooVoigtian.h"
#include "RooExtendPdf.h"
#include "RooChebychev.h"
#include "RooPolynomial.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "RooCBShape.h"
#include "RooLandau.h"
#include "RooFFTConvPdf.h"
#include "RooExponential.h"
#include "RooCFunction1Binding.h"
#include "RooCFunction3Binding.h"
#include "RooCFunction4Binding.h"
#include "RooTFnBinding.h"
#include "RooKeysPdf.h"
#include "RooHistPdf.h"


using namespace std;
using namespace TMath;
using namespace RooFit;

pair<double, double> BinError(int A, int N, double conflev = 0.6827)
{
	double mean = double(A)/N;
	double u = 1;
	double pl = -1;
	double pu = -1;
	double p = mean;
	int precision = 8;
 while(true)
 {
	 p -= Power(0.1,u);
	 if(u == precision) {pl = p; break;}
	 if(p < 0) {p+=Power(0.1,u); u++; continue;}
	 if(BinomialI(p, N, A) < (1.-conflev)/2.){p+=Power(0.1,u); u++;}
 }

p = mean;
u = 1;
 while(true)
 {
	 p += Power(0.1,u);
	 if(u == precision) {pu = p; break;}
	 if(p > 1) {p-=Power(0.1,u); u++; continue;}
	 if((1-BinomialI(p, N, A)) < (1.-conflev)/2.){p-=Power(0.1,u); u++;}
 }
//cout << mean << "+" << pu-mean <<"-" << mean-pl << endl;
return(pair<double, double>(mean-pl, pu-mean));
}


bool PRINT = true;
TCanvas* can;

class Fitter
{
	private:
		RooRealVar x;
		RooRealVar w;
		RooRealVar mean;
		RooRealVar sigma;
		RooRealVar cbm0;
		RooRealVar cbsig;
		RooRealVar cbalpha;
		RooRealVar cbn;

		RooRealVar a0;
		RooRealVar b0;
		RooRealVar c0;
		RooRealVar gmbkg;
		RooRealVar gsbkg;
		RooRealVar numbkg;
		RooBreitWigner bw;
		RooCBShape cb;
		RooGaussian gauss;
		RooGaussian gaussbkg;
		RooAbsPdf* bkg;
		RooAbsPdf* sig;
		TLatex* t;
		void (Fitter::*reset)();

		void resetlandauA()
		{
			mean.setVal(91.2); mean.setRange(86., 96.);
			cbsig.setVal(0.1); cbsig.setRange(0., 1.);
			cbalpha.setVal(5.); cbalpha.setRange(0.1, 100.);
			cbn.setVal(30.); cbn.setRange(0., 50.);
			a0.setVal(90.); a0.setRange(0., 500.);
			b0.setVal(20.); b0.setRange(5., 500.);
			gsbkg.setVal(0.1); gsbkg.setRange(0., 50.);
			//numbkg.setVal(0.5); numbkg.setRange(0., 1.);
		}

		void resetlognormalA()
		{
			mean.setVal(91.2); mean.setRange(86., 96.);
			cbsig.setVal(1.2); cbsig.setRange(0.1, 5.);
			cbalpha.setVal(10.); cbalpha.setRange(1., 100.);
			cbn.setVal(10.); cbn.setRange(5., 1000.);
			a0.setVal(4.219); a0.setRange(1, 15.);
			b0.setVal(0.5); b0.setRange(0.1, 1.5);
		}

	public:
		Fitter() :
			x("Mass","M_{#mu#mu}", 50, 130, "GeV"),
			w("Weight","weight", 1.),
			mean("mean","mean",91.2, 86., 96.) ,
			sigma("sigma","sigma",2.4952) ,
			cbm0("cbm0","cbm0",0., -5., 5.),
			cbsig("cbsig","cbsig",0.1, 0.0, 1.),
			cbalpha("cbalpha","cbalpha", 5, 0.1, 100.),
			cbn("cbn","cbn",30, 0., 50.),
			a0("a0","a0", 90, 0., 500.),
			b0("b0","b0", 20, 5., 500.),
			c0("c0","c0", 0),
			gmbkg("gmbkg","gmbkg", 0.),
			gsbkg("gsbkg","gsbkg", 0.1, 0., 50.),
			numbkg("numbkg", "numbkg", 0.1, 0., 1.),
			bw("bw","bw",x,mean,sigma),
			cb("cb","cb", x, cbm0, cbsig, cbalpha, cbn),
			gauss("gauss","gauss", x, cbm0, cbsig),
			gaussbkg("gaussbkg","gaussbkg", x, gmbkg, gsbkg),
			bkg(0),
			sig(0),
			t(new TLatex())
			{
			}

		void Init(string typ)
		{
			if(bkg != 0) delete bkg;
			if(sig != 0) delete sig;
			x.setBins(1000, "fft");
			sig = new RooFFTConvPdf("sig", "sig",x, bw, cb);

			if(typ.find("landauA") != string::npos)
			{	
				bkg = new RooLandau("landau","landau",x,a0, b0);	
				reset = &Fitter::resetlandauA;
				Reset();
			}
			else if(typ.find("lognormalA") != string::npos)
			{
				bkg = bindPdf("bkg", ROOT::Math::lognormal_pdf, x, a0, b0, c0);
				reset = &Fitter::resetlognormalA;
				Reset();
			}
		}

		~Fitter()
		{
			delete t;
			delete sig;
			delete bkg;
		}

		void Reset()
		{
			(this->*reset)();
		}

		double GetFit(TTree* tree, string fitname, Double_t& Numsig, Double_t& Numsigerr, Double_t& Numbkg, Double_t& Numbkgerr, TTree* sigshape = 0)
		{
			Double_t num = Numsig;
			numbkg = Numsigerr;
			//if(num < 10) 
			//{
			//	Numsig = 0.;
			//	Numbkg = 0.;
			//	Numbkgerr = 0.;
			//	Numsigerr = 0.;
			//	return(-2.);
			//}

			RooDataSet* dataub = new RooDataSet("data", "Data", tree, RooArgSet(x, w), 0, "Weight");
			RooDataHist* databi = 0;
			RooAbsData* data = dataub;

			if(num > 5000)
			{
				x.setBins(160);
				databi = dataub->binnedClone();
				data = databi;
			}

			RooDataSet* sigset = 0;
			RooAbsPdf* tmpsig = 0;
			if(sigshape != 0)
			{
				sigset = new RooDataSet("mcshape", "mcshape", sigshape, RooArgSet(x, w), 0, "Weight");
				if(sigshape->GetEntries() > 5000)
				{
					x.setBins(160);
					RooDataHist* mcsigbi = sigset->binnedClone();
					tmpsig = new RooHistPdf("mcshapepdf","mcshapepdf",x,*mcsigbi,2);
				}
				else
				{
					tmpsig = new RooKeysPdf("mcshapepdf","mcshapepdf",x,*sigset);
				}
				x.setBins(10000, "fft");
				if(sig != 0) delete sig;
				sig = new RooFFTConvPdf(("sig"+fitname).c_str(), ("sig"+fitname).c_str(),x, *tmpsig, gauss);
			}

			// Fit model to data
			RooAddPdf* model = new RooAddPdf(("model"+fitname).c_str(),("sig+bkg"+fitname).c_str(),RooArgList(*bkg, *sig),RooArgList(numbkg)) ;
			//model->fitTo(*data, Range(50., 130.), Minos(true), Extended(true), PrintLevel(1)) ;
			model->fitTo(*data, Range(50., 130.), Minos(true), PrintLevel(1), SumW2Error(kTRUE)) ;

			Numsig = (1.-numbkg.getVal())*num;
			Numsigerr = numbkg.getError()*num;
			Numbkg = numbkg.getVal()*num;
			Numbkgerr = numbkg.getError()*num;

			double chi2 = 1.;
			if(PRINT)
			{
				Int_t numbins = 80;
				x.setBins(numbins);

				// Plot data and PDF overlaid
				RooPlot* xframe = x.frame(Title("Tag & Probe")) ;
				data->plotOn(xframe, MarkerColor(kBlack));
				model->plotOn(xframe);
				double chi2 = -1;
				if(databi != 0)
				{
					chi2 = model->createChi2(*databi)->getVal()/157.;
				}
				cout << "CHI2/NDOF: " << chi2 << endl;

				// Overlay the background component of model with a dashed line
				model->plotOn(xframe,Components(*bkg), LineColor(kRed), LineStyle(1)) ;

				// Overlay the background+sig2 components of model with a dotted line
				model->plotOn(xframe,Components(RooArgSet(*bkg,*sig)), LineColor(kBlue), LineStyle(1)) ;
				data->plotOn(xframe, MarkerColor(kBlack));
				model->plotOn(xframe,Components(*sig), LineColor(kGreen), LineStyle(kDotted)) ;
				//double chi2 = xframe->chiSquare(("model"+fitname).c_str(), "data", 4);
				chi2 = xframe->chiSquare(4);
				stringstream texta;
				stringstream textl;
				stringstream textb;
				stringstream textc;
				stringstream textd;
				if(fitname.find("passing") != string::npos)
				{
					texta << "Passing probes";
					textl << "#scale[1.5]{(a)}";
				}
				else
				{
					texta << "Failing probes";
					textl << "#scale[1.5]{(b)}";
				}
				textb << "Signal = " << int(Numsig) << " #pm " << int(Numsigerr);
				textc << "Bkg = " << int(Numbkg) << " #pm " << int(Numbkgerr);
				textd << "#chi^{2}/ndof = " << std::setprecision(4) << chi2;
				t->SetTextFont(132);
				t->SetTextSize(0.04);
				t->SetNDC(true);
				// Draw the frame on the canvas
				can->cd();
				//gPad->SetLeftMargin(0.15) ;
				xframe->GetYaxis()->SetTitleOffset(2.0) ;
				xframe->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]") ;
				xframe->GetYaxis()->SetTitle("Events / 1 GeV") ;
				xframe->Draw();
				t->DrawLatex(0.18, 0.85, texta.str().c_str());
				t->DrawLatex(0.18, 0.79, textb.str().c_str());
				t->DrawLatex(0.18, 0.73, textc.str().c_str());
				t->DrawLatex(0.18, 0.67, textd.str().c_str());
				//t->DrawLatex(0.25, 0.85, textl.str().c_str());
				can->SaveAs((fitname + ".png").c_str());
				//can->SaveAs((fitname.str() + string(".pdf")).c_str());
			}
			delete model;
			delete dataub;
			if(databi != 0) delete databi;
			if(sigset != 0) delete sigset;
			if(tmpsig != 0) delete tmpsig; 
			return(chi2);
		}
};


TGraphAsymmErrors* mydivide(string name, TH1D* hista, TH1D* histb)
{
	int nbins = hista->GetXaxis()->GetNbins();
	vector<Double_t> xpos;
	vector<Double_t> xposm;
	vector<Double_t> xposp;
	vector<Double_t> ypos;
	vector<Double_t> yposm;
	vector<Double_t> yposp;
	
	for(int i = 1 ; i <= nbins ; i++)
	{
		Double_t A = hista->GetBinContent(i);
		Double_t AErr = hista->GetBinError(i);
		Double_t B = histb->GetBinContent(i);
		Double_t BErr = histb->GetBinError(i);
		Double_t X = hista->GetBinCenter(i);
		Double_t Xm = hista->GetXaxis()->GetBinLowEdge(i);
		Double_t Xp = hista->GetXaxis()->GetBinUpEdge(i);
		xpos.push_back(X);
		xposm.push_back(X-Xm);
		xposp.push_back(Xp-X);
		if(A < 5)
		{
			ypos.push_back(-1.);
			yposm.push_back(0.);
			yposp.push_back(0.);
			continue;
		}
		double C = A + B;
		ypos.push_back(A/C);
		pair<double, double> errors = BinError(A, C);
		double fiterrQ = Power(A/(C*C)*BErr, 2) + Power((1./C - A/(C*C))*AErr,2);
		yposm.push_back(Sqrt(errors.first*errors.first + fiterrQ));
		yposp.push_back(Sqrt(errors.second*errors.second + fiterrQ));
	}
	TGraphAsymmErrors* result = new TGraphAsymmErrors(nbins, &(xpos[0]), &(ypos[0]), &(xposm[0]), &(xposp[0]), &(yposm[0]), &(yposp[0]));
	result->SetTitle(name.c_str());
	result->SetName(name.c_str());
	return(result);
}

vector<TH2D*> mydivide(string name, TH2D* hista, TH2D* histb)
{
	TH2D* result = new TH2D(*hista);
	TH2D* resulterrp = new TH2D(*hista);
	TH2D* resulterrm = new TH2D(*hista);
	result->SetTitle(name.c_str());
	result->SetName(name.c_str());
	resulterrp->SetTitle((name+"_errp").c_str());
	resulterrp->SetName((name+"_errp").c_str());
	resulterrm->SetTitle((name+"_errm").c_str());
	resulterrm->SetName((name+"_errm").c_str());
	for(int i = 1 ; i <= hista->GetYaxis()->GetNbins() ; i++)
	{
		for(int j = 1 ; j <= hista->GetXaxis()->GetNbins() ; j++)
		{
			Double_t A = hista->GetBinContent(j, i);
			Double_t AErr = hista->GetBinError(j, i);
			Double_t B = histb->GetBinContent(j, i);
			Double_t BErr = histb->GetBinError(j, i);
			if(A < 5)
			{
				result->SetBinContent(j, i, -1.);
				resulterrp->SetBinContent(j, i, -1.);
				resulterrm->SetBinContent(j, i, -1.);
				continue;
			}
			double C = A + B;
			result->SetBinContent(j, i, A/C);
			pair<double, double> errors = BinError(A, C);
			double fiterrQ = Power(A/(C*C)*BErr, 2) + Power((1./C - A/(C*C))*AErr,2);
			result->SetBinError(j, i, Sqrt(Power(Max(errors.second, errors.first), 2) + fiterrQ));
			resulterrp->SetBinContent(j, i, errors.second);
			resulterrm->SetBinContent(j, i, errors.first);
		}
	}
	vector<TH2D*> vec(3);
	vec[0] = result;
	vec[1] = resulterrp;
	vec[2] = resulterrm;
	return(vec);
}

struct Test
{
	public:
		string label;
		UInt_t passing;
		UInt_t all;
		Test(string Label, UInt_t Passing, UInt_t All) : label(Label), passing(Passing), all(All) {}
};

class TPAnalyse
{
	private:
		map<string, vector<Double_t> > bins;
		string name;
		TTree* tree;
		TFile* file;
		UInt_t truebits, falsebits;

		Float_t tpx, tpy, tpz, ppx, ppy, ppz, eta, rho, weight;
		UInt_t tpresult, run;
		Int_t pileup;

		//Shape from MC
		TTree* mctree;
		TFile* mcfile;
		Fitter myfitterpassing;
		Fitter myfitterfailing;
	public:
		TPAnalyse(string filename, string treename, string typpassing = "landauA passing", string typfailing = "landauA failing") : mctree(0), mcfile(0)
		{
			file = new TFile(filename.c_str());
			tree = dynamic_cast<TTree*>(file->Get(treename.c_str()));
			tree->SetBranchAddress("tpx", &tpx);
			tree->SetBranchAddress("tpy", &tpy);
			tree->SetBranchAddress("tpz", &tpz);
			tree->SetBranchAddress("ppx", &ppx);
			tree->SetBranchAddress("ppy", &ppy);
			tree->SetBranchAddress("ppz", &ppz);
			//tree->SetBranchAddress("eta", &eta);
			tree->SetBranchAddress("rho", &rho);
			tree->SetBranchAddress("numgoodvertices", &pileup);
			tree->SetBranchAddress("weight", &weight);
			tree->SetBranchAddress("run", &run);
			tree->SetBranchAddress("tpresult", &tpresult);
		}

		~TPAnalyse()
		{
			file->Close();
		}

		void SetBins(string name, vector<Double_t>& Ptbins) {bins[name] = Ptbins;}

		void SetMCShape(string filename, string treename)
		{
			mcfile = new TFile(filename.c_str());
			mctree = dynamic_cast<TTree*>(mcfile->Get(treename.c_str()));
			mctree->SetBranchAddress("tpx", &tpx);
			mctree->SetBranchAddress("tpy", &tpy);
			mctree->SetBranchAddress("tpz", &tpz);
			mctree->SetBranchAddress("ppx", &ppx);
			mctree->SetBranchAddress("ppy", &ppy);
			mctree->SetBranchAddress("ppz", &ppz);
			//mctree->SetBranchAddress("eta", &eta);
			mctree->SetBranchAddress("rho", &rho);
			mctree->SetBranchAddress("numgoodvertices", &pileup);
			mctree->SetBranchAddress("weight", &weight);
			mctree->SetBranchAddress("run", &run);
			mctree->SetBranchAddress("tpresult", &tpresult);
		}

		vector<TGraphAsymmErrors*> Get1DHist(string namex, string labelx, Test test)
		{
			UInt_t bittrue = test.passing;
			UInt_t bitfalse = test.all;

			Int_t nbins = bins[namex].size()-1;
			TH1D* htrue = new TH1D("htmp1", "htmp1", nbins, &(bins[namex][0]));
			TH1D* hall = new TH1D("htmp2", "htmp2", nbins, &(bins[namex][0]));
			TH1D* htrueweights = new TH1D("htmp1weights", "htmp1weights", nbins, &(bins[namex][0]));
			TH1D* hallweights = new TH1D("htmp2weights", "htmp2weights", nbins, &(bins[namex][0]));
			TH1D* htruemcweights = new TH1D("htmp1mcweights", "htmp1mcweights", nbins, &(bins[namex][0]));
			TH1D* hallmcweights = new TH1D("htmp2mcweights", "htmp2mcweights", nbins, &(bins[namex][0]));
			Double_t mass;
			Double_t myweight;
			vector<TTree*> passingtrees;
			vector<TTree*> failingtrees;
			for(int binnum = 0 ; binnum < nbins ; binnum++)
			{
				stringstream name;
				name << "_" << binnum;
				passingtrees.push_back(new TTree(("Tree_passing" + name.str()).c_str(), ("Tree_passing" + name.str()).c_str(), 1));
				passingtrees.back()->Branch("Mass", &mass, "Mass/D");
				passingtrees.back()->Branch("Weight", &myweight, "Weight/D");
				failingtrees.push_back(new TTree(("Tree_failing" + name.str()).c_str(), ("Tree_failing" + name.str()).c_str(), 1));
				failingtrees.back()->Branch("Mass", &mass, "Mass/D");
				failingtrees.back()->Branch("Weight", &myweight, "Weight/D");
			}
			cout << "OTTO Loading TP ..." << endl;
			for(int i = 0 ; i < tree->GetEntries() ; i++)
			{
				tree->GetEntry(i);
				TVector3 VT(tpx, tpy, tpz);
				TLorentzVector PT(VT, VT.Mag());
				TVector3 VP(ppx, ppy, ppz);
				TLorentzVector PP(VP, VP.Mag());
				mass = (PT+PP).M();
				myweight = weight;
				map<string, Double_t> vals;
				vals["eta"] = PP.Eta();
				vals["pt"] = PP.Pt();
				vals["pileup"] = pileup;
				vals["run"] = run;
				if(mass > 50. && mass < 130. && eta >= bins["eta"].front() && eta < bins["eta"].back() && PP.Pt() >= bins["pt"].front() && PP.Pt() < bins["pt"].back() && pileup >= bins["pileup"].front() && pileup < bins["pileup"].back() && run >= bins["run"].front() && run < bins["run"].back())
				{
					for(int binnum = 0 ; binnum < nbins ; binnum++)
					{
						if(vals[namex] >= bins[namex][binnum] &&  vals[namex] < bins[namex][binnum+1])
						{
							if((tpresult&bittrue) == bittrue)
							{
								passingtrees[binnum]->Fill();
								htrueweights->Fill(vals[namex], weight);
							}
							else if((tpresult&bitfalse) == bitfalse)
							{
								failingtrees[binnum]->Fill();
								hallweights->Fill(vals[namex], weight);
							}
							break;
						}
					}
				}
			}
			vector<TTree*> passingmctrees;
			vector<TTree*> failingmctrees;
			if(mctree != 0)
			{
				cout << "OTTO Loading MC-shape ..." << endl;
				for(int binnum = 0 ; binnum < nbins ; binnum++)
				{
					stringstream name;
					name << "_" << binnum;
					passingmctrees.push_back(new TTree(("MCTree_passing" + name.str()).c_str(), ("TMCree_passing" + name.str()).c_str(), 1));
					passingmctrees.back()->Branch("Mass", &mass, "Mass/D");
					passingmctrees.back()->Branch("Weight", &myweight, "Weight/D");
					failingmctrees.push_back(new TTree(("MCTree_failing" + name.str()).c_str(), ("MCTree_failing" + name.str()).c_str(), 1));
					failingmctrees.back()->Branch("Mass", &mass, "Mass/D");
					failingmctrees.back()->Branch("Weight", &myweight, "Weight/D");
				}
				for(int i = 0 ; i < mctree->GetEntries() ; i++)
				{
					mctree->GetEntry(i);
					TVector3 VT(tpx, tpy, tpz);
					TLorentzVector PT(VT, VT.Mag());
					TVector3 VP(ppx, ppy, ppz);
					TLorentzVector PP(VP, VP.Mag());
					//PT = PT * 1.003;
					//PP = PP * 1.003;
					mass = (PT+PP).M();
					myweight = weight;
					map<string, Double_t> vals;
					vals["eta"] = PP.Eta();
					vals["pt"] = PP.Pt();
					vals["pileup"] = pileup;
					vals["run"] = run;
					if(mass > 50. && mass < 130. && eta >= bins["eta"].front() && eta < bins["eta"].back() && PP.Pt() >= bins["pt"].front() && PP.Pt() < bins["pt"].back() && pileup >= bins["pileup"].front() && pileup < bins["pileup"].back() && run >= bins["run"].front() && run < bins["run"].back())
					{
						//cout << (tpresult&(1<<18)) << endl;
						if((tpresult&(1<<22)) == 0) continue;
						for(int binnum = 0 ; binnum < nbins ; binnum++)
						{
							if(vals[namex] >= bins[namex][binnum] &&  vals[namex] < bins[namex][binnum+1])
							{
								if((tpresult&bittrue) == bittrue)
								{
									passingmctrees[binnum]->Fill();
									htruemcweights->Fill(vals[namex], weight);
								}
								else if((tpresult&bitfalse) == bitfalse)
								{
									failingmctrees[binnum]->Fill();
									hallmcweights->Fill(vals[namex], weight);
								}
								break;
							}
						}
					}
				}
			}
			Double_t fBKG, fBKGErr, fSIG, fSIGErr;
			Double_t tBKG, tBKGErr, tSIG, tSIGErr;
			myfitterpassing.Init("landauA");
			myfitterfailing.Init("landauA");
			for(int binnum = 0 ; binnum < nbins ; binnum++)
			{
				//myfitterpassing.Reset();
				//myfitterfailing.Reset();
				stringstream fitname;
				fitname << test.label << "_" << binnum << "_" << namex;
cout << "OTTO P" << passingmctrees[binnum]->GetEntries()  << " " << passingtrees[binnum]->GetEntries() << " " << htrueweights->GetBinContent(binnum+1) << endl;
				tSIG = htrueweights->GetBinContent(binnum+1);
				tSIGErr = 0.1;
				myfitterpassing.GetFit(passingtrees[binnum], fitname.str()+"_passing", tSIG, tSIGErr, tBKG, tBKGErr, passingmctrees[binnum]);
				htrue->SetBinContent(binnum+1, tSIG);	
				htrue->SetBinError(binnum+1, tSIGErr);	
				delete passingtrees[binnum];
				delete passingmctrees[binnum];
cout << "OTTO F" << failingmctrees[binnum]->GetEntries() << " " << failingtrees[binnum]->GetEntries() << " " << hallweights->GetBinContent(binnum+1) << endl;
				fSIG = hallweights->GetBinContent(binnum+1);
				fSIGErr = 0.2;
				myfitterfailing.GetFit(failingtrees[binnum], fitname.str()+"_failing", fSIG, fSIGErr, fBKG, fBKGErr, failingmctrees[binnum]);
				hall->SetBinContent(binnum+1, fSIG);	
				hall->SetBinError(binnum+1, fSIGErr);	
				delete failingtrees[binnum];
				delete failingmctrees[binnum];
			}
			cout << "OTTO Fits done" << endl;
			TGraphAsymmErrors* resdata = mydivide(name, htrue, hall);
			TGraphAsymmErrors* resmc = mydivide(name, htruemcweights, hallmcweights);
			vector<TGraphAsymmErrors*> results;
			results.push_back(resdata);
			results.push_back(resmc);
			for(size_t i = 0 ; i < results.size() ; ++i)
			{
				results[i]->GetXaxis()->SetTitle(labelx.c_str());
				results[i]->GetYaxis()->SetTitle((string("#epsilon ") + name).c_str());
			}
			delete htrue;
			delete hall;
			delete htrueweights;
			delete hallweights;
			delete htruemcweights;
			delete hallmcweights;
			return(results);
		}

		vector<TH2D*> Get2DHist(string namex, string labelx, string namey, string labely, Test test)
		{
			UInt_t bittrue = test.passing;
			UInt_t bitfalse = test.all;

			Int_t nbinsx = bins[namex].size()-1;
			Int_t nbinsy = bins[namey].size()-1;
			TH2D* htrue = new TH2D("htmp1", "htmp1", nbinsx, &(bins[namex][0]), nbinsy, &(bins[namey][0]));
			TH2D* hall = new TH2D("htmp2", "htmp2", nbinsx, &(bins[namex][0]), nbinsy, &(bins[namey][0]));
			TH2D* htrueweights = new TH2D("htrueweights", "htrueweights", nbinsx, &(bins[namex][0]), nbinsy, &(bins[namey][0]));
			TH2D* hallweights = new TH2D("hallweights", "hallweights", nbinsx, &(bins[namex][0]), nbinsy, &(bins[namey][0]));
			TH2D* htruemcweights = new TH2D("htruemcweights", "htruemcweights", nbinsx, &(bins[namex][0]), nbinsy, &(bins[namey][0]));
			TH2D* hallmcweights = new TH2D("hallmcweights", "hallmcweights", nbinsx, &(bins[namex][0]), nbinsy, &(bins[namey][0]));
			Double_t mass;
			Double_t myweight;
			vector< vector<TTree*> > passingtrees(nbinsx);
			vector< vector<TTree*> > failingtrees(nbinsx);

			for(int bx = 0 ; bx < nbinsx ; bx++)
			{
				for(int by = 0 ; by < nbinsy ; by++)
				{
					stringstream name;
					name << "_" << bx << "_" << by;
					passingtrees[bx].push_back(new TTree(("Tree_passing" + name.str()).c_str(), ("Tree_passing" + name.str()).c_str(), 1));
					passingtrees[bx].back()->Branch("Mass", &mass, "Mass/D");
					passingtrees[bx].back()->Branch("Weight", &myweight, "Weight/D");
					failingtrees[bx].push_back(new TTree(("Tree_failing" + name.str()).c_str(), ("Tree_failing" + name.str()).c_str(), 1));
					failingtrees[bx].back()->Branch("Mass", &mass, "Mass/D");
					failingtrees[bx].back()->Branch("Weight", &myweight, "Weight/D");
				}
			}
			for(int i = 0 ; i < tree->GetEntries() ; i++)
			{
				tree->GetEntry(i);
				TVector3 VT(tpx, tpy, tpz);
				TLorentzVector PT(VT, VT.Mag());
				TVector3 VP(ppx, ppy, ppz);
				TLorentzVector PP(VP, VP.Mag());
				mass = (PT+PP).M();
				myweight = weight;
				map<string, Double_t> vals;
				vals["eta"] = PP.Eta();
				vals["pt"] = PP.Pt();
				vals["pileup"] = pileup;
				vals["run"] = run;
				if(mass > 50. && mass < 130. && eta >= bins["eta"].front() && eta < bins["eta"].back() && PP.Pt() >= bins["pt"].front() && PP.Pt() < bins["pt"].back() && pileup >= bins["pileup"].front() && pileup < bins["pileup"].back() && run >= bins["run"].front() && run < bins["run"].back())
				{
					for(int bx = 0 ; bx < nbinsx ; bx++)
					{
						for(int by = 0 ; by < nbinsy ; by++)
						{
							if(vals[namex] >= bins[namex][bx] && vals[namex] < bins[namex][bx+1] && vals[namey] >= bins[namey][by] && vals[namey] < bins[namey][by+1])
							{
								if((tpresult&bittrue) == bittrue)
								{
									passingtrees[bx][by]->Fill();
									htrueweights->Fill(vals[namex], vals[namey], weight);
								}
								else if((tpresult&bitfalse) == bitfalse)
								{
									failingtrees[bx][by]->Fill();
									hallweights->Fill(vals[namex], vals[namey], weight);
								}
								break;
							}
						}
					}
				}
			}

			vector< vector<TTree*> > passingmctrees(nbinsx);
			vector< vector<TTree*> > failingmctrees(nbinsx);
			if(mctree != 0)
			{
				cout << "OTTO Loading MC-shape ..." << endl;
				for(int bx = 0 ; bx < nbinsx ; bx++)
				{
					for(int by = 0 ; by < nbinsy ; by++)
					{
						stringstream name;
						name << "_" << bx << "_" << by;
						passingmctrees[bx].push_back(new TTree(("MCTree_passing" + name.str()).c_str(), ("MCTree_passing" + name.str()).c_str(), 1));
						passingmctrees[bx].back()->Branch("Mass", &mass, "Mass/D");
						passingmctrees[bx].back()->Branch("Weight", &myweight, "Weight/D");
						failingmctrees[bx].push_back(new TTree(("MCTree_failing" + name.str()).c_str(), ("MCTree_failing" + name.str()).c_str(), 1));
						failingmctrees[bx].back()->Branch("Mass", &mass, "Mass/D");
						failingmctrees[bx].back()->Branch("Weight", &myweight, "Weight/D");

					}
				}
				for(int i = 0 ; i < mctree->GetEntries() ; i++)
				{
					mctree->GetEntry(i);
					TVector3 VT(tpx, tpy, tpz);
					TLorentzVector PT(VT, VT.Mag());
					TVector3 VP(ppx, ppy, ppz);
					TLorentzVector PP(VP, VP.Mag());
					mass = (PT+PP).M();
					myweight = weight;
					map<string, Double_t> vals;
					vals["eta"] = PP.Eta();
					vals["pt"] = PP.Pt();
					vals["pileup"] = pileup;
					vals["run"] = run;
					if(mass > 50 && mass < 130 && eta >= bins["eta"].front() && eta < bins["eta"].back() && PP.Pt() >= bins["pt"].front() && PP.Pt() < bins["pt"].back() && pileup >= bins["pileup"].front() && pileup < bins["pileup"].back() && run >= bins["run"].front() && run < bins["run"].back())
					{
						if((tpresult&(1<<22)) == 0) continue;
						for(int bx = 0 ; bx < nbinsx ; bx++)
						{
							for(int by = 0 ; by < nbinsy ; by++)
							{
								if(vals[namex] >= bins[namex][bx] && vals[namex] < bins[namex][bx+1] && vals[namey] >= bins[namey][by] && vals[namey] < bins[namey][by+1])
								{
									if((tpresult&bittrue) == bittrue)
									{
										passingmctrees[bx][by]->Fill();
										htruemcweights->Fill(vals[namex], vals[namey], weight);
									}
									else if((tpresult&bitfalse) == bitfalse)
									{
										hallmcweights->Fill(vals[namex], vals[namey], weight);
										failingmctrees[bx][by]->Fill();
									}
									break;
								}
							}
						}
					}
				}
			}

			Double_t fBKG, fBKGErr, fSIG, fSIGErr;
			Double_t tBKG, tBKGErr, tSIG, tSIGErr;
			myfitterpassing.Init("landauA");
			myfitterfailing.Init("landauA");
			for(int bx = 0 ; bx < nbinsx ; bx++)
			{
				myfitterpassing.Reset();
				myfitterfailing.Reset();
				for(int by = 0 ; by < nbinsy ; by++)
				{
					stringstream fitname;
					fitname << test.label << "_" << namex << "_" << bx << "_" << namey << " " << by;
					tSIG = htrueweights->GetBinContent(bx+1, by+1);
					tSIGErr = 0.1;
					myfitterpassing.GetFit(passingtrees[bx][by], fitname.str()+"_passing", tSIG, tSIGErr, tBKG, tBKGErr, passingmctrees[bx][by]);
					htrue->SetBinContent(bx+1, by+1, tSIG);	
					htrue->SetBinError(bx+1, by+1, tSIGErr);	
					delete passingtrees[bx][by];
					delete passingmctrees[bx][by];
					fSIG = hallweights->GetBinContent(bx+1, by+1);
					fSIGErr = 0.2;
					double chi2 =  myfitterfailing.GetFit(failingtrees[bx][by], fitname.str()+"_failing", fSIG, fSIGErr, fBKG, fBKGErr, failingmctrees[bx][by]);
					cout << "OTTO CHI2: " << chi2 << endl;
					//if(chi2 > 30)
					//{
					//	for(double frac = 0.3 ; frac < 1. ; frac+=0.2)
					//	{
					//		fSIG = hallweights->GetBinContent(bx+1, by+1);
					//		fSIGErr = frac;
					//		cout << "OTTO CHI2: " << chi2 << " " << frac << endl;
					//		chi2 =  myfitterfailing.GetFit(failingtrees[bx][by], fitname.str()+"_failing", fSIG, fSIGErr, fBKG, fBKGErr, failingmctrees[bx][by]);
					//		if(chi2 < 30) break;
					//	}

					//}
					hall->SetBinContent(bx+1, by+1, fSIG);	
					hall->SetBinError(bx+1, by+1, fSIGErr);	
					delete failingtrees[bx][by];
					delete failingmctrees[bx][by];
				}
			}
			vector<TH2D*> result = mydivide(name, htrue, hall);
			vector<TH2D*> resultMC = mydivide(name+"MC", htruemcweights, hallmcweights);
			result.insert(result.end(), resultMC.begin(), resultMC.end());
			for(size_t i = 0 ; i < result.size() ; i++)
			{
				result[i]->GetXaxis()->SetTitle(labelx.c_str());
				result[i]->GetYaxis()->SetTitle(labely.c_str());
			}
			delete htrue;
			delete hall;
			delete htrueweights;
			delete hallweights;
			delete htruemcweights;
			delete hallmcweights;
			return(result);
		}
};


Int_t main(int argc, char** argv)
{
	if(argc != 3) {exit(-1);}
	string inputfile(argv[1]);
	string typ(argv[2]);

        gStyle->SetPadTopMargin(0.05);
        gStyle->SetPadRightMargin(0.05);
        gStyle->SetPadBottomMargin(0.1);
        gStyle->SetPadLeftMargin(0.15);
        gStyle->SetOptTitle(0);
        gStyle->SetOptStat(0);
        gStyle->SetOptFit(0);

	TPAnalyse anadata(inputfile.c_str(), "TPMU/TPTree");
	anadata.SetMCShape("out_1.root", "TPMU/TPTree");
	//anadata.SetMCShape("/home/oh057574/MyAnalysis/JOBS8_jan22/TPMM4_1/HIST_DYmad.root", "TPMuon_A/TPTree");
	TFile* fileout = new TFile(("Fit_"+typ+".root").c_str(), "RECREATE");
	TFile* fileschrott = new TFile("schrott.root", "RECREATE");
	fileschrott->cd();

	can = new TCanvas("can","can",600,600);

	vector<Double_t> ptbins;
	ptbins.push_back(20);
	ptbins.push_back(30);
	ptbins.push_back(40);
	ptbins.push_back(50);
	ptbins.push_back(250);

	vector<Double_t> etabins;
	etabins.push_back(-2.4);
	etabins.push_back(-2.1);
	etabins.push_back(-1.6);
	etabins.push_back(-1.2);
	etabins.push_back(-0.6);
	etabins.push_back(0.0);
	etabins.push_back(0.6);
	etabins.push_back(1.2);
	etabins.push_back(1.6);
	etabins.push_back(2.1);
	etabins.push_back(2.4);

	vector<Double_t> ptbins1d;
	ptbins1d.push_back(20);
	ptbins1d.push_back(22);
	ptbins1d.push_back(24);
	ptbins1d.push_back(26);
	ptbins1d.push_back(28);
	ptbins1d.push_back(30);
	ptbins1d.push_back(40);
	ptbins1d.push_back(50);
	ptbins1d.push_back(70);
	ptbins1d.push_back(100);
	ptbins1d.push_back(250);
	ptbins1d.push_back(10000);

	vector<Double_t> etabins1d;
	etabins1d.push_back(-2.4);
	etabins1d.push_back(-2.1);
	etabins1d.push_back(-1.6);
	etabins1d.push_back(-1.2);
	etabins1d.push_back(-0.9);
	etabins1d.push_back(-0.6);
	etabins1d.push_back(-0.3);
	etabins1d.push_back(-0.2);
	etabins1d.push_back(0.0);
	etabins1d.push_back(0.2);
	etabins1d.push_back(0.3);
	etabins1d.push_back(0.6);
	etabins1d.push_back(0.9);
	etabins1d.push_back(1.2);
	etabins1d.push_back(1.6);
	etabins1d.push_back(2.1);
	etabins1d.push_back(2.4);

	vector<Double_t> isoetabins;
	isoetabins.push_back(-2.1);
	isoetabins.push_back(-1.6);
	isoetabins.push_back(-1.2);
	isoetabins.push_back(-0.9);
	isoetabins.push_back(-0.6);
	isoetabins.push_back(-0.3);
	isoetabins.push_back(-0.2);
	isoetabins.push_back(0.0);
	isoetabins.push_back(0.2);
	isoetabins.push_back(0.3);
	isoetabins.push_back(0.6);
	isoetabins.push_back(0.9);
	isoetabins.push_back(1.2);
	isoetabins.push_back(1.6);
	isoetabins.push_back(2.1);

	vector<Double_t> pileupbins;
	pileupbins.push_back(1.);
	pileupbins.push_back(3.);
	pileupbins.push_back(4.);
	pileupbins.push_back(5.);
	pileupbins.push_back(6.);
	pileupbins.push_back(7.);
	pileupbins.push_back(8.);
	pileupbins.push_back(9.);
	pileupbins.push_back(10.);
	pileupbins.push_back(11.);
	pileupbins.push_back(12.);
	pileupbins.push_back(13.);
	pileupbins.push_back(14.);
	pileupbins.push_back(15.);
	pileupbins.push_back(17.);
	pileupbins.push_back(20.);
	pileupbins.push_back(24.);
	pileupbins.push_back(300.);

	vector<Double_t> runbins;
	runbins.push_back(0);
	//runbins.push_back(193687);
	runbins.push_back(10000000000);

	UInt_t TRK = 1<<0;
	UInt_t SAgTRK = 1<<1;
	UInt_t SA = 1<<2;
	UInt_t TRKgSA = 1<<3;
	UInt_t ID = 1<<4;
	UInt_t ISO = 1<<5;
	UInt_t TR_ISOMu24 = 1<<6;
	UInt_t TR_Mu17Mu8_L1 = 1<<7;
	UInt_t TR_Mu17Mu8_L1gt10 = 1<<8;
	UInt_t TR_Mu17Mu8_L2DMF = 1<<9;
	UInt_t TR_Mu17Mu8_L2SMF = 1<<10;
	UInt_t TR_Mu17Mu8_L3DMF = 1<<11;
	UInt_t TR_Mu17Mu8_L3SMF = 1<<12;
	UInt_t TR_ISOMu24_test = 1<<15;
	UInt_t TR_Mu17Mu8_L1_test = 1<<16;
	UInt_t TR_Mu17Mu8_L1gt10_test = 1<<17;
	UInt_t TR_Mu17Mu8_L2DMF_test = 1<<18;
	UInt_t TR_Mu17Mu8_L2SMF_test = 1<<19;
	UInt_t TR_Mu17Mu8_L3DMF_test = 1<<20;
	UInt_t TR_Mu17Mu8_L3SMF_test = 1<<21;
	//UInt_t DZtest = 1<<13;
	//UInt_t DZpos = 1<<14;

	vector<Test> tests;
	if(typ == "SA") tests.push_back(Test("SA", SAgTRK|TRK, TRK));
	if(typ == "TRK") tests.push_back(Test("TRK", TRKgSA|SA, SA));
	if(typ == "ID") tests.push_back(Test("ID", ID|SAgTRK|TRK, SAgTRK|TRK));
	if(typ == "ISO") tests.push_back(Test("ISO", ISO|ID|SAgTRK|TRK, ID|SAgTRK|TRK));
	if(typ == "TOT") tests.push_back(Test("TOT", ISO|ID|SAgTRK|TRK, TRK));
	if(typ == "ISOTRG") tests.push_back(Test("ISOTRG", ISO|ID|TR_ISOMu24, ID|ISO|TR_ISOMu24_test));
	if(typ == "DMF") tests.push_back(Test("DMF", ISO|ID|TR_Mu17Mu8_L3DMF, ID|ISO|TR_Mu17Mu8_L3DMF_test));
	if(typ == "SMF") tests.push_back(Test("SMF", ISO|ID|TR_Mu17Mu8_L3SMF, ID|ISO|TR_Mu17Mu8_L3SMF_test));
	if(typ == "SMFgDMF") tests.push_back(Test("SMFgDMF", ISO|ID|TR_Mu17Mu8_L3SMF|TR_Mu17Mu8_L3DMF, ID|ISO|TR_Mu17Mu8_L3DMF|TR_Mu17Mu8_L3SMF_test));
	if(typ == "L2DMF") tests.push_back(Test("L2DMF", ISO|ID|TR_Mu17Mu8_L2DMF, ID|ISO|TR_Mu17Mu8_L2DMF_test));
	if(typ == "L2SMF") tests.push_back(Test("L2SMF", ISO|ID|TR_Mu17Mu8_L2SMF, ID|ISO|TR_Mu17Mu8_L2SMF_test));
	if(typ == "L1") tests.push_back(Test("L1", ISO|ID|TR_Mu17Mu8_L1, ID|ISO|TR_Mu17Mu8_L1_test));
	if(typ == "L1gt10") tests.push_back(Test("L1gt10", ISO|ID|TR_Mu17Mu8_L1gt10, ID|ISO|TR_Mu17Mu8_L1gt10_test));

	for(UInt_t i = 0 ; i < tests.size() ; i++)
	{
		anadata.SetBins("pt", ptbins1d);
		anadata.SetBins("eta", etabins1d);
		anadata.SetBins("pileup", pileupbins);
		anadata.SetBins("run", runbins);
		//if(typ == "ISOTRG") {anadata.SetBins("eta", isoetabins);}
		vector<TGraphAsymmErrors*> dataeffpt = anadata.Get1DHist("pt", "p_{T} [GeV]", tests[i]);
		vector<TGraphAsymmErrors*> dataeffeta = anadata.Get1DHist("eta", "#eta", tests[i]);
		//vector<TGraphAsymmErrors*> dataeffvertex = anadata.Get1DHist("pileup", "#Vertices", tests[i]);
		//anadata.SetBins("pt", ptbins);
		//anadata.SetBins("eta", etabins);
		//vector<TH2D*> dataeffetapt = anadata.Get2DHist("eta", "#eta", "pt", "p_{T} [GeV]", tests[i]);
		fileout->cd();
		dataeffpt[0]->Write((tests[i].label+string("_Pt")).c_str());
		dataeffpt[1]->Write((tests[i].label+string("_Pt_MC")).c_str());
		dataeffeta[0]->Write((tests[i].label+string("_Eta")).c_str());
		dataeffeta[1]->Write((tests[i].label+string("_Eta_MC")).c_str());
		//dataeffvertex[0]->Write((tests[i].label+string("_Vertex")).c_str());
		//dataeffvertex[1]->Write((tests[i].label+string("_Vertex_MC")).c_str());
		//dataeffetapt[0]->Write((tests[i].label+string("_Eta_Pt")).c_str());
		//dataeffetapt[1]->Write((tests[i].label+string("_Eta_Pt_ERRP")).c_str());
		//dataeffetapt[2]->Write((tests[i].label+string("_Eta_Pt_ERRM")).c_str());
		//dataeffetapt[3]->Write((tests[i].label+string("_Eta_Pt_MC")).c_str());
		fileschrott->cd();
	}
	fileout->Write();
	fileout->Close();
	fileschrott->Close();
}

