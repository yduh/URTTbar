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
	double tbmin = 10.;
	double tbmax = 30.;
	plot1d.AddHist("MET", 500, 0, 2000, "MET", "Events");
	plot1d.AddHist("njets", 15, 0, 15, "additional jets", "Events");
	plot1d.AddHist("btag_blep", 100, 0, 1, "CSVv2", "Events");
	plot1d.AddHist("btag_bhad", 100, 0, 1, "CSVv2", "Events");
	plot1d.AddHist("DRminW", 150, 0, 15, "#DeltaR_{min W-jet}", "Events");
	plot1d.AddHist("DRminbl", 150, 0, 15, "#DeltaR_{min b-jet}", "Events");
	plot1d.AddHist("DRminbh", 150, 0, 15, "#DeltaR_{min b-jet}", "Events");
	plot1d.AddHist("DRminadd", 150, 0, 15, "#DeltaR_{min add-jet}", "Events");
	plot1d.AddHist("addjets_pt", 200, 25, 425, "add. jets p_{T} [GeV]", "Events");
	plot1d.AddHist("addjet1_pt", 200, 25, 425, "1. add. jet p_{T} [GeV]", "Events");
	plot1d.AddHist("addjet2_pt", 200, 25, 425, "2. add. jet p_{T} [GeV]", "Events");
	plot1d.AddHist("addjet3_pt", 200, 25, 425, "3. add. jet p_{T} [GeV]", "Events");
	plot1d.AddHist("addjet4_pt", 200, 25, 425, "4. add. jet p_{T} [GeV]", "Events");
	plot1d.AddHist("addjet5_pt", 200, 25, 425, "5. add. jet p_{T} [GeV]", "Events");
	plot1d.AddHist("DPhiMET_Nu", 100, 0, 3, "#Delta#Phi(#nu, MET)", "Events");
	plot2d.AddHist("METvsDPhiMET_Nu", 120, 0, 1200, 100, 0, 3, "MET [GeV]", "#Delta#Phi(#nu, MET)");
	plot2d.AddHist("METvsChi", 120, 0, 1200, 25, 0., 100., "MET [GeV]", "#chi");
	plot1d.AddHist("Mt_W", 500, 0, 500, "M_{t}(W) [GeV]", "Events");
	plot1d.AddHist("Mt_t", 500, 0, 1000, "M_{t}(t_{l}) [GeV]", "Events");
	plot1d.AddHist("mttest", 500, 0, 100, "mttest", "Events");
	plot1d.AddHist("masstest", 500, 0, 100, "-log(#lambda_{m})", "Events");
	plot1d.AddHist("nutest", 500, 0, 100, "nutest", "Events");
	plot1d.AddHist("chiqtest", 500, 0, 100, "D_{#nu,min}", "Events");
	plot1d.AddHist("massmttest", 500, 0, 100, "mttest", "Events");
	plot1d.AddHist("massnutest", 500, 0, 100, "-log(#lambda)", "Events");
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

