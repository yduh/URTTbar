#include "ttbarxsec.h"
#include "TRandom3.h"

#include "Permutation.h"
#include "PDFuncertainty.h"
#include "NeutrinoSolver.h"
#include "ConfigParser.h"
//#include "jet3.h"
#include <TGraph.h>

using namespace std;

ttbar::ttbar(const std::string output_filename):
	AnalyzerBase("ttbar", output_filename),
        reco3j2d("3j"),
        reco3j1d("3j"),
        right3j2d("3j"),
        right3j1d("3j"),
        wrong3j2d("3j"),
        wrong3j1d("3j"),
        semi3j2d("3j"),
        semi3j1d("3j"),
        other3j2d("3j"),
        other3j1d("3j"),
        gen3j2d("3j"), 
        gen3j1d("3j"),
        alpha3j1d("3j"), 
        alpha3j2d("3j"),
        missj1d("3j"), 
        missj2d("3j"), 
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
        //lhe1d("lhe"), 
        //lhe2d("lhe"), 
        yuka1d_gen("yukawa"),
        //yuka1d_offshell("yukawa"),
        yuka1d_reco("yukawa"),
        yuka1d_reco_right("yukawa"),
        yuka1d_reco_wrong("yukawa"),
        yuka1d_reco_semi("yukawa"),
        yuka1d_reco_other("yukawa"),
        yuka2d_gen("yukawa"),
        //yuka2d_offshell("yukawa"),
        yuka2d_reco("yukawa"),
        yuka2d_reco_right("yukawa"),
        yuka2d_reco_wrong("yukawa"),
        yuka2d_reco_semi("yukawa"),
        yuka2d_reco_other("yukawa"),
	response("response", this),
	response2d("response"),
	response_ps("response_ps", this),
	//jetscaler("jetuncertainty.root"),
	PDFTEST(false),
	PSEUDOTOP(true),
	BTAGMODE(false), //set true for the b-tag efficiency measurement
	JETSCALEMODE(false), //set true for jet scale measurement
	ELECTRONS(true),
	MUONS(true),
	B_TIGHT(0.935),
	B_MEDIUM(0.8484),
	B_LOOSE(0.5426),
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
        ctoprapweight(0.),
	crenscale(0),
	cfacscale(0),
        chdamp(0),
	cbtagunc(0),
	cltagunc(0),
	cpileup(0),
        TTMC(false),
        FULLHAD(false),
        SEMILEP(false),
        FULLLEP(false),
        SEMILEPACC(false),
	HERWIGPP(false),
	PYTHIA6(false)
        //SCALEUP(false), 
        //SCALEDOWN(false)
{
	ConfigParser CP("ttbarxsec.cfg");
        cLeptonScaleFactor = CP.Get<string>("LeptonScaleFactor");
        cJetEnergyUncertainty = CP.Get<string>("JetEnergyUncertainty");
        cBTaggingSF = CP.Get<string>("BTaggingSF");
        cBTaggingEff = CP.Get<string>("BTaggingEff");
        cJetResolution = CP.Get<string>("JetResolution");
        cJetResolutionSF = CP.Get<string>("JetResolutionSF");
        cel27eff = CP.GetVector<double>("el27eff");
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
        cjecuncertainty = CP.Get<string>("jecuncertainty");
	cjetres = CP.Get<double>("jetres");
	csigmamet = CP.Get<double>("sigmamet");
	ctopptweight = CP.Get<double>("topptweight");
	cttptweight = CP.Get<double>("ttptweight");
	ctoprapweight = CP.Get<double>("toprapweight");
	if(CP.Get<bool>("mcspecificcorrections"))
	{
		if(output_filename.find("Hpp") != string::npos){HERWIGPP = true;}
		if(output_filename.find("P6") != string::npos){PYTHIA6 = true;}
		if(output_filename.find("isrup") != string::npos){ISRUP = true;}
		if(output_filename.find("isrdown") != string::npos){ISRDOWN = true;}
		if(output_filename.find("fsrup") != string::npos){FSRUP = true;}
		if(output_filename.find("fsrdown") != string::npos){FSRDOWN = true;}
		if(output_filename.find("tuneup") != string::npos){TUNEUP = true;}
		if(output_filename.find("tunedown") != string::npos){TUNEDOWN = true;}
	}

	if(output_filename.find("tt_PowhegP8") != string::npos)
	{
		cfacscale = CP.Get<int>("facscale");
		crenscale = CP.Get<int>("renscale");
                chdamp = CP.Get<int>("hdamp");
		PDFTEST = CP.Get<bool>("PDFTEST");
		cbdecay = CP.Get<double>("bdecay");
		cbfrag = CP.Get<double>("bfrag");
	}
	cbtagunc = CP.Get<int>("btagunc");
	cltagunc = CP.Get<int>("ltagunc");
	cpileup = CP.Get<int>("pileupunc");

        yukawasf = CP.Get<string>("yukawatxt");
        njetsmin = CP.Get<int>("njetsmin");
        njetsmax = CP.Get<int>("njetsmax");

	cout << output_filename << endl;
	//CHECK mcspecificcorrections
        //if(output_filename.find("Hpp") != string::npos){HERWIGPP = true;}
	//if(output_filename.find("P6") != string::npos){PYTHIA6 = true;}
        //if(output_filename.find("scaleup") != string::npos){SCALEUP = true;}
        //if(output_filename.find("scaledown") != string::npos){SCALEDOWN = true;}
        if(output_filename.find("tt_") == 0){TTMC = true;}
        isDA = 0;
        if(output_filename.find("DATAEL") == 0){isDA = 11;}
        if(output_filename.find("DATAMU") == 0){isDA = 13;} 

	jetptmin = min(cwjetptsoft, cbjetptsoft);
	
	topptbins = {0.0, 45.0, 90.0, 135.0, 180.0, 225.0, 270.0, 315.0, 400.0, 800.0};
	topybins = {0.0, 0.2, 0.4, 0.7, 1.0, 1.3, 1.6, 2.5};
	ttmbins = {300.0, 375.0, 450.0, 530.0, 625.0, 740.0, 850.0, 1100.0, 2000.0};
        mttbins = {0, 40, 80, 120, 160, 200, 240, 280, 320, 360, 400, 440, 480, 520, 560, 600, 640, 680, 720, 760, 800, 840, 880, 920, 960, 1000, 1040, 1080, 1120, 1160, 1200, 1240, 1280, 1320, 1360, 1400, 1440, 1480, 1520, 1560, 1600, 1640, 1680, 1720, 1760, 1800, 1840, 1880, 1920, 1960, 2000};
	ttybins = {0.0, 0.2, 0.4, 0.6, 0.9, 1.3, 2.3};
	ttptbins = {0.0, 35.0, 80.0, 140.0, 200.0, 500.0};
	metbins = {0.0, 30.0, 45.0, 60.0, 80.0, 120.0, 580.0};
	jetbins = {-0.5, 0.5, 1.5, 2.5, 3.5, 4.5};
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

        TDirectory* dir_3jgen = outFile_.mkdir("3j_GEN");
        dir_3jgen->cd();
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

        gen3j1d.AddHist("thadmiss_pz", 300, -300, 300, "miss-jet p_{z}", "Events");
        gen3j1d.AddHist("thadmiss_pt", 250, 0, 500, "miss-jet p_{T}", "Events"); 
        gen3j1d.AddHist("thadmiss_eta", 100, 0, 5, "miss-jet #eta", "Events");
        gen3j2d.AddHist("thadmiss_pt_eta", 250, 0, 500, 100, 0, 5, "miss-jet p_{T}", "miss-jet #eta");
        gen3j1d.AddHist("thadmiss_DeltaR", 100, 0, 5, "W-jets #DeltaR", "Events");
        gen3j1d.AddHist("thadmiss_DeltaR2", 100, 0, 5, "W-jet2 b_{h} #DeltaR", "Events");
        gen3j1d.AddHist("thadmiss_DeltaR3", 100, 0, 5, "W-jet2 b_{l} #DeltaR", "Events");

/*
        TDirectory* dir_3jalpha = outFile_.mkdir("3j_ALPHA");
        dir_3jalpha->cd();
        alpha3j2d.AddHist("genmtt_recomtt", 100, 0, 2000, 100, 0, 2000, "gen M(t#bar{t})", "reco M(t+p)");
        alpha3j2d.AddHist("mp_alphamp", 250, 0, 1000, 250, 0, 1000, "M(p)", "#alphaM(p)");
        alpha3j2d.AddHist("mp_alpha", 250, 0, 1000, 100, 0, 10, "M(p)", "#alpha");
        alpha3j1d.AddHist("thad_pt", 400, 0, 800, "p_{T}(t_{h})", "Events");
        alpha3j1d.AddHist("thad_y", 200, 0, 5, "|y(t_{h})|", "Events");
        alpha3j1d.AddHist("thad_M", 500, 0, 1000, "M(t_{h})", "Events");
        alpha3j1d.AddHist("tt_pt", 500, 0, 1000," p_{t}(t#bar{t})", "Event");
        alpha3j1d.AddHist("tt_y", 200, 0, 5, "|y(t#bar{t})|", "Event");
        alpha3j1d.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
        alpha3j1d.AddHist("delY", 1200, -6, 6, "#Deltay(t#bar{t})", "Events");
        alpha3j2d.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay(t#bar{t})");
        alpha3j1d.AddHist("Mtt_resol", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        alpha3j1d.AddHist("Mtt_resol_noalpha", 40, -2, 2, "#DeltaM(t#bar{t})/M(t#bar{t})", "Events");
        alpha3j1d.AddHist("delY_resol", 80, -4, 4, "#Deltay(t#bar{t}) reco/gen", "Events");
        alpha3j1d.AddHist("delY_resol_noalpha", 40, -2, 2, "#Delta(#Deltay(t#bar{t}))/(#Deltay(t#bar{t}))", "Events");
*/

        TDirectory* dir_3jmissj = outFile_.mkdir("3j_MISSJ");
        dir_3jmissj->cd();
        missj1d.AddHist("counter", 10, 0, 10, "counter", "Events");
        missj1d.AddHist("counter2", 10, 0, 10, "counter2", "Events");
        //missj1d.AddHist("miss_chi2", 100, 0, 500, "chi2 missj", "Events");
        //missj1d.AddHist("missj_pz", 300, -300, 300, "missj p_{z}", "Events");
        //missj1d.AddHist("missj_pt", 250, 0, 500, "missj p_{T}", "Events");
        //missj1d.AddHist("missj_eta", 100, 0, 5, "missj #eta", "Events");
        //missj1d.AddHist("missj_dpz_pz", 40, -2, 2, "missj #Deltap_{z}/p_{z}", "Events");
        //missj1d.AddHist("missj_dpt_pt", 40, -2, 2, "missj #Deltap_{T}/p_{T}", "Events");
        //missj1d.AddHist("thad_pt", 400, 0, 800, "p_{T}(t_{h})", "Events");
        //missj1d.AddHist("thad_y", 200, 0, 5, "|y(t_{h})|", "Events");
        //missj1d.AddHist("whad_M", 250, 0, 500, "M(W_{h})", "Events");
        //missj1d.AddHist("thad_M", 500, 0, 1000, "M(t_{h})", "Events");
        //missj2d.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay(t#bar{t})");
        //missj1d.AddHist("Mtt_resol", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        //missj1d.AddHist("delY_resol", 40, -2, 2, "#Delta(#Deltay(t#bar{t}))/(#Deltay(t#bar{t}))", "Events");
        /*missj1d.AddHist("likelihood2", 175, 0, 35, "-ln(#lambda)", "Events");
        missj1d.AddHist("likelihood2_right", 175, 0, 35, "-ln(#lambda)", "Events");
        missj1d.AddHist("likelihood2_wrong", 175, 0, 35, "-ln(#lambda)", "Events");
        missj1d.AddHist("likelihood2_semi", 175, 0, 35, "-ln(#lambda)", "Events");
        missj1d.AddHist("likelihood2_other", 175, 0, 35, "-ln(#lambda)", "Events");*/

        //missj1d.AddHist("acceff", 180, 0, 360, "eff on NS ellipse out of acceptance", "Events");
        //missj1d.AddHist("acceff2", 180, 0, 360, "eff on NS ellipse out of acceptance", "Events");

        missj1d.AddHist("min_nspt_right", 250, 0, 500, "min NS ellipse p_{T}", "Events");
        missj1d.AddHist("min_nspt_wrong", 250, 0, 500, "min NS ellipse p_{T}", "Events");
        missj1d.AddHist("max_nseta_right", 200, 0, 10, "max NS ellipse #eta", "Events");
        missj1d.AddHist("max_nseta_wrong", 200, 0, 10, "max NS ellipse #eta", "Events");

        missj2d.AddHist("nspt_nseta_right", 250, 0, 500, 200, 0, 10, "NS ellipse p_{T}", "NS ellipse #eta");
        missj2d.AddHist("nspt_nseta_wrong", 250, 0, 500, 200, 0, 10, "NS ellipse p_{T}", "NS ellipse #eta");
        missj2d.AddHist("nspt_thadM_right", 250, 0, 500, 500, 0, 1000, "min NS ellipse p_{T}", "M(t_{h})");
        missj2d.AddHist("nspt_thadM_wrong", 250, 0, 500, 500, 0, 1000, "min NS ellipse p_{T}", "M(t_{h})");
        missj2d.AddHist("nspt_nschi_right", 250, 0, 500, 250, 0, 500, "min NS ellipse p_{T}", "#chi neutrino-test");
        missj2d.AddHist("nspt_nschi_wrong", 250, 0, 500, 250, 0, 500, "min NS ellipse p_{T}", "#chi neutrino-test");
        missj2d.AddHist("nseta_thadM_right", 200, 0, 10, 500, 0, 1000, "NS ellipse #eta", "M(t_{h})");
        missj2d.AddHist("nseta_thadM_wrong", 200, 0, 10, 500, 0, 1000, "NS ellipse #eta", "M(t_{h})");
        missj2d.AddHist("nseta_nschi_right", 200, 0, 10, 250, 0, 500, "NS ellipse #eta", "#chi neutrino-test");
        missj2d.AddHist("nseta_nschi_wrong", 200, 0, 10, 250, 0, 500, "NS ellipse #eta", "#chi neutrino-test");
        

        //missj1d.AddHist("min_nspt_right", 250, 0, 500, "min NS ellipse p_{T}", "Events");
        //missj1d.AddHist("max_nseta_right", 200, 0, 10, "max NS ellipse #eta", "Events");
        //missj1d.AddHist("acceff_right", 180, 0, 360, "eff on NS ellipse out of acceptance", "Events");
        //missj1d.AddHist("acceff2_right", 180, 0, 360, "eff on NS ellipse out of acceptance", "Events");
        //missj1d.AddHist("min_nspt_wrong", 250, 0, 500, "min NS ellipse p_{T}", "Events");
        //missj1d.AddHist("max_nseta_wrong", 200, 0, 10, "max NS ellipse #eta", "Events");
        //missj1d.AddHist("acceff_wrong", 180, 0, 360, "eff on NS ellipse out of acceptance", "Events");
        //missj1d.AddHist("acceff2_wrong", 180, 0, 360, "eff on NS ellipse out of acceptance", "Events");
        //missj1d.AddHist("min_nspt_semi", 250, 0, 500, "min NS ellipse p_{T}", "Events");
        //missj1d.AddHist("max_nseta_semi", 200, 0, 10, "max NS ellipse #eta", "Events");
        //missj1d.AddHist("acceff_semi", 180, 0, 360, "eff on NS ellipse out of acceptance", "Events");
        //missj1d.AddHist("acceff2_semi", 180, 0, 360, "eff on NS ellipse out of acceptance", "Events");
        //missj1d.AddHist("min_nspt_other", 250, 0, 500, "min NS ellipse p_{T}", "Events");
        //missj1d.AddHist("max_nseta_other", 200, 0, 10, "max NS ellipse #eta", "Events");
        //missj1d.AddHist("acceff_other", 180, 0, 360, "eff on NS ellipse out of acceptance", "Events");
        //missj1d.AddHist("acceff2_other", 180, 0, 360, "eff on NS ellipse out of acceptance", "Events");

        TDirectory* dir_3jreco = outFile_.mkdir("3j_RECO");
        dir_3jreco->cd();
        reco3j1d.AddHist("counter", 30, 0, 30, "counter", "Events");
        reco3j1d.AddHist("counter2", 10, 0, 10, "counter2", "Events");
        reco3j1d.AddHist("counter_chi2", 15, 0, 15, "counter_chi2", "Events");

        reco3j2d.AddHist("blepnschi_bhadnschi", 250, 0, 500, 250, 0, 500, "#chi^{2} b_{l}", "#chi^{2} b_{h}");
        reco3j2d.AddHist("blept_bhadpt", 250, 0, 500, 250, 0, 500, "p_{T}(b_{l})", "p_{T}(b_{h})");
        reco3j2d.AddHist("blepy_bhady", 200, 0, 5, 200, 0, 5, "|y(b_{l})|", "|y(b_{h})|");

        reco3j1d.AddHist("tlep_pt", 400, 0, 800, "p_{T}(t_{l})", "Events");
        reco3j1d.AddHist("thad_pt", 400, 0, 800, "p_{T}(t_{h})", "Events");
        reco3j1d.AddHist("tlep_y", 200, 0, 5, "|y(t_{l})|", "Events");
        reco3j1d.AddHist("thad_y", 200, 0, 5, "|y(t_{h})|", "Events");
        reco3j1d.AddHist("tt_pt", 500, 0, 1000," p_{t}(t#bar{t})", "Event");
        reco3j1d.AddHist("tt_y", 200, 0, 5, "|y(t#bar{t})|", "Event");
        reco3j2d.AddHist("thad_M_Whad_M", 250, 0, 500, 50, 0, 100, "M(t_{h}) [GeV]", "M(W_{h}) [GeV]");
        
        reco3j1d.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
        reco3j1d.AddHist("delY", 1200, -6, 6, "#Deltay(t#bar{t})", "Events");
        reco3j2d.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay(t#bar{t})");
        reco3j1d.AddHist("Mtt_resol", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        reco3j1d.AddHist("delY_resol", 80, -4, 4, "#Deltay(t#bar{t}) reco/gen", "Events");
       
        reco3j1d.AddHist("met_pz", 300, -300, 300, "met p_{z}", "Events");
        reco3j1d.AddHist("MET", 150, 0, 300, "MET p_{T}", "Events");
        reco3j1d.AddHist("lep_eta", 480, -2.4, 2.4, "#eta(l)", "Events");
        reco3j2d.AddHist("met_pt_pz", 15, 0, 300, 30, -300, 300, "met p_{T}", "met p_{z}");
        reco3j1d.AddHist("met_dpz_pz", 40, -2, 2, "met #Deltap_{z}/p_{z}", "Events");
        reco3j1d.AddHist("met_dpt_pt", 40, -2, 2, "met #Deltap_{T}/p_{T}", "Events");
        reco3j2d.AddHist("met_dpt_pxpy", 40, -2, 2, 40, -2, 2, "met #Deltap_{x}/p_{x}", "met #Deltap_{y}/p_{y}");
        reco3j2d.AddHist("met_dpt_dpz", 40, -2, 2, 40, -2, 2, "met #Deltap_{T}/p_{T}", "met #Deltap_{z}/p_{z}");

        reco3j1d.AddHist("nschi_right", 250, 0., 500., "#chi neutrino-test", "Events");
        reco3j1d.AddHist("nschi_wrong", 250, 0., 500., "#chi neutrino-test", "Events");
        //reco3j1d.AddHist("nschi_semi", 250, 0., 500., "#chi neutrino-test", "Events");
        //reco3j1d.AddHist("nschi_other", 250, 0., 500., "#chi neutrino-test", "Events");
        reco3j1d.AddHist("thadM_right", 500, 0, 1000, "M(t_{h})", "Events");
        reco3j1d.AddHist("thadM_wrong", 500, 0, 1000, "M(t_{h})", "Events");
        //reco3j1d.AddHist("thad_M_semi", 500, 0, 1000, "M(t_{h})", "Events");
        //reco3j1d.AddHist("thad_M_other", 500, 0, 1000, "M(t_{h})", "Events");
        reco3j1d.AddHist("likelihood_right", 125, 0, 25, "-ln(#lambda)", "Events");
        reco3j1d.AddHist("likelihood_wrong", 125, 0, 25, "-ln(#lambda)", "Events");
        //reco3j1d.AddHist("likelihood_semi", 125, 0, 25, "-ln(#lambda)", "Events");
        //reco3j1d.AddHist("likelihood_other", 125, 0, 25, "-ln(#lambda)", "Events");
        
        //reco3j2d.AddHist("nschi_thadM", 250, 0., 500., 500, 0., 1000., "#chi neutrino-test", "M(b_{h}+j_{1})");
        reco3j2d.AddHist("nschi_thadM_right", 250, 0., 500., 500, 0., 1000., "#chi neutrino-test", "M(b_{h}+j_{1})");
        reco3j2d.AddHist("nschi_thadM_wrong", 250, 0., 500., 500, 0., 1000., "#chi neutrino-test", "M(b_{h}+j_{1})");

        reco3j2d.AddHist("nschi12", 250, 0., 500., 250, 0., 500., "#chi neutrino-test 1", "#chi neutrino-test 2");
        reco3j2d.AddHist("nschi23", 250, 0., 500., 250, 0., 500., "#chi neutrino-test 2", "#chi neutrino-test 3");
        reco3j2d.AddHist("nschi13", 250, 0., 500., 250, 0., 500., "#chi neutrino-test 1", "#chi neutrino-test 3");
        reco3j2d.AddHist("nschi1p2_nschi1_right", 250, -250, 250, 250, 0., 500., "D_{l} - D_{h}", "D_{l}");
        reco3j2d.AddHist("nschi1p2_nschi1_wrong", 250, -250, 250, 250, 0., 500., "D_{l} - D_{h}", "D_{l}");

        TDirectory* dir_3jreco_right = outFile_.mkdir("3j_RECO_right");
        dir_3jreco_right->cd();
        right3j2d.AddHist("blepnschi_bhadnschi", 250, 0, 500, 250, 0, 500, "#chi^{2} b_{l}", "#chi^{2} b_{h}");
        right3j2d.AddHist("blept_bhadpt", 250, 0, 500, 250, 0, 500, "p_{T}(b_{l})", "p_{T}(b_{h})");
        right3j2d.AddHist("blepy_bhady", 200, 0, 5, 200, 0, 5, "|y(b_{l})|", "|y(b_{h})|");
        right3j1d.AddHist("tlep_pt", 400, 0, 800, "p_{T}(t_{l})", "Events");
        right3j1d.AddHist("thad_pt", 400, 0, 800, "p_{T}(t_{h})", "Events");
        right3j1d.AddHist("tlep_y", 200, 0, 5, "|y(t_{l})|", "Events");
        right3j1d.AddHist("thad_y", 200, 0, 5, "|y(t_{h})|", "Events");
        right3j1d.AddHist("thad_M", 500, 0, 1000, "M(t_{h})", "Events");
        right3j1d.AddHist("tt_pt", 500, 0, 1000," p_{t}(t#bar{t})", "Event");
        right3j1d.AddHist("tt_y", 200, 0, 5, "|y(t#bar{t})|", "Event");
        
        right3j1d.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
        right3j1d.AddHist("delY", 1200, -6, 6, "#Deltay(t#bar{t})", "Events");
        right3j2d.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay(t#bar{t})");
        right3j1d.AddHist("Mtt_resol", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        right3j1d.AddHist("delY_resol", 80, -4, 4, "#Deltay(t#bar{t}) reco/gen", "Events");
        right3j1d.AddHist("likelihood", 125, 0, 25, "-ln(#lambda)", "Events");
        right3j1d.AddHist("shuffledlikelihood", 125, 0, 25, "-ln(#lambda)", "Events");
        right3j1d.AddHist("nschi", 250, 0, 500, "#chi neutrino-test", "Events");
       
        right3j1d.AddHist("met_pz", 300, -300, 300, "met p_{z}", "Events");
        right3j1d.AddHist("MET", 150, 0, 300, "MET p_{T}", "Events");
        right3j1d.AddHist("lep_eta", 480, -2.4, 2.4, "#eta(l)", "Events");
        right3j2d.AddHist("MTwl_MET", 500, 0, 1000, 150, 0, 300, "transverse mass W_{T}", "MET p_{T}");


        TDirectory* dir_3jreco_wrong = outFile_.mkdir("3j_RECO_wrong");
        dir_3jreco_wrong->cd();
        wrong3j2d.AddHist("blepnschi_bhadnschi", 250, 0, 500, 250, 0, 500, "#chi^{2} b_{l}", "#chi^{2} b_{h}");
        wrong3j2d.AddHist("blept_bhadpt", 250, 0, 500, 250, 0, 500, "p_{T}(b_{l})", "p_{T}(b_{h})");
        wrong3j2d.AddHist("blepy_bhady", 200, 0, 5, 200, 0, 5, "|y(b_{l})|", "|y(b_{h})|");
        wrong3j1d.AddHist("tlep_pt", 400, 0, 800, "p_{T}(t_{l})", "Events");
        wrong3j1d.AddHist("thad_pt", 400, 0, 800, "p_{T}(t_{h})", "Events");
        wrong3j1d.AddHist("tlep_y", 200, 0, 5, "|y(t_{l})|", "Events");
        wrong3j1d.AddHist("thad_y", 200, 0, 5, "|y(t_{h})|", "Events");
        wrong3j1d.AddHist("thad_M", 500, 0, 1000, "M(t_{h})", "Events");
        wrong3j1d.AddHist("thad_M_rightbh", 500, 0, 1000, "M(t_{h})", "Events");
        wrong3j1d.AddHist("thad_M_wrongbs", 500, 0, 1000, "M(t_{h})", "Events");
        wrong3j1d.AddHist("thad_M_wrongbh", 500, 0, 1000, "M(t_{h})", "Events");
        wrong3j1d.AddHist("tt_pt", 500, 0, 1000," p_{t}(t#bar{t})", "Event");
        wrong3j1d.AddHist("tt_y", 200, 0, 5, "|y(t#bar{t})|", "Event");
        
        wrong3j1d.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
        wrong3j1d.AddHist("delY", 1200, -6, 6, "#Deltay(t#bar{t})", "Events");
        wrong3j2d.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay(t#bar{t})");
        wrong3j1d.AddHist("Mtt_resol", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        wrong3j1d.AddHist("delY_resol", 80, -4, 4, "#Deltay(t#bar{t}) reco/gen", "Events");
        wrong3j1d.AddHist("Mtt_resol_blright", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        wrong3j1d.AddHist("delY_resol_blright", 80, -4, 4, "#Deltay(t#bar{t}) reco/gen", "Events");
        wrong3j1d.AddHist("Mtt_resol_bhright", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        wrong3j1d.AddHist("delY_resol_bhright", 80, -4, 4, "#Deltay(t#bar{t}) reco/gen", "Events");
        wrong3j1d.AddHist("Mtt_resol_bswrong", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        wrong3j1d.AddHist("delY_resol_bswrong", 80, -4, 4, "#Deltay(t#bar{t}) reco/gen", "Events");
        wrong3j1d.AddHist("likelihood", 125, 0, 25, "-ln(#lambda)", "Events");
        wrong3j1d.AddHist("shuffledlikelihood", 125, 0, 25, "-ln(#lambda)", "Events");
        wrong3j1d.AddHist("nschi", 250, 0, 500, "#chi neutrino-test", "Events");
        wrong3j1d.AddHist("nschi_rightbl", 250, 0., 500., "#chi neutrino-test", "Events");
        wrong3j1d.AddHist("nschi_wrongbl", 250, 0., 500., "#chi neutrino-test", "Events");
        wrong3j1d.AddHist("nschi_wrongbs", 250, 0., 500., "#chi neutrino-test", "Events");
        
        wrong3j1d.AddHist("met_pz", 300, -300, 300, "met p_{z}", "Events");
        wrong3j1d.AddHist("MET", 150, 0, 300, "MET p_{T}", "Events");
        wrong3j1d.AddHist("lep_eta", 480, -2.4, 2.4, "#eta(l)", "Events");
        wrong3j2d.AddHist("MTwl_MET", 500, 0, 1000, 150, 0, 300, "transverse mass W_{l}", "MET p_{T}");

        TDirectory* dir_3jreco_semi = outFile_.mkdir("3j_RECO_semi");
        dir_3jreco_semi->cd();
        semi3j2d.AddHist("blepnschi_bhadnschi", 250, 0, 500, 250, 0, 500, "#chi^{2} b_{l}", "#chi^{2} b_{h}");
        semi3j2d.AddHist("blept_bhadpt", 250, 0, 500, 250, 0, 500, "p_{T}(b_{l})", "p_{T}(b_{h})");
        semi3j2d.AddHist("blepy_bhady", 200, 0, 5, 200, 0, 5, "|y(b_{l})|", "|y(b_{h})|");
        semi3j1d.AddHist("tlep_pt", 400, 0, 800, "p_{T}(t_{l})", "Events");
        semi3j1d.AddHist("thad_pt", 400, 0, 800, "p_{T}(t_{h})", "Events");
        semi3j1d.AddHist("tlep_y", 200, 0, 5, "|y(t_{l})|", "Events");
        semi3j1d.AddHist("thad_y", 200, 0, 5, "|y(t_{h})|", "Events");
        semi3j1d.AddHist("thad_M", 500, 0, 1000, "M(t_{h})", "Events");
        semi3j1d.AddHist("tt_pt", 500, 0, 1000," p_{t}(t#bar{t})", "Event");
        semi3j1d.AddHist("tt_y", 200, 0, 5, "|y(t#bar{t})|", "Event");
        
        semi3j1d.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
        semi3j1d.AddHist("delY", 1200, -6, 6, "#Deltay(t#bar{t})", "Events");
        semi3j2d.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay(t#bar{t})");
        semi3j1d.AddHist("Mtt_resol", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        semi3j1d.AddHist("delY_resol", 80, -4, 4, "#Deltay(t#bar{t}) reco/gen", "Events");
        semi3j1d.AddHist("likelihood", 125, 0, 25, "-ln(#lambda)", "Events");
        semi3j1d.AddHist("shuffledlikelihood", 125, 0, 25, "-ln(#lambda)", "Events");
        semi3j1d.AddHist("nschi", 250, 0, 500, "#chi neutrino-test", "Events");
       
        semi3j1d.AddHist("met_pz", 300, -300, 300, "met p_{z}", "Events");
        semi3j1d.AddHist("MET", 150, 0, 300, "MET p_{T}", "Events");
        semi3j1d.AddHist("lep_eta", 480, -2.4, 2.4, "#eta(l)", "Events");
        semi3j2d.AddHist("MTwl_MET", 500, 0, 1000, 150, 0, 300, "transverse mass W_{l}", "MET p_{T}");

        TDirectory* dir_3jreco_other = outFile_.mkdir("3j_RECO_other");
        dir_3jreco_other->cd();
        other3j2d.AddHist("blepnschi_bhadnschi", 250, 0, 500, 250, 0, 500, "#chi^{2} b_{l}", "#chi^{2} b_{h}");
        other3j2d.AddHist("blept_bhadpt", 250, 0, 500, 250, 0, 500, "p_{T}(b_{l})", "p_{T}(b_{h})");
        other3j2d.AddHist("blepy_bhady", 200, 0, 5, 200, 0, 5, "|y(b_{l})|", "|y(b_{h})|");
        other3j1d.AddHist("tlep_pt", 400, 0, 800, "p_{T}(t_{l})", "Events");
        other3j1d.AddHist("thad_pt", 400, 0, 800, "p_{T}(t_{h})", "Events");
        other3j1d.AddHist("tlep_y", 200, 0, 5, "|y(t_{l})|", "Events");
        other3j1d.AddHist("thad_y", 200, 0, 5, "|y(t_{h})|", "Events");
        other3j1d.AddHist("thad_M", 500, 0, 1000, "M(t_{h})", "Events");
        other3j1d.AddHist("tt_pt", 500, 0, 1000," p_{t}(t#bar{t})", "Event");
        other3j1d.AddHist("tt_y", 200, 0, 5, "|y(t#bar{t})|", "Event");
        
        other3j1d.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
        other3j1d.AddHist("delY", 1200, -6, 6, "#Deltay(t#bar{t})", "Events");
        other3j2d.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay(t#bar{t})");
        other3j1d.AddHist("Mtt_resol", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        other3j1d.AddHist("delY_resol", 80, -4, 4, "#Deltay(t#bar{t}) reco/gen", "Events");
        other3j1d.AddHist("likelihood", 125, 0, 25, "-ln(#lambda)", "Events");
        other3j1d.AddHist("shuffledlikelihood", 125, 0, 25, "-ln(#lambda)", "Events");
        other3j1d.AddHist("nschi", 250, 0, 500, "#chi neutrino-test", "Events");

        other3j1d.AddHist("met_pz", 300, -300, 300, "met p_{z}", "Events");
        other3j1d.AddHist("MET", 150, 0, 300, "MET p_{T}", "Events");
        other3j1d.AddHist("lep_eta", 480, -2.4, 2.4, "#eta(l)", "Events");
        other3j2d.AddHist("MTwl_MET", 500, 0, 1000, 150, 0, 300, "transverse mass W_{l}", "MET p_{T}");

	
        TDirectory* dir_gen = outFile_.mkdir("GEN");
	dir_gen->cd();
	gen1d.AddHist("TYP", 4, 0., 4., "Decay TYP", "Events");
	gen1d.AddHist("DRW", 600, 0., 6., "DR", "Events");
	gen1d.AddHist("DRB", 600, 0., 6., "DR", "Events");
        gen1d.AddHist("tpt", 500, 0, 1000, "p_{T}(t) [GeV]", "Events");
        gen1d.AddHist("ty", 500, 0, 10, "y(t)", "Events");
        gen1d.AddHist("ttpt", 500, 0, 1000, "p_{T}(t#bar{t}) [GeV]", "Events");
        gen1d.AddHist("tty", 500, 0, 10, "y(t#bar{t})", "Events");
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
        truth2d.AddHist("met_dpt_pxpy", 40, -2, 2, 40, -2, 2, "NS px - MET px", "NS py - MET py");
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
	truth1d.AddHist("xb", 150, 0, 1.5, "x_{b}", "Events");
	truth1d.AddHist("xc", 150, 0, 1.5, "x_{c}", "Events");
	truth1d.AddHist("bids_had", 10000, 0, 10000, "pdgids b", "Events");
	truth1d.AddHist("bids_lep", 10000, 0, 10000, "pdgids b", "Events");

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
		/*pdfunc->Add1dHist("pdfunc_thardpt", topptbins, "hard p_{T}(t) [GeV]", "Events");
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
		pdfunc->Add1dHist("pdfunc_reco_njets_ttpt", response2d.GetNBins("njets_ttpt"), 0., response2d.GetNBins("njets_ttpt") , "bin", "Events");*/
                
                //add mtt and dely here later for PDF uncertainties 
		//pdfunc->Add1dHist("pdfunc_dy1mtt", mttbins, "|y(t)|-|y(#bar{t})|", "Events");//|dy| = 0 ~ 0.5
		//pdfunc->Add1dHist("pdfunc_dy2mtt", mttbins, "|y(t)|-|y(#bar{t})|", "Events");//|dy| = 0.5 ~ 1.0
		//pdfunc->Add1dHist("pdfunc_dy3mtt", mttbins, "|y(t)|-|y(#bar{t})|", "Events");//|dy| = 1.0 ~ 1.5
		//pdfunc->Add1dHist("pdfunc_dy4mtt", mttbins, "|y(t)|-|y(#bar{t})|", "Events");//|dy| = 1.5 ~ 6.0
		
                pdfunc->Add1dHist("pdfunc_reco_dy1mtt", mttbins, "|y(t)|-|y(#bar{t})|", "Events");//|dy| = 0 ~ 0.5
		pdfunc->Add1dHist("pdfunc_reco_dy2mtt", mttbins, "|y(t)|-|y(#bar{t})|", "Events");//|dy| = 0.5 ~ 1.0
		pdfunc->Add1dHist("pdfunc_reco_dy3mtt", mttbins, "|y(t)|-|y(#bar{t})|", "Events");//|dy| = 1.0 ~ 1.5
		pdfunc->Add1dHist("pdfunc_reco_dy4mtt", mttbins, "|y(t)|-|y(#bar{t})|", "Events");//|dy| = 1.5 ~ 6.0

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

	btageff.Init(B_MEDIUM, 0, 0);

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

        /*TDirectory* dir_LHE = outFile_.mkdir("LHE");
	dir_LHE->cd();
        lhe1d.AddHist("mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
	lhe1d.AddHist("delY", 1200, -6, 6, "#Deltay_{t#bar{t}}", "Events");
	lhe2d.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay_{t#bar{t}}");
        lhe1d.AddHist("mtt_constrain", 1000, 0, 2000, "M(t#bar{t})", "Events");
	lhe2d.AddHist("Mtt_constrain_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay_{t#bar{t}}");*/
        

        TDirectory* dir_yukawagen = outFile_.mkdir("YUKAWA_GEN");
	dir_yukawagen->cd();
        //yuka1d_gen.AddHist("parametrize", 65, 150, 800, "running M(t)", "Events");
        yuka2d_gen.AddHist("Mtt_coshy2", 1000, 0, 2000, 500, 0, 5, "M(t#bar{t})", "cosh(#Deltay/2)");
        yuka2d_gen.AddHist("reweight_Mt", 500, 0, 1000, 400, -0.2, 0.2, "factor v", "EW/LO");
        yuka2d_gen.AddHist("reweight_Mtt", 1000, 0, 2000, 400, -0.2, 0.2, "M(t#bar{t})", "EW/LO");
        yuka2d_gen.AddHist("reweight_delY", 1200, -6, 6, 400, -0.2, 0.2, "#Deltay(t#bar{t})", "EW/LO");
        yuka2d_gen.AddHist("Mtt_coshy2_origin", 1000, 0, 2000, 500, 0, 5, "M(t#bar{t})", "cosh(#Deltay/2)");
        yuka2d_gen.AddHist("reweight_Mt_origin", 500, 0, 1000, 400, -0.2, 0.2, "factor v", "EW/LO");
        yuka2d_gen.AddHist("reweight_Mtt_origin", 1000, 0, 2000, 400, -0.2, 0.2, "M(t#bar{t})", "EW/LO");
        yuka2d_gen.AddHist("reweight_delY_origin", 1200, -6, 6, 400, -0.2, 0.2, "#Deltay(t#bar{t})", "EW/LO");

	yuka1d_gen.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
	//yuka1d_gen.AddHist("costheta", 40, -1, 1, "cos#theta", "Events");
	yuka1d_gen.AddHist("Y", 160, -4, 4,"y_t", "Events");
	yuka1d_gen.AddHist("delY", 1200, -6, 6, "#Deltay_{t#bar{t}}", "Events");
	//yuka1d_gen.AddHist("delBeta", 200, 0, 2, "#Delta#beta_{t#bar{t}}", "Events");
        //yuka2d_gen.AddHist("Mtt_costheta", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
        //yuka2d_gen.AddHist("delY_costheta", 1200, -6, 6, 40, -1, 1, "#Deltay(t#bar{t})", "cos#theta");
	yuka2d_gen.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay_{t#bar{t}}");
	//yuka2d_gen.AddHist("Mtt_delBeta", 1000, 0, 2000, 200, 0, 2, "M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	//yuka2d_gen.AddHist("delY_delBeta", 1200, -6, 6, 200, 0, 2, "#Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");	
	
        yuka1d_gen.AddHist("tlepy", 200, 0, 5, "|y(t_{l}|", "Events");
        yuka2d_gen.AddHist("Mtt_tlepy", 1000, 0, 2000, 200, 0, 5, "M(t#bar{t})", "|y(t_{l}|");
        yuka2d_gen.AddHist("delY_tlepy", 1200, -6, 6, 200, 0, 5, "#Deltay_{t#bar{t}}", "|y(t_{l}|");

        /*yuka1d_offshell.AddHist("Mt", 250, 0, 500, "offshell M(t) check", "Events");
        yuka1d_offshell.AddHist("Mtbar", 250, 0, 500, "offshell M(t) check", "Events");
        yuka1d_offshell.AddHist("pt", 250, 0, 500, "offshell pt(t)", "Events");
        yuka1d_offshell.AddHist("pz", 500, 0, 1000, "offshell pz(t)", "Events");
        yuka1d_offshell.AddHist("diffweight", 400, 0.8, 1.2, "weight", "Events");
	yuka2d_gen.AddHist("newMtt_delY", 1000, 0, 2000, 1200, -6, 6, "new offshell M(t#bar{t})", "offshell #Deltay_{t#bar{t}}");*/

	TDirectory* dir_yukawareco = outFile_.mkdir("YUKAWA_RECO");
	dir_yukawareco->cd();
        //yuka1d_reco.AddHist("parametrize", 65, 150, 800, "running M(t)", "Events");
        yuka2d_reco.AddHist("Mtt_coshy2", 1000, 0, 2000, 500, 0, 5, "M(t#bar{t})", "cosh(#Deltay/2)");
        yuka2d_reco.AddHist("reweight_Mt", 500, 0, 1000, 400, -0.2, 0.2, "factor v", "EW/LO");
        yuka2d_reco.AddHist("reweight_Mtt", 1000, 0, 2000, 400, -0.2, 0.2, "M(t#bar{t})", "EW/LO");
        yuka2d_reco.AddHist("reweight_delY", 1200, -6, 6, 400, -0.2, 0.2, "#Deltay(t#bar{t})", "EW/LO");
        yuka2d_reco.AddHist("Mtt_coshy2_origin", 1000, 0, 2000, 500, 0, 5, "M(t#bar{t})", "cosh(#Deltay/2)");
        yuka2d_reco.AddHist("reweight_Mt_origin", 500, 0, 1000, 400, -0.2, 0.2, "factor v", "EW/LO");
        yuka2d_reco.AddHist("reweight_Mtt_origin", 1000, 0, 2000, 400, -0.2, 0.2, "M(t#bar{t})", "EW/LO");
        yuka2d_reco.AddHist("reweight_delY_origin", 1200, -6, 6, 400, -0.2, 0.2, "#Deltay(t#bar{t})", "EW/LO");

        yuka1d_reco.AddHist("Mtt_resol", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        yuka1d_reco.AddHist("delY_resol", 80, -4, 4, "#Deltay(t#bar{t}) reco/gen", "Events");
        yuka1d_reco.AddHist("MTwl", 500, 0, 1000, "transverse mass W_{T}", "Events");
        yuka1d_reco.AddHist("njets", 11, 0, 11, "njets", "Events");
        yuka1d_reco.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
	//yuka1d_reco.AddHist("costheta", 40, -1, 1, "cos#theta", "Events");
	yuka1d_reco.AddHist("Y", 160, -4, 4,"y_t", "Events");
	yuka1d_reco.AddHist("delY", 1200, -6, 6,"#Deltay_{t#bar{t}}", "Events");
	//yuka1d_reco.AddHist("delBeta", 200, 0, 2, "#Delta#beta_{t#bar{t}}", "Events");
        //yuka2d_reco.AddHist("Mtt_costheta", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
        //yuka2d_reco.AddHist("delY_costheta", 1200, -6, 6, 40, -1, 1, "#Deltay(t#bar{t})", "cos#theta");
	yuka2d_reco.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay_{t#bar{t}}");
	//yuka2d_reco.AddHist("newMtt_delY", 1000, 0, 2000, 1200, -6, 6, "new offshell M(t#bar{t})", "offshell #Deltay_{t#bar{t}}");
	//yuka2d_reco.AddHist("Mtt_delBeta", 1000, 0, 2000, 200, 0, 2, "M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	//yuka2d_reco.AddHist("delY_delBeta", 1200, -6, 6, 200, 0, 2, "#Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

        yuka1d_reco.AddHist("tlepy", 200, 0, 5, "|y(t_{l}|", "Events");
        yuka2d_reco.AddHist("Mtt_tlepy", 1000, 0, 2000, 200, 0, 5, "M(t#bar{t})", "|y(t_{l}|");
        yuka2d_reco.AddHist("delY_tlepy", 1200, -6, 6, 200, 0, 5, "#Deltay_{t#bar{t}}", "|y(t_{l}|");
	
        TDirectory* dir_yukawareco_right = outFile_.mkdir("YUKAWA_RECO_right");
	dir_yukawareco_right->cd();
        yuka1d_reco_right.AddHist("Mtt_resol", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        yuka1d_reco_right.AddHist("delY_resol", 80, -4, 4, "#Deltay(t#bar{t}) reco/gen", "Events");
        yuka1d_reco_right.AddHist("MTwl", 500, 0, 1000, "transverse mass W_{T}", "Events");
        yuka1d_reco_right.AddHist("njets", 11, 0, 11, "njets", "Events");
	yuka1d_reco_right.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
	//yuka1d_reco_right.AddHist("costheta", 40, -1, 1, "cos#theta", "Events");
	yuka1d_reco_right.AddHist("Y", 160, -4, 4,"y_t", "Events");
	yuka1d_reco_right.AddHist("delY", 1200, -6, 6,"#Deltay_{t#bar{t}}", "Events");
	//yuka1d_reco_right.AddHist("delBeta", 200, 0, 2, "#Delta#beta_{t#bar{t}}", "Events");
        //yuka2d_reco_right.AddHist("Mtt_costheta", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
        //yuka2d_reco_right.AddHist("delY_costheta", 1200, -6, 6, 40, -1, 1, "#Deltay(t#bar{t})", "cos#theta");
	yuka2d_reco_right.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay_{t#bar{t}}");
	//yuka2d_reco_right.AddHist("Mtt_delBeta", 1000, 0, 2000, 200, 0, 2, "M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	//yuka2d_reco_right.AddHist("delY_delBeta", 1200, -6, 6, 200, 0, 2, "#Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

        yuka1d_reco_right.AddHist("tlepy", 200, 0, 5, "|y(t_{l}|", "Events");
        yuka2d_reco_right.AddHist("Mtt_tlepy", 1000, 0, 2000, 200, 0, 5, "M(t#bar{t})", "|y(t_{l}|");
        yuka2d_reco_right.AddHist("delY_tlepy", 1200, -6, 6, 200, 0, 5, "#Deltay_{t#bar{t}}", "|y(t_{l}|");

	TDirectory* dir_yukawareco_wrong = outFile_.mkdir("YUKAWA_RECO_wrong");
	dir_yukawareco_wrong->cd();
        yuka1d_reco_wrong.AddHist("Mtt_resol", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        yuka1d_reco_wrong.AddHist("delY_resol", 80, -4, 4, "#Deltay(t#bar{t}) reco/gen", "Events");
        yuka1d_reco_wrong.AddHist("MTwl", 500, 0, 1000, "transverse mass W_{T}", "Events");
        yuka1d_reco_wrong.AddHist("njets", 11, 0, 11, "njets", "Events");
	yuka1d_reco_wrong.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
	//yuka1d_reco_wrong.AddHist("costheta", 40, -1, 1, "cos#theta", "Events");
	yuka1d_reco_wrong.AddHist("Y", 160, -4, 4,"y_t", "Events");
	yuka1d_reco_wrong.AddHist("delY", 1200, -6, 6,"#Deltay_{t#bar{t}}", "Events");
	//yuka1d_reco_wrong.AddHist("delBeta", 200, 0, 2, "#Delta#beta_{t#bar{t}}", "Events");
        //yuka2d_reco_wrong.AddHist("Mtt_costheta", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
        //yuka2d_reco_wrong.AddHist("delY_costheta", 1200, -6, 6, 40, -1, 1, "#Deltay(t#bar{t})", "cos#theta");
	yuka2d_reco_wrong.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay_{t#bar{t}}");
	//yuka2d_reco_wrong.AddHist("Mtt_delBeta", 1000, 0, 2000, 200, 0, 2, "M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	//yuka2d_reco_wrong.AddHist("delY_delBeta", 1200, -6, 6, 200, 0, 2, "#Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

        yuka1d_reco_wrong.AddHist("tlepy", 200, 0, 5, "|y(t_{l}|", "Events");
        yuka2d_reco_wrong.AddHist("Mtt_tlepy", 1000, 0, 2000, 200, 0, 5, "M(t#bar{t})", "|y(t_{l}|");
        yuka2d_reco_wrong.AddHist("delY_tlepy", 1200, -6, 6, 200, 0, 5, "#Deltay_{t#bar{t}}", "|y(t_{l}|");

	TDirectory* dir_yukawareco_semi = outFile_.mkdir("YUKAWA_RECO_semi");
	dir_yukawareco_semi->cd();
        yuka1d_reco_semi.AddHist("Mtt_resol", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        yuka1d_reco_semi.AddHist("delY_resol", 80, -4, 4, "#Deltay(t#bar{t}) reco/gen", "Events");
        yuka1d_reco_semi.AddHist("MTwl", 500, 0, 1000, "transverse mass W_{T}", "Events");
        yuka1d_reco_semi.AddHist("njets", 11, 0, 11, "njets", "Events");
	yuka1d_reco_semi.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
	//yuka1d_reco_semi.AddHist("costheta", 40, -1, 1, "cos#theta", "Events");
	yuka1d_reco_semi.AddHist("Y", 160, -4, 4,"y_t", "Events");
	yuka1d_reco_semi.AddHist("delY", 1200, -6, 6,"#Deltay_{t#bar{t}}", "Events");
	//yuka1d_reco_semi.AddHist("delBeta", 200, 0, 2, "#Delta#beta_{t#bar{t}}", "Events");
        //yuka2d_reco_semi.AddHist("Mtt_costheta", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
        //yuka2d_reco_semi.AddHist("delY_costheta", 1200, -6, 6, 40, -1, 1, "#Deltay(t#bar{t})", "cos#theta");
	yuka2d_reco_semi.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay_{t#bar{t}}");
	//yuka2d_reco_semi.AddHist("Mtt_delBeta", 1000, 0, 2000, 200, 0, 2, "M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	//yuka2d_reco_semi.AddHist("delY_delBeta", 1200, -6, 6, 200, 0, 2, "#Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

        yuka1d_reco_semi.AddHist("tlepy", 200, 0, 5, "|y(t_{l}|", "Events");
        yuka2d_reco_semi.AddHist("Mtt_tlepy", 1000, 0, 2000, 200, 0, 5, "M(t#bar{t})", "|y(t_{l}|");
        yuka2d_reco_semi.AddHist("delY_tlepy", 1200, -6, 6, 200, 0, 5, "#Deltay_{t#bar{t}}", "|y(t_{l}|");

	TDirectory* dir_yukawareco_other = outFile_.mkdir("YUKAWA_RECO_other");
	dir_yukawareco_other->cd();
        yuka1d_reco_other.AddHist("Mtt_resol", 40, -2, 2, "M(t#bar{t}) reco/gen", "Events");
        yuka1d_reco_other.AddHist("delY_resol", 80, -4, 4, "#Deltay(t#bar{t}) reco/gen", "Events");
        yuka1d_reco_other.AddHist("MTwl", 500, 0, 1000, "transverse mass W_{T}", "Events");
        yuka1d_reco_other.AddHist("njets", 11, 0, 11, "njets", "Events");
	yuka1d_reco_other.AddHist("Mtt", 1000, 0, 2000, "M(t#bar{t})", "Events");
	//yuka1d_reco_other.AddHist("costheta", 40, -1, 1, "cos#theta", "Events");
	yuka1d_reco_other.AddHist("Y", 160, -4, 4,"y_t", "Events");
	yuka1d_reco_other.AddHist("delY", 1200, -6, 6,"#Deltay_{t#bar{t}}", "Events");
	//yuka1d_reco_other.AddHist("delBeta", 200, 0, 2, "#Delta#beta_{t#bar{t}}", "Events");
        //yuka2d_reco_other.AddHist("Mtt_costheta", 1000, 0, 2000, 40, -1, 1, "M(t#bar{t})", "cos#theta");
        //yuka2d_reco_other.AddHist("delY_costheta", 1200, -6, 6, 40, -1, 1, "#Deltay(t#bar{t})", "cos#theta");
	yuka2d_reco_other.AddHist("Mtt_delY", 1000, 0, 2000, 1200, -6, 6, "M(t#bar{t})", "#Deltay_{t#bar{t}}");
	//yuka2d_reco_other.AddHist("Mtt_delBeta", 1000, 0, 2000, 200, 0, 2, "M(t#bar{t})", "#Delta#beta_{t#bar{t}}");
	//yuka2d_reco_other.AddHist("delY_delBeta", 1200, -6, 6, 200, 0, 2, "#Deltay_{t#bar{t}}", "#Delta#beta_{t#bar{t}}");

        yuka1d_reco_other.AddHist("tlepy", 200, 0, 5, "|y(t_{l}|", "Events");
        yuka2d_reco_other.AddHist("Mtt_tlepy", 1000, 0, 2000, 200, 0, 5, "M(t#bar{t})", "|y(t_{l}|");
        yuka2d_reco_other.AddHist("delY_tlepy", 1200, -6, 6, 200, 0, 5, "#Deltay_{t#bar{t}}", "|y(t_{l}|");


        //jetscaler.Init("Fall15_25nsV2_DATA_UncertaintySources_AK4PFchs.txt", cjecuncertainty);
        //jetscaler.Init("Spring16_25nsV6_DATA_UncertaintySources_AK4PFchs.txt", cjecuncertainty);
        jetscaler.Init(cJetEnergyUncertainty, cjecuncertainty);
	jetscaler.Init(cJetEnergyUncertainty, "FlavorPureBottom");
	jetscaler.Init(cJetEnergyUncertainty, "FlavorQCD");
        //jetscaler.InitResolution("jetresolution.txt", "jetresolutionsf.txt");
        jetscaler.InitResolution(cJetResolution, cJetResolutionSF);
        jetscaler.InitMCrescale(this, "jetrescale.root");

	string probfilename("Prob_parton.root");
	//string probfilename("LH_parton.root");
	if(PSEUDOTOP){probfilename = "Prob_pseudo.root";}
	//if(PSEUDOTOP){probfilename = "LH_pseudo.root";}
	if(BTAGMODE)
	{
		cnbtag = 1;
		ttsolver.Init(PSEUDOTOP, probfilename, false, true, true);//for btag
	}
	if(JETSCALEMODE)
	{
		ttsolver.Init(PSEUDOTOP, probfilename, true, true, false);//don't use mass info
	}
	else
	{
		ttsolver.Init(PSEUDOTOP, probfilename, false, true, true);
		//ttsolver.Init(probfilename, false, true);
	}
	//btagweight.Init(this, "btaggingeff.root", cbtagunc, cltagunc);
        //btagweight.Init(this, "BTag_SF.csv", "btaggingeff.root", cbtagunc, cltagunc);
        btagweight.Init(this, cBTaggingSF, cBTaggingEff, cbtagunc, cltagunc);
	string puhistname("pu_central");
	if(cpileup == -1) puhistname = "pu_minus";
	if(cpileup == 1) puhistname = "pu_plus";

	TFile* f = TFile::Open("PUweight.root");
	puhist = (TH1D*)f->Get(puhistname.c_str());
	TFile* fl = TFile::Open(cLeptonScaleFactor.c_str());
        musfhist = (TH2D*)fl->Get("MuSF");
        elsfhist = (TH2D*)fl->Get("ElSF");
	//musfhist = (TH2D*)fl->Get("MuTOT");
	//elsfhist = (TH2D*)fl->Get("ElTOT");
	//mutrgsfhist = (TH2D*)fl->Get("MuISOTRG");
	//eltrgsfhist = (TH2D*)fl->Get("ElISOTRG");
        
	// this is for yukawa studies:
	//TFile* fyuka_beta = TFile::Open("yukawa2_beta.root");
	//yukahist_beta = (TH1D*)fyuka_beta->Get("XSR_beta");

        //TFile* fyuka_2d = TFile::Open("yukawa_reweighing1.0.root");
        TFile* fyuka_2d = TFile::Open(yukawasf.c_str());
	yukahist_2d = (TH2D*)fyuka_2d->Get("EWtoLO");
	yukahist_2dlo = (TH2D*)fyuka_2d->Get("LO");
	yukahist_2dew = (TH2D*)fyuka_2d->Get("EW");

        //TDirectory* dir = gDirectory;
        TFile* tf = TFile::Open("likelihood3j.root");
        //hlikelihood3j_nsd = dynamic_cast<TGraph*>(tf->Get(("nschi_blright").c_str()));
        //hlikelihood3j_thad = dynamic_cast<TGraph*>(tf->Get(("thad_bhright").c_str()));
        grlikelihood3j_nsd = (TGraph*)tf->Get("nschi_blright");
        grlikelihood3j_thad = (TGraph*)tf->Get("thad_bhright");
        //TGraph* grlikelihood3j_nsd_wrong = (TGraph*)tf->Get("nschi_blwrong");
        //TGraph* grlikelihood3j_thad_wrong = (TGraph*)tf->Get("thad_bhwrong");
        //grlikelihood3j_nsptmin = (TGraph*)tf->Get("nsptmin_bhright");
        //grlikelihood3j_nsetamax = (TGraph*)tf->Get("nsetamax_bhright");
        //likelihood3j_nspteta_2d = (TH2D*)tf->Get("nspteta_right");

	bdecayweights.Init(cbdecay);
	bfragweights.Init("bfragweights.root", cbfrag);
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

//otto's new version:
void ttbar::SelectGenParticles(URStreamer& event)
{
	SEMILEP = false;
	SEMILEPACC = false;
	FULLLEP = false;
	FULLHAD = false;
	const vector<Ttgen>& ttgens = event.TTGens();
	if(ttgens.size() != 8) { return; }
	for(size_t n = 0 ; n < 8 ; ++n) { gps[n] = ttgens[n]; }
	gen1d["TYP"]->Fill(0.5, weight);
	gentq = gps[0];
	gentqbar = gps[4];
	bool tlep = false;
	bool tbarlep = false;
	bool tem = false;
	bool tbarem = false;
	if(gps[2].pdgId() == -11 || gps[2].pdgId() == -13){tem = true;}
	if(gps[7].pdgId() == 11 || gps[7].pdgId() == 13){tbarem = true;}
	if(gps[2].pdgId() == -11 || gps[2].pdgId() == -13 || gps[2].pdgId() == -15){tlep = true;}
	if(gps[7].pdgId() == 11 || gps[7].pdgId() == 13 || gps[7].pdgId() == 15){tbarlep = true;}

	if(tem && !tbarlep)
	{
		SEMILEPACC = true;
		gen1d["TYP"]->Fill(4.5, weight);
		genallper.Init(&gps[6], &gps[7], &gps[5], &gps[1], &gps[2], gps[2].pdgId(), &gps[3]);
		gentqhad = gentqbar;
		gentqlep = gentq;

	}
	else if(!tlep && tbarem)
	{
		SEMILEPACC = true;
		gen1d["TYP"]->Fill(4.5, weight);
		genallper.Init(&gps[2], &gps[3], &gps[1], &gps[5], &gps[7], gps[7].pdgId(), &gps[6]);
		gentqhad = gentq;
		gentqlep = gentqbar;
	}

	if(tlep && tbarlep)
	{
		FULLLEP = true;
		gen1d["TYP"]->Fill(3.5, weight);
	}
	else if(!tlep && !tbarlep)
	{
		FULLHAD = true;
		gen1d["TYP"]->Fill(1.5, weight);
	}
	else 
	{
		SEMILEP = true;
		gen1d["TYP"]->Fill(2.5, weight);
	}

        /*
        //Lhe lhet;
        //Lhe lhetbar;
	const vector<Lhe>& lheparticles = event.lhes();
        //for(const Lhe& lp : lheparticles)
	for(Lhe lp : lheparticles)
        {
            if(lp.pdgid() == 6) lhet = lp;
            if(lp.pdgid() == -6) lhetbar = lp;
        }

        lhe1d["mtt"]->Fill((lhet+lhetbar).Mag(), 1);
        lhe1d["delY"]->Fill(lhet.Rapidity()-lhetbar.Rapidity(), 1);
        lhe2d["Mtt_delY"]->Fill((lhet+lhetbar).Mag(), lhet.Rapidity()-lhetbar.Rapidity(), 1);

        TLorentzVector v1(lhet.Px(), lhet.Py(), lhet.Pz(), sqrt(lhet.P()*lhet.P()+172.5*172.5));
        TLorentzVector v2(lhetbar.Px(), lhetbar.Py(), lhetbar.Pz(), sqrt(lhetbar.P()*lhetbar.P()+172.5*172.5));
        TLorentzVector v3(gent.Px(), gent.Py(), gent.Pz(), sqrt(gent.P()*gent.P()+172.5*172.5));
        TLorentzVector v4(gentbar.Px(), gentbar.Py(), gentbar.Pz(), sqrt(gentbar.P()*gentbar.P()+172.5*172.5));
        double LO = yukahist_2dlo->GetBinContent(yukahist_2dlo->GetXaxis()->FindFixBin((v1+v2).M()), yukahist_2dlo->GetYaxis()->FindFixBin(lhet.Rapidity()-lhetbar.Rapidity()));
        double EW = yukahist_2dew->GetBinContent(yukahist_2dew->GetXaxis()->FindFixBin((v1+v2).M()), yukahist_2dew->GetYaxis()->FindFixBin(lhet.Rapidity()-lhetbar.Rapidity()));
        lhe1d["mtt_constrain"]->Fill((v1+v2).Mag(), 1);
        //lhe1d["delY"]->Fill((lhet.Rapidity()-lhetbar.Rapidity())/LO, weight);
        lhe2d["Mtt_constrain_delY"]->Fill((v1+v2).Mag(), (lhet.Rapidity()-lhetbar.Rapidity()), 1);
        //cout<<"lhe = "<<(lhet+lhetbar).Mag()<<" "<<(v1+v2).Mag()<<", "<<(lhet.Rapidity()-lhetbar.Rapidity())<<endl;
        //cout<<"powheg = "<<(gent+gentbar).Mag()<<" "<<(v3+v4).Mag()<<", "<<(gent.Rapidity()-gentbar.Rapidity())<<endl;
        //cout<<"LO = "<<yukahist_2dlo->GetBinContent(yukahist_2dlo->GetXaxis()->FindFixBin((lhet+lhetbar).Mag()), yukahist_2dlo->GetYaxis()->FindFixBin(lhet.Rapidity()-lhetbar.Rapidity()))<<", "<<LO<<endl;

        //lhe2d["Mtt_delY_LO"]->Fill((lhet+lhetbar).Mag()/LO, (lhet.Rapidity()-lhetbar.Rapidity())/LO, weight);
        */
}

//otto's new version
void ttbar::SelectPseudoTop(URStreamer& event)
{
	const vector<Pl>& pls = event.PLs();
	GenObject* lepton = nullptr;
	int lc = 0;
	TLorentzVector nu;
	vector<TLorentzVector*> pstbjets;
	vector<TLorentzVector*> pstljets;
	for(Pl pl : pls)
	{
		if(abs(pl.pdgId()) == 13 || abs(pl.pdgId()) == 11)
		{
			if(Abs(pl.Eta()) < 2.5 && pl.Pt() > clptmin)
			{
				lc++;
				if(lc == 2) {return;}
			}
			if(Abs(pl.Eta()) < cpletamax && pl.Pt() > cplptmin)
			{
				sgenparticles.push_back(pl);
				lepton = &(sgenparticles.back());
			}
		}

		if(abs(pl.pdgId()) == 12 || abs(pl.pdgId()) == 14 || abs(pl.pdgId()) == 16)
		{
			nu += pl;
		}

		if(abs(pl.pdgId()) < 6)
		{
			if(pl.Pt() > cpbjetptsoft && Abs(pl.Eta()) < cpjetetamax)
			{
				sgenparticles.push_back(pl);
				if(Abs(pl.pdgId()) == 5)
				{
					pstbjets.push_back(&(sgenparticles.back()));	
				}	
				pstljets.push_back(&(sgenparticles.back()));	
			}
		}
	}
	
	if(lepton == nullptr) {return;}
	if(pstbjets.size() < 2){return;}

	TLorentzVector wl(*lepton + nu);
	double chi2min = 1.E100;
	double MW = 80.4;
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
	
}

//otto's new version
void ttbar::AddGenJetSelection(URStreamer& event)
{
	const vector<Pl>& pls = event.PLs();
	//int addbs = 0;	
	//int addcs = 0;	
	double bweights = 1.;
	for(const Pl& gj : pls)
	{
		if(abs(gj.pdgId()) > 5) {continue;}
		sgenparticles.push_back(gj);
		//genalljets.push_back(&(sgenparticles.back()));
		if(gj.pdgId() == 5)
		{
			genbjets.push_back(&(sgenparticles.back()));
			bweights *= bfragweights.Weight(gj);
			bweights *= bdecayweights.Weight(gj);
			//if(genallper.IsComplete() && genallper.DRminTTjets(&gj) > 0.4) {addbs++;}
		}
		else if(gj.pdgId() == 4)
		{
			gencjets.push_back(&(sgenparticles.back()));
			//if(genallper.IsComplete() && genallper.DRminTTjets(&gj) > 0.4) {addcs++;}
		}
		else
		{
			genljets.push_back(&(sgenparticles.back()));
		}
	}
	weight *= bweights;
	for(const Pl& gj : pls)
	{
		if(abs(gj.pdgId()) == 4)
		{
			truth1d["xc"]->Fill(gj.isoR4(), weight);
		}
		else if(abs(gj.pdgId()) == 5)
		{
			truth1d["xb"]->Fill(gj.isoR4(), weight);
			float pdgid = gj.isoR3();
			if(fmod(pdgid,1) != 0.)
			{
				truth1d["bids_lep"]->Fill(abs(pdgid-0.5), weight);
			}
			else
			{
				truth1d["bids_had"]->Fill(abs(pdgid), weight);
			}
		}
	}
	//weight *= pow(cbsplitting, addbs);
	//weight *= pow(ccsplitting, addbs);
}

/*
void ttbar::AddGenJetSelection(URStreamer& event)
{
    const vector<Pl>& pls = event.PLs();
    for(const Pl& gj : pls)
    {
        if(abs(gj.pdgId()) > 5) {continue;}
        if(gj.pdgId() == 5 && gj.Pt() > 25. && abs(gj.Eta()) < 2.5)
        {
            sgenparticles.push_back(gj);
            genbjets.push_back(&(sgenparticles.back()));
        }
        if(gj.pdgId() == 4 && gj.Pt() > 25. && abs(gj.Eta()) < 2.5)
        {
            sgenparticles.push_back(gj);
            gencjets.push_back(&(sgenparticles.back()));
        }
    }
}*/

void ttbar::SelectRecoParticles(URStreamer& event)
{
	const vector<Muon>& muons = event.muons();
	for(vector<Muon>::const_iterator muon = muons.begin(); muon != muons.end(); ++muon)
	{
		IDMuon mu(*muon);
		if(mu.ID(IDMuon::LOOSE_16) && mu.Pt() > 15.)
		{
			smuons.push_back(mu);
			loosemuons.push_back(&(smuons.back()));
			if(MUONS && mu.ID(IDMuon::TIGHT_16) && mu.Pt() > clptmin && Abs(mu.Eta()) < cletamax)
			{
				tightmuons.push_back(&(smuons.back()));
			}
		}
	}

	const vector<Electron>& electrons = event.electrons();
	for(vector<Electron>::const_iterator electron = electrons.begin(); electron != electrons.end(); ++electron)
	{
		IDElectron el(*electron);
		if(el.ID(IDElectron::LOOSE_16) && el.Pt() > 15.)
		{
			selectrons.push_back(el);
			looseelectrons.push_back(&(selectrons.back()));
			if(ELECTRONS && el.ID(IDElectron::TIGHT_16) && el.Pt() > clptmin && Abs(el.Eta()) < cletamax)
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
                if(Abs(jet.Eta()) > cjetetamax) {continue;}
                if(!jet.ID() || !jet.Clean(loosemuons, looseelectrons)) {continue;}
		double sfres = jetscaler.GetRes(jet, Min(event.rho().value(), 30.), cjetres);
		double sf = sfres + jetscaler.GetScale(jet, Min(event.rho().value(), 30.), csigmajet);
                //could do some developes later on Wj, ref:otto's ellipse
                //double sf = jetscaler.GetScale(jet, Min(event.rho().value(), 30.), csigmajet, cjetres);
                metcorrx -= (sf-1)*jet.Px();
                metcorry -= (sf-1)*jet.Py();
                //cout << sf << " " << jet.Pt() << " ";
                jet.SetPxPyPzE(jet.Px()*sf, jet.Py()*sf, jet.Pz()*sf, jet.E()*sf);
                //cout << jet.Pt() << endl;
                if(jet.Pt() < jetptmin) {continue;}

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


    if(TTMC)
    {
        //      for(GenObject* p:genfinalpartons)
        //      {
        //              IDJet* bestjet = nullptr;
        //              for(IDJet* j:cleanedjets)
        //              {
        //                      if(j->DeltaR(*p) < 0.4)
        //                      {
        //                              if(bestjet == nullptr){bestjet = j;}
        //                              else if(j->DeltaR(*p) < bestjet->DeltaR(*p)){bestjet = j;}
        //                      }
        //              }
        //              if(bestjet != nullptr) bestjet->genpt += p->Pt();
        //      }
        for(IDJet* j:cleanedjets)
        {
            if(find_if(genbjets.begin(), genbjets.end(), [&](GenObject* bp){return j->DeltaR(*bp) < 0.3;}) != genbjets.end())
            {
                if(Abs(j->Eta()) < 1.5)
                {
                    //truth2d["jetscale_bB"]->Fill(j->Pt(), (j->Pt() - j->genpt)/j->genpt, weight);
                }
                else
                {
                    //truth2d["jetscale_bE"]->Fill(j->Pt(), (j->Pt() - j->genpt)/j->genpt, weight);
                }
            }
            else
            {
                if(Abs(j->Eta()) < 1.5)
                {
                    //truth2d["jetscale_lB"]->Fill(j->Pt(), (j->Pt() - j->genpt)/j->genpt, weight);
                }
                else
                {
                    //truth2d["jetscale_lE"]->Fill(j->Pt(), (j->Pt() - j->genpt)/j->genpt, weight);
                }
            }
        }
    }






	if(SEMILEPACC)
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
	if(SEMILEPACC && rightper.IsComplete()){ttp_truth.Fill(rightper, weight);}

	if(SEMILEPACC)
	{
		if(Abs(genper->LPDGId()) == 11) {truth2d["Ne_Nmu"]->Fill(mediumelectrons.size()+0.5, tightmuons.size()+0.5, weight);}
		if(Abs(genper->LPDGId()) == 13) {truth2d["Nmu_Ne"]->Fill(tightmuons.size()+0.5, mediumelectrons.size()+0.5, weight);}
	}

        if(cleanedjets.size() < 3){return;}

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
	if(lep == 0){return;}
	reco1d["c_lep"]->Fill(event.run+0.5);

	//double nvtx = event.vertexs().size();
	//reco1d["NumVertices"]->Fill(nvtx , mcweight);

	if(isDA == 0)
	{
		if(tightmuons.size() == 1)
		{
			weight *= musfhist->GetBinContent(musfhist->GetXaxis()->FindFixBin(lep->Eta()), musfhist->GetYaxis()->FindFixBin(Min(lep->Pt(), 170.)));
		}
		if(mediumelectrons.size() == 1)
		{
			weight *= elsfhist->GetBinContent(elsfhist->GetXaxis()->FindFixBin(lep->Eta()), elsfhist->GetYaxis()->FindFixBin(Min(lep->Pt(), 170.)));
                        int l1ptmax = event.trigger().El27ptmax();
                        if(l1ptmax != -1 && l1ptmax < 34)
                        {
                            weight*=cel27eff[(l1ptmax-24)/2];
                        }
		}

	}
	//reco1d["NumVerticesWeighted"]->Fill(nvtx , weight);
	
        //keeping only the n leading jets. 
	sort(cleanedjets.begin(), cleanedjets.end(), [](IDJet* A, IDJet* B){return(A->Pt() > B->Pt());});
	int reducedsize = Min(cleanedjets.size(), cnusedjets);
	reducedjets.resize(reducedsize);
	copy(cleanedjets.begin(), cleanedjets.begin()+reducedsize, reducedjets.begin());
      
  
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
	if(SEMILEPACC)
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
	if(cleanedjets.size() < njetsmin || cleanedjets.size() > njetsmax){return;}
	reco1d["c_jets"]->Fill(event.run+0.5);
	if(BTAGMODE && cleanedjets.size() < 4){return;}
	reco1d["counter"]->Fill(2.5, weight);
	if(SEMILEPACC) truth1d["counter"]->Fill(4.5, weight);
	if(tightmuons.size() == 1)
	{
		reco1d["MuIsolation"]->Fill(tightmuons[0]->PFIsoDb()/tightmuons[0]->Pt() , weight);
	}
	else
	{
		reco1d["ElIsolation"]->Fill(mediumelectrons[0]->CorPFIsolation(), weight);
	}
	double Mt_W = Sqrt(2.*met.Pt()*lep->Pt()-2.*(met.Px()*lep->Px() + met.Py()*lep->Py()));
	reco1d["Mt_W"]->Fill(Mt_W, weight);
	//calculating btag eff.
	sort(reducedjets.begin(), reducedjets.end(), [](IDJet* A, IDJet* B){return(A->csvIncl() > B->csvIncl());});
	int nbjets = count_if(reducedjets.begin(), reducedjets.end(), [&](IDJet* A){return(A->csvIncl() > B_MEDIUM);});
	reco1d["bjetmulti"]->Fill(nbjets, weight);
	if(isDA == 0 && !BTAGMODE)
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
            if(cleanedjets.size() == 3)
                //{if(reducedjets[0]->csvIncl() < B_MEDIUM || reducedjets[1]->csvIncl() < B_MEDIUM){return;}} // add for 3j 
                {if(reducedjets[0]->csvIncl() > 0.6 || reducedjets[1]->csvIncl() > 0.6){return;}}// for bck templates 
            else
                //{if(reducedjets[0]->csvIncl() < B_MEDIUM || reducedjets[1]->csvIncl() < B_MEDIUM){return;}} 
                {if(reducedjets[0]->csvIncl() > 0.6 || reducedjets[1]->csvIncl() > 0.6){return;}} //for bck templates
                //{if(reducedjets[0]->csvIncl() < B_MEDIUM || reducedjets[1]->csvIncl() < B_LOOSE){return;}}
	}
	double nvtx = event.vertexs().size();
	reco1d["NumVertices"]->Fill(nvtx , mcweight);
	reco1d["NumVerticesWeighted"]->Fill(nvtx , weight);
	reco1d["c_btag"]->Fill(event.run+0.5);
	reco1d["counter"]->Fill(3.5, weight);
	if(SEMILEPACC) truth1d["counter"]->Fill(5.5, weight);

// add for 3j
        //double MTwl = Sqrt(pow((*lep+met).Mag(), 2) + pow((*lep+met).Px(), 2) + pow((*lep+met).Py(), 2));
        double MTwl = Sqrt(2* (lep->Perp() * met.Perp() - lep->Px() * met.Px() - lep->Py() * met.Py()));
        if(MTwl >140.) return;

    if(cleanedjets.size() == 3){

        TLorentzVector * bleper;
        TLorentzVector * bhadper;
        TLorentzVector * wbleper;
        TLorentzVector * wbhadper;
        double chi2candidate1, chi2candidate2;
        double chi2lep, chi2had;
        double wchi2lep, wchi2had;
        bool NS1sol = false;

        NeutrinoSolver NS_3ja = NeutrinoSolver(lep, reducedjets[0], 80., 173.);//remove the last arg for neutrino mass=0 and set it in NeutrinoSolver.cc, even setting 0 here still caused inf problems
        TLorentzVector meta = NS_3ja.GetBest(met.X(), met.Y(), 1, 1, 0, chi2candidate1);
        NeutrinoSolver NS_3jb = NeutrinoSolver(lep, reducedjets[1], 80., 173.);
        TLorentzVector metb = NS_3jb.GetBest(met.X(), met.Y(), 1, 1, 0, chi2candidate2);
        TLorentzVector metsolver;
        TLorentzVector wmetsolver;

        
        reco3j1d["counter_chi2"]->Fill(0.5, weight);

        if(chi2candidate1>=0 && chi2candidate2>=0) reco3j1d["counter_chi2"]->Fill(1.5, weight);
        else if(chi2candidate2<0 && chi2candidate1>=0)   reco3j1d["counter_chi2"]->Fill(2.5, weight);
        else if(chi2candidate1<0 && chi2candidate2>=0)   reco3j1d["counter_chi2"]->Fill(3.5, weight);
        else {reco3j1d["counter_chi2"]->Fill(4.5, weight); return;}
        
        if(chi2candidate1/Sqrt(Abs(chi2candidate1)) > 500 || chi2candidate2/Sqrt(Abs(chi2candidate2)) > 500) return;
        if((*reducedjets[0]+*reducedjets[2]).Mag() > 1000 || (*reducedjets[1]+*reducedjets[2]).Mag() > 1000) return;
       

        //b jet permutation
          if(chi2candidate1>=0 && chi2candidate2>=0){
            if(chi2candidate1 <= chi2candidate2){
                bleper = reducedjets[0];
                bhadper = reducedjets[1];
                metsolver = meta;
                chi2lep = chi2candidate1;
                chi2had = chi2candidate2;
            }else{
                bleper = reducedjets[1];
                bhadper = reducedjets[0];
                metsolver = metb;
                chi2lep = chi2candidate2;
                chi2had = chi2candidate1;
            }
          }
          else if(chi2candidate1>=0 && chi2candidate2<0){
              bleper = reducedjets[0];
              bhadper = reducedjets[1];
              metsolver = meta;
              chi2lep = chi2candidate1;
              chi2had = chi2candidate2;
              NS1sol = true;
          }
          else if(chi2candidate2>=0 && chi2candidate1<0){
                bleper = reducedjets[1];
                bhadper = reducedjets[0];
                metsolver = metb;
                chi2lep = chi2candidate2;
                chi2had = chi2candidate1;
                NS1sol = true;
          }

        TLorentzVector tlep_3j = *bleper + *lep + metsolver;
        TLorentzVector thad_3j = *bhadper + *reducedjets[2];
        TLorentzVector thadwrong_3j = *bleper + *reducedjets[2];
        TLorentzVector *wj1 = reducedjets[2];
        wj1->SetPx(reducedjets[2]->Px());
        wj1->SetPy(reducedjets[2]->Py());
        wj1->SetPz(reducedjets[2]->Pz());
        wj1->SetE(Sqrt(reducedjets[2]->Px()*reducedjets[2]->Px() + reducedjets[2]->Py()*reducedjets[2]->Py() + reducedjets[2]->Pz()*reducedjets[2]->Pz()));
        

        yuka1d_reco["MTwl"]->Fill(MTwl, weight);
        yuka1d_reco["njets"]->Fill(3, weight);

        //if(thad_3j.Mag()>200.) return;//instead of cut off, use to build up a likelihood distribution
        
        double Logtlike3j;
        double Logblike3j1; //= 1000; 
        double Logblike3j2;
        double Logblike3jr1;
        double Logblike3jr2;
        double Logblike3j;
        bool bjetsflip = false;

        //Logtlike3j = -1*Log(grlikelihood3j_mtwl->Eval(MTwl));
        //Logtlike3j -= Log(grlikelihood3j_met->Eval(met));
        if(!NS1sol){
            Logblike3j1 = -1*Log(grlikelihood3j_nsd->Eval((chi2lep/Sqrt(Abs(chi2lep)))));
            Logblike3j1 -= Log(grlikelihood3j_thad->Eval(thad_3j.Mag()));
            
            Logblike3j2 = -1*Log(grlikelihood3j_nsd->Eval(chi2had/Sqrt(chi2had)));
            Logblike3j2 -= Log(grlikelihood3j_thad->Eval(thadwrong_3j.Mag()));
            
            
            Logblike3jr1 = -1*Log(grlikelihood3j_nsd->Eval((chi2candidate1/Sqrt(Abs(chi2candidate1)))));
            Logblike3jr1 -= Log(grlikelihood3j_thad->Eval((*reducedjets[1]+*reducedjets[2]).Mag()));
            
            Logblike3jr2 = -1*Log(grlikelihood3j_nsd->Eval(chi2candidate2/Sqrt(chi2candidate2)));
            Logblike3jr2 -= Log(grlikelihood3j_thad->Eval((*reducedjets[0]+*reducedjets[2]).Mag()));
        }//yiting

        //for probability read in 
        //like in 4+ jets: TRUTH/truth_nschi_right TRUTH/truth_nschi_wrong
        if(!NS1sol){
            if(rightper.BLep() == reducedjets[0]) reco3j1d["nschi_right"]->Fill(chi2candidate1/Sqrt(Abs(chi2candidate1)), weight); 
            if(rightper.BLep() == reducedjets[1]) reco3j1d["nschi_right"]->Fill(chi2candidate2/Sqrt(Abs(chi2candidate2)), weight);
            if(rightper.BLep() != reducedjets[0]) reco3j1d["nschi_wrong"]->Fill(chi2candidate1/Sqrt(Abs(chi2candidate1)), weight);
            if(rightper.BLep() != reducedjets[1]) reco3j1d["nschi_wrong"]->Fill(chi2candidate2/Sqrt(Abs(chi2candidate2)), weight);

            if(rightper.BLep() == reducedjets[0]) reco3j1d["thadM_right"]->Fill((*reducedjets[1]+*reducedjets[2]).Mag(), weight); 
            if(rightper.BLep() == reducedjets[1]) reco3j1d["thadM_right"]->Fill((*reducedjets[0]+*reducedjets[2]).Mag(), weight);
            if(rightper.BLep() != reducedjets[0]) reco3j1d["thadM_wrong"]->Fill((*reducedjets[1]+*reducedjets[2]).Mag(), weight);
            if(rightper.BLep() != reducedjets[1]) reco3j1d["thadM_wrong"]->Fill((*reducedjets[0]+*reducedjets[2]).Mag(), weight);

            if(rightper.BLep() == reducedjets[0]) reco3j2d["nschi_thadM_right"]->Fill(chi2candidate1/Sqrt(Abs(chi2candidate1)), (*reducedjets[1]+*reducedjets[2]).Mag(), weight);
            if(rightper.BLep() == reducedjets[1]) reco3j2d["nschi_thadM_right"]->Fill(chi2candidate2/Sqrt(Abs(chi2candidate2)), (*reducedjets[0]+*reducedjets[2]).Mag(), weight);
            if(rightper.BLep() != reducedjets[0]) reco3j2d["nschi_thadM_wrong"]->Fill(chi2candidate1/Sqrt(Abs(chi2candidate1)), (*reducedjets[1]+*reducedjets[2]).Mag(), weight);
            if(rightper.BLep() != reducedjets[1]) reco3j2d["nschi_thadM_wrong"]->Fill(chi2candidate2/Sqrt(Abs(chi2candidate2)), (*reducedjets[0]+*reducedjets[2]).Mag(), weight);
        
            //combined likelihood
            if(rightper.BLep() == reducedjets[0]) reco3j1d["likelihood_right"]->Fill(Logblike3jr1, weight);
            if(rightper.BLep() == reducedjets[1]) reco3j1d["likelihood_right"]->Fill(Logblike3jr2, weight);
            if(rightper.BLep() != reducedjets[0]) reco3j1d["likelihood_wrong"]->Fill(Logblike3jr1, weight);
            if(rightper.BLep() != reducedjets[1]) reco3j1d["likelihood_wrong"]->Fill(Logblike3jr2, weight);
        }
        
    /*if(rightper.IsComplete3Ja() || rightper.IsComplete3Jb()){

    }else if(SEMILEP){
        reco3j1d["nschi_semi"]->Fill(chi2lep/Sqrt(Abs(chi2lep)), weight);
        reco3j1d["thad_M_semi"]->Fill(thad_3j.Mag(), weight);
        reco3j1d["likelihood_semi"]->Fill(Logblike3j1, weight);
    }else{
        reco3j1d["nschi_other"]->Fill(chi2lep/Sqrt(Abs(chi2lep)), weight);
        reco3j1d["thad_M_other"]->Fill(thad_3j.Mag(), weight);
        reco3j1d["likelihood_other"]->Fill(Logblike3j1, weight);
    }*/


    //counters - check the NS on b algorithm efficiency
    /*reco3j1d["counter2"]->Fill(0.5, weight);
    if(rightper.IsComplete3Ja() || rightper.IsComplete3Jb()){
        
        reco3j1d["counter"]->Fill(0.5, weight);
        reco3j1d["counter2"]->Fill(1.5, weight);

        if(rightper.BLep() == reducedjets[0]){ 
            reco3j1d["counter"]->Fill(1.5, weight); 
            if(rightper.BLep() == bleper) reco3j1d["counter"]->Fill(11.5, weight); 
            if(rightper.BLep() == bhadper) reco3j1d["counter"]->Fill(21.5, weight);
        }
        if(rightper.BLep() == reducedjets[1]){ 
            reco3j1d["counter"]->Fill(2.5, weight); 
            if(rightper.BLep() == bleper) reco3j1d["counter"]->Fill(12.5, weight); 
            if(rightper.BLep() == bhadper) reco3j1d["counter"]->Fill(22.5, weight);
        }
        if(rightper.BLep() == reducedjets[2]){
            reco3j1d["counter"]->Fill(3.5, weight);
            if(rightper.BLep() == bleper) reco3j1d["counter"]->Fill(13.5, weight);
            if(rightper.BLep() == bhadper) reco3j1d["counter"]->Fill(23.5, weight);
        }
        if(rightper.BHad() == reducedjets[0]){
            reco3j1d["counter"]->Fill(4.5, weight);
            if(rightper.BLep() == bleper) reco3j1d["counter"]->Fill(14.5, weight);
            if(rightper.BLep() == bhadper) reco3j1d["counter"]->Fill(24.5, weight);
        }
        if(rightper.BHad() == reducedjets[1]){
            reco3j1d["counter"]->Fill(5.5, weight);
            if(rightper.BLep() == bleper) reco3j1d["counter"]->Fill(15.5, weight);
            if(rightper.BLep() == bhadper) reco3j1d["counter"]->Fill(25.5, weight);
        }
        if(rightper.BHad() == reducedjets[2]){ 
            reco3j1d["counter"]->Fill(6.5, weight);
            if(rightper.BLep() == bleper) reco3j1d["counter"]->Fill(16.5, weight);
            if(rightper.BLep() == bhadper) reco3j1d["counter"]->Fill(26.5, weight);
        }

        if(rightper.BLep() == reducedjets[0] && rightper.BHad() == reducedjets[1]){ 
            reco3j1d["counter"]->Fill(7.5, weight);
            if(rightper.BLep() == bleper) reco3j1d["counter"]->Fill(17.5, weight); 
            if(rightper.BLep() == bhadper) reco3j1d["counter"]->Fill(27.5, weight);
        }else if(rightper.BLep() == reducedjets[1] && rightper.BHad() == reducedjets[0]){ 
            reco3j1d["counter"]->Fill(8.5, weight);
            if(rightper.BLep() == bleper) reco3j1d["counter"]->Fill(18.5, weight);
            if(rightper.BLep() == bhadper) reco3j1d["counter"]->Fill(28.5, weight);
        }else  reco3j1d["counter"]->Fill(9.5, weight);

        if(rightper.BLep() == bleper)   reco3j1d["counter2"]->Fill(2.5, weight);
        if(rightper.BLep() == bhadper)  reco3j1d["counter2"]->Fill(3.5, weight);
        if(rightper.BLep() == bleper && rightper.BHad() == bhadper)   reco3j1d["counter2"]->Fill(4.5, weight);
    }*/
    

    if((rightper.IsComplete3Ja() || rightper.IsComplete3Jb())){
        
/*        double chi21, chi22, chi23;
        NeutrinoSolver NS_3j1 = NeutrinoSolver(lep, rightper.BLep(), 80., 173.);
        TLorentzVector met1 = NS_3j1.GetBest(met.X(), met.Y(), 1, 1, 0, chi21);
        NeutrinoSolver NS_3j2 = NeutrinoSolver(lep, rightper.BHad(), 80., 173.);
        TLorentzVector met2 = NS_3j2.GetBest(met.X(), met.Y(), 1, 1, 0, chi22);
        
        TLorentzVector * rightperwja;
        if(rightper.WJa() != 0 && rightper.WJb() == 0) rightperwja = rightper.WJa();
        else if(rightper.WJb() != 0 && rightper.WJa() == 0) rightperwja = rightper.WJb();
        NeutrinoSolver NS_3j3 = NeutrinoSolver(lep, rightperwja, 80., 173.);
        TLorentzVector met3 = NS_3j3.GetBest(met.X(), met.Y(), 1, 1, 0, chi23);

        //if((rightper.BLep() == reducedjets[0] && rightper.BHad() == reducedjets[1])||(rightper.BHad() == reducedjets[0] && rightper.BLep() == reducedjets[1])){
        reco3j2d["nschi12"]->Fill(chi21/Sqrt(Abs(chi21)), chi22/Sqrt(Abs(chi22)), weight);
        reco3j2d["nschi13"]->Fill(chi21/Sqrt(Abs(chi21)), chi23/Sqrt(Abs(chi23)), weight);
        reco3j2d["nschi23"]->Fill(chi22/Sqrt(Abs(chi22)), chi23/Sqrt(Abs(chi23)), weight);
            if(chi21/Sqrt(Abs(chi21)) >0 && chi22/Sqrt(Abs(chi22)) >0){
                //cout<<chi21/Sqrt(Abs(chi21))<<", "<<chi22/Sqrt(Abs(chi22))<<", "<<chi23/Sqrt(Abs(chi23))<<endl;
                //if((chi21/Sqrt(Abs(chi21)) -chi22/Sqrt(Abs(chi22)) )< 0) {cout<<"right_sol2"<<<<endl;
                //reco3j2d["nschi1p2_nschi1_right"]->Fill(chi21/Sqrt(Abs(chi21)) -chi22/Sqrt(Abs(chi22)), chi21/Sqrt(Abs(chi21)), weight);}
                //else {cout<<"wrong_sol2"<<chi21/Sqrt(Abs(chi21))<<", "<<chi22/sqrt(Abs(chi22))<<endl; 
                //reco3j2d["nschi1p2_nschi1_wrong"]->Fill(chi21/Sqrt(Abs(chi21)) -chi22/Sqrt(Abs(chi22)), chi21/Sqrt(Abs(chi21)), weight);}
            }else if(chi21/Sqrt(Abs(chi21)) >0){
                //cout<<"right_sol1"<<endl;
                //reco3j2d["nschi1p2_nschi1_right"]->Fill(chi21/Sqrt(Abs(chi21)) -chi22/Sqrt(Abs(chi22)), chi21/Sqrt(Abs(chi21)), weight);
            }else if(chi22/Sqrt(Abs(chi22)) >0){
                //cout<<"wrong_sol1"<<endl;
                //reco3j2d["nschi1p2_nschi1_wrong"]->Fill(chi21/Sqrt(Abs(chi21)) -chi22/Sqrt(Abs(chi22)), chi21/Sqrt(Abs(chi21)), weight);
            }else{
                //cout<<"no sol"<<endl;
            }
        //}else{
                //cout<<"rightperb are not the hightest 2 btaged"<<endl;
        //}          
*/

        //thsis is the eff if two highest b-tagged jets are bjets: ~94%
        //if((rightper.BLep() == reducedjets[0] && rightper.BHad() == reducedjets[1]) || (rightper.BLep() == reducedjets[1] && rightper.BHad() == reducedjets[0])) cout<<"bs pick right"<<endl;
        //else cout<<"bs pick wrong"<<endl;

        //this is tt correctly reco eff using NS: ~72%
        //if(rightper.BLep() == bleper && rightper.BHad() == bhadper) cout<<"Dv bs right"<<endl;
        //else cout<<"Dv bs wrong"<<endl;


/*        if(grlikelihood3j_thad->Eval(thad_3j.Mag())>=grlikelihood3j_thad->Eval(thadwrong_3j.Mag())){
            //cout<<"keep bs"<<endl;
        }else{
            if(bleper == reducedjets[0] && bhadper == reducedjets[1]) {bleper = reducedjets[1]; bhadper = reducedjets[0];}//cout<<"flip 1"<<endl;}
            else if(bleper == reducedjets[1] && bhadper ==reducedjets[0]) {bleper = reducedjets[0]; bhadper = reducedjets[1];}//cout<<"flip 2"<<endl;}
            else cout<<"not only flip, Mh"<<endl;
        }
*/        
        //this is b right eff using Mh: ~70%
/*        if(rightper.BLep() == bleper && rightper.BHad() == bhadper)
            cout<<"Mh right"<<endl;
        else 
            cout<<"Mh wrong"<<endl;
*/

    }

    //if(!NS1sol && (Logblike3jr1 > 12 || Logblike3jr2 >12)) return; //make tt correctly reco eff to ~75%
    //if(!NS1sol && min(Logblike3j1, Logblike3j2) >14) return; //almost no effect


        /*if(rightper.IsComplete3Ja() || rightper.IsComplete3Jb()){
            if(NS1sol){
                if(rightper.BLep() == bleper && rightper.BHad() == bhadper) cout<<"1sol right"<<endl; //reco3j1d["counter"]->Fill(weight);
                else cout<<"1sol wrong"<<endl; //reco3j1d["counter"]->Fill(weight);
            }else{
                if(rightper.BLep() == bleper && rightper.BHad() == bhadper) cout<<"2sol 72right"<<endl;
                else cout<<"2sol 72wrong"<<endl;
            }
        }*/
                
        if(!NS1sol){
            if(Logblike3j1 <= Logblike3j2){
            }else{
                bjetsflip = true;
                if(bleper == reducedjets[0] && bhadper == reducedjets[1]) 
                    {bleper = reducedjets[1]; bhadper = reducedjets[0]; metsolver = metb; chi2lep = chi2candidate2; chi2had = chi2candidate1;}
                else if(bleper == reducedjets[1] && bhadper ==reducedjets[0]) 
                    {bleper = reducedjets[0]; bhadper = reducedjets[1]; metsolver = meta; chi2lep = chi2candidate1; chi2had = chi2candidate2;}
                else cout<<"not only flip"<<endl;
            }
        }
        if(NS1sol) Logblike3j = Logblike3j1;
        else Logblike3j = min(Logblike3j1, Logblike3j2);
        //Logblike3j = Logblike3j1;
        if(bjetsflip){
            tlep_3j = *bleper + *lep + metsolver;
            thad_3j = *bhadper + *reducedjets[2];
            thadwrong_3j = *bleper + *reducedjets[2];
        }
        //yiting


        //this is tt correctly reco eff using NS + Mh: ~80%
        /*if(rightper.IsComplete3Ja() || rightper.IsComplete3Jb()){
            if(!NS1sol){
                if(rightper.BLep() == bleper && rightper.BHad() == bhadper) cout<<"2sol 80right"<<endl; //reco3j1d["counter"]->Fill(weight);
                else cout<<"2sol 80wrong"<<endl; //reco3j1d["counter"]->Fill(weight);
            }
        }*/

        //basic variables
        reco3j2d["blepnschi_bhadnschi"]->Fill(chi2lep/Sqrt(Abs(chi2lep)), chi2had/Sqrt(Abs(chi2had)), weight);
        reco3j2d["blept_bhadpt"]->Fill(bleper->Pt(), bhadper->Pt(), weight);
        reco3j2d["blepy_bhady"]->Fill(Abs(bleper->Rapidity()), Abs(bhadper->Rapidity()), weight);

        reco3j1d["tlep_pt"]->Fill(tlep_3j.Pt(), weight);
        reco3j1d["thad_pt"]->Fill(thad_3j.Pt(), weight);
        reco3j1d["tlep_y"]->Fill(Abs(tlep_3j.Rapidity()), weight);
        reco3j1d["thad_y"]->Fill(Abs(thad_3j.Rapidity()), weight);
        reco3j1d["tt_pt"]->Fill((tlep_3j + thad_3j).Pt(), weight);
        reco3j1d["tt_y"]->Fill(Abs((tlep_3j + thad_3j).Rapidity()), weight);
        reco3j2d["thad_M_Whad_M"]->Fill(thad_3j.Mag(), reducedjets[2]->Mag(), weight);

        reco3j1d["Mtt"]->Fill((tlep_3j + thad_3j).Mag(), weight);
        reco3j1d["delY"]->Fill(tlep_3j.Rapidity()-thad_3j.Rapidity(), weight);
        //cout<<"lhe = "<<(lhet+lhetbar).Mag()<<", "<<(lhet.Rapidity()-lhetbar.Rapidity())<<endl;
        //cout<<"gen powheg = "<<(gentq+gentqbar).M()<<", "<<gentqlep.Rapidity()-gentqhad.Rapidity()<<endl;
        //cout<<"reco powheg = "<<(tlep_3j + thad_3j).Mag()<<", "<<tlep_3j.Rapidity()-thad_3j.Rapidity()<<endl<<endl;
        reco3j2d["Mtt_delY"]->Fill((tlep_3j + thad_3j).Mag(), tlep_3j.Rapidity()-thad_3j.Rapidity(), weight);
        reco3j1d["Mtt_resol"]->Fill(((tlep_3j + thad_3j).Mag() - (gentqhad+gentqlep).Mag())/(gentqhad + gentqlep).Mag(), weight);
        reco3j1d["delY_resol"]->Fill(((tlep_3j.Rapidity() - thad_3j.Rapidity()) - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);

        reco3j1d["met_pz"]->Fill(metsolver.Pz(), weight);
        reco3j1d["MET"]->Fill(met.Pt(), weight);
        reco3j1d["lep_eta"]->Fill(lep->Eta(), weight);
        reco3j2d["met_pt_pz"]->Fill(metsolver.Pt(), metsolver.Pz(), weight);
        reco3j1d["met_dpz_pz"]->Fill((metsolver.Pz()-genper->Nu().Pz())/genper->Nu().Pz(), weight);
        reco3j1d["met_dpt_pt"]->Fill((metsolver.Pt()-genper->Nu().Pt())/genper->Nu().Pt(), weight);
        reco3j2d["met_dpt_pxpy"]->Fill((metsolver.Px()-met.Px())/met.Px(), (metsolver.Py()-met.Py())/met.Py(), weight);
        reco3j2d["met_dpt_dpz"]->Fill((metsolver.Pt()-genper->Nu().Pt())/genper->Nu().Pt(), (metsolver.Pz()-genper->Nu().Pz())/genper->Nu().Pz(), weight);



    //save histograms in part of right/wrong/semi/others
    if(rightper.BLep() == bleper && rightper.BHad() == bhadper && (rightper.WJa() == reducedjets[2] || rightper.WJb() == reducedjets[2]) && (rightper.IsComplete3Ja() || rightper.IsComplete3Jb())){  
        if(!NS1sol){
        if(rightper.BLep() == reducedjets[0]) right3j1d["likelihood"]->Fill(Logblike3jr1, weight);
        if(rightper.BLep() == reducedjets[1]) right3j1d["likelihood"]->Fill(Logblike3jr2, weight);}
        reco3j1d["counter2"]->Fill(6.5, weight);
        right3j2d["blepnschi_bhadnschi"]->Fill(chi2lep/Sqrt(Abs(chi2lep)), chi2had/Sqrt(Abs(chi2had)), weight);
        right3j2d["blept_bhadpt"]->Fill(bleper->Pt(), bhadper->Pt(), weight);
        right3j2d["blepy_bhady"]->Fill(Abs(bleper->Rapidity()), Abs(bhadper->Rapidity()), weight);

        right3j1d["tlep_pt"]->Fill(tlep_3j.Pt(), weight);
        right3j1d["thad_pt"]->Fill(thad_3j.Pt(), weight);
        right3j1d["tlep_y"]->Fill(Abs(tlep_3j.Rapidity()), weight);
        right3j1d["thad_y"]->Fill(Abs(thad_3j.Rapidity()), weight);
        right3j1d["thad_M"]->Fill(thad_3j.Mag(), weight);
        right3j1d["tt_pt"]->Fill((tlep_3j + thad_3j).Pt(), weight);
        right3j1d["tt_y"]->Fill(Abs((tlep_3j + thad_3j).Rapidity()), weight);
        right3j1d["Mtt"]->Fill((tlep_3j + thad_3j).Mag(), weight);
        right3j1d["delY"]->Fill(tlep_3j.Rapidity()-thad_3j.Rapidity(), weight);
        right3j2d["Mtt_delY"]->Fill((tlep_3j + thad_3j).Mag(), tlep_3j.Rapidity()-thad_3j.Rapidity(), weight);
        right3j1d["Mtt_resol"]->Fill(((tlep_3j + thad_3j).Mag() - (gentqhad+gentqlep).Mag())/(gentqhad + gentqlep).Mag(), weight);
        right3j1d["delY_resol"]->Fill(((tlep_3j.Rapidity() - thad_3j.Rapidity()) - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);
        right3j1d["shuffledlikelihood"]->Fill(Logblike3j, weight);
        right3j1d["nschi"]->Fill(chi2lep/Sqrt(Abs(chi2lep)), weight);
            /*if(rightper.BLep() == reducedjets[0] && rightper.BHad() == reducedjets[1]){  
                right3j1d["nschi"]->Fill(chi2candidate1/Sqrt(Abs(chi2candidate1)), weight); 
            }
            else if(rightper.BLep() == reducedjets[1] && rightper.BHad() == reducedjets[0]){
                right3j1d["nschi"]->Fill(chi2candidate2/Sqrt(Abs(chi2candidate2)), weight);
            }*/

        right3j1d["met_pz"]->Fill(metsolver.Pz(), weight);
        right3j1d["MET"]->Fill(met.Pt(), weight);
        right3j1d["lep_eta"]->Fill(lep->Eta(), weight);
        reco3j2d["met_pt_pz"]->Fill(metsolver.Pt(), metsolver.Pz(), weight);
        right3j2d["MTwl_MET"]->Fill(MTwl, met.Pt(), weight);
        yuka1d_reco_right["MTwl"]->Fill(MTwl, weight);
        yuka1d_reco_right["njets"]->Fill(3, weight);

    }else if(rightper.IsComplete3Ja() || rightper.IsComplete3Jb()){
        if(!NS1sol){
        if(rightper.BLep() != reducedjets[0]) wrong3j1d["likelihood"]->Fill(Logblike3jr1, weight);
        if(rightper.BLep() != reducedjets[1]) wrong3j1d["likelihood"]->Fill(Logblike3jr2, weight);}
        reco3j1d["counter2"]->Fill(7.5, weight);
        wrong3j2d["blepnschi_bhadnschi"]->Fill(chi2lep/Sqrt(Abs(chi2lep)), chi2had/Sqrt(Abs(chi2had)), weight);
        wrong3j2d["blept_bhadpt"]->Fill(bleper->Pt(), bhadper->Pt(), weight);
        wrong3j2d["blepy_bhady"]->Fill(Abs(bleper->Rapidity()), Abs(bhadper->Rapidity()), weight);

        wrong3j1d["tlep_pt"]->Fill(tlep_3j.Pt(), weight);
        wrong3j1d["thad_pt"]->Fill(thad_3j.Pt(), weight);
        wrong3j1d["tlep_y"]->Fill(Abs(tlep_3j.Rapidity()), weight);
        wrong3j1d["thad_y"]->Fill(Abs(thad_3j.Rapidity()), weight);
        wrong3j1d["thad_M"]->Fill(thad_3j.Mag(), weight);
        wrong3j1d["tt_pt"]->Fill((tlep_3j + thad_3j).Pt(), weight);
        wrong3j1d["tt_y"]->Fill(Abs((tlep_3j + thad_3j).Rapidity()), weight);
        wrong3j1d["Mtt"]->Fill((tlep_3j + thad_3j).Mag(), weight);
        wrong3j1d["delY"]->Fill(tlep_3j.Rapidity()-thad_3j.Rapidity(), weight);
        wrong3j2d["Mtt_delY"]->Fill((tlep_3j + thad_3j).Mag(), tlep_3j.Rapidity()-thad_3j.Rapidity(), weight);
        wrong3j1d["Mtt_resol"]->Fill(((tlep_3j + thad_3j).Mag() - (gentqhad+gentqlep).Mag())/(gentqhad + gentqlep).Mag(), weight);
        wrong3j1d["delY_resol"]->Fill(((tlep_3j.Rapidity() - thad_3j.Rapidity()) - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);
        wrong3j1d["shuffledlikelihood"]->Fill(Logblike3j, weight);
        wrong3j1d["nschi"]->Fill(chi2lep/Sqrt(Abs(chi2lep)), weight);
            /*if(rightper.BLep() == reducedjets[0] && rightper.BHad() == reducedjets[1]){  
                wrong3j1d["nschi"]->Fill(chi2candidate1/Sqrt(Abs(chi2candidate1)), weight); 
            }
            else if(rightper.BLep() == reducedjets[1] && rightper.BHad() == reducedjets[0]){
                wrong3j1d["nschi"]->Fill(chi2candidate2/Sqrt(Abs(chi2candidate2)), weight);
            }*/

        wrong3j1d["met_pz"]->Fill(metsolver.Pz(), weight);
        wrong3j1d["MET"]->Fill(met.Pt(), weight);
        wrong3j1d["lep_eta"]->Fill(lep->Eta(), weight);
        reco3j2d["met_pt_pz"]->Fill(metsolver.Pt(), metsolver.Pz(), weight);
        wrong3j2d["MTwl_MET"]->Fill(MTwl, met.Pt(), weight);
        yuka1d_reco_wrong["MTwl"]->Fill(MTwl, weight);
        yuka1d_reco_wrong["njets"]->Fill(3, weight);

          if(rightper.BLep() == bleper){//10% events in wrong meanes bl correct + bh/Wj flips, dely resol no shift 
            wrong3j1d["Mtt_resol_blright"]->Fill(((tlep_3j + thad_3j).Mag() - (gentqhad+gentqlep).Mag())/(gentqhad + gentqlep).Mag(), weight);
            wrong3j1d["delY_resol_blright"]->Fill(((tlep_3j.Rapidity() - thad_3j.Rapidity()) - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);
            wrong3j1d["nschi_rightbl"]->Fill(chi2lep/Sqrt(Abs(chi2lep)), weight);
            wrong3j1d["thad_M_wrongbh"]->Fill(thad_3j.Mag(), weight);
          }else if(rightper.BHad() == bhadper){//5% evernts in wrong means bh correct + bl/Wj flips, dely resol shift ~-1, 
            wrong3j1d["Mtt_resol_bhright"]->Fill(((tlep_3j + thad_3j).Mag() - (gentqhad+gentqlep).Mag())/(gentqhad + gentqlep).Mag(), weight);
            wrong3j1d["delY_resol_bhright"]->Fill(((tlep_3j.Rapidity() - thad_3j.Rapidity()) - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);
            wrong3j1d["nschi_wrongbl"]->Fill(chi2lep/Sqrt(Abs(chi2lep)), weight);
            wrong3j1d["thad_M_rightbh"]->Fill(thad_3j.Mag(), weight);
          }else{//85% events in wrong means Wj correct + both bs flips, dely resol shift ~-1
            wrong3j1d["Mtt_resol_bswrong"]->Fill(((tlep_3j + thad_3j).Mag() - (gentqhad+gentqlep).Mag())/(gentqhad + gentqlep).Mag(), weight);
            wrong3j1d["delY_resol_bswrong"]->Fill(((tlep_3j.Rapidity() - thad_3j.Rapidity()) - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);
            wrong3j1d["nschi_wrongbs"]->Fill(chi2lep/Sqrt(Abs(chi2lep)), weight);
            wrong3j1d["thad_M_wrongbs"]->Fill(thad_3j.Mag(), weight);
          }

    }

    else if(SEMILEPACC){
        if(!NS1sol){
        if(rightper.BLep() != reducedjets[0]) semi3j1d["likelihood"]->Fill(Logblike3jr1, weight);
        if(rightper.BLep() != reducedjets[1]) semi3j1d["likelihood"]->Fill(Logblike3jr2, weight);}
        reco3j1d["counter2"]->Fill(8.5, weight);
        semi3j2d["blepnschi_bhadnschi"]->Fill(chi2lep/Sqrt(Abs(chi2lep)), chi2had/Sqrt(Abs(chi2had)), weight);
        semi3j2d["blept_bhadpt"]->Fill(bleper->Pt(), bhadper->Pt(), weight);
        semi3j2d["blepy_bhady"]->Fill(Abs(bleper->Rapidity()), Abs(bhadper->Rapidity()), weight);

        semi3j1d["tlep_pt"]->Fill(tlep_3j.Pt(), weight);
        semi3j1d["thad_pt"]->Fill(thad_3j.Pt(), weight);
        semi3j1d["tlep_y"]->Fill(Abs(tlep_3j.Rapidity()), weight);
        semi3j1d["thad_y"]->Fill(Abs(thad_3j.Rapidity()), weight);
        semi3j1d["thad_M"]->Fill(thad_3j.Mag(), weight);
        semi3j1d["tt_pt"]->Fill((tlep_3j + thad_3j).Pt(), weight);
        semi3j1d["tt_y"]->Fill(Abs((tlep_3j + thad_3j).Rapidity()), weight);
        semi3j1d["Mtt"]->Fill((tlep_3j + thad_3j).Mag(), weight);
        semi3j1d["delY"]->Fill(tlep_3j.Rapidity()-thad_3j.Rapidity(), weight);
        semi3j2d["Mtt_delY"]->Fill((tlep_3j + thad_3j).Mag(), tlep_3j.Rapidity()-thad_3j.Rapidity(), weight);
        semi3j1d["Mtt_resol"]->Fill(((tlep_3j + thad_3j).Mag() - (gentqhad+gentqlep).Mag())/(gentqhad + gentqlep).Mag(), weight);
        semi3j1d["delY_resol"]->Fill(((tlep_3j.Rapidity() - thad_3j.Rapidity()) - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);
        semi3j1d["shuffledlikelihood"]->Fill(Logblike3j, weight);
        semi3j1d["nschi"]->Fill(chi2lep/Sqrt(Abs(chi2lep)), weight);

        semi3j1d["met_pz"]->Fill(metsolver.Pz(), weight);
        semi3j1d["MET"]->Fill(met.Pt(), weight);
        semi3j1d["lep_eta"]->Fill(lep->Eta(), weight);
        reco3j2d["met_pt_pz"]->Fill(metsolver.Pt(), metsolver.Pz(), weight);
        semi3j2d["MTwl_MET"]->Fill(MTwl, met.Pt(), weight);
        yuka1d_reco_semi["MTwl"]->Fill(MTwl, weight);
        yuka1d_reco_semi["njets"]->Fill(3, weight);
        //cout<<"semiNS1sol="<<NS1sol<<", bjetsflip="<<bjetsflip<<endl;

    }else{
        if(!NS1sol){
        if(rightper.BLep() != reducedjets[0]) other3j1d["likelihood"]->Fill(Logblike3jr1, weight);
        if(rightper.BLep() != reducedjets[1]) other3j1d["likelihood"]->Fill(Logblike3jr2, weight);}
        reco3j1d["counter2"]->Fill(9.5, weight);
        other3j2d["blepnschi_bhadnschi"]->Fill(chi2lep/Sqrt(Abs(chi2lep)), chi2had/Sqrt(Abs(chi2had)), weight);
        other3j2d["blept_bhadpt"]->Fill(bleper->Pt(), bhadper->Pt(), weight);
        other3j2d["blepy_bhady"]->Fill(Abs(bleper->Rapidity()), Abs(bhadper->Rapidity()), weight);

        other3j1d["tlep_pt"]->Fill(tlep_3j.Pt(), weight);
        other3j1d["thad_pt"]->Fill(thad_3j.Pt(), weight);
        other3j1d["tlep_y"]->Fill(Abs(tlep_3j.Rapidity()), weight);
        other3j1d["thad_y"]->Fill(Abs(thad_3j.Rapidity()), weight);
        other3j1d["thad_M"]->Fill(thad_3j.Mag(), weight);
        other3j1d["tt_pt"]->Fill((tlep_3j + thad_3j).Pt(), weight);
        other3j1d["tt_y"]->Fill(Abs((tlep_3j + thad_3j).Rapidity()), weight);
        other3j1d["Mtt"]->Fill((tlep_3j + thad_3j).Mag(), weight);
        other3j1d["delY"]->Fill(tlep_3j.Rapidity()-thad_3j.Rapidity(), weight);
        other3j2d["Mtt_delY"]->Fill((tlep_3j + thad_3j).Mag(), tlep_3j.Rapidity()-thad_3j.Rapidity(), weight);
        other3j1d["Mtt_resol"]->Fill(((tlep_3j + thad_3j).Mag() - (gentqhad+gentqlep).Mag())/(gentqhad + gentqlep).Mag(), weight);
        other3j1d["delY_resol"]->Fill(((tlep_3j.Rapidity() - thad_3j.Rapidity()) - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);
        other3j1d["shuffledlikelihood"]->Fill(Logblike3j, weight);
        other3j1d["nschi"]->Fill(chi2lep/Sqrt(Abs(chi2lep)), weight);
    
        other3j1d["met_pz"]->Fill(metsolver.Pz(), weight);
        other3j1d["MET"]->Fill(met.Pt(), weight);
        other3j1d["lep_eta"]->Fill(lep->Eta(), weight);
        reco3j2d["met_pt_pz"]->Fill(metsolver.Pt(), metsolver.Pz(), weight);
        other3j2d["MTwl_MET"]->Fill(MTwl, met.Pt(), weight);
        yuka1d_reco_other["MTwl"]->Fill(MTwl, weight);
        yuka1d_reco_other["njets"]->Fill(3, weight);
        //cout<<"otherNS1sol="<<NS1sol<<", bjetsflip="<<bjetsflip<<endl;
    }



        //method 1: scale factor alpha on TLorentzVector 2j
        /*
    if(rightper.BLep() == bleper && (rightper.IsComplete3Ja() || rightper.IsComplete3Jb())){ 

        double alphap, alpham;
        //double mttpeak = ((tlep_3j + thad_3j).Mag() + 22.3221)/0.947628;
        double mttpeak = (gentqlep + gentqhad).Mag()*0.947628 - 22.3221;//mttpeak-> y, y(x) = -22.3221 + 0.947628x (know gen info, get reco peak info, it's mttpaek, solve alpha)
        alphap = -2*(tlep_3j*thad_3j) + Sqrt(4*pow(tlep_3j*thad_3j,2)-4*pow(thad_3j.Mag(),2)*(pow(tlep_3j.Mag(),2)-pow(mttpeak, 2)));
        alpham = -2*(tlep_3j*thad_3j) - Sqrt(4*pow(tlep_3j*thad_3j,2)-4*pow(thad_3j.Mag(),2)*(pow(tlep_3j.Mag(),2)-pow(mttpeak, 2)));
        alphap = alphap/(2*pow(thad_3j.Mag(),2));
        alpham = alpham/(2*pow(thad_3j.Mag(),2));
        //cout <<alphap <<", "<<alpham <<endl;
        alpha3j2d["genmtt_recomtt"]->Fill((gentqhad + gentqlep).Mag(), (tlep_3j + thad_3j).Mag(), weight);
        alpha3j2d["mp_alphamp"]->Fill(thad_3j.Mag(), alphap*(thad_3j.Mag()), weight);
        alpha3j2d["mp_alpha"]->Fill(thad_3j.Mag(), alphap, weight);

        //y = 125.846 + (x-139.401)* exp(-0.0115772x)
        //y = -118.127 -1.43278x +37.7185*sqrt(x)
        //double alpha = (125.846 + (thad_3j.Mag() - 139.401)* exp(-0.0115772*thad_3j.Mag()))/thad_3j.Mag();
        double alpha = (-118.127 -1.43278*thad_3j.Mag() + 37.7185*Sqrt(thad_3j.Mag()))/thad_3j.Mag();
        //cout << thad_3j.Mag() <<", "<< alpha <<" = "<< alpha*thad_3j.Mag()<< endl;
        alpha3j1d["thad_pt"]->Fill((alpha*thad_3j).Pt(), weight);
        alpha3j1d["thad_y"]->Fill(Abs((alpha*thad_3j).Rapidity()), weight);
        alpha3j1d["thad_M"]->Fill((alpha*thad_3j).Mag(), weight);
        alpha3j1d["tt_pt"]->Fill((tlep_3j + (alpha*thad_3j)).Pt(), weight);
        alpha3j1d["tt_y"]->Fill(Abs((tlep_3j + (alpha*thad_3j)).Rapidity()), weight);
        alpha3j1d["Mtt"]->Fill((tlep_3j + (alpha*thad_3j)).Mag(), weight);
        alpha3j1d["delY"]->Fill(tlep_3j.Rapidity()-(alpha*thad_3j).Rapidity(), weight);
        alpha3j2d["Mtt_delY"]->Fill((tlep_3j + (alpha*thad_3j)).Mag(), tlep_3j.Rapidity()-(alpha*thad_3j).Rapidity(), weight);
        alpha3j1d["Mtt_resol"]->Fill(((tlep_3j + alpha*thad_3j).Mag() - (gentqhad + gentqlep).Mag())/(gentqhad + gentqlep).Mag(), weight);
        alpha3j1d["Mtt_resol_noalpha"]->Fill(((tlep_3j + thad_3j).Mag() - (gentqhad + gentqlep).Mag())/(gentqhad + gentqlep).Mag(), weight);
        alpha3j1d["delY_resol"]->Fill(((tlep_3j.Rapidity() - alpha*thad_3j.Rapidity()) - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);
        alpha3j1d["delY_resol_noalpha"]->Fill(((tlep_3j.Rapidity() - thad_3j.Rapidity()) - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);
    }//end of method 1
    */
    
    
    
        //method 2: NS applies on missj 
        //(after allocate the permutation b, reconstruct TLorentzVector nu and assign the TLorentzVector for b) 
        //double chi2missj;
        //TLorentzVector missj;
        double nsp1, nsp2;
        double nspt1, nspt2;
        double nspz1, nspz2;
        double nseta1, nseta2;
        double min_nspt1 = -1., min_nsptr1 = -1.;
        double min_nspt2 = -1., min_nsptr2 = -1.;
        double max_nseta1 = -1., max_nsetar1 = -1.;
        double max_nseta2 = -1., max_nsetar2 = -1.;
        bool outNSconstraint = false;
        

        //NS mass constraint
        //if((*reducedjets[0]+*reducedjets[2]).Mag()>=173 || (*reducedjets[1]+*reducedjets[2]).Mag()>=173) outNSconstraint = true;//use this one
        
        //if(thad_3j.Mag()>=173 || thadwrong_3j.Mag()>=173) outNSconstraint = true;
        //if((*wj1+*bhadper).Mag()>173.) return; //this is the mass constrain 
            
        
        /*if(!outNSconstraint && !NS1sol){
            NeutrinoSolver NS_missj1(wj1, bhadper, 80., 173.); 
            NeutrinoSolver NS_missj2(wj1, bleper, 80., 173.); 
            NeutrinoSolver NS_missjr1(wj1, reducedjets[0], 80., 173.); 
            NeutrinoSolver NS_missjr2(wj1, reducedjets[1], 80., 173.); 

            //NeutrinoSolver NS_missj = NeutrinoSolver(reducedjets[2], bhadper, 80., 173., genper->WJa()->Mag()); 
            //missj = NS_missj.GetBest(genper->WJb()->Px(), genper->WJb()->Py(), 1, 1, 0, chi2missj);
            //temp turn off: missj = NS_missj.GetBest((NS_missj.GetSolution(0.).Px() + NS_missj.GetSolution(3.1415927).Px())/2, (NS_missj.GetSolution(0.).Py() + NS_missj.GetSolution(3.1415927).Py())/2, 1, 1, 0, chi2missj);
     
            missj1d["counter"]->Fill(0.5, weight);

            min_nspt1 = NS_missj1.GetSolution(0).Pt();
            max_nseta1 = Abs(NS_missj1.GetSolution(0).Eta());
            min_nspt2 = NS_missj2.GetSolution(0).Pt();
            max_nseta2 = Abs(NS_missj2.GetSolution(0).Eta());
            int acc3j = 0;

            min_nsptr1 = NS_missjr1.GetSolution(0).Pt();
            max_nsetar1 = Abs(NS_missjr1.GetSolution(0).Eta());
            min_nsptr2 = NS_missjr2.GetSolution(0).Pt();
            max_nsetar2 = Abs(NS_missjr2.GetSolution(0).Eta());
            
            for(int angle = 0; angle<360; angle = angle + 1){
                double rad = (angle*2*3.1415927)/360;
                
                if(NS_missj1.GetSolution(rad).Pt() != NS_missj1.GetSolution(rad).Pt()) return;
                if(NS_missj2.GetSolution(rad).Pt() != NS_missj2.GetSolution(rad).Pt()) return;
                if(NS_missj1.GetSolution(rad).Pt() < min_nspt1) min_nspt1 = NS_missj1.GetSolution(rad).Pt();
                if(NS_missj2.GetSolution(rad).Pt() < min_nspt2) min_nspt2 = NS_missj2.GetSolution(rad).Pt();
                if(Abs(NS_missj1.GetSolution(rad).Eta()) > max_nseta1) max_nseta1 = Abs(NS_missj1.GetSolution(rad).Eta());
                if(Abs(NS_missj2.GetSolution(rad).Eta()) > max_nseta2) max_nseta2 = Abs(NS_missj2.GetSolution(rad).Eta());
                
                
                if(NS_missjr1.GetSolution(rad).Pt() != NS_missjr1.GetSolution(rad).Pt()) return;
                if(NS_missjr2.GetSolution(rad).Pt() != NS_missjr2.GetSolution(rad).Pt()) return;
                if(NS_missjr1.GetSolution(rad).Pt() < min_nsptr1) min_nsptr1 = NS_missjr1.GetSolution(rad).Pt();
                if(NS_missjr2.GetSolution(rad).Pt() < min_nsptr2) min_nsptr2 = NS_missjr2.GetSolution(rad).Pt();
                if(Abs(NS_missjr1.GetSolution(rad).Eta()) > max_nsetar1) max_nsetar1 = Abs(NS_missjr1.GetSolution(rad).Eta());
                if(Abs(NS_missjr2.GetSolution(rad).Eta()) > max_nsetar2) max_nsetar2 = Abs(NS_missjr2.GetSolution(rad).Eta());

            }

        }//end of outNSconstraint
        */


        //if(min_nspt1 > 500 || min_nspt2 > 500) return;
        //if(max_nseta1 > 10 || max_nseta2 > 10) return;

       // cout<<"before: "<<Logblike3j1<<", "<<Logblike3j2<<endl;
        /*if(!outNSconstraint && !NS1sol){
            //Logblike3j1 -= Log(grlikelihood3j_nsptmin->Eval(min_nspt1));
            //Logblike3j1 -= Log(grlikelihood3j_nsetamax->Eval(max_nseta1));
            Logblike3j1 -= Log(likelihood3j_nspteta_2d->GetBinContent(likelihood3j_nspteta_2d->GetXaxis()->FindFixBin(min_nspt1), likelihood3j_nspteta_2d->GetYaxis()->FindFixBin(max_nseta1)));

            //Logblike3j2 -= Log(grlikelihood3j_nsptmin->Eval(min_nspt2));
            //Logblike3j2 -= Log(grlikelihood3j_nsetamax->Eval(max_nseta2));
            Logblike3j2 -= Log(likelihood3j_nspteta_2d->GetBinContent(likelihood3j_nspteta_2d->GetXaxis()->FindFixBin(min_nspt2), likelihood3j_nspteta_2d->GetYaxis()->FindFixBin(max_nseta2)));
        }*/
        //cout<<"after: "<<Logblike3j1<<", "<<Logblike3j2<<endl;
        
/*        if(rightper.IsComplete3Ja() || rightper.IsComplete3Jb()){
        //this is b right eff using NS + Mh: ~81%
        if(rightper.BLep() == bleper && rightper.BHad() == bhadper)
            cout<<"initial right"<<endl;
        else 
            cout<<"initial wrong"<<endl;
        }
*/
        
        //if(!outNSconstraint && !NS1sol){
/*        if(!NS1sol){
            //cout<<"outNSconstraint = "<<outNSconstraint<<", NS0solhad = "<<NS0solhad<<", NS1solhad = "<<NS1solhad<<endl;
            if(Logblike3j1 <= Logblike3j2){
                //cout<<"keep bs"<<endl;
            }else{
                //bjetsflip = true;
                if(bleper == reducedjets[0] && bhadper == reducedjets[1]) {bleper = reducedjets[1]; bhadper = reducedjets[0];}//cout<<"flip 1"<<endl;}
                else if(bleper == reducedjets[1] && bhadper ==reducedjets[0]) {bleper = reducedjets[0]; bhadper = reducedjets[1];}//cout<<"flip 2"<<endl;}
                else cout<<"not only flip, combined"<<endl;
            }
        }
*/

/*    if(rightper.IsComplete3Ja() || rightper.IsComplete3Jb()){
        //this is b right eff using NS + Mh: ~81%
        if(rightper.BLep() == bleper && rightper.BHad() == bhadper)
            cout<<"combine right"<<endl;
        else 
            cout<<"combine wrong"<<endl;
        }
*/

/*        if(rightper.IsComplete3Ja() || rightper.IsComplete3Jb()){
            cout<<"NS1sol ="<<NS1sol<<", outNSconstraint = "<<outNSconstraint<<endl;//", NS0solhad = "<<NS0solhad<<", NS1solhad = "<<NS1solhad<<endl; //always has 2 sol if it's in constraint

        if(NS1sol){
            if(rightper.BLep() == bleper && rightper.BHad() == bhadper)
            {reco3j1d["counter_chi2"]->Fill(8.5, weight); cout<<"1sol right"<<endl;}
            else
            {reco3j1d["counter_chi2"]->Fill(9.5, weight); cout<<"1sol wrong"<<endl;}
        }
        if(outNSconstraint){
            if(rightper.BLep() == bleper && rightper.BHad() == bhadper) cout<<"outNSconstraint right"<<endl;
            else cout<<"outNSconstraint wrong"<<endl;
        }else{
            if(rightper.BLep() == bleper && rightper.BHad() == bhadper) cout<<"NSconstraint right"<<endl;
            else cout<<"NSconstraint wrong"<<endl;
        }

        if(rightper.BLep() == bleper && rightper.BHad() == bhadper)
            cout<<"combine4 right"<<endl<<endl;
        else 
            cout<<"combine4 wrong"<<endl<<endl;

        }
*/

        //for probability read in 
        /*if(!outNSconstraint && !NS1sol){
            if(rightper.BHad() == reducedjets[0]) missj1d["min_nspt_right"]->Fill(min_nsptr1, weight);
            if(rightper.BHad() == reducedjets[1]) missj1d["min_nspt_right"]->Fill(min_nsptr2, weight);
            if(rightper.BHad() != reducedjets[0]) missj1d["min_nspt_wrong"]->Fill(min_nsptr1, weight);
            if(rightper.BHad() != reducedjets[1]) missj1d["min_nspt_wrong"]->Fill(min_nsptr2, weight);

            if(rightper.BHad() == reducedjets[0]) missj1d["max_nseta_right"]->Fill(max_nsetar1, weight);
            if(rightper.BHad() == reducedjets[1]) missj1d["max_nseta_right"]->Fill(max_nsetar2, weight);
            if(rightper.BHad() != reducedjets[0]) missj1d["max_nseta_wrong"]->Fill(max_nsetar1, weight);
            if(rightper.BHad() != reducedjets[1]) missj1d["max_nseta_wrong"]->Fill(max_nsetar2, weight);

            //check corrBHadion nspt
            if(rightper.BHad() == reducedjets[0]) missj2d["nspt_nseta_right"]->Fill(min_nsptr1, max_nsetar1, weight);
            if(rightper.BHad() == reducedjets[1]) missj2d["nspt_nseta_right"]->Fill(min_nsptr2, max_nsetar2, weight);
            if(rightper.BHad() != reducedjets[0]) missj2d["nspt_nseta_wrong"]->Fill(min_nsptr1, max_nsetar1, weight);
            if(rightper.BHad() != reducedjets[1]) missj2d["nspt_nseta_wrong"]->Fill(min_nsptr2, max_nsetar2, weight);

            if(rightper.BHad() == reducedjets[0]) missj2d["nspt_thadM_right"]->Fill(min_nsptr1, (*reducedjets[1]+*reducedjets[2]).Mag(), weight);
            if(rightper.BHad() == reducedjets[1]) missj2d["nspt_thadM_right"]->Fill(min_nsptr2, (*reducedjets[0]+*reducedjets[2]).Mag(), weight);
            if(rightper.BHad() != reducedjets[0]) missj2d["nspt_thadM_wrong"]->Fill(min_nsptr1, (*reducedjets[1]+*reducedjets[2]).Mag(), weight);
            if(rightper.BHad() != reducedjets[1]) missj2d["nspt_thadM_wrong"]->Fill(min_nsptr2, (*reducedjets[0]+*reducedjets[2]).Mag(), weight);

            if(rightper.BHad() == reducedjets[0]) missj2d["nspt_nschi_right"]->Fill(min_nsptr1, chi2candidate1/Sqrt(Abs(chi2candidate1)), weight);
            if(rightper.BHad() == reducedjets[1]) missj2d["nspt_nschi_right"]->Fill(min_nsptr2, chi2candidate2/Sqrt(Abs(chi2candidate2)), weight);
            if(rightper.BHad() != reducedjets[0]) missj2d["nspt_nschi_wrong"]->Fill(min_nsptr1, chi2candidate1/Sqrt(Abs(chi2candidate1)), weight);
            if(rightper.BHad() != reducedjets[1]) missj2d["nspt_nschi_wrong"]->Fill(min_nsptr2, chi2candidate2/Sqrt(Abs(chi2candidate2)), weight);

            //check corrBHadon nseta
            if(rightper.BHad() == reducedjets[0]) missj2d["nseta_thadM_right"]->Fill(max_nsetar1, (*reducedjets[1]+*reducedjets[2]).Mag(), weight);
            if(rightper.BHad() == reducedjets[1]) missj2d["nseta_thadM_right"]->Fill(max_nsetar2, (*reducedjets[0]+*reducedjets[2]).Mag(), weight);
            if(rightper.BHad() != reducedjets[0]) missj2d["nseta_thadM_wrong"]->Fill(max_nsetar1, (*reducedjets[1]+*reducedjets[2]).Mag(), weight);
            if(rightper.BHad() != reducedjets[1]) missj2d["nseta_thadM_wrong"]->Fill(max_nsetar2, (*reducedjets[0]+*reducedjets[2]).Mag(), weight);

            if(rightper.BHad() == reducedjets[0]) missj2d["nseta_nschi_right"]->Fill(max_nsetar1, chi2candidate1/Sqrt(Abs(chi2candidate1)), weight);
            if(rightper.BHad() == reducedjets[1]) missj2d["nseta_nschi_right"]->Fill(max_nsetar2, chi2candidate2/Sqrt(Abs(chi2candidate2)), weight);
            if(rightper.BHad() != reducedjets[0]) missj2d["nseta_nschi_wrong"]->Fill(max_nsetar1, chi2candidate1/Sqrt(Abs(chi2candidate1)), weight);
            if(rightper.BHad() != reducedjets[1]) missj2d["nseta_nschi_wrong"]->Fill(max_nsetar2, chi2candidate2/Sqrt(Abs(chi2candidate2)), weight);
        }*/

        /*if((rightper.IsComplete3Ja() || rightper.IsComplete3Jb()) && !outNSconstraint){  
            //if(acc3j>0) missj1d["acceff2"]->Fill(acc3j, weight);
            //missj1d["acceff"]->Fill(acc3j, weight);

            //if(rightper.BHad() == bhadper && rightper.BLep() == bleper){
            if(rightper.BHad() == bhadper){
                missj1d["min_nspt_bhright"]->Fill(min_nspt1, weight);
                missj1d["max_nseta_bhright"]->Fill(max_nseta1, weight);
                //if(acc3j>0) missj1d["acceff2_bhright"]->Fill(acc3j, weight);
                //missj1d["acceff_bhright"]->Fill(acc3j, weight);
            }else{
                missj1d["min_nspt_bhwrong"]->Fill(min_nspt1, weight);
                missj1d["max_nseta_bhwrong"]->Fill(max_nseta1, weight);
                //if(acc3j>0) missj1d["acceff2_bhwrong"]->Fill(acc3j, weight);
                //missj1d["acceff_bhwrong"]->Fill(acc3j, weight);
            }
        }*/


        /*if(rightper.BLep() == bleper && rightper.BHad() == bhadper && (rightper.WJa() == reducedjets[2] || rightper.WJb() == reducedjets[2]) && (rightper.IsComplete3Ja() || rightper.IsComplete3Jb())){  
            missj1d["min_nspt_right"]->Fill(min_nspt, weight);
            missj1d["max_nseta_right"]->Fill(max_nseta, weight);
            if(acc3j>0) missj1d["acceff2_right"]->Fill(acc3j, weight);
            missj1d["acceff_right"]->Fill(acc3j, weight);
        
           // missj1d["likelihood2_right"]->Fill(Logblike3j, weight);

        }else if(rightper.IsComplete3Ja() || rightper.IsComplete3Jb()){ 
            missj1d["min_nspt_wrong"]->Fill(min_nspt, weight);
            missj1d["max_nseta_wrong"]->Fill(max_nseta, weight);
            if(acc3j>0) missj1d["acceff2_wrong"]->Fill(acc3j, weight);
            missj1d["acceff_wrong"]->Fill(acc3j, weight);

            //missj1d["likelihood2_wrong"]->Fill(Logblike3j, weight);

        }else if(SEMILEP){
            missj1d["min_nspt_semi"]->Fill(min_nspt, weight);
            missj1d["max_nseta_semi"]->Fill(max_nseta, weight);
            if(acc3j>0) missj1d["acceff2_semi"]->Fill(acc3j, weight);
            missj1d["acceff_semi"]->Fill(acc3j, weight);

            //missj1d["likelihood2_semi"]->Fill(Logblike3j, weight);

        }else{
            missj1d["min_nspt_other"]->Fill(min_nspt, weight);
            missj1d["max_nseta_other"]->Fill(max_nseta, weight);
            if(acc3j>0) missj1d["acceff2_other"]->Fill(acc3j, weight);
            missj1d["acceff_other"]->Fill(acc3j, weight);
            
            //missj1d["likelihood2_other"]->Fill(Logblike3j, weight);
        }*/
            

/*            if(acc3j == 0){
                missj1d["counter"]->Fill(2.5, weight);
                if(chi2candidate1 >=0 && chi2candidate2 >=0)  missj1d["counter"]->Fill(3.5, weight); 
                else if(chi2candidate1 <0 && chi2candidate2 >=0)  missj1d["counter"]->Fill(4.5, weight);
                else if(chi2candidate1 >=0 && chi2candidate2 <0)  missj1d["counter"]->Fill(5.5, weight); 
            }
    */
            /*if(acc3j == 0){
                for(int angle = 0; angle<360; angle = angle + 1){
                    double rad = (angle*2*3.1415927)/360;
                        nspt = NS_missj.GetSolution(rad).Pt();
                        nspz = NS_missj.GetSolution(rad).Pz();
                        nsp = NS_missj.GetSolution(rad).P();
                        nseta = Abs(NS_missj.GetSolution(rad).Eta());
                        cout<<"rad = "<<rad<<", nspt = "<<nspt<<", nseta = "<<nseta<<", wj1:"<<wj1->Px()<<", "<<wj1->Py()<<", "<<wj1->Pz()<<", "<<wj1->Mag()<<", bhadper:"<<bhadper->Px()<<", "<<bhadper->Py()<<", "<<bhadper->Pz()<<", "<<bhadper->Mag()<<", wj1+bhadper:"<<(*wj1 + *bhadper).Px()<<", "<<(*wj1 + *bhadper).Py()<<", "<<(*wj1 + *bhadper).Pz()<<", "<<(*wj1+ *bhadper).Mag()<<endl;
                }
            }*/
            //if(acc3j <0) cout<<"negative acc3j:"<<NS_missj.GetSolution(0).Pt()<<endl;
            //end of r's another idea
        

        //TLorentzVector thadsolver = *bhadper + *reducedjets[2] + missj;
        //if(chi2missj <0) return;//{reco3j1d["counter_chi2"]->Fill(7.5, weight); return;}
        //missj1d["miss_chi2"]->Fill(chi2missj, weight);


        //some properities for the reconstructed missj
        /*missj1d["missj_pz"]->Fill(missj.Pz(), weight);
        missj1d["missj_pt"]->Fill(missj.Pt(), weight);
        missj1d["missj_eta"]->Fill(Abs(missj.Eta()), weight);
        if(rightper.WJa() != 0){
            missj1d["missj_dpz_pz"]->Fill((missj.Pz()-genper->WJb()->Pz())/genper->WJb()->Pz(), weight);
            missj1d["missj_dpt_pt"]->Fill((missj.Pt()-genper->WJb()->Pt())/genper->WJb()->Pt(), weight);}
        else if(rightper.WJb() != 0){
            missj1d["missj_dpz_pz"]->Fill((missj.Pz()-genper->WJa()->Pz())/genper->WJa()->Pz(), weight);
            missj1d["missj_dpt_pt"]->Fill((missj.Pt()-genper->WJa()->Pt())/genper->WJa()->Pt(), weight);
        }
        missj1d["thad_pt"]->Fill((*bhadper+*wj1+missj).Pt(), weight);
        missj1d["thad_y"]->Fill((*bhadper+*wj1+missj).Rapidity(), weight);
        missj1d["whad_M"]->Fill((*wj1+missj).Mag(), weight);
        missj1d["thad_M"]->Fill((*bhadper+*wj1+missj).Mag(), weight);
        missj2d["Mtt_delY"]->Fill((*bhadper+*wj1+missj + tlep_3j).Mag(), tlep_3j.Rapidity() - (*bhadper+*wj1+missj).Rapidity(), weight);
        missj1d["Mtt_resol"]->Fill(((*bhadper+*wj1+missj + tlep_3j).Mag() - (gentqhad + gentqlep).Mag())/(gentqhad + gentqlep).Mag(), weight);
        missj1d["delY_resol"]->Fill(((tlep_3j.Rapidity() - (*bhadper+*wj1+missj).Rapidity()) - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);
*/
        /*missj1d["thad_pt"]->Fill((*bhadper+*reducedjets[2]+missj).Pt(), weight);
        missj1d["delY_resol"]->Fill(((tlep_3j.Rapidity() - (*bhadper+*reducedjets[2]+missj).Rapidity()) - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);*/
    //}//end of method 2


        //some GEN properities of the missj
        if(rightper.WJa() != 0 && rightper.WJb() == 0){
            gen3j1d["thadmiss_pz"]->Fill(genper->WJb()->Pz(), weight); //genper: GENJET, genallper: GEN parton
            gen3j1d["thadmiss_pt"]->Fill(genper->WJb()->Pt(), weight); 
            //gen3j1d["thadmiss_pt"]->Fill(genallper.WJb()->Pt(), weight);
            gen3j1d["thadmiss_eta"]->Fill(Abs(genper->WJb()->Eta()), weight);
            gen3j2d["thadmiss_pt_eta"]->Fill(genper->WJb()->Pt(), Abs(genper->WJb()->Eta()), weight);
            gen3j1d["thadmiss_DeltaR"]->Fill(genper->WJb()->DeltaR(*genper->WJa()), weight);
            gen3j1d["thadmiss_DeltaR2"]->Fill(genper->WJb()->DeltaR(*genper->BHad()), weight);
            gen3j1d["thadmiss_DeltaR3"]->Fill(genper->WJb()->DeltaR(*genper->BLep()), weight);
        }else if(rightper.WJb() != 0 && rightper.WJa() == 0){ 
            gen3j1d["thadmiss_pz"]->Fill(genper->WJa()->Pz(), weight);
            gen3j1d["thadmiss_pt"]->Fill(genper->WJa()->Pt(), weight);
            //gen3j1d["thadmiss_pt"]->Fill(genallper.WJa()->Pt(), weight);
            gen3j1d["thadmiss_eta"]->Fill(Abs(genper->WJa()->Eta()), weight);
            gen3j2d["thadmiss_pt_eta"]->Fill(genper->WJa()->Pt(), Abs(genper->WJa()->Eta()), weight);
            gen3j1d["thadmiss_DeltaR"]->Fill(genper->WJa()->DeltaR(*genper->WJb()), weight);
            gen3j1d["thadmiss_DeltaR2"]->Fill(genper->WJa()->DeltaR(*genper->BHad()), weight);
            gen3j1d["thadmiss_DeltaR3"]->Fill(genper->WJa()->DeltaR(*genper->BLep()), weight);
        }        

	if(SEMILEPACC)
	{
		if(PDFTEST)
		{
                        double absdely = Abs(genper->THad().Rapidity() - genper->TLep().Rapidity());
		        if(absdely <0.5) pdfunc->Fill1d("pdfunc_reco_dy1mtt", genper->TT().M(), weight);
                        else if(absdely <1.0) pdfunc->Fill1d("pdfunc_reco_dy2mtt", genper->TT().M(), weight);
                        else if(absdely <1.5) pdfunc->Fill1d("pdfunc_reco_dy3mtt", genper->TT().M(), weight);
                        else pdfunc->Fill1d("pdfunc_reco_dy4mtt", genper->TT().M(), weight);
                }
        }

    }//end of cleanedjets.size()==3 (save time and size)

    //if(cleanedjets.size() == 3) {return;}

        //check what we have reconstructed
	if(SEMILEPACC)
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
            if(bestper.MtWLep() >140) return; //in TRUTH/truth_Mt_W or right_Mt_W/wrong_Mt_W/semi_Mt_W/other_Mt_W
		TLorentzVector CMttbar = bestper.THad() + bestper.TLep();
		TLorentzVector CMlept = bestper.TLep();
		TLorentzVector CMhadt = bestper.THad();
		CMttbar.Boost(-CMttbar.BoostVector());
		CMlept.Boost(-CMttbar.BoostVector());
		CMhadt.Boost(-CMttbar.BoostVector());

		double Mtt = (bestper.THad() + bestper.TLep()).M();
		double Mtt_boost = CMttbar.M();
                double Mthad = bestper.THad().M();
                double Mtlep = bestper.TLep().M();
		double costheta_had = bestper.THad().Pz()/bestper.THad().Mag();
		double costheta_lep = bestper.TLep().Pz()/bestper.TLep().Mag();
		double costheta_had_boost = CMhadt.Pz()/CMhadt.Mag();
		double costheta_lep_boost = CMlept.Pz()/CMlept.Mag();
		double deltaY = bestper.TLep().Rapidity() - bestper.THad().Rapidity();
		double deltaY_boost = CMlept.Rapidity() - CMhadt.Rapidity();
		double deltaBeta = bestper.TLep().P()/bestper.TLep().E() - bestper.THad().P()/bestper.THad().E();
		double deltaBeta_boost = CMlept.P()/CMlept.E() + CMhadt.P()/CMhadt.E();
                double tlepy = Abs(bestper.TLep().Rapidity());
                                    

                //for(int runmt = 150; runmt <= 800; runmt = runmt + 10){
                //    if(Mtt>= 2*runmt*cosh(deltaY/2) && Mtt< 2*(runmt+10)*cosh(deltaY/2)) yuka1d_reco["parametrize"]->Fill(runmt, weight);}
                if(TTMC){
                yuka2d_reco["Mtt_coshy2"]->Fill(Mtt, cosh(deltaY/2), weightparametrize);
                yuka2d_reco["reweight_Mt"]->Fill(Mtt/(2*cosh(deltaY/2)), weightparametrize-1, weightparametrize);
                yuka2d_reco["reweight_Mtt"]->Fill(Mtt, weightparametrize-1, weightparametrize);
                yuka2d_reco["reweight_delY"]->Fill(deltaY, weightparametrize-1, weightparametrize);
                yuka2d_reco["Mtt_coshy2_origin"]->Fill(Mtt, cosh(deltaY/2), weightparametrize_origin);
                yuka2d_reco["reweight_Mt_origin"]->Fill(Mtt/(2*cosh(deltaY/2)), weightparametrize_origin-1, weightparametrize_origin);
                yuka2d_reco["reweight_Mtt_origin"]->Fill(Mtt, weightparametrize_origin-1, weightparametrize_origin);
                yuka2d_reco["reweight_delY_origin"]->Fill(deltaY, weightparametrize_origin-1, weightparametrize_origin);}

               //cout << "hello!" <<endl; 
               //cout << Mtt <<", "<<deltaY<< ", "<<weight<<endl;

                yuka1d_reco["Mtt_resol"]->Fill((Mtt - (gentqhad+gentqlep).Mag())/(gentqhad+gentqlep).Mag(), weight);
                yuka1d_reco["delY_resol"]->Fill((deltaY - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);
        
                yuka1d_reco["MTwl"]->Fill(MTwl, weight);
                yuka1d_reco["njets"]->Fill(cleanedjets.size(), weight); 
		yuka1d_reco["Mtt"]->Fill(Mtt, weight);
		//yuka1d_reco["costheta"]->Fill(costheta_had, weight);
		//yuka1d_reco["costheta"]->Fill(costheta_lep, weight);
		yuka1d_reco["delY"]->Fill(deltaY, weight);
                //cout<<"lhe = "<<(lhet+lhetbar).Mag()<<", "<<(lhet.Rapidity()-lhetbar.Rapidity())<<endl;
                //cout<<"gen powheg = "<<(gentq+gentqbar).M()<<", "<<gentqlep.Rapidity()-gentqhad.Rapidity()<<endl;
                //cout<<"reco powheg = "<<(bestper.THad() + bestper.TLep()).M()<<", "<<bestper.TLep().Rapidity()-bestper.THad().Rapidity()<<endl;
		yuka1d_reco["Y"]->Fill(bestper.TLep().Rapidity(), weight);
		yuka1d_reco["Y"]->Fill(bestper.THad().Rapidity(), weight);
		//yuka1d_reco["delBeta"]->Fill(deltaBeta, weight);
                //yuka2d_reco["Mtt_costheta"]->Fill(Mtt, costheta_had, weight);
                //yuka2d_reco["Mtt_costheta"]->Fill(Mtt, costheta_lep, weight);
                //yuka2d_reco["delY_costheta"]->Fill(deltaY, costheta_lep, weight);
                //yuka2d_reco["delY_costheta"]->Fill(deltaY, costheta_had, weight);
		yuka2d_reco["Mtt_delY"]->Fill(Mtt, deltaY, weight);
		//yuka2d_reco["Mtt_delBeta"]->Fill(Mtt, deltaBeta, weight);
		//yuka2d_reco["delY_delBeta"]->Fill(deltaY, deltaBeta, weight);
	
                yuka1d_reco["tlepy"]->Fill(tlepy, weight);
                yuka2d_reco["Mtt_tlepy"]->Fill(Mtt, tlepy, weight);
                yuka2d_reco["delY_tlepy"]->Fill(deltaY, tlepy, weight);


		//end of yukawa studies

	if(SEMILEPACC)
	{
		if(PDFTEST)
		{
			/*pdfunc->Fill1d("pdfunc_reco_thardpt", genper->THard().Pt(), weight);
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
			pdfunc->Fill1d("pdfunc_reco_njets_ttpt", response2d.GetBin("njets_ttpt", cleanedjets.size() - 4, bestper.TT().Pt())-0.5, weight);*/
	
                        double absdely = Abs(genper->THad().Rapidity() - genper->TLep().Rapidity());
		        if(absdely <0.5) pdfunc->Fill1d("pdfunc_reco_dy1mtt", genper->TT().M(), weight);
                        else if(absdely <1.0) pdfunc->Fill1d("pdfunc_reco_dy2mtt", genper->TT().M(), weight);
                        else if(absdely <1.5) pdfunc->Fill1d("pdfunc_reco_dy3mtt", genper->TT().M(), weight);
                        else pdfunc->Fill1d("pdfunc_reco_dy4mtt", genper->TT().M(), weight);
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
                yuka1d_reco_right["Mtt_resol"]->Fill((Mtt - (gentqhad+gentqlep).Mag())/(gentqhad+gentqlep).Mag(), weight);
                yuka1d_reco_right["delY_resol"]->Fill((deltaY - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);

		yuka1d_reco_right["Mtt"]->Fill(Mtt, weight);
		//yuka1d_reco_right["costheta"]->Fill(costheta_had, weight);
		//yuka1d_reco_right["costheta"]->Fill(costheta_lep, weight);
		yuka1d_reco_right["delY"]->Fill(deltaY, weight);
		yuka1d_reco_right["Y"]->Fill(bestper.TLep().Rapidity(), weight);
		yuka1d_reco_right["Y"]->Fill(bestper.THad().Rapidity(), weight);
		//yuka1d_reco_right["delBeta"]->Fill(deltaBeta, weight);
                //yuka2d_reco_right["Mtt_costheta"]->Fill(Mtt, costheta_had, weight);
                //yuka2d_reco_right["Mtt_costheta"]->Fill(Mtt, costheta_lep, weight);
                //yuka2d_reco_right["delY_costheta"]->Fill(deltaY, costheta_lep, weight);
                //yuka2d_reco_right["delY_costheta"]->Fill(deltaY, costheta_had, weight);
		yuka2d_reco_right["Mtt_delY"]->Fill(Mtt, deltaY, weight);
		//yuka2d_reco_right["Mtt_delBeta"]->Fill(Mtt, deltaBeta, weight);
		//yuka2d_reco_right["delY_delBeta"]->Fill(deltaY, deltaBeta, weight);
	
                yuka1d_reco_right["tlepy"]->Fill(tlepy, weight);
                yuka2d_reco_right["Mtt_tlepy"]->Fill(Mtt, tlepy, weight);
                yuka2d_reco_right["delY_tlepy"]->Fill(deltaY, tlepy, weight);
                yuka1d_reco_right["MTwl"]->Fill(MTwl, weight);
                yuka1d_reco_right["njets"]->Fill(cleanedjets.size(), weight); 

		//end of yukawa studies

		truth1d["dRNu_right"]->Fill(bestper.Nu().DeltaR(genper->Nu()), weight);
		truth1d["dPtNu_right"]->Fill((bestper.Nu().Pt() - genper->Nu().Pt())/genper->Nu().Pt(), weight);
                truth2d["met_dpt_pxpy"]->Fill((bestper.Nu().Px()-met.Px())/met.Px(), (bestper.Nu().Py()-met.Py())/met.Py(), weight);
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
                yuka1d_reco_wrong["Mtt_resol"]->Fill((Mtt - (gentqhad+gentqlep).Mag())/(gentqhad+gentqlep).Mag(), weight);
                yuka1d_reco_wrong["delY_resol"]->Fill((deltaY - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);
		yuka1d_reco_wrong["Mtt"]->Fill(Mtt, weight);
		//yuka1d_reco_wrong["costheta"]->Fill(costheta_had, weight);
		//yuka1d_reco_wrong["costheta"]->Fill(costheta_lep, weight);
		yuka1d_reco_wrong["delY"]->Fill(deltaY, weight);
		yuka1d_reco_wrong["Y"]->Fill(bestper.TLep().Rapidity(), weight);
		yuka1d_reco_wrong["Y"]->Fill(bestper.THad().Rapidity(), weight);
		//yuka1d_reco_wrong["delBeta"]->Fill(deltaBeta, weight);
                //yuka2d_reco_wrong["Mtt_costheta"]->Fill(Mtt, costheta_had, weight);
                //yuka2d_reco_wrong["Mtt_costheta"]->Fill(Mtt, costheta_lep, weight);
                //yuka2d_reco_wrong["delY_costheta"]->Fill(deltaY, costheta_lep, weight);
                //yuka2d_reco_wrong["delY_costheta"]->Fill(deltaY, costheta_had, weight);
		yuka2d_reco_wrong["Mtt_delY"]->Fill(Mtt, deltaY, weight);
		//yuka2d_reco_wrong["Mtt_delBeta"]->Fill(Mtt, deltaBeta, weight);
		//yuka2d_reco_wrong["delY_delBeta"]->Fill(deltaY, deltaBeta, weight);
	
                yuka1d_reco_wrong["tlepy"]->Fill(tlepy, weight);
                yuka2d_reco_wrong["Mtt_tlepy"]->Fill(Mtt, tlepy, weight);
                yuka2d_reco_wrong["delY_tlepy"]->Fill(deltaY, tlepy, weight);
                yuka1d_reco_wrong["MTwl"]->Fill(MTwl, weight);
                yuka1d_reco_wrong["njets"]->Fill(cleanedjets.size(), weight); 

		//end of yukawa studies

	}
	else if(SEMILEPACC)
	{
		ttp_semi.Fill(bestper, weight);
                //for yukawa studies
                yuka1d_reco_semi["Mtt_resol"]->Fill((Mtt - (gentqhad+gentqlep).Mag())/(gentqhad+gentqlep).Mag(), weight);
                yuka1d_reco_semi["delY_resol"]->Fill((deltaY - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);
		yuka1d_reco_semi["Mtt"]->Fill(Mtt, weight);
		//yuka1d_reco_semi["costheta"]->Fill(costheta_had, weight);
		//yuka1d_reco_semi["costheta"]->Fill(costheta_lep, weight);
		yuka1d_reco_semi["delY"]->Fill(deltaY, weight);
		yuka1d_reco_semi["Y"]->Fill(bestper.TLep().Rapidity(), weight);
		yuka1d_reco_semi["Y"]->Fill(bestper.THad().Rapidity(), weight);
		//yuka1d_reco_semi["delBeta"]->Fill(deltaBeta, weight);
                //yuka2d_reco_semi["Mtt_costheta"]->Fill(Mtt, costheta_had, weight);
                //yuka2d_reco_semi["Mtt_costheta"]->Fill(Mtt, costheta_lep, weight);
                //yuka2d_reco_semi["delY_costheta"]->Fill(deltaY, costheta_lep, weight);
                //yuka2d_reco_semi["delY_costheta"]->Fill(deltaY, costheta_had, weight);
		yuka2d_reco_semi["Mtt_delY"]->Fill(Mtt, deltaY, weight);
		//yuka2d_reco_semi["Mtt_delBeta"]->Fill(Mtt, deltaBeta, weight);
		//yuka2d_reco_semi["delY_delBeta"]->Fill(deltaY, deltaBeta, weight);
	
                yuka1d_reco_semi["tlepy"]->Fill(tlepy, weight);
                yuka2d_reco_semi["Mtt_tlepy"]->Fill(Mtt, tlepy, weight);
                yuka2d_reco_semi["delY_tlepy"]->Fill(deltaY, tlepy, weight);
                yuka1d_reco_semi["MTwl"]->Fill(MTwl, weight);
                yuka1d_reco_semi["njets"]->Fill(cleanedjets.size(), weight); 

		//end of yukawa studies

	}
	else
	{
		ttp_other.Fill(bestper, weight);
                //for yukawa studies
                yuka1d_reco_other["Mtt_resol"]->Fill((Mtt - (gentqhad+gentqlep).Mag())/(gentqhad+gentqlep).Mag(), weight);
                yuka1d_reco_other["delY_resol"]->Fill((deltaY - (gentqlep.Rapidity() - gentqhad.Rapidity()))/(gentqlep.Rapidity() - gentqhad.Rapidity()), weight);
		yuka1d_reco_other["Mtt"]->Fill(Mtt, weight);
		//yuka1d_reco_other["costheta"]->Fill(costheta_had, weight);
		//yuka1d_reco_other["costheta"]->Fill(costheta_lep, weight);
		yuka1d_reco_other["delY"]->Fill(deltaY, weight);
		yuka1d_reco_other["Y"]->Fill(bestper.TLep().Rapidity(), weight);
		yuka1d_reco_other["Y"]->Fill(bestper.THad().Rapidity(), weight);
		//yuka1d_reco_other["delBeta"]->Fill(deltaBeta, weight);
                //yuka2d_reco_other["Mtt_costheta"]->Fill(Mtt, costheta_had, weight);
                //yuka2d_reco_other["Mtt_costheta"]->Fill(Mtt, costheta_lep, weight);
                //yuka2d_reco_other["delY_costheta"]->Fill(deltaY, costheta_lep, weight);
                //yuka2d_reco_other["delY_costheta"]->Fill(deltaY, costheta_had, weight);
		yuka2d_reco_other["Mtt_delY"]->Fill(Mtt, deltaY, weight);
		//yuka2d_reco_other["Mtt_delBeta"]->Fill(Mtt, deltaBeta, weight);
		//yuka2d_reco_other["delY_delBeta"]->Fill(deltaY, deltaBeta, weight);
	
                yuka1d_reco_other["tlepy"]->Fill(tlepy, weight);
                yuka2d_reco_other["Mtt_tlepy"]->Fill(Mtt, tlepy, weight);
                yuka2d_reco_other["delY_tlepy"]->Fill(deltaY, tlepy, weight);
                yuka1d_reco_other["MTwl"]->Fill(MTwl, weight);
                yuka1d_reco_other["njets"]->Fill(cleanedjets.size(), weight); 

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
	else if(SEMILEPACC)
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
		genbjets.clear();
		gencjets.clear();
		genljets.clear();
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
		//weightlhe = 1.;	
                weightparametrize = 1.;
                weightparametrize_origin = 1.;
                //weighttheoryunc = 1.;
		mcweight = 1.;	
		if(event.PUInfos().size() > 0)
		{
			const Geninfo& info = event.genInfo();
			mcweight = (info.weight() < 1. ? -1. : 1.);
			truth1d["counter"]->Fill(19.5, weight);
                        if(TTMC)
                        {
		    	    const vector<Mcweight>& ws =  event.MCWeights();
                            //frac. and ren. scale reweighnig, sys uncertainties 
			    if(cfacscale == -1) mcweight = ws[2].weights()/Abs(ws[0].weights());
			    else if(cfacscale == 1) mcweight = ws[1].weights()/Abs(ws[0].weights());
			    if(crenscale == -1) mcweight = ws[6].weights()/Abs(ws[0].weights());
			    else if(crenscale == 1) mcweight = ws[3].weights()/Abs(ws[0].weights());
                            if(chdamp == -1) mcweight = ws[227].weights()/Abs(ws[0].weights());
                            else if(chdamp == 1) mcweight = ws[245].weights()/Abs(ws[0].weights());

                        }
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
			runinfo[event.run].insert(event.lumi);
		}

                if(TTMC)
                {
                    SelectGenParticles(event);
                    gen1d["tpt"]->Fill(gentq.Pt(), weight);
                    gen1d["ty"]->Fill(Abs(gentq.Rapidity()), weight);
                    gen1d["ttpt"]->Fill((gentq+gentqbar).Pt(), weight);
                    gen1d["tty"]->Fill(Abs((gentq+gentqbar).Rapidity()), weight);
                    SelectPseudoTop(event);

                                TLorentzVector CMttbar = gentq + gentqbar; //genallper.THad() + genallper.TLep(); //gentoplep + gentophad;
				TLorentzVector CMhadt = gentqhad; //genallper.THad(); //gentophad;
				TLorentzVector CMlept = gentqlep; //genallper.TLep(); //gentoplep;
				CMttbar.Boost(-CMttbar.BoostVector());
				CMhadt.Boost(-CMttbar.BoostVector());
				CMlept.Boost(-CMttbar.BoostVector());

				//CASE1: Mtt and cos(theta)
				double Mtt = (gentq + gentqbar).M();//(genallper.TLep() + genallper.THad()).M(); //(gentoplep + gentophad).M();
				double Mtt_boost = CMttbar.M();
                                double Mt = gentq.M();
                                double Mtbar = gentqbar.M();
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
                                
                                    /*if(SEMILEPACC){ 
                                    yuka1d_offshell["Mt"]->Fill(Mt, weight);
                                    yuka1d_offshell["Mtbar"]->Fill(Mtbar, weight);
                                    TLorentzVector v1(gentq.Px(), gentq.Py(), gentq.Pz(), sqrt(gentq.P()*gentq.P()+172.5*172.5));
                                    TLorentzVector v2(gentqbar.Px(), gentqbar.Py(), gentqbar.Pz(), sqrt(gentqbar.P()*gentqbar.P()+172.5*172.5));
                                    yuka1d_offshell["pt"]->Fill(v1.Pt(), weight);
                                    yuka1d_offshell["pt"]->Fill(v2.Pt(), weight);
                                    yuka1d_offshell["pz"]->Fill(v1.Pz(), weight);
                                    yuka1d_offshell["pz"]->Fill(v2.Pz(), weight);
                                    weight2 *= yukahist_2d->GetBinContent(yukahist_2d->GetXaxis()->FindFixBin((v1+v2).M()), yukahist_2d->GetYaxis()->FindFixBin(deltaY)) + 1;
                                    yuka2d_gen["newMtt_delY"]->Fill(Mtt, deltaY, weight2);
                                    if(Mtt< 2*172.5*cosh(deltaY/2)) yuka1d_offshell["diffweight"]->Fill(weight2-1, weight);

                                    }*/

                                TLorentzVector v1(gentq.Px(), gentq.Py(), gentq.Pz(), sqrt(gentq.P()*gentq.P()+172.5*172.5));
                                TLorentzVector v2(gentqbar.Px(), gentqbar.Py(), gentqbar.Pz(), sqrt(gentqbar.P()*gentqbar.P()+172.5*172.5));
                                //weightlhe = weight;
                                weight *= yukahist_2d->GetBinContent(yukahist_2d->GetXaxis()->FindFixBin((v1+v2).M()), yukahist_2d->GetYaxis()->FindFixBin(deltaY)) + 1;
                                weightparametrize *= yukahist_2d->GetBinContent(yukahist_2d->GetXaxis()->FindFixBin((v1+v2).M()), yukahist_2d->GetYaxis()->FindFixBin(deltaY)) + 1;
                                weightparametrize_origin *= yukahist_2d->GetBinContent(yukahist_2d->GetXaxis()->FindFixBin(Mtt), yukahist_2d->GetYaxis()->FindFixBin(deltaY)) + 1;
                                //weightthoeryunc *= yukahist_2d->GetBinContent(yukahist_2d->GetXaxis()->FindFixBin((v1+v2).M()), yukahist_2d->GetYaxis()->FindFixBin(deltaY)) + 1;
                                //hcout<<"weightlhe="<<weightlhe<<", weight="<<weight<<endl;
                                

                                //if(Mtt>= 2*172.5*cosh(deltaY/2))
                                //    weight *= yukahist_2d->GetBinContent(yukahist_2d->GetXaxis()->FindFixBin(Mtt), yukahist_2d->GetYaxis()->FindFixBin(deltaY)) + 1;
                                //else
                                //    weight *= 1;
                                

				//CASE3: deltaBate
				double deltaBeta = gentqlep.P()/gentqlep.E() - gentqhad.P()/gentqhad.E(); 
                                //genallper.TLep().P()/genallper.TLep().E() - genallper.THad().P()/genallper.THad().E(); //gentoplep.P()/gentoplep.E() - gentophad.P()/gentophad.E();
				double deltaBeta_boost = CMlept.P()/CMlept.E() + CMhadt.P()/CMhadt.E();
				//if(deltaBeta >2) continue;
				//int weight_bin_beta = int(deltaBeta/0.1 + 0.5);
				//weight_beta *= yukahist_beta->GetBinContent(weight_bin_beta + 1) + 1;
			
                                double tlepy = Abs(gentqlep.Rapidity());

                                if(SEMILEPACC){
                                    /*for(int runmt = 150; runmt <= 800; runmt = runmt + 10){
                                        if(Mtt>= 2*runmt*cosh(deltaY/2) && Mtt< 2*(runmt+10)*cosh(deltaY/2)){ 
                                            yuka1d_gen["parametrize"]->Fill(runmt, weight);
                                        }
                                    }*/
                                    yuka2d_gen["Mtt_coshy2"]->Fill(Mtt, cosh(deltaY/2), weightparametrize);
                                    yuka2d_gen["reweight_Mt"]->Fill(Mtt/(2*cosh(deltaY/2)), weightparametrize-1, weightparametrize);
                                    yuka2d_gen["reweight_Mtt"]->Fill(Mtt, weightparametrize-1, weightparametrize);
                                    yuka2d_gen["reweight_delY"]->Fill(deltaY, weightparametrize-1, weightparametrize);
                                    yuka2d_gen["Mtt_coshy2_origin"]->Fill(Mtt, cosh(deltaY/2), weightparametrize_origin);
                                    yuka2d_gen["reweight_Mt_origin"]->Fill(Mtt/(2*cosh(deltaY/2)), weightparametrize_origin-1, weightparametrize_origin);
                                    yuka2d_gen["reweight_Mtt_origin"]->Fill(Mtt, weightparametrize_origin-1, weightparametrize_origin);
                                    yuka2d_gen["reweight_delY_origin"]->Fill(deltaY, weightparametrize_origin-1, weightparametrize_origin);

                                    /*
                                    yuka1d_gen["ht"]->Fill(gentqhad.P());
                                    double ht = gentqhad.P();
                                    for(int ht = 0; ht <= 2000; ht = ht + 200){
                                        if(ht > 200 && ht <= ht+200) yuka2d_gen["ht"]->Fill(Mtt, deltaY, weight);
                                    }*/
				//weight = weight_mtt* weight_dely* weight_beta;
				yuka1d_gen["Mtt"]->Fill(Mtt, weight);
				//yuka1d_gen["costheta"]->Fill(costheta_lep, weight);
				//yuka1d_gen["costheta"]->Fill(costheta_had, weight);
				yuka1d_gen["delY"]->Fill(deltaY, weight);
				yuka1d_gen["Y"]->Fill(gentq.Rapidity(), weight); //Fill(genallper.TLep().Rapidity(), weight);
				yuka1d_gen["Y"]->Fill(gentqbar.Rapidity(), weight); //Fill(genallper.THad().Rapidity(), weight);
				//yuka1d_gen["delBeta"]->Fill(deltaBeta, weight);
                                //yuka2d_gen["Mtt_costheta"]->Fill(Mtt, costheta_lep, weight);
                                //yuka2d_gen["Mtt_costheta"]->Fill(Mtt, costheta_had, weight);
                                //yuka2d_gen["delY_costheta"]->Fill(deltaY, costheta_lep, weight);
                                //yuka2d_gen["delY_costheta"]->Fill(deltaY, costheta_had, weight);
				yuka2d_gen["Mtt_delY"]->Fill(Mtt, deltaY, weight);
				//yuka2d_gen["Mtt_delBeta"]->Fill(Mtt, deltaBeta, weight);
				//yuka2d_gen["delY_delBeta"]->Fill(deltaY, deltaBeta, weight);
				
                                yuka1d_gen["tlepy"]->Fill(tlepy, weight);
                                yuka2d_gen["Mtt_tlepy"]->Fill(Mtt, tlepy, weight);
                                yuka2d_gen["delY_tlepy"]->Fill(deltaY, tlepy, weight);

                                /*if(cleanedjets.size() == 3){
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
                                gen3j2d["Mtt_delY"]->Fill((gentqlep + gentqhad_3j).Mag(), gentqlep.Rapidity()-gentqhad_3j.Rapidity(), weight);}*/
                                }
                    
                }//end of TTMC

                //cout << "before SelectGenParticles" <<endl;
		//SelectGenParticles(event);
                //cout << "before SelectPseudoTop" <<endl;
		//SelectPseudoTop(event);
		// Reweighting stuffs for yukawa study (should do it before next if statement, doesn't matter before or after the SelectPseudoTop(event) but keep after the SelecGenParticles())
        

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

		if(SEMILEPACC) 
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
				/*pdfunc->Fill1d("pdfunc_thardpt", genper->THard().Pt(), weight);
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
				pdfunc->Fill1d("pdfunc_njets_ttpt", response2d.GetBin("njets_ttpt", genaddjets.size(), genper->TT().Pt())-0.5, weight);*/
			
                                //double absdely = Abs(genper->THad().Rapidity() - genper->TLep().Rapidity());
                                //if(absdely <0.5) pdfunc->Fill1d("pdfunc_dy1mtt", genper->TT().M(), weight);
                                //else if(absdely <1.0) pdfunc->Fill1d("pdfunc_dy2mtt", genper->TT().M(), weight);
                                //else if(absdely <1.5) pdfunc->Fill1d("pdfunc_dy3mtt", genper->TT().M(), weight);
                                //else pdfunc->Fill1d("pdfunc_dy4mtt", genper->TT().M(), weight);
                        }
		}
		if(SEMILEPACC)
		{
			ttp_genacc.Fill(*genper, weight);
			truth1d["counter"]->Fill(2.5, weight);
		}

		if(isDA && Abs(event.trigger().HLT_IsoMu24()) != 1) {cout << "TRIGGER UNDEFINED IsoMu24:" << event.trigger().HLT_IsoMu24() << endl; }
		if(isDA && Abs(event.trigger().HLT_Ele27_WPTight_Gsf()) != 1) {cout << "TRIGGER UNDEFINED EL:" <<  event.trigger().HLT_Ele27_WPTight_Gsf() << endl; }
		if(isDA && Abs(event.trigger().HLT_IsoTkMu24()) != 1) {cout << "TRIGGER UNDEFINED: TKMu24" << event.trigger().HLT_IsoTkMu24() << endl; }
		if(
				(
                                 isDA == 0
                                 && (
                                        event.trigger().HLT_IsoMu24() == 1 || event.trigger().HLT_IsoTkMu24() == 1
                                     || (event.trigger().El27ptmax() != -1 && event.trigger().HLT_Ele27_WPTight_Gsf() == 1)
                                    )
                                ) ||
                                (
                                 isDA == 13 &&
                                     (
                                        event.trigger().HLT_IsoMu24() == 1 || event.trigger().HLT_IsoTkMu24() == 1 //2016
                                     )
                                ) ||
                                (
                                 isDA == 11 &&
                                    (
                                        event.trigger().HLT_IsoMu24() == -1 && event.trigger().HLT_IsoTkMu24() == -1 && event.trigger().El27ptmax() != -1 && event.trigger().HLT_Ele27_WPTight_Gsf() == 1 //2016 
                                    )
                                )
                )
		{
			//cout << "sel " << event.run << " " << event.lumi << " " << event.evt << " " << -1 << " " << cleanedjets.size() << endl;
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
