#include <TTBarGenPlots.h>
#include <URStreamer.h>
#include <Permutation.h>
#include <ttbarxsec.h>

using namespace std;
using namespace TMath;

TTBarGenPlots::TTBarGenPlots(string prefix) : TTBarPlotsBase(prefix)
{

}

TTBarGenPlots::~TTBarGenPlots()
{

}

void TTBarGenPlots::Init(ttbar* analysis)
{
	TTBarPlotsBase::Init(analysis);

    plot1d.AddHist("tlepy", an->topybins, "y(t_{l})", "Events");
	plot1d.AddHist("tleppt", an->topptbins, "p_{T}(t_{l}) [GeV]", "Events");
	plot1d.AddHist("thady", an->topybins, "y(t_{h})", "Events");
	plot1d.AddHist("thadpt", an->topptbins, "p_{T}(t_{h}) [GeV]", "Events");
	plot1d.AddHist("ttm", an->ttmbins, "M(t#bar{t}) [GeV]", "Events");
	plot1d.AddHist("ttpt", an->ttptbins, "p_{T}(t#bar{t}) [GeV]", "Events");
	plot1d.AddHist("tty", an->ttybins, "y(t#bar{t})", "Events");

}

void TTBarGenPlots::Fill(Permutation& per, double weight)
{
	TTBarPlotsBase::Fill(per, weight);
	plot1d["tlepy"]->Fill(Abs(per.TLep().Rapidity()), weight);
	plot1d["tleppt"]->Fill(per.TLep().Pt(), weight);
	plot1d["thady"]->Fill(Abs(per.THad().Rapidity()), weight);
    plot1d["thadpt"]->Fill(per.THad().Pt(), weight);
	plot1d["ttm"]->Fill(per.TT().M(), weight);
	plot1d["ttpt"]->Fill(per.TT().Pt(), weight);
	plot1d["tty"]->Fill(Abs(per.TT().Rapidity()), weight);
}

