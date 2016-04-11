#include "ttbarxsec.h"
#include "TRandom3.h"

#include "Permutation.h"
#include "PDFuncertainty.h"
#include "NeutrinoSolver.h"
#include "ConfigParser.h"
//#include "jet3.h"

using namespace std;

ttbar::ttbar(const std::string output_filename):
	AnalyzerBase("ttbar", output_filename),
        reco3j2d("3j"),
        reco3j1d("3j"),
        gen3j2d("3j"), 
        gen3j1d("3j"),
        //fourjets("4j"), 
	gen1d("gen"),
	gen2d("gen"),
	ttp_genall("genall"),
	ttp_genacc("genacc"),
	ttp_genjet("genjet"),
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
	//ttp_jetspos_right("jetspos_right"),
	//ttp_jetspos_wrong("jetspos_wrong"),
	//	ttp_hadjets_right("hadjets_right"),
	//	ttp_hadjets_wrong("hadjets_wrong"),
	//	ttp_jets_right("jets_right"),
	//	ttp_jets_wrong("jets_wrong"),
	//	ttp_blep_right("blep_right"),
	//	ttp_blep_wrong("blep_wrong"),
	//ttp_whad_right("whad_right"),
	//ttp_whad_wrong("whad_wrong"),
	ttp_tlepthad_right("ttp_tlepthad_right"),
	ttp_tlep_right("ttp_tlep_right"),
	ttp_thad_right("ttp_thad_right"),
	ttp_nn_right("ttp_nn_right"),
	ttp_nsemi_right("ttp_nsemi_right"),
        yuka1d_gen("yukawa"),
        yuka1d_reco("yukawa"),
        yuka1d_reco_right("yukawa"),
        yuka1d_reco_wrong("yukawa"),
        yuka1d_reco_semi("yukawa"),
        yuka1d_reco_other("yukawa"),
        yuka2d_gen("yukawa"),
        yuka2d_reco("yukawa"),
        yuka2d_reco_right("yukawa"),
        yuka2d_reco_wrong("yukawa"),
        yuka2d_reco_semi("yukawa"),
        yuka2d_reco_other("yukawa"),
	response("response", this),
	response2d("response"),
	response_ps("response_ps", this),
	jetscaler("jetuncertainty.root"),
	PDFTEST(false),
	PSEUDOTOP(true),
	BTAGMODE(false), //set true for the b-tag efficiency measurement
	JETSCALEMODE(false), //set true for jet scale measurement
	ELECTRONS(true),
	MUONS(true),
	B_TIGHT(0.935),
	B_MEDIUM(0.800),
	B_LOOSE(0.460),
	cnbtag(2), //1: one thight b-jet, 2: two medium b-jets
	cnusedjets(1000), //only nused jets, ordered by pT are used for the permutations
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
	HERWIGPP(false),
	PYTHIA6(false) 
        //yukawatxt("yukawa_reweighing1.0.root")
{
	ConfigParser CP("ttbarxsec.cfg");
	PSEUDOTOP = CP.Get<bool>("PSEUDOTOP");
	BTAGMODE = CP.Get<bool>("BTAGMODE");
	ELECTRONS = CP.Get<bool>("ELECTRONS");
	MUONS = CP.Get<bool>("MUONS");
	IDMuon::USEISO = CP.Get<bool>("LEPTONISO");
	IDElectron::USEISO = CP.Get<bool>("LEPTONISO");
	cnbtag = CP.Get<int>("nbtag");
	clikelihoodcut = CP.Get<double>("likelihoodcut");

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
		PDFTEST = CP.Get<bool>("PDFTEST");
	}
	cbtagunc = CP.Get<int>("btagunc");
	cltagunc = CP.Get<int>("ltagunc");
	cpileup = CP.Get<int>("pileupunc");

        //yukawatxt = CP.Get<const char>("yukawa");

	cout << output_filename << endl;
	if(output_filename.find("Hpp") != string::npos){HERWIGPP = true;}
	if(output_filename.find("P6") != string::npos){PYTHIA6 = true;}

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

	////topptbins = {0.0, 65.0, 100.0, 135.0, 175.0, 240.0, 500.0};
	//topptbins = {0.0, 60.0, 100.0, 150.0, 200.0, 260.0, 320.0, 400.0, 500.0};
	////topybins = {0.0, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 2.5};
	//topybins = {0.0, 0.25, 0.5, 0.75, 1.0, 1.5, 2.5};
	//ttmbins = {280., 400.0, 450.0, 510.0, 580.0, 720.0, 1400.0};
	//ttybins = {0.0, 0.2, 0.4, 0.6, 0.85, 1.25, 2.5};
	//ttptbins = {0.0, 25.0, 40.0, 60.0, 85.0, 150.0, 500.0};
	//metbins = {0.0, 30.0, 45.0, 60.0, 80.0, 120.0, 580.0};
	//jetbins = {-0.5, 0.5, 1.5, 2.5, 3.5};
	//nobins = {0., 13000.};
	
	topptbins = {0.0, 45.0, 65.0, 85.0, 100.0, 120.0, 140.0, 160.0, 180.0, 210.0, 250.0, 350.0, 600.0};
	topybins = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1.0, 1.2, 1.5, 2.0, 2.5};
	ttmbins = {300., 360.0, 400.0, 430.0, 455.0, 480.0, 510.0, 545.0, 585.0, 640.0, 720.0, 870.0, 1800.0};
	ttybins = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.75, 0.9, 1.1, 1.4, 2.5};
	ttptbins = {0.0, 15.0, 25.0, 35.0, 45.0, 55., 70.0, 90.0, 110.0, 150.0, 350.0, 500.0};
	metbins = {0.0, 30.0, 45.0, 60.0, 80.0, 120.0, 580.0};
	jetbins = {-0.5, 0.5, 1.5, 2.5, 3.5, 4.5, 5.5};
	dybins = {-2.0, -1.5, -1., -0.5, 0., 0.5, 1.0, 1.5, 2.0};
	nobins = {0., 13000.};

	btagpt = {30., 50., 70., 100., 140., 200., 300., 670.};
	//vector<string> testpdf = {"CT10", "CT10as", "NNPDF30_nnlo_as_0118"};
	//vector<string> testpdf = {"CT10nlo", "NNPDF30_nlo_as_0118", "MMHT2014nlo68clas118"};
	vector<string> testpdf;
	if(PDFTEST) {pdfunc = new PDFuncertainty("NNPDF30_nlo_as_0118", 0, testpdf);}

	runbins = {254231., 254232., 254790., 254852., 254879., 254906., 254907., 254914., 256630., 256673., 256674., 256675., 256676., 256677., 256801., 256842., 256843., 256866., 256867., 256868., 256869., 256926., 256941., 257461., 257531., 257599., 257613., 257614., 257645., 257682., 257722., 257723., 257735., 257751., 257804., 257805., 257816., 257819., 257968., 257969., 258129., 258136., 258157., 258158., 258159., 258177., 258211., 258213., 258214., 258215., 258287., 258403., 258425., 258426., 258427., 258428., 258432., 258434., 258440., 258444., 258445., 258446., 258448., 258655., 258656., 258694., 258702., 258703., 258705., 258706., 258712., 258713., 258714., 258741., 258742., 258745., 258749., 258750., 259626., 259637., 259681., 259683., 259685., 259686., 259721., 259809., 259810., 259811., 259813., 259817., 259818., 259820., 259821., 259822., 259861., 259862., 259884., 259890., 259891., 260373., 260424., 260425., 260426., 260427., 260431., 260532., 260533., 260534., 260536., 260538., 260541., 260575., 260576., 260577., 260593., 260627., 260628.};

}

