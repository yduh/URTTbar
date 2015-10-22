#include "ttbarxsec.h"
#include "TRandom3.h"

#include "Permutation.h"
#include "PDFuncertainty.h"
#include "NeutrinoSolver.h"
#include "ConfigParser.h"

using namespace std;

ttbar::ttbar(const std::string output_filename):
	AnalyzerBase("ttbar", output_filename),
	gen1d("gen"),
	gen2d("gen"),
    ttp_genall("genall"),
    ttp_genacc("genacc"),
	reco1d("reco"),
	reco2d("reco"),
	truth1d("truth"),
	truth2d("truth"),
	ttp_truth("truth"),
	ttp_right("right"),
	ttp_wrong("wrong"),
	ttp_semi("semi"),
	ttp_other("other"),
	ttp_all("all"),
	ttp_jetspos_right("jetspos_right"),
	ttp_jetspos_wrong("jetspos_wrong"),
//	ttp_hadjets_right("hadjets_right"),
//	ttp_hadjets_wrong("hadjets_wrong"),
//	ttp_jets_right("jets_right"),
//	ttp_jets_wrong("jets_wrong"),
//	ttp_blep_right("blep_right"),
//	ttp_blep_wrong("blep_wrong"),
	ttp_whad_right("whad_right"),
	ttp_whad_wrong("whad_wrong"),
	ttp_tlepthad_right("ttp_tlepthad_right"),
	ttp_tlep_right("ttp_tlep_right"),
	ttp_thad_right("ttp_thad_right"),
	ttp_nn_right("ttp_nn_right"),
	ttp_nsemi_right("ttp_nsemi_right"),
	response("response", this),
	responseall("responseall", this),
	jetscaler("jetuncertainty.root"),
	DATASIM(false),
	PSEUDOTOP(true),
	BTAGMODE(false), //set true for the b-tag efficiency measurement
	JETSCALEMODE(false), //set true for jet scale measurement
	ELECTRONS(true),
	MUONS(true),
	B_TIGHT(0.97),
	B_MEDIUM(0.89),
	cnbtag(2), //1: one thight b-jet, 2: two medium b-jets
	cnusedjets(100), //only nused jets, ordered by pT are used for the permutations
	cwjetptsoft(25.), //min pT of softer W-jet
	cwjetpthard(35.), //min pT of harder W-jet 
	cbjetptsoft(25.), //min pT of softer b-jets
	cbjetpthard(35.), //min pT of harder b-jets
	cjetetamax(2.4),//max |eta| for jets
	clptmin(30.), //min pT of lepton (el or mu)
	cletamax(2.1),//max |eta| of leptons (max allowed value is 2.4) 
	cpwjetptsoft(25.), //min pT of softer W-jet
	cpwjetpthard(35.), //min pT of harder W-jet 
	cpbjetptsoft(25.), //min pT of softer b-jets
	cpbjetpthard(35.), //min pT of harder b-jets
	cpjetetamax(2.4),//max |eta| for jets
	cplptmin(30.), //min pT of lepton (el or mu)
	cpletamax(2.1),//max |eta| of leptons (max allowed value is 2.4) 
	cpjetsep(0.),
	csigmajet(0.),
	cjetres(0.),
	csigmamet(0.),
	ctopptweight(0.),
	cttptweight(0.),
	crenscale(0),
	cfacscale(0),
	cbtagunc(0),
	cltagunc(0),
	cpileup(0),
	HERWIGPP(false)
{
	
	ConfigParser CP("ttbarxsec.cfg");
	PSEUDOTOP = CP.Get<bool>("PSEUDOTOP");
	BTAGMODE = CP.Get<bool>("BTAGMODE");
	ELECTRONS = CP.Get<bool>("ELECTRONS");
	MUONS = CP.Get<bool>("MUONS");
	IDMuon::USEISO = CP.Get<bool>("LEPTONISO");
	IDElectron::USEISO = CP.Get<bool>("LEPTONISO");
	cnbtag = CP.Get<int>("nbtag");

	cwjetptsoft = CP.Get<double>("wjetptsoft"); 
	cwjetpthard = CP.Get<double>("wjetpthard"); 
	cbjetptsoft = CP.Get<double>("bjetptsoft"); 
	cbjetpthard = CP.Get<double>("bjetpthard"); 
	cjetetamax = CP.Get<double>("jetetamax");
	clptmin = CP.Get<double>("lptmin"); 
	cletamax = CP.Get<double>("letamax");

	cpwjetptsoft = CP.Get<double>("Pwjetptsoft"); 
	cpwjetpthard = CP.Get<double>("Pwjetpthard"); 
	cpbjetptsoft = CP.Get<double>("Pbjetptsoft"); 
	cpbjetpthard = CP.Get<double>("Pbjetpthard"); 
	cpjetetamax = CP.Get<double>("Pjetetamax");
	cplptmin = CP.Get<double>("Plptmin"); 
	cpletamax = CP.Get<double>("Pletamax");
	cpjetsep = CP.Get<double>("Pjetsep");

	csigmajet = CP.Get<double>("sigmajet");
	cjetres = CP.Get<double>("jetres");
	csigmamet = CP.Get<double>("sigmamet");
	ctopptweight = CP.Get<double>("topptweight");
	cttptweight = CP.Get<double>("ttptweight");
	if(output_filename.find("tt_PowhegP8") != string::npos)
	{
		cfacscale = CP.Get<int>("facscale");
		crenscale = CP.Get<int>("renscale");
	}
	cbtagunc = CP.Get<int>("btagunc");
	cltagunc = CP.Get<int>("ltagunc");
	cpileup = CP.Get<int>("pileupunc");

	DATASIM = CP.Get<bool>("DATASIM");
	double lumi = CP.Get<double>("lumi");
	crandomseed = CP.Get<int>("randomseed");
	cout << output_filename << endl;
	if(output_filename.find("WJets") != string::npos)
	{
		selectionprob = lumi*61524./14113879.;
	}
	else if(output_filename.find("DYJets") != string::npos)
	{
		selectionprob = lumi*6024./11416389.;
	}
	else if(output_filename.find("Wtbar") != string::npos)
	{
		selectionprob = lumi*35.6/499999.;
	}
	else if(output_filename.find("Wt") != string::npos)
	{
		selectionprob = lumi*35.6/500000.;
	}
	else if(output_filename.find("STt") != string::npos)
	{
		selectionprob = lumi*217./860071.;
	}
	else if(output_filename.find("tt_PowhegP8") != string::npos)
	{
		selectionprob = lumi*806./19086610;
	}
	else if(output_filename.find("tt_aMCatNLO") != string::npos)
	{
		selectionprob = lumi*806./1656654.;
	}
	if(output_filename.find("tt_PowhegHpp") != string::npos){HERWIGPP = true;}

	jetptmin = min(cwjetptsoft, cbjetptsoft);
//	topptbins = {0., 40., 55., 65., 75., 85., 95., 105., 115., 125., 135., 145., 155., 170., 185., 200., 220., 240., 265., 300., 350., 400., 800.};
//	topetabins = {0., 0.2, 0.4, 0.6,  0.8,  1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.3, 2.8, 4.0};
//	ttmbins = {250., 350., 370., 390., 410., 430., 450., 470., 490., 510., 530., 550., 575., 600., 630., 670., 720., 770., 900, 1500.};
//	ttybins = {0., 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 3.};
//	ttptbins = {0., 20., 30., 40., 50., 60., 70., 90., 110., 140., 180., 250., 500.};
//	metbins = {0., 20., 30., 40., 50., 60., 70., 90., 110., 140., 180., 250., 1000.};
	
	//topptbins = {0.0, 65.0, 100.0, 135.0, 175.0, 240.0, 775.0};
	//topybins = {0.0, 0.25, 0.5, 0.75, 1.0, 1.5, 2.5};
	//ttmbins = {280., 400.0, 450.0, 510.0, 580.0, 720.0, 2000.0};
	//ttybins = {0.0, 0.2, 0.4, 0.6, 0.85, 1.25, 2.5};
	//ttptbins = {0.0, 25.0, 40.0, 60.0, 85.0, 150.0, 500.0};
	//metbins = {0.0, 30.0, 45.0, 60.0, 80.0, 120.0, 580.0};
	//jetbins = {-0.5, 0.5, 1.5, 2.5, 10.};
	//nobins = {0., 13000.};

	topptbins = {0.0, 65.0, 100.0, 135.0, 175.0, 240.0, 500.0};
	topybins = {0.0, 0.25, 0.5, 0.75, 1.0, 1.5, 2.5};
	ttmbins = {280., 400.0, 450.0, 510.0, 580.0, 720.0, 1400.0};
	ttybins = {0.0, 0.2, 0.4, 0.6, 0.85, 1.25, 2.5};
	ttptbins = {0.0, 25.0, 40.0, 60.0, 85.0, 150.0, 500.0};
	metbins = {0.0, 30.0, 45.0, 60.0, 80.0, 120.0, 580.0};
	jetbins = {-0.5, 0.5, 1.5, 2.5, 3.5};
	nobins = {0., 13000.};
	
	btagpt = {30., 50., 70., 100., 140., 200., 300., 670.};
	//vector<string> testpdf = {"CT10", "CT10as", "NNPDF30_nnlo_as_0118"};
	vector<string> testpdf = {"CT10nlo", "NNPDF30_nlo_as_0118", "MMHT2014nlo68clas118"};
	//vector<string> testpdf = {"CT10nlo", "NNPDF30_nlo_as_0118"};
	pdfunc = new PDFuncertainty("NNPDF30_nlo_as_0118", 0, testpdf);

}

