#include "../interface/RootMaker.h"
#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"

using namespace reco;
typedef ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > SMatrixSym3D;
typedef ROOT::Math::SVector<double, 3> SVector3;

RootMaker::RootMaker(const edm::ParameterSet& iConfig) :
	baseio("UREVENT", true),
	baseiorun("URRUN", true),
	baseiolumi("URLUMI", true),
	cgen(iConfig.getUntrackedParameter<bool>("GenSomeParticles", false)),
	cgenallparticles(iConfig.getUntrackedParameter<bool>("GenAllParticles", false)),
	cgenak5jets(iConfig.getUntrackedParameter<bool>("GenAK5Jets", false)),
	ctrigger(iConfig.getUntrackedParameter<bool>("Trigger", false)),
	cbeamspot(iConfig.getUntrackedParameter<bool>("RecBeamSpot", false)),
	crectrack(iConfig.getUntrackedParameter<bool>("RecTrack", false)),
	crecprimvertex(iConfig.getUntrackedParameter<bool>("RecPrimVertex", false)),
	crecsupercluster(iConfig.getUntrackedParameter<bool>("RecSuperCluster", false)),
	crecsuperclusterFilterPtMin(iConfig.getUntrackedParameter<double>("RecSuperClusterFilterPtMin", 0.)),
	crecsuperclusterFilterEtaMax(iConfig.getUntrackedParameter<double>("RecSuperClusterFilterEtaMax", 100000.)),
	crecsuperclustermember(iConfig.getUntrackedParameter<bool>("RecSuperClusterBasicCluster", false)),
	crecsuperclusterhit(iConfig.getUntrackedParameter<bool>("RecSuperClusterHit", false)),
	crecmuon(iConfig.getUntrackedParameter<bool>("RecMuon", false)),
	crectau(iConfig.getUntrackedParameter<bool>("RecTau", false)),
	crecelectron(iConfig.getUntrackedParameter<bool>("RecElectron", false)),
	crecphoton(iConfig.getUntrackedParameter<bool>("RecPhoton", false)),
	crecallconversion(iConfig.getUntrackedParameter<bool>("RecAllConversion", false)),
	crecak5calojet(iConfig.getUntrackedParameter<bool>("RecAK5CaloJet", false)),
	crecak5jptjet(iConfig.getUntrackedParameter<bool>("RecAK5JPTJet", false)),
	crecak5pfjet(iConfig.getUntrackedParameter<bool>("RecAK5PFJet", false)),
	crecak5pfchsjet(iConfig.getUntrackedParameter<bool>("RecAK5PFCHSJet", false)),
	crecpfmet(iConfig.getUntrackedParameter<bool>("RecPFMet", false)),
	crecsecvertices(iConfig.getUntrackedParameter<bool>("RecSecVertices", false)),
	crecmusecvertices(iConfig.getUntrackedParameter<bool>("RecMuSecVertices", false)),
	cHLTriggerNamesSelection(iConfig.getUntrackedParameter<vector<string> >("HLTriggerSelection")),
	cTriggerProcess(iConfig.getUntrackedParameter<string>("TriggerProcess", "HLT")),
	cMuPtMin(iConfig.getUntrackedParameter<double>("RecMuonPtMin", 0.)),
	cMuTrackIso(iConfig.getUntrackedParameter<double>("RecMuonTrackIso", 100000.)),
	cMuEtaMax(iConfig.getUntrackedParameter<double>("RecMuonEtaMax", 1000000.)),
	cMuHLTriggerMatching(iConfig.getUntrackedParameter<vector<string> >("RecMuonHLTriggerMatching")),
	cMuNum(iConfig.getUntrackedParameter<int>("RecMuonNum", 0)),
	cElPtMin(iConfig.getUntrackedParameter<double>("RecElectronPtMin", 0.)),
	cElTrackIso(iConfig.getUntrackedParameter<double>("RecElectronTrackIso", 1000000.)),
	cElEtaMax(iConfig.getUntrackedParameter<double>("RecElectronEtaMax", 1000000.)),
	cElHLTriggerMatching(iConfig.getUntrackedParameter<vector<string> >("RecElectronHLTriggerMatching")),
	cElNum(iConfig.getUntrackedParameter<int>("RecElectronNum", 0)),
	cElFilterPtMin(iConfig.getUntrackedParameter<double>("RecElectronFilterPtMin", 0.)),
	cElFilterEtaMax(iConfig.getUntrackedParameter<double>("RecElectronFilterEtaMax", 1000000.)),
	cTauPtMin(iConfig.getUntrackedParameter<double>("RecTauPtMin", 0.)),
	cTauEtaMax(iConfig.getUntrackedParameter<double>("RecTauEtaMax", 1000000.)),
	cTauHLTriggerMatching(iConfig.getUntrackedParameter<vector<string> >("RecTauHLTriggerMatching")),
	cTauDiscriminators(iConfig.getUntrackedParameter<vector<string> >("RecTauDiscriminators")),
	cTauNum(iConfig.getUntrackedParameter<int>("RecTauNum", 0)),
	cTrackFilterPtMin(iConfig.getUntrackedParameter<double>("RecTrackFilterPtMin", 0.)),
	cTrackPtMin(iConfig.getUntrackedParameter<double>("RecTrackPtMin", 0.)),
	cTrackEtaMax(iConfig.getUntrackedParameter<double>("RecTrackEtaMax", 1000000.)),
	cTrackNum(iConfig.getUntrackedParameter<int>("RecTrackNum", 0)),
	cPhotonPtMin(iConfig.getUntrackedParameter<double>("RecPhotonPtMin", 0.)),
	cPhotonEtaMax(iConfig.getUntrackedParameter<double>("RecPhotonEtaMax", 1000000.)),
	cPhotonHLTriggerMatching(iConfig.getUntrackedParameter<vector<string> >("RecPhotonHLTriggerMatching")),
	cPhotonNum(iConfig.getUntrackedParameter<int>("RecPhotonNum", 0)),
	cPhotonFilterPtMin(iConfig.getUntrackedParameter<double>("RecPhotonFilterPtMin", 0.)),
	cPhotonFilterEtaMax(iConfig.getUntrackedParameter<double>("RecPhotonFilterEtaMax", 1000000.)),
	cAK5CaloFilterPtMin(iConfig.getUntrackedParameter<double>("RecAK5CaloFilterPtMin", 0.)),
	cAK5CaloPtMin(iConfig.getUntrackedParameter<double>("RecAK5CaloPtMin", 0.)),
	cAK5CaloEtaMax(iConfig.getUntrackedParameter<double>("RecAK5CaloEtaMax", 1000000.)),
	cAK5CaloNum(iConfig.getUntrackedParameter<int>("RecAK5CaloNum", 0)),
	cAK5JPTFilterPtMin(iConfig.getUntrackedParameter<double>("RecAK5JPTFilterPtMin", 0.)),
	cAK5JPTPtMin(iConfig.getUntrackedParameter<double>("RecAK5JPTPtMin", 0.)),
	cAK5JPTEtaMax(iConfig.getUntrackedParameter<double>("RecAK5JPTEtaMax", 1000000.)),
	cAK5JPTNum(iConfig.getUntrackedParameter<int>("RecAK5JPTNum", 0)),
	cAK5PFCHSFilterPtMin(iConfig.getUntrackedParameter<double>("RecAK5PFCHSFilterPtMin", 0.)),
	cAK5PFCHSPtMin(iConfig.getUntrackedParameter<double>("RecAK5PFCHSPtMin", 0.)),
	cAK5PFCHSEtaMax(iConfig.getUntrackedParameter<double>("RecAK5PFCHSEtaMax", 1000000.)),
	cAK5PFCHSNum(iConfig.getUntrackedParameter<int>("RecAK5PFCHSNum", 0)),
	cAK5PFFilterPtMin(iConfig.getUntrackedParameter<double>("RecAK5PFFilterPtMin", 0.)),
	cAK5PFPtMin(iConfig.getUntrackedParameter<double>("RecAK5PFPtMin", 0.)),
	cAK5PFEtaMax(iConfig.getUntrackedParameter<double>("RecAK5PFEtaMax", 1000000.)),
	cAK5PFNum(iConfig.getUntrackedParameter<int>("RecAK5PFNum", 0)),
	cJetCorrection(iConfig.getUntrackedParameter<string>("JetCorrection", "L1FastL2L3Residual")),
	cJetHLTriggerMatching(iConfig.getUntrackedParameter<vector<string> >("RecJetHLTriggerMatching")),
	cMassMuMuMin(iConfig.getUntrackedParameter<double>("RecMassMuMuMin", 0.)),
	cMassMuMuMax(iConfig.getUntrackedParameter<double>("RecMassMuMuMax", 0.)),
	cVertexTRKChi2(iConfig.getUntrackedParameter<double>("RecVertexTRKChi2", 10.)),
	cVertexTRKHitsMin(iConfig.getUntrackedParameter<int>("RecVertexTRKHitsMin", 6)),
	cVertexChi2(iConfig.getUntrackedParameter<double>("RecVertexChi2", 5.)),
	cVertexSig2D(iConfig.getUntrackedParameter<double>("RecVertexSig2D", 15.)),
	cKaonMassWindow(iConfig.getUntrackedParameter<double>("RecVertexKaonMassWin", 0.05)),
	cLambdaMassWindow(iConfig.getUntrackedParameter<double>("RecVertexLambdaMassWin", 0.02)),
	propagatorWithMaterial(0)
{
	double barrelRadius = 129.; //p81, p50, ECAL TDR
	double endcapZ = 320.5; // fig 3.26, p81, ECAL TDR
	Surface::RotationType rot;

	ecalBarrel = Cylinder::build(Surface::PositionType(0, 0, 0), rot, barrelRadius);
	ecalNegativeEtaEndcap = Plane::build(Surface::PositionType(0, 0, -endcapZ), rot);
	ecalPositiveEtaEndcap = Plane::build(Surface::PositionType(0, 0, endcapZ), rot);

    testids.push_back(24);//0
    testids.push_back(-24);//1
    testids.push_back(22);//2
    testids.push_back(23);//3
    testids.push_back(25);//4
    testids.push_back(35);//5
    testids.push_back(36);//6
    testids.push_back(37);//7
    testids.push_back(-37);//8
    testids.push_back(6);//9
    testids.push_back(-6);//10
    testids.push_back(5);//11
    testids.push_back(-5);//12
    testids.push_back(32);//13
    testids.push_back(8);//14
    testids.push_back(-8);//15
    testids.push_back(15);//16
    testids.push_back(-15);//17
    testids.push_back(6000047);//18

}

