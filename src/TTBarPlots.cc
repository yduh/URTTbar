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
	double tbmax = 150.;
	plot1d.AddHist("MET", 500, 0, 2000, "MET", "Events");
	plot1d.AddHist("njets", 15, 0, 15, "additional jets", "Events");
	plot1d.AddHist("ptaddjets", 200, 0, 400, "p_{T}(add. jets) [GeV]", "Events");
	plot1d.AddHist("DRminWjets", 200, 0, 10, "#DeltaR_{min W-jet}", "Events");
	plot1d.AddHist("DRminbjets", 200, 0, 10, "#DeltaR_{min b-jet}", "Events");
	plot1d.AddHist("DPhiMET_Nu", 100, 0, 3, "#Delta#Phi(#nu, MET)", "Events");
	plot2d.AddHist("METvsDPhiMET_Nu", 120, 0, 1200, 100, 0, 3, "MET [GeV]", "#Delta#Phi(#nu, MET)");
	plot2d.AddHist("METvsChi", 120, 0, 1200, 25, 0., 100., "MET [GeV]", "#chi");
	plot1d.AddHist("Mt_W", 500, 0, 500, "M_{t}(W) [GeV]", "Events");
	plot2d.AddHist("METunc", 100, 0, 0.5, 100, 0., .5, "#sigma(MET_{x})/MET_{x}", "#sigma(MET_{y})/MET_{y}");
	for(int jn : jetbins)
	{
		stringstream jb;
		if(jn != -1) jb << jn << "_";
		plot2d.AddHist("test_"+jb.str()+"testb", ta, tamin, tamax, tb, tbmin, tbmax, "-log(#lambda_{m})", "D_{#nu,min} [GeV]");
		plot2d.AddHist("test_"+jb.str()+"nobin", ta, tamin, tamax, an->nobins, "-log(#lambda_{m})", "all");
		plot2d.AddHist("testb_"+jb.str()+"nobin", tb, tbmin, tbmax, an->nobins, "D_{#nu,min} [GeV]", "all");
		plot2d.AddHist("test_"+jb.str()+"tpt", ta, tamin, tamax, an->topptbins, "-log(#lambda_{m})", "p_{T}(t) [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"tpt", tb, tbmin, tbmax, an->topptbins, "D_{#nu,min} [GeV]", "p_{T}(t) [GeV]");
		plot2d.AddHist("test_"+jb.str()+"ty", ta, tamin, tamax, an->topybins, "-log(#lambda_{m})", "|y(t)|");
		plot2d.AddHist("testb_"+jb.str()+"ty", tb, tbmin, tbmax, an->topybins, "D_{#nu,min} [GeV]", "|y(t)|");
		plot2d.AddHist("test_"+jb.str()+"thadpt", ta, tamin, tamax, an->topptbins, "-log(#lambda_{m})", "p_{T}(t_{h}) [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"thadpt", tb, tbmin, tbmax, an->topptbins, "D_{#nu,min} [GeV]", "p_{T}(t_{h}) [GeV]");
		plot2d.AddHist("test_"+jb.str()+"thady", ta, tamin, tamax, an->topybins, "-log(#lambda_{m})", "|y(t_{h})|");
		plot2d.AddHist("testb_"+jb.str()+"thady", tb, tbmin, tbmax, an->topybins, "D_{#nu,min} [GeV]", "|y(t_{h})|");
		plot2d.AddHist("test_"+jb.str()+"tleppt", ta, tamin, tamax, an->topptbins, "-log(#lambda_{m})", "p_{T}(t_{l}) [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"tleppt", tb, tbmin, tbmax, an->topptbins, "D_{#nu,min} [GeV]", "p_{T}(t_{l}) [GeV]");
		plot2d.AddHist("test_"+jb.str()+"tlepy", ta, tamin, tamax, an->topybins, "-log(#lambda_{m})", "|y(t_{l})|");
		plot2d.AddHist("testb_"+jb.str()+"tlepy", tb, tbmin, tbmax, an->topybins, "D_{#nu,min} [GeV]", "|y(t_{l})|");
		plot2d.AddHist("test_"+jb.str()+"ttm", ta, tamin, tamax, an->ttmbins, "-log(#lambda_{m})", "M(tt) [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"ttm", tb, tbmin, tbmax, an->ttmbins, "D_{#nu,min} [GeV]", "M(tt) [GeV]");
		plot2d.AddHist("test_"+jb.str()+"tty", ta, tamin, tamax, an->ttybins, "-log(#lambda_{m})", "y(tt)");
		plot2d.AddHist("testb_"+jb.str()+"tty", tb, tbmin, tbmax, an->ttybins, "D_{#nu,min} [GeV]", "y(tt)");
		plot2d.AddHist("test_"+jb.str()+"ttpt", ta, tamin, tamax, an->ttptbins, "-log(#lambda_{m})", "p_{T}(tt) [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"ttpt", tb, tbmin, tbmax, an->ttptbins, "D_{#nu,min} [GeV]", "p_{T}(tt) [GeV]");
		plot2d.AddHist("test_"+jb.str()+"costhetastar", ta, tamin, tamax, 10, -1., 1., "-log(#lambda_{m})", "cos(#Theta*)");
		plot2d.AddHist("testb_"+jb.str()+"costhetastar", tb, tbmin, tbmax, 10, -1., 1., "D_{#nu,min} [GeV]", "cos(#Theta*)");
		plot2d.AddHist("test_"+jb.str()+"njet", ta, tamin, tamax, an->jetbins, "-log(#lambda_{m})", "additional jets");
		plot2d.AddHist("testb_"+jb.str()+"njet", tb, tbmin, tbmax, an->jetbins, "D_{#nu,min} [GeV]", "additional jets");
		plot2d.AddHist("test_"+jb.str()+"met", ta, tamin, tamax, an->metbins, "-log(#lambda_{m})", "MET [GeV]");
		plot2d.AddHist("testb_"+jb.str()+"met", tb, tbmin, tbmax, an->metbins, "D_{#nu,min} [GeV]", "MET [GeV]");
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
	double Mt_W = Sqrt(2.*an->met.Pt()*per.L()->Pt()-2.*(an->met.Px()*per.L()->Px() + an->met.Py()*per.L()->Py()));
	plot1d["Mt_W"]->Fill(Mt_W, weight);
	plot2d["METunc"]->Fill(an->met.pxunctot()/an->met.Px(), an->met.pyunctot()/an->met.Py(), weight);
	for(int jn : jetbins)
	{
		stringstream jb;
		if(jn != -1) jb << jn << "_";
		if((jn == -1) || (an->reducedjets.size() - 4 == size_t(jn)) || (jn == jetbins.back() && an->reducedjets.size() - 4 > size_t(jn)))
		{
			plot2d["test_"+jb.str()+"testb"]->Fill(test, testb, weight);
			plot2d["test_"+jb.str()+"nobin"]->Fill(test, thad.Pt(), weight);
			plot2d["testb_"+jb.str()+"nobin"]->Fill(testb, thad.Pt(), weight);
			double thadpt = Min(thad.Pt(), plot2d["test_"+jb.str()+"thadpt"]->GetYaxis()->GetXmax()-.0001);
			plot2d["test_"+jb.str()+"thadpt"]->Fill(test, thadpt, weight);
			plot2d["testb_"+jb.str()+"thadpt"]->Fill(testb, thadpt, weight);
			double tleppt = Min(tlep.Pt(), plot2d["test_"+jb.str()+"tleppt"]->GetYaxis()->GetXmax()-.0001);
			plot2d["test_"+jb.str()+"tleppt"]->Fill(test, tleppt, weight);
			plot2d["testb_"+jb.str()+"tleppt"]->Fill(testb, tleppt, weight);
			plot2d["test_"+jb.str()+"tpt"]->Fill(test, tleppt, weight);
			plot2d["testb_"+jb.str()+"tpt"]->Fill(testb, tleppt, weight);
			plot2d["test_"+jb.str()+"tpt"]->Fill(test, thadpt, weight);
			plot2d["testb_"+jb.str()+"tpt"]->Fill(testb, thadpt, weight);
			double thady = Min(Abs(thad.Rapidity()), plot2d["test_"+jb.str()+"thady"]->GetYaxis()->GetXmax()-.0001);
			plot2d["test_"+jb.str()+"thady"]->Fill(test, thady, weight);
			plot2d["testb_"+jb.str()+"thady"]->Fill(testb, thady, weight);
			double tlepy = Min(Abs(tlep.Rapidity()), plot2d["test_"+jb.str()+"tlepy"]->GetYaxis()->GetXmax()-.0001);
			plot2d["test_"+jb.str()+"tlepy"]->Fill(test, tlepy, weight);
			plot2d["testb_"+jb.str()+"tlepy"]->Fill(testb, tlepy, weight);
			plot2d["test_"+jb.str()+"ty"]->Fill(test, tlepy, weight);
			plot2d["testb_"+jb.str()+"ty"]->Fill(testb, tlepy, weight);
			plot2d["test_"+jb.str()+"ty"]->Fill(test, thady, weight);
			plot2d["testb_"+jb.str()+"ty"]->Fill(testb, thady, weight);
			double ttm = Min(tt.M(), plot2d["test_"+jb.str()+"ttm"]->GetYaxis()->GetXmax()-.0001);
			plot2d["test_"+jb.str()+"ttm"]->Fill(test, ttm, weight);
			plot2d["testb_"+jb.str()+"ttm"]->Fill(testb, ttm, weight);
			double tty = Min(Abs(tt.Rapidity()), plot2d["test_"+jb.str()+"tty"]->GetYaxis()->GetXmax()-.0001);
			plot2d["test_"+jb.str()+"tty"]->Fill(test, tty, weight);
			plot2d["testb_"+jb.str()+"tty"]->Fill(testb, tty, weight);
			double ttpt = Min(tt.Pt(), plot2d["test_"+jb.str()+"ttpt"]->GetYaxis()->GetXmax()-.0001);
			plot2d["test_"+jb.str()+"ttpt"]->Fill(test, ttpt, weight);
			plot2d["testb_"+jb.str()+"ttpt"]->Fill(testb, ttpt, weight);
			plot2d["test_"+jb.str()+"costhetastar"]->Fill(test, tCMS.CosTheta(), weight);
			plot2d["testb_"+jb.str()+"costhetastar"]->Fill(testb, tCMS.CosTheta(), weight);
			plot2d["test_"+jb.str()+"njet"]->Fill(test, an->cleanedjets.size()-4, weight);
			plot2d["testb_"+jb.str()+"njet"]->Fill(testb, an->cleanedjets.size()-4, weight);
			plot2d["test_"+jb.str()+"met"]->Fill(test, an->met.Pt(), weight);
			plot2d["testb_"+jb.str()+"met"]->Fill(testb, an->met.Pt(), weight);
		}
	}
}