void ttbar::begin()
{
	outFile_.cd();
	TDirectory* dir_gen = outFile_.mkdir("GEN");
	dir_gen->cd();
	gen1d.AddHist("TYP", 4, 0., 4., "Decay TYP", "Events");
	gen1d.AddHist("DRW", 600, 0., 6., "DR", "Events");
	gen1d.AddHist("DRB", 600, 0., 6., "DR", "Events");
	//gen2d.AddHist("wjets_eta", 100, 0., 5., 100, 0, 5, "W-jet #eta_{min}", "W-jet #eta_{max}");
	//gen2d.AddHist("bjets_eta", 100, 0., 5., 100, 0, 5, "b-jet #eta_{min}", "b-jet #eta_{max}");
	//gen2d.AddHist("wjets_pt", 200, 0, 400, 200, 0., 400, "W-jet p_{T,min} [GeV]", "W-jet p_{T,max} [GeV]");
	//gen2d.AddHist("bjets_pt", 200, 0, 400, 200, 0., 400, "b-jet p_{T,min} [GeV]", "b-jet p_{T,max} [GeV]");
	//gen2d.AddHist("t_pt", 200, 0, 600, 200, 0., 600, "t p_{T,min} [GeV]", "t p_{T,max} [GeV]");
	//gen1d.AddHist("bjets_dr", 100, 0, 5, "b-jets #DeltaR", "Events");
	//gen1d.AddHist("wjets_dr", 100, 0, 5, "W-jets #DeltaR", "Events");
	//gen2d.AddHist("Wmasshad_tmasshad", 2000, 0., 500., 2000, 0., 500, "M(W) [GeV]", "M(t) [GeV]");
    ttp_genall.Init(this);
    ttp_genacc.Init(this);

	TDirectory* dir_truth = outFile_.mkdir("TRUTH");
	dir_truth->cd();
	truth1d.AddHist("counter", 20, 0., 20., "counter", "Events");
	truth1d.AddHist("npu", 100, 0., 100., "npu", "Events");
	truth1d.AddHist("npuorig", 100, 0., 100., "npuorig", "Events");
	truth1d.AddHist("found", 10, 0., 10., "found", "Events");
	truth1d.AddHist("foundgen", 10, 0., 10., "found", "Events");
	truth2d.AddHist("btag2d_true", 100, 0., 1., 100, 0., 1., "btag", "btag");
	truth1d.AddHist("btag_true", 100, 0., 1., "btag", "Events");
	truth1d.AddHist("btag_wrong", 100, 0., 1., "btag", "Events");
	truth1d.AddHist("wjet_sep", 100, 0., 100, "sep", "Events");	
	truth1d.AddHist("bjet_sep", 100, 0., 100, "sep", "Events");
	truth2d.AddHist("tt_jets", 3, 0., 3., 3, 0., 3., "num b-jets", "num w-jets");
	truth2d.AddHist("Jetstt_JetsAll", 5, 0., 5., 20, 0., 20., "jet tt-system", "all jets");
	truth2d.AddHist("Nmu_Ne", 5, 0., 5., 5, 0., 5., "# #mu", "# e");
	truth2d.AddHist("Ne_Nmu", 5, 0., 5., 5, 0., 5., "# e", "# #mu");
	truth2d.AddHist("dPtJet_right", 50, 0., 250., 200, -2., 2., "p_{T} [GeV]", "#Deltap_{T}/p_{T}");
	truth2d.AddHist("dPtbJet_right", 50, 0., 250., 200, -2., 2., "p_{T} [GeV]", "#Deltap_{T}/p_{T}");
	truth1d.AddHist("dRNu_right", 200, 0., 10., "#DeltaR(#nu_{gen}, #nu_{rec})", "Events");
	truth1d.AddHist("dPtNu_right", 200, -2., 2., "#Deltap_{T}/p_{T}", "Events");
	truth1d.AddHist("dPzNu_right", 200, -2., 2., "#Deltap_{z}/p_{z}", "Events");
	truth2d.AddHist("dPzNu_dPhi_right", 200, -2., 2., 100, 0., 3.15, "#Deltap_{z}/p_{z}", "#Delta#Phi(#nu, met)");
	truth2d.AddHist("dPzNu_chi2_right", 200, -2., 2., 100, 0., 100, "#Deltap_{z}/p_{z}", "#Delta#Phi(#nu, met)");
	truth2d.AddHist("RES_Mtt_right", 200, -1., 1., 50, 300, 1300 , "#Delta M_{tt}/M_{tt}", "M_{tt} [GeV]");
	truth2d.AddHist("RES_Mtt_all", 200, -1., 1., 50, 300, 1300 , "#Delta M_{tt}/M_{tt}", "M_{tt} [GeV]");
	truth2d.AddHist("RES_dbeta_right", 200, -1., 1., 50, 0, 2 , "#Delta (#delta #beta)/#delta #beta", "#delta #beta");
	truth2d.AddHist("RES_dbeta_all", 200, -1., 1., 50, 0, 2 , "#Delta (#delta #beta)/#delta #beta", "#delta #beta");
	truth1d.AddHist("dRNuMet_right", 200, 0., 10., "#DeltaR(#nu_{gen}, #nu_{rec})", "Events");
	truth1d.AddHist("dPtNuMet_right", 200, -2., 2., "#Deltap_{T}/p_{T}", "Events");
	truth2d.AddHist("Wmasshad_tmasshad_right", 500, 0., 500., 500, 0., 500, "M(t) [GeV]", "M(W) [GeV]");
	truth2d.AddHist("Wmasshad_tmasshad_wrong", 500, 0., 500., 500, 0., 500, "M(t) [GeV]", "M(W) [GeV]");
	truth2d.AddHist("Wmasslep_tmasslep_right", 500, 0., 500., 500, 0., 500, "M(W) [GeV]", "M(t) [GeV]");
	truth2d.AddHist("Wmasslep_tmasslep_wrong", 500, 0., 500., 500, 0., 500, "M(W) [GeV]", "M(t) [GeV]");
	truth2d.AddHist("Wmasshad_tmasshad_rightw", 500, 0., 500., 500, 0., 500, "M(W) [GeV]", "M(t) [GeV]");
	truth2d.AddHist("Wmasshad_tmasshad_wrongw", 500, 0., 500., 500, 0., 500, "M(W) [GeV]", "M(t) [GeV]");
	truth1d.AddHist("btagtest_wrong", 1000, -100, 100., "-Log(p) btag-test", "Events");
	truth1d.AddHist("masstest_wrong", 1000, -100, 100., "-Log(p) mass-test", "Events");
	truth1d.AddHist("nstest_wrong", 200, 0, 20., "neutrino-test", "Events");
	truth1d.AddHist("nschi_wrong", 75, 0., 150., "#chi neutrino-test", "Events");
	truth1d.AddHist("comtest_wrong", 1000, -100, 100., "-Log(p)", "Events");
	truth1d.AddHist("btagtest_right", 1000, -100, 100., "-Log(p) btag-test", "Events");
	truth1d.AddHist("masstest_right", 1000, -100, 100., "-Log(p) mass-test", "Events");
	truth1d.AddHist("nstest_right", 200, 0, 20., "neutrino-test", "Events");
	truth1d.AddHist("nschi_right", 75, 0., 150., "#chi neutrino-test", "Events");
	truth1d.AddHist("comtest_right", 1000, -100, 100., "-Log(p)", "Events");
	truth2d.AddHist("Elrho_iso_1", 10, 0., 50., 100, 0., 150, "rho", "iso");
	truth2d.AddHist("Elrho_iso_2", 10, 0., 50., 100, 0., 150, "rho", "iso");
	truth2d.AddHist("Elrho_iso_3", 10, 0., 50., 100, 0., 150, "rho", "iso");
	truth2d.AddHist("Elrho_iso_4", 10, 0., 50., 100, 0., 150, "rho", "iso");
	truth2d.AddHist("Murho_iso_1", 10, 0., 50., 100, 0., 150, "rho", "iso");
	truth2d.AddHist("Murho_iso_2", 10, 0., 50., 100, 0., 150, "rho", "iso");
	truth2d.AddHist("Murho_iso_3", 10, 0., 50., 100, 0., 150, "rho", "iso");
	truth2d.AddHist("Murho_iso_4", 10, 0., 50., 100, 0., 150, "rho", "iso");
	truth1d.AddHist("TTRECO", 20, 0, 20, "ttreco", "Events");
	truth1d.AddHist("Eff_Bpassing", btagpt, "p_{T} [GeV]", "Events");
	truth1d.AddHist("Eff_Ball", btagpt, "p_{T} [GeV]", "Events");
	truth1d.AddHist("Eff_Lpassing", btagpt, "p_{T} [GeV]", "Events");
	truth1d.AddHist("Eff_Lall", btagpt, "p_{T} [GeV]", "Events");
	truth1d.AddHist("Mu", 200, 0, 100, "#mu", "Events");
	truth1d.AddHist("MuWeighted", 200, 0, 200, "#mu", "Events");
	

	response.AddMatrix("tpt", topptbins, topptbins, "p_{T}(t) [GeV]");
	response.AddMatrix("ty", topybins, topybins, "|y(t)|");
	response.AddMatrix("thadpt", topptbins, topptbins, "p_{T}(t_{h}) [GeV]");
	response.AddMatrix("thady", topybins, topybins, "|y(t_{h})|");
	response.AddMatrix("tleppt", topptbins, topptbins, "p_{T}(t_{l}) [GeV]");
	response.AddMatrix("tlepy", topybins, topybins, "|y(t_{l})|");
	response.AddMatrix("ttm", ttmbins, ttmbins, "m(t#bar{t}) [GeV]");
	response.AddMatrix("ttpt", ttptbins, ttptbins, "p_{T}(t#bar{t}) [GeV]");
	response.AddMatrix("tty", ttybins, ttybins, "|y(t#bar{t})|");
	response.AddMatrix("njet", jetbins, jetbins, "n-jets");
	response.AddMatrix("nobin", nobins, nobins, "total");

	responseall.AddMatrix("tpt", topptbins, topptbins, "p_{T}(t) [GeV]");
	responseall.AddMatrix("ty", topybins, topybins, "|y(t)|");
	responseall.AddMatrix("thadpt", topptbins, topptbins, "p_{T}(t_{h}) [GeV]");
	responseall.AddMatrix("thady", topybins, topybins, "|y(t_{h})|");
	responseall.AddMatrix("tleppt", topptbins, topptbins, "p_{T}(t_{l}) [GeV]");
	responseall.AddMatrix("tlepy", topybins, topybins, "|y(t_{l})|");
	responseall.AddMatrix("ttm", ttmbins, ttmbins, "m(t#bar{t}) [GeV]");
	responseall.AddMatrix("ttpt", ttptbins, ttptbins, "p_{T}(t#bar{t}) [GeV]");
	responseall.AddMatrix("tty", ttybins, ttybins, "|y(t#bar{t})|");
	responseall.AddMatrix("njet", jetbins, jetbins, "n-jets");
	responseall.AddMatrix("nobin", nobins, nobins, "total");

	pdfunc->Add1dHist("pdfunc_tpt", topptbins, "p_{T}(t) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_ty", topybins, "|y(t)|", "Events");
	pdfunc->Add1dHist("pdfunc_thadpt", topptbins, "p_{T}(t_{h}) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_thady", topybins, "|y(t_{h})|", "Events");
	pdfunc->Add1dHist("pdfunc_tleppt", topptbins, "p_{T}(t_{l}) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_tlepy", topybins, "|y(t_{l})|", "Events");
	pdfunc->Add1dHist("pdfunc_ttm", ttmbins, "M(t#bar{t}) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_tty", ttybins, "|y(t#bar{t})|", "Events");
	pdfunc->Add1dHist("pdfunc_ttpt", ttptbins, "p_{T}(t#bar{t}) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_njet", jetbins, "n-jets", "Events");
	pdfunc->Add1dHist("pdfunc_nobin", nobins, "total", "Events");

	pdfunc->Add1dHist("pdfunc_reco_tpt", topptbins, "p_{T}(t) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_reco_ty", topybins, "|y(t)|", "Events");
	pdfunc->Add1dHist("pdfunc_reco_thadpt", topptbins, "p_{T}(t_{h}) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_reco_thady", topybins, "|y(t_{h})|", "Events");
	pdfunc->Add1dHist("pdfunc_reco_tleppt", topptbins, "p_{T}(t_{l}) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_reco_tlepy", topybins, "|y(t_{l})|", "Events");
	pdfunc->Add1dHist("pdfunc_reco_ttm", ttmbins, "M(t#bar{t}) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_reco_tty", ttybins, "|y(t#bar{t})|", "Events");
	pdfunc->Add1dHist("pdfunc_reco_ttpt", ttptbins, "p_{T}(t#bar{t}) [GeV]", "Events");
	pdfunc->Add1dHist("pdfunc_reco_njet", jetbins, "n-jets", "Events");
	pdfunc->Add1dHist("pdfunc_reco_nobin", nobins, "total", "Events");

	ttp_truth.Init(this);
	ttp_right.Init(this);
	ttp_wrong.Init(this);
	ttp_semi.Init(this);
	ttp_other.Init(this);

//	ttp_hadjets_right.Init(this);
//	ttp_hadjets_wrong.Init(this);
//	ttp_jets_right.Init(this);
//	ttp_jets_wrong.Init(this);
//	ttp_blep_right.Init(this);
//	ttp_blep_wrong.Init(this);
	ttp_whad_right.Init(this);
	ttp_whad_wrong.Init(this);
	ttp_jetspos_right.Init(this);
	ttp_jetspos_wrong.Init(this);

	ttp_tlepthad_right.Init(this);
	ttp_tlep_right.Init(this);
	ttp_thad_right.Init(this);
	ttp_nn_right.Init(this);
	ttp_nsemi_right.Init(this);

	btageff.Init(B_TIGHT);
	jetscale.Init("jetscale");

	TDirectory* dir_reco = outFile_.mkdir("RECO");
	dir_reco->cd();
	reco1d.AddHist("counter", 20, 0., 20., "counter", "Events");
	reco2d.AddHist("Wmasshad_tmasshad", 500, 0., 500., 500, 0., 500, "M(W) [GeV]", "M(t) [GeV]");
	reco1d.AddHist("btag_high", 100, 0., 1., "btag", "Events");
	reco1d.AddHist("btag_low", 100, 0., 1., "btag", "Events");
	reco1d.AddHist("bjetmulti", 10, 0., 10., "b-jets", "Events");
	reco1d.AddHist("bjetmultiW", 10, 0., 10., "b-jets", "Events");
	reco1d.AddHist("btagtest", 1000, -100, 100., "-Log(p) btag-test", "Events");
	reco1d.AddHist("masstest", 1000, -100, 100., "-Log(p) mass-test", "Events");
	reco1d.AddHist("nstest", 200, 0, 20., "neutrino-test", "Events");
	reco1d.AddHist("MuIsolation", 200, 0, 2, "rel. iso #mu", "Events");
	reco1d.AddHist("ElIsolation", 200, 0, 2, "rel. iso el", "Events");
	reco1d.AddHist("NumVertices", 200, 0, 200, "num vertices", "Events");
	reco1d.AddHist("NumVerticesWeighted", 200, 0, 200, "num vertices", "Events");
	reco1d.AddHist("Mt_W", 200, 0, 200, "M_{t}(W) [GeV]", "Events");
	ttp_all.Init(this);

	//string probfilename("Prob_parton.root");
	string probfilename("Prob_parton.root");
	//string probfilename("Prob_parton_jecp2.root");
	//string probfilename("Prob_parton_jecm2.root");
	//string probfilename("Prob_parton_jeccenS5.root");
	if(PSEUDOTOP){probfilename = "Prob_pseudo.root";}
	if(BTAGMODE)
	{
		cnbtag = 1;
		ttsolver.Init(probfilename, false, true, true);//for btag
	}
	if(JETSCALEMODE)
	{
		ttsolver.Init(probfilename, true, true, false);//don't use mass info
	}
	else
	{
		ttsolver.Init(probfilename, false, true, true);
		//ttsolver.Init(probfilename, false, true);
	}
	btagweight.Init(this, probfilename);
	string pufile("PUweight.root");
	if(cpileup == -1) pufile = "PUweightm2.root";
	if(cpileup == 1) pufile = "PUweightp2.root";

	TFile* f = TFile::Open(pufile.c_str());
	puhist = (TH1D*)f->Get("muweight");
	TFile* fl = TFile::Open("Lep_SF.root");
	musfhist = (TH1D*)fl->Get("Scale_MuTOT_Pt");
	elsfhist = (TH1D*)fl->Get("Scale_ElTOT_Pt");

}

