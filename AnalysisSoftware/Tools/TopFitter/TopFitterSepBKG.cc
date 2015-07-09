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


int main(int argc, char** argv)
{
TCanvas* can = new TCanvas("can", "can", 2000, 1600);

string infilename(argv[1]);

TFile* infile = TFile::Open(infilename.c_str());

TRandom3 rand;

TFile* outfile = new TFile("fithists.root", "recreate");

int rebin = 0;
int interpolate = 0;
	RooRealVar x("x","x", 0, -12, 12);
	TH1D* ghright = (static_cast<TH2D*>(infile->Get("TRUTH/right_imp_test_ptthad")))->ProjectionX("right");
	ghright->Rebin(rebin);
	TH1D* ghwrong = (static_cast<TH2D*>(infile->Get("TRUTH/wrong_imp_test_ptthad")))->ProjectionX("wrong");
	ghwrong->Rebin(rebin);
	TH1D* ghsemi = (static_cast<TH2D*>(infile->Get("TRUTH/semi_imp_test_ptthad")))->ProjectionX("semi");
	ghsemi->Rebin(rebin);
	TH1D* ghother = (static_cast<TH2D*>(infile->Get("TRUTH/other_imp_test_ptthad")))->ProjectionX("other");
	ghother->Rebin(rebin);
	RooDataHist gright("ghistright", "ghistright", x, ghright);
	RooDataHist gwrong("ghistwrong", "ghistwrong", x, ghwrong);
	RooDataHist gsemi("ghistsemi", "ghistsemi", x, ghsemi);
	RooDataHist gother("ghistother", "ghistother", x, ghother);

	RooHistPdf gpdf_right("gpdf_right", "gpdf_right", x, gright, interpolate);
	RooHistPdf gpdf_wrong("gpdf_wrong", "gpdf_wrong", x, gwrong, interpolate);
	RooHistPdf gpdf_semi("gpdf_semi", "gpdf_semi", x, gsemi, interpolate);
	RooHistPdf gpdf_other("gpdf_other", "gpdf_other", x, gother, interpolate);

for(int i = 0 ; i < 11 ; ++i)
{

	int binstart = i*10;
	int binend = i*10+10;
	stringstream ss;
	ss << i;

	TH1D* hdata = (static_cast<TH2D*>(infile->Get("RECO/all_imp_test_ptthad")))->ProjectionX(("data_" +ss.str()).c_str(), binstart, binend);
	hdata->Rebin(rebin);
	for(int b = 1 ; b <= hdata->GetNbinsX() ; b++)
	{
		int n = rand.Poisson(hdata->GetBinContent(b)*806.*5000./25246680.);
		hdata->SetBinContent(b, n);
		hdata->SetBinError(b, Sqrt(n));
	}	

	TH1D* hright = (static_cast<TH2D*>(infile->Get("TRUTH/right_imp_test_ptthad")))->ProjectionX(("right_" +ss.str()).c_str(), binstart, binend);
	hright->Rebin(rebin);
	TH1D* hwrong = (static_cast<TH2D*>(infile->Get("TRUTH/wrong_imp_test_ptthad")))->ProjectionX(("wrong_" +ss.str()).c_str(), binstart, binend);
	hwrong->Rebin(rebin);
	TH1D* hsemi = (static_cast<TH2D*>(infile->Get("TRUTH/semi_imp_test_ptthad")))->ProjectionX(("semi_" +ss.str()).c_str(), binstart, binend);
	hsemi->Rebin(rebin);
	TH1D* hother = (static_cast<TH2D*>(infile->Get("TRUTH/other_imp_test_ptthad")))->ProjectionX(("other_" +ss.str()).c_str(), binstart, binend);
	hother->Rebin(rebin);

	double Ndata = hdata->Integral();

	RooDataHist data("histdata", "histdata", x, hdata);
	RooDataHist right("histright", "histright", x, hright);
	RooDataHist wrong("histwrong", "histwrong", x, hwrong);
	RooDataHist semi("histsemi", "histsemi", x, hsemi);
	RooDataHist other("histother", "histother", x, hother);

	RooHistPdf pdf_right("pdf_right", "pdf_right", x, right, interpolate);
	RooHistPdf pdf_wrong("pdf_wrong", "pdf_wrong", x, wrong, interpolate);
	RooHistPdf pdf_semi("pdf_semi", "pdf_semi", x, semi, interpolate);
	RooHistPdf pdf_other("pdf_other", "pdf_other", x, other, interpolate);


	RooRealVar gm("gm", "gm", 170., 100., 300.);
	RooRealVar gs("gs", "gs", 15, 0, 30.);
	//RooRealVar gs("gs", "gs", 18.);
	RooGaussian gauss("gauss", "gauss", x, gm, gs);

	RooRealVar lm("lm", "lm", 120., 100., 300.);
	RooRealVar ls("ls", "ls", 30, 0, 100.);
	RooLandau landau("landau", "landau", x, lm, ls);

	RooRealVar nright("nright", "nright", Ndata*0.25, 0., 2.*Ndata);
	RooRealVar nwrong("nwrong", "nwrong", Ndata*0.25, 0., 2.*Ndata);
	RooRealVar nsemi("nsemi", "nsemi", Ndata*0.25, 0., 2.*Ndata);
	RooRealVar nother("nother", "nother", Ndata*0.25, 0., 2.*Ndata);

	RooRealVar gnright("gnright", "gnright", Ndata*0.25, 0., 2.*Ndata);
	RooRealVar gnwrong("gnwrong", "gnwrong", Ndata*0.25, 0., 2.*Ndata);
	RooRealVar gnsemi("gnsemi", "gnsemi", Ndata*0.25, 0., 2.*Ndata);
	RooRealVar gnother("gnother", "gnother", Ndata*0.25, 0., 2.*Ndata);

	RooAddPdf model("model", "model", RooArgList(pdf_right, pdf_wrong, pdf_semi, pdf_other), RooArgList(nright, nwrong, nsemi, nother));
	RooAddPdf gmodel("gmodel", "gmodel", RooArgList(gpdf_right, gpdf_wrong, gpdf_semi, gpdf_other), RooArgList(gnright, gnwrong, gnsemi, gnother));

	RooFitResult* fitresult = model.fitTo(data, Minos(true), PrintLevel(-1), Save());
	RooFitResult* gfitresult = gmodel.fitTo(data, Minos(true), PrintLevel(-1), Save());

	cout << "OTTO " << i << ": " << hright->Integral()*806.*5000./25246680. << " " << nright.getVal() <<"+-" << nright.getError() << " " << gnright.getVal() << "+-" << gnright.getError() << endl;
	cout << "OTTO " << i << ": " << hwrong->Integral()*806.*5000./25246680. << " " << nwrong.getVal() <<"+-" << nwrong.getError() << " " << gnwrong.getVal() << "+-" << gnwrong.getError() << endl;
	cout << "OTTO " << i << ": " << hsemi->Integral()*806.*5000./25246680. << " " << nsemi.getVal() <<"+-" << nsemi.getError() << " " << gnsemi.getVal() << "+-" << gnsemi.getError() << endl;
	cout << "OTTO " << i << ": " << hother->Integral()*806.*5000./25246680. << " " << nother.getVal() <<"+-" << nother.getError() << " " << gnother.getVal() << "+-" << gnother.getError() << endl;
	//cout << "OTTO " << i << ": " << hright->Integral() << " " << nright.getVal() << " " << gnright.getVal() << endl;

	RooPlot* xframe = x.frame() ;
	data.plotOn(xframe, MarkerColor(kBlack));
	model.plotOn(xframe);
	model.plotOn(xframe, Components(pdf_right), LineColor(kRed), LineStyle(4), Name("psig"));
	model.plotOn(xframe, Components(RooArgSet(pdf_wrong, pdf_semi, pdf_other)), LineColor(kBlue), LineStyle(4), Name("pbkg"));
	data.plotOn(xframe, MarkerColor(kBlack), MarkerStyle(20), MarkerSize(2), Name("pdata"));

	xframe->Draw();

	can->SaveAs(("pic_"+ss.str()+".png").c_str());

	RooPlot* gxframe = x.frame() ;
	data.plotOn(gxframe, MarkerColor(kBlack));
	gmodel.plotOn(gxframe);
	gmodel.plotOn(gxframe, Components(gpdf_right), LineColor(kRed), LineStyle(4), Name("gpsig"));
	gmodel.plotOn(gxframe, Components(RooArgSet(gpdf_wrong, gpdf_semi, gpdf_other)), LineColor(kBlue), LineStyle(4), Name("gpbkg"));
	data.plotOn(gxframe, MarkerColor(kBlack), MarkerStyle(20), MarkerSize(2), Name("pdata"));

	gxframe->Draw();

	can->SaveAs(("gpic_"+ss.str()+".png").c_str());
}


infile->Close();

outfile->Write();
outfile->Close();

}
