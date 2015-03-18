#include <TTBarPlots.h>
#include <URStreamer.h>
#include <Permutation.h>
#include <ttbarxsec.h>

using namespace std;
using namespace TMath;

TTBarPlots::TTBarPlots(string prefix) : prefix_(prefix), plot1d(prefix), plot2d(prefix)
{

}

TTBarPlots::~TTBarPlots()
{

}

void TTBarPlots::Init(ttbar* analysis)
{
    plot2d.AddHist("bjets_pt", 500, 0., 500., 500, 0., 500., "p_{T}(b)_{min} (GeV)", "p_{T}(b)_{max} (GeV)");
    plot2d.AddHist("wjets_pt", 500, 0., 500., 500, 0., 500., "p_{T} (GeV)", "p_{T} (GeV)");
    plot2d.AddHist("Whad_M_thad_M", 500, 0., 500., 500, 0., 500., "M(W_{had}) (GeV)", "M(t_{had}) (GeV)");
    plot2d.AddHist("Wlep_M_tlep_M", 500, 0., 500., 500, 0., 500., "M(W_{lep}) (GeV)", "M(t_{lep}) (GeV)");
    plot2d.AddHist("thad_M_tlep_M", 500, 0., 500., 500, 0., 500., "M(t_{had}) (GeV)", "M(t_{lep}) (GeV)");
    plot2d.AddHist("test_ptthad", 96, -12., 12., analysis->topptbins, "test", "p_{T}(t_{had}) (GeV)");
    plot2d.AddHist("test_pttlep", 96, -12., 12., analysis->topptbins, "test", "p_{T}(t_{lep}) (GeV)");
    plot2d.AddHist("test_mtt", 96, -12., 12., analysis->ttmbins, "test", "M_{tt} (GeV)");
    plot1d.AddHist("lep_pt", 500, 0., 500., "p_{T}(l) (GeV)", "Events");
    plot1d.AddHist("lepp_eta", 200, -5, 5., "#eta(l+)", "Events");
    plot1d.AddHist("lepm_eta", 200, -5, 5., "#eta(l-)", "Events");
    plot1d.AddHist("thad_pt", 200, 0, 400, "p_{T}(t_{had}) (GeV)", "Events");
    plot1d.AddHist("tlep_pt", 200, 0, 400, "p_{T}(t_{lep}) (GeV)", "Events");
    plot1d.AddHist("thad_eta", 200, -5, 5, "#eta(t_{had})", "Events");
    plot1d.AddHist("tlep_eta", 200, -5, 5, "#eta(t_{lep})", "Events");
    plot1d.AddHist("thad_y", 200, -5, 5, "y(t_{had})", "Events");
    plot1d.AddHist("tlep_y", 200, -5, 5, "y(t_{lep})", "Events");
    plot1d.AddHist("tt_M", 500, 0, 1000, "M(t#bar{t}) (GeV)", "Events");
    plot1d.AddHist("whad_pt", 100, 0, 200, "p_{T}(W_{had}) (GeV)", "Events");
    plot1d.AddHist("wj_dphi", 100, -4, 4, "#Delta#phi(j_{whad}) (GeV)", "Events");
    plot1d.AddHist("res_Mtt", 100, -4, 4, "#DeltaM_{t#bar{t}}/M_{t#bar{t}}", "Events");
    plot1d.AddHist("res_ptt", 100, -4, 4, "#Deltap_{T}(t)/p_{T}(t)", "Events");
    plot1d.AddHist("res_wjets", 100, -4, 4, "#Deltap_{T}(t)/p_{T}(t)", "Events");
}

void TTBarPlots::Fill(TLorentzVector* Hb, TLorentzVector* Hwa, TLorentzVector* Hwb, TLorentzVector* Lb, TLorentzVector* Ll, TLorentzVector* Ln, int lepcharge, double test, double weight)
{

	TLorentzVector whad(*Hwa + *Hwb);
	TLorentzVector thad(whad + *Hb);
	TLorentzVector tlep(*Ll + *Ln + *Lb);
    plot2d["Whad_M_thad_M"]->Fill((*Hwa + *Hwb).M(), thad.M(), weight);
    plot2d["test_ptthad"]->Fill(test, thad.Pt(), weight);
	plot2d["test_pttlep"]->Fill(test, tlep.Pt(), weight);
	plot2d["test_mtt"]->Fill(test, (tlep+thad).M(), weight);
	plot2d["bjets_pt"]->Fill(Min(Hb->Pt(), Lb->Pt()), Max(Hb->Pt(), Lb->Pt()), weight);
	plot2d["wjets_pt"]->Fill(Min(Hwa->Pt(), Hwb->Pt()), Max(Hwa->Pt(), Hwb->Pt()), weight);
	plot2d["Wlep_M_tlep_M"]->Fill((*Ll + *Ln).M(), tlep.M(), weight);
	plot2d["thad_M_tlep_M"]->Fill(thad.M(), tlep.M(), weight);
	plot1d["lep_pt"]->Fill(Ll->Pt(), weight);
	if(lepcharge > 0) {plot1d["lepp_eta"]->Fill(Ll->Eta(), weight);}
	if(lepcharge < 0) {plot1d["lepm_eta"]->Fill(Ll->Eta(), weight);}
	plot1d["thad_pt"]->Fill(thad.Pt(), weight);
	plot1d["tlep_pt"]->Fill(tlep.Pt(), weight);
	plot1d["thad_eta"]->Fill(thad.Eta(), weight);
	plot1d["tlep_eta"]->Fill(tlep.Eta(), weight);
	plot1d["thad_y"]->Fill(thad.Rapidity(), weight);
	plot1d["tlep_y"]->Fill(tlep.Rapidity(), weight);
	plot1d["tt_M"]->Fill((thad+tlep).M(), weight);
	plot1d["whad_pt"]->Fill(whad.Pt(), weight);
	plot1d["wj_dphi"]->Fill(Hwa->DeltaPhi(*Hwb), weight);
	
	}

void TTBarPlots::Fill(Permutation& per, int lepcharge, double weight)
{
	TLorentzVector nu(per.Nu());
	Fill(per.BHad(), per.WJa(), per.WJb(), per.BLep(), per.L(), &nu, lepcharge, per.Prob(), weight);
}


