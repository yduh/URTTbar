#ifndef URStreamer_h
#define URStreamer_h

//#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <vector>
using namespace std;

class Met{
friend class URStreamer;
public:
//  Met(const float &i_px_,const float &i_py_,const float &i_et_,const float &i_phi_):
//    
//  {}
  Met():
    px_(0),
    py_(0),
    et_(0),
    phi_(0)
  {}
  float px() const {return px_;}
  float py() const {return py_;}
  float et() const {return et_;}
  float phi() const {return phi_;}
private:
  float px_;
  float py_;
  float et_;
  float phi_;
  void setpx(const float value) {px_ = value;}
  void setpy(const float value) {py_ = value;}
  void setet(const float value) {et_ = value;}
  void setphi(const float value) {phi_ = value;}
};

class Geninfo{
friend class URStreamer;
public:
//  Geninfo(const Float_t &i_weight_,const Float_t &i_pdfid1_,const Float_t &i_pdfid2_,const Float_t &i_renScale_):
//    
//  {}
  Geninfo():
    weight_(0),
    pdfid1_(0),
    pdfid2_(0),
    renScale_(0)
  {}
  Float_t weight() const {return weight_;}
  Float_t pdfid1() const {return pdfid1_;}
  Float_t pdfid2() const {return pdfid2_;}
  Float_t renScale() const {return renScale_;}
private:
  Float_t weight_;
  Float_t pdfid1_;
  Float_t pdfid2_;
  Float_t renScale_;
  void setweight(const Float_t value) {weight_ = value;}
  void setpdfid1(const Float_t value) {pdfid1_ = value;}
  void setpdfid2(const Float_t value) {pdfid2_ = value;}
  void setrenScale(const Float_t value) {renScale_ = value;}
};

class Photon{
friend class URStreamer;
public:
//  Photon(const float &i_px_,const float &i_py_,const float &i_pz_,const float &i_pt_,const float &i_eta_,const float &i_phi_,const int &i_charge_,const float &i_x_,const float &i_y_,const float &i_z_,const float &i_energy_,const float &i_rawEnergy_,const float &i_phiWidth_,const float &i_etaWidth_,const float &i_e3x3_,const float &i_maxCrystalEnergy_,const bool &i_isEB_,const bool &i_isEE_,const bool &i_isPFlowPhoton_,const bool &i_hasConversionTracks_,const bool &i_hasPixelSeed_):
//    
//  {}
  Photon():
    px_(0),
    py_(0),
    pz_(0),
    pt_(0),
    eta_(0),
    phi_(0),
    charge_(0),
    x_(0),
    y_(0),
    z_(0),
    energy_(0),
    rawEnergy_(0),
    phiWidth_(0),
    etaWidth_(0),
    e3x3_(0),
    maxCrystalEnergy_(0),
    isEB_(0),
    isEE_(0),
    isPFlowPhoton_(0),
    hasConversionTracks_(0),
    hasPixelSeed_(0)
  {}
  float px() const {return px_;}
  float py() const {return py_;}
  float pz() const {return pz_;}
  float pt() const {return pt_;}
  float eta() const {return eta_;}
  float phi() const {return phi_;}
  int charge() const {return charge_;}
  float x() const {return x_;}
  float y() const {return y_;}
  float z() const {return z_;}
  float energy() const {return energy_;}
  float rawEnergy() const {return rawEnergy_;}
  float phiWidth() const {return phiWidth_;}
  float etaWidth() const {return etaWidth_;}
  float e3x3() const {return e3x3_;}
  float maxCrystalEnergy() const {return maxCrystalEnergy_;}
  bool isEB() const {return isEB_;}
  bool isEE() const {return isEE_;}
  bool isPFlowPhoton() const {return isPFlowPhoton_;}
  bool hasConversionTracks() const {return hasConversionTracks_;}
  bool hasPixelSeed() const {return hasPixelSeed_;}
private:
  float px_;
  float py_;
  float pz_;
  float pt_;
  float eta_;
  float phi_;
  int charge_;
  float x_;
  float y_;
  float z_;
  float energy_;
  float rawEnergy_;
  float phiWidth_;
  float etaWidth_;
  float e3x3_;
  float maxCrystalEnergy_;
  bool isEB_;
  bool isEE_;
  bool isPFlowPhoton_;
  bool hasConversionTracks_;
  bool hasPixelSeed_;
  void setpx(const float value) {px_ = value;}
  void setpy(const float value) {py_ = value;}
  void setpz(const float value) {pz_ = value;}
  void setpt(const float value) {pt_ = value;}
  void seteta(const float value) {eta_ = value;}
  void setphi(const float value) {phi_ = value;}
  void setcharge(const int value) {charge_ = value;}
  void setx(const float value) {x_ = value;}
  void sety(const float value) {y_ = value;}
  void setz(const float value) {z_ = value;}
  void setenergy(const float value) {energy_ = value;}
  void setrawEnergy(const float value) {rawEnergy_ = value;}
  void setphiWidth(const float value) {phiWidth_ = value;}
  void setetaWidth(const float value) {etaWidth_ = value;}
  void sete3x3(const float value) {e3x3_ = value;}
  void setmaxCrystalEnergy(const float value) {maxCrystalEnergy_ = value;}
  void setisEB(const bool value) {isEB_ = value;}
  void setisEE(const bool value) {isEE_ = value;}
  void setisPFlowPhoton(const bool value) {isPFlowPhoton_ = value;}
  void sethasConversionTracks(const bool value) {hasConversionTracks_ = value;}
  void sethasPixelSeed(const bool value) {hasPixelSeed_ = value;}
};

class Trigger{
friend class URStreamer;
public:
//  Trigger(const Bool_t &i_IsoMu20_eta2p1_,const Bool_t &i_IsoMu24_,const Bool_t &i_IsoMu24_eta2p1_,const Bool_t &i_IsoMu30_,const Bool_t &i_IsoMu30_eta2p1_,const Bool_t &i_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_,const Bool_t &i_Ele27_WP80_,const Bool_t &i_Ele30_CaloIdVT_TrkIdT_):
//    
//  {}
  Trigger():
    IsoMu20_eta2p1_(0),
    IsoMu24_(0),
    IsoMu24_eta2p1_(0),
    IsoMu30_(0),
    IsoMu30_eta2p1_(0),
    Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_(0),
    Ele27_WP80_(0),
    Ele30_CaloIdVT_TrkIdT_(0)
  {}
  Bool_t IsoMu20_eta2p1() const {return IsoMu20_eta2p1_;}
  Bool_t IsoMu24() const {return IsoMu24_;}
  Bool_t IsoMu24_eta2p1() const {return IsoMu24_eta2p1_;}
  Bool_t IsoMu30() const {return IsoMu30_;}
  Bool_t IsoMu30_eta2p1() const {return IsoMu30_eta2p1_;}
  Bool_t Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL() const {return Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_;}
  Bool_t Ele27_WP80() const {return Ele27_WP80_;}
  Bool_t Ele30_CaloIdVT_TrkIdT() const {return Ele30_CaloIdVT_TrkIdT_;}
private:
  Bool_t IsoMu20_eta2p1_;
  Bool_t IsoMu24_;
  Bool_t IsoMu24_eta2p1_;
  Bool_t IsoMu30_;
  Bool_t IsoMu30_eta2p1_;
  Bool_t Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_;
  Bool_t Ele27_WP80_;
  Bool_t Ele30_CaloIdVT_TrkIdT_;
  void setIsoMu20_eta2p1(const Bool_t value) {IsoMu20_eta2p1_ = value;}
  void setIsoMu24(const Bool_t value) {IsoMu24_ = value;}
  void setIsoMu24_eta2p1(const Bool_t value) {IsoMu24_eta2p1_ = value;}
  void setIsoMu30(const Bool_t value) {IsoMu30_ = value;}
  void setIsoMu30_eta2p1(const Bool_t value) {IsoMu30_eta2p1_ = value;}
  void setEle27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL(const Bool_t value) {Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_ = value;}
  void setEle27_WP80(const Bool_t value) {Ele27_WP80_ = value;}
  void setEle30_CaloIdVT_TrkIdT(const Bool_t value) {Ele30_CaloIdVT_TrkIdT_ = value;}
};

class Electron{
friend class URStreamer;
public:
//  Electron(const float &i_px_,const float &i_py_,const float &i_pz_,const float &i_pt_,const float &i_eta_,const float &i_phi_,const int &i_charge_,const float &i_chargedIso_,const float &i_neutralIso_,const float &i_photonIso_,const float &i_puIso_,const float &i_r9_,const float &i_ESCOverETrack_,const float &i_DEtaSCTrk_,const float &i_DPhiSCTrk_,const float &i_ecalEnergy_,const bool &i_isEB_,const bool &i_isEE_,const bool &i_isEBGap_,const bool &i_isEBEtaGap_,const bool &i_isEBPhiGap_,const bool &i_isEEGap_,const bool &i_isEERingGap_,const bool &i_isEEDeeGap_,const bool &i_isEBEEGap_,const bool &i_isElectron_,const bool &i_ecalSeed_,const bool &i_trackSeed_,const bool &i_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_,const bool &i_Ele27_WP80_,const bool &i_Ele30_CaloIdVT_TrkIdT_,const float &i_e1x5_,const float &i_e5x5_,const float &i_sigmaIEtaIEta_,const float &i_sigmaIPhiIPhi_,const float &i_hcalDepth1OverEcalBc_,const float &i_hcalDepth2OverEcalBc_,const float &i_x_,const float &i_y_,const float &i_z_,const float &i_energy_,const float &i_rawEnergy_,const float &i_phiWidth_,const float &i_etaWidth_):
//    
//  {}
  Electron():
    px_(0),
    py_(0),
    pz_(0),
    pt_(0),
    eta_(0),
    phi_(0),
    charge_(0),
    chargedIso_(0),
    neutralIso_(0),
    photonIso_(0),
    puIso_(0),
    r9_(0),
    ESCOverETrack_(0),
    DEtaSCTrk_(0),
    DPhiSCTrk_(0),
    ecalEnergy_(0),
    isEB_(0),
    isEE_(0),
    isEBGap_(0),
    isEBEtaGap_(0),
    isEBPhiGap_(0),
    isEEGap_(0),
    isEERingGap_(0),
    isEEDeeGap_(0),
    isEBEEGap_(0),
    isElectron_(0),
    ecalSeed_(0),
    trackSeed_(0),
    Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_(0),
    Ele27_WP80_(0),
    Ele30_CaloIdVT_TrkIdT_(0),
    e1x5_(0),
    e5x5_(0),
    sigmaIEtaIEta_(0),
    sigmaIPhiIPhi_(0),
    hcalDepth1OverEcalBc_(0),
    hcalDepth2OverEcalBc_(0),
    x_(0),
    y_(0),
    z_(0),
    energy_(0),
    rawEnergy_(0),
    phiWidth_(0),
    etaWidth_(0)
  {}
  float px() const {return px_;}
  float py() const {return py_;}
  float pz() const {return pz_;}
  float pt() const {return pt_;}
  float eta() const {return eta_;}
  float phi() const {return phi_;}
  int charge() const {return charge_;}
  float chargedIso() const {return chargedIso_;}
  float neutralIso() const {return neutralIso_;}
  float photonIso() const {return photonIso_;}
  float puIso() const {return puIso_;}
  float r9() const {return r9_;}
  float ESCOverETrack() const {return ESCOverETrack_;}
  float DEtaSCTrk() const {return DEtaSCTrk_;}
  float DPhiSCTrk() const {return DPhiSCTrk_;}
  float ecalEnergy() const {return ecalEnergy_;}
  bool isEB() const {return isEB_;}
  bool isEE() const {return isEE_;}
  bool isEBGap() const {return isEBGap_;}
  bool isEBEtaGap() const {return isEBEtaGap_;}
  bool isEBPhiGap() const {return isEBPhiGap_;}
  bool isEEGap() const {return isEEGap_;}
  bool isEERingGap() const {return isEERingGap_;}
  bool isEEDeeGap() const {return isEEDeeGap_;}
  bool isEBEEGap() const {return isEBEEGap_;}
  bool isElectron() const {return isElectron_;}
  bool ecalSeed() const {return ecalSeed_;}
  bool trackSeed() const {return trackSeed_;}
  bool Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL() const {return Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_;}
  bool Ele27_WP80() const {return Ele27_WP80_;}
  bool Ele30_CaloIdVT_TrkIdT() const {return Ele30_CaloIdVT_TrkIdT_;}
  float e1x5() const {return e1x5_;}
  float e5x5() const {return e5x5_;}
  float sigmaIEtaIEta() const {return sigmaIEtaIEta_;}
  float sigmaIPhiIPhi() const {return sigmaIPhiIPhi_;}
  float hcalDepth1OverEcalBc() const {return hcalDepth1OverEcalBc_;}
  float hcalDepth2OverEcalBc() const {return hcalDepth2OverEcalBc_;}
  float x() const {return x_;}
  float y() const {return y_;}
  float z() const {return z_;}
  float energy() const {return energy_;}
  float rawEnergy() const {return rawEnergy_;}
  float phiWidth() const {return phiWidth_;}
  float etaWidth() const {return etaWidth_;}
private:
  float px_;
  float py_;
  float pz_;
  float pt_;
  float eta_;
  float phi_;
  int charge_;
  float chargedIso_;
  float neutralIso_;
  float photonIso_;
  float puIso_;
  float r9_;
  float ESCOverETrack_;
  float DEtaSCTrk_;
  float DPhiSCTrk_;
  float ecalEnergy_;
  bool isEB_;
  bool isEE_;
  bool isEBGap_;
  bool isEBEtaGap_;
  bool isEBPhiGap_;
  bool isEEGap_;
  bool isEERingGap_;
  bool isEEDeeGap_;
  bool isEBEEGap_;
  bool isElectron_;
  bool ecalSeed_;
  bool trackSeed_;
  bool Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_;
  bool Ele27_WP80_;
  bool Ele30_CaloIdVT_TrkIdT_;
  float e1x5_;
  float e5x5_;
  float sigmaIEtaIEta_;
  float sigmaIPhiIPhi_;
  float hcalDepth1OverEcalBc_;
  float hcalDepth2OverEcalBc_;
  float x_;
  float y_;
  float z_;
  float energy_;
  float rawEnergy_;
  float phiWidth_;
  float etaWidth_;
  void setpx(const float value) {px_ = value;}
  void setpy(const float value) {py_ = value;}
  void setpz(const float value) {pz_ = value;}
  void setpt(const float value) {pt_ = value;}
  void seteta(const float value) {eta_ = value;}
  void setphi(const float value) {phi_ = value;}
  void setcharge(const int value) {charge_ = value;}
  void setchargedIso(const float value) {chargedIso_ = value;}
  void setneutralIso(const float value) {neutralIso_ = value;}
  void setphotonIso(const float value) {photonIso_ = value;}
  void setpuIso(const float value) {puIso_ = value;}
  void setr9(const float value) {r9_ = value;}
  void setESCOverETrack(const float value) {ESCOverETrack_ = value;}
  void setDEtaSCTrk(const float value) {DEtaSCTrk_ = value;}
  void setDPhiSCTrk(const float value) {DPhiSCTrk_ = value;}
  void setecalEnergy(const float value) {ecalEnergy_ = value;}
  void setisEB(const bool value) {isEB_ = value;}
  void setisEE(const bool value) {isEE_ = value;}
  void setisEBGap(const bool value) {isEBGap_ = value;}
  void setisEBEtaGap(const bool value) {isEBEtaGap_ = value;}
  void setisEBPhiGap(const bool value) {isEBPhiGap_ = value;}
  void setisEEGap(const bool value) {isEEGap_ = value;}
  void setisEERingGap(const bool value) {isEERingGap_ = value;}
  void setisEEDeeGap(const bool value) {isEEDeeGap_ = value;}
  void setisEBEEGap(const bool value) {isEBEEGap_ = value;}
  void setisElectron(const bool value) {isElectron_ = value;}
  void setecalSeed(const bool value) {ecalSeed_ = value;}
  void settrackSeed(const bool value) {trackSeed_ = value;}
  void setEle27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL(const bool value) {Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_ = value;}
  void setEle27_WP80(const bool value) {Ele27_WP80_ = value;}
  void setEle30_CaloIdVT_TrkIdT(const bool value) {Ele30_CaloIdVT_TrkIdT_ = value;}
  void sete1x5(const float value) {e1x5_ = value;}
  void sete5x5(const float value) {e5x5_ = value;}
  void setsigmaIEtaIEta(const float value) {sigmaIEtaIEta_ = value;}
  void setsigmaIPhiIPhi(const float value) {sigmaIPhiIPhi_ = value;}
  void sethcalDepth1OverEcalBc(const float value) {hcalDepth1OverEcalBc_ = value;}
  void sethcalDepth2OverEcalBc(const float value) {hcalDepth2OverEcalBc_ = value;}
  void setx(const float value) {x_ = value;}
  void sety(const float value) {y_ = value;}
  void setz(const float value) {z_ = value;}
  void setenergy(const float value) {energy_ = value;}
  void setrawEnergy(const float value) {rawEnergy_ = value;}
  void setphiWidth(const float value) {phiWidth_ = value;}
  void setetaWidth(const float value) {etaWidth_ = value;}
};