RootMaker::~RootMaker()
{
	if(propagatorWithMaterial != 0){ delete propagatorWithMaterial;}
}

void RootMaker::beginJob()
{
	edm::Service<TFileService> FS;
	baseio.SetFile(&(FS->file()));
	baseiorun.SetFile(&(FS->file()));
	baseiolumi.SetFile(&(FS->file()));
	histmu = FS->make<TH1D>("mu", "mu", 600, 0., 300.);
	histpu = FS->make<TH1D>("pu", "pu", 300, 0., 300.);
	histpum = FS->make<TH1D>("pum", "pum", 300, 0., 300.);
	histpup = FS->make<TH1D>("pup", "pup", 300, 0., 300.);
}

void RootMaker::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup)
{
	if(propagatorWithMaterial != 0){ delete propagatorWithMaterial;}
	iSetup.get<IdealMagneticFieldRecord>().get(magneticField);
	propagatorWithMaterial = new PropagatorWithMaterial(alongMomentum, 0.10566, &(*magneticField));
	iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", TTrackBuilder);

	using namespace BASEIORUN;
	baseiorun.StartFilling();
	IORunInfo runinfo(baseiorun.GetIORunInfo(0));
	runinfo.RunNumber(iRun.run());

//	//L1 prescales
//	edm::ESHandle<L1GtPrescaleFactors> l1GtPfAlgo;
//	iSetup.get<L1GtPrescaleFactorsAlgoTrigRcd>().get(l1GtPfAlgo);
//	runinfo.NumL1Algos((l1GtPfAlgo.product()->gtPrescaleFactors())[0].size());
//	runinfo.L1AlgoPrescaleColumns((l1GtPfAlgo.product()->gtPrescaleFactors()).size());
//	for(Int_t i = 0 ; i < runinfo.L1AlgoPrescaleColumns() ; i++)
//	{
//		for(Int_t j = 0 ; j < runinfo.NumL1Algos() ; j++)
//		{
//			runinfo.L1AlgoPrescaleTable((l1GtPfAlgo.product()->gtPrescaleFactors())[i][j], j+runinfo.NumL1Algos()*i);
//		}
//	}
//
//	//L1Tech
//	edm::ESHandle<L1GtPrescaleFactors> l1GtPfTech;
//	iSetup.get<L1GtPrescaleFactorsTechTrigRcd>().get(l1GtPfTech);
//
//	runinfo.NumL1Techs((l1GtPfTech.product()->gtPrescaleFactors())[0].size());
//	runinfo.L1TechPrescaleColumns((l1GtPfTech.product()->gtPrescaleFactors()).size());
//	for(Int_t i = 0 ; i < runinfo.L1TechPrescaleColumns() ; i++)
//	{
//		for(Int_t j = 0 ; j < runinfo.NumL1Techs() ; j++)
//		{
//			runinfo.L1TechPrescaleTable((l1GtPfTech.product()->gtPrescaleFactors())[i][j], j+runinfo.NumL1Techs()*i);
//		}
//	}

	if(ctrigger)
	{
		bool changed = true;
		HLTConfiguration.init(iRun, iSetup, cTriggerProcess, changed);
	}
	runinfo.NumHLTs(HLTConfiguration.size());
	runinfo.HLTPrescaleColumns(HLTConfiguration.prescaleSize());

	boost::cmatch what;
	vector<boost::regex> trigregexes;
	for(unsigned i = 0 ; i < cHLTriggerNamesSelection.size() ; i++)
	{
		trigregexes.push_back(boost::regex(cHLTriggerNamesSelection[i].c_str()));
	}

	string allnames;
	for(unsigned i = 0 ; i < HLTConfiguration.size() ; i++)
	{
		unsigned TriggerIndex = HLTConfiguration.triggerIndex(HLTConfiguration.triggerName(i));
		for(unsigned j = 0 ; j < trigregexes.size() ; j++)
		{
			if(boost::regex_match(HLTConfiguration.triggerName(i).c_str(), what, trigregexes[j]))
			{
				HLTriggerIndexSelection.push_back(TriggerIndex);
			}
		}
		allnames += HLTConfiguration.triggerName(i) + string(" ");
	}
	String(runinfo.HLTNames()).Set(allnames);

	string allmuonnames;
	string allelectronnames;
	string alltaunames;
	string allphotonnames;
	string alljetnames;

	TriggerIndexSelection(cMuHLTriggerMatching, muontriggers, allmuonnames);
	String(runinfo.HLTNamesMuMatched()).Set(allmuonnames);
	TriggerIndexSelection(cElHLTriggerMatching, electrontriggers, allelectronnames);
	String(runinfo.HLTNamesElMatched()).Set(allelectronnames);
	TriggerIndexSelection(cTauHLTriggerMatching, tautriggers, alltaunames);
	String(runinfo.HLTNamesTauMatched()).Set(alltaunames);
	TriggerIndexSelection(cPhotonHLTriggerMatching, photontriggers, allphotonnames);
	String(runinfo.HLTNamesPhMatched()).Set(allphotonnames);
	TriggerIndexSelection(cJetHLTriggerMatching, jettriggers, alljetnames);
	String(runinfo.HLTNamesJetMatched()).Set(alljetnames);

	L1GtUtils l1info;
	l1info.retrieveL1EventSetup(iSetup);

	edm::ESHandle<L1GtPrescaleFactors> l1GtPfAlgo;
	iSetup.get<L1GtPrescaleFactorsAlgoTrigRcd>().get(l1GtPfAlgo);
	edm::ESHandle<L1GtPrescaleFactors> l1GtPfTech;
	iSetup.get<L1GtPrescaleFactorsTechTrigRcd>().get(l1GtPfTech);
	cout << "HALLO " << HLTConfiguration.prescaleSize() << " " << HLTConfiguration.size() << endl;
	for(unsigned j = 0 ; j < HLTConfiguration.prescaleSize() ; j++)
	{
		for(unsigned i = 0 ; i < HLTConfiguration.size() ; i++)
		{
			int l1bit = -1;
			int l1prescalealgo = 0;
			int l1prescaletech = 0;
			L1GtUtils::TriggerCategory trigCategory;
			const vector<pair<bool, string> > l1seed = HLTConfiguration.hltL1GTSeeds(i);
			if(l1seed.size() == 1)
			{
				l1info.l1AlgoTechTrigBitNumber(l1seed[0].second, trigCategory, l1bit);
				l1prescalealgo = (l1GtPfAlgo.product()->gtPrescaleFactors())[j][l1bit];
				l1prescaletech = (l1GtPfTech.product()->gtPrescaleFactors())[j][l1bit];
			}
			runinfo.HLTPrescaleTable(HLTConfiguration.prescaleValue(j, HLTConfiguration.triggerName(i)), i+HLTConfiguration.size()*j);
			runinfo.HLTSeedAlgoPrescaleTable(l1prescalealgo, i+HLTConfiguration.size()*j);
			runinfo.HLTSeedTechPrescaleTable(l1prescaletech, i+HLTConfiguration.size()*j);
			cout << j << " " << i << " " << HLTConfiguration.prescaleValue(j, HLTConfiguration.triggerName(i)) << " " << l1prescalealgo << " " << l1prescaletech << endl;
		}
	}

	baseiorun.Fill();
}