void ttbar::begin()
{
	outFile_.cd();

        TDirectory* dir_3j_gen = outFile_.mkdir("3j_GEN");
        dir_3j_gen->cd();
        gen3j1d.AddHist("tlep_pt", 400, 0, 800, "p_{T}(t_{l})", "Events");
        gen3j1d.AddHist("thad_pt", 400, 0, 800, "p_{T}(t_{h})", "Events");
        gen3j1d.AddHist("tlep_y", 200, 0, 5, "|y(t_{l})|", "Events");
        gen3j1d.AddHist("thad_y", 200, 0, 5, "|y(t_{h})|", "Events");
        gen3j1d.AddHist("tlep_M", 500, 0, 1000, "M(t_{l})", "Events");
        gen3j1d.AddHist("thad_M", 500, 0, 1000, "M(t_{h})", "Events");
        gen3j1d.AddHist("tt_pt", 500, 0, 1000," p_{t}(t#bar{t})", "Event");
        gen3j1d.AddHist("tt_y", 200, 0, 5, "|y(t#bar{t})|", "Event");
        gen3j1d.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
        gen3j1d.AddHist("delY", 1200, -6, 6, "#Deltay(t#bar{t})", "Events");
        gen3j2d.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay(t#bar{t})");

        gen3j1d.AddHist("thadmiss_e", 250, 0, 500, "miss-jet E", "Events");
        gen3j1d.AddHist("thadmiss_pt", 250, 0, 500, "miss-jet p_{T}", "Events"); 
        gen3j1d.AddHist("thadmiss_y", 200, 0, 5, "miss-jet |y|", "Events");
        gen3j1d.AddHist("thadmiss_DeltaR", 100, 0, 5, "W-jets #DeltaR", "Events");
        gen3j1d.AddHist("delpt_pt", 20, 0, 2, "#Deltap_{T}(t_{h})/p_{T}(t_{h})", "Events");
        gen3j1d.AddHist("dely_y", 20, 0, 2, "#Deltay(t_{h})/y(t_{h})", "Events");

        TDirectory* dir_3j_reco = outFile_.mkdir("3j_RECO");
        dir_3j_reco->cd();
        reco3j1d.AddHist("tlep_pt", 400, 0, 800, "p_{T}(t_{l})", "Events");
        reco3j1d.AddHist("thad_pt", 400, 0, 800, "p_{T}(t_{h})", "Events");
        reco3j1d.AddHist("tlep_y", 200, 0, 5, "|y(t_{l})|", "Events");
        reco3j1d.AddHist("thad_y", 200, 0, 5, "|y(t_{h})|", "Events");
        reco3j1d.AddHist("tlep_M", 500, 0, 1000, "M(t_{l})", "Events");
        reco3j1d.AddHist("thad_M", 500, 0, 1000, "M(t_{h})", "Events");
        reco3j1d.AddHist("tt_pt", 500, 0, 1000," p_{t}(t#bar{t})", "Event");
        reco3j1d.AddHist("tt_y", 200, 0, 5, "|y(t#bar{t})|", "Event");
        reco3j1d.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
        reco3j1d.AddHist("delY", 1200, -6, 6, "#Deltay(t#bar{t})", "Events");
        reco3j2d.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay(t#bar{t})");
       
        reco3j2d.AddHist("selectchi2", 400, 0, 200, 400, 0, 200, "#chi^{2} b[0]", "#chi^{2} b[1]");
        reco3j2d.AddHist("selectcsv", 12, 0.4, 1, 12, 0.4, 1, "CSV b[0]", "CSV b[1]");
        reco3j1d.AddHist("blep_chi2", 400, 0, 200, "#chi^{2} right", "Events");
        reco3j1d.AddHist("blepwrong_chi2", 400, 0, 200, "#chi^{2} wrong", "Events");
        reco3j2d.AddHist("blep_bhad_csv", 12, 0.4, 1, 12, 0.4, 1, "CSV b_{l}", "CSV b_{h}");
        reco3j2d.AddHist("blep_bhad_pt", 250, 0, 500, 250, 0, 500, "p_{T}(b_{l})", "p_{T}(b_{h})");
        reco3j1d.AddHist("thadmiss_e", 250, 0, 500, "miss-jet E", "Events");
        reco3j1d.AddHist("thadmiss_pt", 250, 0, 500, "miss-jet p_{T}", "Events"); 
        reco3j1d.AddHist("thadmiss_y", 200, 0, 5, "miss-jet |y|", "Events");
        reco3j1d.AddHist("thadmiss_DeltaR", 100, 0, 5, "W-jets #DeltaR", "Events");
        reco3j1d.AddHist("delpt_pt", 20, 0, 2, "#Deltap_{T}(t_{h})/p_{T}(t_{h})", "Events");
        reco3j1d.AddHist("dely_y", 20, 0, 2, "#Deltay(t_{h})/y(t_{h})", "Events");


        /*threejets.AddHist("Mtt_exact3j", 1000, 0, 2000, "M(t#bar{t})", "Events");
        threejets.AddHist("Mtt_exact4j", 1000, 0, 2000, "M(t#bar{t})", "Events");
        threejets.AddHist("Mtt_exact5j", 1000, 0, 2000, "M(t#bar{t})", "Events");
        threejets.AddHist("Mtt_above3j", 1000, 0, 2000, "M(t#bar{t})", "Events");
        threejets.AddHist("Mtt_above4j", 1000, 0, 2000, "M(t#bar{t})", "Events");
        threejets.AddHist("Mtt_above5j", 1000, 0, 2000, "M(t#bar{t})", "Events");*/

        /*fourjets.AddHist("Mtt_exact4j", 1000, 0, 2000, "M(t#bar{t})", "Events");
        fourjets.AddHist("Mtt_exact5j", 1000, 0, 2000, "M(t#bar{t})", "Events");
        fourjets.AddHist("Mtt_above4j", 1000, 0, 2000, "M(t#bar{t})", "Events");
        fourjets.AddHist("Mtt_above5j", 1000, 0, 2000, "M(t#bar{t})", "Events");*/
	
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
    ttp_genjet.Init(this);

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
	truth2d.AddHist("Wmasshad_tmasshad_right", 500, 0., 500., 500, 0., 500, "M(t_{h}) [GeV]", "M(W_{h}) [GeV]");
	truth2d.AddHist("Wmtlep_tmtlep_right", 500, 0., 500., 500, 0., 500, "M_{T}(t_{l}) [GeV]", "M(W_{l}) [GeV]");
	truth2d.AddHist("Wmasshad_tmasshad_wrong", 500, 0., 500., 500, 0., 500, "M(t) [GeV]", "M(W) [GeV]");
	truth2d.AddHist("Wmtlep_tmtlep_wrong", 500, 0., 500., 500, 0., 500, "M_{T}(t_{l}) [GeV]", "M(W_{l}) [GeV]");
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
	truth1d.AddHist("Eff_BpassingM", btagpt, "p_{T} [GeV]", "Events");
	truth1d.AddHist("Eff_BpassingL", btagpt, "p_{T} [GeV]", "Events");
	truth1d.AddHist("Eff_Ball", btagpt, "p_{T} [GeV]", "Events");
	truth1d.AddHist("Eff_CpassingM", btagpt, "p_{T} [GeV]", "Events");
	truth1d.AddHist("Eff_CpassingL", btagpt, "p_{T} [GeV]", "Events");
	truth1d.AddHist("Eff_Call", btagpt, "p_{T} [GeV]", "Events");
	truth1d.AddHist("Eff_LpassingM", btagpt, "p_{T} [GeV]", "Events");
	truth1d.AddHist("Eff_LpassingL", btagpt, "p_{T} [GeV]", "Events");
	truth1d.AddHist("Eff_Lall", btagpt, "p_{T} [GeV]", "Events");
	truth1d.AddHist("Mu", 100, 0, 100, "#mu", "Events");
	truth1d.AddHist("MuWeighted", 100, 0, 100, "#mu", "Events");

	response.AddMatrix("thardpt", topptbins, topptbins, "high p_{T}(t) [GeV]");
	response.AddMatrix("tsoftpt", topptbins, topptbins, "soft p_{T}(t) [GeV]");
	response.AddMatrix("thadpt", topptbins, topptbins, "p_{T}(t_{h}) [GeV]");
	response.AddMatrix("thady", topybins, topybins, "|y(t_{h})|");
	response.AddMatrix("tleppt", topptbins, topptbins, "p_{T}(t_{l}) [GeV]");
	response.AddMatrix("tlepy", topybins, topybins, "|y(t_{l})|");
	response.AddMatrix("ttm", ttmbins, ttmbins, "m(t#bar{t}) [GeV]");
	response.AddMatrix("ttpt", ttptbins, ttptbins, "p_{T}(t#bar{t}) [GeV]");
	response.AddMatrix("tty", ttybins, ttybins, "|y(t#bar{t})|");
	response.AddMatrix("njet", jetbins, jetbins, "n-jets");
	response.AddMatrix("nobin", nobins, nobins, "total");
	response.AddMatrix("dymp", dybins, dybins, "|y(t)|-|y(#bar{t})|");

	vector<double> jetbins_large = {-0.5, 0.5, 1.5, 2.5, 3.5};
	vector<double> topptbins_large = {0.0, 60.0, 100.0, 140.0, 180.0, 250.0, 500.0};
	response2d.AddMatrix("njets_thadpt", jetbins_large, topptbins_large, jetbins_large, topptbins_large);
	vector<double> ttptbins_large = {0.0, 25.0, 45.0, 70.0, 110.0, 200., 500.0};
	response2d.AddMatrix("njets_ttpt", jetbins_large, ttptbins_large, jetbins_large, ttptbins_large);

	response_ps.AddMatrix("thardpt", topptbins, topptbins, "hard p_{T}(t_{h}) [GeV]");
	response_ps.AddMatrix("tsoftpt", topptbins, topptbins, "soft p_{T}(t_{h}) [GeV]");
	response_ps.AddMatrix("thadpt", topptbins, topptbins, "p_{T}(t_{h}) [GeV]");
	response_ps.AddMatrix("thady", topybins, topybins, "|y(t_{h})|");
	response_ps.AddMatrix("tleppt", topptbins, topptbins, "p_{T}(t_{l}) [GeV]");
	response_ps.AddMatrix("tlepy", topybins, topybins, "|y(t_{l})|");
	response_ps.AddMatrix("ttm", ttmbins, ttmbins, "m(t#bar{t}) [GeV]");
	response_ps.AddMatrix("ttpt", ttptbins, ttptbins, "p_{T}(t#bar{t}) [GeV]");
	response_ps.AddMatrix("tty", ttybins, ttybins, "|y(t#bar{t})|");

	if(PDFTEST)
	{
		pdfunc->Add1dHist("pdfunc_thardpt", topptbins, "hard p_{T}(t) [GeV]", "Events");
		pdfunc->Add1dHist("pdfunc_tsoftpt", topptbins, "soft p_{T}(t) [GeV]", "Events");
		pdfunc->Add1dHist("pdfunc_thadpt", topptbins, "p_{T}(t_{h}) [GeV]", "Events");
		pdfunc->Add1dHist("pdfunc_thady", topybins, "|y(t_{h})|", "Events");
		pdfunc->Add1dHist("pdfunc_tleppt", topptbins, "p_{T}(t_{l}) [GeV]", "Events");
		pdfunc->Add1dHist("pdfunc_tlepy", topybins, "|y(t_{l})|", "Events");
		pdfunc->Add1dHist("pdfunc_ttm", ttmbins, "M(t#bar{t}) [GeV]", "Events");
		pdfunc->Add1dHist("pdfunc_tty", ttybins, "|y(t#bar{t})|", "Events");
		pdfunc->Add1dHist("pdfunc_ttpt", ttptbins, "p_{T}(t#bar{t}) [GeV]", "Events");
		pdfunc->Add1dHist("pdfunc_njet", jetbins, "n-jets", "Events");
		pdfunc->Add1dHist("pdfunc_nobin", nobins, "total", "Events");
		pdfunc->Add1dHist("pdfunc_dymp", dybins, "|y(t)|-|y(#bar{t})|", "Events");
		pdfunc->Add1dHist("pdfunc_njets_thadpt", response2d.GetNBins("njets_thadpt"), 0., response2d.GetNBins("njets_thadpt") , "bin", "Events");
		pdfunc->Add1dHist("pdfunc_njets_ttpt", response2d.GetNBins("njets_ttpt"), 0., response2d.GetNBins("njets_ttpt") , "bin", "Events");

		pdfunc->Add1dHist("pdfunc_reco_thardpt", topptbins, "hard p_{T}(t) [GeV]", "Events");
		pdfunc->Add1dHist("pdfunc_reco_tsoftpt", topptbins, "soft p_{T}(t) [GeV]", "Events");
		pdfunc->Add1dHist("pdfunc_reco_thadpt", topptbins, "p_{T}(t_{h}) [GeV]", "Events");
		pdfunc->Add1dHist("pdfunc_reco_thady", topybins, "|y(t_{h})|", "Events");
		pdfunc->Add1dHist("pdfunc_reco_tleppt", topptbins, "p_{T}(t_{l}) [GeV]", "Events");
		pdfunc->Add1dHist("pdfunc_reco_tlepy", topybins, "|y(t_{l})|", "Events");
		pdfunc->Add1dHist("pdfunc_reco_ttm", ttmbins, "M(t#bar{t}) [GeV]", "Events");
		pdfunc->Add1dHist("pdfunc_reco_tty", ttybins, "|y(t#bar{t})|", "Events");
		pdfunc->Add1dHist("pdfunc_reco_ttpt", ttptbins, "p_{T}(t#bar{t}) [GeV]", "Events");
		pdfunc->Add1dHist("pdfunc_reco_njet", jetbins, "n-jets", "Events");
		pdfunc->Add1dHist("pdfunc_reco_nobin", nobins, "total", "Events");
		pdfunc->Add1dHist("pdfunc_reco_dymp", dybins, "|y(t)|-|y(#bar{t})|", "Events");
		pdfunc->Add1dHist("pdfunc_reco_njets_thadpt", response2d.GetNBins("njets_thadpt"), 0., response2d.GetNBins("njets_thadpt") , "bin", "Events");
		pdfunc->Add1dHist("pdfunc_reco_njets_ttpt", response2d.GetNBins("njets_ttpt"), 0., response2d.GetNBins("njets_ttpt") , "bin", "Events");
	}

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
//	ttp_whad_right.Init(this);
//	ttp_whad_wrong.Init(this);
//	ttp_jetspos_right.Init(this);
//	ttp_jetspos_wrong.Init(this);

	ttp_tlepthad_right.Init(this);
	ttp_tlep_right.Init(this);
	ttp_thad_right.Init(this);
	ttp_nn_right.Init(this);
	ttp_nsemi_right.Init(this);

	btageff.Init(B_MEDIUM);

	TDirectory* dir_reco = outFile_.mkdir("RECO");
	dir_reco->cd();
	reco1d.AddHist("counter", 20, 0., 20., "counter", "Events");
	reco1d.AddHist("c_lep", runbins, "c_lep", "Events");
	reco1d.AddHist("c_mu", runbins, "c_mu", "Events");
	reco1d.AddHist("c_el", runbins, "c_el", "Events");
	reco1d.AddHist("c_jets", runbins, "c_jets", "Events");
	reco1d.AddHist("c_btag", runbins, "c_btag", "Events");
	reco1d.AddHist("c_algo", runbins, "c_algo", "Events");
	reco2d.AddHist("Wmasshad_tmasshad", 500, 0., 500., 500, 0., 500, "M(W) [GeV]", "M(t) [GeV]");
	reco1d.AddHist("btag_high", 100, 0., 1., "btag", "Events");
	reco1d.AddHist("btag_low", 100, 0., 1., "btag", "Events");
	reco1d.AddHist("bjetmulti", 10, 0., 10., "b-jets", "Events");
	reco1d.AddHist("jetmulti", 20, 0., 20., "jets", "Events");
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

        TDirectory* dir_yukawagen = outFile_.mkdir("YUKAWA_GEN");
	dir_yukawagen->cd();
	yuka1d_gen.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
	yuka1d_gen.AddHist("costheta", 40, -1, 1, "cos#theta", "Events");
	yuka1d_gen.AddHist("Y", 160, -4, 4,"y_t", "Events");
	yuka1d_gen.AddHist("delY", 1200, -6, 6, "#Deltay_{t#bar{t}}", "Events");
	yuka1d_gen.AddHist("delBeta", 200, 0, 2, "#Delta#beta_{t#bar{t}}", "Events");
        yuka2d_gen.AddHist("Mtt_costheta", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
        yuka2d_gen.AddHist("delY_costheta", 1200, -6, 6, 40, -1, 1, "#Deltay(t#bar{t})", "cos#theta");
	yuka2d_gen.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay_{t#bar{t}}");
	yuka2d_gen.AddHist("Mtt_delBeta", 1000, 0, 2000, 200, 0, 2, "M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	yuka2d_gen.AddHist("delY_delBeta", 1200, -6, 6, 200, 0, 2, "#Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");	
	
	yuka1d_gen.AddHist("Mtt_boo", 1000, 0, 2000, "CM M(t#bar{t})", "Events");
	yuka1d_gen.AddHist("costheta_boo", 40, -1, 1, "CM cos#theta", "Events");
	yuka1d_gen.AddHist("delY_boo", 1200, -6, 6,"CM #Deltay_{t#bar{t}}", "Events");
	yuka1d_gen.AddHist("delBeta_boo", 200, 0, 2, "CM #Delta#beta_{t#bar{t}}", "Events");
        yuka2d_gen.AddHist("Mtt_costheta_boo", 1000, 0, 2000, 40, -1, 1, "CM M(t#bar{t})", "CM cos#theta");
	yuka2d_gen.AddHist("Mtt_delY_boo", 1000, 0, 2000, 1200, -6, 6, "CM M(t#bar{t})", "#Deltay_{t#bar{t}}");
	yuka2d_gen.AddHist("Mtt_delBeta_boo", 1000, 0, 2000, 200, 0, 2, "CM M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	yuka2d_gen.AddHist("delY_delBeta_boo", 1200, -6, 6, 200, 0, 2, "CM #Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

        yuka1d_gen.AddHist("tlepy", 200, 0, 5, "|y(t_{l}|", "Events");
        yuka2d_gen.AddHist("Mtt_tlepy", 1000, 0, 2000, 200, 0, 5, "M(t#bar{t})", "|y(t_{l}|");
        yuka2d_gen.AddHist("delY_tlepy", 1200, -6, 6, 200, 0, 5, "#Deltay_{t#bar{t}}", "|y(t_{l}|");


	TDirectory* dir_yukawareco = outFile_.mkdir("YUKAWA_RECO");
	dir_yukawareco->cd();
	yuka1d_reco.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
	yuka1d_reco.AddHist("costheta", 40, -1, 1, "cos#theta", "Events");
	yuka1d_reco.AddHist("Y", 160, -4, 4,"y_t", "Events");
	yuka1d_reco.AddHist("delY", 1200, -6, 6,"#Deltay_{t#bar{t}}", "Events");
	yuka1d_reco.AddHist("delBeta", 200, 0, 2, "#Delta#beta_{t#bar{t}}", "Events");
        yuka2d_reco.AddHist("Mtt_costheta", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
        yuka2d_reco.AddHist("delY_costheta", 1200, -6, 6, 40, -1, 1, "#Deltay(t#bar{t})", "cos#theta");
	yuka2d_reco.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay_{t#bar{t}}");
	yuka2d_reco.AddHist("Mtt_delBeta", 1000, 0, 2000, 200, 0, 2, "M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	yuka2d_reco.AddHist("delY_delBeta", 1200, -6, 6, 200, 0, 2, "#Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

	yuka1d_reco.AddHist("Mtt_boo", 1000, 0, 2000, "CM M(t#bar{t})", "Events");
	yuka1d_reco.AddHist("costheta_boo", 40, -1, 1, "CM cos#theta", "Events");
	yuka1d_reco.AddHist("delY_boo", 1200, -6, 6,"CM #Deltay_{t#bar{t}}", "Events");
	yuka1d_reco.AddHist("delBeta_boo", 200, 0, 2, "CM #Delta#beta_{t#bar{t}}", "Events");
        yuka2d_reco.AddHist("Mtt_costheta_boo", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
	yuka2d_reco.AddHist("Mtt_delY_boo", 1000, 0, 2000, 1200, -6, 6, "CM M(t#bar{t})", "#Deltay_{t#bar{t}}");
	yuka2d_reco.AddHist("Mtt_delBeta_boo", 1000, 0, 2000, 200, 0, 2, "CM M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	yuka2d_reco.AddHist("delY_delBeta_boo", 1200, -6, 6, 200, 0, 2, "CM #Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

        yuka1d_reco.AddHist("tlepy", 200, 0, 5, "|y(t_{l}|", "Events");
        yuka2d_reco.AddHist("Mtt_tlepy", 1000, 0, 2000, 200, 0, 5, "M(t#bar{t})", "|y(t_{l}|");
        yuka2d_reco.AddHist("delY_tlepy", 1200, -6, 6, 200, 0, 5, "#Deltay_{t#bar{t}}", "|y(t_{l}|");
	
        TDirectory* dir_yukawareco_right = outFile_.mkdir("YUKAWA_RECO_right");
	dir_yukawareco_right->cd();
	yuka1d_reco_right.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
	yuka1d_reco_right.AddHist("costheta", 40, -1, 1, "cos#theta", "Events");
	yuka1d_reco_right.AddHist("Y", 160, -4, 4,"y_t", "Events");
	yuka1d_reco_right.AddHist("delY", 1200, -6, 6,"#Deltay_{t#bar{t}}", "Events");
	yuka1d_reco_right.AddHist("delBeta", 200, 0, 2, "#Delta#beta_{t#bar{t}}", "Events");
        yuka2d_reco_right.AddHist("Mtt_costheta", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
        yuka2d_reco_right.AddHist("delY_costheta", 1200, -6, 6, 40, -1, 1, "#Deltay(t#bar{t})", "cos#theta");
	yuka2d_reco_right.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay_{t#bar{t}}");
	yuka2d_reco_right.AddHist("Mtt_delBeta", 1000, 0, 2000, 200, 0, 2, "M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	yuka2d_reco_right.AddHist("delY_delBeta", 1200, -6, 6, 200, 0, 2, "#Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

	yuka1d_reco_right.AddHist("Mtt_boo", 1000, 0, 2000, "CM M(t#bar{t})", "Events");
	yuka1d_reco_right.AddHist("costheta_boo", 40, -1, 1, "CM cos#theta", "Events");
	yuka1d_reco_right.AddHist("delY_boo", 1200, -6, 6,"CM #Deltay_{t#bar{t}}", "Events");
	yuka1d_reco_right.AddHist("delBeta_boo", 200, 0, 2, "CM #Delta#beta_{t#bar{t}}", "Events");
        yuka2d_reco_right.AddHist("Mtt_costheta_boo", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
	yuka2d_reco_right.AddHist("Mtt_delY_boo", 1000, 0, 2000, 1200, -6, 6, "CM M(t#bar{t})", "#Deltay_{t#bar{t}}");
	yuka2d_reco_right.AddHist("Mtt_delBeta_boo", 1000, 0, 2000, 200, 0, 2, "CM M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	yuka2d_reco_right.AddHist("delY_delBeta_boo", 1200, -6, 6, 200, 0, 2, "CM #Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

        yuka1d_reco_right.AddHist("tlepy", 200, 0, 5, "|y(t_{l}|", "Events");
        yuka2d_reco_right.AddHist("Mtt_tlepy", 1000, 0, 2000, 200, 0, 5, "M(t#bar{t})", "|y(t_{l}|");
        yuka2d_reco_right.AddHist("delY_tlepy", 1200, -6, 6, 200, 0, 5, "#Deltay_{t#bar{t}}", "|y(t_{l}|");

	TDirectory* dir_yukawareco_wrong = outFile_.mkdir("YUKAWA_RECO_wrong");
	dir_yukawareco_wrong->cd();
	yuka1d_reco_wrong.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
	yuka1d_reco_wrong.AddHist("costheta", 40, -1, 1, "cos#theta", "Events");
	yuka1d_reco_wrong.AddHist("Y", 160, -4, 4,"y_t", "Events");
	yuka1d_reco_wrong.AddHist("delY", 1200, -6, 6,"#Deltay_{t#bar{t}}", "Events");
	yuka1d_reco_wrong.AddHist("delBeta", 200, 0, 2, "#Delta#beta_{t#bar{t}}", "Events");
        yuka2d_reco_wrong.AddHist("Mtt_costheta", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
        yuka2d_reco_wrong.AddHist("delY_costheta", 1200, -6, 6, 40, -1, 1, "#Deltay(t#bar{t})", "cos#theta");
	yuka2d_reco_wrong.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay_{t#bar{t}}");
	yuka2d_reco_wrong.AddHist("Mtt_delBeta", 1000, 0, 2000, 200, 0, 2, "M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	yuka2d_reco_wrong.AddHist("delY_delBeta", 1200, -6, 6, 200, 0, 2, "#Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

	yuka1d_reco_wrong.AddHist("Mtt_boo", 1000, 0, 2000, "CM M(t#bar{t})", "Events");
	yuka1d_reco_wrong.AddHist("costheta_boo", 40, -1, 1, "CM cos#theta", "Events");
	yuka1d_reco_wrong.AddHist("delY_boo", 1200, -6, 6,"CM #Deltay_{t#bar{t}}", "Events");
	yuka1d_reco_wrong.AddHist("delBeta_boo", 200, 0, 2, "CM #Delta#beta_{t#bar{t}}", "Events");
        yuka2d_reco_wrong.AddHist("Mtt_costheta_boo", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
	yuka2d_reco_wrong.AddHist("Mtt_delY_boo", 1000, 0, 2000, 1200, -6, 6, "CM M(t#bar{t})", "#Deltay_{t#bar{t}}");
	yuka2d_reco_wrong.AddHist("Mtt_delBeta_boo", 1000, 0, 2000, 200, 0, 2, "CM M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	yuka2d_reco_wrong.AddHist("delY_delBeta_boo", 1200, -6, 6, 200, 0, 2, "CM #Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

        yuka1d_reco_wrong.AddHist("tlepy", 200, 0, 5, "|y(t_{l}|", "Events");
        yuka2d_reco_wrong.AddHist("Mtt_tlepy", 1000, 0, 2000, 200, 0, 5, "M(t#bar{t})", "|y(t_{l}|");
        yuka2d_reco_wrong.AddHist("delY_tlepy", 1200, -6, 6, 200, 0, 5, "#Deltay_{t#bar{t}}", "|y(t_{l}|");

	TDirectory* dir_yukawareco_semi = outFile_.mkdir("YUKAWA_RECO_semi");
	dir_yukawareco_semi->cd();
	yuka1d_reco_semi.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
	yuka1d_reco_semi.AddHist("costheta", 40, -1, 1, "cos#theta", "Events");
	yuka1d_reco_semi.AddHist("Y", 160, -4, 4,"y_t", "Events");
	yuka1d_reco_semi.AddHist("delY", 1200, -6, 6,"#Deltay_{t#bar{t}}", "Events");
	yuka1d_reco_semi.AddHist("delBeta", 200, 0, 2, "#Delta#beta_{t#bar{t}}", "Events");
        yuka2d_reco_semi.AddHist("Mtt_costheta", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
        yuka2d_reco_semi.AddHist("delY_costheta", 1200, -6, 6, 40, -1, 1, "#Deltay(t#bar{t})", "cos#theta");
	yuka2d_reco_semi.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay_{t#bar{t}}");
	yuka2d_reco_semi.AddHist("Mtt_delBeta", 1000, 0, 2000, 200, 0, 2, "M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	yuka2d_reco_semi.AddHist("delY_delBeta", 1200, -6, 6, 200, 0, 2, "#Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

	yuka1d_reco_semi.AddHist("Mtt_boo", 1000, 0, 2000, "CM M(t#bar{t})", "Events");
	yuka1d_reco_semi.AddHist("costheta_boo", 40, -1, 1, "CM cos#theta", "Events");
	yuka1d_reco_semi.AddHist("delY_boo", 1200, -6, 6,"CM #Deltay_{t#bar{t}}", "Events");
	yuka1d_reco_semi.AddHist("delBeta_boo", 200, 0, 2, "CM #Delta#beta_{t#bar{t}}", "Events");
        yuka2d_reco_semi.AddHist("Mtt_costheta_boo", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
	yuka2d_reco_semi.AddHist("Mtt_delY_boo", 1000, 0, 2000, 1200, -6, 6, "CM M(t#bar{t})", "#Deltay_{t#bar{t}}");
	yuka2d_reco_semi.AddHist("Mtt_delBeta_boo", 1000, 0, 2000, 200, 0, 2, "CM M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	yuka2d_reco_semi.AddHist("delY_delBeta_boo", 1200, -6, 6, 200, 0, 2, "CM #Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");


        yuka1d_reco_semi.AddHist("tlepy", 200, 0, 5, "|y(t_{l}|", "Events");
        yuka2d_reco_semi.AddHist("Mtt_tlepy", 1000, 0, 2000, 200, 0, 5, "M(t#bar{t})", "|y(t_{l}|");
        yuka2d_reco_semi.AddHist("delY_tlepy", 1200, -6, 6, 200, 0, 5, "#Deltay_{t#bar{t}}", "|y(t_{l}|");

	TDirectory* dir_yukawareco_other = outFile_.mkdir("YUKAWA_RECO_other");
	dir_yukawareco_other->cd();
	yuka1d_reco_other.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
	yuka1d_reco_other.AddHist("costheta", 40, -1, 1, "cos#theta", "Events");
	yuka1d_reco_other.AddHist("Y", 160, -4, 4,"y_t", "Events");
	yuka1d_reco_other.AddHist("delY", 1200, -6, 6,"#Deltay_{t#bar{t}}", "Events");
	yuka1d_reco_other.AddHist("delBeta", 200, 0, 2, "#Delta#beta_{t#bar{t}}", "Events");
        yuka2d_reco_other.AddHist("Mtt_costheta", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
        yuka2d_reco_other.AddHist("delY_costheta", 1200, -6, 6, 40, -1, 1, "#Deltay(t#bar{t})", "cos#theta");
	yuka2d_reco_other.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay_{t#bar{t}}");
	yuka2d_reco_other.AddHist("Mtt_delBeta", 1000, 0, 2000, 200, 0, 2, "M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	yuka2d_reco_other.AddHist("delY_delBeta", 1200, -6, 6, 200, 0, 2, "#Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

	yuka1d_reco_other.AddHist("Mtt_boo", 1000, 0, 2000, "CM M(t#bar{t})", "Events");
	yuka1d_reco_other.AddHist("costheta_boo", 40, -1, 1, "CM cos#theta", "Events");
	yuka1d_reco_other.AddHist("delY_boo", 1200, -6, 6,"CM #Deltay_{t#bar{t}}", "Events");
	yuka1d_reco_other.AddHist("delBeta_boo", 200, 0, 2, "CM #Delta#beta_{t#bar{t}}", "Events");
        yuka2d_reco_other.AddHist("Mtt_costheta_boo", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
	yuka2d_reco_other.AddHist("Mtt_delY_boo", 1000, 0, 2000, 1200, -6, 6, "CM M(t#bar{t})", "#Deltay_{t#bar{t}}");
	yuka2d_reco_other.AddHist("Mtt_delBeta_boo", 1000, 0, 2000, 200, 0, 2, "CM M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	yuka2d_reco_other.AddHist("delY_delBeta_boo", 1200, -6, 6, 200, 0, 2, "CM #Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

        yuka1d_reco_other.AddHist("tlepy", 200, 0, 5, "|y(t_{l}|", "Events");
        yuka2d_reco_other.AddHist("Mtt_tlepy", 1000, 0, 2000, 200, 0, 5, "M(t#bar{t})", "|y(t_{l}|");
        yuka2d_reco_other.AddHist("delY_tlepy", 1200, -6, 6, 200, 0, 5, "#Deltay_{t#bar{t}}", "|y(t_{l}|");

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
	btagweight.Init(this, "btaggingeff.root", cbtagunc, cltagunc);
	string puhistname("pu_central");
	if(cpileup == -1) puhistname = "pu_minus";
	if(cpileup == 1) puhistname = "pu_plus";

	TFile* f = TFile::Open("PUweight.root");
	puhist = (TH1D*)f->Get(puhistname.c_str());
	TFile* fl = TFile::Open("Lep_SF.root");
	musfhist = (TH2D*)fl->Get("MuTOT");
	elsfhist = (TH2D*)fl->Get("ElTOT");
	mutrgsfhist = (TH2D*)fl->Get("MuISOTRG");
	eltrgsfhist = (TH2D*)fl->Get("ElISOTRG");
	// this is for yukawa studies:
	//TFile* fyuka_beta = TFile::Open("yukawa2_beta.root");
	//yukahist_beta = (TH1D*)fyuka_beta->Get("XSR_beta");
	
        TFile* fyuka_2d = TFile::Open("yukawa_reweighing1.0.root");
	yukahist_2d = (TH2D*)fyuka_2d->Get("EWtoLO");


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
/*
void ttbar::SelectGenParticles(URStreamer& event)
{
	int lepdecays = 0;
	int topcounter = 0;
	vector<GenObject*> genwpartons;
	vector<GenObject*> gencls;
	vector<GenObject*> gennls;
	GenObject* genb = 0;
	GenObject* genbbar = 0;
	const vector<Genparticle>& gps = event.genParticles();
	for(vector<Genparticle>::const_iterator gp = gps.begin(); gp != gps.end(); ++gp)
	{
		if(int(Abs(gp->pdgId()) % 10000) / 1000 == 5 || int(Abs(gp->pdgId()) % 1000) / 100 == 5)
		{
			sgenparticles.push_back(*gp);
			genbpartons.push_back(&(sgenparticles.back()));
		}
		else if(int(Abs(gp->pdgId()) % 10000) / 1000 == 4 || int(Abs(gp->pdgId()) % 1000) / 100 == 4)
		{
			sgenparticles.push_back(*gp);
			gencpartons.push_back(&(sgenparticles.back()));
		}
		if(HERWIGPP)
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
		else
		{

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
		SEMILEPACC = true;
		if(gencls[0]->pdgId() > 0){genallper.Init(genwpartons[0], genwpartons[1], genb, genbbar, gencls[0], gencls[0]->pdgId(), gennls[0]);}
		else{genallper.Init(genwpartons[0], genwpartons[1], genbbar, genb, gencls[0], gencls[0]->pdgId(), gennls[0]);}

	//	if(Abs(genallper.L()->Eta()) < cpletamax && genallper.L()->Pt() > cplptmin && Abs(genallper.WJa()->Eta()) < cpjetetamax && Abs(genallper.WJb()->Eta()) < cpjetetamax && Abs(genallper.BHad()->Eta()) < cpjetetamax && Abs(genallper.BLep()->Eta()) < cpjetetamax)
	//	{
	//		if(Min(genallper.WJa()->Pt(), genallper.WJb()->Pt()) > cpwjetptsoft && Max(genallper.WJa()->Pt(), genallper.WJb()->Pt()) > cpwjetpthard && Min(genallper.BHad()->Pt(), genallper.BLep()->Pt()) > cpbjetptsoft && Max(genallper.BHad()->Pt(), genallper.BLep()->Pt()) > cpbjetpthard)
	//		{
	//			if(genallper.WJa()->DeltaR(*genallper.WJb()) > cpjetsep && genallper.WJa()->DeltaR(*genallper.BHad()) > cpjetsep && genallper.WJa()->DeltaR(*genallper.BLep()) > cpjetsep && genallper.WJb()->DeltaR(*genallper.BHad()) > cpjetsep && genallper.WJb()->DeltaR(*genallper.BLep()) > cpjetsep && genallper.BHad()->DeltaR(*genallper.BLep()) > cpjetsep)
	//			{
	//				SEMILEPACC = true;
	//			}
	//		}
	//	}
	}

	if(SEMILEPACC)
	{
		const vector<Genparticle>& gps = event.genParticles();
		for(vector<Genparticle>::const_iterator gp = gps.begin(); gp != gps.end(); ++gp)
		{
			if(gp->status() == 1)
			{
				gen1d["DRW"]->Fill(genallper.WJa()->DeltaR(*gp), gp->E()/genallper.WJa()->E());
				gen1d["DRW"]->Fill(genallper.WJb()->DeltaR(*gp), gp->E()/genallper.WJb()->E());
				gen1d["DRB"]->Fill(genallper.BHad()->DeltaR(*gp), gp->E()/genallper.BHad()->E());
				gen1d["DRB"]->Fill(genallper.BLep()->DeltaR(*gp), gp->E()/genallper.BLep()->E());
			}

		}
	}

//Jet studies
	if(SEMILEP)
	{
		Permutation jetper;
		jetper.Nu(gennls[0]);
		if(Abs(gencls[0]->Eta()) < cpletamax && gencls[0]->Pt() > cplptmin)
		{
			jetper.L(gencls[0], gencls[0]->pdgId());
		}
		double wa = 0.;
		double wb = 0.;
		double bl = 0.;
		double bh = 0.;
		const vector<Genjet>& genjets = event.genjets();
		for(vector<Genjet>::const_iterator Gj = genjets.begin(); Gj != genjets.end(); ++Gj)
		{
			if(Gj->Pt() < jetptmin || Abs(Gj->Eta()) > cjetetamax) {continue;}
			sgenjets.push_back(Genjet(*Gj));
			Genjet* gj = &(sgenjets.back());
			if(gj->DeltaR(*gencls[0]) < 0.4)
			{
				continue;
			}
			if(gj->DeltaR(*genallper.WJa()) < 0.4 && gj->Pt() > wa)
			{
				wa = gj->Pt();
				jetper.WJa(gj);
			}
			if(gj->DeltaR(*genallper.WJb()) < 0.4 && gj->Pt() > wb)
			{
				wb = gj->Pt();
				jetper.WJb(gj);
			}
			if(gj->DeltaR(*genallper.BLep()) < 0.4 && gj->Pt() > bl)
			{
				bl = gj->Pt();
				jetper.BLep(gj);
			}
			if(gj->DeltaR(*genallper.BHad()) < 0.4 && gj->Pt() > bh)
			{
				bh = gj->Pt();
				jetper.BHad(gj);
			}
		}
		if(jetper.IsComplete())
		{
			ttp_genjet.Fill(jetper, weight);
		}
	}
}*/
void ttbar::SelectGenParticles(URStreamer& event)
{
    int lepdecays = 0;
    int topcounter = 0;
    vector<GenObject*> genwpartons;
    vector<GenObject*> gencls;
    vector<GenObject*> gennls;
    GenObject* genb = 0;
    GenObject* genbbar = 0;
    GenObject* genwm = 0;
    GenObject* genwp = 0;
    TLorentzVector* genmiss = 0;
    TLorentzVector* genmisspartner = 0;

//      const vector<Pxlhe>& lhepx = event.PXLHEs();
//      const vector<Pylhe>& lhepy = event.PYLHEs();
//      const vector<Pzlhe>& lhepz = event.PZLHEs();
//      const vector<Elhe>& lhee = event.ELHEs();
//      const vector<Pdgidlhe>& lhepdgid = event.PDGIDLHEs();
//      const vector<Statuslhe>& lhestatus = event.STATUSLHEs();
//      int npnlo = event.NPNLOLHE().npnlo();
//      for(size_t s = 0 ; s < lhepx.size() ; ++s)
//      {
//              TLorentzVector tm(lhepx[s].px(), lhepy[s].py(), lhepz[s].pz(), lhee[s].e());
//              cout << lhestatus[s].status() << " lhe " << lhepdgid[s].pdgid() << " " << tm.M() << endl;
//      }

    const vector<Genparticle>& gps = event.genParticles();
    for(vector<Genparticle>::const_iterator igp = gps.begin(); igp != gps.end(); ++igp)
    {
        Genparticle gp(*igp);
        gp.SetE(gp.e());

        //cout << gp.status() << " " << gp.pdgId() << " " << gp.M() << " " << gp.Pt() << endl;

        if(int(Abs(gp.pdgId()) % 10000) / 1000 == 5 || int(Abs(gp.pdgId()) % 1000) / 100 == 5)
        {
            sgenparticles.push_back(gp);
            genbpartons.push_back(&(sgenparticles.back()));
        }
        else if(int(Abs(gp.pdgId()) % 10000) / 1000 == 4 || int(Abs(gp.pdgId()) % 1000) / 100 == 4)
        {
            sgenparticles.push_back(gp);
            gencpartons.push_back(&(sgenparticles.back()));
        }
        if(HERWIGPP)
        {
            if(gp.status() == 11)
            {
                if(gp.pdgId() == 6 && gps[gp.firstDaughtIdx()].pdgId() != 6)
                {
                    weight *= 1.+ctopptweight*(gp.Pt()-200.)/1000.;
                    //weight *= 1.+ctoprapweight*(0.2-0.2*Abs(gp.Rapidity()));
                    topcounter++;
                    gentq = gp;
                }
                else if(gp.pdgId() == -6 && gps[gp.firstDaughtIdx()].pdgId() != -6)
                {
                    topcounter++;
                    gentqbar = gp;
                }
                else if(gp.pdgId() == 5 && gps[gp.momIdx()[0]].pdgId() == 6)
                {
                    int dindex = igp-gps.begin();
                    for(int i = 0 ; i < 2 ; ++i)
                    {
                        dindex = gps[dindex].firstDaughtIdx();
                    }
                    sgenparticles.push_back(gps[dindex]);
                    genb = &(sgenparticles.back());
                    //cout << "B " << gps[dindex].pdgId() << " " << gps[gps[dindex].firstDaughtIdx()].pdgId() << endl;
                }
                else if(gp.pdgId() == -5 && gps[gp.momIdx()[0]].pdgId() == -6)
                {
                    int dindex = igp-gps.begin();
                    for(int i = 0 ; i < 2 ; ++i)
                    {
                        dindex = gps[dindex].firstDaughtIdx();
                    }
                    sgenparticles.push_back(gps[dindex]);
                    genbbar = &(sgenparticles.back());
                    //cout << "Bb " << gps[dindex].pdgId() << " " << gps[gps[dindex].firstDaughtIdx()].pdgId() << endl;
                }
                else if(Abs(gp.pdgId()) < 6 && gp.momIdx().size() != 0 && Abs(gps[gp.momIdx()[0]].pdgId()) == 24)
                {
                    int dindex = igp-gps.begin();
                    for(int i = 0 ; i < 1 ; ++i)
                    {
                        dindex = gps[dindex].firstDaughtIdx();
                    }
                    //cout << "Q " << gps[dindex].pdgId() << " " << gps[gps[dindex].firstDaughtIdx()].pdgId() << endl;
                    sgenparticles.push_back(gps[dindex]);
                    genwpartons.push_back(&(sgenparticles.back()));
	            sort(genwpartons.begin(), genwpartons.end(), [](GenObject* A, GenObject* B){return(A->Pt() > B->Pt());});//for 3j
                    genmiss = genwpartons[1];
                    genmiss = genwpartons[0];
                }
                else if(gp.momIdx().size() != 0 && Abs(gps[gp.momIdx()[0]].pdgId()) == 24)
                {
                    if(Abs(gp.pdgId()) == 11 || Abs(gp.pdgId()) == 13)
                    {
                        int dindex = igp-gps.begin();
                        for(int i = 0 ; i < 1 ; ++i)
                        {
                            dindex = gps[dindex].firstDaughtIdx();
                        }
                        //cout << gps[dindex].pdgId() << " lc " << gps[dindex].status() << endl;
                        sgenparticles.push_back(gps[dindex]);
                        gencls.push_back(&(sgenparticles.back()));
                        //genfincls.push_back(&(sgenparticles.back()));
                        lepdecays++;
                    }
                    if(Abs(gp.pdgId()) == 12 || Abs(gp.pdgId()) == 14)
                    {
                        int dindex = igp-gps.begin();
                        for(int i = 0 ; i < 1 ; ++i)
                        {
                            dindex = gps[dindex].firstDaughtIdx();
                        }
                        //cout << gps[dindex].pdgId() << " ln " << gps[dindex].status() << endl;
                        sgenparticles.push_back(gps[dindex]);
                        gennls.push_back(&(sgenparticles.back()));
                    }
                    if(Abs(gp.pdgId()) == 16)
                    {
                        //cout << "tau" << endl;
                        lepdecays++;
                    }
                }
            }
        }
        else
        {

            //if(gp.status() > 21 && gp.status() < 30 && gp.momIdx().size() != 0)
            if(gp.status() > 60 && gp.status() < 70)
            {
                if(gp.pdgId() == 6)
                {
                    weight *= 1.+ctopptweight*(gp.Pt()-200.)/1000.;
                    //weight *= 1.+ctoprapweight*(0.2-0.2*Abs(gp.Rapidity()));
                    topcounter++;
                    gentq = gp;
                }
                else if(gp.pdgId() == -6)
                {
                    topcounter++;
                    gentqbar = gp;
                }
            }
            if(gp.status() > 21 && gp.status() < 30 && gp.momIdx().size() != 0)
            {
                if(gp.pdgId() == 5 && gps[gp.momIdx()[0]].pdgId() != 24)
                {
                    sgenparticles.push_back(gp);
                    genb = &(sgenparticles.back());
                }
                else if(gp.pdgId() == -5 && gps[gp.momIdx()[0]].pdgId() != -24)
                {
                    sgenparticles.push_back(gp);
                    genbbar = &(sgenparticles.back());
                }
                else if(Abs(gp.pdgId()) < 6 && Abs(gps[gp.momIdx()[0]].pdgId()) == 24)
                {
                    sgenparticles.push_back(gp);
                    genwpartons.push_back(&(sgenparticles.back()));
	            sort(genwpartons.begin(), genwpartons.end(), [](GenObject* A, GenObject* B){return(A->Pt() > B->Pt());});//for 3j
                    genmiss = genwpartons[1];
                    genmisspartner = genwpartons[0];
                }
            }

            if(gp.momIdx().size() != 0 && Abs(gps[gp.momIdx()[0]].pdgId()) == 24)
            {
                if(Abs(gp.pdgId()) == 11 || Abs(gp.pdgId()) == 13)
                {
                    sgenparticles.push_back(gp);
                    gencls.push_back(&(sgenparticles.back()));
                }
                if(Abs(gp.pdgId()) == 12 || Abs(gp.pdgId()) == 14)
                {
                    sgenparticles.push_back(gp);
                    gennls.push_back(&(sgenparticles.back()));
                    lepdecays++;
                }
                if(Abs(gp.pdgId()) == 16)
                {
                    lepdecays++;
                }
            }

        //      if(gp->status() == 1 && gp->momIdx().size() != 0 && (Abs(gps[gp->momIdx()[0]].pdgId()) == 24 || gp->pdgId() == gps[gp->momIdx()[0]].pdgId()))
        //      {
        //              if(Abs(gp->pdgId()) == 11 || Abs(gp->pdgId()) == 13)
        //              {
        //                      sgenparticles.push_back(*gp);
        //                      genfincls.push_back(&(sgenparticles.back()));
        //              }
        //      }

        }
    }
    //cout << gentq.Pt() << " " << gentqbar.Pt() << endl;
    FULLHAD = false;
    SEMILEP = false;
    FULLLEP = false;
    SEMILEPACC = false;
    //cout << topcounter << " " << lepdecays << " " << genwpartons.size() << " " << genfincls.size() << endl;
    if(topcounter == 2 && genb != 0 && genbbar != 0)
    {
        weight *= 1.+cttptweight*((gentq + gentqbar).Pt()-100.)/500.;
        //weight *= 1.+ Gaus((gentq + gentqbar).M(), 600, 30)*0.2;
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
        SEMILEPACC = true;
        
        if(gencls[0]->pdgId() > 0)
        {
            genallper.Init(genwpartons[0], genwpartons[1], genb, genbbar, gencls[0], gencls[0]->pdgId(), gennls[0]);
            gentqlep = gentqbar;
            gentqhad = gentq;
            gentqhad_3j = gentqhad - *genmiss;
            gentqhad_miss = *genmiss;
            gentqhad_misspartner = *genmisspartner;
        }
        else
        {
            genallper.Init(genwpartons[0], genwpartons[1], genbbar, genb, gencls[0], gencls[0]->pdgId(), gennls[0]);
            gentqlep = gentq;
            gentqhad = gentqbar;
            gentqhad_3j = gentqhad - *genmiss;
            gentqhad_miss = *genmiss;
            gentqhad_misspartner = *genmisspartner;
        }
    }

    if(SEMILEPACC)
    {
        const vector<Genparticle>& gps = event.genParticles();
        for(vector<Genparticle>::const_iterator gp = gps.begin(); gp != gps.end(); ++gp)
        {
            if(gp->status() == 1)
            {
                gen1d["DRW"]->Fill(genallper.WJa()->DeltaR(*gp), gp->E()/genallper.WJa()->E());
                gen1d["DRW"]->Fill(genallper.WJb()->DeltaR(*gp), gp->E()/genallper.WJb()->E());
                gen1d["DRB"]->Fill(genallper.BHad()->DeltaR(*gp), gp->E()/genallper.BHad()->E());
                gen1d["DRB"]->Fill(genallper.BLep()->DeltaR(*gp), gp->E()/genallper.BLep()->E());
            }

        }
    }

//Jet studies
//      if(SEMILEP)
//      {
//              Permutation jetper;
//              jetper.Nu(gennls[0]);
//              if(Abs(gencls[0]->Eta()) < cpletamax && gencls[0]->Pt() > cplptmin)
//              {
//                      jetper.L(gencls[0], gencls[0]->pdgId());
//              }
//              double wa = 0.;
//              double wb = 0.;
//              double bl = 0.;
//              double bh = 0.;
//              const vector<Genjet>& genjets = event.genjets();
//              for(vector<Genjet>::const_iterator Gj = genjets.begin(); Gj != genjets.end(); ++Gj)
//              {
//                      if(Gj->Pt() < jetptmin || Abs(Gj->Eta()) > cjetetamax) {continue;}
//                      sgenjets.push_back(Genjet(*Gj));
//                      Genjet* gj = &(sgenjets.back());
//                      if(gj->DeltaR(*gencls[0]) < 0.4)
//                      {
//                              continue;
//                      }
//                      if(gj->DeltaR(*genallper.WJa()) < 0.4 && gj->Pt() > wa)
//                      {
//                              wa = gj->Pt();
//                              jetper.WJa(gj);
//                      }
//                      if(gj->DeltaR(*genallper.WJb()) < 0.4 && gj->Pt() > wb)
//                      {
//                              wb = gj->Pt();
//                              jetper.WJb(gj);
//                      }
//                      if(gj->DeltaR(*genallper.BLep()) < 0.4 && gj->Pt() > bl)
//                      {
//                              bl = gj->Pt();
//                              jetper.BLep(gj);
//                      }
//                      if(gj->DeltaR(*genallper.BHad()) < 0.4 && gj->Pt() > bh)
//                      {
//                              bh = gj->Pt();
//                              jetper.BHad(gj);
//                      }
//              }
//              if(jetper.IsComplete())
//              {
//                      ttp_genjet.Fill(jetper, weight);
//              }
//      }
}
void ttbar::SelectPseudoTop(URStreamer& event)
{
	const vector<Pstlepton>& psls = event.PSTleptons();
	//if(psls.size() != 1){return;}
	GenObject* lepton = nullptr;
	int lc = 0;
	for(const Pstlepton& pl : psls)
	{
		if(Abs(pl.Eta()) < cpletamax && pl.Pt() > cplptmin){
			lc++;
			if(lc == 2) {return;}
			sgenparticles.push_back(pl);
			lepton = &(sgenparticles.back());
		}
	}
	if(lepton == nullptr) {return;}

	const vector<Pstneutrino>& psns = event.PSTneutrinos();
	//if(psns.size() != 1){return;}
	TLorentzVector nu;
	for(const Pstneutrino& pl : psns)
	{
		nu += pl;
	}

	const vector<Pstjet>& psjs = event.PSTjets();
	vector<TLorentzVector*> pstbjets;
	vector<TLorentzVector*> pstljets;
	for(const Pstjet& pj : psjs)
	{
		if(pj.Pt() > cpbjetptsoft && Abs(pj.Eta()) < cpjetetamax)
		{
			sgenparticles.push_back(pj);
			if(Abs(pj.pdgId()) == 5)
			{
				pstbjets.push_back(&(sgenparticles.back()));	
			}	
			pstljets.push_back(&(sgenparticles.back()));	
		}
	}

	if(pstbjets.size() < 2){return;}
	//sort(pstbjets.begin(), pstbjets.end(), [](TLorentzVector* A, TLorentzVector* B){return(A->Pt() > B->Pt());});
	//sort(pstljets.begin(), pstljets.end(), [](TLorentzVector* A, TLorentzVector* B){return(A->Pt() > B->Pt());});
	//if(pstbjets[0]->Pt() < cpbjetpthard || pstljets[0]->Pt() < cpwjetpthard) {return;}


	TLorentzVector wl(*lepton + nu);
	double chi2min = 1.E100;
	double MW = 80.;
	double Mt = 172.5;
	for(int wa = 0 ; wa < pstljets.size() ; wa++)
	{
		for(int wb = 0 ; wb < wa ; wb++)
		{
			if((pstljets[wa]->Pt() < cpwjetpthard && pstljets[wa]->Pt() < cpwjetpthard)){continue;}
			TLorentzVector wh(*pstljets[wa] + *pstljets[wb]);
			for(int bl = 0 ; bl < pstbjets.size() ; bl++)
			{
				if(pstbjets[bl] == pstljets[wa] || pstbjets[bl] == pstljets[wb]) {continue;}
				for(int bh = 0 ; bh < pstbjets.size() ; bh++)
				{
					if(pstbjets[bh] == pstljets[wa] || pstbjets[bh] == pstljets[wb] || pstbjets[bh] == pstbjets[bl]){continue;}
					if(pstbjets[bh]->Pt() < cpbjetpthard && pstbjets[bl]->Pt() < cpbjetpthard) {continue;}
					TLorentzVector th(wh + *pstbjets[bh]);
					TLorentzVector tl(wl + *pstbjets[bl]);
					double chi2 = Power(th.M() - Mt, 2) + Power(tl.M() - Mt, 2) + Power(wh.M() - MW, 2);
					if(chi2 < chi2min)
					{
						chi2min = chi2;
						psper.Init(pstljets[wa], pstljets[wb], pstbjets[bh], pstbjets[bl], lepton, lepton->pdgId(), &nu);
					}
				}
			}
		}
	}
//cout << psper.WHad().M() << " " << psper.THad().M() << " " << psper.WLep().M() << " " << psper.TLep().M() << endl;
}

void ttbar::AddGenJetSelection(URStreamer& event)
{
	if(!SEMILEP) return;
	const vector<Genjet>& genjets = event.genjets();
	bool wa = false;
	bool wb = false;
	bool ba = false;
	bool bb = false;
	for(vector<Genjet>::const_iterator gj = genjets.begin(); gj != genjets.end(); ++gj)
	{
		if(gj->Pt() < cpwjetptsoft || Abs(gj->Eta()) > cpjetetamax) {continue;}
		if(gj->DeltaR(*genper->L()) < 0.4)
		{
			continue;
		}
		if(gj->DeltaR(*genper->WJa()) < 0.4)
		{
			if(SEMILEPACC && !wa){wa = true; truth1d["foundgen"]->Fill(0.5, weight);}
			continue;
		}
		if(gj->DeltaR(*genper->WJb()) < 0.4)
		{
			if(SEMILEPACC && !wb){wb = true; truth1d["foundgen"]->Fill(1.5, weight);}
			continue;
		}
		if(gj->DeltaR(*genper->BLep()) < 0.4)
		{
			if(SEMILEPACC && !ba){ba = true; truth1d["foundgen"]->Fill(2.5, weight);}
			continue;
		}
		if(gj->DeltaR(*genper->BHad()) < 0.4)
		{
			if(SEMILEPACC && !bb){bb = true; truth1d["foundgen"]->Fill(3.5, weight);}
			continue;
		}

		sgenjets.push_back(Genjet(*gj));
		genaddjets.push_back(&(sgenjets.back()));
	}
	if(wa && wb && ba && bb){truth1d["foundgen"]->Fill(4.5, weight);}
}

void ttbar::SelectRecoParticles(URStreamer& event)
{
	const vector<Muon>& muons = event.muons();
	for(vector<Muon>::const_iterator muon = muons.begin(); muon != muons.end(); ++muon)
	{
		IDMuon mu(*muon);
		//if(mu.ID(IDMuon::TIGHT_12) && mu.Pt() > 15.)
		if(genper->IsComplete() && mu.DeltaR(*genper->L()) < 0.4)
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
		if(genper->IsComplete() && el.DeltaR(*genper->L()) < 0.4)
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
		double jetcorr = 1.;
		if(!isMC)
		{
			if(Abs(jet.Eta()) < 1.5)
			{
				jetcorr = 1. + (-0.1719*Exp(-0.07861*jet.Pt()) - 0.003335);
			}
			else
			{
				jetcorr = 1. + (-0.0176);
			}
		}
		if(isMC)
		{
			if(Abs(jet.Eta()) < 1.5)
			{
				//jetcorr = gRandom->Gaus(jetcorr, 0.4546*Exp(-0.05498*jet.Pt()) + 0.05571);
				jetcorr = gRandom->Gaus(jetcorr, 0.1468*Exp(-0.0165*jet.Pt()) + 0.01973);
			}
			else
			{
				jetcorr = gRandom->Gaus(jetcorr, 3.031*Exp(-0.1092*jet.Pt()) + 0.0344);
			}
		}
		double sf = csigmajet;
		if(sf < 0.){sf *= jetscaler.GetUncM(jet);}
		if(sf > 0.){sf *= jetscaler.GetUncP(jet);}
		if(cjetres > 0.)
		{
			sf = gRandom->Gaus(sf, cjetres);
		}

		sf += 1;
		sf *= jetcorr;
		//if(!isMC) sf *= 1.02651;
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

	////Gen-Reco matching
	//list<IDJet*> pujets(cleanedjets.begin(), cleanedjets.end());
	//cout << "Num J: " << pujets.size() << endl;
	//const vector<Genjet>& genjets = event.genjets();
	//for(vector<Genjet>::const_iterator Gj = genjets.begin(); Gj != genjets.end(); ++Gj)
	//{
	//	int matchcount = 0;
	//	double drmin = 0.4;
	//	list<IDJet*>::iterator best = pujets.end();
	//	for(list<IDJet*>::iterator jit = pujets.begin() ; jit != pujets.end() ; ++jit)
	//	{
	//		IDJet* jet = *jit;
	//		double dr = Gj->DeltaR(*jet);
	//		if(dr < drmin) {drmin = dr; matchcount++; best = jit;}
	//	}
	//	if(best != pujets.end()) 
	//	{
	//		pujets.erase(best);
	//		cout << matchcount << " " << (*best)->Pt()/Gj->Pt() << endl;
	//	}
	//}
	//cout << "Num PJ: " << pujets.size() << endl;

	if(SEMILEP)
	{
			rightper.MET(&met);
			for(IDElectron* el : mediumelectrons)
			{
				if(el->DeltaR(*genper->L()) < 0.2)
				{
					rightper.L(el, el->charge());
					if(SEMILEPACC){truth1d["found"]->Fill(4.5, weight);}
				}
			}

			for(IDMuon* mu : tightmuons)
			{
				if(mu->DeltaR(*genper->L()) < 0.2)
				{
					rightper.L(mu, mu->charge());
					if(SEMILEPACC){truth1d["found"]->Fill(5.5, weight);}
				}
			}

			bool wa = false;
			bool wb = false;
			bool ba = false;
			bool bb = false;
			double ptbhadmax = 0.;
			double ptblepmax = 0.;
			double wjaptmax = 0.;
			double wjbptmax = 0.;
			for(size_t j = 0 ; j < cleanedjets.size() ; ++j)
			{
				IDJet* jet = cleanedjets[j];
				if(jet->DeltaR(*genper->BHad()) < 0.4 && jet->Pt() > ptbhadmax)
				{
					ptbhadmax = jet->Pt();
					rightper.BHad(jet);
					if(SEMILEPACC && !ba){ba = true; truth1d["found"]->Fill(0.5, weight);}
				}
				if(jet->DeltaR(*genper->BLep()) < 0.4 && jet->Pt() > ptblepmax)
				{
					ptblepmax = jet->Pt();
					rightper.BLep(jet);
					if(SEMILEPACC && !bb){bb = true; truth1d["found"]->Fill(1.5, weight);}
				}
				if(jet->DeltaR(*genper->WJa()) < 0.4 && jet->Pt() > wjaptmax)
				{
					wjaptmax = jet->Pt();
					rightper.WJa(jet);
					if(SEMILEPACC && !wa){wa = true; truth1d["found"]->Fill(2.5, weight);}
				}
				if(jet->DeltaR(*genper->WJb()) < 0.4 && jet->Pt() > wjbptmax)
				{
					wjbptmax = jet->Pt();
					rightper.WJb(jet);
					if(SEMILEPACC && !wb){wb = true; truth1d["found"]->Fill(3.5, weight);}
				}
			}
			if(SEMILEPACC && rightper.IsComplete()){truth1d["found"]->Fill(6.5, weight);} 
			truth1d["found"]->Fill(7.5, weight);
	}
}

void ttbar::ttanalysis(URStreamer& event)
{
	reco1d["counter"]->Fill(0.5, weight);

	if(SEMILEPACC)
	{
		if(Abs(genper->LPDGId()) == 11) {truth2d["Ne_Nmu"]->Fill(mediumelectrons.size()+0.5, tightmuons.size()+0.5, weight);}
		if(Abs(genper->LPDGId()) == 13) {truth2d["Nmu_Ne"]->Fill(tightmuons.size()+0.5, mediumelectrons.size()+0.5, weight);}
	}

	//keeping only the n leading jets. 
	sort(cleanedjets.begin(), cleanedjets.end(), [](IDJet* A, IDJet* B){return(A->Pt() > B->Pt());});
	int reducedsize = Min(cleanedjets.size(), cnusedjets);
	reducedjets.resize(reducedsize);
	copy(cleanedjets.begin(), cleanedjets.begin()+reducedsize, reducedjets.begin());

	//check for lepton:
	TLorentzVector* lep = 0;
	int leppdgid = 0;
	if(tightmuons.size() == 1 && loosemuons.size() == 1 && looseelectrons.size() == 0)
	{
		reco1d["c_mu"]->Fill(event.run+0.5);
		lep = dynamic_cast<TLorentzVector*>(tightmuons[0]);
		leppdgid = tightmuons[0]->charge()*-13;
	}
	if(loosemuons.size() == 0 && mediumelectrons.size() == 1 && looseelectrons.size() == 1)
	{
		reco1d["c_el"]->Fill(event.run+0.5);
		lep = dynamic_cast<TLorentzVector*>(mediumelectrons[0]);
		leppdgid = mediumelectrons[0]->charge()*-11;
	}
	if(SEMILEPACC && rightper.IsComplete()){ttp_truth.Fill(rightper, weight);}
	if(lep == 0){return;}
	reco1d["c_lep"]->Fill(event.run+0.5);

	double nvtx = event.vertexs().size();
	reco1d["NumVertices"]->Fill(nvtx , mcweight);

	if(isMC)
	{
		if(tightmuons.size() == 1)
		{
			weight *= musfhist->GetBinContent(musfhist->GetXaxis()->FindFixBin(lep->Eta()), musfhist->GetYaxis()->FindFixBin(Min(lep->Pt(), 120.)));
			weight *= mutrgsfhist->GetBinContent(mutrgsfhist->GetXaxis()->FindFixBin(lep->Eta()), mutrgsfhist->GetYaxis()->FindFixBin(Min(lep->Pt(), 120.)));
		}
		if(mediumelectrons.size() == 1)
		{
			weight *= elsfhist->GetBinContent(elsfhist->GetXaxis()->FindFixBin(lep->Eta()), elsfhist->GetYaxis()->FindFixBin(Min(lep->Pt(), 120.)));
			weight *= eltrgsfhist->GetBinContent(eltrgsfhist->GetXaxis()->FindFixBin(lep->Eta()), eltrgsfhist->GetYaxis()->FindFixBin(Min(lep->Pt(), 120.)));
		}

	}
	reco1d["NumVerticesWeighted"]->Fill(nvtx , weight);

	reco1d["counter"]->Fill(1.5, weight);
	if(SEMILEPACC)
	{
		truth1d["counter"]->Fill(3.5, weight);
	}
	if(rightper.NumTTBarJets() == 4)
	{
		truth2d["dPtJet_right"]->Fill(genper->WJa()->Pt(), (rightper.WJa()->Pt() - genper->WJa()->Pt())/genper->WJa()->Pt(), weight);
		truth2d["dPtJet_right"]->Fill(genper->WJb()->Pt(), (rightper.WJb()->Pt() - genper->WJb()->Pt())/genper->WJb()->Pt(), weight);
		truth2d["dPtbJet_right"]->Fill(genper->BHad()->Pt(), (rightper.BHad()->Pt() - genper->BHad()->Pt())/genper->BHad()->Pt(), weight);
		truth2d["dPtbJet_right"]->Fill(genper->BLep()->Pt(), (rightper.BLep()->Pt() - genper->BLep()->Pt())/genper->BLep()->Pt(), weight);
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
		truth2d["btag2d_true"]->Fill(Min(dynamic_cast<IDJet*>(rightper.BHad())->csvIncl(), dynamic_cast<IDJet*>(rightper.BLep())->csvIncl()), Max(dynamic_cast<IDJet*>(rightper.BHad())->csvIncl(), dynamic_cast<IDJet*>(rightper.BLep())->csvIncl()), weight);
		truth1d["btag_true"]->Fill(dynamic_cast<IDJet*>(rightper.BHad())->csvIncl(), weight);
		truth1d["btag_true"]->Fill(dynamic_cast<IDJet*>(rightper.BLep())->csvIncl(), weight);
		truth1d["btag_wrong"]->Fill(dynamic_cast<IDJet*>(rightper.WJa())->csvIncl(), weight);
		truth1d["btag_wrong"]->Fill(dynamic_cast<IDJet*>(rightper.WJb())->csvIncl(), weight);
	}

	//cut on number of jets
	reco1d["jetmulti"]->Fill(cleanedjets.size(), weight);
//cout << "NC: " << cleanedjets.size() << endl;
	if(cleanedjets.size() < 4){return;} // change for 3j test
	reco1d["c_jets"]->Fill(event.run+0.5);
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
	if(isMC && !BTAGMODE)
	{
		double btw = btagweight.SF(reducedjets);
//cout << weight << " " << btw << endl;
		weight *= btw;
	}
	reco1d["bjetmultiW"]->Fill(nbjets, weight);

	//check for b-jets
	reco1d["btag_high"]->Fill(reducedjets[0]->csvIncl(), weight);
	reco1d["btag_low"]->Fill(reducedjets[1]->csvIncl(), weight);
	if(!BTAGMODE)
	{
		if(reducedjets[0]->csvIncl() < B_MEDIUM || reducedjets[1]->csvIncl() < B_LOOSE){return;}
		//if(reducedjets[0]->csvIncl() < B_MEDIUM || reducedjets[1]->csvIncl() < B_MEDIUM){return;} // add for 3j test
		//if(reducedjets[0]->csvIncl() < B_MEDIUM || reducedjets[1]->csvIncl() < B_MEDIUM){return;}
	}
	reco1d["c_btag"]->Fill(event.run+0.5);
	reco1d["counter"]->Fill(3.5, weight);
	if(SEMILEPACC) truth1d["counter"]->Fill(5.5, weight);

// add for 3j test
        //NeutrinoSolver NS_3j;
        const TLorentzVector * bcandidate1 = (TLorentzVector*)reducedjets[0];
        const TLorentzVector * bcandidate2 = (TLorentzVector*)reducedjets[1];
        const TLorentzVector * lcandidate = (TLorentzVector*)lep;
        double chi2candidate1;
        double chi2candidate2;
        const TLorentzVector * blepjet_3j;
        const TLorentzVector * bhadjet_3j;
        NeutrinoSolver NS_3ja = NeutrinoSolver(lcandidate, bcandidate1, 80., 173.);
        NS_3ja.GetBest(met.X(), met.Y(), 1, 1, 0, chi2candidate1);
        NeutrinoSolver NS_3jb = NeutrinoSolver(lcandidate, bcandidate2, 80., 173.);
        NS_3jb.GetBest(met.X(), met.Y(), 1, 1, 0, chi2candidate2);

        reco3j2d["selectchi2"]->Fill(chi2candidate1, chi2candidate2, weight);
        //threej1d["selectchi2_b1"]->Fill(chi2candidate1, weight);
        //threej1d["selectchi2_b2"]->Fill(reducedjets[1]->csvIncl(), weight);
        reco3j2d["selectcsv"]->Fill(reducedjets[0]->csvIncl(), reducedjets[1]->csvIncl(), weight);
        //b jet permutation
        if(chi2candidate1 <= chi2candidate2){
            blepjet_3j = bcandidate1;
            bhadjet_3j = bcandidate2;
            reco3j1d["blep_chi2"]->Fill(chi2candidate1, weight);
            reco3j1d["blepwrong_chi2"]->Fill(chi2candidate2, weight);
            reco3j2d["blep_bhad_csv"]->Fill(reducedjets[0]->csvIncl(), reducedjets[1]->csvIncl(), weight);
        }
        else{
            blepjet_3j = bcandidate2;
            bhadjet_3j = bcandidate1;
            reco3j1d["blep_chi2"]->Fill(chi2candidate2, weight);
            reco3j1d["blepwrong_chi2"]->Fill(chi2candidate1, weight);
            reco3j2d["blep_bhad_csv"]->Fill(reducedjets[1]->csvIncl(), reducedjets[0]->csvIncl(), weight);
        }

        TLorentzVector tlep_3j = *blepjet_3j + *lcandidate + met;
        TLorentzVector thad_3j = *bhadjet_3j + *reducedjets[2];
        TLorentzVector thad_miss = *reducedjets[3];

        reco3j1d["tlep_pt"]->Fill(tlep_3j.Pt(), weight);
        reco3j1d["thad_pt"]->Fill(thad_3j.Pt(), weight);
        reco3j1d["tlep_y"]->Fill(Abs(tlep_3j.Rapidity()), weight);
        reco3j1d["thad_y"]->Fill(Abs(thad_3j.Rapidity()), weight);
        reco3j1d["tlep_M"]->Fill(tlep_3j.Mag(), weight);
        reco3j1d["thad_M"]->Fill(thad_3j.Mag(), weight);
        reco3j1d["tt_pt"]->Fill((tlep_3j + thad_3j).Pt(), weight);
        reco3j1d["tt_y"]->Fill(Abs((tlep_3j + thad_3j).Rapidity()), weight);
        reco3j1d["Mtt"]->Fill((tlep_3j + thad_3j).Mag(), weight);
        reco3j1d["delY"]->Fill(tlep_3j.Rapidity()-thad_3j.Rapidity(), weight);
        reco3j2d["Mtt_delY"]->Fill((tlep_3j + thad_3j).Mag(), tlep_3j.Rapidity()-thad_3j.Rapidity(), weight);

        reco3j2d["blep_bhad_pt"]->Fill(blepjet_3j->Pt(), bhadjet_3j->Pt(), weight);
        reco3j1d["thadmiss_e"]->Fill(thad_miss.E(), weight);
        reco3j1d["thadmiss_pt"]->Fill(thad_miss.Pt(), weight);
        reco3j1d["thadmiss_y"]->Fill(thad_miss.Rapidity(), weight);
        reco3j1d["thadmiss_DeltaR"]->Fill(thad_miss.DeltaR(*reducedjets[2]), weight);
        reco3j1d["delpt_pt"]->Fill(((thad_3j + *reducedjets[3]).Pt() - thad_3j.Pt())/thad_3j.Pt(), weight);
        reco3j1d["dely_y"]->Fill(((thad_3j + *reducedjets[3]).Rapidity() - thad_3j.Rapidity())/thad_3j.Rapidity(), weight);

  /*      
        if(reducedjets.size() == 3){
            threejets["Mtt_exact3j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *lep + met).Mag(), weight);
        }
        if(reducedjets.size() == 4){
            threejets["Mtt_above3j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *lep + met).Mag(), weight);
            //fourjets["Mtt_above3j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *reducedjets[3] + *lep + met).Mag(), weight);
            threejets["Mtt_exact4j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *lep + met).Mag(), weight); 
            fourjets["Mtt_exact4j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *reducedjets[3] + *lep + met).Mag(), weight); 
        }
        if(reducedjets.size() == 5){
            threejets["Mtt_above3j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *lep + met).Mag(), weight);
            //fourjets["Mtt_above3j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *reducedjets[3] + *lep + met).Mag(), weight);
            threejets["Mtt_above4j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *lep + met).Mag(), weight);
            fourjets["Mtt_above4j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *reducedjets[3] + *lep + met).Mag(), weight);
            threejets["Mtt_exact5j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *lep + met).Mag(), weight);
            fourjets["Mtt_exact5j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *reducedjets[3] + *lep + met).Mag(), weight); 
        }
        if(reducedjets.size() > 5){
            threejets["Mtt_above3j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *lep + met).Mag(), weight);
            //fourjets["Mtt_above3j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *reducedjets[3] + *lep + met).Mag(), weight);
            threejets["Mtt_above4j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *lep + met).Mag(), weight);
            fourjets["Mtt_above4j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *reducedjets[3] + *lep + met).Mag(), weight);
            threejets["Mtt_above5j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *lep + met).Mag(), weight);
            fourjets["Mtt_above5j"]->Fill((*reducedjets[0] + *reducedjets[1] + *reducedjets[2] + *reducedjets[3] + *lep + met).Mag(), weight);
        }
*/
// end for the 3j test

        //check what we have reconstructed
	if(SEMILEP)
	{
		truth2d["tt_jets"]->Fill(rightper.NumBJets()+0.5, rightper.NumWJets()+0.5, weight);
	}
	//cout << "PS " << cleanedjets.size() << " " << psper.IsComplete() << endl; 

	//cout << "SYNC " << event.run << " " << event.lumi << " " << event.evt << endl;

	//reconstruction

	if(SEMILEPACC && rightper.IsComplete()) truth1d["counter"]->Fill(6.5, weight);

	if(rightper.IsComplete())
	{
		rightper.Solve(ttsolver);
		truth1d["nschi_right"]->Fill(ttsolver.NSChi2()/Sqrt(Abs(ttsolver.NSChi2())), weight);
		truth2d["Wmasshad_tmasshad_right"]->Fill(rightper.THad().M(), rightper.WHad().M(), weight);
		truth2d["Wmtlep_tmtlep_right"]->Fill(rightper.MttLep(), rightper.MtWLep(), weight);
		//double maxwjpt = max({rightper.WJa()->Pt(), rightper.WJb()->Pt()});
		//double minbjpt = min({rightper.BHad()->Pt(), rightper.BLep()->Pt()});
		//cout << (rightper.BHad()->Pt() > maxwjpt || rightper.BLep()->Pt() > maxwjpt) << endl;
		//cout << (rightper.WJa()->Pt() < minbjpt || rightper.WJb()->Pt() < minbjpt) << endl;
	}


////////////////////////////////////////////////////////////
//	
//	
//		const vector<Pstlepton>& psls = event.PSTleptons();
//		if(psls.size() != 1){cout << "no lep";}
//	
//		const vector<Pstjet>& psjs = event.PSTjets();
//		vector<TLorentzVector*> pstbjets;
//		vector<TLorentzVector*> pstljets;
//		for(const Pstjet& pj : psjs)
//		{
//			if(Abs(pj.pdgId()) == 5)
//			{
//				if(pj.Pt() > cpbjetptsoft && Abs(pj.Eta()) < cpjetetamax)
//				{
//					sgenparticles.push_back(pj);
//					pstbjets.push_back(&(sgenparticles.back()));	
//				}
//			}	
//			else
//			{
//				if(pj.Pt() > cpwjetptsoft && Abs(pj.Eta()) < cpjetetamax)
//				{
//					sgenparticles.push_back(pj);
//					pstljets.push_back(&(sgenparticles.back()));	
//				}
//			}
//		}
//	
//		if(pstbjets.size() < 2 || pstljets.size() < 2){
//	cout << "no jets " << pstbjets.size() << " " << pstljets.size() << " " << SEMILEP << endl;
//	
//		//Gen-Reco matching
//		list<IDJet*> pujets(cleanedjets.begin(), cleanedjets.end());
//		cout << "Num J: " << pujets.size() << endl;
//		const vector<Genjet>& genjets = event.genjets();
//		for(vector<Genjet>::const_iterator Gj = genjets.begin(); Gj != genjets.end(); ++Gj)
//		{
//			if(Gj->Pt() < 25. || Abs(Gj->Eta()) > 2.5) continue;
//			int matchcount = 0;
//			double drmin = 0.4;
//			list<IDJet*>::iterator best = pujets.end();
//			for(list<IDJet*>::iterator jit = pujets.begin() ; jit != pujets.end() ; ++jit)
//			{
//				IDJet* jet = *jit;
//				double dr = Gj->DeltaR(*jet);
//				if(dr < drmin) {drmin = dr; matchcount++; best = jit;}
//			}
//			if(best != pujets.end()) 
//			{
//				pujets.erase(best);
//				cout << matchcount << " " << (*best)->Pt()/Gj->Pt() << endl;
//			}
//		}
//		cout << "Num PJ: " << pujets.size() << endl;
//	
//	
//	}
//		//sort(pstbjets.begin(), pstbjets.end(), [](TLorentzVector* A, TLorentzVector* B){return(A->Pt() > B->Pt());});
//		//sort(pstljets.begin(), pstljets.end(), [](TLorentzVector* A, TLorentzVector* B){return(A->Pt() > B->Pt());});
//		//if(pstbjets[0]->Pt() < cpbjetpthard || pstljets[0]->Pt() < cpwjetpthard) {cout << "no hard jets" << endl;}
//	
//	
//////////////////////////////////////////////////////////////

	if(false && PSEUDOTOP)
	{
		TLorentzVector* bl = reducedjets[0];
		TLorentzVector* bh = reducedjets[1];

		if(lep->DeltaR(*reducedjets[1]) < lep->DeltaR(*reducedjets[0]))
		{
			bl = reducedjets[1];
			bh = reducedjets[0];
		}

		vector<TLorentzVector*> wvec(2, 0);

		partial_sort_copy(reducedjets.begin()+2, reducedjets.end(), wvec.begin(), wvec.end(), [](TLorentzVector* A, TLorentzVector* B){return(A->Pt() > B->Pt());});
		//cout << wvec[0]->Pt() << " " << wvec[1]->Pt() << endl;
		bestper.Init(wvec[0], wvec[1], bh, bl, lep, leppdgid, &met);
		bestper.Solve(ttsolver);

	}
	else
	{
		int nbtaglocal = 2;
		if(BTAGMODE) nbtaglocal = 0;
		bestper.Reset();
		int percount = 0;
		for(size_t i = nbtaglocal ; i < reducedjets.size() ; ++i)
		{
			for(size_t j = nbtaglocal ; j < i ; ++j)
			{
				for(size_t k = 0 ; k < (nbtaglocal == 2 ? 2 : reducedjets.size()) ; ++k)
				{
					//if(!BTAGMODE && reducedjets[k]->csvIncl() < B_MEDIUM){continue;}	
					if(i == k || j == k) continue;
					for(size_t l = 0 ; l < (nbtaglocal == 2 ? 2 : reducedjets.size()) ; ++l)
					{
						//if(!BTAGMODE && reducedjets[l]->csvIncl() < B_MEDIUM){continue;}	
						if(l == i || l == j || l == k) continue;
						if(nbtaglocal == 1 && k != 0 && l != 0) continue;
						testper.Init(reducedjets[i], reducedjets[j], reducedjets[k], reducedjets[l], lep, leppdgid, &met);
						if(testper.WJa()->Pt() < cwjetpthard && testper.WJb()->Pt() < cwjetpthard) continue;
						if(testper.WJa()->Pt() < cwjetptsoft || testper.WJb()->Pt() < cwjetptsoft) continue;
						if(testper.BHad()->Pt() < cbjetpthard && testper.BLep()->Pt() < cbjetpthard) continue;
						if(testper.BHad()->Pt() < cbjetptsoft || testper.BLep()->Pt() < cbjetptsoft) continue;
						testper.Solve(ttsolver);

						//if(rightper.IsComplete()) cout << (percount++) << " " << rightper.IsCorrect(testper) << " "  << testper.WHad().M() << " " << testper.THad().M() << " " <<  testper.Prob() << " " << testper.MassDiscr() << " " << testper.NuChisq() << endl;

						reco2d["Wmasshad_tmasshad"]->Fill(testper.WHad().M(), testper.THad().M());
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
								truth2d["Wmtlep_tmtlep_wrong"]->Fill(testper.MttLep(), testper.MtWLep(), weight);
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
								truth1d["masstest_right"]->Fill(ttsolver.MassRes(), weight);
								truth1d["comtest_right"]->Fill(ttsolver.Res(), weight);
							}
							else
							{
								if(rightper.IsWHadCorrect(testper))
								{
									truth2d["Wmasshad_tmasshad_rightw"]->Fill(testper.WHad().M(), testper.THad().M(), weight);
								}
								else
								{
									truth2d["Wmasshad_tmasshad_wrongw"]->Fill(testper.WHad().M(), testper.THad().M(), weight);
								}
								truth2d["Wmasshad_tmasshad_wrong"]->Fill(testper.THad().M(), testper.WHad().M(), weight);
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
	//if(!BTAGMODE && bestper.MassDiscr() > clikelihoodcut){return;}
	if(!BTAGMODE && bestper.Prob() > clikelihoodcut){return;}
	reco1d["c_algo"]->Fill(event.run+0.5);

	reco1d["counter"]->Fill(4.5, weight);
	if(SEMILEPACC && rightper.IsComplete()) truth1d["counter"]->Fill(7.5, weight);
	//Fill reconstructed hists
	ttp_all.Fill(bestper, weight);
	response.FillAll("thardpt", bestper.THard().Pt(), weight);
	response.FillAll("tsoftpt", bestper.TSoft().Pt(), weight);
	response.FillAll("nobin", bestper.THad().Pt(), weight);
	response.FillAll("thadpt", bestper.THad().Pt(), weight);
	response.FillAll("thady", Abs(bestper.THad().Rapidity()), weight);
	response.FillAll("tleppt", bestper.TLep().Pt(), weight);
	response.FillAll("tlepy", Abs(bestper.TLep().Rapidity()), weight);
	response.FillAll("ttm", (bestper.THad() + bestper.TLep()).M(), weight);
	response.FillAll("ttpt", (bestper.THad() + bestper.TLep()).Pt(), weight);
	response.FillAll("tty", Abs((bestper.THad() + bestper.TLep()).Rapidity()), weight);
	response.FillAll("njet", cleanedjets.size() - 4, weight);
	response.FillAll("dymp", Abs(bestper.T().Rapidity()) - Abs(bestper.Tb().Rapidity()), weight);
	response2d.FillAll("njets_thadpt", cleanedjets.size() - 4, bestper.THad().Pt(), weight);
	response2d.FillAll("njets_ttpt", cleanedjets.size() - 4, bestper.TT().Pt(), weight);

        //for yukawa studies
		TLorentzVector CMttbar = bestper.THad() + bestper.TLep();
		TLorentzVector CMlept = bestper.TLep();
		TLorentzVector CMhadt = bestper.THad();
		CMttbar.Boost(-CMttbar.BoostVector());
		CMlept.Boost(-CMttbar.BoostVector());
		CMhadt.Boost(-CMttbar.BoostVector());

		double Mtt = (bestper.THad() + bestper.TLep()).M();
		double Mtt_boost = CMttbar.M();
		double costheta_had = bestper.THad().Pz()/bestper.THad().Mag();
		double costheta_lep = bestper.TLep().Pz()/bestper.TLep().Mag();
		double costheta_had_boost = CMhadt.Pz()/CMhadt.Mag();
		double costheta_lep_boost = CMlept.Pz()/CMlept.Mag();
		double deltaY = bestper.TLep().Rapidity() - bestper.THad().Rapidity();
		double deltaY_boost = CMlept.Rapidity() - CMhadt.Rapidity();
		double deltaBeta = bestper.TLep().P()/bestper.TLep().E() - bestper.THad().P()/bestper.THad().E();
		double deltaBeta_boost = CMlept.P()/CMlept.E() + CMhadt.P()/CMhadt.E();
                double tlepy = Abs(bestper.TLep().Rapidity());

		yuka1d_reco["Mtt"]->Fill(Mtt, weight);
		yuka1d_reco["costheta"]->Fill(costheta_had, weight);
		yuka1d_reco["costheta"]->Fill(costheta_lep, weight);
		yuka1d_reco["delY"]->Fill(deltaY, weight);
		yuka1d_reco["Y"]->Fill(bestper.TLep().Rapidity(), weight);
		yuka1d_reco["Y"]->Fill(bestper.THad().Rapidity(), weight);
		yuka1d_reco["delBeta"]->Fill(deltaBeta, weight);
                yuka2d_reco["Mtt_costheta"]->Fill(Mtt, costheta_had, weight);
                yuka2d_reco["Mtt_costheta"]->Fill(Mtt, costheta_lep, weight);
                yuka2d_reco["delY_costheta"]->Fill(deltaY, costheta_lep, weight);
                yuka2d_reco["delY_costheta"]->Fill(deltaY, costheta_had, weight);
		yuka2d_reco["Mtt_delY"]->Fill(Mtt, deltaY, weight);
		yuka2d_reco["Mtt_delBeta"]->Fill(Mtt, deltaBeta, weight);
		yuka2d_reco["delY_delBeta"]->Fill(deltaY, deltaBeta, weight);
	
		yuka1d_reco["Mtt_boo"]->Fill(Mtt_boost, weight);
		yuka1d_reco["costheta"]->Fill(costheta_had_boost, weight);
		yuka1d_reco["costheta"]->Fill(costheta_lep_boost, weight);
		yuka1d_reco["delY_boo"]->Fill(deltaY_boost, weight);
		yuka1d_reco["delBeta_boo"]->Fill(deltaBeta_boost, weight);
                yuka2d_reco["Mtt_costheta_boo"]->Fill(Mtt_boost, costheta_had_boost, weight);
                yuka2d_reco["Mtt_costheta_boo"]->Fill(Mtt_boost, costheta_lep_boost, weight);
		yuka2d_reco["Mtt_delY_boo"]->Fill(Mtt_boost, deltaY_boost, weight);
		yuka2d_reco["Mtt_delBeta_boo"]->Fill(Mtt_boost, deltaBeta_boost, weight);
		yuka2d_reco["delY_delBeta_boo"]->Fill(deltaY_boost, deltaBeta_boost, weight);

                yuka1d_reco["tlepy"]->Fill(tlepy, weight);
                yuka2d_reco["Mtt_tlepy"]->Fill(Mtt, tlepy, weight);
                yuka2d_reco["delY_tlepy"]->Fill(deltaY, tlepy, weight);


		//end of yukawa studies



	if(SEMILEP)
	{
		if(PDFTEST)
		{
			pdfunc->Fill1d("pdfunc_reco_thardpt", genper->THard().Pt(), weight);
			pdfunc->Fill1d("pdfunc_reco_tsoftpt", genper->TSoft().Pt() , weight);
			pdfunc->Fill1d("pdfunc_reco_nobin", genper->THad().Pt(), weight);
			pdfunc->Fill1d("pdfunc_reco_thadpt", genper->THad().Pt(), weight);
			pdfunc->Fill1d("pdfunc_reco_tleppt", genper->TLep().Pt(), weight);
			pdfunc->Fill1d("pdfunc_reco_thady", Abs(genper->THad().Rapidity()), weight);
			pdfunc->Fill1d("pdfunc_reco_tlepy", Abs(genper->TLep().Rapidity()), weight);
			pdfunc->Fill1d("pdfunc_reco_ttm", genper->TT().M(), weight);
			pdfunc->Fill1d("pdfunc_reco_tty", Abs(genper->TT().Rapidity()), weight);
			pdfunc->Fill1d("pdfunc_reco_ttpt", genper->TT().Pt(), weight);
			pdfunc->Fill1d("pdfunc_reco_njet", genaddjets.size(), weight);
			pdfunc->Fill1d("pdfunc_reco_dymp", Abs(genper->T().Rapidity()) - Abs(genper->Tb().Rapidity()), weight);
			pdfunc->Fill1d("pdfunc_reco_njets_thadpt", response2d.GetBin("njets_thadpt", cleanedjets.size() - 4, bestper.THad().Pt())-0.5, weight);
			pdfunc->Fill1d("pdfunc_reco_njets_ttpt", response2d.GetBin("njets_ttpt", cleanedjets.size() - 4, bestper.TT().Pt())-0.5, weight);
		}
		response.FillTruthReco("thardpt", genper->THard().Pt(), bestper.THard().Pt(), weight);
		response.FillTruthReco("tsoftpt", genper->TSoft().Pt(), bestper.TSoft().Pt(), weight);
		response.FillTruthReco("nobin", genper->THad().Pt(), bestper.THad().Pt(), weight);
		response.FillTruthReco("thadpt", genper->THad().Pt(), bestper.THad().Pt(), weight);
		response.FillTruthReco("thady", Abs(genper->THad().Rapidity()), Abs(bestper.THad().Rapidity()), weight);
		response.FillTruthReco("tleppt", genper->TLep().Pt(), bestper.TLep().Pt(), weight);
		response.FillTruthReco("tlepy", Abs(genper->TLep().Rapidity()), Abs(bestper.TLep().Rapidity()), weight);
		response.FillTruthReco("ttm", genper->TT().M(), bestper.TT().M(), weight);
		response.FillTruthReco("ttpt", genper->TT().Pt(), bestper.TT().Pt(), weight);
		response.FillTruthReco("tty", Abs(genper->TT().Rapidity()), Abs(bestper.TT().Rapidity()), weight);
		response.FillTruthReco("njet", genaddjets.size(), cleanedjets.size() - 4, weight);
		response.FillTruthReco("dymp", Abs(genper->T().Rapidity()) - Abs(genper->Tb().Rapidity()), Abs(bestper.T().Rapidity()) - Abs(bestper.Tb().Rapidity()) , weight);
		response2d.FillTruthReco("njets_thadpt", genaddjets.size(), genper->THad().Pt(), cleanedjets.size() - 4, bestper.THad().Pt(), weight);
		response2d.FillTruthReco("njets_ttpt", genaddjets.size(), genper->TT().Pt(), cleanedjets.size() - 4, bestper.TT().Pt(), weight);
	}
	else
	{
		response.FillBKG("thardpt", bestper.THard().Pt(), weight);
		response.FillBKG("tsoftpt", bestper.TSoft().Pt(), weight);
		response.FillBKG("nobin", bestper.THad().Pt(), weight);
		response.FillBKG("thadpt", bestper.THad().Pt(), weight);
		response.FillBKG("thady", Abs(bestper.THad().Rapidity()), weight);
		response.FillBKG("tleppt", bestper.TLep().Pt(), weight);
		response.FillBKG("tlepy", Abs(bestper.TLep().Rapidity()), weight);
		response.FillBKG("ttm", (bestper.THad() + bestper.TLep()).M(), weight);
		response.FillBKG("ttpt", (bestper.THad() + bestper.TLep()).Pt(), weight);
		response.FillBKG("tty", Abs((bestper.THad() + bestper.TLep()).Rapidity()), weight);
		response.FillBKG("njet", cleanedjets.size() - 4, weight);
		response.FillBKG("dymp", Abs(bestper.T().Rapidity()) - Abs(bestper.Tb().Rapidity()), weight);
		response2d.FillBKG("njets_thadpt", cleanedjets.size() - 4, bestper.THad().Pt(), weight);
		response2d.FillBKG("njets_ttpt", cleanedjets.size() - 4, bestper.TT().Pt(), weight);
	}

	//Fill reconstructed hists with matching information
	if(rightper.IsCorrect(bestper))
	{
		ttp_right.Fill(bestper, weight);
		truth1d["counter"]->Fill(8.5, weight);
                //for yukawa studies
		yuka1d_reco_right["Mtt"]->Fill(Mtt, weight);
		yuka1d_reco_right["costheta"]->Fill(costheta_had, weight);
		yuka1d_reco_right["costheta"]->Fill(costheta_lep, weight);
		yuka1d_reco_right["delY"]->Fill(deltaY, weight);
		yuka1d_reco_right["Y"]->Fill(bestper.TLep().Rapidity(), weight);
		yuka1d_reco_right["Y"]->Fill(bestper.THad().Rapidity(), weight);
		yuka1d_reco_right["delBeta"]->Fill(deltaBeta, weight);
                yuka2d_reco_right["Mtt_costheta"]->Fill(Mtt, costheta_had, weight);
                yuka2d_reco_right["Mtt_costheta"]->Fill(Mtt, costheta_lep, weight);
                yuka2d_reco_right["delY_costheta"]->Fill(deltaY, costheta_lep, weight);
                yuka2d_reco_right["delY_costheta"]->Fill(deltaY, costheta_had, weight);
		yuka2d_reco_right["Mtt_delY"]->Fill(Mtt, deltaY, weight);
		yuka2d_reco_right["Mtt_delBeta"]->Fill(Mtt, deltaBeta, weight);
		yuka2d_reco_right["delY_delBeta"]->Fill(deltaY, deltaBeta, weight);
	
		yuka1d_reco_right["Mtt_boo"]->Fill(Mtt_boost, weight);
		yuka1d_reco_right["costheta"]->Fill(costheta_had_boost, weight);
		yuka1d_reco_right["costheta"]->Fill(costheta_lep_boost, weight);
		yuka1d_reco_right["delY_boo"]->Fill(deltaY_boost, weight);
		yuka1d_reco_right["delBeta_boo"]->Fill(deltaBeta_boost, weight);
                yuka2d_reco_right["Mtt_costheta_boo"]->Fill(Mtt_boost, costheta_had_boost, weight);
                yuka2d_reco_right["Mtt_costheta_boo"]->Fill(Mtt_boost, costheta_lep_boost, weight);
		yuka2d_reco_right["Mtt_delY_boo"]->Fill(Mtt_boost, deltaY_boost, weight);
		yuka2d_reco_right["Mtt_delBeta_boo"]->Fill(Mtt_boost, deltaBeta_boost, weight);
		yuka2d_reco_right["delY_delBeta_boo"]->Fill(deltaY_boost, deltaBeta_boost, weight);

                yuka1d_reco_right["tlepy"]->Fill(tlepy, weight);
                yuka2d_reco_right["Mtt_tlepy"]->Fill(Mtt, tlepy, weight);
                yuka2d_reco_right["delY_tlepy"]->Fill(deltaY, tlepy, weight);

		//end of yukawa studies

		truth1d["dRNu_right"]->Fill(bestper.Nu().DeltaR(genper->Nu()), weight);
		truth1d["dPtNu_right"]->Fill((bestper.Nu().Pt() - genper->Nu().Pt())/genper->Nu().Pt(), weight);
		truth1d["dPzNu_right"]->Fill((bestper.Nu().Pz() - genper->Nu().Pz())/genper->Nu().Pz(), weight);
		truth2d["dPzNu_dPhi_right"]->Fill((bestper.Nu().Pz() - genper->Nu().Pz())/genper->Nu().Pz(), Abs(bestper.Nu().DeltaPhi(met)), weight);
		truth2d["dPzNu_chi2_right"]->Fill((bestper.Nu().Pz() - genper->Nu().Pz())/genper->Nu().Pz(), Sqrt(bestper.NuChisq()), weight);
		truth2d["RES_Mtt_right"]->Fill(((bestper.THad() + bestper.TLep()).M() - (genper->THad() + genper->TLep()).M())/(genper->THad() + genper->TLep()).M(), (genper->THad() + genper->TLep()).M(), weight);
		truth2d["RES_dbeta_right"]->Fill(((bestper.THad().BoostVector() - bestper.TLep().BoostVector()).Mag() - (genper->THad().BoostVector() - genper->TLep().BoostVector()).Mag())/(genper->THad().BoostVector() - genper->TLep().BoostVector()).Mag(), (genper->THad().BoostVector() - genper->TLep().BoostVector()).Mag(), weight);
		truth1d["dRNuMet_right"]->Fill(met.DeltaR(genper->Nu()), weight);
		truth1d["dPtNuMet_right"]->Fill((met.Pt() - genper->Nu().Pt())/genper->Nu().Pt(), weight);
		truth2d["RES_Mtt_all"]->Fill((bestper.TT().M() - genper->TT().M())/genper->TT().M(), genper->TT().M(), weight);
		truth2d["RES_dbeta_all"]->Fill(((bestper.THad().BoostVector() - bestper.TLep().BoostVector()).Mag() - (genper->THad().BoostVector() - genper->TLep().BoostVector()).Mag())/(genper->THad().BoostVector() - genper->TLep().BoostVector()).Mag(), (genper->THad().BoostVector() - genper->TLep().BoostVector()).Mag(), weight);
	}
	else if(rightper.IsComplete())
	{
		ttp_wrong.Fill(bestper, weight);
		truth1d["counter"]->Fill(9.5, weight);
                //for yukawa studies
		yuka1d_reco_wrong["Mtt"]->Fill(Mtt, weight);
		yuka1d_reco_wrong["costheta"]->Fill(costheta_had, weight);
		yuka1d_reco_wrong["costheta"]->Fill(costheta_lep, weight);
		yuka1d_reco_wrong["delY"]->Fill(deltaY, weight);
		yuka1d_reco_wrong["Y"]->Fill(bestper.TLep().Rapidity(), weight);
		yuka1d_reco_wrong["Y"]->Fill(bestper.THad().Rapidity(), weight);
		yuka1d_reco_wrong["delBeta"]->Fill(deltaBeta, weight);
                yuka2d_reco_wrong["Mtt_costheta"]->Fill(Mtt, costheta_had, weight);
                yuka2d_reco_wrong["Mtt_costheta"]->Fill(Mtt, costheta_lep, weight);
                yuka2d_reco_wrong["delY_costheta"]->Fill(deltaY, costheta_lep, weight);
                yuka2d_reco_wrong["delY_costheta"]->Fill(deltaY, costheta_had, weight);
		yuka2d_reco_wrong["Mtt_delY"]->Fill(Mtt, deltaY, weight);
		yuka2d_reco_wrong["Mtt_delBeta"]->Fill(Mtt, deltaBeta, weight);
		yuka2d_reco_wrong["delY_delBeta"]->Fill(deltaY, deltaBeta, weight);
	
		yuka1d_reco_wrong["Mtt_boo"]->Fill(Mtt_boost, weight);
		yuka1d_reco_wrong["costheta"]->Fill(costheta_had_boost, weight);
		yuka1d_reco_wrong["costheta"]->Fill(costheta_lep_boost, weight);
		yuka1d_reco_wrong["delY_boo"]->Fill(deltaY_boost, weight);
		yuka1d_reco_wrong["delBeta_boo"]->Fill(deltaBeta_boost, weight);
                yuka2d_reco_wrong["Mtt_costheta_boo"]->Fill(Mtt_boost, costheta_had_boost, weight);
                yuka2d_reco_wrong["Mtt_costheta_boo"]->Fill(Mtt_boost, costheta_lep_boost, weight);
		yuka2d_reco_wrong["Mtt_delY_boo"]->Fill(Mtt_boost, deltaY_boost, weight);
		yuka2d_reco_wrong["Mtt_delBeta_boo"]->Fill(Mtt_boost, deltaBeta_boost, weight);
		yuka2d_reco_wrong["delY_delBeta_boo"]->Fill(deltaY_boost, deltaBeta_boost, weight);

                yuka1d_reco_wrong["tlepy"]->Fill(tlepy, weight);
                yuka2d_reco_wrong["Mtt_tlepy"]->Fill(Mtt, tlepy, weight);
                yuka2d_reco_wrong["delY_tlepy"]->Fill(deltaY, tlepy, weight);

		//end of yukawa studies

	}
	else if(SEMILEP)
	{
		ttp_semi.Fill(bestper, weight);
                //for yukawa studies
		yuka1d_reco_semi["Mtt"]->Fill(Mtt, weight);
		yuka1d_reco_semi["costheta"]->Fill(costheta_had, weight);
		yuka1d_reco_semi["costheta"]->Fill(costheta_lep, weight);
		yuka1d_reco_semi["delY"]->Fill(deltaY, weight);
		yuka1d_reco_semi["Y"]->Fill(bestper.TLep().Rapidity(), weight);
		yuka1d_reco_semi["Y"]->Fill(bestper.THad().Rapidity(), weight);
		yuka1d_reco_semi["delBeta"]->Fill(deltaBeta, weight);
                yuka2d_reco_semi["Mtt_costheta"]->Fill(Mtt, costheta_had, weight);
                yuka2d_reco_semi["Mtt_costheta"]->Fill(Mtt, costheta_lep, weight);
                yuka2d_reco_semi["delY_costheta"]->Fill(deltaY, costheta_lep, weight);
                yuka2d_reco_semi["delY_costheta"]->Fill(deltaY, costheta_had, weight);
		yuka2d_reco_semi["Mtt_delY"]->Fill(Mtt, deltaY, weight);
		yuka2d_reco_semi["Mtt_delBeta"]->Fill(Mtt, deltaBeta, weight);
		yuka2d_reco_semi["delY_delBeta"]->Fill(deltaY, deltaBeta, weight);
	
		yuka1d_reco_semi["Mtt_boo"]->Fill(Mtt_boost, weight);
		yuka1d_reco_semi["costheta"]->Fill(costheta_had_boost, weight);
		yuka1d_reco_semi["costheta"]->Fill(costheta_lep_boost, weight);
		yuka1d_reco_semi["delY_boo"]->Fill(deltaY_boost, weight);
		yuka1d_reco_semi["delBeta_boo"]->Fill(deltaBeta_boost, weight);
                yuka2d_reco_semi["Mtt_costheta_boo"]->Fill(Mtt_boost, costheta_had_boost, weight);
                yuka2d_reco_semi["Mtt_costheta_boo"]->Fill(Mtt_boost, costheta_lep_boost, weight);
		yuka2d_reco_semi["Mtt_delY_boo"]->Fill(Mtt_boost, deltaY_boost, weight);
		yuka2d_reco_semi["Mtt_delBeta_boo"]->Fill(Mtt_boost, deltaBeta_boost, weight);
		yuka2d_reco_semi["delY_delBeta_boo"]->Fill(deltaY_boost, deltaBeta_boost, weight);

                yuka1d_reco_semi["tlepy"]->Fill(tlepy, weight);
                yuka2d_reco_semi["Mtt_tlepy"]->Fill(Mtt, tlepy, weight);
                yuka2d_reco_semi["delY_tlepy"]->Fill(deltaY, tlepy, weight);

		//end of yukawa studies

	}
	else
	{
		ttp_other.Fill(bestper, weight);
                //for yukawa studies
		yuka1d_reco_other["Mtt"]->Fill(Mtt, weight);
		yuka1d_reco_other["costheta"]->Fill(costheta_had, weight);
		yuka1d_reco_other["costheta"]->Fill(costheta_lep, weight);
		yuka1d_reco_other["delY"]->Fill(deltaY, weight);
		yuka1d_reco_other["Y"]->Fill(bestper.TLep().Rapidity(), weight);
		yuka1d_reco_other["Y"]->Fill(bestper.THad().Rapidity(), weight);
		yuka1d_reco_other["delBeta"]->Fill(deltaBeta, weight);
                yuka2d_reco_other["Mtt_costheta"]->Fill(Mtt, costheta_had, weight);
                yuka2d_reco_other["Mtt_costheta"]->Fill(Mtt, costheta_lep, weight);
                yuka2d_reco_other["delY_costheta"]->Fill(deltaY, costheta_lep, weight);
                yuka2d_reco_other["delY_costheta"]->Fill(deltaY, costheta_had, weight);
		yuka2d_reco_other["Mtt_delY"]->Fill(Mtt, deltaY, weight);
		yuka2d_reco_other["Mtt_delBeta"]->Fill(Mtt, deltaBeta, weight);
		yuka2d_reco_other["delY_delBeta"]->Fill(deltaY, deltaBeta, weight);
	
		yuka1d_reco_other["Mtt_boo"]->Fill(Mtt_boost, weight);
		yuka1d_reco_other["costheta"]->Fill(costheta_had_boost, weight);
		yuka1d_reco_other["costheta"]->Fill(costheta_lep_boost, weight);
		yuka1d_reco_other["delY_boo"]->Fill(deltaY_boost, weight);
		yuka1d_reco_other["delBeta_boo"]->Fill(deltaBeta_boost, weight);
                yuka2d_reco_other["Mtt_costheta_boo"]->Fill(Mtt_boost, costheta_had_boost, weight);
                yuka2d_reco_other["Mtt_costheta_boo"]->Fill(Mtt_boost, costheta_lep_boost, weight);
		yuka2d_reco_other["Mtt_delY_boo"]->Fill(Mtt_boost, deltaY_boost, weight);
		yuka2d_reco_other["Mtt_delBeta_boo"]->Fill(Mtt_boost, deltaBeta_boost, weight);
		yuka2d_reco_other["delY_delBeta_boo"]->Fill(deltaY_boost, deltaBeta_boost, weight);

                yuka1d_reco_other["tlepy"]->Fill(tlepy, weight);
                yuka2d_reco_other["Mtt_tlepy"]->Fill(Mtt, tlepy, weight);
                yuka2d_reco_other["delY_tlepy"]->Fill(deltaY, tlepy, weight);

		//end of yukawa studies

	}

	if(bestper.AreHadJetsCorrect(rightper) && bestper.IsBLepCorrect(rightper))
	{
		ttp_tlepthad_right.Fill(bestper, weight);
	}
	else if(bestper.AreHadJetsCorrect(rightper))
	{
		ttp_thad_right.Fill(bestper, weight);
	}
	else if(bestper.IsBLepCorrect(rightper))
	{
		ttp_tlep_right.Fill(bestper, weight);
	}
	else if(SEMILEP)
	{
		ttp_nn_right.Fill(bestper, weight);
	}
	else
	{
		ttp_nsemi_right.Fill(bestper, weight);
	}

	//if(BTAGMODE){btageff.Fill(bestper, nvtx, rightper.IsCorrect(bestper), weight);}
	if(BTAGMODE){btageff.Fill(bestper, nvtx, bestper.IsTHadCorrect(rightper), bestper.IsBLepCorrect(rightper), weight);}
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
	while(event.next())
	{
		nevent++;
		if(nevent % 10000 == 0)cout << "Event:" << nevent << " " << event.run << endl;
		sgenparticles.clear();
		genfincls.clear();
		genbpartons.clear();
		gencpartons.clear();
		//gent = 0;
		//gentbar = 0;

		sgenjets.clear();
		genaddjets.clear();

		genallper.Reset();
		psper.Reset();
		rightper.Reset();
		genper = 0;

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
		mcweight = 1.;	
		if(event.PUInfos().size() > 0)
		{
			isMC = true;
			const Geninfo& info = event.genInfo();
			mcweight = (info.weight() < 1. ? -1. : 1.);
			truth1d["counter"]->Fill(19.5, weight);
			const vector<Mcweight>& ws =  event.MCWeights();
			//mcweight = (ws[0].weights() < 1. ? -1. : 1.);
                        //these lines below are frac. and ren. scale reweighnig, for sys uncertainties 
			if(cfacscale == -1) mcweight = ws[2].weights()/Abs(ws[0].weights());
			else if(cfacscale == 1) mcweight = ws[1].weights()/Abs(ws[0].weights());
			if(crenscale == -1) mcweight = ws[6].weights()/Abs(ws[0].weights());
			else if(crenscale == 1) mcweight = ws[3].weights()/Abs(ws[0].weights());
			truth1d["counter"]->Fill(18.5, mcweight);
			weight *= mcweight;
			double npu = event.PUInfos()[0].nInteractions();
			//cout << event.PUInfos()[0].nInteractions() << " " << event.PUInfos()[1].nInteractions() << endl;
			truth1d["Mu"]->Fill(npu, weight);
			weight *= puhist->GetBinContent(puhist->FindFixBin(npu));
			//cout << weight << " " << npu << endl;
			truth1d["MuWeighted"]->Fill(npu, weight);
			truth1d["counter"]->Fill(17.5, weight);
		}
		else
		{
			//if(event.run < 251244) {return;}
			runinfo[event.run].insert(event.lumi);
		}

		SelectGenParticles(event);
		SelectPseudoTop(event);
		// Reweighting stuffs for yukawa study (should do it before next if statement, doesn't matter before or after the SelectPseudoTop(event) but keep after the SelecGenParticles())
        
        if(SEMILEP){
	//sort(genwpartons.begin(), genwpartons.end(), [](GenObject* A, GenObject* B){return(A->Pt() > B->Pt());});
        //TLorentzVector tlep_3j = gentqlep;
        //TLorentzVector thad_3j_gen = gentqhad - genwpartons[1];

        gen3j1d["tlep_pt"]->Fill(gentqlep.Pt(), weight);
        gen3j1d["thad_pt"]->Fill(gentqhad_3j.Pt(), weight);
        gen3j1d["tlep_y"]->Fill(Abs(gentqlep.Rapidity()), weight);
        gen3j1d["thad_y"]->Fill(Abs(gentqhad_3j.Rapidity()), weight);
        gen3j1d["tlep_M"]->Fill(gentqlep.Mag(), weight);
        gen3j1d["thad_M"]->Fill(gentqhad_3j.Mag(), weight);
        gen3j1d["tt_pt"]->Fill((gentqlep + gentqhad_3j).Pt(), weight);
        gen3j1d["tt_y"]->Fill(Abs((gentqlep + gentqhad_3j).Rapidity()), weight);
        gen3j1d["Mtt"]->Fill((gentqlep + gentqhad_3j).Mag(), weight);
        gen3j1d["delY"]->Fill(gentqlep.Rapidity() - gentqhad_3j.Rapidity(), weight);
        gen3j2d["Mtt_delY"]->Fill((gentqlep + gentqhad_3j).Mag(), gentqlep.Rapidity()-gentqhad_3j.Rapidity(), weight);

        gen3j1d["thadmiss_e"]->Fill(gentqhad_miss.E(), weight);
        gen3j1d["thadmiss_pt"]->Fill(gentqhad_miss.Pt(), weight);
        gen3j1d["thadmiss_y"]->Fill(gentqhad_miss.Rapidity(), weight);
        gen3j1d["thadmiss_DeltaR"]->Fill(gentqhad_miss.DeltaR(gentqhad_misspartner), weight);
        gen3j1d["delpt_pt"]->Fill((gentqhad.Pt() - gentqhad_3j.Pt())/gentqhad_3j.Pt(), weight);
        gen3j1d["dely_y"]->Fill((gentqhad.Rapidity() - gentqhad_3j.Rapidity())/gentqhad_3j.Rapidity(), weight);


        //}

                //if(genallper.IsComplete()){
                                TLorentzVector CMttbar = gentq + gentqbar; //genallper.THad() + genallper.TLep(); //gentoplep + gentophad;
				TLorentzVector CMhadt = gentqhad; //genallper.THad(); //gentophad;
				TLorentzVector CMlept = gentqlep; //genallper.TLep(); //gentoplep;
				CMttbar.Boost(-CMttbar.BoostVector());
				CMhadt.Boost(-CMttbar.BoostVector());
				CMlept.Boost(-CMttbar.BoostVector());

				//CASE1: Mtt and cos(theta)
				double Mtt = (gentq + gentqbar).M();//(genallper.TLep() + genallper.THad()).M(); //(gentoplep + gentophad).M();
				double Mtt_boost = CMttbar.M();
				//if(Mtt >3000 || Mtt<346) continue;
				//int weight_bin_mtt = int( (Mtt - 346)/1 + 0.5);//0.5 is for int
				
				double costheta_had = gentqhad.Pz()/gentqhad.Mag(); //genallper.THad().Px()/genallper.THad().Mag(); //gentophad.Pz()/gentophad.Mag();
				double costheta_lep = gentqlep.Pz()/gentqlep.Mag(); //genallper.TLep().Pz()/genallper.TLep().Mag(); //gentoplep.Pz()/gentoplep.Mag();
				double costheta_had_boost = CMhadt.Pz()/CMhadt.Mag();
				double costheta_lep_boost = CMlept.Pz()/CMlept.Mag();

				//CASE2: deltaY
				double deltaY = gentqlep.Rapidity() - gentqhad.Rapidity(); //genallper.TLep().Rapidity() - genallper.THad().Rapidity(); //gentoplep.Rapidity() - gentophad.Rapidity();
				double deltaY_boost = CMlept.Rapidity() - CMhadt.Rapidity();
				//if(Abs(deltaY) >5.69) continue;
				//int weight_bin_dely = int( (deltaY + 5.69)/0.01 + 0.5);
				//weight *= yukahist_2d->GetBinContent(weight_bin_mtt+1, weight_bin_dely+1) + 1
                                //
                               
                                
                                if(Mtt>= 2*173*cosh(deltaY/2))
                                weight *= yukahist_2d->GetBinContent(yukahist_2d->GetXaxis()->FindFixBin(Mtt), yukahist_2d->GetYaxis()->FindFixBin(deltaY)) + 1;
                                else
                                weight *= 1;
                                

				//CASE3: deltaBate
				double deltaBeta = gentqlep.P()/gentqlep.E() - gentqhad.P()/gentqhad.E(); //genallper.TLep().P()/genallper.TLep().E() - genallper.THad().P()/genallper.THad().E(); //gentoplep.P()/gentoplep.E() - gentophad.P()/gentophad.E();
				double deltaBeta_boost = CMlept.P()/CMlept.E() + CMhadt.P()/CMhadt.E();
				//if(deltaBeta >2) continue;
				//int weight_bin_beta = int(deltaBeta/0.1 + 0.5);
				//weight_beta *= yukahist_beta->GetBinContent(weight_bin_beta + 1) + 1;
			
                                double tlepy = Abs(gentqlep.Rapidity());


				//weight = weight_mtt* weight_dely* weight_beta;
				//cout<< "weight_mtt = " << weight_mtt <<", "<< "weight_dely = " << weight_dely <<", "<< "weight_beta = " << weight_beta <<", "<< "weight = " << weight << endl;
				yuka1d_gen["Mtt"]->Fill(Mtt, weight);
				yuka1d_gen["costheta"]->Fill(costheta_lep, weight);
				yuka1d_gen["costheta"]->Fill(costheta_had, weight);
				yuka1d_gen["delY"]->Fill(deltaY, weight);
				yuka1d_gen["Y"]->Fill(gentq.Rapidity(), weight); //Fill(genallper.TLep().Rapidity(), weight);
				yuka1d_gen["Y"]->Fill(gentqbar.Rapidity(), weight); //Fill(genallper.THad().Rapidity(), weight);
				yuka1d_gen["delBeta"]->Fill(deltaBeta, weight);
                                yuka2d_gen["Mtt_costheta"]->Fill(Mtt, costheta_lep, weight);
                                yuka2d_gen["Mtt_costheta"]->Fill(Mtt, costheta_had, weight);
                                yuka2d_gen["delY_costheta"]->Fill(deltaY, costheta_lep, weight);
                                yuka2d_gen["delY_costheta"]->Fill(deltaY, costheta_had, weight);
				yuka2d_gen["Mtt_delY"]->Fill(Mtt, deltaY, weight);
				yuka2d_gen["Mtt_delBeta"]->Fill(Mtt, deltaBeta, weight);
				yuka2d_gen["delY_delBeta"]->Fill(deltaY, deltaBeta, weight);
				
				yuka1d_gen["Mtt_boo"]->Fill(Mtt_boost, weight);	
				yuka1d_gen["costheta_boo"]->Fill(costheta_lep_boost, weight);
				yuka1d_gen["costheta_boo"]->Fill(costheta_had_boost, weight);
				yuka1d_gen["delY_boo"]->Fill(deltaY_boost, weight);
				yuka1d_gen["delBeta_boo"]->Fill(deltaBeta_boost, weight);
                                yuka2d_gen["Mtt_costheta_boo"]->Fill(Mtt_boost, costheta_lep_boost, weight);
                                yuka2d_gen["Mtt_costheta_boo"]->Fill(Mtt_boost, costheta_had_boost, weight);
				yuka2d_gen["Mtt_delY_boo"]->Fill(Mtt_boost, deltaY_boost, weight);
				yuka2d_gen["Mtt_delBeta_boo"]->Fill(Mtt_boost, deltaBeta_boost, weight);
				yuka2d_gen["delY_delBeta_boo"]->Fill(deltaY_boost, deltaBeta_boost, weight);

                                yuka1d_gen["tlepy"]->Fill(tlepy, weight);
                                yuka2d_gen["Mtt_tlepy"]->Fill(Mtt, tlepy, weight);
                                yuka2d_gen["delY_tlepy"]->Fill(deltaY, tlepy, weight);
				//cout<<"yukawa sample weight = "<<weight<<endl;
				//the end of the twice yukawa const test.
                    }

		if(PSEUDOTOP)
		{
			if(genallper.IsComplete())
			{
				response_ps.FillTruth("thardpt", Max(gentq.Pt(), gentqbar.Pt()), weight);
				response_ps.FillTruth("tsoftpt", Min(gentq.Pt(), gentqbar.Pt()), weight);
				response_ps.FillTruth("thadpt", gentqhad.Pt(), weight);
				response_ps.FillTruth("thady", Abs(gentqhad.Rapidity()), weight);
				response_ps.FillTruth("tleppt", gentqlep.Pt(), weight);
				response_ps.FillTruth("tlepy", Abs(gentqlep.Rapidity()), weight);
				response_ps.FillTruth("ttm", (gentq+gentqbar).M(), weight);
				response_ps.FillTruth("ttpt",(gentq+gentqbar).Pt(), weight);
				response_ps.FillTruth("tty", Abs((gentq+gentqbar).Rapidity()), weight);
			}
			if(psper.IsComplete())
			{
				response_ps.FillAll("thardpt", psper.THard().Pt(), weight);
				response_ps.FillAll("tsoftpt", psper.TSoft().Pt(), weight);
				response_ps.FillAll("thadpt", psper.THad().Pt(), weight);
				response_ps.FillAll("thady", Abs(psper.THad().Rapidity()), weight);
				response_ps.FillAll("tleppt", psper.TLep().Pt(), weight);
				response_ps.FillAll("tlepy", Abs(psper.TLep().Rapidity()), weight);
				response_ps.FillAll("ttm", psper.TT().M(), weight);
				response_ps.FillAll("ttpt", psper.TT().Pt(), weight);
				response_ps.FillAll("tty", Abs(psper.TT().Rapidity()), weight);
			}
			if(genallper.IsComplete() && psper.IsComplete())
			{
				response_ps.FillTruthReco("thardpt", Max(gentq.Pt(), gentqbar.Pt()), psper.THard().Pt(), weight);
				response_ps.FillTruthReco("tsoftpt", Min(gentq.Pt(), gentqbar.Pt()), psper.TSoft().Pt(), weight);
				response_ps.FillTruthReco("thadpt", gentqhad.Pt(), psper.THad().Pt(), weight);
				response_ps.FillTruthReco("thady", Abs(gentqhad.Rapidity()), Abs(psper.THad().Rapidity()), weight);
				response_ps.FillTruthReco("tleppt", gentqlep.Pt(), psper.TLep().Pt(), weight);
				response_ps.FillTruthReco("tlepy", Abs(gentqlep.Rapidity()), Abs(psper.TLep().Rapidity()), weight);
				response_ps.FillTruthReco("ttm", (gentq+gentqbar).M(), psper.TT().M(), weight);
				response_ps.FillTruthReco("ttpt", (gentq+gentqbar).Pt(), psper.TT().Pt(), weight);
				response_ps.FillTruthReco("tty", Abs((gentq+gentqbar).Rapidity()), Abs(psper.TT().Rapidity()), weight);
			}
			genper = &psper;
			SEMILEP = psper.IsComplete();
			SEMILEPACC = SEMILEP;
                        if(psper.IsComplete())
                        {
                            gent = psper.T();
                            gentbar = psper.Tb();
                            gentlep = psper.TLep();
                            genthad = psper.THad();
                        }
                                
		}
		else
		{
			genper = &genallper;
                        gent = gentq;
                        gentbar = gentqbar;
                        gentlep = gentqlep;
                        genthad = gentqhad;
		}
		AddGenJetSelection(event);

		if(SEMILEP) 
		{
			truth1d["counter"]->Fill(1.5, weight);
			ttp_genall.Fill(*genper, weight);

			response.FillTruth("thardpt", genper->THard().Pt(), weight);
			response.FillTruth("tsoftpt", genper->TSoft().Pt(), weight);
			response.FillTruth("nobin", genper->THad().Pt(), weight);
			response.FillTruth("thadpt", genper->THad().Pt(), weight);
			response.FillTruth("thady", Abs(genper->THad().Rapidity()), weight);
			response.FillTruth("tleppt", genper->TLep().Pt(), weight);
			response.FillTruth("tlepy", Abs(genper->TLep().Rapidity()), weight);
			response.FillTruth("ttm", genper->TT().M(), weight);
			response.FillTruth("ttpt", genper->TT().Pt(), weight);
			response.FillTruth("tty", Abs(genper->TT().Rapidity()), weight);
			response.FillTruth("njet", genaddjets.size(), weight);
			response.FillTruth("dymp", Abs(genper->T().Rapidity()) - Abs(genper->Tb().Rapidity()), weight);
			response2d.FillTruth("njets_thadpt", genaddjets.size(), genper->THad().Pt(), weight);
			response2d.FillTruth("njets_ttpt", genaddjets.size(), genper->TT().Pt(), weight);
			if(PDFTEST)
			{
				pdfunc->Fill1d("pdfunc_thardpt", genper->THard().Pt(), weight);
				pdfunc->Fill1d("pdfunc_tsoftpt", genper->TSoft().Pt(), weight);
				pdfunc->Fill1d("pdfunc_nobin", genper->THad().Pt(), weight);
				pdfunc->Fill1d("pdfunc_thadpt", genper->THad().Pt(), weight);
				pdfunc->Fill1d("pdfunc_tleppt", genper->TLep().Pt(), weight);
				pdfunc->Fill1d("pdfunc_thady", Abs(genper->THad().Rapidity()), weight);
				pdfunc->Fill1d("pdfunc_tlepy", Abs(genper->TLep().Rapidity()), weight);
				pdfunc->Fill1d("pdfunc_ttm", genper->TT().M(), weight);
				pdfunc->Fill1d("pdfunc_tty", Abs(genper->TT().Rapidity()), weight);
				pdfunc->Fill1d("pdfunc_ttpt", genper->TT().Pt(), weight);
				pdfunc->Fill1d("pdfunc_njet", genaddjets.size(), weight);
				pdfunc->Fill1d("pdfunc_dymp", Abs(genper->T().Rapidity()) - Abs(genper->Tb().Rapidity()), weight);
				pdfunc->Fill1d("pdfunc_njets_thadpt", response2d.GetBin("njets_thadpt", genaddjets.size(), genper->THad().Pt())-0.5, weight);
				pdfunc->Fill1d("pdfunc_njets_ttpt", response2d.GetBin("njets_ttpt", genaddjets.size(), genper->TT().Pt())-0.5, weight);
			}
		}
		if(SEMILEPACC)
		{
			ttp_genacc.Fill(*genper, weight);
			truth1d["counter"]->Fill(2.5, weight);
		}
		//cout << event.filter().Flag_goodVertices() << " " <<  event.filter().Flag_CSCTightHaloFilter() << " " << event.filter().Flag_HBHENoiseFilter() << " " << event.filter().HBHEnew() << endl;
		//event.filter().Flag_goodVertices() == 1 && event.filter().Flag_CSCTightHaloFilter() == 1 &&
		if(!isMC && (Abs(event.trigger().HLT_IsoMu20()) != 1 || Abs(event.trigger().HLT_Ele22_eta2p1_WPLoose_Gsf()) != 1)) {cout << "TRIGGER UNDEFINED:" << event.trigger().HLT_IsoMu20() << " " << event.trigger().HLT_Ele22_eta2p1_WPLoose_Gsf() << endl; }
		if(
				(
				 isMC && 
				 (
				  event.trigger().HLT_IsoMu20() == 1
				  || event.trigger().HLT_IsoTkMu20() == 1
				  || event.trigger().HLT_Ele22_eta2p1_WPLoose_Gsf() == 1
				 )
				) ||
				(
				 !isMC &&
				 (
				  //event.filter().Flag_goodVertices() == 1 && event.filter().Flag_CSCTightHaloFilter() == 1 && event.filter().HBHEnew() == 1 &&
				  (
				   //event.trigger().HLT_IsoMu24_eta2p1() == 1 || (event.trigger().HLT_IsoMu24_eta2p1() == -1 && event.trigger().HLT_Ele27_eta2p1_WPLoose_Gsf() == 1)
				   event.trigger().HLT_IsoMu20() == 1 || event.trigger().HLT_IsoTkMu20() == 1 || (event.trigger().HLT_IsoMu22() == -1 && event.trigger().HLT_IsoTkMu20() == -1 && event.trigger().HLT_Ele22_eta2p1_WPLoose_Gsf() == 1)
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


//make it executable
int main(int argc, char *argv[])
{
	URParser &parser = URParser::instance(argc, argv);
	URDriver<ttbar> test;
	return test.run();
}
