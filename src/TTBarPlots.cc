#include <TTBarPlots.h>
#include <URStreamer.h>
#include <Permutation.h>
#include <ttbarxsec.h>

#include <sstream>

using namespace std;
using namespace TMath;

TTBarPlots::TTBarPlots(string prefix) : TTBarPlotsBase(prefix)
{

}

TTBarPlots::~TTBarPlots()
{

}

void TTBarPlots::Init(ttbar* analysis)
{
	TTBarPlotsBase::Init(analysis);
	int ta = 40.;
	double tamin = 6.;
	double tamax = 18.;
	int tb = 40.;
	//double tbmin = -20.;
	//double tbmax = 10.;
	double tbmin = 0.;
	double tbmax = 80.;
	plot1d.AddHist("MET", 500, 0, 2000, "MET", "Events");
	plot1d.AddHist("njets", 15, 0, 15, "n-jets", "Events");
	plot1d.AddHist("ptaddjets", 200, 0, 400, "p_{T}(add. jets) [GeV]", "Events");
	plot1d.AddHist("DRminWjets", 200, 0, 10, "#DeltaR_{min W-jet)", "Events");
	plot1d.AddHist("DRminbjets", 200, 0, 10, "#DeltaR_{min b-jet)", "Events");
	plot1d.AddHist("DPhiMET_Nu", 100, 0, 3, "#Delta#Phi(#nu, MET)", "Events");
	plot2d.AddHist("METvsDPhiMET_Nu", 120, 0, 1200, 100, 0, 3, "MET [GeV]", "#Delta#Phi(#nu, MET)");
	plot2d.AddHist("METvsChi", 120, 0, 1200, 25, 0., 100., "MET [GeV]", "#chi");
	for(int jn : jetbins)
	{
		stringstream jb;
		if(jn != -1) jb << jn << "_";
		plot2d.AddHist("test_"+jb.str()+"thadpt", ta, tamin, tamax, an->topptbins, "test", "p_{T}(t_{had}) [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"thadpt", tb, tbmin, tbmax, an->topptbins, "testb", "p_{T}(t_{had}) [GeV]");
		plot2d.AddHist("test_"+jb.str()+"nobin", ta, tamin, tamax, an->nobins, "test", "all");
		plot2d.AddHist("testb_"+jb.str()+"nobin", tb, tbmin, tbmax, an->nobins, "testb", "all");
		plot2d.AddHist("test_"+jb.str()+"tleppt", ta, tamin, tamax, an->topptbins, "test", "p_{T}(t_{lep}) [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"tleppt", tb, tbmin, tbmax, an->topptbins, "testb", "p_{T}(t_{lep}) [GeV]");
		plot2d.AddHist("test_"+jb.str()+"thady", ta, tamin, tamax, an->topybins, "test", "|y(t_{had})|");
		plot2d.AddHist("testb_"+jb.str()+"thady", tb, tbmin, tbmax, an->topybins, "testb", "|y(t_{had})|");
		plot2d.AddHist("test_"+jb.str()+"tlepy", ta, tamin, tamax, an->topybins, "test", "|y(t_{lep})|");
		plot2d.AddHist("testb_"+jb.str()+"tlepy", tb, tbmin, tbmax, an->topybins, "testb", "|y(t_{lep})|");
		plot2d.AddHist("test_"+jb.str()+"ttm", ta, tamin, tamax, an->ttmbins, "test", "M(tt) [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"ttm", tb, tbmin, tbmax, an->ttmbins, "testb", "M(tt) [GeV]");
		plot2d.AddHist("test_"+jb.str()+"tty", ta, tamin, tamax, an->ttybins, "test", "y(tt)");
		plot2d.AddHist("testb_"+jb.str()+"tty", tb, tbmin, tbmax, an->ttybins, "testb", "y(tt)");
		plot2d.AddHist("test_"+jb.str()+"ttpt", ta, tamin, tamax, an->ttptbins, "test", "p_{T}(tt) [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"ttpt", tb, tbmin, tbmax, an->ttptbins, "testb", "p_{T}(tt) [GeV]");
		plot2d.AddHist("test_"+jb.str()+"costhetastar", ta, tamin, tamax, 10, -1., 1., "test", "cos(#Theta*)");
		plot2d.AddHist("testb_"+jb.str()+"costhetastar", tb, tbmin, tbmax, 10, -1., 1., "testb", "cos(#Theta*)");
		plot2d.AddHist("test_"+jb.str()+"njet", ta, tamin, tamax, an->jetbins, "test", "n-jets");
		plot2d.AddHist("testb_"+jb.str()+"njet", tb, tbmin, tbmax, an->jetbins, "testb", "n-jets");
		plot2d.AddHist("test_"+jb.str()+"met", ta, tamin, tamax, an->metbins, "test", "MET [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"met", tb, tbmin, tbmax, an->metbins, "testb", "MET [GeV]");
		plot2d.AddHist("test_"+jb.str()+"testb", ta, tamin, tamax, tb, tbmin, tbmax, "test", "testb");
	}
}

void TTBarPlots::Fill(Permutation& per, int lepcharge, double weight)
{
	TLorentzVector nu(per.Nu());
	TTBarPlotsBase::Fill(per.BHad(), per.WJa(), per.WJb(), per.BLep(), per.L(), &nu, lepcharge, weight);
	double test = per.MassDiscr();
	//double testb = per.BDiscr();
	double testb = Sqrt(per.NuChisq());
	//double testb = (*per.BLep() + *per.L()).Mt(); 
	if(test == numeric_limits<double>::max()) {test = 0; testb = 0;}
	plot1d["MET"]->Fill(an->met.Pt(), weight);
	plot1d["njets"]->Fill(an->cleanedjets.size()-4, weight);
	double drminw = 100.;
	double drminb = 100.;
	for(size_t j = 0 ; j < an->cleanedjets.size() ; ++j)
	{
		if(per.IsJetIn(an->cleanedjets[j])) continue;
		if(drminw > per.WJa()->DeltaR(*an->cleanedjets[j])) {drminw = per.WJa()->DeltaR(*an->cleanedjets[j]);}
		if(drminw > per.WJb()->DeltaR(*an->cleanedjets[j])) {drminw = per.WJb()->DeltaR(*an->cleanedjets[j]);}
		if(drminb > per.BHad()->DeltaR(*an->cleanedjets[j])) {drminb = per.BHad()->DeltaR(*an->cleanedjets[j]);}
		if(drminb > per.BLep()->DeltaR(*an->cleanedjets[j])) {drminb = per.BLep()->DeltaR(*an->cleanedjets[j]);}
		plot1d["ptaddjets"]->Fill(an->cleanedjets[j]->Pt(), weight);
	}
	plot1d["DRminWjets"]->Fill(drminw, weight);
	plot1d["DRminbjets"]->Fill(drminb, weight);
	plot2d["METvsChi"]->Fill(an->met.Pt(), testb, weight);
	plot1d["DPhiMET_Nu"]->Fill(Abs(nu.DeltaPhi(an->met)), weight);
	plot2d["METvsDPhiMET_Nu"]->Fill(an->met.Pt(), Abs(nu.DeltaPhi(an->met)), weight);
	for(int jn : jetbins)
	{
		stringstream jb;
		if(jn != -1) jb << jn << "_";
		if((jn == -1) || (an->reducedjets.size() - 4 == size_t(jn)) || (jn == jetbins.back() && an->reducedjets.size() - 4 > size_t(jn)))
		{
			plot2d["test_"+jb.str()+"testb"]->Fill(test, testb, weight);
			plot2d["test_"+jb.str()+"thadpt"]->Fill(test, thad.Pt(), weight);
			plot2d["testb_"+jb.str()+"thadpt"]->Fill(testb, thad.Pt(), weight);
			plot2d["test_"+jb.str()+"nobin"]->Fill(test, thad.Pt(), weight);
			plot2d["testb_"+jb.str()+"nobin"]->Fill(testb, thad.Pt(), weight);
			plot2d["test_"+jb.str()+"tleppt"]->Fill(test, tlep.Pt(), weight);
			plot2d["testb_"+jb.str()+"tleppt"]->Fill(testb, tlep.Pt(), weight);
			plot2d["test_"+jb.str()+"thady"]->Fill(test, Abs(thad.Rapidity()), weight);
			plot2d["testb_"+jb.str()+"thady"]->Fill(testb, Abs(thad.Rapidity()), weight);
			plot2d["test_"+jb.str()+"tlepy"]->Fill(test, Abs(tlep.Rapidity()), weight);
			plot2d["testb_"+jb.str()+"tlepy"]->Fill(testb, Abs(tlep.Rapidity()), weight);
			plot2d["test_"+jb.str()+"ttm"]->Fill(test, tt.M(), weight);
			plot2d["testb_"+jb.str()+"ttm"]->Fill(testb, tt.M(), weight);
			plot2d["test_"+jb.str()+"tty"]->Fill(test, Abs(tt.Rapidity()), weight);
			plot2d["testb_"+jb.str()+"tty"]->Fill(testb, Abs(tt.Rapidity()), weight);
			plot2d["test_"+jb.str()+"ttpt"]->Fill(test, tt.Pt(), weight);
			plot2d["testb_"+jb.str()+"ttpt"]->Fill(testb, tt.Pt(), weight);
			plot2d["test_"+jb.str()+"costhetastar"]->Fill(test, tCMS.CosTheta(), weight);
			plot2d["testb_"+jb.str()+"costhetastar"]->Fill(testb, tCMS.CosTheta(), weight);
			plot2d["test_"+jb.str()+"njet"]->Fill(test, an->cleanedjets.size()-4, weight);
			plot2d["testb_"+jb.str()+"njet"]->Fill(testb, an->cleanedjets.size()-4, weight);
			plot2d["test_"+jb.str()+"met"]->Fill(test, an->met.Pt(), weight);
			plot2d["testb_"+jb.str()+"met"]->Fill(testb, an->met.Pt(), weight);
		}
	}
}


