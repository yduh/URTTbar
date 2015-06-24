#include <TTBarPlotsBase.h>
#include <ttbarxsec.h>

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
    plot2d.AddHist("wjets_pt", 500, 0., 500., 500, 0., 500., "p_{T}(j_{W})_{min} [GeV]", "p_{T}(j_{W})_{max} [GeV]");
    plot2d.AddHist("Whad_M_thad_M", 500, 0., 500., 500, 0., 500., "M(W_{had}) [GeV]", "M(t_{had}) [GeV]");
    plot2d.AddHist("Wlep_M_tlep_M", 500, 0., 500., 500, 0., 500., "M(W_{lep}) [GeV]", "M(t_{lep}) [GeV]");
    plot2d.AddHist("thad_M_tlep_M", 500, 0., 500., 500, 0., 500., "M(t_{had}) [GeV]", "M(t_{lep}) [GeV]");
    plot1d.AddHist("lep_pt", 500, 0., 500., "p_{T}(l) [GeV]", "Events");
    plot1d.AddHist("nu_pt", 500, 0., 500., "p_{T}(#nu) [GeV]", "Events");
    plot1d.AddHist("nu_eta", 200, -5, 5., "#eta(#nu)", "Events");
    plot1d.AddHist("lepp_eta", 200, -5, 5., "#eta(l+)", "Events");
    plot1d.AddHist("lepm_eta", 200, -5, 5., "#eta(l-)", "Events");
    plot1d.AddHist("thad_pt", 400, 0, 800, "p_{T}(t_{had}) [GeV]", "Events");
    plot1d.AddHist("tlep_pt", 400, 0, 800, "p_{T}(t_{lep}) [GeV]", "Events");
    plot1d.AddHist("thad_eta", 200, -5, 5, "#eta(t_{had})", "Events");
    plot1d.AddHist("tlep_eta", 200, -5, 5, "#eta(t_{lep})", "Events");
    plot1d.AddHist("thad_y", 200, 0, 5, "|y(t_{had})|", "Events");
    plot1d.AddHist("tlep_y", 200, 0, 5, "|y(t_{lep})|", "Events");
    plot1d.AddHist("tt_M", 1000, 0, 2000, "M(t#bar{t}) [GeV]", "Events");
    plot1d.AddHist("tt_pt", 250, 0, 500, "p_{T}(t#bar{t}) [GeV]", "Events");
    plot1d.AddHist("tt_y", 200, 0, 10., "|y(t#bar{t})|", "Events");
    plot1d.AddHist("tt_DeltaPhi", 200, -Pi(), Pi(), "#Delta#Phi(t#bar{t})", "Events");
    plot1d.AddHist("whad_pt", 100, 0, 200, "p_{T}(W_{had}) [GeV]", "Events");
    plot1d.AddHist("wj_dphi", 100, -Pi(), Pi(), "#Delta#phi(j_{whad})", "Events");
    plot1d.AddHist("wj_dr", 100, 0., 5., "#Delta#R(j_{whad})", "Events");
    plot1d.AddHist("bjet_pt", 100, 0., 500., "p_{T}(b) [GeV]", "Events");
    plot1d.AddHist("bjet_eta", 100, -2.5, 2.5, "#eta(b)", "Events");
    plot1d.AddHist("wjet_pt", 100, 0., 500., "p_{T}(wj) [GeV]", "Events");
    plot1d.AddHist("wjet_eta", 100, -2.5, 2.5, "#eta(wj)", "Events");
	plot1d.AddHist("costhetastar", 20, -1., 1., "cos(#theta*)", "Events");
}

void TTBarPlotsBase::Fill(TLorentzVector* Hb, TLorentzVector* Hwa, TLorentzVector* Hwb, TLorentzVector* Lb, TLorentzVector* Ll, TLorentzVector* Ln, int lepcharge, double weight)
{
	whad = (*Hwa + *Hwb);
	thad = (whad + *Hb);
	wlep = (*Ll + *Ln);
	tlep = (*Ll + *Ln + *Lb);
	tt = (thad + tlep);
	TVector3 bv(-1.*tt.BoostVector());
	tCMS = tlep;	
	if(lepcharge == -1)
	{
		tCMS = thad;	
	}
	tCMS.Boost(bv);

    plot2d["Whad_M_thad_M"]->Fill(whad.M(), thad.M(), weight);
	plot2d["bjets_pt"]->Fill(Min(Hb->Pt(), Lb->Pt()), Max(Hb->Pt(), Lb->Pt()), weight);
	plot2d["wjets_pt"]->Fill(Min(Hwa->Pt(), Hwb->Pt()), Max(Hwa->Pt(), Hwb->Pt()), weight);
	plot2d["Wlep_M_tlep_M"]->Fill(wlep.M(), tlep.M(), weight);
	plot2d["thad_M_tlep_M"]->Fill(thad.M(), tlep.M(), weight);
	plot1d["lep_pt"]->Fill(Ll->Pt(), weight);
	if(lepcharge > 0) {plot1d["lepp_eta"]->Fill(Ll->Eta(), weight);}
	if(lepcharge < 0) {plot1d["lepm_eta"]->Fill(Ll->Eta(), weight);}
	plot1d["nu_pt"]->Fill(Ln->Pt(), weight);
	plot1d["nu_eta"]->Fill(Ln->Eta(), weight);
	plot1d["thad_pt"]->Fill(thad.Pt(), weight);
	plot1d["tlep_pt"]->Fill(tlep.Pt(), weight);
	plot1d["thad_eta"]->Fill(thad.Eta(), weight);
	plot1d["tlep_eta"]->Fill(tlep.Eta(), weight);
	plot1d["thad_y"]->Fill(Abs(thad.Rapidity()), weight);
	plot1d["tlep_y"]->Fill(Abs(tlep.Rapidity()), weight);
	plot1d["tt_DeltaPhi"]->Fill(tlep.DeltaPhi(thad), weight);
	plot1d["tt_M"]->Fill(tt.M(), weight);
	plot1d["tt_pt"]->Fill(tt.Pt(), weight);
	plot1d["tt_y"]->Fill(Abs(tt.Rapidity()), weight);
	plot1d["whad_pt"]->Fill(whad.Pt(), weight);
	plot1d["wj_dphi"]->Fill(Hwa->DeltaPhi(*Hwb), weight);
	plot1d["wj_dr"]->Fill(Hwa->DeltaR(*Hwb), weight);
	plot1d["costhetastar"]->Fill(tCMS.CosTheta(), weight);
	plot1d["bjet_pt"]->Fill(Hb->Pt(), weight);
	plot1d["bjet_pt"]->Fill(Lb->Pt(), weight);
	plot1d["bjet_eta"]->Fill(Hb->Eta(), weight);
	plot1d["bjet_eta"]->Fill(Lb->Eta(), weight);
	plot1d["wjet_pt"]->Fill(Hwa->Pt(), weight);
	plot1d["wjet_pt"]->Fill(Hwb->Pt(), weight);
	plot1d["wjet_eta"]->Fill(Hwa->Eta(), weight);
	plot1d["wjet_eta"]->Fill(Hwb->Eta(), weight);

}