ttbar::~ttbar()
{
	
	int lscounter = 0;
	for(map<int, set<int> >::const_iterator ita = runinfo.begin() ; ita != runinfo.end() ; ++ita)
	{
		cout << "\"" << ita->first << "\": [[";
		int last = -2;
		int counter = 0;
		for(set<int>::const_iterator itb = ita->second.begin() ; itb != ita->second.end() ; ++itb)
		{
			lscounter++;
			counter++;
			int newval = *itb;
			if(counter == 1 && ita->second.size() == 1)
			{
				cout << newval << "," << newval << "]";
				continue;
			}
			if(counter == 1)
			{
				cout << newval;
				last = newval;
				continue;
			}
			if(counter == ita->second.size())
			{
				cout << "," << newval << "]";
				continue;
			}
			if(newval == last+1)
			{
				last = newval;
				continue;
			}
			else
			{
				cout<< "," << last <<"],[" << newval;
				last = newval;
				continue;
			}
		}
		cout << "]," << endl;
	}
	cout << "Number of LS: " << lscounter << endl;
}

void ttbar::SelectGenParticles(URStreamer& event)
{
	int lepdecays = 0;
	int topcounter = 0;
	//vector<Genparticle> bpartons;	
	if(PSEUDOTOP)
	{
		const vector<Pst>& pseudotops = event.PSTs();
		if(pseudotops.size() == 10)
		{
			topcounter = 2;
			//for(int t = 0 ; t < 10 ; ++t) cout << t << " " << pseudotops[t].pdgId() << ", ";
			//cout << endl;

			if(Abs(pseudotops[8].pdgId()) == 11 || Abs(pseudotops[8].pdgId()) == 13)
			{
				sgenparticles.push_back(pseudotops[8]);
				gencls.push_back(&(sgenparticles.back()));
				genfincls.push_back(&(sgenparticles.back()));
				sgenparticles.push_back(pseudotops[9]);
				gennls.push_back(&(sgenparticles.back()));
				lepdecays++;
			}
			else
			{
				sgenparticles.push_back(pseudotops[8]);
				genwpartons.push_back(&(sgenparticles.back()));
				sgenparticles.push_back(pseudotops[9]);
				genwpartons.push_back(&(sgenparticles.back()));
			}

			if(Abs(pseudotops[3].pdgId()) == 11 || Abs(pseudotops[3].pdgId()) == 13)
			{
				sgenparticles.push_back(pseudotops[3]);
				gencls.push_back(&(sgenparticles.back()));
				genfincls.push_back(&(sgenparticles.back()));
				sgenparticles.push_back(pseudotops[4]);
				gennls.push_back(&(sgenparticles.back()));
				lepdecays++;
			}
			else
			{
				sgenparticles.push_back(pseudotops[3]);
				genwpartons.push_back(&(sgenparticles.back()));
				sgenparticles.push_back(pseudotops[4]);
				genwpartons.push_back(&(sgenparticles.back()));
			}

			if(pseudotops[2].pdgId() == 5)
			{
				sgenparticles.push_back(pseudotops[2]);
				genb = &(sgenparticles.back());
				sgenparticles.push_back(pseudotops[7]);
				genbbar = &(sgenparticles.back());
			}
			else
			{
				sgenparticles.push_back(pseudotops[2]);
				genbbar = &(sgenparticles.back());
				sgenparticles.push_back(pseudotops[7]);
				genb = &(sgenparticles.back());
			}
		}
		//else {cout << "no pseudotop" << endl;}
	}
	else if(HERWIGPP)
	{
		const vector<Genparticle>& gps = event.genParticles();
		for(vector<Genparticle>::const_iterator gp = gps.begin(); gp != gps.end(); ++gp)
		{
			if(gp->status() == 11)
			{
				if(gp->pdgId() == 6 && gps[gp->firstDaughtIdx()].pdgId() != 6)
				{
					//cout << gps[gp->firstDaughtIdx()].pdgId() << endl;
					weight *= 1.+ctopptweight*(gp->Pt()-200.)/1000.;
					topcounter++;
					sgenparticles.push_back(*gp);
					gent = &(sgenparticles.back());
				}
				else if(gp->pdgId() == -6 && gps[gp->firstDaughtIdx()].pdgId() != -6)
				{
					//cout << "P1 " << gps[gp->firstDaughtIdx()].pdgId() << endl;
					topcounter++;
					sgenparticles.push_back(*gp);
					gentbar = &(sgenparticles.back());
				}
				else if(gp->pdgId() == 5 && gps[gp->momIdx()[0]].pdgId() == 6)
				{
					sgenparticles.push_back(*gp);
					genb = &(sgenparticles.back());
				}
				else if(gp->pdgId() == -5 && gps[gp->momIdx()[0]].pdgId() == -6)
				{
					sgenparticles.push_back(*gp);
					genbbar = &(sgenparticles.back());
				}
				else if(Abs(gp->pdgId()) < 6 && gp->momIdx().size() != 0 && Abs(gps[gp->momIdx()[0]].pdgId()) == 24)
				{
					sgenparticles.push_back(*gp);
					genwpartons.push_back(&(sgenparticles.back()));
				}
				else if(gp->momIdx().size() != 0 && Abs(gps[gp->momIdx()[0]].pdgId()) == 24)
				{
					if(Abs(gp->pdgId()) == 11 || Abs(gp->pdgId()) == 13)
					{
						sgenparticles.push_back(*gp);
						gencls.push_back(&(sgenparticles.back()));
						genfincls.push_back(&(sgenparticles.back()));	
					}
					if(Abs(gp->pdgId()) == 12 || Abs(gp->pdgId()) == 14)
					{
						sgenparticles.push_back(*gp);
						gennls.push_back(&(sgenparticles.back()));	
						lepdecays++;
					}
					if(Abs(gp->pdgId()) == 16)
					{
						lepdecays++;
					}
				}
			}
			//if(gp->status() == 1 && gp->momIdx().size() != 0 && (Abs(gps[gp->momIdx()[0]].pdgId()) == 24 || gp->pdgId() == gps[gp->momIdx()[0]].pdgId()))
			//{
			//	if(Abs(gp->pdgId()) == 11 || Abs(gp->pdgId()) == 13)
			//	{
			//		sgenparticles.push_back(*gp);
			//		genfincls.push_back(&(sgenparticles.back()));	
			//	}
			//}
		}
	}
	else
	{
		const vector<Genparticle>& gps = event.genParticles();
		for(vector<Genparticle>::const_iterator gp = gps.begin(); gp != gps.end(); ++gp)
		{
			//if(Abs(gp->pdgId()) == 5 && gp->status() <=70 && gp->status() > 21)
			if(Abs(gp->pdgId()) > 500 && Abs(gp->pdgId()) < 600)
			{
				sgenparticles.push_back(*gp);
				genbpartons.push_back(&(sgenparticles.back()));
					//cout << gp-gps.begin() << " " << gp->pdgId() << " " << gp->status() << " " << (gp->momIdx().size() != 0 ? gps[gp->momIdx()[0]].pdgId():0) << endl;
			}
			//cout << gp-gps.begin() << " " << gp->status() << " " << gp->pdgId() << endl;

			if(gp->status() > 21 && gp->status() < 30 && gp->momIdx().size() != 0)
			{
				if(gp->pdgId() == 6)
				{
					weight *= 1.+ctopptweight*(gp->Pt()-200.)/1000.;
					topcounter++;
					sgenparticles.push_back(*gp);
					gent = &(sgenparticles.back());
				}
				else if(gp->pdgId() == -6)
				{
					topcounter++;
					sgenparticles.push_back(*gp);
					gentbar = &(sgenparticles.back());
				}
				else if(gp->pdgId() == 5 && gps[gp->momIdx()[0]].pdgId() != 24)
				{
					sgenparticles.push_back(*gp);
					genb = &(sgenparticles.back());
				}
				else if(gp->pdgId() == -5 && gps[gp->momIdx()[0]].pdgId() != -24)
				{
					sgenparticles.push_back(*gp);
					genbbar = &(sgenparticles.back());
				}
				else if(Abs(gp->pdgId()) < 6 && Abs(gps[gp->momIdx()[0]].pdgId()) == 24)
				{
					sgenparticles.push_back(*gp);
					genwpartons.push_back(&(sgenparticles.back()));
				}
			}

			if(gp->momIdx().size() != 0 && Abs(gps[gp->momIdx()[0]].pdgId()) == 24)
			{	
				if(Abs(gp->pdgId()) == 11 || Abs(gp->pdgId()) == 13)
				{
					sgenparticles.push_back(*gp);
					gencls.push_back(&(sgenparticles.back()));
				}
				if(Abs(gp->pdgId()) == 12 || Abs(gp->pdgId()) == 14)
				{
					sgenparticles.push_back(*gp);
					gennls.push_back(&(sgenparticles.back()));	
					lepdecays++;
				}
				if(Abs(gp->pdgId()) == 16)
				{
					lepdecays++;
				}
			}

			if(gp->status() == 1 && gp->momIdx().size() != 0 && (Abs(gps[gp->momIdx()[0]].pdgId()) == 24 || gp->pdgId() == gps[gp->momIdx()[0]].pdgId()))
			{
				if(Abs(gp->pdgId()) == 11 || Abs(gp->pdgId()) == 13)
				{
					sgenparticles.push_back(*gp);
					genfincls.push_back(&(sgenparticles.back()));	
				}
			}
		}
	}

	FULLHAD = false;
	SEMILEP = false;
	FULLLEP = false;
	SEMILEPACC = false;
	//cout << topcounter << " " << lepdecays << " " << genwpartons.size() << " " << genfincls.size() << endl;
	if(topcounter == 2 && genb != 0 && genbbar != 0)
	{
		weight *= 1.+cttptweight*((*gent + *gentbar).Pt()-100.)/500.;
		if(lepdecays == 2 && genwpartons.size() == 0)
		{ 
			FULLLEP = true; gen1d["TYP"]->Fill(0.5, weight);
		}
		else if(lepdecays == 1 && genwpartons.size() == 2)
		{
			gen1d["TYP"]->Fill(1.5, weight);
		}
		else if(lepdecays == 0 && genwpartons.size() == 4)
		{
			FULLHAD = true; gen1d["TYP"]->Fill(2.5, weight);
		}
	}
	else
	{
		gen1d["TYP"]->Fill(3.5, weight);
	}

	if(topcounter == 2 && genwpartons.size() == 2 && gencls.size() == 1 && gennls.size() == 1 && genb != 0 && genbbar != 0)//no tau
	{
		SEMILEP = true;
		if(gencls[0]->pdgId() > 0){genbl = genbbar; genbh = genb;} else {genbh = genbbar; genbl = genb;}
		//if(false)
		//{
		//	const vector<Genjet>& genjets = event.genjets();
		//	vector<Genjet> genbjets;
		//	vector<Genjet> genlightjets;
		//	for(vector<Genjet>::const_iterator gja = genjets.begin(); gja != genjets.end(); ++gja)
		//	{
		//		if(gja->Pt() < 20. || Abs(gja->Eta()) > 2.4){continue;}
		//		if(gja->DeltaR(*gencls[0]) < 0.4) {continue;}
		//		if(gja->DeltaR(*gennls[0]) < 0.4) {continue;}
		//		for(int bp = 0 ; bp < bpartons.size() ; ++bp)
		//		{
		//			if(bpartons[bp].DeltaR(*gja) < 0.4)
		//			{
		//				genbjets.push_back(*gja);
		//				goto nextjet;
		//			}
		//		}
		//		genlightjets.push_back(*gja);
		//		nextjet: continue;
		//	}
		//	//cout << bpartons.size() << " " << genbjets.size() << endl;
		//	if(genbjets.size() < 2 || genlightjets.size() < 2)
		//	{
		//		SEMILEP = false;
		//	}
		//	else
		//	{		
		//		double vmin = 1000000000; 
		//		vector<Genjet> res(4);
		//		for(size_t wja = 0 ; wja < genlightjets.size() ; ++wja)
		//		{
		//			for(size_t wjb = 0 ; wjb < wja ; ++wjb)
		//			{
		//				for(size_t bja = 0 ; bja < genbjets.size();  ++bja)
		//				{
		//					for(size_t bjb = 0 ; bjb < genbjets.size();  ++bjb)
		//					{
		//						if(bja == bjb) continue;
		//						//if(gjd->DeltaR(*genbl) > 1.) continue;

		//						double mw = (genlightjets[wja] + genlightjets[wjb]).M();
		//						double mth = (genlightjets[wja] + genlightjets[wjb] + genbjets[bja]).M();
		//						double mtl = (*genfincls[0] + *gennls[0] + genbjets[bjb]).M();
		//						double v = Power(mw-80., 2) + Power(mth-172.5, 2) + Power(mtl-172.5, 2);
		//						if(vmin > v)
		//						{
		//							vmin = v;
		//							res[0] = genlightjets[wja];
		//							res[1] = genlightjets[wjb];
		//							res[2] = genbjets[bja];
		//							res[3] = genbjets[bjb];
		//						}
		//					}
		//				}
		//			}
		//		}
		//		//if(res[0].DeltaR(*genwpartons[0]) < 0.4 || res[0].DeltaR(*genwpartons[1]) < 0.4)
		//		//{
		//		//	cout << "WA" << endl;
		//		//}
		//		//if(res[1].DeltaR(*genwpartons[0]) < 0.4 || res[1].DeltaR(*genwpartons[1]) < 0.4)
		//		//{
		//		//	cout << "WB" << endl;
		//		//}
		//		//if(res[2].DeltaR(*genbh) < 0.4)
		//		//{
		//		//	cout << "Bh" << endl;
		//		//}
		//		//if(res[3].DeltaR(*genbl) < 0.4)
		//		//{
		//		//	cout << "Bl" << endl;
		//		//}
		//		//cout << vmin << " " << bpartons.size() << " " << genbjets.size() << " " << genlightjets.size() << endl;
		//		sgenparticles.push_back(res[0]);
		//		genwpartons[0] = &(sgenparticles.back());
		//		sgenparticles.push_back(res[1]);
		//		genwpartons[1] = &(sgenparticles.back());
		//		sgenparticles.push_back(res[2]);
		//		genbh = &(sgenparticles.back());
		//		sgenparticles.push_back(res[3]);
		//		genbl = &(sgenparticles.back());
		//		if(gencls[0]->pdgId() > 0) {genbbar = genbl; genb = genbh;} else {genb = genbl; genbbar = genbh;}
		//	}
		//}

		sort(genwpartons.begin(), genwpartons.end(), [](GenObject* A, GenObject* B){return(A->Pt() > B->Pt());});
		gentophad = (*genwpartons[0] + *genwpartons[1] + *genbh);
		gentoplep = (*gencls[0] + *gennls[0] + *genbl);

		if(Abs(gencls[0]->Eta()) < cpletamax && gencls[0]->Pt() > cplptmin && Abs(genwpartons[0]->Eta()) < cpjetetamax && Abs(genwpartons[1]->Eta()) < cpjetetamax && Abs(genb->Eta()) < cpjetetamax && Abs(genbbar->Eta()) < cpjetetamax)
		{
			if(Min(genwpartons[0]->Pt(), genwpartons[1]->Pt()) > cpwjetptsoft && Max(genwpartons[0]->Pt(), genwpartons[1]->Pt()) > cpwjetpthard && Min(genb->Pt(), genbbar->Pt()) > cpbjetptsoft && Max(genb->Pt(), genbbar->Pt()) > cpbjetpthard)
			{
				if(genwpartons[0]->DeltaR(*genwpartons[1]) > cpjetsep && genwpartons[0]->DeltaR(*genb) > cpjetsep && genwpartons[0]->DeltaR(*genbbar) > cpjetsep && genwpartons[1]->DeltaR(*genb) > cpjetsep && genwpartons[1]->DeltaR(*genbbar) > cpjetsep && genb->DeltaR(*genbbar) > cpjetsep)
				{
					SEMILEPACC = true;
				}
			}
		}
	}
	else
	{
		genwpartons.clear();
		gencls.clear();
		gennls.clear();
		genb = 0;
		genbbar = 0;
	}

	if(SEMILEPACC)
	{
		const vector<Genparticle>& gps = event.genParticles();
		for(vector<Genparticle>::const_iterator gp = gps.begin(); gp != gps.end(); ++gp)
		{
			if(gp->status() == 1)
			{
				gen1d["DRW"]->Fill(genwpartons[0]->DeltaR(*gp), gp->E()/genwpartons[0]->E());
				gen1d["DRW"]->Fill(genwpartons[1]->DeltaR(*gp), gp->E()/genwpartons[1]->E());
				gen1d["DRB"]->Fill(genb->DeltaR(*gp), gp->E()/genb->E());
				gen1d["DRB"]->Fill(genbbar->DeltaR(*gp), gp->E()/genbbar->E());
			}

		}
	}

	if(SEMILEP)
	{
		const vector<Genjet>& genjets = event.genjets();
		bool wa = false;
		bool wb = false;
		bool ba = false;
		bool bb = false;
		for(vector<Genjet>::const_iterator gj = genjets.begin(); gj != genjets.end(); ++gj)
		{
			if(gj->Pt() < jetptmin || Abs(gj->Eta()) > cjetetamax) {continue;}
			if(gj->DeltaR(*gencls[0]) < 0.4)
			{
				continue;
			}
			if(gj->DeltaR(*genwpartons[0]) < 0.4)
			{
				if(SEMILEPACC && !wa){wa = true; truth1d["foundgen"]->Fill(0.5, weight);}
				continue;
			}
			if(gj->DeltaR(*genwpartons[1]) < 0.4)
			{
				if(SEMILEPACC && !wb){wb = true; truth1d["foundgen"]->Fill(1.5, weight);}
				continue;
			}
			if(gj->DeltaR(*genbbar) < 0.4)
			{
				if(SEMILEPACC && !ba){ba = true; truth1d["foundgen"]->Fill(2.5, weight);}
				continue;
			}
			if(gj->DeltaR(*genb) < 0.4)
			{
				if(SEMILEPACC && !bb){bb = true; truth1d["foundgen"]->Fill(3.5, weight);}
				continue;
			}

			sgenjets.push_back(Genjet(*gj));
			genaddjets.push_back(&(sgenjets.back()));
		}
		if(wa && wb && ba && bb){truth1d["foundgen"]->Fill(4.5, weight);}
	}
}