class Jet{
friend class URStreamer;
public:
//  Jet(const float &i_px_,const float &i_py_,const float &i_pz_,const float &i_pt_,const float &i_eta_,const float &i_phi_,const int &i_charge_,const float &i_e_,const float &i_area_,const float &i_mass_,const float &i_chargedHadronEnergy_,const float &i_neutralHadronEnergy_,const float &i_photonEnergy_,const float &i_electronEnergy_,const float &i_muonEnergy_,const float &i_forwardEM_,const float &i_forwardHad_,const float &i_numChargedHadrons_,const float &i_numNeutralHadrons_,const float &i_numPhotons_,const float &i_numElectrons_,const float &i_numMuons_,const float &i_numForwardEMs_,const float &i_numForwardHads_,const float &i_puId_,const float &i_jetBProb_,const float &i_jetProb_,const float &i_trkHiPur_,const float &i_trkHiEff_,const float &i_ssvHiEff_,const float &i_ssvHiPur_,const float &i_csv_,const float &i_csvIncl_,const float &i_vtxMass_,const float &i_vtxNtracks_,const float &i_vtx3DVal_,const float &i_vtx3DSig_):
//    
//  {}
  Jet():
    px_(0),
    py_(0),
    pz_(0),
    pt_(0),
    eta_(0),
    phi_(0),
    charge_(0),
    e_(0),
    area_(0),
    mass_(0),
    chargedHadronEnergy_(0),
    neutralHadronEnergy_(0),
    photonEnergy_(0),
    electronEnergy_(0),
    muonEnergy_(0),
    forwardEM_(0),
    forwardHad_(0),
    numChargedHadrons_(0),
    numNeutralHadrons_(0),
    numPhotons_(0),
    numElectrons_(0),
    numMuons_(0),
    numForwardEMs_(0),
    numForwardHads_(0),
    puId_(0),
    jetBProb_(0),
    jetProb_(0),
    trkHiPur_(0),
    trkHiEff_(0),
    ssvHiEff_(0),
    ssvHiPur_(0),
    csv_(0),
    csvIncl_(0),
    vtxMass_(0),
    vtxNtracks_(0),
    vtx3DVal_(0),
    vtx3DSig_(0)
  {}
  float px() const {return px_;}
  float py() const {return py_;}
  float pz() const {return pz_;}
  float pt() const {return pt_;}
  float eta() const {return eta_;}
  float phi() const {return phi_;}
  int charge() const {return charge_;}
  float e() const {return e_;}
  float area() const {return area_;}
  float mass() const {return mass_;}
  float chargedHadronEnergy() const {return chargedHadronEnergy_;}
  float neutralHadronEnergy() const {return neutralHadronEnergy_;}
  float photonEnergy() const {return photonEnergy_;}
  float electronEnergy() const {return electronEnergy_;}
  float muonEnergy() const {return muonEnergy_;}
  float forwardEM() const {return forwardEM_;}
  float forwardHad() const {return forwardHad_;}
  float numChargedHadrons() const {return numChargedHadrons_;}
  float numNeutralHadrons() const {return numNeutralHadrons_;}
  float numPhotons() const {return numPhotons_;}
  float numElectrons() const {return numElectrons_;}
  float numMuons() const {return numMuons_;}
  float numForwardEMs() const {return numForwardEMs_;}
  float numForwardHads() const {return numForwardHads_;}
  float puId() const {return puId_;}
  float jetBProb() const {return jetBProb_;}
  float jetProb() const {return jetProb_;}
  float trkHiPur() const {return trkHiPur_;}
  float trkHiEff() const {return trkHiEff_;}
  float ssvHiEff() const {return ssvHiEff_;}
  float ssvHiPur() const {return ssvHiPur_;}
  float csv() const {return csv_;}
  float csvIncl() const {return csvIncl_;}
  float vtxMass() const {return vtxMass_;}
  float vtxNtracks() const {return vtxNtracks_;}
  float vtx3DVal() const {return vtx3DVal_;}
  float vtx3DSig() const {return vtx3DSig_;}
private:
  float px_;
  float py_;
  float pz_;
  float pt_;
  float eta_;
  float phi_;
  int charge_;
  float e_;
  float area_;
  float mass_;
  float chargedHadronEnergy_;
  float neutralHadronEnergy_;
  float photonEnergy_;
  float electronEnergy_;
  float muonEnergy_;
  float forwardEM_;
  float forwardHad_;
  float numChargedHadrons_;
  float numNeutralHadrons_;
  float numPhotons_;
  float numElectrons_;
  float numMuons_;
  float numForwardEMs_;
  float numForwardHads_;
  float puId_;
  float jetBProb_;
  float jetProb_;
  float trkHiPur_;
  float trkHiEff_;
  float ssvHiEff_;
  float ssvHiPur_;
  float csv_;
  float csvIncl_;
  float vtxMass_;
  float vtxNtracks_;
  float vtx3DVal_;
  float vtx3DSig_;
  void setpx(const float value) {px_ = value;}
  void setpy(const float value) {py_ = value;}
  void setpz(const float value) {pz_ = value;}
  void setpt(const float value) {pt_ = value;}
  void seteta(const float value) {eta_ = value;}
  void setphi(const float value) {phi_ = value;}
  void setcharge(const int value) {charge_ = value;}
  void sete(const float value) {e_ = value;}
  void setarea(const float value) {area_ = value;}
  void setmass(const float value) {mass_ = value;}
  void setchargedHadronEnergy(const float value) {chargedHadronEnergy_ = value;}
  void setneutralHadronEnergy(const float value) {neutralHadronEnergy_ = value;}
  void setphotonEnergy(const float value) {photonEnergy_ = value;}
  void setelectronEnergy(const float value) {electronEnergy_ = value;}
  void setmuonEnergy(const float value) {muonEnergy_ = value;}
  void setforwardEM(const float value) {forwardEM_ = value;}
  void setforwardHad(const float value) {forwardHad_ = value;}
  void setnumChargedHadrons(const float value) {numChargedHadrons_ = value;}
  void setnumNeutralHadrons(const float value) {numNeutralHadrons_ = value;}
  void setnumPhotons(const float value) {numPhotons_ = value;}
  void setnumElectrons(const float value) {numElectrons_ = value;}
  void setnumMuons(const float value) {numMuons_ = value;}
  void setnumForwardEMs(const float value) {numForwardEMs_ = value;}
  void setnumForwardHads(const float value) {numForwardHads_ = value;}
  void setpuId(const float value) {puId_ = value;}
  void setjetBProb(const float value) {jetBProb_ = value;}
  void setjetProb(const float value) {jetProb_ = value;}
  void settrkHiPur(const float value) {trkHiPur_ = value;}
  void settrkHiEff(const float value) {trkHiEff_ = value;}
  void setssvHiEff(const float value) {ssvHiEff_ = value;}
  void setssvHiPur(const float value) {ssvHiPur_ = value;}
  void setcsv(const float value) {csv_ = value;}
  void setcsvIncl(const float value) {csvIncl_ = value;}
  void setvtxMass(const float value) {vtxMass_ = value;}
  void setvtxNtracks(const float value) {vtxNtracks_ = value;}
  void setvtx3DVal(const float value) {vtx3DVal_ = value;}
  void setvtx3DSig(const float value) {vtx3DSig_ = value;}
};

class Muon{
friend class URStreamer;
public:
//  Muon(const float &i_px_,const float &i_py_,const float &i_pz_,const float &i_pt_,const float &i_eta_,const float &i_phi_,const int &i_charge_,const float &i_chargedIso_,const float &i_neutralIso_,const float &i_photonIso_,const float &i_puIso_,const float &i_ECalEnergy_,const float &i_HCalEnergy_,const int &i_numChambers_,const int &i_numMatchedStations_,const float &i_ptErr_,const float &i_chi2_,const int &i_ndof_,const float &i_validHits_,const bool &i_isGlobal_,const bool &i_isTracker_,const bool &i_isCalo_,const bool &i_isPF_,const bool &i_isStandAlone_,const bool &i_isLoose_,const bool &i_IsoMu20_eta2p1_,const bool &i_IsoMu24_,const bool &i_IsoMu24_eta2p1_,const bool &i_IsoMu30_,const bool &i_IsoMu30_eta2p1_):
//    
//  {}
  Muon():
    px_(0),
    py_(0),
    pz_(0),
    pt_(0),
    eta_(0),
    phi_(0),
    charge_(0),
    chargedIso_(0),
    neutralIso_(0),
    photonIso_(0),
    puIso_(0),
    ECalEnergy_(0),
    HCalEnergy_(0),
    numChambers_(0),
    numMatchedStations_(0),
    ptErr_(0),
    chi2_(0),
    ndof_(0),
    validHits_(0),
    isGlobal_(0),
    isTracker_(0),
    isCalo_(0),
    isPF_(0),
    isStandAlone_(0),
    isLoose_(0),
    IsoMu20_eta2p1_(0),
    IsoMu24_(0),
    IsoMu24_eta2p1_(0),
    IsoMu30_(0),
    IsoMu30_eta2p1_(0)
  {}
  float px() const {return px_;}
  float py() const {return py_;}
  float pz() const {return pz_;}
  float pt() const {return pt_;}
  float eta() const {return eta_;}
  float phi() const {return phi_;}
  int charge() const {return charge_;}
  float chargedIso() const {return chargedIso_;}
  float neutralIso() const {return neutralIso_;}
  float photonIso() const {return photonIso_;}
  float puIso() const {return puIso_;}
  float ECalEnergy() const {return ECalEnergy_;}
  float HCalEnergy() const {return HCalEnergy_;}
  int numChambers() const {return numChambers_;}
  int numMatchedStations() const {return numMatchedStations_;}
  float ptErr() const {return ptErr_;}
  float chi2() const {return chi2_;}
  int ndof() const {return ndof_;}
  float validHits() const {return validHits_;}
  bool isGlobal() const {return isGlobal_;}
  bool isTracker() const {return isTracker_;}
  bool isCalo() const {return isCalo_;}
  bool isPF() const {return isPF_;}
  bool isStandAlone() const {return isStandAlone_;}
  bool isLoose() const {return isLoose_;}
  bool IsoMu20_eta2p1() const {return IsoMu20_eta2p1_;}
  bool IsoMu24() const {return IsoMu24_;}
  bool IsoMu24_eta2p1() const {return IsoMu24_eta2p1_;}
  bool IsoMu30() const {return IsoMu30_;}
  bool IsoMu30_eta2p1() const {return IsoMu30_eta2p1_;}
private:
  float px_;
  float py_;
  float pz_;
  float pt_;
  float eta_;
  float phi_;
  int charge_;
  float chargedIso_;
  float neutralIso_;
  float photonIso_;
  float puIso_;
  float ECalEnergy_;
  float HCalEnergy_;
  int numChambers_;
  int numMatchedStations_;
  float ptErr_;
  float chi2_;
  int ndof_;
  float validHits_;
  bool isGlobal_;
  bool isTracker_;
  bool isCalo_;
  bool isPF_;
  bool isStandAlone_;
  bool isLoose_;
  bool IsoMu20_eta2p1_;
  bool IsoMu24_;
  bool IsoMu24_eta2p1_;
  bool IsoMu30_;
  bool IsoMu30_eta2p1_;
  void setpx(const float value) {px_ = value;}
  void setpy(const float value) {py_ = value;}
  void setpz(const float value) {pz_ = value;}
  void setpt(const float value) {pt_ = value;}
  void seteta(const float value) {eta_ = value;}
  void setphi(const float value) {phi_ = value;}
  void setcharge(const int value) {charge_ = value;}
  void setchargedIso(const float value) {chargedIso_ = value;}
  void setneutralIso(const float value) {neutralIso_ = value;}
  void setphotonIso(const float value) {photonIso_ = value;}
  void setpuIso(const float value) {puIso_ = value;}
  void setECalEnergy(const float value) {ECalEnergy_ = value;}
  void setHCalEnergy(const float value) {HCalEnergy_ = value;}
  void setnumChambers(const int value) {numChambers_ = value;}
  void setnumMatchedStations(const int value) {numMatchedStations_ = value;}
  void setptErr(const float value) {ptErr_ = value;}
  void setchi2(const float value) {chi2_ = value;}
  void setndof(const int value) {ndof_ = value;}
  void setvalidHits(const float value) {validHits_ = value;}
  void setisGlobal(const bool value) {isGlobal_ = value;}
  void setisTracker(const bool value) {isTracker_ = value;}
  void setisCalo(const bool value) {isCalo_ = value;}
  void setisPF(const bool value) {isPF_ = value;}
  void setisStandAlone(const bool value) {isStandAlone_ = value;}
  void setisLoose(const bool value) {isLoose_ = value;}
  void setIsoMu20_eta2p1(const bool value) {IsoMu20_eta2p1_ = value;}
  void setIsoMu24(const bool value) {IsoMu24_ = value;}
  void setIsoMu24_eta2p1(const bool value) {IsoMu24_eta2p1_ = value;}
  void setIsoMu30(const bool value) {IsoMu30_ = value;}
  void setIsoMu30_eta2p1(const bool value) {IsoMu30_eta2p1_ = value;}
};