void RootMaker::beginLuminosityBlock(const edm::LuminosityBlock& iLumiBlock, const edm::EventSetup& iSetup)
{
    lumi_eventsprocessed = 0;
    lumi_eventsfiltered = 0;
}

void RootMaker::endLuminosityBlock(const edm::LuminosityBlock& iLumiBlock, const edm::EventSetup& iSetup)
{
	using namespace BASEIOLUMI;
	baseiolumi.StartFilling();
	IOLumiInfo lumiinfo(baseiolumi.GetIOLumiInfo(0));

	lumiinfo.RunNumber(iLumiBlock.run());
	lumiinfo.BlockNumber(iLumiBlock.luminosityBlock());
	lumiinfo.EventsProcessed(lumi_eventsprocessed);
	lumiinfo.EventsFiltered(lumi_eventsfiltered);
	lumiinfo.HLTPrescaleColumn(lumi_hltprescaletable);

	edm::Handle<LumiSummary> lumiSummary;
	iLumiBlock.getByLabel(edm::InputTag("lumiProducer"), lumiSummary);
	if(lumiSummary.isValid())
	{
		lumiinfo.LumiValue(lumiSummary->avgInsDelLumi());
		lumiinfo.LumiValueUnc(lumiSummary->avgInsDelLumiErr());
	}
	else
	{
		lumiinfo.LumiValue(-1);
		lumiinfo.LumiValueUnc(-1);
	}

	baseiolumi.Fill();
}


void RootMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace BASEIO;
	baseio.StartFilling();
	lumi_eventsprocessed++;
	bool takeevent = false;

	pv_position = math::XYZPoint(0.,0.,0.);
	bs_position = math::XYZPoint(0.,0.,0.);

	iEvent.getByLabel(edm::InputTag("TriggerResults", "", cTriggerProcess), HLTrigger);

	EventInfo evinfo(baseio.GetIOEventInfo(0), HLTrigger->size());
	evinfo.EventNumber(iEvent.id().event());
	evinfo.RunNumber(iEvent.id().run());
	evinfo.LumiSectionNumber(iEvent.getLuminosityBlock().luminosityBlock());
	evinfo.TimeUnix(iEvent.time().unixTime());
	evinfo.TimeMuSec(iEvent.time().microsecondOffset());
	edm::Handle<double> rho;
	iEvent.getByLabel(edm::InputTag("kt6PFJets", "rho", "ROOTMAKER"), rho);
	evinfo.AK5PFRho(*rho);
	edm::Handle<double> sigma;
	iEvent.getByLabel(edm::InputTag("kt6PFJets", "sigma", "ROOTMAKER"), sigma);
	evinfo.AK5PFSigma(*sigma);

	edm::Handle<vector<PileupSummaryInfo> > PUInfo;
	iEvent.getByLabel(edm::InputTag("addPileupInfo"), PUInfo);

	if(PUInfo.isValid())
	{
		for(vector<PileupSummaryInfo>::const_iterator PVI = PUInfo->begin(); PVI != PUInfo->end(); ++PVI)
		{
			int BX = PVI->getBunchCrossing();
			if(BX == -1)
			{ 
				histpum->Fill(PVI->getPU_NumInteractions());
			}
			else if(BX == 0)
			{ 
				histpu->Fill(PVI->getPU_NumInteractions());
				histmu->Fill(PVI->getTrueNumInteractions());
			}
			else if(BX == 1)
			{ 
				histpup->Fill(PVI->getPU_NumInteractions());
			}
		}
	}

	//HLTriggerResults
	for(unsigned i = 0 ; i < min(unsigned(HLTrigger->size()), unsigned(1200)) ; i++)
	{
		evinfo.SetHLT(i, HLTrigger->accept(i));
		if(HLTrigger->accept(i) && find(HLTriggerIndexSelection.begin(), HLTriggerIndexSelection.end(), i) != HLTriggerIndexSelection.end())
		{ takeevent = true;}
	}
	evinfo.Write();
	lumi_hltprescaletable = HLTConfiguration.prescaleSet(iEvent, iSetup);
	//TriggerEvent for matching
	iEvent.getByLabel(edm::InputTag("hltTriggerSummaryAOD", "", cTriggerProcess), HLTriggerEvent);
	//cout<<"PASSED!"<<endl;
	if(HLTriggerIndexSelection.size() == 0 || !ctrigger)
	{
		takeevent = true;
	}

	if(!takeevent) return;

	if(crecprimvertex)
	{
		iEvent.getByLabel(edm::InputTag("offlinePrimaryVertices"), Vertices);

		if(Vertices.isValid())
		{
			for(unsigned i = 0 ; i < Vertices->size(); i++)
			{
				PrimaryVertex vtx = baseio.GetPrimaryVertex(baseio.NumPrimaryVertexs());
				vtx.Vx((*Vertices)[i].x());
				vtx.Vy((*Vertices)[i].y());
				vtx.Vz((*Vertices)[i].z());
				vtx.ChiQ((*Vertices)[i].chi2());
				vtx.NDOF((*Vertices)[i].ndof());
				vtx.NumTracks((*Vertices)[i].tracksSize());
				Float_t ptq = 0.;
				for(Vertex::trackRef_iterator it = (*Vertices)[i].tracks_begin() ; it != (*Vertices)[i].tracks_end() ; ++it)
				{
					ptq += (*it)->pt() * (*it)->pt();
				}
				vtx.SumPtQ(ptq);
			}
			if(Vertices->size() > 0)
			{
				pv_position = (*Vertices)[0].position();
				primvertex = (*Vertices)[0];
			}
			else
			{
				return;
			}
		}
	}

	if(cbeamspot)
	{
		edm::Handle<BeamSpot> TheBeamSpot;
		iEvent.getByLabel(edm::InputTag("offlineBeamSpot"), TheBeamSpot);
		if(TheBeamSpot.isValid())
		{
			IOBeamSpot bs = baseio.GetIOBeamSpot(0);
			bs.Vx(TheBeamSpot->x0());
			bs.Vy(TheBeamSpot->y0());
			bs.Vz(TheBeamSpot->z0());
			bs.XWidth(TheBeamSpot->BeamWidthX());
			bs.YWidth(TheBeamSpot->BeamWidthY());
			bs.YWidth(TheBeamSpot->sigmaZ());
			bs_position = TheBeamSpot->position();
		}
	}

	takeevent = false;
	//iEvent.getByLabel(edm::InputTag("dedxHarmonic2"), dEdxharmonic2);

	if(crectrack)
	{
		takeevent = AddTracks(iEvent) || takeevent;
	}
	if(crecmuon)
	{
		takeevent = AddMuons(iEvent) || takeevent;
	}
	if(crecelectron)
	{
		takeevent = AddElectrons(iEvent, iSetup) || takeevent;
	}
	if(crecphoton)
	{
		takeevent = AddPhotons(iEvent, iSetup) || takeevent;
	}
	if(crecak5pfjet)
	{
		takeevent = AddAK5PFJets(iEvent, iSetup) || takeevent;
	}
	if(!takeevent) return;

	if(crecpfmet)
	{
		IOMET met = baseio.GetIOMET(0);
		edm::Handle<reco::PFMETCollection> pfMet;
		iEvent.getByLabel(edm::InputTag("pfMet"), pfMet);
		if(pfMet.isValid() && pfMet->size() > 0)
		{
			met.px((*pfMet)[0].px());
			met.py((*pfMet)[0].py());
			met.pxUnc((*pfMet)[0].getSignificanceMatrix()(0,0));
			met.pyUnc((*pfMet)[0].getSignificanceMatrix()(1,1));
			met.pxpyUnc((*pfMet)[0].getSignificanceMatrix()(1,0));
		}
		met = baseio.GetIOMET(1);
		edm::Handle<reco::PFMETCollection> pfMetT0;
		iEvent.getByLabel(edm::InputTag("pfMetT0pc"), pfMetT0);
		if(pfMetT0.isValid() && pfMetT0->size() > 0)
		{
			met.px((*pfMetT0)[0].px());
			met.py((*pfMetT0)[0].py());
			met.pxUnc((*pfMet)[0].getSignificanceMatrix()(0,0));
			met.pyUnc((*pfMet)[0].getSignificanceMatrix()(1,1));
			met.pxpyUnc((*pfMet)[0].getSignificanceMatrix()(1,0));
		}
		met = baseio.GetIOMET(2);
		edm::Handle<reco::PFMETCollection> pfMetT1;
		iEvent.getByLabel(edm::InputTag("pfMetT1"), pfMetT1);
		if(pfMetT1.isValid() && pfMetT1->size() > 0)
		{
			met.px((*pfMetT1)[0].px());
			met.py((*pfMetT1)[0].py());
			met.pxUnc((*pfMet)[0].getSignificanceMatrix()(0,0));
			met.pyUnc((*pfMet)[0].getSignificanceMatrix()(1,1));
			met.pxpyUnc((*pfMet)[0].getSignificanceMatrix()(1,0));
		}
		met = baseio.GetIOMET(3);
		edm::Handle<reco::PFMETCollection> pfMetT0T1;
		iEvent.getByLabel(edm::InputTag("pfMetT0pcT1"), pfMetT0T1);
		if(pfMetT0T1.isValid() && pfMetT0T1->size() > 0)
		{
			met.px((*pfMetT0T1)[0].px());
			met.py((*pfMetT0T1)[0].py());
			met.pxUnc((*pfMet)[0].getSignificanceMatrix()(0,0));
			met.pyUnc((*pfMet)[0].getSignificanceMatrix()(1,1));
			met.pxpyUnc((*pfMet)[0].getSignificanceMatrix()(1,0));
		}
		//edm::Handle<reco::PFMETCollection> pfMetType0Type1;
		//iEvent.getByLabel(edm::InputTag("pfType0Type1CorrectedMet"), pfMetType0Type1);
		//if(pfMetType0Type1.isValid() && pfMetType0Type1->size() > 0)
		//{
		//	met.pfmetpx((*pfMetType0Type1)[0].px());
		//	met.pfmetpy((*pfMetType0Type1)[0].py());
		//}
	}

	if(cgen || cgenallparticles || cgenak5jets)
	{
		GenInfo gi = baseio.GetGenInfo(0);
		edm::Handle<GenEventInfoProduct> HEPMC;
		iEvent.getByLabel(edm::InputTag("generator"), HEPMC);
		if(HEPMC.isValid())
		{
			gi.Weight(HEPMC->weight());
			gi.PDGID1(HEPMC->pdf()->id.first);
			gi.x1(HEPMC->pdf()->x.first);
			gi.PDGID2(HEPMC->pdf()->id.second);
			gi.x2(HEPMC->pdf()->x.second);
			gi.RenScale(HEPMC->qScale());
			gi.FacScale(HEPMC->qScale());
		}
		if(PUInfo.isValid())
		{
			for(vector<PileupSummaryInfo>::const_iterator PVI = PUInfo->begin(); PVI != PUInfo->end(); ++PVI)
			{
				int BX = PVI->getBunchCrossing();
				if(BX == -1)
				{ 
					gi.NumPUInteractionsBefore(PVI->getPU_NumInteractions());
				}
				else if(BX == 0)
				{ 
					gi.NumPUInteractions(PVI->getPU_NumInteractions());
					gi.NumTrueInteractions(PVI->getTrueNumInteractions());
				}
				else if(BX == 1)
				{ 
					gi.NumPUInteractionsAfter(PVI->getPU_NumInteractions());
				}
			}
		}


		edm::Handle<GenMETCollection> GenMetTrue;
		iEvent.getByLabel(edm::InputTag("genMetTrue"), GenMetTrue);
		if(GenMetTrue.isValid() && GenMetTrue->size() > 0)
		{
			
			gi.METx((*GenMetTrue)[0].px());
			gi.METy((*GenMetTrue)[0].py());
		}
		else
		{
			gi.METx(0.);
			gi.METy(0.);
		}

	}  

	if(cgen)
	{
		edm::Handle<GenParticleCollection> GenParticles;
		iEvent.getByLabel(edm::InputTag("genPlusSimParticles"), GenParticles);
		if(!GenParticles.isValid())
		{
			iEvent.getByLabel(edm::InputTag("genParticles"), GenParticles);
		}

		if(GenParticles.isValid())
		{
			for(unsigned i = 0 ; i < GenParticles->size() ; i++)
			{
				bool fill = false;
				if((*GenParticles)[i].status() > 20 && (*GenParticles)[i].status() < 30){fill = true;}
				if((abs((*GenParticles)[i].pdgId()) <= 6 || (*GenParticles)[i].pdgId() == 21))
				{
					for(size_t nm = 0 ; nm < (*GenParticles)[i].numberOfMothers() ; ++nm)
					{
						if(TMath::Abs((*GenParticles)[i].mother(nm)->pdgId()) == 24)
						{
							fill = true;
						}
					}
				}
				if((*GenParticles)[i].status() == 1)
				{
					if(TMath::Abs((*GenParticles)[i].pdgId()) >= 11 && TMath::Abs((*GenParticles)[i].pdgId()) <= 16) {fill = true;}
				}
				if(fill)
				{
					SelectedGenParticle gp = baseio.GetSelectedGenParticle(baseio.NumSelectedGenParticles());
					gp.e((*GenParticles)[i].energy());
					gp.px((*GenParticles)[i].px());
					gp.py((*GenParticles)[i].py());
					gp.pz((*GenParticles)[i].pz());
					//gp.vx((*GenParticles)[i].vx());
					//gp.vy((*GenParticles)[i].vy());
					//gp.vz((*GenParticles)[i].vz());
					gp.PDGID((*GenParticles)[i].pdgId());
					gp.Status((*GenParticles)[i].status());
					pair<Int_t, Int_t> motherinfo = HasAnyMother(&(*GenParticles)[i], testids);
					gp.Info(motherinfo.first);
					gp.IndirectMother(motherinfo.second);
					if((*GenParticles)[i].numberOfMothers() > 0)
					{
						gp.Mother((*GenParticles)[i].mother(0)->pdgId());
					}
					else
					{
						gp.Mother(0);
					}
				}
			}
		}
	}

	if(cgenallparticles)
	{
		edm::Handle<GenParticleCollection> GenParticles;
		iEvent.getByLabel(edm::InputTag("genPlusSimParticles"), GenParticles);
		if(!GenParticles.isValid())
		{
			iEvent.getByLabel(edm::InputTag("genParticles"), GenParticles);
		}

		if(GenParticles.isValid())
		{
			for(unsigned i = 0 ; i < GenParticles->size() ; i++)
			{
				AllGenParticle gp = baseio.GetAllGenParticle(baseio.NumAllGenParticles());
				gp.e((*GenParticles)[i].energy());
				gp.px((*GenParticles)[i].px());
				gp.py((*GenParticles)[i].py());
				gp.pz((*GenParticles)[i].pz());
				gp.vx((*GenParticles)[i].vx());
				gp.vy((*GenParticles)[i].vy());
				gp.vz((*GenParticles)[i].vz());
				gp.PDGID((*GenParticles)[i].pdgId());
				gp.Status((*GenParticles)[i].status());
				int counter = 0;
				for(unsigned j = 0 ; j < (*GenParticles)[i].numberOfMothers() ; j++)
				{
					gp.Mother(FindGenParticle(*GenParticles, (*GenParticles)[i].mother(j)), counter);
					counter++;
				}
				counter = 0;
				for(unsigned j = 0 ; j < (*GenParticles)[i].numberOfDaughters() ; j++)
				{
					gp.Daughter(FindGenParticle(*GenParticles, (*GenParticles)[i].daughter(j)), counter);
					counter++;
				}
			}

		}
	}

	if(takeevent)
	{
		lumi_eventsfiltered++;
		baseio.Fill();
	}
}