void ttbar::SelectRecoParticles(URStreamer& event)
{
	const vector<Muon>& muons = event.muons();
	for(vector<Muon>::const_iterator muon = muons.begin(); muon != muons.end(); ++muon)
	{
		IDMuon mu(*muon);
		//if(mu.ID(IDMuon::TIGHT_12) && mu.Pt() > 15.)
		if(gencls.size() == 1 && mu.DeltaR(*gencls[0]) < 0.4)
		{
			double eta = Abs(mu.Eta());
			if(eta < 0.8){truth2d["Murho_iso_1"]->Fill(event.rho().value(), mu.neutralIso() + mu.photonIso(), weight);}
			else if(eta < 1.5){truth2d["Murho_iso_2"]->Fill(event.rho().value(), mu.neutralIso() + mu.photonIso(), weight);}
			else if(eta < 2.1){truth2d["Murho_iso_3"]->Fill(event.rho().value(), mu.neutralIso() + mu.photonIso(), weight);}
			else if(eta < 2.4){truth2d["Murho_iso_4"]->Fill(event.rho().value(), mu.neutralIso() + mu.photonIso(), weight);}
		}
		if(mu.ID(IDMuon::LOOSE_12) && mu.Pt() > 15.)
		{
			smuons.push_back(mu);
			loosemuons.push_back(&(smuons.back()));
			if(MUONS && mu.ID(IDMuon::TIGHT_12) && mu.Pt() > clptmin && Abs(mu.Eta()) < cletamax)
			{
				tightmuons.push_back(&(smuons.back()));
			}
		}
	}

	const vector<Electron>& electrons = event.electrons();
	for(vector<Electron>::const_iterator electron = electrons.begin(); electron != electrons.end(); ++electron)
	{
		IDElectron el(*electron);
		if(gencls.size() == 1 && el.DeltaR(*gencls[0]) < 0.4)
		{
			double eta = Abs(TVector3(el.x(), el.y(), el.z()).Eta());
			if(eta < 0.8){truth2d["Elrho_iso_1"]->Fill(event.rho().value(), el.neutralIso() + el.photonIso(), weight);}
			else if(eta < 1.4442){truth2d["Elrho_iso_2"]->Fill(event.rho().value(), el.neutralIso() + el.photonIso(), weight);}
			else if(eta < 1.566){}
			else if(eta < 2.1){truth2d["Elrho_iso_3"]->Fill(event.rho().value(), el.neutralIso() + el.photonIso(), weight);}
			else if(eta < 2.5){truth2d["Elrho_iso_4"]->Fill(event.rho().value(), el.neutralIso() + el.photonIso(), weight);}
		}
		if(el.ID(IDElectron::LOOSE_15_25ns) && el.Pt() > 15.)
		{
			selectrons.push_back(el);
			looseelectrons.push_back(&(selectrons.back()));
			if(ELECTRONS && el.ID(IDElectron::MEDIUM_15_25ns) && el.Pt() > clptmin && Abs(el.Eta()) < cletamax)
			{
				mediumelectrons.push_back(&(selectrons.back()));
			}
		}
	}

	double  metcorrx = 0.;
	double  metcorry = 0.;
	const vector<Jet>& jets = event.jets();
	for(vector<Jet>::const_iterator jetit = jets.begin(); jetit != jets.end(); ++jetit)
	{
		IDJet jet(*jetit);
		//double sf = gRandom->Gaus(1., 0.05);
		double sf = csigmajet;
		if(sf < 0.){sf *= jetscaler.GetUncM(jet);}
		if(sf > 0.){sf *= jetscaler.GetUncP(jet);}
		if(cjetres > 0.)
		{
			sf = gRandom->Gaus(sf, cjetres);
		}

		sf += 1;
		//double sf = 1. + sigma*0.177829 * Power(jet.Pt(), -0.58647);
		metcorrx -= (sf-1)*jet.Px(); 
		metcorry -= (sf-1)*jet.Py(); 
		jet.SetPxPyPzE(jet.Px()*sf, jet.Py()*sf, jet.Pz()*sf, jet.E()*sf);
		if(jet.Pt() < jetptmin || Abs(jet.Eta()) > cjetetamax) {continue;}
		if(!jet.ID() || !jet.Clean(loosemuons, looseelectrons)) {continue;}

		sjets.push_back(jet);
		cleanedjets.push_back(&(sjets.back()));
	}

	//const vector<Nohfmet>& mets = event.NoHFMETs();
	const vector<Met>& mets = event.METs();
	if(mets.size() == 1)
	{
		met = mets[0];
		met.SetPx(met.Px() + metcorrx + csigmamet*met.pxunc());
		met.SetPy(met.Py() + metcorry + csigmamet*met.pyunc());
		met.SetE(Sqrt(met.Px()*met.Px() + met.Py()*met.Py()));
	}

	//Gen-Reco matching
	if(SEMILEP)
	{

		rightper.MET(&met);
		for(IDElectron* el : mediumelectrons)
		{
			if(el->DeltaR(*genfincls[0]) < 0.2)
			{
				rightper.L(el);
				if(SEMILEPACC){truth1d["found"]->Fill(4.5, weight);}
			}
		}

		for(IDMuon* mu : tightmuons)
		{
			if(mu->DeltaR(*genfincls[0]) < 0.2)
			{
				rightper.L(mu);
				if(SEMILEPACC){truth1d["found"]->Fill(5.5, weight);}
			}
		}

		bool wa = false;
		bool wb = false;
		bool ba = false;
		bool bb = false;
		double ptbmax = 0.;
		double ptbbarmax = 0.;
		vector<double> wjptmax(genwpartons.size(), 0.);
		for(size_t j = 0 ; j < cleanedjets.size() ; ++j)
		{
			IDJet* jet = cleanedjets[j];


			
		//	double jetsep = 0;
		//	for(size_t k = 0 ; k < cleanedjets.size() ; ++k)
		//	{
		//		if(k != j && jet->DeltaR(*cleanedjets[k]) < 0.8)
		//		{
		//			jetsep += cleanedjets[k]->Pt();
		//		}
		//	}
			
			if(jet->DeltaR(*genb) < 0.4 && jet->Pt() > ptbmax)
			{
				ptbmax = jet->Pt();
				if(gencls[0]->pdgId() < 0)
				{
					rightper.BLep(jet);
					if(SEMILEPACC && !ba){ba = true; truth1d["found"]->Fill(0.5, weight);}
				}
				else if(gencls[0]->pdgId() > 0)
				{
					rightper.BHad(jet);
					if(SEMILEPACC && !ba){ba = true; truth1d["found"]->Fill(0.5, weight);}
				}
			}
			if(jet->DeltaR(*genbbar) < 0.4 && jet->Pt() > ptbbarmax)
			{
				ptbbarmax = jet->Pt();
				if(gencls[0]->pdgId() < 0)
				{
					rightper.BHad(jet);
					if(SEMILEPACC && !bb){bb = true; truth1d["found"]->Fill(1.5, weight);}
				}
				else if(gencls[0]->pdgId() > 0)
				{
					rightper.BLep(jet);
					if(SEMILEPACC && !bb){bb = true; truth1d["found"]->Fill(1.5, weight);}
				}
			}
			if(jet->DeltaR(*genwpartons[0]) < 0.4 && jet->Pt() > wjptmax[0])
			{
				wjptmax[0] = jet->Pt();
				rightper.WJa(jet);
				if(SEMILEPACC && !wa){wa = true; truth1d["found"]->Fill(2.5, weight);}
			}
			if(jet->DeltaR(*genwpartons[1]) < 0.4 && jet->Pt() > wjptmax[1])
			{
				wjptmax[1] = jet->Pt();
				rightper.WJb(jet);
				if(SEMILEPACC && !wb){wb = true; truth1d["found"]->Fill(3.5, weight);}
			}
		}
		if(SEMILEPACC && rightper.IsComplete()){truth1d["found"]->Fill(6.5, weight);} 
		//cout << rightper.IsValid() << endl;
		//if(!rightper.IsValid()) {rightper.Reset();}
	}
}