class Genparticle{
friend class URStreamer;
public:
//  Genparticle(const float &i_px_,const float &i_py_,const float &i_pz_,const float &i_pt_,const float &i_eta_,const float &i_phi_,const int &i_charge_,const float &i_e_,const float &i_vx_,const float &i_vy_,const float &i_vz_,const int &i_pdgId_,const int &i_status_,const int &i_idx_,const vector<int> &i_momIdx_,const int &i_nDaught_,const int &i_firstDaughtIdx_):
//    
//  {}
  Genparticle():
    px_(0),
    py_(0),
    pz_(0),
    pt_(0),
    eta_(0),
    phi_(0),
    charge_(0),
    e_(0),
    vx_(0),
    vy_(0),
    vz_(0),
    pdgId_(0),
    status_(0),
    idx_(0),
    momIdx_(0),
    nDaught_(0),
    firstDaughtIdx_(0)
  {}
  float px() const {return px_;}
  float py() const {return py_;}
  float pz() const {return pz_;}
  float pt() const {return pt_;}
  float eta() const {return eta_;}
  float phi() const {return phi_;}
  int charge() const {return charge_;}
  float e() const {return e_;}
  float vx() const {return vx_;}
  float vy() const {return vy_;}
  float vz() const {return vz_;}
  int pdgId() const {return pdgId_;}
  int status() const {return status_;}
  int idx() const {return idx_;}
  vector<int> momIdx() const {return momIdx_;}
  int nDaught() const {return nDaught_;}
  int firstDaughtIdx() const {return firstDaughtIdx_;}
private:
  float px_;
  float py_;
  float pz_;
  float pt_;
  float eta_;
  float phi_;
  int charge_;
  float e_;
  float vx_;
  float vy_;
  float vz_;
  int pdgId_;
  int status_;
  int idx_;
  vector<int> momIdx_;
  int nDaught_;
  int firstDaughtIdx_;
  void setpx(const float value) {px_ = value;}
  void setpy(const float value) {py_ = value;}
  void setpz(const float value) {pz_ = value;}
  void setpt(const float value) {pt_ = value;}
  void seteta(const float value) {eta_ = value;}
  void setphi(const float value) {phi_ = value;}
  void setcharge(const int value) {charge_ = value;}
  void sete(const float value) {e_ = value;}
  void setvx(const float value) {vx_ = value;}
  void setvy(const float value) {vy_ = value;}
  void setvz(const float value) {vz_ = value;}
  void setpdgId(const int value) {pdgId_ = value;}
  void setstatus(const int value) {status_ = value;}
  void setidx(const int value) {idx_ = value;}
  void setmomIdx(const vector<int> value) {momIdx_ = value;}
  void setnDaught(const int value) {nDaught_ = value;}
  void setfirstDaughtIdx(const int value) {firstDaughtIdx_ = value;}
};

class Vertex{
friend class URStreamer;
public:
//  Vertex(const float &i_x_,const float &i_y_,const float &i_z_,const float &i_chi2_,const float &i_ndof_,const float &i_nTracks_):
//    
//  {}
  Vertex():
    x_(0),
    y_(0),
    z_(0),
    chi2_(0),
    ndof_(0),
    nTracks_(0)
  {}
  float x() const {return x_;}
  float y() const {return y_;}
  float z() const {return z_;}
  float chi2() const {return chi2_;}
  float ndof() const {return ndof_;}
  float nTracks() const {return nTracks_;}
private:
  float x_;
  float y_;
  float z_;
  float chi2_;
  float ndof_;
  float nTracks_;
  void setx(const float value) {x_ = value;}
  void sety(const float value) {y_ = value;}
  void setz(const float value) {z_ = value;}
  void setchi2(const float value) {chi2_ = value;}
  void setndof(const float value) {ndof_ = value;}
  void setnTracks(const float value) {nTracks_ = value;}
};

class Puinfo{
friend class URStreamer;
public:
//  Puinfo(const float &i_bx_,const float &i_nPU_,const float &i_nInteractions_):
//    
//  {}
  Puinfo():
    bx_(0),
    nPU_(0),
    nInteractions_(0)
  {}
  float bx() const {return bx_;}
  float nPU() const {return nPU_;}
  float nInteractions() const {return nInteractions_;}
private:
  float bx_;
  float nPU_;
  float nInteractions_;
  void setbx(const float value) {bx_ = value;}
  void setnPU(const float value) {nPU_ = value;}
  void setnInteractions(const float value) {nInteractions_ = value;}
};


class URStreamer{
public:
  UInt_t run;
  UInt_t lumi;
  ULong64_t evt;

  URStreamer(TTree *tree):
    run(0),
    lumi(0),
    evt(0),
    trigger_IsoMu20_eta2p1_(0),
    trigger_IsoMu24_(0),
    trigger_IsoMu24_eta2p1_(0),
    trigger_IsoMu30_(0),
    trigger_IsoMu30_eta2p1_(0),
    trigger_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_(0),
    trigger_Ele27_WP80_(0),
    trigger_Ele30_CaloIdVT_TrkIdT_(0),
    muons_px_(0),
    muons_py_(0),
    muons_pz_(0),
    muons_pt_(0),
    muons_eta_(0),
    muons_phi_(0),
    muons_charge_(0),
    muons_chargedIso_(0),
    muons_neutralIso_(0),
    muons_photonIso_(0),
    muons_puIso_(0),
    muons_ECalEnergy_(0),
    muons_HCalEnergy_(0),
    muons_numChambers_(0),
    muons_numMatchedStations_(0),
    muons_ptErr_(0),
    muons_chi2_(0),
    muons_ndof_(0),
    muons_validHits_(0),
    muons_isGlobal_(0),
    muons_isTracker_(0),
    muons_isCalo_(0),
    muons_isPF_(0),
    muons_isStandAlone_(0),
    muons_isLoose_(0),
    muons_IsoMu20_eta2p1_(0),
    muons_IsoMu24_(0),
    muons_IsoMu24_eta2p1_(0),
    muons_IsoMu30_(0),
    muons_IsoMu30_eta2p1_(0),
    jets_px_(0),
    jets_py_(0),
    jets_pz_(0),
    jets_pt_(0),
    jets_eta_(0),
    jets_phi_(0),
    jets_charge_(0),
    jets_e_(0),
    jets_area_(0),
    jets_mass_(0),
    jets_chargedHadronEnergy_(0),
    jets_neutralHadronEnergy_(0),
    jets_photonEnergy_(0),
    jets_electronEnergy_(0),
    jets_muonEnergy_(0),
    jets_forwardEM_(0),
    jets_forwardHad_(0),
    jets_numChargedHadrons_(0),
    jets_numNeutralHadrons_(0),
    jets_numPhotons_(0),
    jets_numElectrons_(0),
    jets_numMuons_(0),
    jets_numForwardEMs_(0),
    jets_numForwardHads_(0),
    jets_puId_(0),
    jets_jetBProb_(0),
    jets_jetProb_(0),
    jets_trkHiPur_(0),
    jets_trkHiEff_(0),
    jets_ssvHiEff_(0),
    jets_ssvHiPur_(0),
    jets_csv_(0),
    jets_csvIncl_(0),
    jets_vtxMass_(0),
    jets_vtxNtracks_(0),
    jets_vtx3DVal_(0),
    jets_vtx3DSig_(0),
    electrons_px_(0),
    electrons_py_(0),
    electrons_pz_(0),
    electrons_pt_(0),
    electrons_eta_(0),
    electrons_phi_(0),
    electrons_charge_(0),
    electrons_chargedIso_(0),
    electrons_neutralIso_(0),
    electrons_photonIso_(0),
    electrons_puIso_(0),
    electrons_r9_(0),
    electrons_ESCOverETrack_(0),
    electrons_DEtaSCTrk_(0),
    electrons_DPhiSCTrk_(0),
    electrons_ecalEnergy_(0),
    electrons_isEB_(0),
    electrons_isEE_(0),
    electrons_isEBGap_(0),
    electrons_isEBEtaGap_(0),
    electrons_isEBPhiGap_(0),
    electrons_isEEGap_(0),
    electrons_isEERingGap_(0),
    electrons_isEEDeeGap_(0),
    electrons_isEBEEGap_(0),
    electrons_isElectron_(0),
    electrons_ecalSeed_(0),
    electrons_trackSeed_(0),
    electrons_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_(0),
    electrons_Ele27_WP80_(0),
    electrons_Ele30_CaloIdVT_TrkIdT_(0),
    electrons_e1x5_(0),
    electrons_e5x5_(0),
    electrons_sigmaIEtaIEta_(0),
    electrons_sigmaIPhiIPhi_(0),
    electrons_hcalDepth1OverEcalBc_(0),
    electrons_hcalDepth2OverEcalBc_(0),
    electrons_x_(0),
    electrons_y_(0),
    electrons_z_(0),
    electrons_energy_(0),
    electrons_rawEnergy_(0),
    electrons_phiWidth_(0),
    electrons_etaWidth_(0),
    photons_px_(0),
    photons_py_(0),
    photons_pz_(0),
    photons_pt_(0),
    photons_eta_(0),
    photons_phi_(0),
    photons_charge_(0),
    photons_x_(0),
    photons_y_(0),
    photons_z_(0),
    photons_energy_(0),
    photons_rawEnergy_(0),
    photons_phiWidth_(0),
    photons_etaWidth_(0),
    photons_e3x3_(0),
    photons_maxCrystalEnergy_(0),
    photons_isEB_(0),
    photons_isEE_(0),
    photons_isPFlowPhoton_(0),
    photons_hasConversionTracks_(0),
    photons_hasPixelSeed_(0),
    vertexs_x_(0),
    vertexs_y_(0),
    vertexs_z_(0),
    vertexs_chi2_(0),
    vertexs_ndof_(0),
    vertexs_nTracks_(0),
    METs_px_(0),
    METs_py_(0),
    METs_et_(0),
    METs_phi_(0),
    genInfo_weight_(0),
    genInfo_pdfid1_(0),
    genInfo_pdfid2_(0),
    genInfo_renScale_(0),
    PUInfos_bx_(0),
    PUInfos_nPU_(0),
    PUInfos_nInteractions_(0),
    genParticles_px_(0),
    genParticles_py_(0),
    genParticles_pz_(0),
    genParticles_pt_(0),
    genParticles_eta_(0),
    genParticles_phi_(0),
    genParticles_charge_(0),
    genParticles_e_(0),
    genParticles_vx_(0),
    genParticles_vy_(0),
    genParticles_vz_(0),
    genParticles_pdgId_(0),
    genParticles_status_(0),
    genParticles_idx_(0),
    genParticles_momIdx_(0),
    genParticles_nDaught_(0),
    genParticles_firstDaughtIdx_(0),
    are_METs_loaded_(0), METs_(),
    are_genInfo_loaded_(0), genInfo_(),
    are_photons_loaded_(0), photons_(),
    are_trigger_loaded_(0), trigger_(),
    are_electrons_loaded_(0), electrons_(),
    are_jets_loaded_(0), jets_(),
    are_muons_loaded_(0), muons_(),
    are_genParticles_loaded_(0), genParticles_(),
    are_vertexs_loaded_(0), vertexs_(),
    are_PUInfos_loaded_(0), PUInfos_()
  {
    tree_ = tree;
    current_entry_ = 0;
    entries_ = tree_->GetEntries();
    tree_->SetBranchStatus("*",0); 
    tree_->SetBranchStatus("run/i", 1); tree_->SetBranchAddress("run/i", &run);
    tree_->SetBranchStatus("lumi/i", 1); tree_->SetBranchAddress("lumi/i", &lumi);
    tree_->SetBranchStatus("evt/l", 1); tree_->SetBranchAddress("evt/l", &evt);
  }