void RootMaker::endJob()
{
}

bool RootMaker::AddMuons(const edm::Event& iEvent)
{
	using namespace BASEIO;
	int NumGood = 0;
	edm::Handle<MuonCollection> Muons;
	iEvent.getByLabel(edm::InputTag("muons"), Muons);
	for(unsigned i = 0 ; i < Muons->size() ; i++)
	{
		MuonRef mu(Muons, i);
		IOMuon muout(baseio.GetIOMuon(baseio.NumIOMuons()));
		muout.px(mu->px());
		muout.py(mu->py());
		muout.pz(mu->pz());
		muout.TriggerMatching(GetTrigger(*mu, muontriggers));
		muout.ECalEnergy(mu->calEnergy().em);
		muout.HCalEnergy(mu->calEnergy().had);
		muout.NumChambers(mu->numberOfChambers());
		muout.NumChambersWithSegments(mu->numberOfMatches(Muon::SegmentAndTrackArbitration));
		muout.NumMatchedStations(mu->numberOfMatchedStations());
		if(mu->globalTrack().isNonnull())
		{
			muout.PtUnc(mu->globalTrack()->ptError());
			muout.ChiQ(mu->globalTrack()->chi2());
			muout.NDOF(mu->globalTrack()->ndof());
			muout.NumValidMuonHits(mu->globalTrack()->hitPattern().numberOfValidMuonHits());
		}
		else
		{
			muout.PtUnc(-1.);
			muout.ChiQ(-1.);
			muout.NDOF(0);
			muout.NumValidMuonHits(-1);
		}

		TrackRef innertrack = mu->innerTrack();
		if(innertrack.isNonnull()) 
		{
			IOTrack tr(muout.InnerTrack(0));
			FillTrack(tr, innertrack);
		}
		//PFIsolation
		const reco::MuonPFIsolation pfisor04 = mu->pfIsolationR04();
		PFIsolation r4(muout.PFR4());
		r4.Charged(pfisor04.sumChargedHadronPt);
		r4.Neutral(pfisor04.sumNeutralHadronEt);
		r4.Photon(pfisor04.sumPhotonEt);
	
		UInt_t info = 0;
		info |= mu->isGlobalMuon() << 0;
		info |= mu->isTrackerMuon() << 1;
		info |= mu->isStandAloneMuon() << 2;
		info |= mu->isCaloMuon() << 3;
		info |= mu->isPFMuon() << 4;
		info |= (mu->charge() == 1) << 5;
		muout.Info(info);

		if(mu->pt() >= cMuPtMin && fabs(mu->eta()) <= cMuEtaMax)
		{
			NumGood++;
		}
	}

    if(NumGood >= cMuNum) {return true;}
	return false;
}

bool RootMaker::AddPhotons(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace BASEIO;
	int NumGood = 0;
	edm::Handle<PhotonCollection> Photons;
	iEvent.getByLabel(edm::InputTag("photons"), Photons);

	if(Photons.isValid() && Photons->size() > 0)
	{
		edm::Handle<GsfElectronCollection> Electrons;
		iEvent.getByLabel(edm::InputTag("gedGsfElectrons"), Electrons);
		edm::Handle<ConversionCollection> Conversions;
		iEvent.getByLabel(edm::InputTag("allConversions"), Conversions);

		for(size_t n = 0 ; n < Photons->size() ; n++)
		{
			PhotonRef ph(Photons, n);
			if(ph->pt() < cPhotonFilterPtMin || TMath::Abs(ph->eta()) > cPhotonFilterEtaMax){continue;}
			IOPhoton phout(baseio.GetIOPhoton(baseio.NumIOPhotons()));
			phout.px(ph->px());
			phout.py(ph->py());
			phout.pz(ph->pz());
			phout.E1x5(ph->e1x5());
			phout.E1x5(ph->e2x5());
			phout.E1x5(ph->e3x3());
			phout.E1x5(ph->e5x5());
			phout.MaxCrystalEnergy(ph->maxEnergyXtal());
			phout.SigmaIEtaIEta(ph->sigmaIetaIeta());
			phout.EHCalTowerOverECalD1(ph->hadronicDepth1OverEm());
			phout.EHCalTowerOverECalD2(ph->hadronicDepth2OverEm());
			PFIsolation r3(phout.PFR3());
			r3.Charged(ph->chargedHadronIso());
			r3.Neutral(ph->neutralHadronIso());
			r3.Photon(ph->photonIso());
			IOSuperCluster phsc(phout.SC());
			SuperClusterRef sc = ph->superCluster();
			FillSC(phsc, sc);
			UInt_t info = 0;
			info |= ph->isEB() << 0;
			info |= ph->isEE() << 1;
			info |= ph->isPFlowPhoton() << 2;
			info |= ph->hasConversionTracks() << 3;
			info |= ph->hasPixelSeed() << 4;
			info |= (ConversionTools::hasMatchedPromptElectron(ph->superCluster(), Electrons, Conversions, bs_position)) << 5;
			phout.Info(info);
			phout.TriggerMatching(GetTrigger(*ph, photontriggers));
			if(ph->pt() >= cPhotonPtMin && fabs(ph->eta()) <= cPhotonEtaMax)
			{
				NumGood++;
			}
		}
	}
	if(NumGood >= cPhotonNum){return true;}
	return(false);
}

bool RootMaker::AddTracks(const edm::Event& iEvent)
{
	using namespace BASEIO;
	int NumGood = 0;
        edm::Handle<TrackCollection> Tracks;
        iEvent.getByLabel(edm::InputTag("generalTracks"), Tracks);
	for(unsigned i = 0 ; i < Tracks->size() ; i++)
	{
		TrackRef trr(Tracks, i);
		if(trr->pt() >= cTrackFilterPtMin)
		{
			IOTrack trout = baseio.GetIOTrack(baseio.NumIOTracks());
			FillTrack(trout, trr);
			if(trr->pt() >= cTrackPtMin && fabs(trr->eta()) <= cTrackEtaMax)
			{
				NumGood++;
			}
		}
	}
	if(NumGood >= cTrackNum){return true;}
	return false ;
}

void RootMaker::FillTrack(BASEIO::IOTrack& trout, TrackRef& trin)
{
	trout.px(trin->px());
	trout.py(trin->py());
	trout.pz(trin->pz());
	trout.Dxy(trin->dxy(pv_position));
	trout.DxyUnc(trin->dxyError());
	trout.Dz(trin->dz(pv_position));
	trout.DzUnc(trin->dzError());
	trout.ChiQ(trin->chi2());
	trout.NDOF(trin->ndof());
	trout.Charge(trin->charge());
	trout.VertexNumber(getPrimVertex(trin));
	trout.NStripHits(trin->hitPattern().numberOfValidStripHits());
	trout.NPixelHits(trin->hitPattern().numberOfValidPixelHits());
	trout.NMissingHits(trin->hitPattern().numberOfLostHits(HitPattern::TRACK_HITS));
	trout.NMissingInnerHits(trin->hitPattern().numberOfHits(HitPattern::MISSING_INNER_HITS));
	trout.NPixelLayers(trin->hitPattern().pixelLayersWithMeasurement());
	trout.NStripLayers(trin->hitPattern().stripLayersWithMeasurement());
	//trout.DeDx((*dEdxharmonic2)[trin].dEdx());
}