void ttbar::ttanalysis(URStreamer& event)
{
	reco1d["counter"]->Fill(0.5, weight);

	if(SEMILEPACC)
	{
		if(Abs(gencls[0]->pdgId()) == 11) {truth2d["Ne_Nmu"]->Fill(mediumelectrons.size()+0.5, tightmuons.size()+0.5, weight);}
		if(Abs(gencls[0]->pdgId()) == 13) {truth2d["Nmu_Ne"]->Fill(tightmuons.size()+0.5, mediumelectrons.size()+0.5, weight);}
	}

	//keeping only the n leading jets. 
	sort(cleanedjets.begin(), cleanedjets.end(), [](IDJet* A, IDJet* B){return(A->Pt() > B->Pt());});
	int reducedsize = Min(cleanedjets.size(), cnusedjets);
	reducedjets.resize(reducedsize);
	copy(cleanedjets.begin(), cleanedjets.begin()+reducedsize, reducedjets.begin());

	//check for lepton:
	TLorentzVector* lep = 0;
	int lepcharge = 0;
	if(tightmuons.size() == 1 && loosemuons.size() == 1 && looseelectrons.size() == 0)
	{
		lep = dynamic_cast<TLorentzVector*>(tightmuons[0]);
		lepcharge = tightmuons[0]->charge();
	}
	if(loosemuons.size() == 0 && mediumelectrons.size() == 1 && looseelectrons.size() == 1)
	{
		lep = dynamic_cast<TLorentzVector*>(mediumelectrons[0]);
		lepcharge = mediumelectrons[0]->charge();
	}
	if(SEMILEPACC && rightper.IsComplete()){ttp_truth.Fill(rightper, lepcharge, weight);}
	if(lep == 0){return;}

	double nvtx = event.vertexs().size();
	reco1d["NumVertices"]->Fill(nvtx , weight);

	if(isMC)
	{
		//double npu = event.vertexs().size();
		//truth1d["npuorig"]->Fill(npu+0.5, weight);
		//if(npu > 0)
		//{
		//	//weight *= puhist->GetBinContent(npu+1);
		//}
		//truth1d["npu"]->Fill(npu+0.5, weight);
		
		//if(tightmuons.size() == 1){weight *= musfhist->GetBinContent(musfhist->FindFixBin(Min(lep->Pt(), 95.)));}
		//if(mediumelectrons.size() == 1){weight *= elsfhist->GetBinContent(elsfhist->FindFixBin(Min(lep->Pt(), 95.)));}

	}
	reco1d["NumVerticesWeighted"]->Fill(nvtx , weight);

	reco1d["counter"]->Fill(1.5, weight);
	if(SEMILEPACC)
	{
		truth1d["counter"]->Fill(3.5, weight);
	}
	if(rightper.NumTTBarJets() == 4)
	{
		truth2d["dPtJet_right"]->Fill(genwpartons[0]->Pt(), (rightper.WJa()->Pt() - genwpartons[0]->Pt())/genwpartons[0]->Pt(), weight);
		truth2d["dPtJet_right"]->Fill(genwpartons[1]->Pt(), (rightper.WJb()->Pt() - genwpartons[1]->Pt())/genwpartons[1]->Pt(), weight);
		truth2d["dPtbJet_right"]->Fill(genbh->Pt(), (rightper.BHad()->Pt() - genbh->Pt())/genbh->Pt(), weight);
		truth2d["dPtbJet_right"]->Fill(genbl->Pt(), (rightper.BLep()->Pt() - genbl->Pt())/genbl->Pt(), weight);
	}
	//jet number plots
	if(SEMILEP)
	{
		//if(lep != rightper.L()) {cout << "Wrong Lep" << endl;}
		truth2d["Jetstt_JetsAll"]->Fill(rightper.NumTTBarJets()+0.5, cleanedjets.size()+0.5, weight);
	}
	//plot b-tag distribution
	if(rightper.IsComplete())
	{
		truth2d["btag2d_true"]->Fill(Min(rightper.BHad()->csvIncl(), rightper.BLep()->csvIncl()), Max(rightper.BHad()->csvIncl(), rightper.BLep()->csvIncl()), weight);
		truth1d["btag_true"]->Fill(rightper.BHad()->csvIncl(), weight);
		truth1d["btag_true"]->Fill(rightper.BLep()->csvIncl(), weight);
		truth1d["btag_wrong"]->Fill(rightper.WJa()->csvIncl(), weight);
		truth1d["btag_wrong"]->Fill(rightper.WJb()->csvIncl(), weight);
	}

	//cut on number of jets
	if(cleanedjets.size() < 4){return;}
	if(BTAGMODE && cleanedjets.size() > 4){return;}
	reco1d["counter"]->Fill(2.5, weight);
	if(SEMILEPACC) truth1d["counter"]->Fill(4.5, weight);
	if(tightmuons.size() == 1)
	{
		reco1d["MuIsolation"]->Fill(tightmuons[0]->trackiso()/tightmuons[0]->Pt() , weight);
	}
	else
	{
		reco1d["ElIsolation"]->Fill(mediumelectrons[0]->CorPFIsolation2015(), weight);
	}
	double Mt_W = Sqrt(2.*met.Pt()*lep->Pt()-2.*(met.Px()*lep->Px() + met.Py()*lep->Py()));
	reco1d["Mt_W"]->Fill(Mt_W, weight);
	//calculating btag eff.
	sort(reducedjets.begin(), reducedjets.end(), [](IDJet* A, IDJet* B){return(A->csvIncl() > B->csvIncl());});
	int nbjets = count_if(reducedjets.begin(), reducedjets.end(), [&](IDJet* A){return(A->csvIncl() > B_MEDIUM);});
	reco1d["bjetmulti"]->Fill(nbjets, weight);
	if(isMC)
	{
		//double btw = btagweight.SF(reducedjets, cbtagunc, cltagunc);
		//weight *= btw;
	}
	reco1d["bjetmultiW"]->Fill(nbjets, weight);

	//check for b-jets
	reco1d["btag_high"]->Fill(reducedjets[0]->csvIncl(), weight);
	reco1d["btag_low"]->Fill(reducedjets[1]->csvIncl(), weight);
	if(!BTAGMODE)
	{
		if((cnbtag == 1 && reducedjets[0]->csvIncl() < B_TIGHT) || (cnbtag == 2 && reducedjets[1]->csvIncl() < B_MEDIUM)){return;}
	}
	reco1d["counter"]->Fill(3.5, weight);
	if(SEMILEPACC) truth1d["counter"]->Fill(5.5, weight);

	//check what we have reconstructed
	if(SEMILEP)
	{
		truth2d["tt_jets"]->Fill(rightper.NumBJets()+0.5, rightper.NumWJets()+0.5, weight);
	}

	//cout << "SYNC " << event.run << " " << event.lumi << " " << event.evt << endl;

	//reconstruction
	Permutation bestper;

	if(SEMILEPACC && rightper.IsComplete()) truth1d["counter"]->Fill(6.5, weight);

	if(rightper.IsComplete())
	{
		rightper.Solve(ttsolver);
		truth1d["nschi_right"]->Fill(ttsolver.NSChi2()/Sqrt(Abs(ttsolver.NSChi2())), weight);
		truth2d["Wmasshad_tmasshad_right"]->Fill(rightper.THad().M(), rightper.WHad().M(), weight);
	}

	int nbtaglocal = 2;
	if(BTAGMODE) nbtaglocal = 1;
	for(size_t i = nbtaglocal ; i < reducedjets.size() ; ++i)
	{
		for(size_t j = nbtaglocal ; j < i ; ++j)
		{
			for(size_t k = 0 ; k < (nbtaglocal == 2 ? 2 : reducedjets.size()) ; ++k)
			{
				if(i == k || j == k) continue;
				for(size_t l = 0 ; l < (nbtaglocal == 2 ? 2 : reducedjets.size()) ; ++l)
				{
					if(i == l || j == l || k == l) continue;
			//		if(nbtaglocal == 1 && k != 0 && l != 0) continue;
					Permutation testper(reducedjets[i], reducedjets[j], reducedjets[k], reducedjets[l], lep, &met);
					if(testper.WJa()->Pt() < cwjetpthard && testper.WJb()->Pt() < cwjetpthard) continue;
					if(testper.WJa()->Pt() < cwjetptsoft || testper.WJb()->Pt() < cwjetptsoft) continue;
					if(testper.BHad()->Pt() < cbjetpthard && testper.BLep()->Pt() < cbjetpthard) continue;
					if(testper.BHad()->Pt() < cbjetptsoft || testper.BLep()->Pt() < cbjetptsoft) continue;
					testper.Solve(ttsolver);

					TLorentzVector whad(testper.WHad());
					TLorentzVector thad(testper.THad());
					TLorentzVector wlepmiss(*lep + met);
					TLorentzVector tlepmiss(wlepmiss + *testper.BLep());
					reco2d["Wmasshad_tmasshad"]->Fill(whad.M(), thad.M());
					//cout << per++ << " " << ttsolver.NSRes() << " " << ttsolver.BTagRes() << " " << ttsolver.MassRes() << endl;
					if(rightper.IsComplete())
					{
						if(rightper.IsBLepCorrect(testper))
						{
							truth1d["nstest_right"]->Fill(ttsolver.NSRes(), weight);
						}
						else
						{
							truth1d["nstest_wrong"]->Fill(ttsolver.NSRes(), weight);
							truth1d["nschi_wrong"]->Fill(ttsolver.NSChi2()/Sqrt(Abs(ttsolver.NSChi2())), weight);
						}

						if(rightper.AreBsCorrect(testper))
						{
							truth1d["btagtest_right"]->Fill(ttsolver.BTagRes(), weight);
						}
						else
						{
							truth1d["btagtest_wrong"]->Fill(ttsolver.BTagRes(), weight);
						}

						if(rightper.IsCorrect(testper))
						{
							truth2d["Wmasslep_tmasslep_right"]->Fill(wlepmiss.Mt(), tlepmiss.Mt(), weight);
							truth1d["masstest_right"]->Fill(ttsolver.MassRes(), weight);
							truth1d["comtest_right"]->Fill(ttsolver.Res(), weight);
						}
						else
						{
							if(rightper.IsWHadCorrect(testper))
							{
								truth2d["Wmasshad_tmasshad_rightw"]->Fill(whad.M(), thad.M(), weight);
							}
							else
							{
								truth2d["Wmasshad_tmasshad_wrongw"]->Fill(whad.M(), thad.M(), weight);
							}
							truth2d["Wmasshad_tmasshad_wrong"]->Fill(thad.M(), whad.M(), weight);
							truth2d["Wmasslep_tmasslep_wrong"]->Fill(wlepmiss.Mt(), tlepmiss.Mt(), weight);
							truth1d["masstest_wrong"]->Fill(ttsolver.MassRes(), weight);
							truth1d["comtest_wrong"]->Fill(ttsolver.Res(), weight);
						}

					}
					reco1d["btagtest"]->Fill(ttsolver.BTagRes(), weight);
					reco1d["masstest"]->Fill(ttsolver.MassRes(), weight);
					reco1d["nstest"]->Fill(ttsolver.NSRes(), weight);

					if(testper < bestper)
					{
						bestper = testper;
					}
				}
			}
		}
	}

	if(rightper.IsComplete() && reducedjets.size() == 4)
	{
		truth1d["TTRECO"]->Fill(0.5);
		if(rightper.IsWHadCorrect(bestper)) {truth1d["TTRECO"]->Fill(1.5);}
		if(rightper.IsTHadCorrect(bestper)) {truth1d["TTRECO"]->Fill(2.5);}
		if(rightper.IsCorrect(bestper)) {truth1d["TTRECO"]->Fill(3.5);}
		if(!rightper.IsWHadCorrect(bestper) && rightper.IsBHadCorrect(bestper)) {truth1d["TTRECO"]->Fill(4.5);}
		if(!rightper.IsWHadCorrect(bestper) && rightper.IsBLepCorrect(bestper)) {truth1d["TTRECO"]->Fill(5.5);}
		
	}
	if(bestper.Prob() > 1E9){return;}
	if(!BTAGMODE && bestper.MassDiscr() > 10){return;}

	//if(!BTAGMODE || bestper.BHad()->csvIncl() > B_TIGHT || bestper.BLep()->csvIncl() > B_TIGHT)
	if(!BTAGMODE || bestper.BLep()->csvIncl() > B_TIGHT)
{
	reco1d["counter"]->Fill(4.5, weight);
	if(SEMILEPACC && rightper.IsComplete()) truth1d["counter"]->Fill(7.5, weight);
	//Fill reconstructed hists
	ttp_all.Fill(bestper, lepcharge, weight);


	if(SEMILEP)
	{
		pdfunc->Fill1d("pdfunc_reco_tpt", gentophad.Pt(), weight);
		pdfunc->Fill1d("pdfunc_reco_tpt", gentoplep.Pt(), weight);
		pdfunc->Fill1d("pdfunc_reco_ty", Abs(gentophad.Rapidity()), weight);
		pdfunc->Fill1d("pdfunc_reco_ty", Abs(gentoplep.Rapidity()), weight);
		pdfunc->Fill1d("pdfunc_reco_nobin", gentophad.Pt(), weight);
		pdfunc->Fill1d("pdfunc_reco_thadpt", gentophad.Pt(), weight);
		pdfunc->Fill1d("pdfunc_reco_tleppt", gentoplep.Pt(), weight);
		pdfunc->Fill1d("pdfunc_reco_thady", Abs(gentophad.Rapidity()), weight);
		pdfunc->Fill1d("pdfunc_reco_tlepy", Abs(gentoplep.Rapidity()), weight);
		pdfunc->Fill1d("pdfunc_reco_ttm", (gentophad + gentoplep).M(), weight);
		pdfunc->Fill1d("pdfunc_reco_tty", Abs((gentophad + gentoplep).Rapidity()), weight);
		pdfunc->Fill1d("pdfunc_reco_ttpt", (gentophad + gentoplep).Pt(), weight);
		pdfunc->Fill1d("pdfunc_reco_njet", genaddjets.size(), weight);
		responseall.FillReco("tpt", bestper.THad().Pt(), weight);
		responseall.FillReco("ty", Abs(bestper.THad().Rapidity()), weight);
		responseall.FillReco("tpt", bestper.TLep().Pt(), weight);
		responseall.FillReco("ty", Abs(bestper.TLep().Rapidity()), weight);
		responseall.FillReco("nobin", bestper.THad().Pt(), weight);
		responseall.FillReco("thadpt", bestper.THad().Pt(), weight);
		responseall.FillReco("thady", Abs(bestper.THad().Rapidity()), weight);
		responseall.FillReco("tleppt", bestper.TLep().Pt(), weight);
		responseall.FillReco("tlepy", Abs(bestper.TLep().Rapidity()), weight);
		responseall.FillReco("ttm", (bestper.THad() + bestper.TLep()).M(), weight);
		responseall.FillReco("ttpt", (bestper.THad() + bestper.TLep()).Pt(), weight);
		responseall.FillReco("tty", Abs((bestper.THad() + bestper.TLep()).Rapidity()), weight);
		responseall.FillReco("njet", cleanedjets.size() - 4, weight);
		responseall.FillTruthReco("tpt", gentophad.Pt(), bestper.THad().Pt(), weight);
		responseall.FillTruthReco("ty", Abs(gentophad.Rapidity()), Abs(bestper.THad().Rapidity()), weight);
		responseall.FillTruthReco("tpt", gentoplep.Pt(), bestper.TLep().Pt(), weight);
		responseall.FillTruthReco("ty", Abs(gentoplep.Rapidity()), Abs(bestper.TLep().Rapidity()), weight);
		responseall.FillTruthReco("nobin", gentophad.Pt(), bestper.THad().Pt(), weight);
		responseall.FillTruthReco("thadpt", gentophad.Pt(), bestper.THad().Pt(), weight);
		responseall.FillTruthReco("thady", Abs(gentophad.Rapidity()), Abs(bestper.THad().Rapidity()), weight);
		responseall.FillTruthReco("tleppt", gentoplep.Pt(), bestper.TLep().Pt(), weight);
		responseall.FillTruthReco("tlepy", Abs(gentoplep.Rapidity()), Abs(bestper.TLep().Rapidity()), weight);
		responseall.FillTruthReco("ttm", (gentophad + gentoplep).M(), (bestper.THad() + bestper.TLep()).M(), weight);
		responseall.FillTruthReco("ttpt", (gentophad + gentoplep).Pt(), (bestper.THad() + bestper.TLep()).Pt(), weight);
		responseall.FillTruthReco("tty", Abs((gentophad + gentoplep).Rapidity()), Abs((bestper.THad() + bestper.TLep()).Rapidity()), weight);
		responseall.FillTruthReco("njet", genaddjets.size(), cleanedjets.size() - 4, weight);
		truth2d["RES_Mtt_all"]->Fill(((bestper.THad() + bestper.TLep()).M() - (gentophad + gentoplep).M())/(gentophad + gentoplep).M(), (gentophad + gentoplep).M(), weight);
		truth2d["RES_dbeta_all"]->Fill(((bestper.THad().BoostVector() - bestper.TLep().BoostVector()).Mag() - (gentophad.BoostVector() - gentoplep.BoostVector()).Mag())/(gentophad.BoostVector() - gentoplep.BoostVector()).Mag(), (gentophad.BoostVector() - gentoplep.BoostVector()).Mag(), weight);
	}



	//Fill reconstructed hists with matching information
	if(rightper.IsCorrect(bestper))
	{
		ttp_right.Fill(bestper, lepcharge, weight);
		if(SEMILEPACC) truth1d["counter"]->Fill(8.5, weight);
	}
	else if(rightper.IsComplete())
	{
		ttp_wrong.Fill(bestper, lepcharge, weight);
		if(SEMILEPACC) truth1d["counter"]->Fill(9.5, weight);
	}
	else if(SEMILEP)
	{
		ttp_semi.Fill(bestper, lepcharge, weight);
	}
	else
	{
		ttp_other.Fill(bestper, lepcharge, weight);
	}

	if(bestper.IsCorrect(rightper))
	{
		ttp_jetspos_right.Fill(bestper, lepcharge, weight);
		response.FillReco("tpt", bestper.THad().Pt(), weight);
		response.FillReco("ty", Abs(bestper.THad().Rapidity()), weight);
		response.FillReco("tpt", bestper.TLep().Pt(), weight);
		response.FillReco("ty", Abs(bestper.TLep().Rapidity()), weight);
		response.FillReco("nobin", bestper.THad().Pt(), weight);
		response.FillReco("thadpt", bestper.THad().Pt(), weight);
		response.FillReco("thady", Abs(bestper.THad().Rapidity()), weight);
		response.FillReco("tleppt", bestper.TLep().Pt(), weight);
		response.FillReco("tlepy", Abs(bestper.TLep().Rapidity()), weight);
		response.FillReco("ttm", (bestper.THad() + bestper.TLep()).M(), weight);
		response.FillReco("ttpt", (bestper.THad() + bestper.TLep()).Pt(), weight);
		response.FillReco("tty", Abs((bestper.THad() + bestper.TLep()).Rapidity()), weight);
		response.FillReco("njet", cleanedjets.size() - 4, weight);
		if(SEMILEPACC)
		{
			truth1d["counter"]->Fill(10.5, weight);
			response.FillTruthReco("tpt", gentophad.Pt(), bestper.THad().Pt(), weight);
			response.FillTruthReco("ty", Abs(gentophad.Rapidity()), Abs(bestper.THad().Rapidity()), weight);
			response.FillTruthReco("tpt", gentoplep.Pt(), bestper.TLep().Pt(), weight);
			response.FillTruthReco("ty", Abs(gentoplep.Rapidity()), Abs(bestper.TLep().Rapidity()), weight);
			response.FillTruthReco("nobin", gentophad.Pt(), bestper.THad().Pt(), weight);
			response.FillTruthReco("thadpt", gentophad.Pt(), bestper.THad().Pt(), weight);
			response.FillTruthReco("thady", Abs(gentophad.Rapidity()), Abs(bestper.THad().Rapidity()), weight);
			response.FillTruthReco("tleppt", gentoplep.Pt(), bestper.TLep().Pt(), weight);
			response.FillTruthReco("tlepy", Abs(gentoplep.Rapidity()), Abs(bestper.TLep().Rapidity()), weight);
			response.FillTruthReco("ttm", (gentophad + gentoplep).M(), (bestper.THad() + bestper.TLep()).M(), weight);
			response.FillTruthReco("ttpt", (gentophad + gentoplep).Pt(), (bestper.THad() + bestper.TLep()).Pt(), weight);
			response.FillTruthReco("tty", Abs((gentophad + gentoplep).Rapidity()), Abs((bestper.THad() + bestper.TLep()).Rapidity()), weight);
			response.FillTruthReco("njet", genaddjets.size(), cleanedjets.size() - 4, weight);
			truth1d["dRNu_right"]->Fill(bestper.Nu().DeltaR(*gennls[0]), weight);
			truth1d["dPtNu_right"]->Fill((bestper.Nu().Pt() - gennls[0]->Pt())/gennls[0]->Pt(), weight);
			truth1d["dPzNu_right"]->Fill((bestper.Nu().Pz() - gennls[0]->Pz())/gennls[0]->Pz(), weight);
			truth2d["dPzNu_dPhi_right"]->Fill((bestper.Nu().Pz() - gennls[0]->Pz())/gennls[0]->Pz(), Abs(bestper.Nu().DeltaPhi(met)), weight);
			truth2d["dPzNu_chi2_right"]->Fill((bestper.Nu().Pz() - gennls[0]->Pz())/gennls[0]->Pz(), Sqrt(bestper.NuChisq()), weight);
			truth2d["RES_Mtt_right"]->Fill(((bestper.THad() + bestper.TLep()).M() - (gentophad + gentoplep).M())/(gentophad + gentoplep).M(), (gentophad + gentoplep).M(), weight);
			truth2d["RES_dbeta_right"]->Fill(((bestper.THad().BoostVector() - bestper.TLep().BoostVector()).Mag() - (gentophad.BoostVector() - gentoplep.BoostVector()).Mag())/(gentophad.BoostVector() - gentoplep.BoostVector()).Mag(), (gentophad.BoostVector() - gentoplep.BoostVector()).Mag(), weight);
			//if(Abs((bestper.Nu().Pz() - gennls[0]->Pz())/gennls[0]->Pz()) < 0.2)
			//{
			//	cout << (bestper.Nu() + *(bestper.BLep()) + *(bestper.L())).M() << " " << bestper.Nu().Pz() << " " << gennls[0]->Pz() << " "  << bestper.Nu().Pt() << " " << met.Pt() << " " << met.DeltaPhi(bestper.Nu()) << " " << bestper.NuChisq() << endl;
			//	cout << met.Px() << " " << met.Py() << endl;
			//	cout << gennls[0]->Px() << " " << gennls[0]->Py() << " " <<gennls[0]->Pz() << endl;
			//	cout << bestper.Nu().Px() << " " << bestper.Nu().Py() << " " <<bestper.Nu().Pz() << endl;
			//	NeutrinoSolver NS(bestper.L(), bestper.BLep(), 80, 173);
			//	cout << bestper.Nu().Px() << " " << bestper.Nu().Py() << " " <<bestper.Nu().Pz() << endl;
			//	NeutrinoSolver NS(bestper.L(), bestper.BLep(), 80, 173);
			//	double tchi;
			//	TLorentzVector tv(NS.GetBest(met.Px(), met.Py(), Sqrt(met.pxunc()*met.pxunc() + met.pxuncjet()*met.pxuncjet()), Sqrt(met.pyunc()*met.pyunc() + met.pyuncjet()*met.pyuncjet()), 0., tchi, true));
			//	cout << tchi << " " << tv.Pz() << " " << tv.Pt() << endl;
			//}
			truth1d["dRNuMet_right"]->Fill(met.DeltaR(*gennls[0]), weight);
			truth1d["dPtNuMet_right"]->Fill((met.Pt() - gennls[0]->Pt())/gennls[0]->Pt(), weight);
		}
	}
	else
	{
		ttp_jetspos_wrong.Fill(bestper, lepcharge, weight);
	}

	if(bestper.IsCorrect(rightper))
	{
		ttp_tlepthad_right.Fill(bestper, lepcharge, weight);
	}
	else if(bestper.IsTHadCorrect(rightper))
	{
		ttp_thad_right.Fill(bestper, lepcharge, weight);
	}
	else if(bestper.IsBLepCorrect(rightper))
	{
		ttp_tlep_right.Fill(bestper, lepcharge, weight);
	}
	else if(SEMILEP)
	{
		ttp_nn_right.Fill(bestper, lepcharge, weight);
	}
	else
	{
		ttp_nsemi_right.Fill(bestper, lepcharge, weight);
	}
}
	if(JETSCALEMODE)
	{
		if(bestper.IsWHadCorrect(rightper))
		{
			ttp_whad_right.Fill(bestper, lepcharge, weight);
			jetscale.Fill(bestper, true, weight);
		}
		else
		{
			ttp_whad_wrong.Fill(bestper, lepcharge, weight);
			jetscale.Fill(bestper, false, weight);
		}
	}

	if(BTAGMODE){btageff.Fill(bestper, nvtx, rightper.IsCorrect(bestper), weight);}
}

