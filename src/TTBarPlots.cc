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
	plot1d.AddHist("DPhiMET_Nu", 100, 0, 3, "#Delta#Phi(#nu, MET)", "Events");
	plot2d.AddHist("METvsDPhiMET_Nu", 120, 0, 1200, 100, 0, 3, "MET (GeV)", "#Delta#Phi(#nu, MET)");
	plot2d.AddHist("METvsChi", 120, 0, 1200, 25, 0., 100., "MET (GeV)", "#chi");
	for(int jn : jetbins)
	{
		stringstream jb;
		if(jn != -1) jb << jn << "_";
		plot2d.AddHist("test_"+jb.str()+"thadpt", ta, tamin, tamax, an->topptbins, "test", "p_{T}(t_{had}) [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"thadpt", tb, tbmin, tbmax, an->topptbins, "testb", "p_{T}(t_{had}) [GeV]");
		plot2d.AddHist("test_"+jb.str()+"tleppt", ta, tamin, tamax, an->topptbins, "test", "p_{T}(t_{lep}) [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"tleppt", tb, tbmin, tbmax, an->topptbins, "testb", "p_{T}(t_{lep}) [GeV]");
		plot2d.AddHist("test_"+jb.str()+"thadeta", ta, tamin, tamax, an->topetabins, "test", "#eta(t_{had})");
		plot2d.AddHist("testb_"+jb.str()+"thadeta", tb, tbmin, tbmax, an->topetabins, "testb", "#eta(t_{had})");
		plot2d.AddHist("test_"+jb.str()+"tlepeta", ta, tamin, tamax, an->topetabins, "test", "#eta(t_{lep})");
		plot2d.AddHist("testb_"+jb.str()+"tlepeta", tb, tbmin, tbmax, an->topetabins, "testb", "#eta(t_{lep})");
		plot2d.AddHist("test_"+jb.str()+"ttm", ta, tamin, tamax, an->ttmbins, "test", "M(tt) [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"ttm", tb, tbmin, tbmax, an->ttmbins, "testb", "M(tt) [GeV]");
		plot2d.AddHist("test_"+jb.str()+"tty", ta, tamin, tamax, an->ttybins, "test", "y(tt)");
		plot2d.AddHist("testb_"+jb.str()+"tty", tb, tbmin, tbmax, an->ttybins, "testb", "y(tt)");
		plot2d.AddHist("test_"+jb.str()+"ttpt", ta, tamin, tamax, an->ttptbins, "test", "p_{T}(tt) [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"ttpt", tb, tbmin, tbmax, an->ttptbins, "testb", "p_{T}(tt) [GeV]");
		plot2d.AddHist("test_"+jb.str()+"costhetastar", ta, tamin, tamax, 10, -1., 1., "test", "cos(#Theta*)");
		plot2d.AddHist("testb_"+jb.str()+"costhetastar", tb, tbmin, tbmax, 10, -1., 1., "testb", "cos(#Theta*)");
		plot2d.AddHist("test_"+jb.str()+"njet", ta, tamin, tamax, 20, 0., 20., "test", "n-jets");
		plot2d.AddHist("testb_"+jb.str()+"njet", tb, tbmin, tbmax, 20, 0., 20., "testb", "n-jets");
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
			plot2d["test_"+jb.str()+"tleppt"]->Fill(test, tlep.Pt(), weight);
			plot2d["testb_"+jb.str()+"tleppt"]->Fill(testb, tlep.Pt(), weight);
			plot2d["test_"+jb.str()+"thadeta"]->Fill(test, Abs(thad.Eta()), weight);
			plot2d["testb_"+jb.str()+"thadeta"]->Fill(testb, Abs(thad.Eta()), weight);
			plot2d["test_"+jb.str()+"tlepeta"]->Fill(test, Abs(tlep.Eta()), weight);
			plot2d["testb_"+jb.str()+"tlepeta"]->Fill(testb, Abs(tlep.Eta()), weight);
			plot2d["test_"+jb.str()+"ttm"]->Fill(test, tt.M(), weight);
			plot2d["testb_"+jb.str()+"ttm"]->Fill(testb, tt.M(), weight);
			plot2d["test_"+jb.str()+"tty"]->Fill(test, Abs(tt.Rapidity()), weight);
			plot2d["testb_"+jb.str()+"tty"]->Fill(testb, Abs(tt.Rapidity()), weight);
			plot2d["test_"+jb.str()+"ttpt"]->Fill(test, tt.Pt(), weight);
			plot2d["testb_"+jb.str()+"ttpt"]->Fill(testb, tt.Pt(), weight);
			plot2d["test_"+jb.str()+"costhetastar"]->Fill(test, tCMS.CosTheta(), weight);
			plot2d["testb_"+jb.str()+"costhetastar"]->Fill(testb, tCMS.CosTheta(), weight);
			plot2d["test_"+jb.str()+"njet"]->Fill(test, an->reducedjets.size(), weight);
			plot2d["testb_"+jb.str()+"njet"]->Fill(testb, an->reducedjets.size(), weight);
			plot2d["test_"+jb.str()+"met"]->Fill(test, an->met.Pt(), weight);
			plot2d["testb_"+jb.str()+"met"]->Fill(testb, an->met.Pt(), weight);
		}
	}
}


