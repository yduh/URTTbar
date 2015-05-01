#include <TTBarPlots.h>
#include <URStreamer.h>
#include <Permutation.h>
#include <ttbarxsec.h>

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
	int ta = 60.;
	double tamin = 0.;
	double tamax = 12.;
	int tb = 60.;
	double tbmin = -20.;
	double tbmax = 10.;

    plot2d.AddHist("test_ptthad", ta, tamin, tamax, an->topptbins, "test", "p_{T}(t_{had}) [GeV]");
    plot2d.AddHist("testb_ptthad", tb, tbmin, tbmax, an->topptbins, "testb", "p_{T}(t_{had}) [GeV]");
    plot2d.AddHist("test_pttlep", ta, tamin, tamax, an->topptbins, "test", "p_{T}(t_{lep}) [GeV]");
    plot2d.AddHist("testb_pttlep", tb, tbmin, tbmax, an->topptbins, "testb", "p_{T}(t_{lep}) [GeV]");
    plot2d.AddHist("test_etathad", ta, tamin, tamax, an->topetabins, "test", "#eta(t_{had})");
    plot2d.AddHist("testb_etathad", tb, tbmin, tbmax, an->topetabins, "testb", "#eta(t_{had})");
    plot2d.AddHist("test_etatlep", ta, tamin, tamax, an->topetabins, "test", "#eta(t_{lep})");
    plot2d.AddHist("testb_etatlep", tb, tbmin, tbmax, an->topetabins, "testb", "#eta(t_{lep})");
    plot2d.AddHist("test_ttm", ta, tamin, tamax, an->ttmbins, "test", "M(tt) [GeV]");
    plot2d.AddHist("testb_ttm", tb, tbmin, tbmax, an->ttmbins, "testb", "M(tt) [GeV]");
    plot2d.AddHist("test_tty", ta, tamin, tamax, an->ttybins, "test", "y(tt)");
    plot2d.AddHist("testb_tty", tb, tbmin, tbmax, an->ttybins, "testb", "y(tt)");
    plot2d.AddHist("test_ttpt", ta, tamin, tamax, an->ttptbins, "test", "p_{T}(tt) [GeV]");
    plot2d.AddHist("testb_ttpt", tb, tbmin, tbmax, an->ttptbins, "testb", "p_{T}(tt) [GeV]");
    plot2d.AddHist("test_costhetastar", ta, tamin, tamax, 10, -1., 1., "test", "cos(#Theta*)");
    plot2d.AddHist("testb_costhetastar", tb, tbmin, tbmax, 10, -1., 1., "testb", "cos(#Theta*)");
    plot2d.AddHist("test_njet", ta, tamin, tamax, 20, 0., 20., "test", "n-jets");
    plot2d.AddHist("testb_njet", tb, tbmin, tbmax, 20, 0., 20., "testb", "n-jets");
    plot2d.AddHist("test_met", ta, tamin, tamax, an->metbins, "test", "MET [GeV]");
    plot2d.AddHist("testb_met", tb, tbmin, tbmax, an->metbins, "testb", "MET [GeV]");
    plot2d.AddHist("test_testb", ta, tamin, tamax, tb, tbmin, tbmax, "test", "testb");
}

void TTBarPlots::Fill(Permutation& per, int lepcharge, double weight)
{
	TLorentzVector nu(per.Nu());
	TTBarPlotsBase::Fill(per.BHad(), per.WJa(), per.WJb(), per.BLep(), per.L(), &nu, lepcharge, weight);
	double test = per.MassDiscr();
	double testb = per.BDiscr();
    plot2d["test_testb"]->Fill(test, testb, weight);
    plot2d["test_ptthad"]->Fill(test, thad.Pt(), weight);
    plot2d["testb_ptthad"]->Fill(testb, thad.Pt(), weight);
	plot2d["test_pttlep"]->Fill(test, tlep.Pt(), weight);
	plot2d["testb_pttlep"]->Fill(testb, tlep.Pt(), weight);
    plot2d["test_etathad"]->Fill(test, Abs(thad.Eta()), weight);
    plot2d["testb_etathad"]->Fill(testb, Abs(thad.Eta()), weight);
	plot2d["test_etatlep"]->Fill(test, Abs(tlep.Eta()), weight);
	plot2d["testb_etatlep"]->Fill(testb, Abs(tlep.Eta()), weight);
	plot2d["test_ttm"]->Fill(test, tt.M(), weight);
	plot2d["testb_ttm"]->Fill(testb, tt.M(), weight);
	plot2d["test_tty"]->Fill(test, Abs(tt.Rapidity()), weight);
	plot2d["testb_tty"]->Fill(testb, Abs(tt.Rapidity()), weight);
	plot2d["test_ttpt"]->Fill(test, tt.Pt(), weight);
	plot2d["testb_ttpt"]->Fill(testb, tt.Pt(), weight);
	plot2d["test_costhetastar"]->Fill(test, tCMS.CosTheta(), weight);
	plot2d["testb_costhetastar"]->Fill(testb, tCMS.CosTheta(), weight);
	plot2d["test_njet"]->Fill(test, an->reducedjets.size(), weight);
	plot2d["testb_njet"]->Fill(test, an->reducedjets.size(), weight);
	plot2d["test_met"]->Fill(test, an->met.Pt(), weight);
	plot2d["testb_met"]->Fill(test, an->met.Pt(), weight);

}