//This method is called once every file, contains the event loop
//run your proper analysis here
void ttbar::analyze()
{

	int nevent = 0;
	URStreamer event(tree_);
	IDElectron::streamer = &event;
	IDMuon::streamer = &event;
	PDFuncertainty::streamer = &event;
	TRandom3 rand(crandomseed);
	while(event.next())
	{
		nevent++;
//		if(nevent % 1000 == 0)cout << "Event:" << nevent << " " << event.run << endl;
		sgenparticles.clear();
		genwpartons.clear();
		gencls.clear();
		genfincls.clear();
		gennls.clear();
		genbpartons.clear();
		genb = 0;
		genbbar = 0;
		genbl = 0;
		genbh = 0;

		sgenjets.clear();
		genaddjets.clear();

		rightper.Reset();

		sjets.clear();
		cleanedjets.clear();
		reducedjets.clear();
		smuons.clear();
		tightmuons.clear();
		loosemuons.clear();
		selectrons.clear();
		mediumelectrons.clear();
		looseelectrons.clear();

		truth1d["counter"]->Fill(0.5);
		weight = 1.;	
		if(event.PUInfos().size() > 0)
		{
			isMC = true;
			const Geninfo& info = event.genInfo();
			weight *= info.weight()/Abs(info.weight());
			const vector<Mcweight>& ws =  event.MCWeights();
			if(cfacscale == -1) weight *= weight*ws[2].weights()/ws[0].weights();
			else if(cfacscale == 1) weight *= weight*ws[1].weights()/ws[0].weights();
			if(crenscale == -1) weight *= weight*ws[6].weights()/ws[0].weights();
			else if(crenscale == 1) weight *= weight*ws[3].weights()/ws[0].weights();

			double npu = event.PUInfos()[0].nInteractions();
			//cout << event.PUInfos()[0].nInteractions() << " " << event.PUInfos()[1].nInteractions() << endl;
			truth1d["Mu"]->Fill(npu, weight);
			//weight *= puhist->GetBinContent(puhist->FindFixBin(npu));
			truth1d["MuWeighted"]->Fill(npu, weight);
		}
		else
		{
			//if(event.run < 251244) {return;}
			runinfo[event.run].insert(event.lumi);
		}

		if(DATASIM && selectionprob > rand.Uniform())
		{
			if(selectionprob > 1.){weight *= selectionprob;}
			SelectGenParticles(event);
			SelectRecoParticles(event);
			ttanalysis(event);
		}

		//if(Abs(event.trigger().HLT_IsoMu24_eta2p1()) != 1) cout << "No mu trigger" << endl;
		//if(Abs(event.trigger().HLT_Ele27_eta2p1_WPLoose_Gsf()) != 1) cout << "No el trigger" << endl;

		else if(!DATASIM)
		{
			SelectGenParticles(event);
			truth1d["counter"]->Fill(19.5, weight);
			if(SEMILEP) 
			{
				truth1d["counter"]->Fill(1.5, weight);
				ttp_genall.Fill(genbh, genwpartons[0], genwpartons[1], genbl, gencls[0], gennls[0], gencls[0]->pdgId(), weight);
				responseall.FillTruth("tpt", gentophad.Pt(), weight);
				responseall.FillTruth("ty", Abs(gentophad.Rapidity()), weight);
				responseall.FillTruth("tpt", gentoplep.Pt(), weight);
				responseall.FillTruth("ty", Abs(gentoplep.Rapidity()), weight);
				responseall.FillTruth("nobin", gentophad.Pt(), weight);
				responseall.FillTruth("thadpt", gentophad.Pt(), weight);
				responseall.FillTruth("thady", Abs(gentophad.Rapidity()), weight);
				responseall.FillTruth("tleppt", gentoplep.Pt(), weight);
				responseall.FillTruth("tlepy", Abs(gentoplep.Rapidity()), weight);
				responseall.FillTruth("ttm", (gentophad + gentoplep).M(), weight);
				responseall.FillTruth("ttpt", (gentophad + gentoplep).Pt(), weight);
				responseall.FillTruth("tty", Abs((gentophad + gentoplep).Rapidity()), weight);
				responseall.FillTruth("njet", genaddjets.size(), weight);
			}
			if(SEMILEPACC)
			{
				ttp_genacc.Fill(genbh, genwpartons[0], genwpartons[1], genbl, gencls[0], gennls[0], gencls[0]->pdgId(), weight);
				truth1d["counter"]->Fill(2.5, weight);

				response.FillTruth("tpt", gentophad.Pt(), weight);
				response.FillTruth("ty", Abs(gentophad.Rapidity()), weight);
				response.FillTruth("tpt", gentoplep.Pt(), weight);
				response.FillTruth("ty", Abs(gentoplep.Rapidity()), weight);
				response.FillTruth("nobin", gentophad.Pt(), weight);
				response.FillTruth("thadpt", gentophad.Pt(), weight);
				response.FillTruth("thady", Abs(gentophad.Rapidity()), weight);
				response.FillTruth("tleppt", gentoplep.Pt(), weight);
				response.FillTruth("tlepy", Abs(gentoplep.Rapidity()), weight);
				response.FillTruth("ttm", (gentophad + gentoplep).M(), weight);
				response.FillTruth("ttpt", (gentophad + gentoplep).Pt(), weight);
				response.FillTruth("tty", Abs((gentophad + gentoplep).Rapidity()), weight);
				response.FillTruth("njet", genaddjets.size(), weight);

				pdfunc->Fill1d("pdfunc_tpt", gentophad.Pt(), weight);
				pdfunc->Fill1d("pdfunc_tpt", gentoplep.Pt(), weight);
				pdfunc->Fill1d("pdfunc_ty", Abs(gentophad.Rapidity()), weight);
				pdfunc->Fill1d("pdfunc_ty", Abs(gentoplep.Rapidity()), weight);
				pdfunc->Fill1d("pdfunc_nobin", gentophad.Pt(), weight);
				pdfunc->Fill1d("pdfunc_thadpt", gentophad.Pt(), weight);
				pdfunc->Fill1d("pdfunc_tleppt", gentoplep.Pt(), weight);
				pdfunc->Fill1d("pdfunc_thady", Abs(gentophad.Rapidity()), weight);
				pdfunc->Fill1d("pdfunc_tlepy", Abs(gentoplep.Rapidity()), weight);
				pdfunc->Fill1d("pdfunc_ttm", (gentophad + gentoplep).M(), weight);
				pdfunc->Fill1d("pdfunc_tty", Abs((gentophad + gentoplep).Rapidity()), weight);
				pdfunc->Fill1d("pdfunc_ttpt", (gentophad + gentoplep).Pt(), weight);
				pdfunc->Fill1d("pdfunc_njet", genaddjets.size(), weight);
			}
		  //cout << event.filter().Flag_goodVertices() << " " <<  event.filter().Flag_CSCTightHaloFilter() << " " << event.filter().Flag_HBHENoiseFilter() << " " << event.filter().HBHEnew() << endl;
					  //event.filter().Flag_goodVertices() == 1 && event.filter().Flag_CSCTightHaloFilter() == 1 &&
if(!isMC && (Abs(event.trigger().HLT_IsoMu20()) != 1 || Abs(event.trigger().HLT_Ele22_eta2p1_WPLoose_Gsf()) != 1)) {cout << "TRIGGER UNDEFINED:" << event.trigger().HLT_IsoMu20() << " " << event.trigger().HLT_Ele22_eta2p1_WPLoose_Gsf() << endl; }
			if(
					(
					 isMC && 
					 (
					  event.trigger().HLT_IsoMu27() == 1
					  || event.trigger().HLT_Ele27_WP85_Gsf() == 1
					 )
					) ||
					(
					 !isMC &&
					 (
					  event.filter().Flag_goodVertices() == 1 && event.filter().Flag_CSCTightHaloFilter() == 1 && event.filter().HBHEnew() == 1 &&
					  (
					   //event.trigger().HLT_IsoMu24_eta2p1() == 1 || (event.trigger().HLT_IsoMu24_eta2p1() == -1 && event.trigger().HLT_Ele27_eta2p1_WPLoose_Gsf() == 1)
					   event.trigger().HLT_IsoMu20() == 1 || (event.trigger().HLT_IsoMu20() == -1 && event.trigger().HLT_Ele22_eta2p1_WPLoose_Gsf() == 1)
					  )
					 )
					)
			  )
			{
				SelectRecoParticles(event);
				ttanalysis(event);
			}
		}

	}
}


//make it executable
int main(int argc, char *argv[])
{
	URParser &parser = URParser::instance(argc, argv);
	URDriver<ttbar> test;
	return test.run();
}
