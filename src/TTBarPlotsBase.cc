#include <TTBarPlotsBase.h>
#include <ttbarxsec.h>
#include <Permutation.h>


using namespace std;
using namespace TMath;

TTBarPlotsBase::TTBarPlotsBase(string prefix) : prefix_(prefix), plot1d(prefix), plot2d(prefix)
{

}

TTBarPlotsBase::~TTBarPlotsBase()
{

}

void TTBarPlotsBase::Init(ttbar* analysis)
{
	an = analysis;
    plot2d.AddHist("bjets_pt", 500, 0., 500., 500, 0., 500., "p_{T}(b)_{min} [GeV]", "p_{T}(b)_{max} [GeV]");
    plot2d.AddHist("bjets_pthad_ptlep", 500, 0., 500., 500, 0., 500., "p_{T}(b_{had}) [GeV]", "p_{T}(b_{lep}) [GeV]");
    plot2d.AddHist("wjets_pt", 500, 0., 500., 500, 0., 500., "p_{T}(j_{W})_{min} [GeV]", "p_{T}(j_{W})_{max} [GeV]");
    plot2d.AddHist("wjets_bjets_pt", 500, 0., 500., 500, 0., 500., "p_{T}(j_{W})_{max} [GeV]", "p_{T}(b_{had}) [GeV]");
    plot2d.AddHist("thad_M_Whad_M", 500, 0., 500., 500, 0., 500., "M(t_{h}) [GeV]", "M(W_{h}) [GeV]");
    plot2d.AddHist("Wlep_M_tlep_M", 500, 0., 500., 500, 0., 500., "M(W_{l}) [GeV]", "M(t_{l}) [GeV]");
    plot2d.AddHist("thad_M_tlep_M", 500, 0., 500., 500, 0., 500., "M(t_{h}) [GeV]", "M(t_{l}) [GeV]");
    plot2d.AddHist("thad_pt_wjwj_dr", 100, 0., 2000., 100, 0., 5., "p_{T}(t_{h}) [GeV]", "#Delta R(j_{W}, j_{W})");
    plot2d.AddHist("thad_pt_wjbj_dr", 100, 0., 2000., 100, 0., 5., "p_{T}(t_{h}) [GeV]", "#Delta R_{min}(j_{W}, b)");
    plot1d.AddHist("lep_pt", 500, 0., 500., "p_{T}(l) [GeV]", "Events");
    plot1d.AddHist("lep_eta", 420, -2.1, 2.1, "#eta(l)", "Events");
    plot1d.AddHist("nu_pt", 500, 0., 500., "p_{T}(#nu) [GeV]", "Events");
    plot1d.AddHist("nu_eta", 200, -5, 5., "#eta(#nu)", "Events");
    plot1d.AddHist("lepp_eta", 200, -5, 5., "#eta(l+)", "Events");
    plot1d.AddHist("lepm_eta", 200, -5, 5., "#eta(l-)", "Events");
    plot1d.AddHist("thad_pt", 400, 0, 800, "p_{T}(t_{h}) [GeV]", "Events");
    plot1d.AddHist("tlep_pt", 400, 0, 800, "p_{T}(t_{l}) [GeV]", "Events");
    plot1d.AddHist("thad_eta", 200, -5, 5, "#eta(t_{h})", "Events");
    plot1d.AddHist("tlep_eta", 200, -5, 5, "#eta(t_{l})", "Events");
    plot1d.AddHist("thad_y", 200, 0, 5, "|y(t_{h})|", "Events");
    plot1d.AddHist("tlep_y", 200, 0, 5, "|y(t_{l})|", "Events");
    plot1d.AddHist("tt_M", 1000, 0, 2000, "M(t#bar{t}) [GeV]", "Events");
    plot1d.AddHist("tt_pt", 250, 0, 500, "p_{T}(t#bar{t}) [GeV]", "Events");
    plot1d.AddHist("tt_y", 200, 0, 10., "|y(t#bar{t})|", "Events");
    plot1d.AddHist("thard_pt", 400, 0, 800, "p_{T}(t_{hard}) [GeV]", "Events");
    plot1d.AddHist("tsoft_pt", 400, 0, 800, "p_{T}(t_{soft}) [GeV]", "Events");
    plot1d.AddHist("tt_DeltaPhi", 200, -Pi(), Pi(), "#Delta#Phi(t#bar{t})", "Events");
    plot1d.AddHist("whad_pt", 100, 0, 200, "p_{T}(W_{h}) [GeV]", "Events");
    plot1d.AddHist("wj_dphi", 100, -Pi(), Pi(), "#Delta#phi(j_{whad})", "Events");
    plot1d.AddHist("wj_dr", 100, 0., 5., "#Delta#R(j_{whad})", "Events");
    plot1d.AddHist("bjet_pt", 100, 0., 500., "p_{T}(b) [GeV]", "Events");
    plot1d.AddHist("bjet_eta", 100, -2.5, 2.5, "#eta(b)", "Events");
    plot1d.AddHist("wjet_pt", 100, 0., 500., "p_{T}(wj) [GeV]", "Events");
    plot1d.AddHist("wjet_eta", 100, -2.5, 2.5, "#eta(wj)", "Events");
	plot1d.AddHist("costhetastar", 20, -1., 1., "cos(#theta*)", "Events");
	plot1d.AddHist("dbeta", 200, 0, 2., "#Delta#beta", "Events");
	plot1d.AddHist("dymp", 200, -2., 2., "|y(t)|-|y(#bar{t})|", "Events");
}