  ~URStreamer()
  {
    //{ EVT_DESTROY }
  }

  bool next(){
    METs_.clear();
    
    photons_.clear();
    
    electrons_.clear();
    jets_.clear();
    muons_.clear();
    genParticles_.clear();
    vertexs_.clear();
    PUInfos_.clear();
    if(current_entry_ < entries_){
      tree_->GetEntry(current_entry_);
      current_entry_++;
      return true;
    }
    return false;
  }

  void loadMets(){
    if(!are_METs_loaded_){
      tree_->SetBranchStatus("METs.px", 1); tree_->SetBranchAddress("METs.px", &METs_px_);
      tree_->SetBranchStatus("METs.py", 1); tree_->SetBranchAddress("METs.py", &METs_py_);
      tree_->SetBranchStatus("METs.et", 1); tree_->SetBranchAddress("METs.et", &METs_et_);
      tree_->SetBranchStatus("METs.phi", 1); tree_->SetBranchAddress("METs.phi", &METs_phi_);
      are_METs_loaded_ = true;
    }
  }
  
  void loadGeninfo(){
    if(!are_genInfo_loaded_){
      tree_->SetBranchStatus("genInfo.weight/F", 1); tree_->SetBranchAddress("genInfo.weight/F", &genInfo_weight_);
      tree_->SetBranchStatus("genInfo.pdfid1/F", 1); tree_->SetBranchAddress("genInfo.pdfid1/F", &genInfo_pdfid1_);
      tree_->SetBranchStatus("genInfo.pdfid2/F", 1); tree_->SetBranchAddress("genInfo.pdfid2/F", &genInfo_pdfid2_);
      tree_->SetBranchStatus("genInfo.renScale/F", 1); tree_->SetBranchAddress("genInfo.renScale/F", &genInfo_renScale_);
      are_genInfo_loaded_ = true;
    }
  }
  
  void loadPhotons(){
    if(!are_photons_loaded_){
      tree_->SetBranchStatus("photons.px", 1); tree_->SetBranchAddress("photons.px", &photons_px_);
      tree_->SetBranchStatus("photons.py", 1); tree_->SetBranchAddress("photons.py", &photons_py_);
      tree_->SetBranchStatus("photons.pz", 1); tree_->SetBranchAddress("photons.pz", &photons_pz_);
      tree_->SetBranchStatus("photons.pt", 1); tree_->SetBranchAddress("photons.pt", &photons_pt_);
      tree_->SetBranchStatus("photons.eta", 1); tree_->SetBranchAddress("photons.eta", &photons_eta_);
      tree_->SetBranchStatus("photons.phi", 1); tree_->SetBranchAddress("photons.phi", &photons_phi_);
      tree_->SetBranchStatus("photons.charge", 1); tree_->SetBranchAddress("photons.charge", &photons_charge_);
      tree_->SetBranchStatus("photons.x", 1); tree_->SetBranchAddress("photons.x", &photons_x_);
      tree_->SetBranchStatus("photons.y", 1); tree_->SetBranchAddress("photons.y", &photons_y_);
      tree_->SetBranchStatus("photons.z", 1); tree_->SetBranchAddress("photons.z", &photons_z_);
      tree_->SetBranchStatus("photons.energy", 1); tree_->SetBranchAddress("photons.energy", &photons_energy_);
      tree_->SetBranchStatus("photons.rawEnergy", 1); tree_->SetBranchAddress("photons.rawEnergy", &photons_rawEnergy_);
      tree_->SetBranchStatus("photons.phiWidth", 1); tree_->SetBranchAddress("photons.phiWidth", &photons_phiWidth_);
      tree_->SetBranchStatus("photons.etaWidth", 1); tree_->SetBranchAddress("photons.etaWidth", &photons_etaWidth_);
      tree_->SetBranchStatus("photons.e3x3", 1); tree_->SetBranchAddress("photons.e3x3", &photons_e3x3_);
      tree_->SetBranchStatus("photons.maxCrystalEnergy", 1); tree_->SetBranchAddress("photons.maxCrystalEnergy", &photons_maxCrystalEnergy_);
      tree_->SetBranchStatus("photons.isEB", 1); tree_->SetBranchAddress("photons.isEB", &photons_isEB_);
      tree_->SetBranchStatus("photons.isEE", 1); tree_->SetBranchAddress("photons.isEE", &photons_isEE_);
      tree_->SetBranchStatus("photons.isPFlowPhoton", 1); tree_->SetBranchAddress("photons.isPFlowPhoton", &photons_isPFlowPhoton_);
      tree_->SetBranchStatus("photons.hasConversionTracks", 1); tree_->SetBranchAddress("photons.hasConversionTracks", &photons_hasConversionTracks_);
      tree_->SetBranchStatus("photons.hasPixelSeed", 1); tree_->SetBranchAddress("photons.hasPixelSeed", &photons_hasPixelSeed_);
      are_photons_loaded_ = true;
    }
  }
  
  void loadTrigger(){
    if(!are_trigger_loaded_){
      tree_->SetBranchStatus("trigger.IsoMu20_eta2p1/O", 1); tree_->SetBranchAddress("trigger.IsoMu20_eta2p1/O", &trigger_IsoMu20_eta2p1_);
      tree_->SetBranchStatus("trigger.IsoMu24/O", 1); tree_->SetBranchAddress("trigger.IsoMu24/O", &trigger_IsoMu24_);
      tree_->SetBranchStatus("trigger.IsoMu24_eta2p1/O", 1); tree_->SetBranchAddress("trigger.IsoMu24_eta2p1/O", &trigger_IsoMu24_eta2p1_);
      tree_->SetBranchStatus("trigger.IsoMu30/O", 1); tree_->SetBranchAddress("trigger.IsoMu30/O", &trigger_IsoMu30_);
      tree_->SetBranchStatus("trigger.IsoMu30_eta2p1/O", 1); tree_->SetBranchAddress("trigger.IsoMu30_eta2p1/O", &trigger_IsoMu30_eta2p1_);
      tree_->SetBranchStatus("trigger.Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL/O", 1); tree_->SetBranchAddress("trigger.Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL/O", &trigger_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_);
      tree_->SetBranchStatus("trigger.Ele27_WP80/O", 1); tree_->SetBranchAddress("trigger.Ele27_WP80/O", &trigger_Ele27_WP80_);
      tree_->SetBranchStatus("trigger.Ele30_CaloIdVT_TrkIdT/O", 1); tree_->SetBranchAddress("trigger.Ele30_CaloIdVT_TrkIdT/O", &trigger_Ele30_CaloIdVT_TrkIdT_);
      are_trigger_loaded_ = true;
    }
  }
  
  void loadElectrons(){
    if(!are_electrons_loaded_){
      tree_->SetBranchStatus("electrons.px", 1); tree_->SetBranchAddress("electrons.px", &electrons_px_);
      tree_->SetBranchStatus("electrons.py", 1); tree_->SetBranchAddress("electrons.py", &electrons_py_);
      tree_->SetBranchStatus("electrons.pz", 1); tree_->SetBranchAddress("electrons.pz", &electrons_pz_);
      tree_->SetBranchStatus("electrons.pt", 1); tree_->SetBranchAddress("electrons.pt", &electrons_pt_);
      tree_->SetBranchStatus("electrons.eta", 1); tree_->SetBranchAddress("electrons.eta", &electrons_eta_);
      tree_->SetBranchStatus("electrons.phi", 1); tree_->SetBranchAddress("electrons.phi", &electrons_phi_);
      tree_->SetBranchStatus("electrons.charge", 1); tree_->SetBranchAddress("electrons.charge", &electrons_charge_);
      tree_->SetBranchStatus("electrons.chargedIso", 1); tree_->SetBranchAddress("electrons.chargedIso", &electrons_chargedIso_);
      tree_->SetBranchStatus("electrons.neutralIso", 1); tree_->SetBranchAddress("electrons.neutralIso", &electrons_neutralIso_);
      tree_->SetBranchStatus("electrons.photonIso", 1); tree_->SetBranchAddress("electrons.photonIso", &electrons_photonIso_);
      tree_->SetBranchStatus("electrons.puIso", 1); tree_->SetBranchAddress("electrons.puIso", &electrons_puIso_);
      tree_->SetBranchStatus("electrons.r9", 1); tree_->SetBranchAddress("electrons.r9", &electrons_r9_);
      tree_->SetBranchStatus("electrons.ESCOverETrack", 1); tree_->SetBranchAddress("electrons.ESCOverETrack", &electrons_ESCOverETrack_);
      tree_->SetBranchStatus("electrons.DEtaSCTrk", 1); tree_->SetBranchAddress("electrons.DEtaSCTrk", &electrons_DEtaSCTrk_);
      tree_->SetBranchStatus("electrons.DPhiSCTrk", 1); tree_->SetBranchAddress("electrons.DPhiSCTrk", &electrons_DPhiSCTrk_);
      tree_->SetBranchStatus("electrons.ecalEnergy", 1); tree_->SetBranchAddress("electrons.ecalEnergy", &electrons_ecalEnergy_);
      tree_->SetBranchStatus("electrons.isEB", 1); tree_->SetBranchAddress("electrons.isEB", &electrons_isEB_);
      tree_->SetBranchStatus("electrons.isEE", 1); tree_->SetBranchAddress("electrons.isEE", &electrons_isEE_);
      tree_->SetBranchStatus("electrons.isEBGap", 1); tree_->SetBranchAddress("electrons.isEBGap", &electrons_isEBGap_);
      tree_->SetBranchStatus("electrons.isEBEtaGap", 1); tree_->SetBranchAddress("electrons.isEBEtaGap", &electrons_isEBEtaGap_);
      tree_->SetBranchStatus("electrons.isEBPhiGap", 1); tree_->SetBranchAddress("electrons.isEBPhiGap", &electrons_isEBPhiGap_);
      tree_->SetBranchStatus("electrons.isEEGap", 1); tree_->SetBranchAddress("electrons.isEEGap", &electrons_isEEGap_);
      tree_->SetBranchStatus("electrons.isEERingGap", 1); tree_->SetBranchAddress("electrons.isEERingGap", &electrons_isEERingGap_);
      tree_->SetBranchStatus("electrons.isEEDeeGap", 1); tree_->SetBranchAddress("electrons.isEEDeeGap", &electrons_isEEDeeGap_);
      tree_->SetBranchStatus("electrons.isEBEEGap", 1); tree_->SetBranchAddress("electrons.isEBEEGap", &electrons_isEBEEGap_);
      tree_->SetBranchStatus("electrons.isElectron", 1); tree_->SetBranchAddress("electrons.isElectron", &electrons_isElectron_);
      tree_->SetBranchStatus("electrons.ecalSeed", 1); tree_->SetBranchAddress("electrons.ecalSeed", &electrons_ecalSeed_);
      tree_->SetBranchStatus("electrons.trackSeed", 1); tree_->SetBranchAddress("electrons.trackSeed", &electrons_trackSeed_);
      tree_->SetBranchStatus("electrons.Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL", 1); tree_->SetBranchAddress("electrons.Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL", &electrons_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_);
      tree_->SetBranchStatus("electrons.Ele27_WP80", 1); tree_->SetBranchAddress("electrons.Ele27_WP80", &electrons_Ele27_WP80_);
      tree_->SetBranchStatus("electrons.Ele30_CaloIdVT_TrkIdT", 1); tree_->SetBranchAddress("electrons.Ele30_CaloIdVT_TrkIdT", &electrons_Ele30_CaloIdVT_TrkIdT_);
      tree_->SetBranchStatus("electrons.e1x5", 1); tree_->SetBranchAddress("electrons.e1x5", &electrons_e1x5_);
      tree_->SetBranchStatus("electrons.e5x5", 1); tree_->SetBranchAddress("electrons.e5x5", &electrons_e5x5_);
      tree_->SetBranchStatus("electrons.sigmaIEtaIEta", 1); tree_->SetBranchAddress("electrons.sigmaIEtaIEta", &electrons_sigmaIEtaIEta_);
      tree_->SetBranchStatus("electrons.sigmaIPhiIPhi", 1); tree_->SetBranchAddress("electrons.sigmaIPhiIPhi", &electrons_sigmaIPhiIPhi_);
      tree_->SetBranchStatus("electrons.hcalDepth1OverEcalBc", 1); tree_->SetBranchAddress("electrons.hcalDepth1OverEcalBc", &electrons_hcalDepth1OverEcalBc_);
      tree_->SetBranchStatus("electrons.hcalDepth2OverEcalBc", 1); tree_->SetBranchAddress("electrons.hcalDepth2OverEcalBc", &electrons_hcalDepth2OverEcalBc_);
      tree_->SetBranchStatus("electrons.x", 1); tree_->SetBranchAddress("electrons.x", &electrons_x_);
      tree_->SetBranchStatus("electrons.y", 1); tree_->SetBranchAddress("electrons.y", &electrons_y_);
      tree_->SetBranchStatus("electrons.z", 1); tree_->SetBranchAddress("electrons.z", &electrons_z_);
      tree_->SetBranchStatus("electrons.energy", 1); tree_->SetBranchAddress("electrons.energy", &electrons_energy_);
      tree_->SetBranchStatus("electrons.rawEnergy", 1); tree_->SetBranchAddress("electrons.rawEnergy", &electrons_rawEnergy_);
      tree_->SetBranchStatus("electrons.phiWidth", 1); tree_->SetBranchAddress("electrons.phiWidth", &electrons_phiWidth_);
      tree_->SetBranchStatus("electrons.etaWidth", 1); tree_->SetBranchAddress("electrons.etaWidth", &electrons_etaWidth_);
      are_electrons_loaded_ = true;
    }
  }
  
