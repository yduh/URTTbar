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
		UInt_t type;


	public:
		TMatrixD matrix;
		double ma = 0.;
		double mb = 0.;
		double papb = 0;
		double mm = 0;
		int n = 0;
		TH1D* hright = 0;
		TH1D* hwrong = 0;
		TH1D* hmeas = 0;

		BTagEffReader(int bins, double min, double max) : matrix(100, 2)
		{
			hright = new TH1D("right", "right", bins, min, max);
			hwrong = new TH1D("wrong", "wrong", bins, min, max);
			hmeas = new TH1D("meas", "meas", bins, min, max);
		}

		void AddFile(string filename, double scale)
		{
			TFile* f = TFile::Open(filename.c_str());
			TTree* tree = dynamic_cast<TTree*>(f->Get("TRUTH/jetscale"));
			tree->SetBranchAddress("minp", minp);	
			tree->SetBranchAddress("maxp", maxp);	
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
					if(gRandom->Uniform() > 0.5)
					{
						pb.SetPxPyPzE(minp[0], minp[1], minp[2], minp[3]);
						pa.SetPxPyPzE(maxp[0], maxp[1], maxp[2], maxp[3]);
					}
					if(pa.Pt() > jmindown && pa.Pt() < jminup && pb.Pt() > jmaxdown && pb.Pt() < jmaxup)
					{
						if((Bmin && Abs(pa.Eta()) < 1.4) || (!Bmin && Abs(pa.Eta()) > 1.4))
						{
							if((Bmax && Abs(pb.Eta()) < 1.4) || (!Bmax && Abs(pb.Eta()) > 1.4))
							{
								double mass = (pa+pb).M();
								if(type == 1)
								{
									papb += pa * pb;
									ma += pa.M();
									mb += pb.M();
									mm+=mass;
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
					//pa = pa * (1.05);
					//pb = pb * (1.05);
					if(pa.Pt() > jmindown && pa.Pt() < jminup && pb.Pt() > jmaxdown && pb.Pt() < jmaxup)
					{
						if((Bmin && Abs(pa.Eta()) < 1.4) || (!Bmin && Abs(pa.Eta()) > 1.4))
						{
							if((Bmax && Abs(pb.Eta()) < 1.4) || (!Bmax && Abs(pb.Eta()) > 1.4))
							{
								double mass = (pa+pb).M();
								hmeas->Fill(mass, scale);
							}
						}
					}

				}				
			}
		}	

};


int main(int argc, char** argv)
{
	double lumi = 4000; 
	double TTscale = lumi*806./25246993.;
	double Wscale = lumi*61524./10017462.;
	double Zscale = lumi*6024./2499142.;
	double Tscale = lumi*35.6/986100.;
	double Tbarscale = lumi*35.6/971800.;

	int interpolate = 0;
	int nexp = 100;
	TCanvas* can = new TCanvas("can", "can", 2000, 1600);

	int bins = 40;
	double xmin = 40.;
	double xmax = 120.;
	BTagEffReader reader(bins, xmin, xmax);
	reader.AddFile("ttJets.root", TTscale);
	//reader.AddFile("WJets.root", Wscale);
	//reader.AddFile("ZJets.root", Zscale);
	reader.AddFile("singlet.root", Tscale);
	reader.AddFile("singletbar.root", Tbarscale);

	BTagEffReader readerdata(bins, xmin, xmax);
	string datadir("");
	readerdata.AddFile((datadir+"ttJets.root").c_str(), TTscale);
	//readerdata.AddFile((datadir+"WJets.root").c_str(), Wscale);
	//readerdata.AddFile((datadir+"ZJets.root").c_str(), Zscale);
	readerdata.AddFile((datadir+"singlet.root").c_str(), Tscale);
	readerdata.AddFile((datadir+"singletbar.root").c_str(), Tbarscale);

	TRandom3 rand;

	
	//vector<double> wjbin = {25., 30., 40., 55., 150.};
    //vector<double> wjbin = {25., 50., 80., 120., 250.};
	vector<double> wjbin = {35., 40.,70, 100., 250.};

	TFile* infileorig = new TFile("scalefithists_orig.root", "read");
	TFile* infiledata = new TFile("scalefithists.root", "read");

	TMatrix M((wjbin.size()-1)*(wjbin.size()-1), wjbin.size()-1);
	TMatrix S((wjbin.size()-1)*(wjbin.size()-1), 1);

	for(size_t i = 1 ; i < wjbin.size() ; ++i)
	{
		for(size_t j = 1 ; j < wjbin.size() ; ++j)
		{
			
			stringstream ssp;
			ssp << i << "_" << j <<  "_shift";
			TH1D* horig = (TH1D*)infileorig->Get(ssp.str().c_str());
			TH1D* hdata = (TH1D*)infiledata->Get(ssp.str().c_str());
			cout << ssp.str().c_str() << horig << endl;
			double morig = horig->GetMean();
			double mdata = hdata->GetMean();
			cout << morig << " OTTO " << mdata << endl; 

			reader.Fill(wjbin[i-1], wjbin[i], wjbin[j-1], wjbin[j]);
			readerdata.Fill(wjbin[i-1], wjbin[i], wjbin[j-1], wjbin[j]);
			double ma = reader.ma/readerdata.n;
			double mb = reader.mb/readerdata.n;
			double papb = reader.papb/readerdata.n;
			double mm = reader.mm/readerdata.n;
			int nmes = (i-1)*(wjbin.size()-1)+(j-1);
			cout << ma << " " << mb << " " << papb << " " << mm << endl;
			M(nmes, i-1)+=2.*(ma*ma+papb);
			M(nmes, j-1)+=2.*(mb*mb+papb);
			cout << nmes << " " << i-1 << " " << j-1 << endl; 
			S(nmes, 0) = mdata*mdata - morig*morig;
			//S(nmes, 0) = (mm+mdata)*(mm+mdata) - mm*mm;
			//M(nmes, i-1)+=1.;
			//M(nmes, j-1)+=1.;
			//S(nmes, 0) = (mdata*mdata)/(morig*morig)-1.;
			//TMatrixD res(100, 1);
			//for(int m = 0 ; m < 100 ; ++m)
			//{
			//	res(m,0) = mdata*mdata - morig*morig;
			//}

			//TMatrixD MR(readerdata.matrix, TMatrixD::kTransposeMult, res);
			//TMatrixD ML(readerdata.matrix, TMatrixD::kTransposeMult, readerdata.matrix);

			//MR.Print();
			//ML.Print();

			//ML.Invert();
			//ML.Print();
			//TMatrixD F(ML, TMatrixD::kMult, MR);
			//F.Print();

		}
	}

M.Print();
S.Print();
TMatrixD L(M, TMatrixD::kTransposeMult, M);
TMatrixD R(M, TMatrixD::kTransposeMult, S);
L.Invert();
TMatrixD F(L, TMatrixD::kMult, R);
F.Print();

}
