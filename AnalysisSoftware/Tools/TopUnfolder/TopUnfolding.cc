#include <TFile.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TROOT.h>
#include <TDirectory.h>
#include <TRandom3.h>
#include <TMath.h>

#include <RooUnfoldResponse.h>
#include <RooUnfoldBayes.h>
#include <RooUnfoldDagostini.h>
#include <RooUnfoldSvd.h>
#include <RooUnfoldInvert.h>

#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>

#include "helper.h"
#include "ConfigParser.h"


using namespace std;
using namespace TMath;


void cleanhist(TH1D* hist)
{
	for(int i = 1 ; i <= hist->GetNbinsX() ; ++i)
	{
		hist->SetBinContent(i, 0.);
		hist->SetBinError(i, 0.);
	}
}

class TopUnfold
{
	private:
		TH2D* hmatrix = 0;
		TH1D* htruth = 0;
		TH1D* hmeasured = 0;

		TH2D* hmatrix_gen = 0;
		TH1D* hreco_gen = 0;
		TH1D* hmiss_gen = 0;
		TH1D* htruth_gen = 0;
		TH1D* hfake_gen = 0;

		TH1D* gentruth = 0;
		TH1D* genmeasured = 0;
		double maxval = 0;

		RooUnfoldResponse* response = 0;
		int unfoldingparam;

		//RooUnfoldBayes* unfold;
		RooUnfold* unfold;
		int njet;
		string snjet;
	public:
		TopUnfold(string filename, int njet_, string distribution, double scale) : unfoldingparam(2), unfold(0), njet(njet_)
	{
		if(njet != -1){ stringstream ss; ss << "_" << njet_; snjet = ss.str();}
		TDirectory* dir = gDirectory;
		TFile* file = TFile::Open(filename.c_str());
		gROOT->cd();
		hmatrix = new TH2D(*dynamic_cast<TH2D*>(file->Get(("TRUTH/response/" + distribution + "_matrix" + snjet).c_str())));	
		htruth = new TH1D(*dynamic_cast<TH1D*>(file->Get(("TRUTH/response/" + distribution + "_truth" + snjet).c_str())));	
		hmeasured = new TH1D(*dynamic_cast<TH1D*>(file->Get(("TRUTH/response/" + distribution + "_reco" + snjet).c_str())));
		cout << hmatrix->Integral() << " " << htruth->Integral() << " " << hmeasured->Integral() << endl;
		file->Close();
		response = new RooUnfoldResponse(hmeasured, htruth, hmatrix, "response", "response");
		//hmatrix->Scale(scale);
		//htruth->Scale(scale);
		//hmeasured->Scale(scale);
		dir->cd();
	}
		TopUnfold(string filename, int njet_, string distribution, double scale, TH1D* hnjet) : unfoldingparam(2), unfold(0), njet(njet_)
	{
		if(njet != -1){ stringstream ss; ss << "_" << njet_; snjet = ss.str();}
		TDirectory* dir = gDirectory;
		TFile* file = TFile::Open(filename.c_str());
		gROOT->cd();
		vector<TH1D*> vreco(4);
		vector<TH2D*> vmatrix(4);
		vector<TH1D*> vtruth(4);
		double norm = 0.;
		for(int j = 0 ; j < 4 ; ++j)
		{
			stringstream jet;
			jet << "_" << j;
			vreco[j] = dynamic_cast<TH1D*>(file->Get(("TRUTH/response/" + distribution + "_reco" + jet.str()).c_str()));
			if(j == 0){norm = vreco[j]->Integral();}
			double scale = hnjet->GetBinContent(j+1)/vreco[j]->Integral()*norm; 
			cout << scale << endl;
			vmatrix[j] = dynamic_cast<TH2D*>(file->Get(("TRUTH/response/" + distribution + "_matrix" + jet.str()).c_str()));	
			vtruth[j] = dynamic_cast<TH1D*>(file->Get(("TRUTH/response/" + distribution + "_truth" + jet.str()).c_str()));	
			vreco[j]->Scale(scale);
			vmatrix[j]->Scale(scale);
			vtruth[j]->Scale(scale);
		}

		hmeasured = new TH1D(*vreco[0]);
		hmatrix = new TH2D(*vmatrix[0]);	
		htruth = new TH1D(*vtruth[0]);	
		for(int j = 1 ; j < 4 ; ++j)
		{
			hmatrix->Add(vmatrix[j]);
			htruth->Add(vtruth[j]);
			hmeasured->Add(vreco[j]);
		}
		cout << hmatrix->Integral() << " " << htruth->Integral() << " " << hmeasured->Integral() << endl;
		file->Close();
		response = new RooUnfoldResponse(hmeasured, htruth, hmatrix, "response", "response");
		//hmatrix->Scale(scale);
		//htruth->Scale(scale);
		//hmeasured->Scale(scale);
		dir->cd();
	}