  void loadJets(){
    if(!are_jets_loaded_){
      tree_->SetBranchStatus("jets.px", 1); tree_->SetBranchAddress("jets.px", &jets_px_);
      tree_->SetBranchStatus("jets.py", 1); tree_->SetBranchAddress("jets.py", &jets_py_);
      tree_->SetBranchStatus("jets.pz", 1); tree_->SetBranchAddress("jets.pz", &jets_pz_);
      tree_->SetBranchStatus("jets.pt", 1); tree_->SetBranchAddress("jets.pt", &jets_pt_);
      tree_->SetBranchStatus("jets.eta", 1); tree_->SetBranchAddress("jets.eta", &jets_eta_);
      tree_->SetBranchStatus("jets.phi", 1); tree_->SetBranchAddress("jets.phi", &jets_phi_);
      tree_->SetBranchStatus("jets.charge", 1); tree_->SetBranchAddress("jets.charge", &jets_charge_);
      tree_->SetBranchStatus("jets.e", 1); tree_->SetBranchAddress("jets.e", &jets_e_);
      tree_->SetBranchStatus("jets.area", 1); tree_->SetBranchAddress("jets.area", &jets_area_);
      tree_->SetBranchStatus("jets.mass", 1); tree_->SetBranchAddress("jets.mass", &jets_mass_);
      tree_->SetBranchStatus("jets.chargedHadronEnergy", 1); tree_->SetBranchAddress("jets.chargedHadronEnergy", &jets_chargedHadronEnergy_);
      tree_->SetBranchStatus("jets.neutralHadronEnergy", 1); tree_->SetBranchAddress("jets.neutralHadronEnergy", &jets_neutralHadronEnergy_);
      tree_->SetBranchStatus("jets.photonEnergy", 1); tree_->SetBranchAddress("jets.photonEnergy", &jets_photonEnergy_);
      tree_->SetBranchStatus("jets.electronEnergy", 1); tree_->SetBranchAddress("jets.electronEnergy", &jets_electronEnergy_);
      tree_->SetBranchStatus("jets.muonEnergy", 1); tree_->SetBranchAddress("jets.muonEnergy", &jets_muonEnergy_);
      tree_->SetBranchStatus("jets.forwardEM", 1); tree_->SetBranchAddress("jets.forwardEM", &jets_forwardEM_);
      tree_->SetBranchStatus("jets.forwardHad", 1); tree_->SetBranchAddress("jets.forwardHad", &jets_forwardHad_);
      tree_->SetBranchStatus("jets.numChargedHadrons", 1); tree_->SetBranchAddress("jets.numChargedHadrons", &jets_numChargedHadrons_);
      tree_->SetBranchStatus("jets.numNeutralHadrons", 1); tree_->SetBranchAddress("jets.numNeutralHadrons", &jets_numNeutralHadrons_);
      tree_->SetBranchStatus("jets.numPhotons", 1); tree_->SetBranchAddress("jets.numPhotons", &jets_numPhotons_);
      tree_->SetBranchStatus("jets.numElectrons", 1); tree_->SetBranchAddress("jets.numElectrons", &jets_numElectrons_);
      tree_->SetBranchStatus("jets.numMuons", 1); tree_->SetBranchAddress("jets.numMuons", &jets_numMuons_);
      tree_->SetBranchStatus("jets.numForwardEMs", 1); tree_->SetBranchAddress("jets.numForwardEMs", &jets_numForwardEMs_);
      tree_->SetBranchStatus("jets.numForwardHads", 1); tree_->SetBranchAddress("jets.numForwardHads", &jets_numForwardHads_);
      tree_->SetBranchStatus("jets.puId", 1); tree_->SetBranchAddress("jets.puId", &jets_puId_);
      tree_->SetBranchStatus("jets.jetBProb", 1); tree_->SetBranchAddress("jets.jetBProb", &jets_jetBProb_);
      tree_->SetBranchStatus("jets.jetProb", 1); tree_->SetBranchAddress("jets.jetProb", &jets_jetProb_);
      tree_->SetBranchStatus("jets.trkHiPur", 1); tree_->SetBranchAddress("jets.trkHiPur", &jets_trkHiPur_);
      tree_->SetBranchStatus("jets.trkHiEff", 1); tree_->SetBranchAddress("jets.trkHiEff", &jets_trkHiEff_);
      tree_->SetBranchStatus("jets.ssvHiEff", 1); tree_->SetBranchAddress("jets.ssvHiEff", &jets_ssvHiEff_);
      tree_->SetBranchStatus("jets.ssvHiPur", 1); tree_->SetBranchAddress("jets.ssvHiPur", &jets_ssvHiPur_);
      tree_->SetBranchStatus("jets.csv", 1); tree_->SetBranchAddress("jets.csv", &jets_csv_);
      tree_->SetBranchStatus("jets.csvIncl", 1); tree_->SetBranchAddress("jets.csvIncl", &jets_csvIncl_);
      tree_->SetBranchStatus("jets.vtxMass", 1); tree_->SetBranchAddress("jets.vtxMass", &jets_vtxMass_);
      tree_->SetBranchStatus("jets.vtxNtracks", 1); tree_->SetBranchAddress("jets.vtxNtracks", &jets_vtxNtracks_);
      tree_->SetBranchStatus("jets.vtx3DVal", 1); tree_->SetBranchAddress("jets.vtx3DVal", &jets_vtx3DVal_);
      tree_->SetBranchStatus("jets.vtx3DSig", 1); tree_->SetBranchAddress("jets.vtx3DSig", &jets_vtx3DSig_);
      are_jets_loaded_ = true;
    }
  }
  
  void loadMuons(){
    if(!are_muons_loaded_){
      tree_->SetBranchStatus("muons.px", 1); tree_->SetBranchAddress("muons.px", &muons_px_);
      tree_->SetBranchStatus("muons.py", 1); tree_->SetBranchAddress("muons.py", &muons_py_);
      tree_->SetBranchStatus("muons.pz", 1); tree_->SetBranchAddress("muons.pz", &muons_pz_);
      tree_->SetBranchStatus("muons.pt", 1); tree_->SetBranchAddress("muons.pt", &muons_pt_);
      tree_->SetBranchStatus("muons.eta", 1); tree_->SetBranchAddress("muons.eta", &muons_eta_);
      tree_->SetBranchStatus("muons.phi", 1); tree_->SetBranchAddress("muons.phi", &muons_phi_);
      tree_->SetBranchStatus("muons.charge", 1); tree_->SetBranchAddress("muons.charge", &muons_charge_);
      tree_->SetBranchStatus("muons.chargedIso", 1); tree_->SetBranchAddress("muons.chargedIso", &muons_chargedIso_);
      tree_->SetBranchStatus("muons.neutralIso", 1); tree_->SetBranchAddress("muons.neutralIso", &muons_neutralIso_);
      tree_->SetBranchStatus("muons.photonIso", 1); tree_->SetBranchAddress("muons.photonIso", &muons_photonIso_);
      tree_->SetBranchStatus("muons.puIso", 1); tree_->SetBranchAddress("muons.puIso", &muons_puIso_);
      tree_->SetBranchStatus("muons.ECalEnergy", 1); tree_->SetBranchAddress("muons.ECalEnergy", &muons_ECalEnergy_);
      tree_->SetBranchStatus("muons.HCalEnergy", 1); tree_->SetBranchAddress("muons.HCalEnergy", &muons_HCalEnergy_);
      tree_->SetBranchStatus("muons.numChambers", 1); tree_->SetBranchAddress("muons.numChambers", &muons_numChambers_);
      tree_->SetBranchStatus("muons.numMatchedStations", 1); tree_->SetBranchAddress("muons.numMatchedStations", &muons_numMatchedStations_);
      tree_->SetBranchStatus("muons.ptErr", 1); tree_->SetBranchAddress("muons.ptErr", &muons_ptErr_);
      tree_->SetBranchStatus("muons.chi2", 1); tree_->SetBranchAddress("muons.chi2", &muons_chi2_);
      tree_->SetBranchStatus("muons.ndof", 1); tree_->SetBranchAddress("muons.ndof", &muons_ndof_);
      tree_->SetBranchStatus("muons.validHits", 1); tree_->SetBranchAddress("muons.validHits", &muons_validHits_);
      tree_->SetBranchStatus("muons.isGlobal", 1); tree_->SetBranchAddress("muons.isGlobal", &muons_isGlobal_);
      tree_->SetBranchStatus("muons.isTracker", 1); tree_->SetBranchAddress("muons.isTracker", &muons_isTracker_);
      tree_->SetBranchStatus("muons.isCalo", 1); tree_->SetBranchAddress("muons.isCalo", &muons_isCalo_);
      tree_->SetBranchStatus("muons.isPF", 1); tree_->SetBranchAddress("muons.isPF", &muons_isPF_);
      tree_->SetBranchStatus("muons.isStandAlone", 1); tree_->SetBranchAddress("muons.isStandAlone", &muons_isStandAlone_);
      tree_->SetBranchStatus("muons.isLoose", 1); tree_->SetBranchAddress("muons.isLoose", &muons_isLoose_);
      tree_->SetBranchStatus("muons.IsoMu20_eta2p1", 1); tree_->SetBranchAddress("muons.IsoMu20_eta2p1", &muons_IsoMu20_eta2p1_);
      tree_->SetBranchStatus("muons.IsoMu24", 1); tree_->SetBranchAddress("muons.IsoMu24", &muons_IsoMu24_);
      tree_->SetBranchStatus("muons.IsoMu24_eta2p1", 1); tree_->SetBranchAddress("muons.IsoMu24_eta2p1", &muons_IsoMu24_eta2p1_);
      tree_->SetBranchStatus("muons.IsoMu30", 1); tree_->SetBranchAddress("muons.IsoMu30", &muons_IsoMu30_);
      tree_->SetBranchStatus("muons.IsoMu30_eta2p1", 1); tree_->SetBranchAddress("muons.IsoMu30_eta2p1", &muons_IsoMu30_eta2p1_);
      are_muons_loaded_ = true;
    }
  }
  
  void loadGenparticles(){
    if(!are_genParticles_loaded_){
      tree_->SetBranchStatus("genParticles.px", 1); tree_->SetBranchAddress("genParticles.px", &genParticles_px_);
      tree_->SetBranchStatus("genParticles.py", 1); tree_->SetBranchAddress("genParticles.py", &genParticles_py_);
      tree_->SetBranchStatus("genParticles.pz", 1); tree_->SetBranchAddress("genParticles.pz", &genParticles_pz_);
      tree_->SetBranchStatus("genParticles.pt", 1); tree_->SetBranchAddress("genParticles.pt", &genParticles_pt_);
      tree_->SetBranchStatus("genParticles.eta", 1); tree_->SetBranchAddress("genParticles.eta", &genParticles_eta_);
      tree_->SetBranchStatus("genParticles.phi", 1); tree_->SetBranchAddress("genParticles.phi", &genParticles_phi_);
      tree_->SetBranchStatus("genParticles.charge", 1); tree_->SetBranchAddress("genParticles.charge", &genParticles_charge_);
      tree_->SetBranchStatus("genParticles.e", 1); tree_->SetBranchAddress("genParticles.e", &genParticles_e_);
      tree_->SetBranchStatus("genParticles.vx", 1); tree_->SetBranchAddress("genParticles.vx", &genParticles_vx_);
      tree_->SetBranchStatus("genParticles.vy", 1); tree_->SetBranchAddress("genParticles.vy", &genParticles_vy_);
      tree_->SetBranchStatus("genParticles.vz", 1); tree_->SetBranchAddress("genParticles.vz", &genParticles_vz_);
      tree_->SetBranchStatus("genParticles.pdgId", 1); tree_->SetBranchAddress("genParticles.pdgId", &genParticles_pdgId_);
      tree_->SetBranchStatus("genParticles.status", 1); tree_->SetBranchAddress("genParticles.status", &genParticles_status_);
      tree_->SetBranchStatus("genParticles.idx", 1); tree_->SetBranchAddress("genParticles.idx", &genParticles_idx_);
      tree_->SetBranchStatus("genParticles.momIdx", 1); tree_->SetBranchAddress("genParticles.momIdx", &genParticles_momIdx_);
      tree_->SetBranchStatus("genParticles.nDaught", 1); tree_->SetBranchAddress("genParticles.nDaught", &genParticles_nDaught_);
      tree_->SetBranchStatus("genParticles.firstDaughtIdx", 1); tree_->SetBranchAddress("genParticles.firstDaughtIdx", &genParticles_firstDaughtIdx_);
      are_genParticles_loaded_ = true;
    }
  }
  
  void loadVertexs(){
    if(!are_vertexs_loaded_){
      tree_->SetBranchStatus("vertexs.x", 1); tree_->SetBranchAddress("vertexs.x", &vertexs_x_);
      tree_->SetBranchStatus("vertexs.y", 1); tree_->SetBranchAddress("vertexs.y", &vertexs_y_);
      tree_->SetBranchStatus("vertexs.z", 1); tree_->SetBranchAddress("vertexs.z", &vertexs_z_);
      tree_->SetBranchStatus("vertexs.chi2", 1); tree_->SetBranchAddress("vertexs.chi2", &vertexs_chi2_);
      tree_->SetBranchStatus("vertexs.ndof", 1); tree_->SetBranchAddress("vertexs.ndof", &vertexs_ndof_);
      tree_->SetBranchStatus("vertexs.nTracks", 1); tree_->SetBranchAddress("vertexs.nTracks", &vertexs_nTracks_);
      are_vertexs_loaded_ = true;
    }
  }
  