//void TTBarPlotsBase::Fill(TLorentzVector* Hb, TLorentzVector* Hwa, TLorentzVector* Hwb, TLorentzVector* Lb, TLorentzVector* Ll, TLorentzVector* Ln, int lepcharge, double weight)
void TTBarPlotsBase::Fill(Permutation& per, double weight)
{

	plot2d["thad_pt_wjwj_dr"]->Fill(per.THad().Pt(), per.WJa()->DeltaR(*per.WJb()), weight);
	plot2d["thad_pt_wjbj_dr"]->Fill(per.THad().Pt(), Min(per.BHad()->DeltaR(*per.WJa()), per.BHad()->DeltaR(*per.WJb())), weight);
        plot2d["thad_M_Whad_M"]->Fill(per.THad().M(), per.WHad().M(), weight);
	plot2d["bjets_pt"]->Fill(Min(per.BHad()->Pt(), per.BLep()->Pt()), Max(per.BHad()->Pt(), per.BLep()->Pt()), weight);
	plot2d["bjets_pthad_ptlep"]->Fill(per.BHad()->Pt(), per.BLep()->Pt(), weight);
	plot2d["wjets_pt"]->Fill(Min(per.WJa()->Pt(), per.WJb()->Pt()), Max(per.WJa()->Pt(), per.WJb()->Pt()), weight);
	plot2d["wjets_bjets_pt"]->Fill(Max(per.WJa()->Pt(), per.WJb()->Pt()), per.BHad()->Pt(), weight);
	plot2d["Wlep_M_tlep_M"]->Fill(per.WLep().M(), per.TLep().M(), weight);
	plot2d["thad_M_tlep_M"]->Fill(per.THad().M(), per.TLep().M(), weight);
	plot1d["lep_pt"]->Fill(per.L()->Pt(), weight);
	plot1d["lep_eta"]->Fill(per.L()->Eta(), weight);
	if(per.LCharge() > 0) {plot1d["lepp_eta"]->Fill(per.L()->Eta(), weight);}
	if(per.LCharge() < 0) {plot1d["lepm_eta"]->Fill(per.L()->Eta(), weight);}
	plot1d["nu_pt"]->Fill(per.Nu().Pt(), weight);
	plot1d["nu_eta"]->Fill(per.Nu().Eta(), weight);
	plot1d["thad_pt"]->Fill(per.THad().Pt(), weight);
	plot1d["tlep_pt"]->Fill(per.TLep().Pt(), weight);
	plot1d["thad_eta"]->Fill(per.THad().Eta(), weight);
	plot1d["tlep_eta"]->Fill(per.TLep().Eta(), weight);
	plot1d["thad_y"]->Fill(Abs(per.THad().Rapidity()), weight);
	plot1d["tlep_y"]->Fill(Abs(per.TLep().Rapidity()), weight);
	plot1d["thard_pt"]->Fill(per.THard().Pt(), weight);
	plot1d["tsoft_pt"]->Fill(per.TSoft().Pt(), weight);
	plot1d["tt_DeltaPhi"]->Fill(per.TLep().DeltaPhi(per.THad()), weight);
	plot1d["tt_M"]->Fill(per.TT().M(), weight);
	plot1d["tt_pt"]->Fill(per.TT().Pt(), weight);
	plot1d["tt_y"]->Fill(Abs(per.TT().Rapidity()), weight);
	plot1d["whad_pt"]->Fill(per.WHad().Pt(), weight);
	plot1d["wj_dphi"]->Fill(per.WJa()->DeltaPhi(*per.WJb()), weight);
	plot1d["wj_dr"]->Fill(per.WJa()->DeltaR(*per.WJb()), weight);
	plot1d["costhetastar"]->Fill(per.T_CMS().CosTheta(), weight);
	plot1d["bjet_pt"]->Fill(per.BHad()->Pt(), weight);
	plot1d["bjet_pt"]->Fill(per.BLep()->Pt(), weight);
	plot1d["bjet_eta"]->Fill(per.BHad()->Eta(), weight);
	plot1d["bjet_eta"]->Fill(per.BLep()->Eta(), weight);
	plot1d["wjet_pt"]->Fill(per.WJa()->Pt(), weight);
	plot1d["wjet_pt"]->Fill(per.WJb()->Pt(), weight);
	plot1d["wjet_eta"]->Fill(per.WJa()->Eta(), weight);
	plot1d["wjet_eta"]->Fill(per.WJb()->Eta(), weight);
	plot1d["dbeta"]->Fill((per.THad().BoostVector() - per.TLep().BoostVector()).Mag(), weight);
	plot1d["dymp"]->Fill(Abs(per.T().Rapidity())-Abs(per.Tb().Rapidity()), weight);

}