		void SetUnfoldingParam(int param) {unfoldingparam = param;}


		~TopUnfold()
		{
		}

		TH1D* GetUnfolded(TH1D* tounfold)
		{
			TDirectory* dir = gDirectory;
			gROOT->cd();
			if(unfold != 0) {delete unfold;}
			//RooUnfoldDagostini unfold(response, tounfold, unfoldingparam);
			unfold = new RooUnfoldBayes(response, tounfold, unfoldingparam);
			//unfold = new RooUnfoldDagostini(response, tounfold, unfoldingparam);
			//unfold = new RooUnfoldInvert(response, tounfold);
			//unfold = new RooUnfoldDagostini(response, tounfold, unfoldingparam);
			//unfold = new RooUnfoldInvert(response, tounfold);
			//RooUnfoldInvert unfold(response, tounfold);
			dir->cd();
			//unfold.Ereco().Print();
			return(dynamic_cast<TH1D*>(unfold->Hreco(RooUnfold::kErrors)));
		}

		TH2D* GetCorrelationMatrix()
		{
			if(unfold == 0){return(0);}
			TDirectory* dir = gDirectory;
			gROOT->cd();
			int ncols = unfold->Ereco().GetNcols();
			int nrows = unfold->Ereco().GetNrows();
			TH2D* cm = new TH2D("correlation", "correlation", ncols, 0, ncols, nrows, 0, nrows);
			for(int x = 0 ; x < ncols ; ++x)
			{
				for(int y = 0 ; y < ncols ; ++y)
				{
					cm->SetBinContent(x+1, y+1, unfold->Ereco()(x,y));
				}
			}
			dir->cd();
			return(cm);

		}
		
		void SetGenFile(string filename, string distribution, double scale)
		{
			TDirectory* dir = gDirectory;
			TFile* file = TFile::Open(filename.c_str());
			gROOT->cd();
			//hmatrix_gen = new TH2D(*dynamic_cast<TH2D*>(file->Get(("TRUTH/truth_response_" + distribution + "_matrix").c_str())));	
			//hmiss_gen = new TH1D(*dynamic_cast<TH1D*>(file->Get(("TRUTH/truth_response_" + distribution + "_truth").c_str())));	
			//hfake_gen = new TH1D(*dynamic_cast<TH1D*>(file->Get(("TRUTH/truth_response_" + distribution + "_measured").c_str())));
			hmatrix_gen = new TH2D(*dynamic_cast<TH2D*>(file->Get(("TRUTH/response/" + distribution + "_matrix" + snjet).c_str())));	
			htruth_gen = new TH1D(*dynamic_cast<TH1D*>(file->Get(("TRUTH/response/" + distribution + "_truth" + snjet).c_str())));	
			hmiss_gen = new TH1D(*dynamic_cast<TH1D*>(file->Get(("TRUTH/response/" + distribution + "_truth" + snjet).c_str())));	
			hreco_gen = new TH1D(*dynamic_cast<TH1D*>(file->Get(("TRUTH/response/" + distribution + "_reco" + snjet).c_str())));
			hfake_gen = new TH1D(*dynamic_cast<TH1D*>(file->Get(("TRUTH/response/" + distribution + "_reco" + snjet).c_str())));
			hmatrix_gen->Scale(scale);
			htruth_gen->Scale(scale);
			hmiss_gen->Scale(scale);
			hreco_gen->Scale(scale);
			hfake_gen->Scale(scale);
			file->Close();
			hmiss_gen->Add(hmatrix_gen->ProjectionX(), -1.);
			hfake_gen->Add(hmatrix_gen->ProjectionY(), -1.);
			gentruth = new TH1D(*htruth_gen);
			genmeasured = new TH1D(*hreco_gen);
			dir->cd();
			vector<double> maxima(3);
			maxima[0] = hmiss_gen->GetMaximum();
			maxima[1] = hfake_gen->GetMaximum();
			maxima[2] = hmatrix_gen->GetMaximum();
			sort(maxima.begin(), maxima.end());
			maxval = maxima[2]*1.1;

		}

