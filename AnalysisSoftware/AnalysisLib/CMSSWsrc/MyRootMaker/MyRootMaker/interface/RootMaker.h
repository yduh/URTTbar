#ifndef RootMaker_h
#define RootMaker_h
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <algorithm>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "DataFormats/Luminosity/interface/LumiSummary.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"

//#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
//#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"
//#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
//#include "DataFormats/L1Trigger/interface/L1EmParticleFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
//#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "CondFormats/L1TObjects/interface/L1GtPrescaleFactors.h"
#include "CondFormats/DataRecord/interface/L1GtPrescaleFactorsAlgoTrigRcd.h"
#include "CondFormats/DataRecord/interface/L1GtPrescaleFactorsTechTrigRcd.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "DataFormats/CaloRecHit/interface/CaloCluster.h" 
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h" 
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "DataFormats/TrackReco/interface/DeDxData.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METFwd.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/JetCollection.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETFwd.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"

//#include "DataFormats/PatCandidates/interface/Muon.h"
//#include "DataFormats/PatCandidates/interface/Electron.h"
//#include "DataFormats/PatCandidates/interface/Photon.h"
//#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "RecoEgamma/EgammaTools/interface/ConversionInfo.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"

#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include <Math/Functions.h>
#include <Math/SVector.h>
#include <Math/SMatrix.h>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/GeometrySurface/interface/SimpleCylinderBounds.h"
#include "DataFormats/GeometrySurface/interface/SimpleDiskBounds.h"
#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include "DataFormats/GeometrySurface/interface/Plane.h"
#include "DataFormats/GeometrySurface/interface/BoundCylinder.h"
#include "DataFormats/GeometrySurface/interface/BoundDisk.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/MaterialEffects/interface/PropagatorWithMaterial.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

#include "BASEIO.h"
#include "iohelper.h"
#include "BASEIORUN.h"
#include "BASEIOLUMI.h"

//#include "EGamma/EGammaAnalysisTools/src/PFIsolationEstimator.cc"
//#include "PFIsolation/SuperClusterFootprintRemoval/interface/SuperClusterFootprintRemoval.h"

using namespace std;
using namespace reco;