bool RootMaker::AddAK5PFJets(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace BASEIO;
	int NumGood = 0;

	edm::Handle<PFJetCollection> ak5pfJets;
	iEvent.getByLabel(edm::InputTag("ak4PFJets"), ak5pfJets);
	const JetCorrector* corrector = JetCorrector::getJetCorrector((string("ak4PF")+cJetCorrection).c_str(), iSetup);

	edm::Handle<reco::JetTagCollection> bTagCSVHandle;
	iEvent.getByLabel("combinedSecondaryVertexBJetTags", bTagCSVHandle);
	const reco::JetTagCollection& bTagCSVs = *(bTagCSVHandle.product());
	edm::Handle<reco::JetTagCollection> bTagCSVv2Handle;
	iEvent.getByLabel("combinedInclusiveSecondaryVertexV2BJetTags", bTagCSVv2Handle);
	const reco::JetTagCollection& bTagCSVv2s = *(bTagCSVv2Handle.product());

	edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
	iSetup.get<JetCorrectionsRecord>().get("AK4PF",JetCorParColl);
	JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
	JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(JetCorPar);
	for(unsigned i = 0 ; i < ak5pfJets->size() ; i++)
	{
		PFJet corjet((*ak5pfJets)[i]);
		double jec = corrector->correction(corjet, iEvent, iSetup);
		corjet.scaleEnergy(jec);
		if(corjet.pt() >= cAK5PFFilterPtMin)
		{
			IOPFJet jetout(baseio.GetIOPFJet(baseio.NumIOPFJets()));

			jetout.e(corjet.energy());
			jetout.px(corjet.px());
			jetout.py(corjet.py());
			jetout.pz(corjet.pz());
			double tag = -1.;
			for(size_t b = 0; b < bTagCSVs.size(); ++b)
			{
				if(sqrt((corjet.eta() - bTagCSVs[b].first->eta())*(corjet.eta() - bTagCSVs[b].first->eta()) + (corjet.phi() - bTagCSVs[b].first->phi())*(corjet.phi() - bTagCSVs[b].first->phi())) < 0.2)
				{
					tag = bTagCSVs[b].second;
				}
			}
			jetout.BTagCSV(tag);
			tag = -1.;
			for(size_t b = 0; b < bTagCSVv2s.size(); ++b)
			{
				if(sqrt((corjet.eta() - bTagCSVv2s[b].first->eta())*(corjet.eta() - bTagCSVv2s[b].first->eta()) + (corjet.phi() - bTagCSVv2s[b].first->phi())*(corjet.phi() - bTagCSVv2s[b].first->phi())) < 0.2)
				{
					tag = bTagCSVv2s[b].second;
				}
			}
			jetout.BTagCSVv2(tag);

			jetout.TriggerMatching(GetTrigger(corjet, jettriggers));
			jetout.Area(corjet.jetArea());
			jetout.ChargedHadronEnergy(corjet.chargedHadronEnergy());
			jetout.NeutralHadronEnergy(corjet.neutralHadronEnergy());
			jetout.PhotonEnergy(corjet.photonEnergy());
			jetout.ElectronEnergy(corjet.electronEnergy());
			jetout.MuonEnergy(corjet.muonEnergy());
			jetout.ForwardHad(corjet.HFHadronEnergy());
			jetout.ForwardEM(corjet.HFEMEnergy());
			jetout.NumChargedHadrons(corjet.chargedHadronMultiplicity());
			jetout.NumNeutralHadrons(corjet.neutralHadronMultiplicity());
			jetout.NumPhotons(corjet.photonMultiplicity());
			jetout.NumElectrons(corjet.electronMultiplicity());
			jetout.NumMuons(corjet.muonMultiplicity());
			jetout.NumForwardHads(corjet.HFHadronMultiplicity());
			jetout.NumForwardEMs(corjet.HFEMMultiplicity());
			jetout.EnergyCorrection(1./jec);
			jecUnc->setJetEta(corjet.eta());
			jecUnc->setJetPt(corjet.pt());
			jetout.EnergyCorrectionUnc(jecUnc->getUncertainty(true));
			JetShape shape = getJetShape(corjet);
			jetout.Mass(shape.mass);
			jetout.ChargedPtMomPA(shape.chargeda);
			jetout.ChargedPtMomPB(shape.chargedb);
			jetout.ConstituentPtMomPA(shape.alla);
			jetout.ConstituentPtMomPB(shape.allb);
			jetout.PtFractionWrongPrimaryVertex(shape.chargedfractionmv);
			jetout.MaxChargedPtFraction(shape.chargedmaxpt);
			jetout.MaxPtFraction(shape.allmaxpt);
			if(corjet.pt() >= cAK5PFPtMin && fabs(corjet.eta()) < cAK5PFEtaMax) 
			{
				NumGood++;
			}
		}
	}
	delete jecUnc;
	if(NumGood >= cAK5PFNum){return true;}
	return(false);
}


bool RootMaker::AddElectrons(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace BASEIO;
	int NumGood = 0;
	edm::Handle<GsfElectronCollection> Electrons;
	iEvent.getByLabel(edm::InputTag("gedGsfElectrons"), Electrons);
	edm::Handle<ConversionCollection> Conversions;
	iEvent.getByLabel(edm::InputTag("allConversions"), Conversions);
	for(size_t n = 0 ; n < Electrons->size() ; n++)
	{
		GsfElectronRef el(Electrons, n);
		if(el->pt() > cElFilterPtMin && TMath::Abs(el->eta()) < cElFilterEtaMax)
		{
			IOElectron elout(baseio.GetIOElectron(baseio.NumIOElectrons()));
			elout.px(el->px());
			elout.py(el->py());
			elout.pz(el->pz());
			elout.TriggerMatching(GetTrigger(*el, electrontriggers));
			elout.ESCOverETrack(el->eSuperClusterOverP());
			elout.DeltaEtaSCTrack(el->deltaEtaSuperClusterTrackAtVtx());
			elout.DeltaPhiSCTrack(el->deltaPhiSuperClusterTrackAtVtx());
			elout.E1x5(el->e1x5());
			elout.E2x5(el->e2x5Max());
			elout.E5x5(el->e5x5());
			elout.R9(el->r9());
			elout.ECalEnergy(el->correctedEcalEnergy());
			elout.SigmaIEtaIEta(el->sigmaIetaIeta());
			elout.SigmaIPhiIPhi(el->sigmaIphiIphi());
			elout.EHCalTowerOverECalD1(el->hcalDepth1OverEcalBc());
			elout.EHCalTowerOverECalD2(el->hcalDepth2OverEcalBc());
			IOTrack eltrk(elout.GSFTrack());
			GsfTrackRef gsftrk = el->gsfTrack();
			TrackRef ctrk = el->closestTrack();
			FillTrack(eltrk, gsftrk, ctrk);
			IOSuperCluster elsc(elout.SC());
			SuperClusterRef sc = el->superCluster();
			FillSC(elsc, sc);
			PFIsolation r3(elout.PFR3());
			r3.Charged(el->pfIsolationVariables().sumChargedHadronPt);
			r3.Neutral(el->pfIsolationVariables().sumNeutralHadronEt);
			r3.Photon(el->pfIsolationVariables().sumPhotonEt);
			UInt_t info = 0;
			info |= el->isEB() << 0;
			info |= el->isEE() << 1;
			info |= el->isEBGap() << 2;
			info |= el->isEBEtaGap() << 3;
			info |= el->isEBPhiGap() << 4;
			info |= el->isEEGap() << 5;
			info |= el->isEERingGap() << 6;
			info |= el->isEEDeeGap() << 7;
			info |= el->isEBEEGap() << 8;
			info |= el->isElectron() << 9;
			info |= ConversionTools::hasMatchedConversion(*el, Conversions, bs_position) << 10;
			info |= el->ecalDrivenSeed() << 11;
			info |= el->trackerDrivenSeed() << 12;
			elout.Info(info);
			if(el->pt() >= cElPtMin && fabs(el->eta()) <= cElEtaMax)
			{
				NumGood++;
			}
		}

	}
	if(NumGood >= cElNum){return true;}
	return false;
}

void RootMaker::FillTrack(BASEIO::IOTrack& trout, GsfTrackRef& trin, TrackRef& trinc)
{
	trout.px(trin->px());
	trout.py(trin->py());
	trout.pz(trin->pz());
	trout.Dxy(trin->dxy(pv_position));
	trout.DxyUnc(trin->dxyError());
	trout.Dz(trin->dz(pv_position));
	trout.DzUnc(trin->dzError());
	trout.ChiQ(trin->chi2());
	trout.NDOF(trin->ndof());
	trout.Charge(trin->charge());
	if(trinc.isNonnull())
	{
		trout.VertexNumber(getPrimVertex(trinc));
	}
	else
	{
		trout.VertexNumber(-1);
	}
	trout.NStripHits(trin->hitPattern().numberOfValidStripHits());
	trout.NPixelHits(trin->hitPattern().numberOfValidPixelHits());
	trout.NMissingHits(trin->hitPattern().numberOfLostHits(HitPattern::TRACK_HITS));
	trout.NMissingInnerHits(trin->hitPattern().numberOfHits(HitPattern::MISSING_INNER_HITS));
	trout.NPixelLayers(trin->hitPattern().pixelLayersWithMeasurement());
	trout.NStripLayers(trin->hitPattern().stripLayersWithMeasurement());
	trout.DeDx(-1.);
}