void TTBarPlots::Fill(Permutation& per, double weight)
{
	TTBarPlotsBase::Fill(per, weight);
	double test = per.MassDiscr();
	//double testb = per.BDiscr();
	double testb = per.Prob();
	//double testb = (*per.BLep() + *per.L()).Mt(); 
	if(test == numeric_limits<double>::max()) {test = 0; testb = 0;}
	plot1d["MET"]->Fill(an->met.Pt(), weight);
	plot1d["mttest"]->Fill(per.MTDiscr(), weight);
	plot1d["masstest"]->Fill(test, weight);
	plot1d["nutest"]->Fill(per.NuDiscr(), weight);
	plot1d["chiqtest"]->Fill(Sqrt(per.NuChisq()), weight);
	plot1d["massmttest"]->Fill(per.MTDiscr()+test, weight);
	plot1d["massnutest"]->Fill(per.Prob(), weight);

	vector<IDJet*> addjets;
	for(size_t j = 0 ; j < an->cleanedjets.size() ; ++j)
	{
		if(!per.IsJetIn(an->cleanedjets[j])){addjets.push_back(an->cleanedjets[j]);}
	}
	sort(addjets.begin(), addjets.end(), [](IDJet* A, IDJet* B){return A->Pt() > B->Pt();});
	
	plot1d["njets"]->Fill(addjets.size(), weight);
	double drminwa = 100000.;
	double drminwb = 100000.;
	double drminbl = 100000.;
	double drminbh = 100000.;
	double drminadd = 100000.;
	for(size_t j = 0 ; j < addjets.size() ; ++j)
	{
		double ktscale = Min(per.WJa()->Pt(), addjets[j]->Pt())*per.WJa()->DeltaR(*addjets[j])/per.WJa()->Pt();
		if(drminwa > ktscale) {drminwa = ktscale;}
		ktscale = Min(per.WJb()->Pt(), addjets[j]->Pt())*per.WJb()->DeltaR(*addjets[j])/per.WJb()->Pt();
		if(drminwb > ktscale) {drminwb = ktscale;}
		ktscale = Min(per.BHad()->Pt(), addjets[j]->Pt())*per.BHad()->DeltaR(*addjets[j])/per.BHad()->Pt();
		if(drminbh > ktscale) {drminbh = ktscale;}
		ktscale = Min(per.BLep()->Pt(), addjets[j]->Pt())*per.BLep()->DeltaR(*addjets[j])/per.BLep()->Pt();
		if(drminbl > ktscale) {drminbl = ktscale;}
		if(j != 0)
		{

			ktscale = addjets[j]->Pt()*addjets[0]->DeltaR(*addjets[j])/addjets[0]->Pt();
			if(drminadd > ktscale) {drminadd = ktscale;}
		}
		plot1d["addjets_pt"]->Fill(addjets[j]->Pt(), weight);
		if(j == 0){plot1d["addjet1_pt"]->Fill(addjets[j]->Pt(), weight);}
		if(j == 1){plot1d["addjet2_pt"]->Fill(addjets[j]->Pt(), weight);}
		if(j == 2){plot1d["addjet3_pt"]->Fill(addjets[j]->Pt(), weight);}
		if(j == 3){plot1d["addjet4_pt"]->Fill(addjets[j]->Pt(), weight);}
		if(j == 4){plot1d["addjet5_pt"]->Fill(addjets[j]->Pt(), weight);}
	}
	plot1d["btag_blep"]->Fill(dynamic_cast<IDJet*>(per.BLep())->csvIncl(), weight);
	plot1d["btag_bhad"]->Fill(dynamic_cast<IDJet*>(per.BHad())->csvIncl(), weight);
	plot1d["DRminW"]->Fill(drminwa, weight);
	plot1d["DRminW"]->Fill(drminwb, weight);
	plot1d["DRminbl"]->Fill(drminbl, weight);
	plot1d["DRminbh"]->Fill(drminbh, weight);
	plot1d["DRminadd"]->Fill(drminadd, weight);
	plot2d["METvsChi"]->Fill(an->met.Pt(), testb, weight);
	plot1d["DPhiMET_Nu"]->Fill(Abs(per.Nu().DeltaPhi(an->met)), weight);
	plot2d["METvsDPhiMET_Nu"]->Fill(an->met.Pt(), Abs(per.Nu().DeltaPhi(an->met)), weight);
	plot1d["Mt_W"]->Fill(per.MtWLep(), weight);
	plot1d["Mt_t"]->Fill(per.MttLep(), weight);
	plot2d["METunc"]->Fill(an->met.pxunctot()/an->met.Px(), an->met.pyunctot()/an->met.Py(), weight);
	for(int jn : jetbins)
	{
		stringstream jb;
		if(jn != -1) jb << jn << "_";
		if((jn == -1) || (an->reducedjets.size() - 4 == size_t(jn)) || (jn == jetbins.back() && an->reducedjets.size() - 4 > size_t(jn)))
		{
			plot2d["test_"+jb.str()+"testb"]->Fill(test, testb, weight);
			plot2d["test_"+jb.str()+"nobin"]->Fill(test, per.THad().Pt(), weight);
			plot2d["testb_"+jb.str()+"nobin"]->Fill(testb, per.THad().Pt(), weight);
			double thadpt = Min(per.THad().Pt(), plot2d["test_"+jb.str()+"thadpt"]->GetYaxis()->GetXmax()-.0001);
			plot2d["test_"+jb.str()+"thadpt"]->Fill(test, thadpt, weight);
			plot2d["testb_"+jb.str()+"thadpt"]->Fill(testb, thadpt, weight);
			double tleppt = Min(per.TLep().Pt(), plot2d["test_"+jb.str()+"tleppt"]->GetYaxis()->GetXmax()-.0001);
			plot2d["test_"+jb.str()+"tleppt"]->Fill(test, tleppt, weight);
			plot2d["testb_"+jb.str()+"tleppt"]->Fill(testb, tleppt, weight);
			plot2d["test_"+jb.str()+"tpt"]->Fill(test, tleppt, weight);
			plot2d["testb_"+jb.str()+"tpt"]->Fill(testb, tleppt, weight);
			plot2d["test_"+jb.str()+"tpt"]->Fill(test, thadpt, weight);
			plot2d["testb_"+jb.str()+"tpt"]->Fill(testb, thadpt, weight);
			double thady = Min(Abs(per.THad().Rapidity()), plot2d["test_"+jb.str()+"thady"]->GetYaxis()->GetXmax()-.0001);
			plot2d["test_"+jb.str()+"thady"]->Fill(test, thady, weight);
			plot2d["testb_"+jb.str()+"thady"]->Fill(testb, thady, weight);
			double tlepy = Min(Abs(per.TLep().Rapidity()), plot2d["test_"+jb.str()+"tlepy"]->GetYaxis()->GetXmax()-.0001);
			plot2d["test_"+jb.str()+"tlepy"]->Fill(test, tlepy, weight);
			plot2d["testb_"+jb.str()+"tlepy"]->Fill(testb, tlepy, weight);
			plot2d["test_"+jb.str()+"ty"]->Fill(test, tlepy, weight);
			plot2d["testb_"+jb.str()+"ty"]->Fill(testb, tlepy, weight);
			plot2d["test_"+jb.str()+"ty"]->Fill(test, thady, weight);
			plot2d["testb_"+jb.str()+"ty"]->Fill(testb, thady, weight);
			double ttm = Min(per.TT().M(), plot2d["test_"+jb.str()+"ttm"]->GetYaxis()->GetXmax()-.0001);
			plot2d["test_"+jb.str()+"ttm"]->Fill(test, ttm, weight);
			plot2d["testb_"+jb.str()+"ttm"]->Fill(testb, ttm, weight);
			double tty = Min(Abs(per.TT().Rapidity()), plot2d["test_"+jb.str()+"tty"]->GetYaxis()->GetXmax()-.0001);
			plot2d["test_"+jb.str()+"tty"]->Fill(test, tty, weight);
			plot2d["testb_"+jb.str()+"tty"]->Fill(testb, tty, weight);
			double ttpt = Min(per.TT().Pt(), plot2d["test_"+jb.str()+"ttpt"]->GetYaxis()->GetXmax()-.0001);
			plot2d["test_"+jb.str()+"ttpt"]->Fill(test, ttpt, weight);
			plot2d["testb_"+jb.str()+"ttpt"]->Fill(testb, ttpt, weight);
			plot2d["test_"+jb.str()+"costhetastar"]->Fill(test, per.T_CMS().CosTheta(), weight);
			plot2d["testb_"+jb.str()+"costhetastar"]->Fill(testb, per.T_CMS().CosTheta(), weight);
			plot2d["test_"+jb.str()+"njet"]->Fill(test, an->cleanedjets.size()-4, weight);
			plot2d["testb_"+jb.str()+"njet"]->Fill(testb, an->cleanedjets.size()-4, weight);
			plot2d["test_"+jb.str()+"met"]->Fill(test, an->met.Pt(), weight);
			plot2d["testb_"+jb.str()+"met"]->Fill(testb, an->met.Pt(), weight);
		}
	}
}