class RootMaker : public edm::EDAnalyzer{ 
	public:
		explicit RootMaker( const edm::ParameterSet& iConfig );
		~RootMaker();
	private:
		virtual void beginJob();
		virtual void endJob();
		virtual void beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup);
		void TriggerIndexSelection(vector<string> configstring, vector<pair<unsigned, int> >& triggers, string& allnames);
		virtual void beginLuminosityBlock(const edm::LuminosityBlock& iLumiBlock, const edm::EventSetup& iSetup);
		virtual void endLuminosityBlock(const edm::LuminosityBlock& iLumiBlock, const edm::EventSetup& iSetup);
		virtual void analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup );


		edm::Handle<edm::ValueMap<DeDxData> > dEdxharmonic2;
		bool AddTracks(const edm::Event& iEvent);
		void FillTrack(BASEIO::IOTrack& trout, TrackRef& trin);
		void FillTrack(BASEIO::IOTrack& trout, GsfTrackRef& trin, TrackRef& trinc);
		bool AddElectrons(const edm::Event& iEvent, const edm::EventSetup& iSetup);
		bool AddMuons(const edm::Event& iEvent);
		bool AddPhotons(const edm::Event& iEvent, const edm::EventSetup& iSetup);
		bool AddAK5PFJets(const edm::Event& iEvent, const edm::EventSetup& iSetup);
		void FillSC(BASEIO::IOSuperCluster& scout, SuperClusterRef& scin);

		struct JetShape
		{
			Float_t chargeda;
			Float_t chargedb;
			Float_t neutrala;
			Float_t neutralb;
			Float_t alla;
			Float_t allb;
			Float_t chargedfractionmv;
			UInt_t chargedmulti;
			Float_t chargedmaxpt;
			Float_t neutralmaxpt;
			Float_t allmaxpt;
			Float_t mass;
		};
		JetShape getJetShape(const PFJet& jet);
		UInt_t GenParticleInfo(const GenParticle* particle);
		UInt_t GetTrigger(const LeafCandidate& particle, vector<pair<unsigned, int> >& triggers);
		UInt_t FindGenParticle(const GenParticleCollection& genparticles,  const Candidate* particle);
		Int_t HasAnyMother(const GenParticle* particle, int id);
		pair<Int_t, Int_t> HasAnyMother(const GenParticle* particle, vector<int> ids);
		math::XYZPoint PositionOnECalSurface(reco::TransientTrack&);
		Int_t getSuperClusterPh(const SuperClusterRef& A);
		Int_t getSuperClusterEl(const SuperClusterRef& A);
		Int_t getPrimVertex(const TrackRef& trk);
		//Int_t getSuperCluster(const Candidate& A);

		BASEIO::BaseIO baseio;
		BASEIORUN::BaseIO baseiorun;
		BASEIOLUMI::BaseIO baseiolumi;
		TH1D* histmu;
		TH1D* histpum;
		TH1D* histpu;
		TH1D* histpup;
		//Configuration
		bool cgen;
		bool cgenallparticles;
		bool cgenak5jets;
		bool ctrigger;
		bool cbeamspot;
		bool crectrack;
		bool crecprimvertex;
		bool crecsupercluster;
		double crecsuperclusterFilterPtMin;
		double crecsuperclusterFilterEtaMax;
		bool crecsuperclustermember;
		bool crecsuperclusterhit;
		bool crecmuon;
		bool crectau;
		bool crecelectron;
		bool crecphoton;
		bool crecallconversion;
		bool crecak5calojet;
		bool crecak5jptjet;
		bool crecak5pfjet;
		bool crecak5pfchsjet;
		bool crecjettrigger;
		bool crecpfmet;
		bool crecsecvertices;
		bool crecmusecvertices;
		vector<string> cHLTriggerNamesSelection;
		string cTriggerProcess;

		double cMuPtMin;
		double cMuTrackIso;
		double cMuEtaMax;
		vector<string> cMuHLTriggerMatching;
		int cMuNum;
		double cElPtMin;
		double cElTrackIso;
		double cElEtaMax;
		vector<string> cElHLTriggerMatching;
		int cElNum;
		double cElFilterPtMin;
		double cElFilterEtaMax;
		double cTauPtMin;
		double cTauEtaMax;
		vector<string> cTauHLTriggerMatching;
		vector<string> cTauDiscriminators;
		int cTauNum;
		double cTrackFilterPtMin;
		double cTrackPtMin;
		double cTrackEtaMax;
		int cTrackNum;
		double cPhotonPtMin;
		double cPhotonEtaMax;
		vector<string> cPhotonHLTriggerMatching;
		int cPhotonNum;
		double cPhotonFilterPtMin;
		double cPhotonFilterEtaMax;
		double cAK5CaloFilterPtMin;
		double cAK5CaloPtMin;
		double cAK5CaloEtaMax;
		int cAK5CaloNum;
		double cAK5JPTFilterPtMin;
		double cAK5JPTPtMin;
		double cAK5JPTEtaMax;
		int cAK5JPTNum;
		double cAK5PFCHSFilterPtMin;
		double cAK5PFCHSPtMin;
		double cAK5PFCHSEtaMax;
		int cAK5PFCHSNum;
		double cAK5PFFilterPtMin;
		double cAK5PFPtMin;
		double cAK5PFEtaMax;
		int cAK5PFNum;
		string cJetCorrection;
		vector<string> cJetHLTriggerMatching;

		double cMassMuMuMin;
		double cMassMuMuMax;		
		vector<TLorentzVector> MuVector;
		vector<TLorentzVector> TrackVector;

		double cVertexTRKChi2;
		int cVertexTRKHitsMin;
		double cVertexChi2;
		double cVertexSig2D;
		double cKaonMassWindow;
		double cLambdaMassWindow;

		//Variables
	
		UInt_t lumi_eventsprocessed;
		UInt_t lumi_eventsfiltered;
		UInt_t lumi_hltprescaletable;
	
		edm::ESHandle<TransientTrackBuilder> TTrackBuilder;
		edm::ESHandle<MagneticField> magneticField; 
		Cylinder::ConstCylinderPointer ecalBarrel;
		Plane::ConstPlanePointer ecalNegativeEtaEndcap;
		Plane::ConstPlanePointer ecalPositiveEtaEndcap;
		PropagatorWithMaterial* propagatorWithMaterial;

		HLTConfigProvider HLTConfiguration;
		edm::Handle<edm::TriggerResults> HLTrigger;
		edm::Handle<trigger::TriggerEvent> HLTriggerEvent;
		vector<pair<unsigned, int> > muontriggers;
		vector<pair<unsigned, int> > electrontriggers;
		vector<pair<unsigned, int> > tautriggers;
		vector<pair<unsigned, int> > photontriggers;
		vector<pair<unsigned, int> > jettriggers;
		vector<unsigned> HLTriggerIndexSelection;

		vector<string> bdisclabel;

		vector<int> testids;

		double DR(const Candidate& A, const Candidate& B);

		edm::Handle<VertexCollection> Vertices;
		math::XYZPoint pv_position;
		Vertex primvertex;
		math::XYZPoint bs_position;
};

DEFINE_FWK_MODULE(RootMaker);

#endif