  void loadPuinfos(){
    if(!are_PUInfos_loaded_){
      tree_->SetBranchStatus("PUInfos.bx", 1); tree_->SetBranchAddress("PUInfos.bx", &PUInfos_bx_);
      tree_->SetBranchStatus("PUInfos.nPU", 1); tree_->SetBranchAddress("PUInfos.nPU", &PUInfos_nPU_);
      tree_->SetBranchStatus("PUInfos.nInteractions", 1); tree_->SetBranchAddress("PUInfos.nInteractions", &PUInfos_nInteractions_);
      are_PUInfos_loaded_ = true;
    }
  }
  

  const vector<Met>& METs(){
    if(METs_.size() > 0) return METs_;
    loadMets();
    auto it_METs_px_ = METs_px_->cbegin();
    auto it_METs_py_ = METs_py_->cbegin();
    auto it_METs_et_ = METs_et_->cbegin();
    auto it_METs_phi_ = METs_phi_->cbegin();
    for(; it_METs_px_ != METs_px_->cend(); ){
      Met obj;
      obj.setpx(*it_METs_px_);
      obj.setpy(*it_METs_py_);
      obj.setet(*it_METs_et_);
      obj.setphi(*it_METs_phi_);
      METs_.push_back( obj );
      ++it_METs_px_;
      ++it_METs_py_;
      ++it_METs_et_;
      ++it_METs_phi_;
    }
    return METs_;
  }
  
  
  const vector<Photon>& photons(){
    if(photons_.size() > 0) return photons_;
    loadPhotons();
    auto it_photons_px_ = photons_px_->cbegin();
    auto it_photons_py_ = photons_py_->cbegin();
    auto it_photons_pz_ = photons_pz_->cbegin();
    auto it_photons_pt_ = photons_pt_->cbegin();
    auto it_photons_eta_ = photons_eta_->cbegin();
    auto it_photons_phi_ = photons_phi_->cbegin();
    auto it_photons_charge_ = photons_charge_->cbegin();
    auto it_photons_x_ = photons_x_->cbegin();
    auto it_photons_y_ = photons_y_->cbegin();
    auto it_photons_z_ = photons_z_->cbegin();
    auto it_photons_energy_ = photons_energy_->cbegin();
    auto it_photons_rawEnergy_ = photons_rawEnergy_->cbegin();
    auto it_photons_phiWidth_ = photons_phiWidth_->cbegin();
    auto it_photons_etaWidth_ = photons_etaWidth_->cbegin();
    auto it_photons_e3x3_ = photons_e3x3_->cbegin();
    auto it_photons_maxCrystalEnergy_ = photons_maxCrystalEnergy_->cbegin();
    auto it_photons_isEB_ = photons_isEB_->cbegin();
    auto it_photons_isEE_ = photons_isEE_->cbegin();
    auto it_photons_isPFlowPhoton_ = photons_isPFlowPhoton_->cbegin();
    auto it_photons_hasConversionTracks_ = photons_hasConversionTracks_->cbegin();
    auto it_photons_hasPixelSeed_ = photons_hasPixelSeed_->cbegin();
    for(; it_photons_px_ != photons_px_->cend(); ){
      Photon obj;
      obj.setpx(*it_photons_px_);
      obj.setpy(*it_photons_py_);
      obj.setpz(*it_photons_pz_);
      obj.setpt(*it_photons_pt_);
      obj.seteta(*it_photons_eta_);
      obj.setphi(*it_photons_phi_);
      obj.setcharge(*it_photons_charge_);
      obj.setx(*it_photons_x_);
      obj.sety(*it_photons_y_);
      obj.setz(*it_photons_z_);
      obj.setenergy(*it_photons_energy_);
      obj.setrawEnergy(*it_photons_rawEnergy_);
      obj.setphiWidth(*it_photons_phiWidth_);
      obj.setetaWidth(*it_photons_etaWidth_);
      obj.sete3x3(*it_photons_e3x3_);
      obj.setmaxCrystalEnergy(*it_photons_maxCrystalEnergy_);
      obj.setisEB(*it_photons_isEB_);
      obj.setisEE(*it_photons_isEE_);
      obj.setisPFlowPhoton(*it_photons_isPFlowPhoton_);
      obj.sethasConversionTracks(*it_photons_hasConversionTracks_);
      obj.sethasPixelSeed(*it_photons_hasPixelSeed_);
      photons_.push_back( obj );
      ++it_photons_px_;
      ++it_photons_py_;
      ++it_photons_pz_;
      ++it_photons_pt_;
      ++it_photons_eta_;
      ++it_photons_phi_;
      ++it_photons_charge_;
      ++it_photons_x_;
      ++it_photons_y_;
      ++it_photons_z_;
      ++it_photons_energy_;
      ++it_photons_rawEnergy_;
      ++it_photons_phiWidth_;
      ++it_photons_etaWidth_;
      ++it_photons_e3x3_;
      ++it_photons_maxCrystalEnergy_;
      ++it_photons_isEB_;
      ++it_photons_isEE_;
      ++it_photons_isPFlowPhoton_;
      ++it_photons_hasConversionTracks_;
      ++it_photons_hasPixelSeed_;
    }
    return photons_;
  }
  
  
  const vector<Electron>& electrons(){
    if(electrons_.size() > 0) return electrons_;
    loadElectrons();
    auto it_electrons_px_ = electrons_px_->cbegin();
    auto it_electrons_py_ = electrons_py_->cbegin();
    auto it_electrons_pz_ = electrons_pz_->cbegin();
    auto it_electrons_pt_ = electrons_pt_->cbegin();
    auto it_electrons_eta_ = electrons_eta_->cbegin();
    auto it_electrons_phi_ = electrons_phi_->cbegin();
    auto it_electrons_charge_ = electrons_charge_->cbegin();
    auto it_electrons_chargedIso_ = electrons_chargedIso_->cbegin();
    auto it_electrons_neutralIso_ = electrons_neutralIso_->cbegin();
    auto it_electrons_photonIso_ = electrons_photonIso_->cbegin();
    auto it_electrons_puIso_ = electrons_puIso_->cbegin();
    auto it_electrons_r9_ = electrons_r9_->cbegin();
    auto it_electrons_ESCOverETrack_ = electrons_ESCOverETrack_->cbegin();
    auto it_electrons_DEtaSCTrk_ = electrons_DEtaSCTrk_->cbegin();
    auto it_electrons_DPhiSCTrk_ = electrons_DPhiSCTrk_->cbegin();
    auto it_electrons_ecalEnergy_ = electrons_ecalEnergy_->cbegin();
    auto it_electrons_isEB_ = electrons_isEB_->cbegin();
    auto it_electrons_isEE_ = electrons_isEE_->cbegin();
    auto it_electrons_isEBGap_ = electrons_isEBGap_->cbegin();
    auto it_electrons_isEBEtaGap_ = electrons_isEBEtaGap_->cbegin();
    auto it_electrons_isEBPhiGap_ = electrons_isEBPhiGap_->cbegin();
    auto it_electrons_isEEGap_ = electrons_isEEGap_->cbegin();
    auto it_electrons_isEERingGap_ = electrons_isEERingGap_->cbegin();
    auto it_electrons_isEEDeeGap_ = electrons_isEEDeeGap_->cbegin();
    auto it_electrons_isEBEEGap_ = electrons_isEBEEGap_->cbegin();
    auto it_electrons_isElectron_ = electrons_isElectron_->cbegin();
    auto it_electrons_ecalSeed_ = electrons_ecalSeed_->cbegin();
    auto it_electrons_trackSeed_ = electrons_trackSeed_->cbegin();
    auto it_electrons_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_ = electrons_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_->cbegin();
    auto it_electrons_Ele27_WP80_ = electrons_Ele27_WP80_->cbegin();
    auto it_electrons_Ele30_CaloIdVT_TrkIdT_ = electrons_Ele30_CaloIdVT_TrkIdT_->cbegin();
    auto it_electrons_e1x5_ = electrons_e1x5_->cbegin();
    auto it_electrons_e5x5_ = electrons_e5x5_->cbegin();
    auto it_electrons_sigmaIEtaIEta_ = electrons_sigmaIEtaIEta_->cbegin();
    auto it_electrons_sigmaIPhiIPhi_ = electrons_sigmaIPhiIPhi_->cbegin();
    auto it_electrons_hcalDepth1OverEcalBc_ = electrons_hcalDepth1OverEcalBc_->cbegin();
    auto it_electrons_hcalDepth2OverEcalBc_ = electrons_hcalDepth2OverEcalBc_->cbegin();
    auto it_electrons_x_ = electrons_x_->cbegin();
    auto it_electrons_y_ = electrons_y_->cbegin();
    auto it_electrons_z_ = electrons_z_->cbegin();
    auto it_electrons_energy_ = electrons_energy_->cbegin();
    auto it_electrons_rawEnergy_ = electrons_rawEnergy_->cbegin();
    auto it_electrons_phiWidth_ = electrons_phiWidth_->cbegin();
    auto it_electrons_etaWidth_ = electrons_etaWidth_->cbegin();
    for(; it_electrons_px_ != electrons_px_->cend(); ){
      Electron obj;
      obj.setpx(*it_electrons_px_);
      obj.setpy(*it_electrons_py_);
      obj.setpz(*it_electrons_pz_);
      obj.setpt(*it_electrons_pt_);
      obj.seteta(*it_electrons_eta_);
      obj.setphi(*it_electrons_phi_);
      obj.setcharge(*it_electrons_charge_);
      obj.setchargedIso(*it_electrons_chargedIso_);
      obj.setneutralIso(*it_electrons_neutralIso_);
      obj.setphotonIso(*it_electrons_photonIso_);
      obj.setpuIso(*it_electrons_puIso_);
      obj.setr9(*it_electrons_r9_);
      obj.setESCOverETrack(*it_electrons_ESCOverETrack_);
      obj.setDEtaSCTrk(*it_electrons_DEtaSCTrk_);
      obj.setDPhiSCTrk(*it_electrons_DPhiSCTrk_);
      obj.setecalEnergy(*it_electrons_ecalEnergy_);
      obj.setisEB(*it_electrons_isEB_);
      obj.setisEE(*it_electrons_isEE_);
      obj.setisEBGap(*it_electrons_isEBGap_);
      obj.setisEBEtaGap(*it_electrons_isEBEtaGap_);
      obj.setisEBPhiGap(*it_electrons_isEBPhiGap_);
      obj.setisEEGap(*it_electrons_isEEGap_);
      obj.setisEERingGap(*it_electrons_isEERingGap_);
      obj.setisEEDeeGap(*it_electrons_isEEDeeGap_);
      obj.setisEBEEGap(*it_electrons_isEBEEGap_);
      obj.setisElectron(*it_electrons_isElectron_);
      obj.setecalSeed(*it_electrons_ecalSeed_);
      obj.settrackSeed(*it_electrons_trackSeed_);
      obj.setEle27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL(*it_electrons_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_);
      obj.setEle27_WP80(*it_electrons_Ele27_WP80_);
      obj.setEle30_CaloIdVT_TrkIdT(*it_electrons_Ele30_CaloIdVT_TrkIdT_);
      obj.sete1x5(*it_electrons_e1x5_);
      obj.sete5x5(*it_electrons_e5x5_);
      obj.setsigmaIEtaIEta(*it_electrons_sigmaIEtaIEta_);
      obj.setsigmaIPhiIPhi(*it_electrons_sigmaIPhiIPhi_);
      obj.sethcalDepth1OverEcalBc(*it_electrons_hcalDepth1OverEcalBc_);
      obj.sethcalDepth2OverEcalBc(*it_electrons_hcalDepth2OverEcalBc_);
      obj.setx(*it_electrons_x_);
      obj.sety(*it_electrons_y_);
      obj.setz(*it_electrons_z_);
      obj.setenergy(*it_electrons_energy_);
      obj.setrawEnergy(*it_electrons_rawEnergy_);
      obj.setphiWidth(*it_electrons_phiWidth_);
      obj.setetaWidth(*it_electrons_etaWidth_);
      electrons_.push_back( obj );
      ++it_electrons_px_;
      ++it_electrons_py_;
      ++it_electrons_pz_;
      ++it_electrons_pt_;
      ++it_electrons_eta_;
      ++it_electrons_phi_;
      ++it_electrons_charge_;
      ++it_electrons_chargedIso_;
      ++it_electrons_neutralIso_;
      ++it_electrons_photonIso_;
      ++it_electrons_puIso_;
      ++it_electrons_r9_;
      ++it_electrons_ESCOverETrack_;
      ++it_electrons_DEtaSCTrk_;
      ++it_electrons_DPhiSCTrk_;
      ++it_electrons_ecalEnergy_;
      ++it_electrons_isEB_;
      ++it_electrons_isEE_;
      ++it_electrons_isEBGap_;
      ++it_electrons_isEBEtaGap_;
      ++it_electrons_isEBPhiGap_;
      ++it_electrons_isEEGap_;
      ++it_electrons_isEERingGap_;
      ++it_electrons_isEEDeeGap_;
      ++it_electrons_isEBEEGap_;
      ++it_electrons_isElectron_;
      ++it_electrons_ecalSeed_;
      ++it_electrons_trackSeed_;
      ++it_electrons_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_;
      ++it_electrons_Ele27_WP80_;
      ++it_electrons_Ele30_CaloIdVT_TrkIdT_;
      ++it_electrons_e1x5_;
      ++it_electrons_e5x5_;
      ++it_electrons_sigmaIEtaIEta_;
      ++it_electrons_sigmaIPhiIPhi_;
      ++it_electrons_hcalDepth1OverEcalBc_;
      ++it_electrons_hcalDepth2OverEcalBc_;
      ++it_electrons_x_;
      ++it_electrons_y_;
      ++it_electrons_z_;
      ++it_electrons_energy_;
      ++it_electrons_rawEnergy_;
      ++it_electrons_phiWidth_;
      ++it_electrons_etaWidth_;
    }
    return electrons_;
  }
  