void RootMaker::FillSC(BASEIO::IOSuperCluster& scout, SuperClusterRef& scin)
{
	scout.Energy(scin->energy());
	scout.x(scin->x());
	scout.y(scin->y());
	scout.z(scin->z());
	scout.RawEnergy(scin->rawEnergy());
	scout.PhiWidth(scin->phiWidth());
	scout.EtaWidth(scin->etaWidth());
}

UInt_t RootMaker::FindGenParticle(const GenParticleCollection& genparticles,  const Candidate* particle)
{
	for(size_t i = 0 ; i < genparticles.size() ; i++)
	{
		const GenParticle& gp(genparticles[i]);
		if(particle->pdgId() == gp.pdgId() && particle->status() == gp.status() && particle->energy() == gp.energy() && particle->px() == gp.px() && particle->py() == gp.py() && particle->pz() == gp.pz())
		{
			return(i);
		}
	}
	return(10000000);
}

pair<Int_t, Int_t> RootMaker::HasAnyMother(const GenParticle* particle, vector<int> ids)
{
	Int_t motherid = 0;
	vector<unsigned> bknummother;
	vector<const GenParticle*> bkparticle;
	bknummother.reserve(10);
	bkparticle.reserve(10);
	int level = 0;
	bkparticle.push_back(particle);
	bknummother.push_back(0);

	vector<int>::const_iterator it;
	UInt_t result = 0;
	unsigned j = 0;
	while(true)
	{
		if(j == bkparticle[level]->numberOfMothers())
		{
			level--;
			if(level == -1){break;}
			j = bknummother[level];
			bkparticle.resize(level+1);
			bknummother.resize(level+1);
			continue;
		}

		if(motherid == 0 && bkparticle[level]->mother(j)->pdgId() != particle->pdgId()){motherid = bkparticle[level]->mother(j)->pdgId();}
		it = find(ids.begin(), ids.end(), bkparticle[level]->mother(j)->pdgId());
		if(it != ids.end()){result |= 1<<(it-ids.begin());}

		if(bkparticle[level]->mother(j)->numberOfMothers() > 0)
		{
			bknummother[level] = j+1;
			bkparticle.push_back(dynamic_cast<const GenParticle*>(bkparticle[level]->mother(j)));
			bknummother.push_back(0);
			j = 0;
			level++;
			continue;
		}
		j++;
	}
	return(pair<Int_t, Int_t>(result, motherid));
}

Int_t RootMaker::HasAnyMother(const GenParticle* particle, int id)
{
	vector<unsigned> bknummother;
	vector<const GenParticle*> bkparticle;
	bknummother.reserve(10);
	bkparticle.reserve(10);
	int level = 0;
	bkparticle.push_back(particle);
	bknummother.push_back(0);

	unsigned j = 0;
	while(true)
	{
		if(j == bkparticle[level]->numberOfMothers())
		{
			level--;
			if(level == -1){return(0);}
			j = bknummother[level];
			bkparticle.resize(level+1);
			bknummother.resize(level+1);
			continue;
		}

		if(bkparticle[level]->mother(j)->pdgId() == id) return(2);
		if(abs(bkparticle[level]->mother(j)->pdgId()) == abs(id)) return(1);

		if(bkparticle[level]->mother(j)->numberOfMothers() > 0)
		{
			bknummother[level] = j+1;
			bkparticle.push_back(dynamic_cast<const GenParticle*>(bkparticle[level]->mother(j)));
			bknummother.push_back(0);
			j = 0;
			level++;
			continue;
		}
		j++;
	}
	return(0);
}

math::XYZPoint RootMaker::PositionOnECalSurface(TransientTrack& trTrack)
{
	math::XYZPoint ecalPosition(0.,0.,0.);
	const FreeTrajectoryState myTSOS = trTrack.initialFreeState();
	TrajectoryStateOnSurface stateAtECal = propagatorWithMaterial->propagate(myTSOS, *ecalBarrel);

	if(stateAtECal.isValid() && stateAtECal.globalPosition().eta() > 1.479) 
	{
		stateAtECal= propagatorWithMaterial->propagate(myTSOS, *ecalPositiveEtaEndcap);
	}    

	if(stateAtECal.isValid() && stateAtECal.globalPosition().eta() < -1.479) 
	{
		stateAtECal= propagatorWithMaterial->propagate(myTSOS, *ecalNegativeEtaEndcap);
	}    

	if(stateAtECal.isValid())
	{
		ecalPosition = stateAtECal.globalPosition();
	}
	return(ecalPosition);
}

RootMaker::JetShape RootMaker::getJetShape(const PFJet& jet)
{
	using namespace TMath;
	RootMaker::JetShape res;
	Float_t chargedetaeta1 = 0.;
	Float_t chargedphiphi1 = 0.;
	Float_t chargedetaeta2 = 0.;
	Float_t chargedphiphi2 = 0.;
	Float_t chargedetaphi = 0.;
	Float_t chargedmaxpt = 0.;
	UInt_t chargedmulti = 0;
	Float_t chargedptsum = 0.;
	Float_t chargedptsummv = 0.;
	Float_t neutraletaeta1 = 0.;
	Float_t neutralphiphi1 = 0.;
	Float_t neutraletaeta2 = 0.;
	Float_t neutralphiphi2 = 0.;
	Float_t neutraletaphi = 0.;
	Float_t neutralptsum = 0.;
	Float_t neutralmaxpt = 0.;
	Float_t alletaeta1 = 0.;
	Float_t alletaeta2 = 0.;
	Float_t alletaphi = 0.;
	Float_t allphiphi1 = 0.;
	Float_t allphiphi2 = 0.;
	Float_t allptsum = 0.;
	Float_t allmaxpt = 0.;
	TLorentzVector sum(0., 0., 0., 0.);
	vector<PFCandidatePtr> constituents(jet.getPFConstituents());
	for(size_t i = 0 ; i < constituents.size() ; ++i)
	{
		const PFCandidate& con = *(constituents[i]);
		TVector3 tvcon;
		tvcon.SetPtEtaPhi(con.pt(), con.eta(), con.phi());
		TVector3 tvjet;
		tvjet.SetPtEtaPhi(jet.pt(), jet.eta(), jet.phi());
		Float_t deta = jet.eta() - con.eta();
		Float_t dphi = tvjet.DeltaPhi(tvcon);
		sum += TLorentzVector(con.px(), con.py(), con.pz(), Sqrt(con.px()*con.px() + con.py()*con.py() + con.pz()*con.pz()));
		
		//Float_t deta = jet.eta() - con.eta();
		//Float_t dphi = jet.phi() - con.phi();
		//if(dphi > 4.*atan(1.) ) dphi = dphi-8.*atan(1.);
		//if(dphi < -1.*4.*atan(1.) ) dphi = dphi+8.*atan(1.);

		if(con.trackRef().isNonnull())
		{
			chargedptsum += con.pt();      
			int vertex = getPrimVertex(con.trackRef());
			if(vertex == 0 || vertex == -1)
			{
				if(con.pt() > chargedmaxpt) {chargedmaxpt = con.pt();}
				chargedmulti++;
				chargedetaeta1 += deta*con.pt();
				chargedetaeta2 += deta*deta*con.pt();
				chargedetaphi += deta*dphi*con.pt();
				chargedphiphi1 += dphi*con.pt();
				chargedphiphi2 += dphi*dphi*con.pt();
				chargedptsummv += con.pt();
			}
		}
		else
		{
			if(con.pt() > neutralmaxpt) {neutralmaxpt = con.pt();}
			neutralptsum += con.pt();
			neutraletaeta1 += deta*con.pt();
			neutraletaeta2 += deta*deta*con.pt();
			neutraletaphi += deta*dphi*con.pt();
			neutralphiphi1 += dphi*con.pt();
			neutralphiphi2 += dphi*dphi*con.pt();
		}
		if(con.pt() > allmaxpt) {allmaxpt = con.pt();}
		allptsum += con.pt();
		alletaeta1 += deta*con.pt();
		alletaeta2 += deta*deta*con.pt();
		alletaphi += deta*dphi*con.pt();
		allphiphi1 += dphi*con.pt();
		allphiphi2 += dphi*dphi*con.pt();
	}
	if(chargedptsum != 0)
	{
		chargedetaeta1/=chargedptsum;
		chargedetaeta2/=chargedptsum;
		chargedetaphi/=chargedptsum;
		chargedphiphi1/=chargedptsum;
		chargedphiphi2/=chargedptsum;
	}
	else
	{
		chargedetaeta1 = 0.;
		chargedetaeta2 = 0.;
		chargedetaphi = 0.;
		chargedphiphi1 = 0.;
		chargedphiphi2 = 0.;
	}
	if(neutralptsum != 0)
	{
		neutraletaeta1/=neutralptsum;
		neutraletaeta2/=neutralptsum;
		neutraletaphi/=neutralptsum;
		neutralphiphi1/=neutralptsum;
		neutralphiphi2/=neutralptsum;
	}
	else
	{
		neutraletaeta1 = 0.;
		neutraletaeta2 = 0.;
		neutraletaphi = 0.;
		neutralphiphi1 = 0.;
		neutralphiphi2 = 0.;
	}
	if(allptsum != 0)
	{
		alletaeta1/=allptsum;
		alletaeta2/=allptsum;
		alletaphi/=allptsum;
		allphiphi1/=allptsum;
		allphiphi2/=allptsum;
	}
	else
	{
		alletaeta1 = 0.;
		alletaeta2 = 0.;
		alletaphi = 0.;
		allphiphi1 = 0.;
		allphiphi2 = 0.;
	}

	Float_t chargedetavar = chargedetaeta2-chargedetaeta1*chargedetaeta1;
	Float_t chargedphivar = chargedphiphi2-chargedphiphi1*chargedphiphi1;
	Float_t chargedphidetacov = chargedetaphi - chargedetaeta1*chargedphiphi1;

	Float_t chargeddet = (chargedetavar-chargedphivar)*(chargedetavar-chargedphivar)+4*chargedphidetacov*chargedphidetacov;
	Float_t chargedx1 = (chargedetavar+chargedphivar+sqrt(chargeddet))/2.;
	Float_t chargedx2 = (chargedetavar+chargedphivar-sqrt(chargeddet))/2.;

	Float_t neutraletavar = neutraletaeta2-neutraletaeta1*neutraletaeta1;
	Float_t neutralphivar = neutralphiphi2-neutralphiphi1*neutralphiphi1;
	Float_t neutralphidetacov = neutraletaphi - neutraletaeta1*neutralphiphi1;

	Float_t neutraldet = (neutraletavar-neutralphivar)*(neutraletavar-neutralphivar)+4*neutralphidetacov*neutralphidetacov;
	Float_t neutralx1 = (neutraletavar+neutralphivar+sqrt(neutraldet))/2.;
	Float_t neutralx2 = (neutraletavar+neutralphivar-sqrt(neutraldet))/2.;

	Float_t alletavar = alletaeta2-alletaeta1*alletaeta1;
	Float_t allphivar = allphiphi2-allphiphi1*allphiphi1;
	Float_t allphidetacov = alletaphi - alletaeta1*allphiphi1;

	Float_t alldet = (alletavar-allphivar)*(alletavar-allphivar)+4*allphidetacov*allphidetacov;
	Float_t allx1 = (alletavar+allphivar+sqrt(alldet))/2.;
	Float_t allx2 = (alletavar+allphivar-sqrt(alldet))/2.;

	res.chargeda = chargedx1;
	res.chargedb = chargedx2;
	res.neutrala = neutralx1;
	res.neutralb = neutralx2;
	res.alla = allx1;
	res.allb = allx2;
	res.chargedfractionmv = chargedptsummv/chargedptsum;
	res.chargedmulti = chargedmulti;
	res.chargedmaxpt = chargedmaxpt;
	res.neutralmaxpt = neutralmaxpt;
	res.allmaxpt = allmaxpt;
	res.mass = sum.M();
	return(res);

}



