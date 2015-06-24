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

    plot1d.AddHist("toppthad", an->topptbins, "toppthad", "Events");
	plot1d.AddHist("topptlep", an->topptbins, "topptlep", "Events");
	plot1d.AddHist("topetahad", an->topybins, "topetahad", "Events");
	plot1d.AddHist("topetalep", an->topybins, "topetalep", "Events");
	plot1d.AddHist("ttm", an->ttmbins, "ttm", "Events");
	plot1d.AddHist("ttpt", an->ttptbins, "ttpt", "Events");

}

void TTBarGenPlots::Fill(TLorentzVector* Hb, TLorentzVector* Hwa, TLorentzVector* Hwb, TLorentzVector* Lb, TLorentzVector* Ll, TLorentzVector* Ln, int leppdgid, double weight)
{
    int lepcharge;
    if(leppdgid > 0) lepcharge = -1;
    else if(leppdgid < 0) lepcharge = 1;
    TTBarPlotsBase::Fill(Hb, Hwa, Hwb, Lb, Ll, Ln, lepcharge, weight);
    
    plot1d["toppthad"]->Fill(thad.Pt(), weight);
	plot1d["topptlep"]->Fill(tlep.Pt(), weight);
	plot1d["topetahad"]->Fill(Abs(thad.Eta()), weight);
	plot1d["topetalep"]->Fill(Abs(tlep.Eta()), weight);
	plot1d["ttm"]->Fill(tt.M(), weight);
	plot1d["ttpt"]->Fill(tt.Pt(), weight);
}