		TH1D* GetHTruth() const {return(htruth_gen);}
		TH1D* GetHMeasured() const {return(hreco_gen);}

		void Generate(int numevents)
		{
			cleanhist(gentruth);
			cleanhist(genmeasured);
			int nev = gRandom->Poisson(numevents);
			int nbinsx = hmiss_gen->GetNbinsX();
			int nbinsy = hfake_gen->GetNbinsX();
			while(nev != 0)
			{
				int binx = gRandom->Integer(nbinsx+1);
				int biny = gRandom->Integer(nbinsy+1);
				double test = gRandom->Uniform(maxval);
				if(binx > 0 && biny > 0)
				{
					double dest = hmatrix_gen->GetBinContent(binx, biny);
					if(test < dest)
					{
						nev--;
						gentruth->SetBinContent(binx, gentruth->GetBinContent(binx)+1);
						genmeasured->SetBinContent(biny, genmeasured->GetBinContent(biny)+1);
					}
					continue;
				}
				if(binx == 0 && biny > 0)
				{
					double dest = hfake_gen->GetBinContent(biny);
					if(test < dest)
					{
						nev--;
						genmeasured->SetBinContent(biny, genmeasured->GetBinContent(biny)+1);
					}
					continue;
				}
				if(biny == 0 && binx > 0)
				{
					double dest = hmiss_gen->GetBinContent(binx);
					if(test < dest)
					{
						gentruth->SetBinContent(binx, gentruth->GetBinContent(binx)+1);
					}
					continue;
				}

			}
			for(int i = 1 ; i <= nbinsx ; ++i)
			{
				gentruth->SetBinError(i, Sqrt(gentruth->GetBinContent(i)));
			}
			for(int i = 1 ; i <= nbinsy ; ++i)
			{
				genmeasured->SetBinError(i, Sqrt(genmeasured->GetBinContent(i)));
			}
		}


		TH1D* StatTestA(TH1D* hist, int numexp, TDirectory* location, string name)
		{
			TDirectory* dir = gDirectory;
			location->cd();
			TH1D* diff = new TH1D(*hist);
			diff->SetName(("stattest_" + name).c_str());
			TH1D* diffq = new TH1D(*hist);
			vector<TH1D*> diffbins;
			for(int i = 0 ; i < hreco_gen->GetNbinsX() ; ++i)
			{
				stringstream hname;
				hname << name << "_" << i;
				diffbins.push_back(new TH1D(hname.str().c_str(), hname.str().c_str(), 200, -1., 1.));
			}
			dir->cd();

			cleanhist(diff);
			cleanhist(diffq);

			for(int exp = 0 ; exp < numexp ; ++exp)
			{
				for(int b = 1 ; b < hist->GetNbinsX() ; ++b)
				{
					genmeasured->SetBinContent(b, gRandom->Gaus(hist->GetBinContent(b), hist->GetBinError(b)));
				}
				TH1D* genunfolded = GetUnfolded(genmeasured);
				cout << exp << " " << gentruth->Integral() << " " << genunfolded->Integral() << " " << genmeasured->Integral() << endl;
				for(int b = 1 ; b <= genunfolded->GetNbinsX() ; ++b)
				{
					diffbins[b-1]->Fill((genunfolded->GetBinContent(b)-gentruth->GetBinContent(b))/gentruth->GetBinContent(b));
					diff->SetBinContent(b, diff->GetBinContent(b) + genunfolded->GetBinContent(b));
					diffq->SetBinContent(b, diffq->GetBinContent(b) + genunfolded->GetBinContent(b)*genunfolded->GetBinContent(b));
				}
			}

			for(int b = 1 ; b <= diff->GetNbinsX() ; ++b)
			{
				double mean = diff->GetBinContent(b)/numexp;
				double meanq = diffq->GetBinContent(b)/numexp;
				diff->SetBinContent(b, mean);
				diff->SetBinError(b, Sqrt(meanq-mean*mean));
			}

			delete diffq;
			return(diff);
		}