Int_t RootMaker::getPrimVertex(const TrackRef& trk)
{
	if(Vertices.isValid())
	{
		for(unsigned i = 0 ; i < Vertices->size(); i++)
		{
			if((*Vertices)[i].isValid() && !(*Vertices)[i].isFake())
			{
				for(Vertex::trackRef_iterator it = (*Vertices)[i].tracks_begin() ; it != (*Vertices)[i].tracks_end() ; ++it)
				{
					if(trk->px() - (*it)->px() < 0.01 && trk->py() - (*it)->py() < 0.01 && trk->pz() - (*it)->pz() < 0.01) {return i;}
				}
			}
		}
	}
	return(-1);
}

double RootMaker::DR(const Candidate& A, const Candidate& B)
{
	//using namespace TMath;
	TLorentzVector TA(A.px(), A.py(), A.pz(), A.energy());
	TLorentzVector TB(B.px(), B.py(), B.pz(), B.energy());
	return(TA.DeltaR(TB));
}

UInt_t RootMaker::GetTrigger(const LeafCandidate& particle, vector<pair<unsigned, int> >& triggers)
{
	UInt_t result = 0;

	if(HLTrigger.isValid() && HLTriggerEvent.isValid())
	{
		const trigger::TriggerObjectCollection& TOC(HLTriggerEvent->getObjects());
		for(unsigned n = 0 ; n < triggers.size() ; n++)
		{
			unsigned TriggerIndex = triggers[n].first;
			int Filter = abs(triggers[n].second);
			const vector<string>& ModuleLabels(HLTConfiguration.moduleLabels(TriggerIndex));
			const unsigned FilterIndex = HLTriggerEvent->filterIndex(edm::InputTag(ModuleLabels[Filter], "", cTriggerProcess));
			if(FilterIndex < HLTriggerEvent->sizeFilters())
			{
				const trigger::Keys& KEYS(HLTriggerEvent->filterKeys(FilterIndex));
				for(unsigned j = 0 ; j < KEYS.size() ; j++)
				{
					TLorentzVector TA(particle.px(), particle.py(), particle.pz(), particle.energy());
					TLorentzVector TB(TOC[KEYS[j]].px(), TOC[KEYS[j]].py(), TOC[KEYS[j]].pz(), TOC[KEYS[j]].energy());
					double dr = TA.DeltaR(TB);
					//double dpt = TMath::Abs(particle.pt() - TOC[KEYS[j]].pt())/particle.pt();  
					if(dr < 0.3)
					{
						if(triggers[n].second >= 0)
						{
							result |= 1<<n;
							break;
						}
						if(triggers[n].second < 0 && TOC[KEYS[j]].pt() > 10.)
						{
							result |= 1<<n;
							break;
						}

					}
				}
			}
		}
	}
	return(result);
}

void RootMaker::TriggerIndexSelection(vector<string> configstring, vector<pair<unsigned, int> >& triggers, string& allnames)
{
	triggers.clear();
	allnames.clear();
	boost::cmatch what;
	vector<pair<boost::regex, bool> > regexes;
	for(unsigned i = 0 ; i < configstring.size() ; i++)
	{
		vector<string> strs;
		boost::split(strs, configstring[i], boost::is_any_of(":"));
		bool dofilter = false;
		if(strs.size() == 2 && strs[1] == "FilterTrue") dofilter = true;
		regexes.push_back(pair<boost::regex, bool>(boost::regex(strs[0].c_str()), dofilter));
	}

	for(unsigned i = 0 ; i < HLTConfiguration.size() ; i++)
	{
		unsigned TriggerIndex = HLTConfiguration.triggerIndex(HLTConfiguration.triggerName(i));
		const vector<string>& ModuleLabels(HLTConfiguration.moduleLabels(TriggerIndex));
		for(unsigned j = 0 ; j < regexes.size() ; j++)
		{
			if(boost::regex_match(HLTConfiguration.triggerName(i).c_str(), what, regexes[j].first) && triggers.size() < 32)
			{
				for(int u = ModuleLabels.size()-1 ; u >= 0 ; u--)
				{
					if(HLTConfiguration.saveTags(ModuleLabels[u])) 
					{
						allnames += HLTConfiguration.triggerName(i) + string(":") + ModuleLabels[u] + string(" ");
						triggers.push_back(pair<unsigned, int>(TriggerIndex, u));
						cout << triggers.size() << ": " << HLTConfiguration.triggerName(i) << " " << TriggerIndex << " " << ModuleLabels[u] << " " << u << endl;
						if("hltL1sL1DoubleMu10MuOpen" == ModuleLabels[u])
						{
							allnames += HLTConfiguration.triggerName(i) + string(":") + ModuleLabels[u] + string("gt10 ");
							triggers.push_back(pair<unsigned, int>(TriggerIndex, -1*u));
						}
						if(regexes[j].second == false)
						{
							break;
						}
					}
				}
			}
		}
	}
	if(triggers.size() == 32) {cout << "ERROR: more than 32 triggers to match" << endl;}
}