  const vector<Jet>& jets(){
    if(jets_.size() > 0) return jets_;
    loadJets();
    auto it_jets_px_ = jets_px_->cbegin();
    auto it_jets_py_ = jets_py_->cbegin();
    auto it_jets_pz_ = jets_pz_->cbegin();
    auto it_jets_pt_ = jets_pt_->cbegin();
    auto it_jets_eta_ = jets_eta_->cbegin();
    auto it_jets_phi_ = jets_phi_->cbegin();
    auto it_jets_charge_ = jets_charge_->cbegin();
    auto it_jets_e_ = jets_e_->cbegin();
    auto it_jets_area_ = jets_area_->cbegin();
    auto it_jets_mass_ = jets_mass_->cbegin();
    auto it_jets_chargedHadronEnergy_ = jets_chargedHadronEnergy_->cbegin();
    auto it_jets_neutralHadronEnergy_ = jets_neutralHadronEnergy_->cbegin();
    auto it_jets_photonEnergy_ = jets_photonEnergy_->cbegin();
    auto it_jets_electronEnergy_ = jets_electronEnergy_->cbegin();
    auto it_jets_muonEnergy_ = jets_muonEnergy_->cbegin();
    auto it_jets_forwardEM_ = jets_forwardEM_->cbegin();
    auto it_jets_forwardHad_ = jets_forwardHad_->cbegin();
    auto it_jets_numChargedHadrons_ = jets_numChargedHadrons_->cbegin();
    auto it_jets_numNeutralHadrons_ = jets_numNeutralHadrons_->cbegin();
    auto it_jets_numPhotons_ = jets_numPhotons_->cbegin();
    auto it_jets_numElectrons_ = jets_numElectrons_->cbegin();
    auto it_jets_numMuons_ = jets_numMuons_->cbegin();
    auto it_jets_numForwardEMs_ = jets_numForwardEMs_->cbegin();
    auto it_jets_numForwardHads_ = jets_numForwardHads_->cbegin();
    auto it_jets_puId_ = jets_puId_->cbegin();
    auto it_jets_jetBProb_ = jets_jetBProb_->cbegin();
    auto it_jets_jetProb_ = jets_jetProb_->cbegin();
    auto it_jets_trkHiPur_ = jets_trkHiPur_->cbegin();
    auto it_jets_trkHiEff_ = jets_trkHiEff_->cbegin();
    auto it_jets_ssvHiEff_ = jets_ssvHiEff_->cbegin();
    auto it_jets_ssvHiPur_ = jets_ssvHiPur_->cbegin();
    auto it_jets_csv_ = jets_csv_->cbegin();
    auto it_jets_csvIncl_ = jets_csvIncl_->cbegin();
    auto it_jets_vtxMass_ = jets_vtxMass_->cbegin();
    auto it_jets_vtxNtracks_ = jets_vtxNtracks_->cbegin();
    auto it_jets_vtx3DVal_ = jets_vtx3DVal_->cbegin();
    auto it_jets_vtx3DSig_ = jets_vtx3DSig_->cbegin();
    for(; it_jets_px_ != jets_px_->cend(); ){
      Jet obj;
      obj.setpx(*it_jets_px_);
      obj.setpy(*it_jets_py_);
      obj.setpz(*it_jets_pz_);
      obj.setpt(*it_jets_pt_);
      obj.seteta(*it_jets_eta_);
      obj.setphi(*it_jets_phi_);
      obj.setcharge(*it_jets_charge_);
      obj.sete(*it_jets_e_);
      obj.setarea(*it_jets_area_);
      obj.setmass(*it_jets_mass_);
      obj.setchargedHadronEnergy(*it_jets_chargedHadronEnergy_);
      obj.setneutralHadronEnergy(*it_jets_neutralHadronEnergy_);
      obj.setphotonEnergy(*it_jets_photonEnergy_);
      obj.setelectronEnergy(*it_jets_electronEnergy_);
      obj.setmuonEnergy(*it_jets_muonEnergy_);
      obj.setforwardEM(*it_jets_forwardEM_);
      obj.setforwardHad(*it_jets_forwardHad_);
      obj.setnumChargedHadrons(*it_jets_numChargedHadrons_);
      obj.setnumNeutralHadrons(*it_jets_numNeutralHadrons_);
      obj.setnumPhotons(*it_jets_numPhotons_);
      obj.setnumElectrons(*it_jets_numElectrons_);
      obj.setnumMuons(*it_jets_numMuons_);
      obj.setnumForwardEMs(*it_jets_numForwardEMs_);
      obj.setnumForwardHads(*it_jets_numForwardHads_);
      obj.setpuId(*it_jets_puId_);
      obj.setjetBProb(*it_jets_jetBProb_);
      obj.setjetProb(*it_jets_jetProb_);
      obj.settrkHiPur(*it_jets_trkHiPur_);
      obj.settrkHiEff(*it_jets_trkHiEff_);
      obj.setssvHiEff(*it_jets_ssvHiEff_);
      obj.setssvHiPur(*it_jets_ssvHiPur_);
      obj.setcsv(*it_jets_csv_);
      obj.setcsvIncl(*it_jets_csvIncl_);
      obj.setvtxMass(*it_jets_vtxMass_);
      obj.setvtxNtracks(*it_jets_vtxNtracks_);
      obj.setvtx3DVal(*it_jets_vtx3DVal_);
      obj.setvtx3DSig(*it_jets_vtx3DSig_);
      jets_.push_back( obj );
      ++it_jets_px_;
      ++it_jets_py_;
      ++it_jets_pz_;
      ++it_jets_pt_;
      ++it_jets_eta_;
      ++it_jets_phi_;
      ++it_jets_charge_;
      ++it_jets_e_;
      ++it_jets_area_;
      ++it_jets_mass_;
      ++it_jets_chargedHadronEnergy_;
      ++it_jets_neutralHadronEnergy_;
      ++it_jets_photonEnergy_;
      ++it_jets_electronEnergy_;
      ++it_jets_muonEnergy_;
      ++it_jets_forwardEM_;
      ++it_jets_forwardHad_;
      ++it_jets_numChargedHadrons_;
      ++it_jets_numNeutralHadrons_;
      ++it_jets_numPhotons_;
      ++it_jets_numElectrons_;
      ++it_jets_numMuons_;
      ++it_jets_numForwardEMs_;
      ++it_jets_numForwardHads_;
      ++it_jets_puId_;
      ++it_jets_jetBProb_;
      ++it_jets_jetProb_;
      ++it_jets_trkHiPur_;
      ++it_jets_trkHiEff_;
      ++it_jets_ssvHiEff_;
      ++it_jets_ssvHiPur_;
      ++it_jets_csv_;
      ++it_jets_csvIncl_;
      ++it_jets_vtxMass_;
      ++it_jets_vtxNtracks_;
      ++it_jets_vtx3DVal_;
      ++it_jets_vtx3DSig_;
    }
    return jets_;
  }
  
  const vector<Muon>& muons(){
    if(muons_.size() > 0) return muons_;
    loadMuons();
    auto it_muons_px_ = muons_px_->cbegin();
    auto it_muons_py_ = muons_py_->cbegin();
    auto it_muons_pz_ = muons_pz_->cbegin();
    auto it_muons_pt_ = muons_pt_->cbegin();
    auto it_muons_eta_ = muons_eta_->cbegin();
    auto it_muons_phi_ = muons_phi_->cbegin();
    auto it_muons_charge_ = muons_charge_->cbegin();
    auto it_muons_chargedIso_ = muons_chargedIso_->cbegin();
    auto it_muons_neutralIso_ = muons_neutralIso_->cbegin();
    auto it_muons_photonIso_ = muons_photonIso_->cbegin();
    auto it_muons_puIso_ = muons_puIso_->cbegin();
    auto it_muons_ECalEnergy_ = muons_ECalEnergy_->cbegin();
    auto it_muons_HCalEnergy_ = muons_HCalEnergy_->cbegin();
    auto it_muons_numChambers_ = muons_numChambers_->cbegin();
    auto it_muons_numMatchedStations_ = muons_numMatchedStations_->cbegin();
    auto it_muons_ptErr_ = muons_ptErr_->cbegin();
    auto it_muons_chi2_ = muons_chi2_->cbegin();
    auto it_muons_ndof_ = muons_ndof_->cbegin();
    auto it_muons_validHits_ = muons_validHits_->cbegin();
    auto it_muons_isGlobal_ = muons_isGlobal_->cbegin();
    auto it_muons_isTracker_ = muons_isTracker_->cbegin();
    auto it_muons_isCalo_ = muons_isCalo_->cbegin();
    auto it_muons_isPF_ = muons_isPF_->cbegin();
    auto it_muons_isStandAlone_ = muons_isStandAlone_->cbegin();
    auto it_muons_isLoose_ = muons_isLoose_->cbegin();
    auto it_muons_IsoMu20_eta2p1_ = muons_IsoMu20_eta2p1_->cbegin();
    auto it_muons_IsoMu24_ = muons_IsoMu24_->cbegin();
    auto it_muons_IsoMu24_eta2p1_ = muons_IsoMu24_eta2p1_->cbegin();
    auto it_muons_IsoMu30_ = muons_IsoMu30_->cbegin();
    auto it_muons_IsoMu30_eta2p1_ = muons_IsoMu30_eta2p1_->cbegin();
    for(; it_muons_px_ != muons_px_->cend(); ){
      Muon obj;
      obj.setpx(*it_muons_px_);
      obj.setpy(*it_muons_py_);
      obj.setpz(*it_muons_pz_);
      obj.setpt(*it_muons_pt_);
      obj.seteta(*it_muons_eta_);
      obj.setphi(*it_muons_phi_);
      obj.setcharge(*it_muons_charge_);
      obj.setchargedIso(*it_muons_chargedIso_);
      obj.setneutralIso(*it_muons_neutralIso_);
      obj.setphotonIso(*it_muons_photonIso_);
      obj.setpuIso(*it_muons_puIso_);
      obj.setECalEnergy(*it_muons_ECalEnergy_);
      obj.setHCalEnergy(*it_muons_HCalEnergy_);
      obj.setnumChambers(*it_muons_numChambers_);
      obj.setnumMatchedStations(*it_muons_numMatchedStations_);
      obj.setptErr(*it_muons_ptErr_);
      obj.setchi2(*it_muons_chi2_);
      obj.setndof(*it_muons_ndof_);
      obj.setvalidHits(*it_muons_validHits_);
      obj.setisGlobal(*it_muons_isGlobal_);
      obj.setisTracker(*it_muons_isTracker_);
      obj.setisCalo(*it_muons_isCalo_);
      obj.setisPF(*it_muons_isPF_);
      obj.setisStandAlone(*it_muons_isStandAlone_);
      obj.setisLoose(*it_muons_isLoose_);
      obj.setIsoMu20_eta2p1(*it_muons_IsoMu20_eta2p1_);
      obj.setIsoMu24(*it_muons_IsoMu24_);
      obj.setIsoMu24_eta2p1(*it_muons_IsoMu24_eta2p1_);
      obj.setIsoMu30(*it_muons_IsoMu30_);
      obj.setIsoMu30_eta2p1(*it_muons_IsoMu30_eta2p1_);
      muons_.push_back( obj );
      ++it_muons_px_;
      ++it_muons_py_;
      ++it_muons_pz_;
      ++it_muons_pt_;
      ++it_muons_eta_;
      ++it_muons_phi_;
      ++it_muons_charge_;
      ++it_muons_chargedIso_;
      ++it_muons_neutralIso_;
      ++it_muons_photonIso_;
      ++it_muons_puIso_;
      ++it_muons_ECalEnergy_;
      ++it_muons_HCalEnergy_;
      ++it_muons_numChambers_;
      ++it_muons_numMatchedStations_;
      ++it_muons_ptErr_;
      ++it_muons_chi2_;
      ++it_muons_ndof_;
      ++it_muons_validHits_;
      ++it_muons_isGlobal_;
      ++it_muons_isTracker_;
      ++it_muons_isCalo_;
      ++it_muons_isPF_;
      ++it_muons_isStandAlone_;
      ++it_muons_isLoose_;
      ++it_muons_IsoMu20_eta2p1_;
      ++it_muons_IsoMu24_;
      ++it_muons_IsoMu24_eta2p1_;
      ++it_muons_IsoMu30_;
      ++it_muons_IsoMu30_eta2p1_;
    }
    return muons_;
  }
  