		TH1D* StatTest(int numexp, TDirectory* location, string name)
		{
			TDirectory* dir = gDirectory;
			location->cd();
			TH1D* diff = new TH1D(*genmeasured);
			TH1D* diffq = new TH1D(*genmeasured);
			vector<TH1D*> diffbins;
			for(int i = 0 ; i < hreco_gen->GetNbinsX() ; ++i)
			{
				stringstream hname;
				hname << name << "_" << i;
				diffbins.push_back(new TH1D(hname.str().c_str(), hname.str().c_str(), 200, -1., 1.));
			}
			dir->cd();

			cleanhist(diff);
			cleanhist(diffq);
			int numevents = hreco_gen->Integral();

			for(int exp = 0 ; exp < numexp ; ++exp)
			{
				Generate(numevents);
				TH1D* genunfolded = GetUnfolded(genmeasured);
				cout << exp << " " << gentruth->Integral() << " " << genunfolded->Integral() << " " << genmeasured->Integral() << " " << numevents << endl;
				for(int b = 1 ; b <= genunfolded->GetNbinsX() ; ++b)
				{
					double bdiff = genunfolded->GetBinContent(b) - gentruth->GetBinContent(b);
					diffbins[b-1]->Fill(bdiff/gentruth->GetBinContent(b));
					diff->SetBinContent(b, diff->GetBinContent(b) + bdiff);
					diffq->SetBinContent(b, diffq->GetBinContent(b) + bdiff*bdiff);
					//diff->SetBinContent(b, diff->GetBinContent(b)+genunfolded->GetBinContent(b));
				}
			}

			for(int b = 1 ; b <= diff->GetNbinsX() ; ++b)
			{
				double mean = diff->GetBinContent(b)/numexp;
				double meanq = diffq->GetBinContent(b)/numexp;
				diff->SetBinContent(b, mean/htruth_gen->GetBinContent(b));
				diff->SetBinError(b, Sqrt(meanq-mean*mean)/htruth_gen->GetBinContent(b));
			//	diff->SetBinContent(b, diff->GetBinContent(b)/numexp/hreco_gen->GetBinContent(b));
			//	diff->SetBinError(b, Sqrt(diff->GetBinContent(b))/numexp/hreco_gen->GetBinContent(b));
			}
			
			delete diffq;
			return(diff);

		}		


};

