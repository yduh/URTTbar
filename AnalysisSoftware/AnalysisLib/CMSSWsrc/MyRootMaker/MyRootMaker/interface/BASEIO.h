#ifndef CLASS_BASEIO
#define CLASS_BASEIO
#include "TTree.h"
#include "TFile.h"
#include <string>
#include <iostream>
using namespace std;
namespace BASEIO{
class BaseIO;
class Data_IOMuon;
class IOMuon;
class Data_PrimaryVertex;
class PrimaryVertex;
class Data_IOEventInfo;
class IOEventInfo;
class Data_IOPFJet;
class IOPFJet;
class Data_IOString;
class IOString;
class Data_GenInfo;
class GenInfo;
class Data_IOMET;
class IOMET;
class Data_IOSuperCluster;
class IOSuperCluster;
class Data_PFIsolation;
class PFIsolation;
class Data_SelectedGenParticle;
class SelectedGenParticle;
class Data_AllGenParticle;
class AllGenParticle;
class Data_IOElectron;
class IOElectron;
class Data_IOTrack;
class IOTrack;
class Data_IOBeamSpot;
class IOBeamSpot;
class Data_IOPhoton;
class IOPhoton;

class Data_IOMuon
 {
	friend class IOMuon;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		Int_t* VertexNumber_;
		Int_t* NumChambers_;
		Int_t* NumChambersWithSegments_;
		Int_t* NumValidMuonHits_;
		Int_t* NumMatchedStations_;
		UInt_t* TriggerMatching_;
		UInt_t* Info_;
		Data_PFIsolation* PFR4_;
		Float_t* px_;
		Float_t* py_;
		Float_t* pz_;
		Float_t* PtUnc_;
		Float_t* ChiQ_;
		Float_t* NDOF_;
		Float_t* ECalEnergy_;
		Float_t* HCalEnergy_;
		UInt_t* InnerTrack_num_;
		Data_IOTrack* InnerTrack_;
	public:
		void Fill();
		Data_IOMuon(UInt_t size, std::string prefix);
		~Data_IOMuon();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class IOMuon
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_IOMuon* data_;
	public:
		IOMuon(Data_IOMuon* data, UInt_t number);
		IOMuon(const IOMuon& _iomuon);
		void Init();
		Int_t VertexNumber() const;
		Int_t NumChambers() const;
		Int_t NumChambersWithSegments() const;
		Int_t NumValidMuonHits() const;
		Int_t NumMatchedStations() const;
		UInt_t TriggerMatching() const;
		UInt_t Info() const;
		PFIsolation PFR4() const;
		Float_t px() const;
		Float_t py() const;
		Float_t pz() const;
		Float_t PtUnc() const;
		Float_t ChiQ() const;
		Float_t NDOF() const;
		Float_t ECalEnergy() const;
		Float_t HCalEnergy() const;
		IOTrack InnerTrack(UInt_t n) const;
		UInt_t Num_InnerTrack() const;
		void VertexNumber(Int_t _VertexNumber);
		void NumChambers(Int_t _NumChambers);
		void NumChambersWithSegments(Int_t _NumChambersWithSegments);
		void NumValidMuonHits(Int_t _NumValidMuonHits);
		void NumMatchedStations(Int_t _NumMatchedStations);
		void TriggerMatching(UInt_t _TriggerMatching);
		void Info(UInt_t _Info);
		void px(Float_t _px);
		void py(Float_t _py);
		void pz(Float_t _pz);
		void PtUnc(Float_t _PtUnc);
		void ChiQ(Float_t _ChiQ);
		void NDOF(Float_t _NDOF);
		void ECalEnergy(Float_t _ECalEnergy);
		void HCalEnergy(Float_t _HCalEnergy);
 };



class Data_PrimaryVertex
 {
	friend class PrimaryVertex;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		UChar_t* NumTracks_;
		Float_t* Vx_;
		Float_t* Vy_;
		Float_t* Vz_;
		Float_t* ChiQ_;
		Float_t* NDOF_;
		Float_t* SumPtQ_;
	public:
		void Fill();
		Data_PrimaryVertex(UInt_t size, std::string prefix);
		~Data_PrimaryVertex();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class PrimaryVertex
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_PrimaryVertex* data_;
	public:
		PrimaryVertex(Data_PrimaryVertex* data, UInt_t number);
		PrimaryVertex(const PrimaryVertex& _primaryvertex);
		void Init();
		UChar_t NumTracks() const;
		Float_t Vx() const;
		Float_t Vy() const;
		Float_t Vz() const;
		Float_t ChiQ() const;
		Float_t NDOF() const;
		Float_t SumPtQ() const;
		void NumTracks(UChar_t _NumTracks);
		void Vx(Float_t _Vx);
		void Vy(Float_t _Vy);
		void Vz(Float_t _Vz);
		void ChiQ(Float_t _ChiQ);
		void NDOF(Float_t _NDOF);
		void SumPtQ(Float_t _SumPtQ);
 };



class Data_IOEventInfo
 {
	friend class IOEventInfo;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		UInt_t* EventNumber_;
		UInt_t* LumiSectionNumber_;
		UInt_t* RunNumber_;
		UInt_t* TimeUnix_;
		UInt_t* TimeMuSec_;
		Float_t* AK5PFRho_;
		Float_t* AK5PFSigma_;
		UInt_t TriggerHLT_count_;
		UInt_t TriggerHLT_countmax_;
		UInt_t* TriggerHLT_num_;
		UChar_t* TriggerHLT_;
	public:
		void Fill();
		Data_IOEventInfo(UInt_t size, std::string prefix);
		~Data_IOEventInfo();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class IOEventInfo
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_IOEventInfo* data_;
	public:
		IOEventInfo(Data_IOEventInfo* data, UInt_t number);
		IOEventInfo(const IOEventInfo& _ioeventinfo);
		void Init();
		UInt_t EventNumber() const;
		UInt_t LumiSectionNumber() const;
		UInt_t RunNumber() const;
		UInt_t TimeUnix() const;
		UInt_t TimeMuSec() const;
		Float_t AK5PFRho() const;
		Float_t AK5PFSigma() const;
		UChar_t TriggerHLT(UInt_t n) const;
		UInt_t Num_TriggerHLT() const;
		void EventNumber(UInt_t _EventNumber);
		void LumiSectionNumber(UInt_t _LumiSectionNumber);
		void RunNumber(UInt_t _RunNumber);
		void TimeUnix(UInt_t _TimeUnix);
		void TimeMuSec(UInt_t _TimeMuSec);
		void AK5PFRho(Float_t _AK5PFRho);
		void AK5PFSigma(Float_t _AK5PFSigma);
		void TriggerHLT(UChar_t _TriggerHLT, UInt_t n);
 };



class Data_IOPFJet
 {
	friend class IOPFJet;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		UInt_t* TriggerMatching_;
		Int_t* NumChargedHadrons_;
		Int_t* NumNeutralHadrons_;
		Int_t* NumPhotons_;
		Int_t* NumElectrons_;
		Int_t* NumMuons_;
		Int_t* NumForwardEMs_;
		Int_t* NumForwardHads_;
		Float_t* px_;
		Float_t* py_;
		Float_t* pz_;
		Float_t* e_;
		Float_t* Area_;
		Float_t* Mass_;
		Float_t* ChargedHadronEnergy_;
		Float_t* NeutralHadronEnergy_;
		Float_t* PhotonEnergy_;
		Float_t* ElectronEnergy_;
		Float_t* MuonEnergy_;
		Float_t* ForwardEM_;
		Float_t* ForwardHad_;
		Float_t* ChargedPtMomPA_;
		Float_t* ChargedPtMomPB_;
		Float_t* ConstituentPtMomPA_;
		Float_t* ConstituentPtMomPB_;
		Float_t* PtFractionWrongPrimaryVertex_;
		Float_t* MaxChargedPtFraction_;
		Float_t* MaxPtFraction_;
		Float_t* EnergyCorrection_;
		Float_t* EnergyCorrectionUnc_;
		Float_t* BTagCSV_;
		Float_t* BTagCSVv2_;
	public:
		void Fill();
		Data_IOPFJet(UInt_t size, std::string prefix);
		~Data_IOPFJet();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class IOPFJet
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_IOPFJet* data_;
	public:
		IOPFJet(Data_IOPFJet* data, UInt_t number);
		IOPFJet(const IOPFJet& _iopfjet);
		void Init();
		UInt_t TriggerMatching() const;
		Int_t NumChargedHadrons() const;
		Int_t NumNeutralHadrons() const;
		Int_t NumPhotons() const;
		Int_t NumElectrons() const;
		Int_t NumMuons() const;
		Int_t NumForwardEMs() const;
		Int_t NumForwardHads() const;
		Float_t px() const;
		Float_t py() const;
		Float_t pz() const;
		Float_t e() const;
		Float_t Area() const;
		Float_t Mass() const;
		Float_t ChargedHadronEnergy() const;
		Float_t NeutralHadronEnergy() const;
		Float_t PhotonEnergy() const;
		Float_t ElectronEnergy() const;
		Float_t MuonEnergy() const;
		Float_t ForwardEM() const;
		Float_t ForwardHad() const;
		Float_t ChargedPtMomPA() const;
		Float_t ChargedPtMomPB() const;
		Float_t ConstituentPtMomPA() const;
		Float_t ConstituentPtMomPB() const;
		Float_t PtFractionWrongPrimaryVertex() const;
		Float_t MaxChargedPtFraction() const;
		Float_t MaxPtFraction() const;
		Float_t EnergyCorrection() const;
		Float_t EnergyCorrectionUnc() const;
		Float_t BTagCSV() const;
		Float_t BTagCSVv2() const;
		void TriggerMatching(UInt_t _TriggerMatching);
		void NumChargedHadrons(Int_t _NumChargedHadrons);
		void NumNeutralHadrons(Int_t _NumNeutralHadrons);
		void NumPhotons(Int_t _NumPhotons);
		void NumElectrons(Int_t _NumElectrons);
		void NumMuons(Int_t _NumMuons);
		void NumForwardEMs(Int_t _NumForwardEMs);
		void NumForwardHads(Int_t _NumForwardHads);
		void px(Float_t _px);
		void py(Float_t _py);
		void pz(Float_t _pz);
		void e(Float_t _e);
		void Area(Float_t _Area);
		void Mass(Float_t _Mass);
		void ChargedHadronEnergy(Float_t _ChargedHadronEnergy);
		void NeutralHadronEnergy(Float_t _NeutralHadronEnergy);
		void PhotonEnergy(Float_t _PhotonEnergy);
		void ElectronEnergy(Float_t _ElectronEnergy);
		void MuonEnergy(Float_t _MuonEnergy);
		void ForwardEM(Float_t _ForwardEM);
		void ForwardHad(Float_t _ForwardHad);
		void ChargedPtMomPA(Float_t _ChargedPtMomPA);
		void ChargedPtMomPB(Float_t _ChargedPtMomPB);
		void ConstituentPtMomPA(Float_t _ConstituentPtMomPA);
		void ConstituentPtMomPB(Float_t _ConstituentPtMomPB);
		void PtFractionWrongPrimaryVertex(Float_t _PtFractionWrongPrimaryVertex);
		void MaxChargedPtFraction(Float_t _MaxChargedPtFraction);
		void MaxPtFraction(Float_t _MaxPtFraction);
		void EnergyCorrection(Float_t _EnergyCorrection);
		void EnergyCorrectionUnc(Float_t _EnergyCorrectionUnc);
		void BTagCSV(Float_t _BTagCSV);
		void BTagCSVv2(Float_t _BTagCSVv2);
 };



class Data_IOString
 {
	friend class IOString;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		UInt_t str_count_;
		UInt_t str_countmax_;
		UInt_t* str_num_;
		Char_t* str_;
	public:
		void Fill();
		Data_IOString(UInt_t size, std::string prefix);
		~Data_IOString();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class IOString
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_IOString* data_;
	public:
		IOString(Data_IOString* data, UInt_t number);
		IOString(const IOString& _iostring);
		void Init();
		Char_t str(UInt_t n) const;
		UInt_t Num_str() const;
		void str(Char_t _str, UInt_t n);
 };



class Data_GenInfo
 {
	friend class GenInfo;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		Int_t* PDGID1_;
		Int_t* PDGID2_;
		Int_t* NumPUInteractions_;
		Int_t* NumPUInteractionsBefore_;
		Int_t* NumPUInteractionsAfter_;
		Float_t* Weight_;
		Float_t* x1_;
		Float_t* x2_;
		Float_t* RenScale_;
		Float_t* FacScale_;
		Float_t* METx_;
		Float_t* METy_;
		Float_t* NumTrueInteractions_;
	public:
		void Fill();
		Data_GenInfo(UInt_t size, std::string prefix);
		~Data_GenInfo();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class GenInfo
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_GenInfo* data_;
	public:
		GenInfo(Data_GenInfo* data, UInt_t number);
		GenInfo(const GenInfo& _geninfo);
		void Init();
		Int_t PDGID1() const;
		Int_t PDGID2() const;
		Int_t NumPUInteractions() const;
		Int_t NumPUInteractionsBefore() const;
		Int_t NumPUInteractionsAfter() const;
		Float_t Weight() const;
		Float_t x1() const;
		Float_t x2() const;
		Float_t RenScale() const;
		Float_t FacScale() const;
		Float_t METx() const;
		Float_t METy() const;
		Float_t NumTrueInteractions() const;
		void PDGID1(Int_t _PDGID1);
		void PDGID2(Int_t _PDGID2);
		void NumPUInteractions(Int_t _NumPUInteractions);
		void NumPUInteractionsBefore(Int_t _NumPUInteractionsBefore);
		void NumPUInteractionsAfter(Int_t _NumPUInteractionsAfter);
		void Weight(Float_t _Weight);
		void x1(Float_t _x1);
		void x2(Float_t _x2);
		void RenScale(Float_t _RenScale);
		void FacScale(Float_t _FacScale);
		void METx(Float_t _METx);
		void METy(Float_t _METy);
		void NumTrueInteractions(Float_t _NumTrueInteractions);
 };



class Data_IOMET
 {
	friend class IOMET;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		Float_t* px_;
		Float_t* py_;
		Float_t* pxUnc_;
		Float_t* pyUnc_;
		Float_t* pxpyUnc_;
	public:
		void Fill();
		Data_IOMET(UInt_t size, std::string prefix);
		~Data_IOMET();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class IOMET
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_IOMET* data_;
	public:
		IOMET(Data_IOMET* data, UInt_t number);
		IOMET(const IOMET& _iomet);
		void Init();
		Float_t px() const;
		Float_t py() const;
		Float_t pxUnc() const;
		Float_t pyUnc() const;
		Float_t pxpyUnc() const;
		void px(Float_t _px);
		void py(Float_t _py);
		void pxUnc(Float_t _pxUnc);
		void pyUnc(Float_t _pyUnc);
		void pxpyUnc(Float_t _pxpyUnc);
 };



class Data_IOSuperCluster
 {
	friend class IOSuperCluster;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		Float_t* x_;
		Float_t* y_;
		Float_t* z_;
		Float_t* Energy_;
		Float_t* RawEnergy_;
		Float_t* PhiWidth_;
		Float_t* EtaWidth_;
	public:
		void Fill();
		Data_IOSuperCluster(UInt_t size, std::string prefix);
		~Data_IOSuperCluster();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class IOSuperCluster
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_IOSuperCluster* data_;
	public:
		IOSuperCluster(Data_IOSuperCluster* data, UInt_t number);
		IOSuperCluster(const IOSuperCluster& _iosupercluster);
		void Init();
		Float_t x() const;
		Float_t y() const;
		Float_t z() const;
		Float_t Energy() const;
		Float_t RawEnergy() const;
		Float_t PhiWidth() const;
		Float_t EtaWidth() const;
		void x(Float_t _x);
		void y(Float_t _y);
		void z(Float_t _z);
		void Energy(Float_t _Energy);
		void RawEnergy(Float_t _RawEnergy);
		void PhiWidth(Float_t _PhiWidth);
		void EtaWidth(Float_t _EtaWidth);
 };



class Data_PFIsolation
 {
	friend class PFIsolation;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		Float_t* Charged_;
		Float_t* Neutral_;
		Float_t* Hadron_;
		Float_t* Photon_;
	public:
		void Fill();
		Data_PFIsolation(UInt_t size, std::string prefix);
		~Data_PFIsolation();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class PFIsolation
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_PFIsolation* data_;
	public:
		PFIsolation(Data_PFIsolation* data, UInt_t number);
		PFIsolation(const PFIsolation& _pfisolation);
		void Init();
		Float_t Charged() const;
		Float_t Neutral() const;
		Float_t Hadron() const;
		Float_t Photon() const;
		void Charged(Float_t _Charged);
		void Neutral(Float_t _Neutral);
		void Hadron(Float_t _Hadron);
		void Photon(Float_t _Photon);
 };



class Data_SelectedGenParticle
 {
	friend class SelectedGenParticle;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		Int_t* PDGID_;
		Int_t* Status_;
		Int_t* Info_;
		Int_t* IndirectMother_;
		Int_t* Mother_;
		Float_t* px_;
		Float_t* py_;
		Float_t* pz_;
		Float_t* e_;
	public:
		void Fill();
		Data_SelectedGenParticle(UInt_t size, std::string prefix);
		~Data_SelectedGenParticle();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class SelectedGenParticle
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_SelectedGenParticle* data_;
	public:
		SelectedGenParticle(Data_SelectedGenParticle* data, UInt_t number);
		SelectedGenParticle(const SelectedGenParticle& _selectedgenparticle);
		void Init();
		Int_t PDGID() const;
		Int_t Status() const;
		Int_t Info() const;
		Int_t IndirectMother() const;
		Int_t Mother() const;
		Float_t px() const;
		Float_t py() const;
		Float_t pz() const;
		Float_t e() const;
		void PDGID(Int_t _PDGID);
		void Status(Int_t _Status);
		void Info(Int_t _Info);
		void IndirectMother(Int_t _IndirectMother);
		void Mother(Int_t _Mother);
		void px(Float_t _px);
		void py(Float_t _py);
		void pz(Float_t _pz);
		void e(Float_t _e);
 };



class Data_AllGenParticle
 {
	friend class AllGenParticle;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		Int_t* PDGID_;
		Int_t* Status_;
		Float_t* px_;
		Float_t* py_;
		Float_t* pz_;
		Float_t* e_;
		Float_t* vx_;
		Float_t* vy_;
		Float_t* vz_;
		UInt_t Mother_count_;
		UInt_t Mother_countmax_;
		UInt_t* Mother_num_;
		Int_t* Mother_;
		UInt_t Daughter_count_;
		UInt_t Daughter_countmax_;
		UInt_t* Daughter_num_;
		Int_t* Daughter_;
	public:
		void Fill();
		Data_AllGenParticle(UInt_t size, std::string prefix);
		~Data_AllGenParticle();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class AllGenParticle
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_AllGenParticle* data_;
	public:
		AllGenParticle(Data_AllGenParticle* data, UInt_t number);
		AllGenParticle(const AllGenParticle& _allgenparticle);
		void Init();
		Int_t PDGID() const;
		Int_t Status() const;
		Float_t px() const;
		Float_t py() const;
		Float_t pz() const;
		Float_t e() const;
		Float_t vx() const;
		Float_t vy() const;
		Float_t vz() const;
		Int_t Mother(UInt_t n) const;
		UInt_t Num_Mother() const;
		Int_t Daughter(UInt_t n) const;
		UInt_t Num_Daughter() const;
		void PDGID(Int_t _PDGID);
		void Status(Int_t _Status);
		void px(Float_t _px);
		void py(Float_t _py);
		void pz(Float_t _pz);
		void e(Float_t _e);
		void vx(Float_t _vx);
		void vy(Float_t _vy);
		void vz(Float_t _vz);
		void Mother(Int_t _Mother, UInt_t n);
		void Daughter(Int_t _Daughter, UInt_t n);
 };



class Data_IOElectron
 {
	friend class IOElectron;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		UInt_t* TriggerMatching_;
		UInt_t* Info_;
		Data_PFIsolation* PFR3_;
		Data_IOSuperCluster* SC_;
		Data_IOTrack* GSFTrack_;
		Float_t* px_;
		Float_t* py_;
		Float_t* pz_;
		Float_t* DeltaEtaSCTrack_;
		Float_t* DeltaPhiSCTrack_;
		Float_t* ESCOverETrack_;
		Float_t* ECalEnergy_;
		Float_t* E1x5_;
		Float_t* E2x5_;
		Float_t* E5x5_;
		Float_t* R9_;
		Float_t* SigmaIEtaIEta_;
		Float_t* SigmaIPhiIPhi_;
		Float_t* SigmaIEtaIPhi_;
		Float_t* EHCalTowerOverECalD1_;
		Float_t* EHCalTowerOverECalD2_;
	public:
		void Fill();
		Data_IOElectron(UInt_t size, std::string prefix);
		~Data_IOElectron();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class IOElectron
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_IOElectron* data_;
	public:
		IOElectron(Data_IOElectron* data, UInt_t number);
		IOElectron(const IOElectron& _ioelectron);
		void Init();
		UInt_t TriggerMatching() const;
		UInt_t Info() const;
		PFIsolation PFR3() const;
		IOSuperCluster SC() const;
		IOTrack GSFTrack() const;
		Float_t px() const;
		Float_t py() const;
		Float_t pz() const;
		Float_t DeltaEtaSCTrack() const;
		Float_t DeltaPhiSCTrack() const;
		Float_t ESCOverETrack() const;
		Float_t ECalEnergy() const;
		Float_t E1x5() const;
		Float_t E2x5() const;
		Float_t E5x5() const;
		Float_t R9() const;
		Float_t SigmaIEtaIEta() const;
		Float_t SigmaIPhiIPhi() const;
		Float_t SigmaIEtaIPhi() const;
		Float_t EHCalTowerOverECalD1() const;
		Float_t EHCalTowerOverECalD2() const;
		void TriggerMatching(UInt_t _TriggerMatching);
		void Info(UInt_t _Info);
		void px(Float_t _px);
		void py(Float_t _py);
		void pz(Float_t _pz);
		void DeltaEtaSCTrack(Float_t _DeltaEtaSCTrack);
		void DeltaPhiSCTrack(Float_t _DeltaPhiSCTrack);
		void ESCOverETrack(Float_t _ESCOverETrack);
		void ECalEnergy(Float_t _ECalEnergy);
		void E1x5(Float_t _E1x5);
		void E2x5(Float_t _E2x5);
		void E5x5(Float_t _E5x5);
		void R9(Float_t _R9);
		void SigmaIEtaIEta(Float_t _SigmaIEtaIEta);
		void SigmaIPhiIPhi(Float_t _SigmaIPhiIPhi);
		void SigmaIEtaIPhi(Float_t _SigmaIEtaIPhi);
		void EHCalTowerOverECalD1(Float_t _EHCalTowerOverECalD1);
		void EHCalTowerOverECalD2(Float_t _EHCalTowerOverECalD2);
 };



class Data_IOTrack
 {
	friend class IOTrack;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		Int_t* VertexNumber_;
		Char_t* Charge_;
		UChar_t* NStripHits_;
		UChar_t* NPixelHits_;
		UChar_t* NMissingHits_;
		UChar_t* NMissingInnerHits_;
		UChar_t* NPixelLayers_;
		UChar_t* NStripLayers_;
		Float_t* px_;
		Float_t* py_;
		Float_t* pz_;
		Float_t* ChiQ_;
		Float_t* NDOF_;
		Float_t* Dxy_;
		Float_t* DxyUnc_;
		Float_t* Dz_;
		Float_t* DzUnc_;
		Float_t* DeDx_;
	public:
		void Fill();
		Data_IOTrack(UInt_t size, std::string prefix);
		~Data_IOTrack();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class IOTrack
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_IOTrack* data_;
	public:
		IOTrack(Data_IOTrack* data, UInt_t number);
		IOTrack(const IOTrack& _iotrack);
		void Init();
		Int_t VertexNumber() const;
		Char_t Charge() const;
		UChar_t NStripHits() const;
		UChar_t NPixelHits() const;
		UChar_t NMissingHits() const;
		UChar_t NMissingInnerHits() const;
		UChar_t NPixelLayers() const;
		UChar_t NStripLayers() const;
		Float_t px() const;
		Float_t py() const;
		Float_t pz() const;
		Float_t ChiQ() const;
		Float_t NDOF() const;
		Float_t Dxy() const;
		Float_t DxyUnc() const;
		Float_t Dz() const;
		Float_t DzUnc() const;
		Float_t DeDx() const;
		void VertexNumber(Int_t _VertexNumber);
		void Charge(Char_t _Charge);
		void NStripHits(UChar_t _NStripHits);
		void NPixelHits(UChar_t _NPixelHits);
		void NMissingHits(UChar_t _NMissingHits);
		void NMissingInnerHits(UChar_t _NMissingInnerHits);
		void NPixelLayers(UChar_t _NPixelLayers);
		void NStripLayers(UChar_t _NStripLayers);
		void px(Float_t _px);
		void py(Float_t _py);
		void pz(Float_t _pz);
		void ChiQ(Float_t _ChiQ);
		void NDOF(Float_t _NDOF);
		void Dxy(Float_t _Dxy);
		void DxyUnc(Float_t _DxyUnc);
		void Dz(Float_t _Dz);
		void DzUnc(Float_t _DzUnc);
		void DeDx(Float_t _DeDx);
 };



class Data_IOBeamSpot
 {
	friend class IOBeamSpot;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		Float_t* Vx_;
		Float_t* Vy_;
		Float_t* Vz_;
		Float_t* XWidth_;
		Float_t* YWidth_;
		Float_t* ZWidth_;
	public:
		void Fill();
		Data_IOBeamSpot(UInt_t size, std::string prefix);
		~Data_IOBeamSpot();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class IOBeamSpot
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_IOBeamSpot* data_;
	public:
		IOBeamSpot(Data_IOBeamSpot* data, UInt_t number);
		IOBeamSpot(const IOBeamSpot& _iobeamspot);
		void Init();
		Float_t Vx() const;
		Float_t Vy() const;
		Float_t Vz() const;
		Float_t XWidth() const;
		Float_t YWidth() const;
		Float_t ZWidth() const;
		void Vx(Float_t _Vx);
		void Vy(Float_t _Vy);
		void Vz(Float_t _Vz);
		void XWidth(Float_t _XWidth);
		void YWidth(Float_t _YWidth);
		void ZWidth(Float_t _ZWidth);
 };



class Data_IOPhoton
 {
	friend class IOPhoton;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		UInt_t* TriggerMatching_;
		UInt_t* Info_;
		Data_PFIsolation* PFR3_;
		Data_IOSuperCluster* SC_;
		Float_t* px_;
		Float_t* py_;
		Float_t* pz_;
		Float_t* E1x5_;
		Float_t* E2x5_;
		Float_t* E3x3_;
		Float_t* E5x5_;
		Float_t* MaxCrystalEnergy_;
		Float_t* SigmaIEtaIEta_;
		Float_t* SigmaIPhiIPhi_;
		Float_t* SigmaIEtaIPhi_;
		Float_t* EHCalTowerOverECalD1_;
		Float_t* EHCalTowerOverECalD2_;
	public:
		void Fill();
		Data_IOPhoton(UInt_t size, std::string prefix);
		~Data_IOPhoton();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class IOPhoton
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_IOPhoton* data_;
	public:
		IOPhoton(Data_IOPhoton* data, UInt_t number);
		IOPhoton(const IOPhoton& _iophoton);
		void Init();
		UInt_t TriggerMatching() const;
		UInt_t Info() const;
		PFIsolation PFR3() const;
		IOSuperCluster SC() const;
		Float_t px() const;
		Float_t py() const;
		Float_t pz() const;
		Float_t E1x5() const;
		Float_t E2x5() const;
		Float_t E3x3() const;
		Float_t E5x5() const;
		Float_t MaxCrystalEnergy() const;
		Float_t SigmaIEtaIEta() const;
		Float_t SigmaIPhiIPhi() const;
		Float_t SigmaIEtaIPhi() const;
		Float_t EHCalTowerOverECalD1() const;
		Float_t EHCalTowerOverECalD2() const;
		void TriggerMatching(UInt_t _TriggerMatching);
		void Info(UInt_t _Info);
		void px(Float_t _px);
		void py(Float_t _py);
		void pz(Float_t _pz);
		void E1x5(Float_t _E1x5);
		void E2x5(Float_t _E2x5);
		void E3x3(Float_t _E3x3);
		void E5x5(Float_t _E5x5);
		void MaxCrystalEnergy(Float_t _MaxCrystalEnergy);
		void SigmaIEtaIEta(Float_t _SigmaIEtaIEta);
		void SigmaIPhiIPhi(Float_t _SigmaIPhiIPhi);
		void SigmaIEtaIPhi(Float_t _SigmaIEtaIPhi);
		void EHCalTowerOverECalD1(Float_t _EHCalTowerOverECalD1);
		void EHCalTowerOverECalD2(Float_t _EHCalTowerOverECalD2);
 };


class BaseIO
{
	friend class IOMuon;
	friend class PrimaryVertex;
	friend class IOEventInfo;
	friend class IOPFJet;
	friend class IOString;
	friend class GenInfo;
	friend class IOMET;
	friend class IOSuperCluster;
	friend class PFIsolation;
	friend class SelectedGenParticle;
	friend class AllGenParticle;
	friend class IOElectron;
	friend class IOTrack;
	friend class IOBeamSpot;
	friend class IOPhoton;
	private:
		Data_IOMuon IOMuon_container_;
		Data_PrimaryVertex PrimaryVertex_container_;
		Data_IOEventInfo IOEventInfo_container_;
		Data_IOPFJet IOPFJet_container_;
		Data_GenInfo GenInfo_container_;
		Data_IOMET IOMET_container_;
		Data_SelectedGenParticle SelectedGenParticle_container_;
		Data_AllGenParticle AllGenParticle_container_;
		Data_IOElectron IOElectron_container_;
		Data_IOTrack IOTrack_container_;
		Data_IOBeamSpot IOBeamSpot_container_;
		Data_IOPhoton IOPhoton_container_;
		bool writable_;
		Int_t error_[1000];
		UInt_t errorcount_;
		TTree* tree_;
		TTree* copytree_;
		std::string treename_;
	public:
		BaseIO(std::string treename, bool writable);
		~BaseIO();
		void SetFile(TFile* file);
		bool IsWritable() const;
		void Fill();
		void SetError(Int_t errn);
		UInt_t NumErrors();
		void StartFilling();
		UInt_t GetEntries();
		void GetEntry(UInt_t n);
		UInt_t NumIOMuons();
		IOMuon GetIOMuon(UInt_t n);
		void LoadIOMuon(bool load);
		UInt_t NumPrimaryVertexs();
		PrimaryVertex GetPrimaryVertex(UInt_t n);
		void LoadPrimaryVertex(bool load);
		UInt_t NumIOEventInfos();
		IOEventInfo GetIOEventInfo(UInt_t n);
		void LoadIOEventInfo(bool load);
		UInt_t NumIOPFJets();
		IOPFJet GetIOPFJet(UInt_t n);
		void LoadIOPFJet(bool load);
		UInt_t NumGenInfos();
		GenInfo GetGenInfo(UInt_t n);
		void LoadGenInfo(bool load);
		UInt_t NumIOMETs();
		IOMET GetIOMET(UInt_t n);
		void LoadIOMET(bool load);
		UInt_t NumSelectedGenParticles();
		SelectedGenParticle GetSelectedGenParticle(UInt_t n);
		void LoadSelectedGenParticle(bool load);
		UInt_t NumAllGenParticles();
		AllGenParticle GetAllGenParticle(UInt_t n);
		void LoadAllGenParticle(bool load);
		UInt_t NumIOElectrons();
		IOElectron GetIOElectron(UInt_t n);
		void LoadIOElectron(bool load);
		UInt_t NumIOTracks();
		IOTrack GetIOTrack(UInt_t n);
		void LoadIOTrack(bool load);
		UInt_t NumIOBeamSpots();
		IOBeamSpot GetIOBeamSpot(UInt_t n);
		void LoadIOBeamSpot(bool load);
		UInt_t NumIOPhotons();
		IOPhoton GetIOPhoton(UInt_t n);
		void LoadIOPhoton(bool load);
};
}
#endif