  const vector<Genparticle>& genParticles(){
    if(genParticles_.size() > 0) return genParticles_;
    loadGenparticles();
    auto it_genParticles_px_ = genParticles_px_->cbegin();
    auto it_genParticles_py_ = genParticles_py_->cbegin();
    auto it_genParticles_pz_ = genParticles_pz_->cbegin();
    auto it_genParticles_pt_ = genParticles_pt_->cbegin();
    auto it_genParticles_eta_ = genParticles_eta_->cbegin();
    auto it_genParticles_phi_ = genParticles_phi_->cbegin();
    auto it_genParticles_charge_ = genParticles_charge_->cbegin();
    auto it_genParticles_e_ = genParticles_e_->cbegin();
    auto it_genParticles_vx_ = genParticles_vx_->cbegin();
    auto it_genParticles_vy_ = genParticles_vy_->cbegin();
    auto it_genParticles_vz_ = genParticles_vz_->cbegin();
    auto it_genParticles_pdgId_ = genParticles_pdgId_->cbegin();
    auto it_genParticles_status_ = genParticles_status_->cbegin();
    auto it_genParticles_idx_ = genParticles_idx_->cbegin();
    auto it_genParticles_momIdx_ = genParticles_momIdx_->cbegin();
    auto it_genParticles_nDaught_ = genParticles_nDaught_->cbegin();
    auto it_genParticles_firstDaughtIdx_ = genParticles_firstDaughtIdx_->cbegin();
    for(; it_genParticles_px_ != genParticles_px_->cend(); ){
      Genparticle obj;
      obj.setpx(*it_genParticles_px_);
      obj.setpy(*it_genParticles_py_);
      obj.setpz(*it_genParticles_pz_);
      obj.setpt(*it_genParticles_pt_);
      obj.seteta(*it_genParticles_eta_);
      obj.setphi(*it_genParticles_phi_);
      obj.setcharge(*it_genParticles_charge_);
      obj.sete(*it_genParticles_e_);
      obj.setvx(*it_genParticles_vx_);
      obj.setvy(*it_genParticles_vy_);
      obj.setvz(*it_genParticles_vz_);
      obj.setpdgId(*it_genParticles_pdgId_);
      obj.setstatus(*it_genParticles_status_);
      obj.setidx(*it_genParticles_idx_);
      obj.setmomIdx(*it_genParticles_momIdx_);
      obj.setnDaught(*it_genParticles_nDaught_);
      obj.setfirstDaughtIdx(*it_genParticles_firstDaughtIdx_);
      genParticles_.push_back( obj );
      ++it_genParticles_px_;
      ++it_genParticles_py_;
      ++it_genParticles_pz_;
      ++it_genParticles_pt_;
      ++it_genParticles_eta_;
      ++it_genParticles_phi_;
      ++it_genParticles_charge_;
      ++it_genParticles_e_;
      ++it_genParticles_vx_;
      ++it_genParticles_vy_;
      ++it_genParticles_vz_;
      ++it_genParticles_pdgId_;
      ++it_genParticles_status_;
      ++it_genParticles_idx_;
      ++it_genParticles_momIdx_;
      ++it_genParticles_nDaught_;
      ++it_genParticles_firstDaughtIdx_;
    }
    return genParticles_;
  }
  
  const vector<Vertex>& vertexs(){
    if(vertexs_.size() > 0) return vertexs_;
    loadVertexs();
    auto it_vertexs_x_ = vertexs_x_->cbegin();
    auto it_vertexs_y_ = vertexs_y_->cbegin();
    auto it_vertexs_z_ = vertexs_z_->cbegin();
    auto it_vertexs_chi2_ = vertexs_chi2_->cbegin();
    auto it_vertexs_ndof_ = vertexs_ndof_->cbegin();
    auto it_vertexs_nTracks_ = vertexs_nTracks_->cbegin();
    for(; it_vertexs_x_ != vertexs_x_->cend(); ){
      Vertex obj;
      obj.setx(*it_vertexs_x_);
      obj.sety(*it_vertexs_y_);
      obj.setz(*it_vertexs_z_);
      obj.setchi2(*it_vertexs_chi2_);
      obj.setndof(*it_vertexs_ndof_);
      obj.setnTracks(*it_vertexs_nTracks_);
      vertexs_.push_back( obj );
      ++it_vertexs_x_;
      ++it_vertexs_y_;
      ++it_vertexs_z_;
      ++it_vertexs_chi2_;
      ++it_vertexs_ndof_;
      ++it_vertexs_nTracks_;
    }
    return vertexs_;
  }
  
  const vector<Puinfo>& PUInfos(){
    if(PUInfos_.size() > 0) return PUInfos_;
    loadPuinfos();
    auto it_PUInfos_bx_ = PUInfos_bx_->cbegin();
    auto it_PUInfos_nPU_ = PUInfos_nPU_->cbegin();
    auto it_PUInfos_nInteractions_ = PUInfos_nInteractions_->cbegin();
    for(; it_PUInfos_bx_ != PUInfos_bx_->cend(); ){
      Puinfo obj;
      obj.setbx(*it_PUInfos_bx_);
      obj.setnPU(*it_PUInfos_nPU_);
      obj.setnInteractions(*it_PUInfos_nInteractions_);
      PUInfos_.push_back( obj );
      ++it_PUInfos_bx_;
      ++it_PUInfos_nPU_;
      ++it_PUInfos_nInteractions_;
    }
    return PUInfos_;
  }
  

private:
  TTree *tree_;
  Long64_t entries_;
  Long64_t current_entry_;
  Bool_t trigger_IsoMu20_eta2p1_;
  Bool_t trigger_IsoMu24_;
  Bool_t trigger_IsoMu24_eta2p1_;
  Bool_t trigger_IsoMu30_;
  Bool_t trigger_IsoMu30_eta2p1_;
  Bool_t trigger_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_;
  Bool_t trigger_Ele27_WP80_;
  Bool_t trigger_Ele30_CaloIdVT_TrkIdT_;
  vector<float> *muons_px_;
  vector<float> *muons_py_;
  vector<float> *muons_pz_;
  vector<float> *muons_pt_;
  vector<float> *muons_eta_;
  vector<float> *muons_phi_;
  vector<int> *muons_charge_;
  vector<float> *muons_chargedIso_;
  vector<float> *muons_neutralIso_;
  vector<float> *muons_photonIso_;
  vector<float> *muons_puIso_;
  vector<float> *muons_ECalEnergy_;
  vector<float> *muons_HCalEnergy_;
  vector<int> *muons_numChambers_;
  vector<int> *muons_numMatchedStations_;
  vector<float> *muons_ptErr_;
  vector<float> *muons_chi2_;
  vector<int> *muons_ndof_;
  vector<float> *muons_validHits_;
  vector<bool> *muons_isGlobal_;
  vector<bool> *muons_isTracker_;
  vector<bool> *muons_isCalo_;
  vector<bool> *muons_isPF_;
  vector<bool> *muons_isStandAlone_;
  vector<bool> *muons_isLoose_;
  vector<bool> *muons_IsoMu20_eta2p1_;
  vector<bool> *muons_IsoMu24_;
  vector<bool> *muons_IsoMu24_eta2p1_;
  vector<bool> *muons_IsoMu30_;
  vector<bool> *muons_IsoMu30_eta2p1_;
  vector<float> *jets_px_;
  vector<float> *jets_py_;
  vector<float> *jets_pz_;
  vector<float> *jets_pt_;
  vector<float> *jets_eta_;
  vector<float> *jets_phi_;
  vector<int> *jets_charge_;
  vector<float> *jets_e_;
  vector<float> *jets_area_;
  vector<float> *jets_mass_;
  vector<float> *jets_chargedHadronEnergy_;
  vector<float> *jets_neutralHadronEnergy_;
  vector<float> *jets_photonEnergy_;
  vector<float> *jets_electronEnergy_;
  vector<float> *jets_muonEnergy_;
  vector<float> *jets_forwardEM_;
  vector<float> *jets_forwardHad_;
  vector<float> *jets_numChargedHadrons_;
  vector<float> *jets_numNeutralHadrons_;
  vector<float> *jets_numPhotons_;
  vector<float> *jets_numElectrons_;
  vector<float> *jets_numMuons_;
  vector<float> *jets_numForwardEMs_;
  vector<float> *jets_numForwardHads_;
  vector<float> *jets_puId_;
  vector<float> *jets_jetBProb_;
  vector<float> *jets_jetProb_;
  vector<float> *jets_trkHiPur_;
  vector<float> *jets_trkHiEff_;
  vector<float> *jets_ssvHiEff_;
  vector<float> *jets_ssvHiPur_;
  vector<float> *jets_csv_;
  vector<float> *jets_csvIncl_;
  vector<float> *jets_vtxMass_;
  vector<float> *jets_vtxNtracks_;
  vector<float> *jets_vtx3DVal_;
  vector<float> *jets_vtx3DSig_;
  vector<float> *electrons_px_;
  vector<float> *electrons_py_;
  vector<float> *electrons_pz_;
  vector<float> *electrons_pt_;
  vector<float> *electrons_eta_;
  vector<float> *electrons_phi_;
  vector<int> *electrons_charge_;
  vector<float> *electrons_chargedIso_;
  vector<float> *electrons_neutralIso_;
  vector<float> *electrons_photonIso_;
  vector<float> *electrons_puIso_;
  vector<float> *electrons_r9_;
  vector<float> *electrons_ESCOverETrack_;
  vector<float> *electrons_DEtaSCTrk_;
  vector<float> *electrons_DPhiSCTrk_;
  vector<float> *electrons_ecalEnergy_;
  vector<bool> *electrons_isEB_;
  vector<bool> *electrons_isEE_;
  vector<bool> *electrons_isEBGap_;
  vector<bool> *electrons_isEBEtaGap_;
  vector<bool> *electrons_isEBPhiGap_;
  vector<bool> *electrons_isEEGap_;
  vector<bool> *electrons_isEERingGap_;
  vector<bool> *electrons_isEEDeeGap_;
  vector<bool> *electrons_isEBEEGap_;
  vector<bool> *electrons_isElectron_;
  vector<bool> *electrons_ecalSeed_;
  vector<bool> *electrons_trackSeed_;
  vector<bool> *electrons_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_;
  vector<bool> *electrons_Ele27_WP80_;
  vector<bool> *electrons_Ele30_CaloIdVT_TrkIdT_;
  vector<float> *electrons_e1x5_;
  vector<float> *electrons_e5x5_;
  vector<float> *electrons_sigmaIEtaIEta_;
  vector<float> *electrons_sigmaIPhiIPhi_;
  vector<float> *electrons_hcalDepth1OverEcalBc_;
  vector<float> *electrons_hcalDepth2OverEcalBc_;
  vector<float> *electrons_x_;
  vector<float> *electrons_y_;
  vector<float> *electrons_z_;
  vector<float> *electrons_energy_;
  vector<float> *electrons_rawEnergy_;
  vector<float> *electrons_phiWidth_;
  vector<float> *electrons_etaWidth_;
  vector<float> *photons_px_;
  vector<float> *photons_py_;
  vector<float> *photons_pz_;
  vector<float> *photons_pt_;
  vector<float> *photons_eta_;
  vector<float> *photons_phi_;
  vector<int> *photons_charge_;
  vector<float> *photons_x_;
  vector<float> *photons_y_;
  vector<float> *photons_z_;
  vector<float> *photons_energy_;
  vector<float> *photons_rawEnergy_;
  vector<float> *photons_phiWidth_;
  vector<float> *photons_etaWidth_;
  vector<float> *photons_e3x3_;
  vector<float> *photons_maxCrystalEnergy_;
  vector<bool> *photons_isEB_;
  vector<bool> *photons_isEE_;
  vector<bool> *photons_isPFlowPhoton_;
  vector<bool> *photons_hasConversionTracks_;
  vector<bool> *photons_hasPixelSeed_;
  vector<float> *vertexs_x_;
  vector<float> *vertexs_y_;
  vector<float> *vertexs_z_;
  vector<float> *vertexs_chi2_;
  vector<float> *vertexs_ndof_;
  vector<float> *vertexs_nTracks_;
  vector<float> *METs_px_;
  vector<float> *METs_py_;
  vector<float> *METs_et_;
  vector<float> *METs_phi_;
  Float_t genInfo_weight_;
  Float_t genInfo_pdfid1_;
  Float_t genInfo_pdfid2_;
  Float_t genInfo_renScale_;
  vector<float> *PUInfos_bx_;
  vector<float> *PUInfos_nPU_;
  vector<float> *PUInfos_nInteractions_;
  vector<float> *genParticles_px_;
  vector<float> *genParticles_py_;
  vector<float> *genParticles_pz_;
  vector<float> *genParticles_pt_;
  vector<float> *genParticles_eta_;
  vector<float> *genParticles_phi_;
  vector<int> *genParticles_charge_;
  vector<float> *genParticles_e_;
  vector<float> *genParticles_vx_;
  vector<float> *genParticles_vy_;
  vector<float> *genParticles_vz_;
  vector<int> *genParticles_pdgId_;
  vector<int> *genParticles_status_;
  vector<int> *genParticles_idx_;
  vector<vector<int> > *genParticles_momIdx_;
  vector<int> *genParticles_nDaught_;
  vector<int> *genParticles_firstDaughtIdx_;
  bool are_METs_loaded_;
  vector<Met> METs_;
  bool are_genInfo_loaded_;
  Geninfo genInfo_;
  bool are_photons_loaded_;
  vector<Photon> photons_;
  bool are_trigger_loaded_;
  Trigger trigger_;
  bool are_electrons_loaded_;
  vector<Electron> electrons_;
  bool are_jets_loaded_;
  vector<Jet> jets_;
  bool are_muons_loaded_;
  vector<Muon> muons_;
  bool are_genParticles_loaded_;
  vector<Genparticle> genParticles_;
  bool are_vertexs_loaded_;
  vector<Vertex> vertexs_;
  bool are_PUInfos_loaded_;
  vector<Puinfo> PUInfos_;
};

/*#include <iostream>
int test()
{
  TFile *f = TFile::Open("test_ntuple.root");
  TTree *t = (TTree*) f->Get("ntuple/events");
  URStreamer s(t);
  for(int i =0; i < 30; i++){
    s.next();
    vector<Muon> muons = s.muons();
    std::cout<< muons.size() << std::endl;
    for(int j=0; j<muons.size(); ++j){
      std::cout<< *(muons[j].pt) << "  ";
    }
    std::cout<< std::endl;
  }
  return 0;
  }*/

/*#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class vector+;
#pragma link C++ class URStreamer+;
{ LINK_OBJECTS }

#endif*/
#endif