Int_t main(int argc, char** argv)
{
	string hist(argv[1]);
	string datafile(argv[2]);
	int njet = -1;
	if(argc == 4)
	{
		istringstream(argv[3]) >> njet;
	}
	cout << "NJET: " << njet << endl;
    ConfigParser CP("Unfolding.cfg");
    ConfigParser CPscale("SCALE.cfg");
	string responsefile(CP.Get<string>("ResponseFile")); 
	string MCtestfile(CP.Get<string>("MCTestFile")); 
    double lumi = CPscale.Get<double>("lumi");
	double TTscale = lumi*CPscale.Get<double>("tt_xsec")/CPscale.Get<double>(FNbody(responsefile)+"_W");

	TFile* fdata = TFile::Open(datafile.c_str());
	TopUnfold* topunfold = 0;

	if(njet == -1)
	{
		TH1D* hnjet = dynamic_cast<TH1D*>(fdata->Get("njets"));
		topunfold = new TopUnfold(responsefile, njet, hist, TTscale, hnjet);
	}
	else
	{
		topunfold = new TopUnfold(responsefile, njet, hist, TTscale);
	}
	//TH1D* hdata = dynamic_cast<TH1D*>(fdata->Get("nevents_truth"));
	
	TH1D* hdata = dynamic_cast<TH1D*>(fdata->Get("fitsum"));
	TH1D* hdata_unfolded = topunfold->GetUnfolded(hdata);
	TH2D* cm = topunfold->GetCorrelationMatrix();
	double TTscaleGen = lumi*CPscale.Get<double>("tt_xsec")/CPscale.Get<double>(FNbody(MCtestfile)+"_W");
	topunfold->SetGenFile(MCtestfile, hist, TTscaleGen);

	int numexp = CP.Get<int>("nexpunfolding");
	stringstream outfilejet;
	if(njet != -1) {outfilejet << "_" << njet;}
	TFile* outfile = TFile::Open(("unfolding_"+hist+outfilejet.str()+".root").c_str(), "recreate");
	hdata->Write("data_measured");
	hdata_unfolded->Write("data_unfolded");
	cm->Write("correlation");
	topunfold->GetHTruth()->Write("mc_truth");
	topunfold->GetHMeasured()->Write("mc_measured");
	gROOT->cd();

	topunfold->SetUnfoldingParam(1);
	topunfold->StatTestA(hdata, numexp, outfile, "iterations_1");
	topunfold->SetUnfoldingParam(2);
	topunfold->StatTestA(hdata, numexp, outfile, "iterations_2");
	topunfold->SetUnfoldingParam(3);
	topunfold->StatTestA(hdata, numexp, outfile, "iterations_3");
	topunfold->SetUnfoldingParam(4);
	topunfold->StatTestA(hdata, numexp, outfile, "iterations_4");
	topunfold->SetUnfoldingParam(5);
	topunfold->StatTestA(hdata, numexp, outfile, "iterations_5");
	topunfold->SetUnfoldingParam(6);
	topunfold->StatTestA(hdata, numexp, outfile, "iterations_6");
	topunfold->SetUnfoldingParam(7);
	topunfold->StatTestA(hdata, numexp, outfile, "iterations_7");
	topunfold->SetUnfoldingParam(8);
	topunfold->StatTestA(hdata, numexp, outfile, "iterations_8");
	topunfold->SetUnfoldingParam(9);
	topunfold->StatTestA(hdata, numexp, outfile, "iterations_9");
	topunfold->SetUnfoldingParam(10);
	topunfold->StatTestA(hdata, numexp, outfile, "iterations_10");

	//topunfold.SetUnfoldingParam(1);
	//topunfold.StatTest(numexp, outfile, "iterations_1");
	//topunfold.SetUnfoldingParam(2);
	//topunfold.StatTest(numexp, outfile, "iterations_2");
	//topunfold.SetUnfoldingParam(3);
	//topunfold.StatTest(numexp, outfile, "iterations_3");
	//topunfold.SetUnfoldingParam(4);
	//topunfold.StatTest(numexp, outfile, "iterations_4");
	//topunfold.SetUnfoldingParam(5);
	//topunfold.StatTest(numexp, outfile, "iterations_5");
	//topunfold.SetUnfoldingParam(6);
	//topunfold.StatTest(numexp, outfile, "iterations_6");
	//topunfold.SetUnfoldingParam(7);
	//topunfold.StatTest(numexp, outfile, "iterations_7");
	//topunfold.SetUnfoldingParam(8);
	//topunfold.StatTest(numexp, outfile, "iterations_8");
	//topunfold.SetUnfoldingParam(9);
	//topunfold.StatTest(numexp, outfile, "iterations_9");
	//topunfold.SetUnfoldingParam(10);
	//topunfold.StatTest(numexp, outfile, "iterations_10");

	outfile->Write();
	outfile->Close();

}
