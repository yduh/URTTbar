#include "../interface/BASEIO.h"

namespace BASEIO{
//Data_IOMuon Data_IOMuon Data_IOMuon Data_IOMuon Data_IOMuon Data_IOMuon Data_IOMuon Data_IOMuon Data_IOMuon Data_IOMuon 
BaseIO* Data_IOMuon::baseio = 0;
Data_IOMuon::Data_IOMuon(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
, PFR4_(new Data_PFIsolation(size_, prefix_ + "_PFR4"))
, InnerTrack_(new Data_IOTrack(size_*1, prefix_ + "_InnerTrack"))
{
	VertexNumber_ = new Int_t[size_];
	NumChambers_ = new Int_t[size_];
	NumChambersWithSegments_ = new Int_t[size_];
	NumValidMuonHits_ = new Int_t[size_];
	NumMatchedStations_ = new Int_t[size_];
	TriggerMatching_ = new UInt_t[size_];
	Info_ = new UInt_t[size_];
	px_ = new Float_t[size_];
	py_ = new Float_t[size_];
	pz_ = new Float_t[size_];
	PtUnc_ = new Float_t[size_];
	ChiQ_ = new Float_t[size_];
	NDOF_ = new Float_t[size_];
	ECalEnergy_ = new Float_t[size_];
	HCalEnergy_ = new Float_t[size_];
	InnerTrack_num_ = new UInt_t[size_];
}

Data_IOMuon::~Data_IOMuon()
{
	delete[] VertexNumber_;
	delete[] NumChambers_;
	delete[] NumChambersWithSegments_;
	delete[] NumValidMuonHits_;
	delete[] NumMatchedStations_;
	delete[] TriggerMatching_;
	delete[] Info_;
	delete PFR4_;
	delete[] px_;
	delete[] py_;
	delete[] pz_;
	delete[] PtUnc_;
	delete[] ChiQ_;
	delete[] NDOF_;
	delete[] ECalEnergy_;
	delete[] HCalEnergy_;
	delete InnerTrack_;
	delete[] InnerTrack_num_;
}

void Data_IOMuon::Fill()
{
	count_ = 0;
	PFR4_->Fill();
	InnerTrack_->Fill();
}

void Data_IOMuon::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_VertexNumber").c_str(), VertexNumber_, (prefix_ + "_VertexNumber[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_NumChambers").c_str(), NumChambers_, (prefix_ + "_NumChambers[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_NumChambersWithSegments").c_str(), NumChambersWithSegments_, (prefix_ + "_NumChambersWithSegments[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_NumValidMuonHits").c_str(), NumValidMuonHits_, (prefix_ + "_NumValidMuonHits[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_NumMatchedStations").c_str(), NumMatchedStations_, (prefix_ + "_NumMatchedStations[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_TriggerMatching").c_str(), TriggerMatching_, (prefix_ + "_TriggerMatching[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_Info").c_str(), Info_, (prefix_ + "_Info[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_px").c_str(), px_, (prefix_ + "_px[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_py").c_str(), py_, (prefix_ + "_py[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_pz").c_str(), pz_, (prefix_ + "_pz[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_PtUnc").c_str(), PtUnc_, (prefix_ + "_PtUnc[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ChiQ").c_str(), ChiQ_, (prefix_ + "_ChiQ[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_NDOF").c_str(), NDOF_, (prefix_ + "_NDOF[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ECalEnergy").c_str(), ECalEnergy_, (prefix_ + "_ECalEnergy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_HCalEnergy").c_str(), HCalEnergy_, (prefix_ + "_HCalEnergy[" + prefix_ + "_count]/F").c_str());
	PFR4_->SetUpWrite(tree);
	tree->Branch((prefix_ + "_InnerTrack_num").c_str(), InnerTrack_num_, (prefix_ + "_InnerTrack_num[" + prefix_ + "_count]/i").c_str());
	InnerTrack_->SetUpWrite(tree);
}

void Data_IOMuon::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_VertexNumber").c_str(), VertexNumber_);
	tree->SetBranchAddress((prefix_ + "_NumChambers").c_str(), NumChambers_);
	tree->SetBranchAddress((prefix_ + "_NumChambersWithSegments").c_str(), NumChambersWithSegments_);
	tree->SetBranchAddress((prefix_ + "_NumValidMuonHits").c_str(), NumValidMuonHits_);
	tree->SetBranchAddress((prefix_ + "_NumMatchedStations").c_str(), NumMatchedStations_);
	tree->SetBranchAddress((prefix_ + "_TriggerMatching").c_str(), TriggerMatching_);
	tree->SetBranchAddress((prefix_ + "_Info").c_str(), Info_);
	tree->SetBranchAddress((prefix_ + "_px").c_str(), px_);
	tree->SetBranchAddress((prefix_ + "_py").c_str(), py_);
	tree->SetBranchAddress((prefix_ + "_pz").c_str(), pz_);
	tree->SetBranchAddress((prefix_ + "_PtUnc").c_str(), PtUnc_);
	tree->SetBranchAddress((prefix_ + "_ChiQ").c_str(), ChiQ_);
	tree->SetBranchAddress((prefix_ + "_NDOF").c_str(), NDOF_);
	tree->SetBranchAddress((prefix_ + "_ECalEnergy").c_str(), ECalEnergy_);
	tree->SetBranchAddress((prefix_ + "_HCalEnergy").c_str(), HCalEnergy_);
	PFR4_->SetUpRead(tree);
	tree->SetBranchAddress((prefix_ + "_InnerTrack_num").c_str(), InnerTrack_num_);
	InnerTrack_->SetUpRead(tree);
}

void Data_IOMuon::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_VertexNumber").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumChambers").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumChambersWithSegments").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumValidMuonHits").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumMatchedStations").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_TriggerMatching").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Info").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_px").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_py").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_pz").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_PtUnc").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ChiQ").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NDOF").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ECalEnergy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_HCalEnergy").c_str(), load);
	PFR4_->Load(tree, load);
	tree->SetBranchStatus((prefix_ + "_InnerTrack_num").c_str(), load);
	InnerTrack_->Load(tree, load);
}



//IOMuon == IOMuon == IOMuon == IOMuon == IOMuon == IOMuon == IOMuon == IOMuon == IOMuon == IOMuon == IOMuon == IOMuon == IOMuon == IOMuon == IOMuon == 
BaseIO* IOMuon::baseio = 0;
IOMuon::IOMuon(Data_IOMuon* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

IOMuon::IOMuon(const IOMuon& _iomuon) : 
number_( _iomuon.number_),
data_( _iomuon.data_)
{
}

void IOMuon::Init()
{
	if(baseio->IsWritable())
	{
		if(number_ == 0) {data_->InnerTrack_num_[number_] = 0;}
		else {data_->InnerTrack_num_[number_] = data_->InnerTrack_num_[number_-1];}
		data_->count_ = number_+1;
	}
}

Int_t IOMuon::VertexNumber() const
{
	return data_->VertexNumber_[number_];
}

Int_t IOMuon::NumChambers() const
{
	return data_->NumChambers_[number_];
}

Int_t IOMuon::NumChambersWithSegments() const
{
	return data_->NumChambersWithSegments_[number_];
}

Int_t IOMuon::NumValidMuonHits() const
{
	return data_->NumValidMuonHits_[number_];
}

Int_t IOMuon::NumMatchedStations() const
{
	return data_->NumMatchedStations_[number_];
}

UInt_t IOMuon::TriggerMatching() const
{
	return data_->TriggerMatching_[number_];
}

UInt_t IOMuon::Info() const
{
	return data_->Info_[number_];
}

PFIsolation IOMuon::PFR4() const
{
	return PFIsolation(data_->PFR4_, number_);
}

Float_t IOMuon::px() const
{
	return data_->px_[number_];
}

Float_t IOMuon::py() const
{
	return data_->py_[number_];
}

Float_t IOMuon::pz() const
{
	return data_->pz_[number_];
}

Float_t IOMuon::PtUnc() const
{
	return data_->PtUnc_[number_];
}

Float_t IOMuon::ChiQ() const
{
	return data_->ChiQ_[number_];
}

Float_t IOMuon::NDOF() const
{
	return data_->NDOF_[number_];
}

Float_t IOMuon::ECalEnergy() const
{
	return data_->ECalEnergy_[number_];
}

Float_t IOMuon::HCalEnergy() const
{
	return data_->HCalEnergy_[number_];
}

UInt_t IOMuon::Num_InnerTrack() const
{
	return number_ == 0 ? data_->InnerTrack_num_[number_] : data_->InnerTrack_num_[number_] - data_->InnerTrack_num_[number_-1];
}

IOTrack IOMuon::InnerTrack(UInt_t n) const
{
	if(baseio->IsWritable())
	{
		data_->InnerTrack_num_[number_]++;
	}
	return number_ == 0 ? IOTrack(data_->InnerTrack_, n) : IOTrack(data_->InnerTrack_, data_->InnerTrack_num_[number_-1]  + n);
}

void IOMuon::VertexNumber(Int_t _VertexNumber)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(1);//ERROR1
		return;
	}
	data_->VertexNumber_[number_] = _VertexNumber;
}

void IOMuon::NumChambers(Int_t _NumChambers)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(2);//ERROR2
		return;
	}
	data_->NumChambers_[number_] = _NumChambers;
}

void IOMuon::NumChambersWithSegments(Int_t _NumChambersWithSegments)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(3);//ERROR3
		return;
	}
	data_->NumChambersWithSegments_[number_] = _NumChambersWithSegments;
}

void IOMuon::NumValidMuonHits(Int_t _NumValidMuonHits)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(4);//ERROR4
		return;
	}
	data_->NumValidMuonHits_[number_] = _NumValidMuonHits;
}

void IOMuon::NumMatchedStations(Int_t _NumMatchedStations)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(5);//ERROR5
		return;
	}
	data_->NumMatchedStations_[number_] = _NumMatchedStations;
}

void IOMuon::TriggerMatching(UInt_t _TriggerMatching)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(6);//ERROR6
		return;
	}
	data_->TriggerMatching_[number_] = _TriggerMatching;
}

void IOMuon::Info(UInt_t _Info)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(7);//ERROR7
		return;
	}
	data_->Info_[number_] = _Info;
}

void IOMuon::px(Float_t _px)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(8);//ERROR8
		return;
	}
	data_->px_[number_] = _px;
}

void IOMuon::py(Float_t _py)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(9);//ERROR9
		return;
	}
	data_->py_[number_] = _py;
}

void IOMuon::pz(Float_t _pz)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(10);//ERROR10
		return;
	}
	data_->pz_[number_] = _pz;
}

void IOMuon::PtUnc(Float_t _PtUnc)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(11);//ERROR11
		return;
	}
	data_->PtUnc_[number_] = _PtUnc;
}

void IOMuon::ChiQ(Float_t _ChiQ)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(12);//ERROR12
		return;
	}
	data_->ChiQ_[number_] = _ChiQ;
}

void IOMuon::NDOF(Float_t _NDOF)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(13);//ERROR13
		return;
	}
	data_->NDOF_[number_] = _NDOF;
}

void IOMuon::ECalEnergy(Float_t _ECalEnergy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(14);//ERROR14
		return;
	}
	data_->ECalEnergy_[number_] = _ECalEnergy;
}

void IOMuon::HCalEnergy(Float_t _HCalEnergy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(15);//ERROR15
		return;
	}
	data_->HCalEnergy_[number_] = _HCalEnergy;
}



//Data_PrimaryVertex Data_PrimaryVertex Data_PrimaryVertex Data_PrimaryVertex Data_PrimaryVertex Data_PrimaryVertex Data_PrimaryVertex Data_PrimaryVertex Data_PrimaryVertex Data_PrimaryVertex 
BaseIO* Data_PrimaryVertex::baseio = 0;
Data_PrimaryVertex::Data_PrimaryVertex(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
{
	NumTracks_ = new UChar_t[size_];
	Vx_ = new Float_t[size_];
	Vy_ = new Float_t[size_];
	Vz_ = new Float_t[size_];
	ChiQ_ = new Float_t[size_];
	NDOF_ = new Float_t[size_];
	SumPtQ_ = new Float_t[size_];
}

Data_PrimaryVertex::~Data_PrimaryVertex()
{
	delete[] NumTracks_;
	delete[] Vx_;
	delete[] Vy_;
	delete[] Vz_;
	delete[] ChiQ_;
	delete[] NDOF_;
	delete[] SumPtQ_;
}

void Data_PrimaryVertex::Fill()
{
	count_ = 0;
}

void Data_PrimaryVertex::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_NumTracks").c_str(), NumTracks_, (prefix_ + "_NumTracks[" + prefix_ + "_count]/b").c_str());
	tree->Branch((prefix_ + "_Vx").c_str(), Vx_, (prefix_ + "_Vx[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_Vy").c_str(), Vy_, (prefix_ + "_Vy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_Vz").c_str(), Vz_, (prefix_ + "_Vz[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ChiQ").c_str(), ChiQ_, (prefix_ + "_ChiQ[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_NDOF").c_str(), NDOF_, (prefix_ + "_NDOF[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_SumPtQ").c_str(), SumPtQ_, (prefix_ + "_SumPtQ[" + prefix_ + "_count]/F").c_str());
}

void Data_PrimaryVertex::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_NumTracks").c_str(), NumTracks_);
	tree->SetBranchAddress((prefix_ + "_Vx").c_str(), Vx_);
	tree->SetBranchAddress((prefix_ + "_Vy").c_str(), Vy_);
	tree->SetBranchAddress((prefix_ + "_Vz").c_str(), Vz_);
	tree->SetBranchAddress((prefix_ + "_ChiQ").c_str(), ChiQ_);
	tree->SetBranchAddress((prefix_ + "_NDOF").c_str(), NDOF_);
	tree->SetBranchAddress((prefix_ + "_SumPtQ").c_str(), SumPtQ_);
}

void Data_PrimaryVertex::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumTracks").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Vx").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Vy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Vz").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ChiQ").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NDOF").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_SumPtQ").c_str(), load);
}



//PrimaryVertex == PrimaryVertex == PrimaryVertex == PrimaryVertex == PrimaryVertex == PrimaryVertex == PrimaryVertex == PrimaryVertex == PrimaryVertex == PrimaryVertex == PrimaryVertex == PrimaryVertex == PrimaryVertex == PrimaryVertex == PrimaryVertex == 
BaseIO* PrimaryVertex::baseio = 0;
PrimaryVertex::PrimaryVertex(Data_PrimaryVertex* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

PrimaryVertex::PrimaryVertex(const PrimaryVertex& _primaryvertex) : 
number_( _primaryvertex.number_),
data_( _primaryvertex.data_)
{
}

void PrimaryVertex::Init()
{
	if(baseio->IsWritable())
	{
		data_->count_ = number_+1;
	}
}

UChar_t PrimaryVertex::NumTracks() const
{
	return data_->NumTracks_[number_];
}

Float_t PrimaryVertex::Vx() const
{
	return data_->Vx_[number_];
}

Float_t PrimaryVertex::Vy() const
{
	return data_->Vy_[number_];
}

Float_t PrimaryVertex::Vz() const
{
	return data_->Vz_[number_];
}

Float_t PrimaryVertex::ChiQ() const
{
	return data_->ChiQ_[number_];
}

Float_t PrimaryVertex::NDOF() const
{
	return data_->NDOF_[number_];
}

Float_t PrimaryVertex::SumPtQ() const
{
	return data_->SumPtQ_[number_];
}

void PrimaryVertex::NumTracks(UChar_t _NumTracks)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(16);//ERROR16
		return;
	}
	data_->NumTracks_[number_] = _NumTracks;
}

void PrimaryVertex::Vx(Float_t _Vx)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(17);//ERROR17
		return;
	}
	data_->Vx_[number_] = _Vx;
}

void PrimaryVertex::Vy(Float_t _Vy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(18);//ERROR18
		return;
	}
	data_->Vy_[number_] = _Vy;
}

void PrimaryVertex::Vz(Float_t _Vz)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(19);//ERROR19
		return;
	}
	data_->Vz_[number_] = _Vz;
}

void PrimaryVertex::ChiQ(Float_t _ChiQ)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(20);//ERROR20
		return;
	}
	data_->ChiQ_[number_] = _ChiQ;
}

void PrimaryVertex::NDOF(Float_t _NDOF)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(21);//ERROR21
		return;
	}
	data_->NDOF_[number_] = _NDOF;
}

void PrimaryVertex::SumPtQ(Float_t _SumPtQ)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(22);//ERROR22
		return;
	}
	data_->SumPtQ_[number_] = _SumPtQ;
}



//Data_IOEventInfo Data_IOEventInfo Data_IOEventInfo Data_IOEventInfo Data_IOEventInfo Data_IOEventInfo Data_IOEventInfo Data_IOEventInfo Data_IOEventInfo Data_IOEventInfo 
BaseIO* Data_IOEventInfo::baseio = 0;
Data_IOEventInfo::Data_IOEventInfo(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
{
	EventNumber_ = new UInt_t[size_];
	LumiSectionNumber_ = new UInt_t[size_];
	RunNumber_ = new UInt_t[size_];
	TimeUnix_ = new UInt_t[size_];
	TimeMuSec_ = new UInt_t[size_];
	AK5PFRho_ = new Float_t[size_];
	AK5PFSigma_ = new Float_t[size_];
	TriggerHLT_num_ = new UInt_t[size_];
	TriggerHLT_ = new UChar_t[size_*150];
	TriggerHLT_countmax_ = size_*150;
}

Data_IOEventInfo::~Data_IOEventInfo()
{
	delete[] EventNumber_;
	delete[] LumiSectionNumber_;
	delete[] RunNumber_;
	delete[] TimeUnix_;
	delete[] TimeMuSec_;
	delete[] AK5PFRho_;
	delete[] AK5PFSigma_;
	delete[] TriggerHLT_;
	delete[] TriggerHLT_num_;
}

void Data_IOEventInfo::Fill()
{
	count_ = 0;
	TriggerHLT_count_ = 0;
}

void Data_IOEventInfo::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_EventNumber").c_str(), EventNumber_, (prefix_ + "_EventNumber[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_LumiSectionNumber").c_str(), LumiSectionNumber_, (prefix_ + "_LumiSectionNumber[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_RunNumber").c_str(), RunNumber_, (prefix_ + "_RunNumber[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_TimeUnix").c_str(), TimeUnix_, (prefix_ + "_TimeUnix[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_TimeMuSec").c_str(), TimeMuSec_, (prefix_ + "_TimeMuSec[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_AK5PFRho").c_str(), AK5PFRho_, (prefix_ + "_AK5PFRho[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_AK5PFSigma").c_str(), AK5PFSigma_, (prefix_ + "_AK5PFSigma[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_TriggerHLT_count").c_str(), &TriggerHLT_count_, (prefix_ + "_TriggerHLT_count/i").c_str());
	tree->Branch((prefix_ + "_TriggerHLT_num").c_str(), TriggerHLT_num_, (prefix_ + "_TriggerHLT_num[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_TriggerHLT").c_str(), TriggerHLT_, (prefix_ + "_TriggerHLT[" + prefix_ + "_TriggerHLT_count]/b").c_str());
}

void Data_IOEventInfo::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_EventNumber").c_str(), EventNumber_);
	tree->SetBranchAddress((prefix_ + "_LumiSectionNumber").c_str(), LumiSectionNumber_);
	tree->SetBranchAddress((prefix_ + "_RunNumber").c_str(), RunNumber_);
	tree->SetBranchAddress((prefix_ + "_TimeUnix").c_str(), TimeUnix_);
	tree->SetBranchAddress((prefix_ + "_TimeMuSec").c_str(), TimeMuSec_);
	tree->SetBranchAddress((prefix_ + "_AK5PFRho").c_str(), AK5PFRho_);
	tree->SetBranchAddress((prefix_ + "_AK5PFSigma").c_str(), AK5PFSigma_);
	tree->SetBranchAddress((prefix_ + "_TriggerHLT_count").c_str(), &TriggerHLT_count_);
	tree->SetBranchAddress((prefix_ + "_TriggerHLT_num").c_str(), TriggerHLT_num_);
	tree->SetBranchAddress((prefix_ + "_TriggerHLT").c_str(), TriggerHLT_);
}

void Data_IOEventInfo::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_EventNumber").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_LumiSectionNumber").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_RunNumber").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_TimeUnix").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_TimeMuSec").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_AK5PFRho").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_AK5PFSigma").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_TriggerHLT_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_TriggerHLT_num").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_TriggerHLT").c_str(), load);
}



//IOEventInfo == IOEventInfo == IOEventInfo == IOEventInfo == IOEventInfo == IOEventInfo == IOEventInfo == IOEventInfo == IOEventInfo == IOEventInfo == IOEventInfo == IOEventInfo == IOEventInfo == IOEventInfo == IOEventInfo == 
BaseIO* IOEventInfo::baseio = 0;
IOEventInfo::IOEventInfo(Data_IOEventInfo* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

IOEventInfo::IOEventInfo(const IOEventInfo& _ioeventinfo) : 
number_( _ioeventinfo.number_),
data_( _ioeventinfo.data_)
{
}

void IOEventInfo::Init()
{
	if(baseio->IsWritable())
	{
		if(number_ == 0) {data_->TriggerHLT_num_[number_] = 0;}
		else {data_->TriggerHLT_num_[number_] = data_->TriggerHLT_num_[number_-1];}
		data_->count_ = number_+1;
	}
}

UInt_t IOEventInfo::EventNumber() const
{
	return data_->EventNumber_[number_];
}

UInt_t IOEventInfo::LumiSectionNumber() const
{
	return data_->LumiSectionNumber_[number_];
}

UInt_t IOEventInfo::RunNumber() const
{
	return data_->RunNumber_[number_];
}

UInt_t IOEventInfo::TimeUnix() const
{
	return data_->TimeUnix_[number_];
}

UInt_t IOEventInfo::TimeMuSec() const
{
	return data_->TimeMuSec_[number_];
}

Float_t IOEventInfo::AK5PFRho() const
{
	return data_->AK5PFRho_[number_];
}

Float_t IOEventInfo::AK5PFSigma() const
{
	return data_->AK5PFSigma_[number_];
}

UInt_t IOEventInfo::Num_TriggerHLT() const
{
	return number_ == 0 ? data_->TriggerHLT_num_[number_] : data_->TriggerHLT_num_[number_] - data_->TriggerHLT_num_[number_-1];
}

UChar_t IOEventInfo::TriggerHLT(UInt_t n) const
{
	return number_ == 0 ? data_->TriggerHLT_[n] : data_->TriggerHLT_[data_->TriggerHLT_num_[number_-1]  + n];
}

void IOEventInfo::EventNumber(UInt_t _EventNumber)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(23);//ERROR23
		return;
	}
	data_->EventNumber_[number_] = _EventNumber;
}

void IOEventInfo::LumiSectionNumber(UInt_t _LumiSectionNumber)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(24);//ERROR24
		return;
	}
	data_->LumiSectionNumber_[number_] = _LumiSectionNumber;
}

void IOEventInfo::RunNumber(UInt_t _RunNumber)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(25);//ERROR25
		return;
	}
	data_->RunNumber_[number_] = _RunNumber;
}

void IOEventInfo::TimeUnix(UInt_t _TimeUnix)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(26);//ERROR26
		return;
	}
	data_->TimeUnix_[number_] = _TimeUnix;
}

void IOEventInfo::TimeMuSec(UInt_t _TimeMuSec)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(27);//ERROR27
		return;
	}
	data_->TimeMuSec_[number_] = _TimeMuSec;
}

void IOEventInfo::AK5PFRho(Float_t _AK5PFRho)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(28);//ERROR28
		return;
	}
	data_->AK5PFRho_[number_] = _AK5PFRho;
}

void IOEventInfo::AK5PFSigma(Float_t _AK5PFSigma)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(29);//ERROR29
		return;
	}
	data_->AK5PFSigma_[number_] = _AK5PFSigma;
}

void IOEventInfo::TriggerHLT(UChar_t _TriggerHLT, UInt_t n)
{
	if(number_ != 0){n = data_->TriggerHLT_num_[number_-1] +n;}
	if(n >= data_->TriggerHLT_countmax_)
	{
		baseio->SetError(30);//ERROR30
		return;
	}
	if(n != data_->TriggerHLT_count_)
	{
		cerr << "Index already filled" << endl;
		return;
	}
	data_->TriggerHLT_[n] = _TriggerHLT;
	data_->TriggerHLT_num_[number_]++;
	data_->TriggerHLT_count_++;
}



//Data_IOPFJet Data_IOPFJet Data_IOPFJet Data_IOPFJet Data_IOPFJet Data_IOPFJet Data_IOPFJet Data_IOPFJet Data_IOPFJet Data_IOPFJet 
BaseIO* Data_IOPFJet::baseio = 0;
Data_IOPFJet::Data_IOPFJet(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
{
	TriggerMatching_ = new UInt_t[size_];
	NumChargedHadrons_ = new Int_t[size_];
	NumNeutralHadrons_ = new Int_t[size_];
	NumPhotons_ = new Int_t[size_];
	NumElectrons_ = new Int_t[size_];
	NumMuons_ = new Int_t[size_];
	NumForwardEMs_ = new Int_t[size_];
	NumForwardHads_ = new Int_t[size_];
	px_ = new Float_t[size_];
	py_ = new Float_t[size_];
	pz_ = new Float_t[size_];
	e_ = new Float_t[size_];
	Area_ = new Float_t[size_];
	Mass_ = new Float_t[size_];
	ChargedHadronEnergy_ = new Float_t[size_];
	NeutralHadronEnergy_ = new Float_t[size_];
	PhotonEnergy_ = new Float_t[size_];
	ElectronEnergy_ = new Float_t[size_];
	MuonEnergy_ = new Float_t[size_];
	ForwardEM_ = new Float_t[size_];
	ForwardHad_ = new Float_t[size_];
	ChargedPtMomPA_ = new Float_t[size_];
	ChargedPtMomPB_ = new Float_t[size_];
	ConstituentPtMomPA_ = new Float_t[size_];
	ConstituentPtMomPB_ = new Float_t[size_];
	PtFractionWrongPrimaryVertex_ = new Float_t[size_];
	MaxChargedPtFraction_ = new Float_t[size_];
	MaxPtFraction_ = new Float_t[size_];
	EnergyCorrection_ = new Float_t[size_];
	EnergyCorrectionUnc_ = new Float_t[size_];
	BTagCSV_ = new Float_t[size_];
	BTagCSVv2_ = new Float_t[size_];
}

Data_IOPFJet::~Data_IOPFJet()
{
	delete[] TriggerMatching_;
	delete[] NumChargedHadrons_;
	delete[] NumNeutralHadrons_;
	delete[] NumPhotons_;
	delete[] NumElectrons_;
	delete[] NumMuons_;
	delete[] NumForwardEMs_;
	delete[] NumForwardHads_;
	delete[] px_;
	delete[] py_;
	delete[] pz_;
	delete[] e_;
	delete[] Area_;
	delete[] Mass_;
	delete[] ChargedHadronEnergy_;
	delete[] NeutralHadronEnergy_;
	delete[] PhotonEnergy_;
	delete[] ElectronEnergy_;
	delete[] MuonEnergy_;
	delete[] ForwardEM_;
	delete[] ForwardHad_;
	delete[] ChargedPtMomPA_;
	delete[] ChargedPtMomPB_;
	delete[] ConstituentPtMomPA_;
	delete[] ConstituentPtMomPB_;
	delete[] PtFractionWrongPrimaryVertex_;
	delete[] MaxChargedPtFraction_;
	delete[] MaxPtFraction_;
	delete[] EnergyCorrection_;
	delete[] EnergyCorrectionUnc_;
	delete[] BTagCSV_;
	delete[] BTagCSVv2_;
}

void Data_IOPFJet::Fill()
{
	count_ = 0;
}

void Data_IOPFJet::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_TriggerMatching").c_str(), TriggerMatching_, (prefix_ + "_TriggerMatching[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_NumChargedHadrons").c_str(), NumChargedHadrons_, (prefix_ + "_NumChargedHadrons[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_NumNeutralHadrons").c_str(), NumNeutralHadrons_, (prefix_ + "_NumNeutralHadrons[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_NumPhotons").c_str(), NumPhotons_, (prefix_ + "_NumPhotons[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_NumElectrons").c_str(), NumElectrons_, (prefix_ + "_NumElectrons[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_NumMuons").c_str(), NumMuons_, (prefix_ + "_NumMuons[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_NumForwardEMs").c_str(), NumForwardEMs_, (prefix_ + "_NumForwardEMs[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_NumForwardHads").c_str(), NumForwardHads_, (prefix_ + "_NumForwardHads[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_px").c_str(), px_, (prefix_ + "_px[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_py").c_str(), py_, (prefix_ + "_py[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_pz").c_str(), pz_, (prefix_ + "_pz[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_e").c_str(), e_, (prefix_ + "_e[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_Area").c_str(), Area_, (prefix_ + "_Area[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_Mass").c_str(), Mass_, (prefix_ + "_Mass[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ChargedHadronEnergy").c_str(), ChargedHadronEnergy_, (prefix_ + "_ChargedHadronEnergy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_NeutralHadronEnergy").c_str(), NeutralHadronEnergy_, (prefix_ + "_NeutralHadronEnergy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_PhotonEnergy").c_str(), PhotonEnergy_, (prefix_ + "_PhotonEnergy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ElectronEnergy").c_str(), ElectronEnergy_, (prefix_ + "_ElectronEnergy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_MuonEnergy").c_str(), MuonEnergy_, (prefix_ + "_MuonEnergy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ForwardEM").c_str(), ForwardEM_, (prefix_ + "_ForwardEM[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ForwardHad").c_str(), ForwardHad_, (prefix_ + "_ForwardHad[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ChargedPtMomPA").c_str(), ChargedPtMomPA_, (prefix_ + "_ChargedPtMomPA[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ChargedPtMomPB").c_str(), ChargedPtMomPB_, (prefix_ + "_ChargedPtMomPB[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ConstituentPtMomPA").c_str(), ConstituentPtMomPA_, (prefix_ + "_ConstituentPtMomPA[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ConstituentPtMomPB").c_str(), ConstituentPtMomPB_, (prefix_ + "_ConstituentPtMomPB[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_PtFractionWrongPrimaryVertex").c_str(), PtFractionWrongPrimaryVertex_, (prefix_ + "_PtFractionWrongPrimaryVertex[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_MaxChargedPtFraction").c_str(), MaxChargedPtFraction_, (prefix_ + "_MaxChargedPtFraction[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_MaxPtFraction").c_str(), MaxPtFraction_, (prefix_ + "_MaxPtFraction[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_EnergyCorrection").c_str(), EnergyCorrection_, (prefix_ + "_EnergyCorrection[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_EnergyCorrectionUnc").c_str(), EnergyCorrectionUnc_, (prefix_ + "_EnergyCorrectionUnc[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_BTagCSV").c_str(), BTagCSV_, (prefix_ + "_BTagCSV[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_BTagCSVv2").c_str(), BTagCSVv2_, (prefix_ + "_BTagCSVv2[" + prefix_ + "_count]/F").c_str());
}

void Data_IOPFJet::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_TriggerMatching").c_str(), TriggerMatching_);
	tree->SetBranchAddress((prefix_ + "_NumChargedHadrons").c_str(), NumChargedHadrons_);
	tree->SetBranchAddress((prefix_ + "_NumNeutralHadrons").c_str(), NumNeutralHadrons_);
	tree->SetBranchAddress((prefix_ + "_NumPhotons").c_str(), NumPhotons_);
	tree->SetBranchAddress((prefix_ + "_NumElectrons").c_str(), NumElectrons_);
	tree->SetBranchAddress((prefix_ + "_NumMuons").c_str(), NumMuons_);
	tree->SetBranchAddress((prefix_ + "_NumForwardEMs").c_str(), NumForwardEMs_);
	tree->SetBranchAddress((prefix_ + "_NumForwardHads").c_str(), NumForwardHads_);
	tree->SetBranchAddress((prefix_ + "_px").c_str(), px_);
	tree->SetBranchAddress((prefix_ + "_py").c_str(), py_);
	tree->SetBranchAddress((prefix_ + "_pz").c_str(), pz_);
	tree->SetBranchAddress((prefix_ + "_e").c_str(), e_);
	tree->SetBranchAddress((prefix_ + "_Area").c_str(), Area_);
	tree->SetBranchAddress((prefix_ + "_Mass").c_str(), Mass_);
	tree->SetBranchAddress((prefix_ + "_ChargedHadronEnergy").c_str(), ChargedHadronEnergy_);
	tree->SetBranchAddress((prefix_ + "_NeutralHadronEnergy").c_str(), NeutralHadronEnergy_);
	tree->SetBranchAddress((prefix_ + "_PhotonEnergy").c_str(), PhotonEnergy_);
	tree->SetBranchAddress((prefix_ + "_ElectronEnergy").c_str(), ElectronEnergy_);
	tree->SetBranchAddress((prefix_ + "_MuonEnergy").c_str(), MuonEnergy_);
	tree->SetBranchAddress((prefix_ + "_ForwardEM").c_str(), ForwardEM_);
	tree->SetBranchAddress((prefix_ + "_ForwardHad").c_str(), ForwardHad_);
	tree->SetBranchAddress((prefix_ + "_ChargedPtMomPA").c_str(), ChargedPtMomPA_);
	tree->SetBranchAddress((prefix_ + "_ChargedPtMomPB").c_str(), ChargedPtMomPB_);
	tree->SetBranchAddress((prefix_ + "_ConstituentPtMomPA").c_str(), ConstituentPtMomPA_);
	tree->SetBranchAddress((prefix_ + "_ConstituentPtMomPB").c_str(), ConstituentPtMomPB_);
	tree->SetBranchAddress((prefix_ + "_PtFractionWrongPrimaryVertex").c_str(), PtFractionWrongPrimaryVertex_);
	tree->SetBranchAddress((prefix_ + "_MaxChargedPtFraction").c_str(), MaxChargedPtFraction_);
	tree->SetBranchAddress((prefix_ + "_MaxPtFraction").c_str(), MaxPtFraction_);
	tree->SetBranchAddress((prefix_ + "_EnergyCorrection").c_str(), EnergyCorrection_);
	tree->SetBranchAddress((prefix_ + "_EnergyCorrectionUnc").c_str(), EnergyCorrectionUnc_);
	tree->SetBranchAddress((prefix_ + "_BTagCSV").c_str(), BTagCSV_);
	tree->SetBranchAddress((prefix_ + "_BTagCSVv2").c_str(), BTagCSVv2_);
}

void Data_IOPFJet::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_TriggerMatching").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumChargedHadrons").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumNeutralHadrons").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumPhotons").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumElectrons").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumMuons").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumForwardEMs").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumForwardHads").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_px").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_py").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_pz").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_e").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Area").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Mass").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ChargedHadronEnergy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NeutralHadronEnergy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_PhotonEnergy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ElectronEnergy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_MuonEnergy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ForwardEM").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ForwardHad").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ChargedPtMomPA").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ChargedPtMomPB").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ConstituentPtMomPA").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ConstituentPtMomPB").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_PtFractionWrongPrimaryVertex").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_MaxChargedPtFraction").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_MaxPtFraction").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_EnergyCorrection").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_EnergyCorrectionUnc").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_BTagCSV").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_BTagCSVv2").c_str(), load);
}



//IOPFJet == IOPFJet == IOPFJet == IOPFJet == IOPFJet == IOPFJet == IOPFJet == IOPFJet == IOPFJet == IOPFJet == IOPFJet == IOPFJet == IOPFJet == IOPFJet == IOPFJet == 
BaseIO* IOPFJet::baseio = 0;
IOPFJet::IOPFJet(Data_IOPFJet* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

IOPFJet::IOPFJet(const IOPFJet& _iopfjet) : 
number_( _iopfjet.number_),
data_( _iopfjet.data_)
{
}

void IOPFJet::Init()
{
	if(baseio->IsWritable())
	{
		data_->count_ = number_+1;
	}
}

UInt_t IOPFJet::TriggerMatching() const
{
	return data_->TriggerMatching_[number_];
}

Int_t IOPFJet::NumChargedHadrons() const
{
	return data_->NumChargedHadrons_[number_];
}

Int_t IOPFJet::NumNeutralHadrons() const
{
	return data_->NumNeutralHadrons_[number_];
}

Int_t IOPFJet::NumPhotons() const
{
	return data_->NumPhotons_[number_];
}

Int_t IOPFJet::NumElectrons() const
{
	return data_->NumElectrons_[number_];
}

Int_t IOPFJet::NumMuons() const
{
	return data_->NumMuons_[number_];
}

Int_t IOPFJet::NumForwardEMs() const
{
	return data_->NumForwardEMs_[number_];
}

Int_t IOPFJet::NumForwardHads() const
{
	return data_->NumForwardHads_[number_];
}

Float_t IOPFJet::px() const
{
	return data_->px_[number_];
}

Float_t IOPFJet::py() const
{
	return data_->py_[number_];
}

Float_t IOPFJet::pz() const
{
	return data_->pz_[number_];
}

Float_t IOPFJet::e() const
{
	return data_->e_[number_];
}

Float_t IOPFJet::Area() const
{
	return data_->Area_[number_];
}

Float_t IOPFJet::Mass() const
{
	return data_->Mass_[number_];
}

Float_t IOPFJet::ChargedHadronEnergy() const
{
	return data_->ChargedHadronEnergy_[number_];
}

Float_t IOPFJet::NeutralHadronEnergy() const
{
	return data_->NeutralHadronEnergy_[number_];
}

Float_t IOPFJet::PhotonEnergy() const
{
	return data_->PhotonEnergy_[number_];
}

Float_t IOPFJet::ElectronEnergy() const
{
	return data_->ElectronEnergy_[number_];
}

Float_t IOPFJet::MuonEnergy() const
{
	return data_->MuonEnergy_[number_];
}

Float_t IOPFJet::ForwardEM() const
{
	return data_->ForwardEM_[number_];
}

Float_t IOPFJet::ForwardHad() const
{
	return data_->ForwardHad_[number_];
}

Float_t IOPFJet::ChargedPtMomPA() const
{
	return data_->ChargedPtMomPA_[number_];
}

Float_t IOPFJet::ChargedPtMomPB() const
{
	return data_->ChargedPtMomPB_[number_];
}

Float_t IOPFJet::ConstituentPtMomPA() const
{
	return data_->ConstituentPtMomPA_[number_];
}

Float_t IOPFJet::ConstituentPtMomPB() const
{
	return data_->ConstituentPtMomPB_[number_];
}

Float_t IOPFJet::PtFractionWrongPrimaryVertex() const
{
	return data_->PtFractionWrongPrimaryVertex_[number_];
}

Float_t IOPFJet::MaxChargedPtFraction() const
{
	return data_->MaxChargedPtFraction_[number_];
}

Float_t IOPFJet::MaxPtFraction() const
{
	return data_->MaxPtFraction_[number_];
}

Float_t IOPFJet::EnergyCorrection() const
{
	return data_->EnergyCorrection_[number_];
}

Float_t IOPFJet::EnergyCorrectionUnc() const
{
	return data_->EnergyCorrectionUnc_[number_];
}

Float_t IOPFJet::BTagCSV() const
{
	return data_->BTagCSV_[number_];
}

Float_t IOPFJet::BTagCSVv2() const
{
	return data_->BTagCSVv2_[number_];
}

void IOPFJet::TriggerMatching(UInt_t _TriggerMatching)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(31);//ERROR31
		return;
	}
	data_->TriggerMatching_[number_] = _TriggerMatching;
}

void IOPFJet::NumChargedHadrons(Int_t _NumChargedHadrons)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(32);//ERROR32
		return;
	}
	data_->NumChargedHadrons_[number_] = _NumChargedHadrons;
}

void IOPFJet::NumNeutralHadrons(Int_t _NumNeutralHadrons)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(33);//ERROR33
		return;
	}
	data_->NumNeutralHadrons_[number_] = _NumNeutralHadrons;
}

void IOPFJet::NumPhotons(Int_t _NumPhotons)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(34);//ERROR34
		return;
	}
	data_->NumPhotons_[number_] = _NumPhotons;
}

void IOPFJet::NumElectrons(Int_t _NumElectrons)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(35);//ERROR35
		return;
	}
	data_->NumElectrons_[number_] = _NumElectrons;
}

void IOPFJet::NumMuons(Int_t _NumMuons)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(36);//ERROR36
		return;
	}
	data_->NumMuons_[number_] = _NumMuons;
}

void IOPFJet::NumForwardEMs(Int_t _NumForwardEMs)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(37);//ERROR37
		return;
	}
	data_->NumForwardEMs_[number_] = _NumForwardEMs;
}

void IOPFJet::NumForwardHads(Int_t _NumForwardHads)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(38);//ERROR38
		return;
	}
	data_->NumForwardHads_[number_] = _NumForwardHads;
}

void IOPFJet::px(Float_t _px)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(39);//ERROR39
		return;
	}
	data_->px_[number_] = _px;
}

void IOPFJet::py(Float_t _py)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(40);//ERROR40
		return;
	}
	data_->py_[number_] = _py;
}

void IOPFJet::pz(Float_t _pz)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(41);//ERROR41
		return;
	}
	data_->pz_[number_] = _pz;
}

void IOPFJet::e(Float_t _e)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(42);//ERROR42
		return;
	}
	data_->e_[number_] = _e;
}

void IOPFJet::Area(Float_t _Area)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(43);//ERROR43
		return;
	}
	data_->Area_[number_] = _Area;
}

void IOPFJet::Mass(Float_t _Mass)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(44);//ERROR44
		return;
	}
	data_->Mass_[number_] = _Mass;
}

void IOPFJet::ChargedHadronEnergy(Float_t _ChargedHadronEnergy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(45);//ERROR45
		return;
	}
	data_->ChargedHadronEnergy_[number_] = _ChargedHadronEnergy;
}

void IOPFJet::NeutralHadronEnergy(Float_t _NeutralHadronEnergy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(46);//ERROR46
		return;
	}
	data_->NeutralHadronEnergy_[number_] = _NeutralHadronEnergy;
}

void IOPFJet::PhotonEnergy(Float_t _PhotonEnergy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(47);//ERROR47
		return;
	}
	data_->PhotonEnergy_[number_] = _PhotonEnergy;
}

void IOPFJet::ElectronEnergy(Float_t _ElectronEnergy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(48);//ERROR48
		return;
	}
	data_->ElectronEnergy_[number_] = _ElectronEnergy;
}

void IOPFJet::MuonEnergy(Float_t _MuonEnergy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(49);//ERROR49
		return;
	}
	data_->MuonEnergy_[number_] = _MuonEnergy;
}

void IOPFJet::ForwardEM(Float_t _ForwardEM)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(50);//ERROR50
		return;
	}
	data_->ForwardEM_[number_] = _ForwardEM;
}

void IOPFJet::ForwardHad(Float_t _ForwardHad)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(51);//ERROR51
		return;
	}
	data_->ForwardHad_[number_] = _ForwardHad;
}

void IOPFJet::ChargedPtMomPA(Float_t _ChargedPtMomPA)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(52);//ERROR52
		return;
	}
	data_->ChargedPtMomPA_[number_] = _ChargedPtMomPA;
}

void IOPFJet::ChargedPtMomPB(Float_t _ChargedPtMomPB)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(53);//ERROR53
		return;
	}
	data_->ChargedPtMomPB_[number_] = _ChargedPtMomPB;
}

void IOPFJet::ConstituentPtMomPA(Float_t _ConstituentPtMomPA)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(54);//ERROR54
		return;
	}
	data_->ConstituentPtMomPA_[number_] = _ConstituentPtMomPA;
}

void IOPFJet::ConstituentPtMomPB(Float_t _ConstituentPtMomPB)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(55);//ERROR55
		return;
	}
	data_->ConstituentPtMomPB_[number_] = _ConstituentPtMomPB;
}

void IOPFJet::PtFractionWrongPrimaryVertex(Float_t _PtFractionWrongPrimaryVertex)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(56);//ERROR56
		return;
	}
	data_->PtFractionWrongPrimaryVertex_[number_] = _PtFractionWrongPrimaryVertex;
}

void IOPFJet::MaxChargedPtFraction(Float_t _MaxChargedPtFraction)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(57);//ERROR57
		return;
	}
	data_->MaxChargedPtFraction_[number_] = _MaxChargedPtFraction;
}

void IOPFJet::MaxPtFraction(Float_t _MaxPtFraction)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(58);//ERROR58
		return;
	}
	data_->MaxPtFraction_[number_] = _MaxPtFraction;
}

void IOPFJet::EnergyCorrection(Float_t _EnergyCorrection)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(59);//ERROR59
		return;
	}
	data_->EnergyCorrection_[number_] = _EnergyCorrection;
}

void IOPFJet::EnergyCorrectionUnc(Float_t _EnergyCorrectionUnc)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(60);//ERROR60
		return;
	}
	data_->EnergyCorrectionUnc_[number_] = _EnergyCorrectionUnc;
}

void IOPFJet::BTagCSV(Float_t _BTagCSV)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(61);//ERROR61
		return;
	}
	data_->BTagCSV_[number_] = _BTagCSV;
}

void IOPFJet::BTagCSVv2(Float_t _BTagCSVv2)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(62);//ERROR62
		return;
	}
	data_->BTagCSVv2_[number_] = _BTagCSVv2;
}



//Data_IOString Data_IOString Data_IOString Data_IOString Data_IOString Data_IOString Data_IOString Data_IOString Data_IOString Data_IOString 
BaseIO* Data_IOString::baseio = 0;
Data_IOString::Data_IOString(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
{
	str_num_ = new UInt_t[size_];
	str_ = new Char_t[size_*100];
	str_countmax_ = size_*100;
}

Data_IOString::~Data_IOString()
{
	delete[] str_;
	delete[] str_num_;
}

void Data_IOString::Fill()
{
	count_ = 0;
	str_count_ = 0;
}

void Data_IOString::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_str_count").c_str(), &str_count_, (prefix_ + "_str_count/i").c_str());
	tree->Branch((prefix_ + "_str_num").c_str(), str_num_, (prefix_ + "_str_num[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_str").c_str(), str_, (prefix_ + "_str[" + prefix_ + "_str_count]/B").c_str());
}

void Data_IOString::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_str_count").c_str(), &str_count_);
	tree->SetBranchAddress((prefix_ + "_str_num").c_str(), str_num_);
	tree->SetBranchAddress((prefix_ + "_str").c_str(), str_);
}

void Data_IOString::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_str_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_str_num").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_str").c_str(), load);
}



//IOString == IOString == IOString == IOString == IOString == IOString == IOString == IOString == IOString == IOString == IOString == IOString == IOString == IOString == IOString == 
BaseIO* IOString::baseio = 0;
IOString::IOString(Data_IOString* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

IOString::IOString(const IOString& _iostring) : 
number_( _iostring.number_),
data_( _iostring.data_)
{
}

void IOString::Init()
{
	if(baseio->IsWritable())
	{
		if(number_ == 0) {data_->str_num_[number_] = 0;}
		else {data_->str_num_[number_] = data_->str_num_[number_-1];}
		data_->count_ = number_+1;
	}
}

UInt_t IOString::Num_str() const
{
	return number_ == 0 ? data_->str_num_[number_] : data_->str_num_[number_] - data_->str_num_[number_-1];
}

Char_t IOString::str(UInt_t n) const
{
	return number_ == 0 ? data_->str_[n] : data_->str_[data_->str_num_[number_-1]  + n];
}

void IOString::str(Char_t _str, UInt_t n)
{
	if(number_ != 0){n = data_->str_num_[number_-1] +n;}
	if(n >= data_->str_countmax_)
	{
		baseio->SetError(63);//ERROR63
		return;
	}
	if(n != data_->str_count_)
	{
		cerr << "Index already filled" << endl;
		return;
	}
	data_->str_[n] = _str;
	data_->str_num_[number_]++;
	data_->str_count_++;
}



//Data_GenInfo Data_GenInfo Data_GenInfo Data_GenInfo Data_GenInfo Data_GenInfo Data_GenInfo Data_GenInfo Data_GenInfo Data_GenInfo 
BaseIO* Data_GenInfo::baseio = 0;
Data_GenInfo::Data_GenInfo(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
{
	PDGID1_ = new Int_t[size_];
	PDGID2_ = new Int_t[size_];
	NumPUInteractions_ = new Int_t[size_];
	NumPUInteractionsBefore_ = new Int_t[size_];
	NumPUInteractionsAfter_ = new Int_t[size_];
	Weight_ = new Float_t[size_];
	x1_ = new Float_t[size_];
	x2_ = new Float_t[size_];
	RenScale_ = new Float_t[size_];
	FacScale_ = new Float_t[size_];
	METx_ = new Float_t[size_];
	METy_ = new Float_t[size_];
	NumTrueInteractions_ = new Float_t[size_];
}

Data_GenInfo::~Data_GenInfo()
{
	delete[] PDGID1_;
	delete[] PDGID2_;
	delete[] NumPUInteractions_;
	delete[] NumPUInteractionsBefore_;
	delete[] NumPUInteractionsAfter_;
	delete[] Weight_;
	delete[] x1_;
	delete[] x2_;
	delete[] RenScale_;
	delete[] FacScale_;
	delete[] METx_;
	delete[] METy_;
	delete[] NumTrueInteractions_;
}

void Data_GenInfo::Fill()
{
	count_ = 0;
}

void Data_GenInfo::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_PDGID1").c_str(), PDGID1_, (prefix_ + "_PDGID1[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_PDGID2").c_str(), PDGID2_, (prefix_ + "_PDGID2[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_NumPUInteractions").c_str(), NumPUInteractions_, (prefix_ + "_NumPUInteractions[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_NumPUInteractionsBefore").c_str(), NumPUInteractionsBefore_, (prefix_ + "_NumPUInteractionsBefore[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_NumPUInteractionsAfter").c_str(), NumPUInteractionsAfter_, (prefix_ + "_NumPUInteractionsAfter[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_Weight").c_str(), Weight_, (prefix_ + "_Weight[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_x1").c_str(), x1_, (prefix_ + "_x1[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_x2").c_str(), x2_, (prefix_ + "_x2[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_RenScale").c_str(), RenScale_, (prefix_ + "_RenScale[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_FacScale").c_str(), FacScale_, (prefix_ + "_FacScale[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_METx").c_str(), METx_, (prefix_ + "_METx[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_METy").c_str(), METy_, (prefix_ + "_METy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_NumTrueInteractions").c_str(), NumTrueInteractions_, (prefix_ + "_NumTrueInteractions[" + prefix_ + "_count]/F").c_str());
}

void Data_GenInfo::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_PDGID1").c_str(), PDGID1_);
	tree->SetBranchAddress((prefix_ + "_PDGID2").c_str(), PDGID2_);
	tree->SetBranchAddress((prefix_ + "_NumPUInteractions").c_str(), NumPUInteractions_);
	tree->SetBranchAddress((prefix_ + "_NumPUInteractionsBefore").c_str(), NumPUInteractionsBefore_);
	tree->SetBranchAddress((prefix_ + "_NumPUInteractionsAfter").c_str(), NumPUInteractionsAfter_);
	tree->SetBranchAddress((prefix_ + "_Weight").c_str(), Weight_);
	tree->SetBranchAddress((prefix_ + "_x1").c_str(), x1_);
	tree->SetBranchAddress((prefix_ + "_x2").c_str(), x2_);
	tree->SetBranchAddress((prefix_ + "_RenScale").c_str(), RenScale_);
	tree->SetBranchAddress((prefix_ + "_FacScale").c_str(), FacScale_);
	tree->SetBranchAddress((prefix_ + "_METx").c_str(), METx_);
	tree->SetBranchAddress((prefix_ + "_METy").c_str(), METy_);
	tree->SetBranchAddress((prefix_ + "_NumTrueInteractions").c_str(), NumTrueInteractions_);
}

void Data_GenInfo::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_PDGID1").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_PDGID2").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumPUInteractions").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumPUInteractionsBefore").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumPUInteractionsAfter").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Weight").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_x1").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_x2").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_RenScale").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_FacScale").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_METx").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_METy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumTrueInteractions").c_str(), load);
}



//GenInfo == GenInfo == GenInfo == GenInfo == GenInfo == GenInfo == GenInfo == GenInfo == GenInfo == GenInfo == GenInfo == GenInfo == GenInfo == GenInfo == GenInfo == 
BaseIO* GenInfo::baseio = 0;
GenInfo::GenInfo(Data_GenInfo* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

GenInfo::GenInfo(const GenInfo& _geninfo) : 
number_( _geninfo.number_),
data_( _geninfo.data_)
{
}

void GenInfo::Init()
{
	if(baseio->IsWritable())
	{
		data_->count_ = number_+1;
	}
}

Int_t GenInfo::PDGID1() const
{
	return data_->PDGID1_[number_];
}

Int_t GenInfo::PDGID2() const
{
	return data_->PDGID2_[number_];
}

Int_t GenInfo::NumPUInteractions() const
{
	return data_->NumPUInteractions_[number_];
}

Int_t GenInfo::NumPUInteractionsBefore() const
{
	return data_->NumPUInteractionsBefore_[number_];
}

Int_t GenInfo::NumPUInteractionsAfter() const
{
	return data_->NumPUInteractionsAfter_[number_];
}

Float_t GenInfo::Weight() const
{
	return data_->Weight_[number_];
}

Float_t GenInfo::x1() const
{
	return data_->x1_[number_];
}

Float_t GenInfo::x2() const
{
	return data_->x2_[number_];
}

Float_t GenInfo::RenScale() const
{
	return data_->RenScale_[number_];
}

Float_t GenInfo::FacScale() const
{
	return data_->FacScale_[number_];
}

Float_t GenInfo::METx() const
{
	return data_->METx_[number_];
}

Float_t GenInfo::METy() const
{
	return data_->METy_[number_];
}

Float_t GenInfo::NumTrueInteractions() const
{
	return data_->NumTrueInteractions_[number_];
}

void GenInfo::PDGID1(Int_t _PDGID1)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(64);//ERROR64
		return;
	}
	data_->PDGID1_[number_] = _PDGID1;
}

void GenInfo::PDGID2(Int_t _PDGID2)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(65);//ERROR65
		return;
	}
	data_->PDGID2_[number_] = _PDGID2;
}

void GenInfo::NumPUInteractions(Int_t _NumPUInteractions)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(66);//ERROR66
		return;
	}
	data_->NumPUInteractions_[number_] = _NumPUInteractions;
}

void GenInfo::NumPUInteractionsBefore(Int_t _NumPUInteractionsBefore)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(67);//ERROR67
		return;
	}
	data_->NumPUInteractionsBefore_[number_] = _NumPUInteractionsBefore;
}

void GenInfo::NumPUInteractionsAfter(Int_t _NumPUInteractionsAfter)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(68);//ERROR68
		return;
	}
	data_->NumPUInteractionsAfter_[number_] = _NumPUInteractionsAfter;
}

void GenInfo::Weight(Float_t _Weight)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(69);//ERROR69
		return;
	}
	data_->Weight_[number_] = _Weight;
}

void GenInfo::x1(Float_t _x1)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(70);//ERROR70
		return;
	}
	data_->x1_[number_] = _x1;
}

void GenInfo::x2(Float_t _x2)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(71);//ERROR71
		return;
	}
	data_->x2_[number_] = _x2;
}

void GenInfo::RenScale(Float_t _RenScale)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(72);//ERROR72
		return;
	}
	data_->RenScale_[number_] = _RenScale;
}

void GenInfo::FacScale(Float_t _FacScale)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(73);//ERROR73
		return;
	}
	data_->FacScale_[number_] = _FacScale;
}

void GenInfo::METx(Float_t _METx)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(74);//ERROR74
		return;
	}
	data_->METx_[number_] = _METx;
}

void GenInfo::METy(Float_t _METy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(75);//ERROR75
		return;
	}
	data_->METy_[number_] = _METy;
}

void GenInfo::NumTrueInteractions(Float_t _NumTrueInteractions)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(76);//ERROR76
		return;
	}
	data_->NumTrueInteractions_[number_] = _NumTrueInteractions;
}



//Data_IOMET Data_IOMET Data_IOMET Data_IOMET Data_IOMET Data_IOMET Data_IOMET Data_IOMET Data_IOMET Data_IOMET 
BaseIO* Data_IOMET::baseio = 0;
Data_IOMET::Data_IOMET(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
{
	px_ = new Float_t[size_];
	py_ = new Float_t[size_];
	pxUnc_ = new Float_t[size_];
	pyUnc_ = new Float_t[size_];
	pxpyUnc_ = new Float_t[size_];
}

Data_IOMET::~Data_IOMET()
{
	delete[] px_;
	delete[] py_;
	delete[] pxUnc_;
	delete[] pyUnc_;
	delete[] pxpyUnc_;
}

void Data_IOMET::Fill()
{
	count_ = 0;
}

void Data_IOMET::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_px").c_str(), px_, (prefix_ + "_px[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_py").c_str(), py_, (prefix_ + "_py[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_pxUnc").c_str(), pxUnc_, (prefix_ + "_pxUnc[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_pyUnc").c_str(), pyUnc_, (prefix_ + "_pyUnc[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_pxpyUnc").c_str(), pxpyUnc_, (prefix_ + "_pxpyUnc[" + prefix_ + "_count]/F").c_str());
}

void Data_IOMET::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_px").c_str(), px_);
	tree->SetBranchAddress((prefix_ + "_py").c_str(), py_);
	tree->SetBranchAddress((prefix_ + "_pxUnc").c_str(), pxUnc_);
	tree->SetBranchAddress((prefix_ + "_pyUnc").c_str(), pyUnc_);
	tree->SetBranchAddress((prefix_ + "_pxpyUnc").c_str(), pxpyUnc_);
}

void Data_IOMET::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_px").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_py").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_pxUnc").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_pyUnc").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_pxpyUnc").c_str(), load);
}



//IOMET == IOMET == IOMET == IOMET == IOMET == IOMET == IOMET == IOMET == IOMET == IOMET == IOMET == IOMET == IOMET == IOMET == IOMET == 
BaseIO* IOMET::baseio = 0;
IOMET::IOMET(Data_IOMET* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

IOMET::IOMET(const IOMET& _iomet) : 
number_( _iomet.number_),
data_( _iomet.data_)
{
}

void IOMET::Init()
{
	if(baseio->IsWritable())
	{
		data_->count_ = number_+1;
	}
}

Float_t IOMET::px() const
{
	return data_->px_[number_];
}

Float_t IOMET::py() const
{
	return data_->py_[number_];
}

Float_t IOMET::pxUnc() const
{
	return data_->pxUnc_[number_];
}

Float_t IOMET::pyUnc() const
{
	return data_->pyUnc_[number_];
}

Float_t IOMET::pxpyUnc() const
{
	return data_->pxpyUnc_[number_];
}

void IOMET::px(Float_t _px)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(77);//ERROR77
		return;
	}
	data_->px_[number_] = _px;
}

void IOMET::py(Float_t _py)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(78);//ERROR78
		return;
	}
	data_->py_[number_] = _py;
}

void IOMET::pxUnc(Float_t _pxUnc)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(79);//ERROR79
		return;
	}
	data_->pxUnc_[number_] = _pxUnc;
}

void IOMET::pyUnc(Float_t _pyUnc)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(80);//ERROR80
		return;
	}
	data_->pyUnc_[number_] = _pyUnc;
}

void IOMET::pxpyUnc(Float_t _pxpyUnc)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(81);//ERROR81
		return;
	}
	data_->pxpyUnc_[number_] = _pxpyUnc;
}



//Data_IOSuperCluster Data_IOSuperCluster Data_IOSuperCluster Data_IOSuperCluster Data_IOSuperCluster Data_IOSuperCluster Data_IOSuperCluster Data_IOSuperCluster Data_IOSuperCluster Data_IOSuperCluster 
BaseIO* Data_IOSuperCluster::baseio = 0;
Data_IOSuperCluster::Data_IOSuperCluster(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
{
	x_ = new Float_t[size_];
	y_ = new Float_t[size_];
	z_ = new Float_t[size_];
	Energy_ = new Float_t[size_];
	RawEnergy_ = new Float_t[size_];
	PhiWidth_ = new Float_t[size_];
	EtaWidth_ = new Float_t[size_];
}

Data_IOSuperCluster::~Data_IOSuperCluster()
{
	delete[] x_;
	delete[] y_;
	delete[] z_;
	delete[] Energy_;
	delete[] RawEnergy_;
	delete[] PhiWidth_;
	delete[] EtaWidth_;
}

void Data_IOSuperCluster::Fill()
{
	count_ = 0;
}

void Data_IOSuperCluster::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_x").c_str(), x_, (prefix_ + "_x[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_y").c_str(), y_, (prefix_ + "_y[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_z").c_str(), z_, (prefix_ + "_z[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_Energy").c_str(), Energy_, (prefix_ + "_Energy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_RawEnergy").c_str(), RawEnergy_, (prefix_ + "_RawEnergy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_PhiWidth").c_str(), PhiWidth_, (prefix_ + "_PhiWidth[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_EtaWidth").c_str(), EtaWidth_, (prefix_ + "_EtaWidth[" + prefix_ + "_count]/F").c_str());
}

void Data_IOSuperCluster::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_x").c_str(), x_);
	tree->SetBranchAddress((prefix_ + "_y").c_str(), y_);
	tree->SetBranchAddress((prefix_ + "_z").c_str(), z_);
	tree->SetBranchAddress((prefix_ + "_Energy").c_str(), Energy_);
	tree->SetBranchAddress((prefix_ + "_RawEnergy").c_str(), RawEnergy_);
	tree->SetBranchAddress((prefix_ + "_PhiWidth").c_str(), PhiWidth_);
	tree->SetBranchAddress((prefix_ + "_EtaWidth").c_str(), EtaWidth_);
}

void Data_IOSuperCluster::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_x").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_y").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_z").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Energy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_RawEnergy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_PhiWidth").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_EtaWidth").c_str(), load);
}



//IOSuperCluster == IOSuperCluster == IOSuperCluster == IOSuperCluster == IOSuperCluster == IOSuperCluster == IOSuperCluster == IOSuperCluster == IOSuperCluster == IOSuperCluster == IOSuperCluster == IOSuperCluster == IOSuperCluster == IOSuperCluster == IOSuperCluster == 
BaseIO* IOSuperCluster::baseio = 0;
IOSuperCluster::IOSuperCluster(Data_IOSuperCluster* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

IOSuperCluster::IOSuperCluster(const IOSuperCluster& _iosupercluster) : 
number_( _iosupercluster.number_),
data_( _iosupercluster.data_)
{
}

void IOSuperCluster::Init()
{
	if(baseio->IsWritable())
	{
		data_->count_ = number_+1;
	}
}

Float_t IOSuperCluster::x() const
{
	return data_->x_[number_];
}

Float_t IOSuperCluster::y() const
{
	return data_->y_[number_];
}

Float_t IOSuperCluster::z() const
{
	return data_->z_[number_];
}

Float_t IOSuperCluster::Energy() const
{
	return data_->Energy_[number_];
}

Float_t IOSuperCluster::RawEnergy() const
{
	return data_->RawEnergy_[number_];
}

Float_t IOSuperCluster::PhiWidth() const
{
	return data_->PhiWidth_[number_];
}

Float_t IOSuperCluster::EtaWidth() const
{
	return data_->EtaWidth_[number_];
}

void IOSuperCluster::x(Float_t _x)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(82);//ERROR82
		return;
	}
	data_->x_[number_] = _x;
}

void IOSuperCluster::y(Float_t _y)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(83);//ERROR83
		return;
	}
	data_->y_[number_] = _y;
}

void IOSuperCluster::z(Float_t _z)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(84);//ERROR84
		return;
	}
	data_->z_[number_] = _z;
}

void IOSuperCluster::Energy(Float_t _Energy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(85);//ERROR85
		return;
	}
	data_->Energy_[number_] = _Energy;
}

void IOSuperCluster::RawEnergy(Float_t _RawEnergy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(86);//ERROR86
		return;
	}
	data_->RawEnergy_[number_] = _RawEnergy;
}

void IOSuperCluster::PhiWidth(Float_t _PhiWidth)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(87);//ERROR87
		return;
	}
	data_->PhiWidth_[number_] = _PhiWidth;
}

void IOSuperCluster::EtaWidth(Float_t _EtaWidth)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(88);//ERROR88
		return;
	}
	data_->EtaWidth_[number_] = _EtaWidth;
}



//Data_PFIsolation Data_PFIsolation Data_PFIsolation Data_PFIsolation Data_PFIsolation Data_PFIsolation Data_PFIsolation Data_PFIsolation Data_PFIsolation Data_PFIsolation 
BaseIO* Data_PFIsolation::baseio = 0;
Data_PFIsolation::Data_PFIsolation(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
{
	Charged_ = new Float_t[size_];
	Neutral_ = new Float_t[size_];
	Hadron_ = new Float_t[size_];
	Photon_ = new Float_t[size_];
}

Data_PFIsolation::~Data_PFIsolation()
{
	delete[] Charged_;
	delete[] Neutral_;
	delete[] Hadron_;
	delete[] Photon_;
}

void Data_PFIsolation::Fill()
{
	count_ = 0;
}

void Data_PFIsolation::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_Charged").c_str(), Charged_, (prefix_ + "_Charged[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_Neutral").c_str(), Neutral_, (prefix_ + "_Neutral[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_Hadron").c_str(), Hadron_, (prefix_ + "_Hadron[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_Photon").c_str(), Photon_, (prefix_ + "_Photon[" + prefix_ + "_count]/F").c_str());
}

void Data_PFIsolation::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_Charged").c_str(), Charged_);
	tree->SetBranchAddress((prefix_ + "_Neutral").c_str(), Neutral_);
	tree->SetBranchAddress((prefix_ + "_Hadron").c_str(), Hadron_);
	tree->SetBranchAddress((prefix_ + "_Photon").c_str(), Photon_);
}

void Data_PFIsolation::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Charged").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Neutral").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Hadron").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Photon").c_str(), load);
}



//PFIsolation == PFIsolation == PFIsolation == PFIsolation == PFIsolation == PFIsolation == PFIsolation == PFIsolation == PFIsolation == PFIsolation == PFIsolation == PFIsolation == PFIsolation == PFIsolation == PFIsolation == 
BaseIO* PFIsolation::baseio = 0;
PFIsolation::PFIsolation(Data_PFIsolation* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

PFIsolation::PFIsolation(const PFIsolation& _pfisolation) : 
number_( _pfisolation.number_),
data_( _pfisolation.data_)
{
}

void PFIsolation::Init()
{
	if(baseio->IsWritable())
	{
		data_->count_ = number_+1;
	}
}

Float_t PFIsolation::Charged() const
{
	return data_->Charged_[number_];
}

Float_t PFIsolation::Neutral() const
{
	return data_->Neutral_[number_];
}

Float_t PFIsolation::Hadron() const
{
	return data_->Hadron_[number_];
}

Float_t PFIsolation::Photon() const
{
	return data_->Photon_[number_];
}

void PFIsolation::Charged(Float_t _Charged)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(89);//ERROR89
		return;
	}
	data_->Charged_[number_] = _Charged;
}

void PFIsolation::Neutral(Float_t _Neutral)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(90);//ERROR90
		return;
	}
	data_->Neutral_[number_] = _Neutral;
}

void PFIsolation::Hadron(Float_t _Hadron)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(91);//ERROR91
		return;
	}
	data_->Hadron_[number_] = _Hadron;
}

void PFIsolation::Photon(Float_t _Photon)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(92);//ERROR92
		return;
	}
	data_->Photon_[number_] = _Photon;
}



//Data_SelectedGenParticle Data_SelectedGenParticle Data_SelectedGenParticle Data_SelectedGenParticle Data_SelectedGenParticle Data_SelectedGenParticle Data_SelectedGenParticle Data_SelectedGenParticle Data_SelectedGenParticle Data_SelectedGenParticle 
BaseIO* Data_SelectedGenParticle::baseio = 0;
Data_SelectedGenParticle::Data_SelectedGenParticle(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
{
	PDGID_ = new Int_t[size_];
	Status_ = new Int_t[size_];
	Info_ = new Int_t[size_];
	IndirectMother_ = new Int_t[size_];
	Mother_ = new Int_t[size_];
	px_ = new Float_t[size_];
	py_ = new Float_t[size_];
	pz_ = new Float_t[size_];
	e_ = new Float_t[size_];
}

Data_SelectedGenParticle::~Data_SelectedGenParticle()
{
	delete[] PDGID_;
	delete[] Status_;
	delete[] Info_;
	delete[] IndirectMother_;
	delete[] Mother_;
	delete[] px_;
	delete[] py_;
	delete[] pz_;
	delete[] e_;
}

void Data_SelectedGenParticle::Fill()
{
	count_ = 0;
}

void Data_SelectedGenParticle::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_PDGID").c_str(), PDGID_, (prefix_ + "_PDGID[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_Status").c_str(), Status_, (prefix_ + "_Status[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_Info").c_str(), Info_, (prefix_ + "_Info[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_IndirectMother").c_str(), IndirectMother_, (prefix_ + "_IndirectMother[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_Mother").c_str(), Mother_, (prefix_ + "_Mother[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_px").c_str(), px_, (prefix_ + "_px[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_py").c_str(), py_, (prefix_ + "_py[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_pz").c_str(), pz_, (prefix_ + "_pz[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_e").c_str(), e_, (prefix_ + "_e[" + prefix_ + "_count]/F").c_str());
}

void Data_SelectedGenParticle::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_PDGID").c_str(), PDGID_);
	tree->SetBranchAddress((prefix_ + "_Status").c_str(), Status_);
	tree->SetBranchAddress((prefix_ + "_Info").c_str(), Info_);
	tree->SetBranchAddress((prefix_ + "_IndirectMother").c_str(), IndirectMother_);
	tree->SetBranchAddress((prefix_ + "_Mother").c_str(), Mother_);
	tree->SetBranchAddress((prefix_ + "_px").c_str(), px_);
	tree->SetBranchAddress((prefix_ + "_py").c_str(), py_);
	tree->SetBranchAddress((prefix_ + "_pz").c_str(), pz_);
	tree->SetBranchAddress((prefix_ + "_e").c_str(), e_);
}

void Data_SelectedGenParticle::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_PDGID").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Status").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Info").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_IndirectMother").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Mother").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_px").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_py").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_pz").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_e").c_str(), load);
}



//SelectedGenParticle == SelectedGenParticle == SelectedGenParticle == SelectedGenParticle == SelectedGenParticle == SelectedGenParticle == SelectedGenParticle == SelectedGenParticle == SelectedGenParticle == SelectedGenParticle == SelectedGenParticle == SelectedGenParticle == SelectedGenParticle == SelectedGenParticle == SelectedGenParticle == 
BaseIO* SelectedGenParticle::baseio = 0;
SelectedGenParticle::SelectedGenParticle(Data_SelectedGenParticle* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

SelectedGenParticle::SelectedGenParticle(const SelectedGenParticle& _selectedgenparticle) : 
number_( _selectedgenparticle.number_),
data_( _selectedgenparticle.data_)
{
}

void SelectedGenParticle::Init()
{
	if(baseio->IsWritable())
	{
		data_->count_ = number_+1;
	}
}

Int_t SelectedGenParticle::PDGID() const
{
	return data_->PDGID_[number_];
}

Int_t SelectedGenParticle::Status() const
{
	return data_->Status_[number_];
}

Int_t SelectedGenParticle::Info() const
{
	return data_->Info_[number_];
}

Int_t SelectedGenParticle::IndirectMother() const
{
	return data_->IndirectMother_[number_];
}

Int_t SelectedGenParticle::Mother() const
{
	return data_->Mother_[number_];
}

Float_t SelectedGenParticle::px() const
{
	return data_->px_[number_];
}

Float_t SelectedGenParticle::py() const
{
	return data_->py_[number_];
}

Float_t SelectedGenParticle::pz() const
{
	return data_->pz_[number_];
}

Float_t SelectedGenParticle::e() const
{
	return data_->e_[number_];
}

void SelectedGenParticle::PDGID(Int_t _PDGID)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(93);//ERROR93
		return;
	}
	data_->PDGID_[number_] = _PDGID;
}

void SelectedGenParticle::Status(Int_t _Status)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(94);//ERROR94
		return;
	}
	data_->Status_[number_] = _Status;
}

void SelectedGenParticle::Info(Int_t _Info)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(95);//ERROR95
		return;
	}
	data_->Info_[number_] = _Info;
}

void SelectedGenParticle::IndirectMother(Int_t _IndirectMother)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(96);//ERROR96
		return;
	}
	data_->IndirectMother_[number_] = _IndirectMother;
}

void SelectedGenParticle::Mother(Int_t _Mother)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(97);//ERROR97
		return;
	}
	data_->Mother_[number_] = _Mother;
}

void SelectedGenParticle::px(Float_t _px)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(98);//ERROR98
		return;
	}
	data_->px_[number_] = _px;
}

void SelectedGenParticle::py(Float_t _py)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(99);//ERROR99
		return;
	}
	data_->py_[number_] = _py;
}

void SelectedGenParticle::pz(Float_t _pz)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(100);//ERROR100
		return;
	}
	data_->pz_[number_] = _pz;
}

void SelectedGenParticle::e(Float_t _e)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(101);//ERROR101
		return;
	}
	data_->e_[number_] = _e;
}



//Data_AllGenParticle Data_AllGenParticle Data_AllGenParticle Data_AllGenParticle Data_AllGenParticle Data_AllGenParticle Data_AllGenParticle Data_AllGenParticle Data_AllGenParticle Data_AllGenParticle 
BaseIO* Data_AllGenParticle::baseio = 0;
Data_AllGenParticle::Data_AllGenParticle(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
{
	PDGID_ = new Int_t[size_];
	Status_ = new Int_t[size_];
	px_ = new Float_t[size_];
	py_ = new Float_t[size_];
	pz_ = new Float_t[size_];
	e_ = new Float_t[size_];
	vx_ = new Float_t[size_];
	vy_ = new Float_t[size_];
	vz_ = new Float_t[size_];
	Mother_num_ = new UInt_t[size_];
	Mother_ = new Int_t[size_*100];
	Mother_countmax_ = size_*100;
	Daughter_num_ = new UInt_t[size_];
	Daughter_ = new Int_t[size_*100];
	Daughter_countmax_ = size_*100;
}

Data_AllGenParticle::~Data_AllGenParticle()
{
	delete[] PDGID_;
	delete[] Status_;
	delete[] px_;
	delete[] py_;
	delete[] pz_;
	delete[] e_;
	delete[] vx_;
	delete[] vy_;
	delete[] vz_;
	delete[] Mother_;
	delete[] Mother_num_;
	delete[] Daughter_;
	delete[] Daughter_num_;
}

void Data_AllGenParticle::Fill()
{
	count_ = 0;
	Mother_count_ = 0;
	Daughter_count_ = 0;
}

void Data_AllGenParticle::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_PDGID").c_str(), PDGID_, (prefix_ + "_PDGID[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_Status").c_str(), Status_, (prefix_ + "_Status[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_px").c_str(), px_, (prefix_ + "_px[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_py").c_str(), py_, (prefix_ + "_py[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_pz").c_str(), pz_, (prefix_ + "_pz[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_e").c_str(), e_, (prefix_ + "_e[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_vx").c_str(), vx_, (prefix_ + "_vx[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_vy").c_str(), vy_, (prefix_ + "_vy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_vz").c_str(), vz_, (prefix_ + "_vz[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_Mother_count").c_str(), &Mother_count_, (prefix_ + "_Mother_count/i").c_str());
	tree->Branch((prefix_ + "_Mother_num").c_str(), Mother_num_, (prefix_ + "_Mother_num[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_Mother").c_str(), Mother_, (prefix_ + "_Mother[" + prefix_ + "_Mother_count]/I").c_str());
	tree->Branch((prefix_ + "_Daughter_count").c_str(), &Daughter_count_, (prefix_ + "_Daughter_count/i").c_str());
	tree->Branch((prefix_ + "_Daughter_num").c_str(), Daughter_num_, (prefix_ + "_Daughter_num[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_Daughter").c_str(), Daughter_, (prefix_ + "_Daughter[" + prefix_ + "_Daughter_count]/I").c_str());
}

void Data_AllGenParticle::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_PDGID").c_str(), PDGID_);
	tree->SetBranchAddress((prefix_ + "_Status").c_str(), Status_);
	tree->SetBranchAddress((prefix_ + "_px").c_str(), px_);
	tree->SetBranchAddress((prefix_ + "_py").c_str(), py_);
	tree->SetBranchAddress((prefix_ + "_pz").c_str(), pz_);
	tree->SetBranchAddress((prefix_ + "_e").c_str(), e_);
	tree->SetBranchAddress((prefix_ + "_vx").c_str(), vx_);
	tree->SetBranchAddress((prefix_ + "_vy").c_str(), vy_);
	tree->SetBranchAddress((prefix_ + "_vz").c_str(), vz_);
	tree->SetBranchAddress((prefix_ + "_Mother_count").c_str(), &Mother_count_);
	tree->SetBranchAddress((prefix_ + "_Mother_num").c_str(), Mother_num_);
	tree->SetBranchAddress((prefix_ + "_Mother").c_str(), Mother_);
	tree->SetBranchAddress((prefix_ + "_Daughter_count").c_str(), &Daughter_count_);
	tree->SetBranchAddress((prefix_ + "_Daughter_num").c_str(), Daughter_num_);
	tree->SetBranchAddress((prefix_ + "_Daughter").c_str(), Daughter_);
}

void Data_AllGenParticle::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_PDGID").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Status").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_px").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_py").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_pz").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_e").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_vx").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_vy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_vz").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Mother_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Mother_num").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Mother").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Daughter_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Daughter_num").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Daughter").c_str(), load);
}



//AllGenParticle == AllGenParticle == AllGenParticle == AllGenParticle == AllGenParticle == AllGenParticle == AllGenParticle == AllGenParticle == AllGenParticle == AllGenParticle == AllGenParticle == AllGenParticle == AllGenParticle == AllGenParticle == AllGenParticle == 
BaseIO* AllGenParticle::baseio = 0;
AllGenParticle::AllGenParticle(Data_AllGenParticle* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

AllGenParticle::AllGenParticle(const AllGenParticle& _allgenparticle) : 
number_( _allgenparticle.number_),
data_( _allgenparticle.data_)
{
}

void AllGenParticle::Init()
{
	if(baseio->IsWritable())
	{
		if(number_ == 0) {data_->Mother_num_[number_] = 0;}
		else {data_->Mother_num_[number_] = data_->Mother_num_[number_-1];}
		if(number_ == 0) {data_->Daughter_num_[number_] = 0;}
		else {data_->Daughter_num_[number_] = data_->Daughter_num_[number_-1];}
		data_->count_ = number_+1;
	}
}

Int_t AllGenParticle::PDGID() const
{
	return data_->PDGID_[number_];
}

Int_t AllGenParticle::Status() const
{
	return data_->Status_[number_];
}

Float_t AllGenParticle::px() const
{
	return data_->px_[number_];
}

Float_t AllGenParticle::py() const
{
	return data_->py_[number_];
}

Float_t AllGenParticle::pz() const
{
	return data_->pz_[number_];
}

Float_t AllGenParticle::e() const
{
	return data_->e_[number_];
}

Float_t AllGenParticle::vx() const
{
	return data_->vx_[number_];
}

Float_t AllGenParticle::vy() const
{
	return data_->vy_[number_];
}

Float_t AllGenParticle::vz() const
{
	return data_->vz_[number_];
}

UInt_t AllGenParticle::Num_Mother() const
{
	return number_ == 0 ? data_->Mother_num_[number_] : data_->Mother_num_[number_] - data_->Mother_num_[number_-1];
}

Int_t AllGenParticle::Mother(UInt_t n) const
{
	return number_ == 0 ? data_->Mother_[n] : data_->Mother_[data_->Mother_num_[number_-1]  + n];
}

UInt_t AllGenParticle::Num_Daughter() const
{
	return number_ == 0 ? data_->Daughter_num_[number_] : data_->Daughter_num_[number_] - data_->Daughter_num_[number_-1];
}

Int_t AllGenParticle::Daughter(UInt_t n) const
{
	return number_ == 0 ? data_->Daughter_[n] : data_->Daughter_[data_->Daughter_num_[number_-1]  + n];
}

void AllGenParticle::PDGID(Int_t _PDGID)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(102);//ERROR102
		return;
	}
	data_->PDGID_[number_] = _PDGID;
}

void AllGenParticle::Status(Int_t _Status)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(103);//ERROR103
		return;
	}
	data_->Status_[number_] = _Status;
}

void AllGenParticle::px(Float_t _px)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(104);//ERROR104
		return;
	}
	data_->px_[number_] = _px;
}

void AllGenParticle::py(Float_t _py)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(105);//ERROR105
		return;
	}
	data_->py_[number_] = _py;
}

void AllGenParticle::pz(Float_t _pz)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(106);//ERROR106
		return;
	}
	data_->pz_[number_] = _pz;
}

void AllGenParticle::e(Float_t _e)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(107);//ERROR107
		return;
	}
	data_->e_[number_] = _e;
}

void AllGenParticle::vx(Float_t _vx)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(108);//ERROR108
		return;
	}
	data_->vx_[number_] = _vx;
}

void AllGenParticle::vy(Float_t _vy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(109);//ERROR109
		return;
	}
	data_->vy_[number_] = _vy;
}

void AllGenParticle::vz(Float_t _vz)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(110);//ERROR110
		return;
	}
	data_->vz_[number_] = _vz;
}

void AllGenParticle::Mother(Int_t _Mother, UInt_t n)
{
	if(number_ != 0){n = data_->Mother_num_[number_-1] +n;}
	if(n >= data_->Mother_countmax_)
	{
		baseio->SetError(111);//ERROR111
		return;
	}
	if(n != data_->Mother_count_)
	{
		cerr << "Index already filled" << endl;
		return;
	}
	data_->Mother_[n] = _Mother;
	data_->Mother_num_[number_]++;
	data_->Mother_count_++;
}

void AllGenParticle::Daughter(Int_t _Daughter, UInt_t n)
{
	if(number_ != 0){n = data_->Daughter_num_[number_-1] +n;}
	if(n >= data_->Daughter_countmax_)
	{
		baseio->SetError(112);//ERROR112
		return;
	}
	if(n != data_->Daughter_count_)
	{
		cerr << "Index already filled" << endl;
		return;
	}
	data_->Daughter_[n] = _Daughter;
	data_->Daughter_num_[number_]++;
	data_->Daughter_count_++;
}



//Data_IOElectron Data_IOElectron Data_IOElectron Data_IOElectron Data_IOElectron Data_IOElectron Data_IOElectron Data_IOElectron Data_IOElectron Data_IOElectron 
BaseIO* Data_IOElectron::baseio = 0;
Data_IOElectron::Data_IOElectron(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
, PFR3_(new Data_PFIsolation(size_, prefix_ + "_PFR3"))
, SC_(new Data_IOSuperCluster(size_, prefix_ + "_SC"))
, GSFTrack_(new Data_IOTrack(size_, prefix_ + "_GSFTrack"))
{
	TriggerMatching_ = new UInt_t[size_];
	Info_ = new UInt_t[size_];
	px_ = new Float_t[size_];
	py_ = new Float_t[size_];
	pz_ = new Float_t[size_];
	DeltaEtaSCTrack_ = new Float_t[size_];
	DeltaPhiSCTrack_ = new Float_t[size_];
	ESCOverETrack_ = new Float_t[size_];
	ECalEnergy_ = new Float_t[size_];
	E1x5_ = new Float_t[size_];
	E2x5_ = new Float_t[size_];
	E5x5_ = new Float_t[size_];
	R9_ = new Float_t[size_];
	SigmaIEtaIEta_ = new Float_t[size_];
	SigmaIPhiIPhi_ = new Float_t[size_];
	SigmaIEtaIPhi_ = new Float_t[size_];
	EHCalTowerOverECalD1_ = new Float_t[size_];
	EHCalTowerOverECalD2_ = new Float_t[size_];
}

Data_IOElectron::~Data_IOElectron()
{
	delete[] TriggerMatching_;
	delete[] Info_;
	delete PFR3_;
	delete SC_;
	delete GSFTrack_;
	delete[] px_;
	delete[] py_;
	delete[] pz_;
	delete[] DeltaEtaSCTrack_;
	delete[] DeltaPhiSCTrack_;
	delete[] ESCOverETrack_;
	delete[] ECalEnergy_;
	delete[] E1x5_;
	delete[] E2x5_;
	delete[] E5x5_;
	delete[] R9_;
	delete[] SigmaIEtaIEta_;
	delete[] SigmaIPhiIPhi_;
	delete[] SigmaIEtaIPhi_;
	delete[] EHCalTowerOverECalD1_;
	delete[] EHCalTowerOverECalD2_;
}

void Data_IOElectron::Fill()
{
	count_ = 0;
	PFR3_->Fill();
	SC_->Fill();
	GSFTrack_->Fill();
}

void Data_IOElectron::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_TriggerMatching").c_str(), TriggerMatching_, (prefix_ + "_TriggerMatching[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_Info").c_str(), Info_, (prefix_ + "_Info[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_px").c_str(), px_, (prefix_ + "_px[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_py").c_str(), py_, (prefix_ + "_py[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_pz").c_str(), pz_, (prefix_ + "_pz[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_DeltaEtaSCTrack").c_str(), DeltaEtaSCTrack_, (prefix_ + "_DeltaEtaSCTrack[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_DeltaPhiSCTrack").c_str(), DeltaPhiSCTrack_, (prefix_ + "_DeltaPhiSCTrack[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ESCOverETrack").c_str(), ESCOverETrack_, (prefix_ + "_ESCOverETrack[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ECalEnergy").c_str(), ECalEnergy_, (prefix_ + "_ECalEnergy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_E1x5").c_str(), E1x5_, (prefix_ + "_E1x5[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_E2x5").c_str(), E2x5_, (prefix_ + "_E2x5[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_E5x5").c_str(), E5x5_, (prefix_ + "_E5x5[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_R9").c_str(), R9_, (prefix_ + "_R9[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_SigmaIEtaIEta").c_str(), SigmaIEtaIEta_, (prefix_ + "_SigmaIEtaIEta[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_SigmaIPhiIPhi").c_str(), SigmaIPhiIPhi_, (prefix_ + "_SigmaIPhiIPhi[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_SigmaIEtaIPhi").c_str(), SigmaIEtaIPhi_, (prefix_ + "_SigmaIEtaIPhi[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_EHCalTowerOverECalD1").c_str(), EHCalTowerOverECalD1_, (prefix_ + "_EHCalTowerOverECalD1[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_EHCalTowerOverECalD2").c_str(), EHCalTowerOverECalD2_, (prefix_ + "_EHCalTowerOverECalD2[" + prefix_ + "_count]/F").c_str());
	PFR3_->SetUpWrite(tree);
	SC_->SetUpWrite(tree);
	GSFTrack_->SetUpWrite(tree);
}

void Data_IOElectron::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_TriggerMatching").c_str(), TriggerMatching_);
	tree->SetBranchAddress((prefix_ + "_Info").c_str(), Info_);
	tree->SetBranchAddress((prefix_ + "_px").c_str(), px_);
	tree->SetBranchAddress((prefix_ + "_py").c_str(), py_);
	tree->SetBranchAddress((prefix_ + "_pz").c_str(), pz_);
	tree->SetBranchAddress((prefix_ + "_DeltaEtaSCTrack").c_str(), DeltaEtaSCTrack_);
	tree->SetBranchAddress((prefix_ + "_DeltaPhiSCTrack").c_str(), DeltaPhiSCTrack_);
	tree->SetBranchAddress((prefix_ + "_ESCOverETrack").c_str(), ESCOverETrack_);
	tree->SetBranchAddress((prefix_ + "_ECalEnergy").c_str(), ECalEnergy_);
	tree->SetBranchAddress((prefix_ + "_E1x5").c_str(), E1x5_);
	tree->SetBranchAddress((prefix_ + "_E2x5").c_str(), E2x5_);
	tree->SetBranchAddress((prefix_ + "_E5x5").c_str(), E5x5_);
	tree->SetBranchAddress((prefix_ + "_R9").c_str(), R9_);
	tree->SetBranchAddress((prefix_ + "_SigmaIEtaIEta").c_str(), SigmaIEtaIEta_);
	tree->SetBranchAddress((prefix_ + "_SigmaIPhiIPhi").c_str(), SigmaIPhiIPhi_);
	tree->SetBranchAddress((prefix_ + "_SigmaIEtaIPhi").c_str(), SigmaIEtaIPhi_);
	tree->SetBranchAddress((prefix_ + "_EHCalTowerOverECalD1").c_str(), EHCalTowerOverECalD1_);
	tree->SetBranchAddress((prefix_ + "_EHCalTowerOverECalD2").c_str(), EHCalTowerOverECalD2_);
	PFR3_->SetUpRead(tree);
	SC_->SetUpRead(tree);
	GSFTrack_->SetUpRead(tree);
}

void Data_IOElectron::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_TriggerMatching").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Info").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_px").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_py").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_pz").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_DeltaEtaSCTrack").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_DeltaPhiSCTrack").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ESCOverETrack").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ECalEnergy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_E1x5").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_E2x5").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_E5x5").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_R9").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_SigmaIEtaIEta").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_SigmaIPhiIPhi").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_SigmaIEtaIPhi").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_EHCalTowerOverECalD1").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_EHCalTowerOverECalD2").c_str(), load);
	PFR3_->Load(tree, load);
	SC_->Load(tree, load);
	GSFTrack_->Load(tree, load);
}



//IOElectron == IOElectron == IOElectron == IOElectron == IOElectron == IOElectron == IOElectron == IOElectron == IOElectron == IOElectron == IOElectron == IOElectron == IOElectron == IOElectron == IOElectron == 
BaseIO* IOElectron::baseio = 0;
IOElectron::IOElectron(Data_IOElectron* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

IOElectron::IOElectron(const IOElectron& _ioelectron) : 
number_( _ioelectron.number_),
data_( _ioelectron.data_)
{
}

void IOElectron::Init()
{
	if(baseio->IsWritable())
	{
		data_->count_ = number_+1;
	}
}

UInt_t IOElectron::TriggerMatching() const
{
	return data_->TriggerMatching_[number_];
}

UInt_t IOElectron::Info() const
{
	return data_->Info_[number_];
}

PFIsolation IOElectron::PFR3() const
{
	return PFIsolation(data_->PFR3_, number_);
}

IOSuperCluster IOElectron::SC() const
{
	return IOSuperCluster(data_->SC_, number_);
}

IOTrack IOElectron::GSFTrack() const
{
	return IOTrack(data_->GSFTrack_, number_);
}

Float_t IOElectron::px() const
{
	return data_->px_[number_];
}

Float_t IOElectron::py() const
{
	return data_->py_[number_];
}

Float_t IOElectron::pz() const
{
	return data_->pz_[number_];
}

Float_t IOElectron::DeltaEtaSCTrack() const
{
	return data_->DeltaEtaSCTrack_[number_];
}

Float_t IOElectron::DeltaPhiSCTrack() const
{
	return data_->DeltaPhiSCTrack_[number_];
}

Float_t IOElectron::ESCOverETrack() const
{
	return data_->ESCOverETrack_[number_];
}

Float_t IOElectron::ECalEnergy() const
{
	return data_->ECalEnergy_[number_];
}

Float_t IOElectron::E1x5() const
{
	return data_->E1x5_[number_];
}

Float_t IOElectron::E2x5() const
{
	return data_->E2x5_[number_];
}

Float_t IOElectron::E5x5() const
{
	return data_->E5x5_[number_];
}

Float_t IOElectron::R9() const
{
	return data_->R9_[number_];
}

Float_t IOElectron::SigmaIEtaIEta() const
{
	return data_->SigmaIEtaIEta_[number_];
}

Float_t IOElectron::SigmaIPhiIPhi() const
{
	return data_->SigmaIPhiIPhi_[number_];
}

Float_t IOElectron::SigmaIEtaIPhi() const
{
	return data_->SigmaIEtaIPhi_[number_];
}

Float_t IOElectron::EHCalTowerOverECalD1() const
{
	return data_->EHCalTowerOverECalD1_[number_];
}

Float_t IOElectron::EHCalTowerOverECalD2() const
{
	return data_->EHCalTowerOverECalD2_[number_];
}

void IOElectron::TriggerMatching(UInt_t _TriggerMatching)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(113);//ERROR113
		return;
	}
	data_->TriggerMatching_[number_] = _TriggerMatching;
}

void IOElectron::Info(UInt_t _Info)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(114);//ERROR114
		return;
	}
	data_->Info_[number_] = _Info;
}

void IOElectron::px(Float_t _px)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(115);//ERROR115
		return;
	}
	data_->px_[number_] = _px;
}

void IOElectron::py(Float_t _py)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(116);//ERROR116
		return;
	}
	data_->py_[number_] = _py;
}

void IOElectron::pz(Float_t _pz)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(117);//ERROR117
		return;
	}
	data_->pz_[number_] = _pz;
}

void IOElectron::DeltaEtaSCTrack(Float_t _DeltaEtaSCTrack)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(118);//ERROR118
		return;
	}
	data_->DeltaEtaSCTrack_[number_] = _DeltaEtaSCTrack;
}

void IOElectron::DeltaPhiSCTrack(Float_t _DeltaPhiSCTrack)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(119);//ERROR119
		return;
	}
	data_->DeltaPhiSCTrack_[number_] = _DeltaPhiSCTrack;
}

void IOElectron::ESCOverETrack(Float_t _ESCOverETrack)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(120);//ERROR120
		return;
	}
	data_->ESCOverETrack_[number_] = _ESCOverETrack;
}

void IOElectron::ECalEnergy(Float_t _ECalEnergy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(121);//ERROR121
		return;
	}
	data_->ECalEnergy_[number_] = _ECalEnergy;
}

void IOElectron::E1x5(Float_t _E1x5)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(122);//ERROR122
		return;
	}
	data_->E1x5_[number_] = _E1x5;
}

void IOElectron::E2x5(Float_t _E2x5)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(123);//ERROR123
		return;
	}
	data_->E2x5_[number_] = _E2x5;
}

void IOElectron::E5x5(Float_t _E5x5)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(124);//ERROR124
		return;
	}
	data_->E5x5_[number_] = _E5x5;
}

void IOElectron::R9(Float_t _R9)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(125);//ERROR125
		return;
	}
	data_->R9_[number_] = _R9;
}

void IOElectron::SigmaIEtaIEta(Float_t _SigmaIEtaIEta)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(126);//ERROR126
		return;
	}
	data_->SigmaIEtaIEta_[number_] = _SigmaIEtaIEta;
}

void IOElectron::SigmaIPhiIPhi(Float_t _SigmaIPhiIPhi)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(127);//ERROR127
		return;
	}
	data_->SigmaIPhiIPhi_[number_] = _SigmaIPhiIPhi;
}

void IOElectron::SigmaIEtaIPhi(Float_t _SigmaIEtaIPhi)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(128);//ERROR128
		return;
	}
	data_->SigmaIEtaIPhi_[number_] = _SigmaIEtaIPhi;
}

void IOElectron::EHCalTowerOverECalD1(Float_t _EHCalTowerOverECalD1)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(129);//ERROR129
		return;
	}
	data_->EHCalTowerOverECalD1_[number_] = _EHCalTowerOverECalD1;
}

void IOElectron::EHCalTowerOverECalD2(Float_t _EHCalTowerOverECalD2)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(130);//ERROR130
		return;
	}
	data_->EHCalTowerOverECalD2_[number_] = _EHCalTowerOverECalD2;
}



//Data_IOTrack Data_IOTrack Data_IOTrack Data_IOTrack Data_IOTrack Data_IOTrack Data_IOTrack Data_IOTrack Data_IOTrack Data_IOTrack 
BaseIO* Data_IOTrack::baseio = 0;
Data_IOTrack::Data_IOTrack(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
{
	VertexNumber_ = new Int_t[size_];
	Charge_ = new Char_t[size_];
	NStripHits_ = new UChar_t[size_];
	NPixelHits_ = new UChar_t[size_];
	NMissingHits_ = new UChar_t[size_];
	NMissingInnerHits_ = new UChar_t[size_];
	NPixelLayers_ = new UChar_t[size_];
	NStripLayers_ = new UChar_t[size_];
	px_ = new Float_t[size_];
	py_ = new Float_t[size_];
	pz_ = new Float_t[size_];
	ChiQ_ = new Float_t[size_];
	NDOF_ = new Float_t[size_];
	Dxy_ = new Float_t[size_];
	DxyUnc_ = new Float_t[size_];
	Dz_ = new Float_t[size_];
	DzUnc_ = new Float_t[size_];
	DeDx_ = new Float_t[size_];
}

Data_IOTrack::~Data_IOTrack()
{
	delete[] VertexNumber_;
	delete[] Charge_;
	delete[] NStripHits_;
	delete[] NPixelHits_;
	delete[] NMissingHits_;
	delete[] NMissingInnerHits_;
	delete[] NPixelLayers_;
	delete[] NStripLayers_;
	delete[] px_;
	delete[] py_;
	delete[] pz_;
	delete[] ChiQ_;
	delete[] NDOF_;
	delete[] Dxy_;
	delete[] DxyUnc_;
	delete[] Dz_;
	delete[] DzUnc_;
	delete[] DeDx_;
}

void Data_IOTrack::Fill()
{
	count_ = 0;
}

void Data_IOTrack::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_VertexNumber").c_str(), VertexNumber_, (prefix_ + "_VertexNumber[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_Charge").c_str(), Charge_, (prefix_ + "_Charge[" + prefix_ + "_count]/B").c_str());
	tree->Branch((prefix_ + "_NStripHits").c_str(), NStripHits_, (prefix_ + "_NStripHits[" + prefix_ + "_count]/b").c_str());
	tree->Branch((prefix_ + "_NPixelHits").c_str(), NPixelHits_, (prefix_ + "_NPixelHits[" + prefix_ + "_count]/b").c_str());
	tree->Branch((prefix_ + "_NMissingHits").c_str(), NMissingHits_, (prefix_ + "_NMissingHits[" + prefix_ + "_count]/b").c_str());
	tree->Branch((prefix_ + "_NMissingInnerHits").c_str(), NMissingInnerHits_, (prefix_ + "_NMissingInnerHits[" + prefix_ + "_count]/b").c_str());
	tree->Branch((prefix_ + "_NPixelLayers").c_str(), NPixelLayers_, (prefix_ + "_NPixelLayers[" + prefix_ + "_count]/b").c_str());
	tree->Branch((prefix_ + "_NStripLayers").c_str(), NStripLayers_, (prefix_ + "_NStripLayers[" + prefix_ + "_count]/b").c_str());
	tree->Branch((prefix_ + "_px").c_str(), px_, (prefix_ + "_px[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_py").c_str(), py_, (prefix_ + "_py[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_pz").c_str(), pz_, (prefix_ + "_pz[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ChiQ").c_str(), ChiQ_, (prefix_ + "_ChiQ[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_NDOF").c_str(), NDOF_, (prefix_ + "_NDOF[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_Dxy").c_str(), Dxy_, (prefix_ + "_Dxy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_DxyUnc").c_str(), DxyUnc_, (prefix_ + "_DxyUnc[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_Dz").c_str(), Dz_, (prefix_ + "_Dz[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_DzUnc").c_str(), DzUnc_, (prefix_ + "_DzUnc[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_DeDx").c_str(), DeDx_, (prefix_ + "_DeDx[" + prefix_ + "_count]/F").c_str());
}

void Data_IOTrack::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_VertexNumber").c_str(), VertexNumber_);
	tree->SetBranchAddress((prefix_ + "_Charge").c_str(), Charge_);
	tree->SetBranchAddress((prefix_ + "_NStripHits").c_str(), NStripHits_);
	tree->SetBranchAddress((prefix_ + "_NPixelHits").c_str(), NPixelHits_);
	tree->SetBranchAddress((prefix_ + "_NMissingHits").c_str(), NMissingHits_);
	tree->SetBranchAddress((prefix_ + "_NMissingInnerHits").c_str(), NMissingInnerHits_);
	tree->SetBranchAddress((prefix_ + "_NPixelLayers").c_str(), NPixelLayers_);
	tree->SetBranchAddress((prefix_ + "_NStripLayers").c_str(), NStripLayers_);
	tree->SetBranchAddress((prefix_ + "_px").c_str(), px_);
	tree->SetBranchAddress((prefix_ + "_py").c_str(), py_);
	tree->SetBranchAddress((prefix_ + "_pz").c_str(), pz_);
	tree->SetBranchAddress((prefix_ + "_ChiQ").c_str(), ChiQ_);
	tree->SetBranchAddress((prefix_ + "_NDOF").c_str(), NDOF_);
	tree->SetBranchAddress((prefix_ + "_Dxy").c_str(), Dxy_);
	tree->SetBranchAddress((prefix_ + "_DxyUnc").c_str(), DxyUnc_);
	tree->SetBranchAddress((prefix_ + "_Dz").c_str(), Dz_);
	tree->SetBranchAddress((prefix_ + "_DzUnc").c_str(), DzUnc_);
	tree->SetBranchAddress((prefix_ + "_DeDx").c_str(), DeDx_);
}

void Data_IOTrack::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_VertexNumber").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Charge").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NStripHits").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NPixelHits").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NMissingHits").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NMissingInnerHits").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NPixelLayers").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NStripLayers").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_px").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_py").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_pz").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ChiQ").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NDOF").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Dxy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_DxyUnc").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Dz").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_DzUnc").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_DeDx").c_str(), load);
}



//IOTrack == IOTrack == IOTrack == IOTrack == IOTrack == IOTrack == IOTrack == IOTrack == IOTrack == IOTrack == IOTrack == IOTrack == IOTrack == IOTrack == IOTrack == 
BaseIO* IOTrack::baseio = 0;
IOTrack::IOTrack(Data_IOTrack* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

IOTrack::IOTrack(const IOTrack& _iotrack) : 
number_( _iotrack.number_),
data_( _iotrack.data_)
{
}

void IOTrack::Init()
{
	if(baseio->IsWritable())
	{
		data_->count_ = number_+1;
	}
}

Int_t IOTrack::VertexNumber() const
{
	return data_->VertexNumber_[number_];
}

Char_t IOTrack::Charge() const
{
	return data_->Charge_[number_];
}

UChar_t IOTrack::NStripHits() const
{
	return data_->NStripHits_[number_];
}

UChar_t IOTrack::NPixelHits() const
{
	return data_->NPixelHits_[number_];
}

UChar_t IOTrack::NMissingHits() const
{
	return data_->NMissingHits_[number_];
}

UChar_t IOTrack::NMissingInnerHits() const
{
	return data_->NMissingInnerHits_[number_];
}

UChar_t IOTrack::NPixelLayers() const
{
	return data_->NPixelLayers_[number_];
}

UChar_t IOTrack::NStripLayers() const
{
	return data_->NStripLayers_[number_];
}

Float_t IOTrack::px() const
{
	return data_->px_[number_];
}

Float_t IOTrack::py() const
{
	return data_->py_[number_];
}

Float_t IOTrack::pz() const
{
	return data_->pz_[number_];
}

Float_t IOTrack::ChiQ() const
{
	return data_->ChiQ_[number_];
}

Float_t IOTrack::NDOF() const
{
	return data_->NDOF_[number_];
}

Float_t IOTrack::Dxy() const
{
	return data_->Dxy_[number_];
}

Float_t IOTrack::DxyUnc() const
{
	return data_->DxyUnc_[number_];
}

Float_t IOTrack::Dz() const
{
	return data_->Dz_[number_];
}

Float_t IOTrack::DzUnc() const
{
	return data_->DzUnc_[number_];
}

Float_t IOTrack::DeDx() const
{
	return data_->DeDx_[number_];
}

void IOTrack::VertexNumber(Int_t _VertexNumber)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(131);//ERROR131
		return;
	}
	data_->VertexNumber_[number_] = _VertexNumber;
}

void IOTrack::Charge(Char_t _Charge)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(132);//ERROR132
		return;
	}
	data_->Charge_[number_] = _Charge;
}

void IOTrack::NStripHits(UChar_t _NStripHits)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(133);//ERROR133
		return;
	}
	data_->NStripHits_[number_] = _NStripHits;
}

void IOTrack::NPixelHits(UChar_t _NPixelHits)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(134);//ERROR134
		return;
	}
	data_->NPixelHits_[number_] = _NPixelHits;
}

void IOTrack::NMissingHits(UChar_t _NMissingHits)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(135);//ERROR135
		return;
	}
	data_->NMissingHits_[number_] = _NMissingHits;
}

void IOTrack::NMissingInnerHits(UChar_t _NMissingInnerHits)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(136);//ERROR136
		return;
	}
	data_->NMissingInnerHits_[number_] = _NMissingInnerHits;
}

void IOTrack::NPixelLayers(UChar_t _NPixelLayers)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(137);//ERROR137
		return;
	}
	data_->NPixelLayers_[number_] = _NPixelLayers;
}

void IOTrack::NStripLayers(UChar_t _NStripLayers)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(138);//ERROR138
		return;
	}
	data_->NStripLayers_[number_] = _NStripLayers;
}

void IOTrack::px(Float_t _px)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(139);//ERROR139
		return;
	}
	data_->px_[number_] = _px;
}

void IOTrack::py(Float_t _py)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(140);//ERROR140
		return;
	}
	data_->py_[number_] = _py;
}

void IOTrack::pz(Float_t _pz)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(141);//ERROR141
		return;
	}
	data_->pz_[number_] = _pz;
}

void IOTrack::ChiQ(Float_t _ChiQ)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(142);//ERROR142
		return;
	}
	data_->ChiQ_[number_] = _ChiQ;
}

void IOTrack::NDOF(Float_t _NDOF)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(143);//ERROR143
		return;
	}
	data_->NDOF_[number_] = _NDOF;
}

void IOTrack::Dxy(Float_t _Dxy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(144);//ERROR144
		return;
	}
	data_->Dxy_[number_] = _Dxy;
}

void IOTrack::DxyUnc(Float_t _DxyUnc)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(145);//ERROR145
		return;
	}
	data_->DxyUnc_[number_] = _DxyUnc;
}

void IOTrack::Dz(Float_t _Dz)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(146);//ERROR146
		return;
	}
	data_->Dz_[number_] = _Dz;
}

void IOTrack::DzUnc(Float_t _DzUnc)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(147);//ERROR147
		return;
	}
	data_->DzUnc_[number_] = _DzUnc;
}

void IOTrack::DeDx(Float_t _DeDx)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(148);//ERROR148
		return;
	}
	data_->DeDx_[number_] = _DeDx;
}



//Data_IOBeamSpot Data_IOBeamSpot Data_IOBeamSpot Data_IOBeamSpot Data_IOBeamSpot Data_IOBeamSpot Data_IOBeamSpot Data_IOBeamSpot Data_IOBeamSpot Data_IOBeamSpot 
BaseIO* Data_IOBeamSpot::baseio = 0;
Data_IOBeamSpot::Data_IOBeamSpot(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
{
	Vx_ = new Float_t[size_];
	Vy_ = new Float_t[size_];
	Vz_ = new Float_t[size_];
	XWidth_ = new Float_t[size_];
	YWidth_ = new Float_t[size_];
	ZWidth_ = new Float_t[size_];
}

Data_IOBeamSpot::~Data_IOBeamSpot()
{
	delete[] Vx_;
	delete[] Vy_;
	delete[] Vz_;
	delete[] XWidth_;
	delete[] YWidth_;
	delete[] ZWidth_;
}

void Data_IOBeamSpot::Fill()
{
	count_ = 0;
}

void Data_IOBeamSpot::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_Vx").c_str(), Vx_, (prefix_ + "_Vx[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_Vy").c_str(), Vy_, (prefix_ + "_Vy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_Vz").c_str(), Vz_, (prefix_ + "_Vz[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_XWidth").c_str(), XWidth_, (prefix_ + "_XWidth[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_YWidth").c_str(), YWidth_, (prefix_ + "_YWidth[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_ZWidth").c_str(), ZWidth_, (prefix_ + "_ZWidth[" + prefix_ + "_count]/F").c_str());
}

void Data_IOBeamSpot::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_Vx").c_str(), Vx_);
	tree->SetBranchAddress((prefix_ + "_Vy").c_str(), Vy_);
	tree->SetBranchAddress((prefix_ + "_Vz").c_str(), Vz_);
	tree->SetBranchAddress((prefix_ + "_XWidth").c_str(), XWidth_);
	tree->SetBranchAddress((prefix_ + "_YWidth").c_str(), YWidth_);
	tree->SetBranchAddress((prefix_ + "_ZWidth").c_str(), ZWidth_);
}

void Data_IOBeamSpot::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Vx").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Vy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Vz").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_XWidth").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_YWidth").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_ZWidth").c_str(), load);
}



//IOBeamSpot == IOBeamSpot == IOBeamSpot == IOBeamSpot == IOBeamSpot == IOBeamSpot == IOBeamSpot == IOBeamSpot == IOBeamSpot == IOBeamSpot == IOBeamSpot == IOBeamSpot == IOBeamSpot == IOBeamSpot == IOBeamSpot == 
BaseIO* IOBeamSpot::baseio = 0;
IOBeamSpot::IOBeamSpot(Data_IOBeamSpot* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

IOBeamSpot::IOBeamSpot(const IOBeamSpot& _iobeamspot) : 
number_( _iobeamspot.number_),
data_( _iobeamspot.data_)
{
}

void IOBeamSpot::Init()
{
	if(baseio->IsWritable())
	{
		data_->count_ = number_+1;
	}
}

Float_t IOBeamSpot::Vx() const
{
	return data_->Vx_[number_];
}

Float_t IOBeamSpot::Vy() const
{
	return data_->Vy_[number_];
}

Float_t IOBeamSpot::Vz() const
{
	return data_->Vz_[number_];
}

Float_t IOBeamSpot::XWidth() const
{
	return data_->XWidth_[number_];
}

Float_t IOBeamSpot::YWidth() const
{
	return data_->YWidth_[number_];
}

Float_t IOBeamSpot::ZWidth() const
{
	return data_->ZWidth_[number_];
}

void IOBeamSpot::Vx(Float_t _Vx)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(149);//ERROR149
		return;
	}
	data_->Vx_[number_] = _Vx;
}

void IOBeamSpot::Vy(Float_t _Vy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(150);//ERROR150
		return;
	}
	data_->Vy_[number_] = _Vy;
}

void IOBeamSpot::Vz(Float_t _Vz)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(151);//ERROR151
		return;
	}
	data_->Vz_[number_] = _Vz;
}

void IOBeamSpot::XWidth(Float_t _XWidth)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(152);//ERROR152
		return;
	}
	data_->XWidth_[number_] = _XWidth;
}

void IOBeamSpot::YWidth(Float_t _YWidth)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(153);//ERROR153
		return;
	}
	data_->YWidth_[number_] = _YWidth;
}

void IOBeamSpot::ZWidth(Float_t _ZWidth)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(154);//ERROR154
		return;
	}
	data_->ZWidth_[number_] = _ZWidth;
}



//Data_IOPhoton Data_IOPhoton Data_IOPhoton Data_IOPhoton Data_IOPhoton Data_IOPhoton Data_IOPhoton Data_IOPhoton Data_IOPhoton Data_IOPhoton 
BaseIO* Data_IOPhoton::baseio = 0;
Data_IOPhoton::Data_IOPhoton(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
, PFR3_(new Data_PFIsolation(size_, prefix_ + "_PFR3"))
, SC_(new Data_IOSuperCluster(size_, prefix_ + "_SC"))
{
	TriggerMatching_ = new UInt_t[size_];
	Info_ = new UInt_t[size_];
	px_ = new Float_t[size_];
	py_ = new Float_t[size_];
	pz_ = new Float_t[size_];
	E1x5_ = new Float_t[size_];
	E2x5_ = new Float_t[size_];
	E3x3_ = new Float_t[size_];
	E5x5_ = new Float_t[size_];
	MaxCrystalEnergy_ = new Float_t[size_];
	SigmaIEtaIEta_ = new Float_t[size_];
	SigmaIPhiIPhi_ = new Float_t[size_];
	SigmaIEtaIPhi_ = new Float_t[size_];
	EHCalTowerOverECalD1_ = new Float_t[size_];
	EHCalTowerOverECalD2_ = new Float_t[size_];
}

Data_IOPhoton::~Data_IOPhoton()
{
	delete[] TriggerMatching_;
	delete[] Info_;
	delete PFR3_;
	delete SC_;
	delete[] px_;
	delete[] py_;
	delete[] pz_;
	delete[] E1x5_;
	delete[] E2x5_;
	delete[] E3x3_;
	delete[] E5x5_;
	delete[] MaxCrystalEnergy_;
	delete[] SigmaIEtaIEta_;
	delete[] SigmaIPhiIPhi_;
	delete[] SigmaIEtaIPhi_;
	delete[] EHCalTowerOverECalD1_;
	delete[] EHCalTowerOverECalD2_;
}

void Data_IOPhoton::Fill()
{
	count_ = 0;
	PFR3_->Fill();
	SC_->Fill();
}

void Data_IOPhoton::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_TriggerMatching").c_str(), TriggerMatching_, (prefix_ + "_TriggerMatching[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_Info").c_str(), Info_, (prefix_ + "_Info[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_px").c_str(), px_, (prefix_ + "_px[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_py").c_str(), py_, (prefix_ + "_py[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_pz").c_str(), pz_, (prefix_ + "_pz[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_E1x5").c_str(), E1x5_, (prefix_ + "_E1x5[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_E2x5").c_str(), E2x5_, (prefix_ + "_E2x5[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_E3x3").c_str(), E3x3_, (prefix_ + "_E3x3[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_E5x5").c_str(), E5x5_, (prefix_ + "_E5x5[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_MaxCrystalEnergy").c_str(), MaxCrystalEnergy_, (prefix_ + "_MaxCrystalEnergy[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_SigmaIEtaIEta").c_str(), SigmaIEtaIEta_, (prefix_ + "_SigmaIEtaIEta[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_SigmaIPhiIPhi").c_str(), SigmaIPhiIPhi_, (prefix_ + "_SigmaIPhiIPhi[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_SigmaIEtaIPhi").c_str(), SigmaIEtaIPhi_, (prefix_ + "_SigmaIEtaIPhi[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_EHCalTowerOverECalD1").c_str(), EHCalTowerOverECalD1_, (prefix_ + "_EHCalTowerOverECalD1[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_EHCalTowerOverECalD2").c_str(), EHCalTowerOverECalD2_, (prefix_ + "_EHCalTowerOverECalD2[" + prefix_ + "_count]/F").c_str());
	PFR3_->SetUpWrite(tree);
	SC_->SetUpWrite(tree);
}

void Data_IOPhoton::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_TriggerMatching").c_str(), TriggerMatching_);
	tree->SetBranchAddress((prefix_ + "_Info").c_str(), Info_);
	tree->SetBranchAddress((prefix_ + "_px").c_str(), px_);
	tree->SetBranchAddress((prefix_ + "_py").c_str(), py_);
	tree->SetBranchAddress((prefix_ + "_pz").c_str(), pz_);
	tree->SetBranchAddress((prefix_ + "_E1x5").c_str(), E1x5_);
	tree->SetBranchAddress((prefix_ + "_E2x5").c_str(), E2x5_);
	tree->SetBranchAddress((prefix_ + "_E3x3").c_str(), E3x3_);
	tree->SetBranchAddress((prefix_ + "_E5x5").c_str(), E5x5_);
	tree->SetBranchAddress((prefix_ + "_MaxCrystalEnergy").c_str(), MaxCrystalEnergy_);
	tree->SetBranchAddress((prefix_ + "_SigmaIEtaIEta").c_str(), SigmaIEtaIEta_);
	tree->SetBranchAddress((prefix_ + "_SigmaIPhiIPhi").c_str(), SigmaIPhiIPhi_);
	tree->SetBranchAddress((prefix_ + "_SigmaIEtaIPhi").c_str(), SigmaIEtaIPhi_);
	tree->SetBranchAddress((prefix_ + "_EHCalTowerOverECalD1").c_str(), EHCalTowerOverECalD1_);
	tree->SetBranchAddress((prefix_ + "_EHCalTowerOverECalD2").c_str(), EHCalTowerOverECalD2_);
	PFR3_->SetUpRead(tree);
	SC_->SetUpRead(tree);
}

void Data_IOPhoton::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_TriggerMatching").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_Info").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_px").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_py").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_pz").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_E1x5").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_E2x5").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_E3x3").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_E5x5").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_MaxCrystalEnergy").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_SigmaIEtaIEta").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_SigmaIPhiIPhi").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_SigmaIEtaIPhi").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_EHCalTowerOverECalD1").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_EHCalTowerOverECalD2").c_str(), load);
	PFR3_->Load(tree, load);
	SC_->Load(tree, load);
}



//IOPhoton == IOPhoton == IOPhoton == IOPhoton == IOPhoton == IOPhoton == IOPhoton == IOPhoton == IOPhoton == IOPhoton == IOPhoton == IOPhoton == IOPhoton == IOPhoton == IOPhoton == 
BaseIO* IOPhoton::baseio = 0;
IOPhoton::IOPhoton(Data_IOPhoton* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

IOPhoton::IOPhoton(const IOPhoton& _iophoton) : 
number_( _iophoton.number_),
data_( _iophoton.data_)
{
}

void IOPhoton::Init()
{
	if(baseio->IsWritable())
	{
		data_->count_ = number_+1;
	}
}

UInt_t IOPhoton::TriggerMatching() const
{
	return data_->TriggerMatching_[number_];
}

UInt_t IOPhoton::Info() const
{
	return data_->Info_[number_];
}

PFIsolation IOPhoton::PFR3() const
{
	return PFIsolation(data_->PFR3_, number_);
}

IOSuperCluster IOPhoton::SC() const
{
	return IOSuperCluster(data_->SC_, number_);
}

Float_t IOPhoton::px() const
{
	return data_->px_[number_];
}

Float_t IOPhoton::py() const
{
	return data_->py_[number_];
}

Float_t IOPhoton::pz() const
{
	return data_->pz_[number_];
}

Float_t IOPhoton::E1x5() const
{
	return data_->E1x5_[number_];
}

Float_t IOPhoton::E2x5() const
{
	return data_->E2x5_[number_];
}

Float_t IOPhoton::E3x3() const
{
	return data_->E3x3_[number_];
}

Float_t IOPhoton::E5x5() const
{
	return data_->E5x5_[number_];
}

Float_t IOPhoton::MaxCrystalEnergy() const
{
	return data_->MaxCrystalEnergy_[number_];
}

Float_t IOPhoton::SigmaIEtaIEta() const
{
	return data_->SigmaIEtaIEta_[number_];
}

Float_t IOPhoton::SigmaIPhiIPhi() const
{
	return data_->SigmaIPhiIPhi_[number_];
}

Float_t IOPhoton::SigmaIEtaIPhi() const
{
	return data_->SigmaIEtaIPhi_[number_];
}

Float_t IOPhoton::EHCalTowerOverECalD1() const
{
	return data_->EHCalTowerOverECalD1_[number_];
}

Float_t IOPhoton::EHCalTowerOverECalD2() const
{
	return data_->EHCalTowerOverECalD2_[number_];
}

void IOPhoton::TriggerMatching(UInt_t _TriggerMatching)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(155);//ERROR155
		return;
	}
	data_->TriggerMatching_[number_] = _TriggerMatching;
}

void IOPhoton::Info(UInt_t _Info)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(156);//ERROR156
		return;
	}
	data_->Info_[number_] = _Info;
}

void IOPhoton::px(Float_t _px)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(157);//ERROR157
		return;
	}
	data_->px_[number_] = _px;
}

void IOPhoton::py(Float_t _py)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(158);//ERROR158
		return;
	}
	data_->py_[number_] = _py;
}

void IOPhoton::pz(Float_t _pz)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(159);//ERROR159
		return;
	}
	data_->pz_[number_] = _pz;
}

void IOPhoton::E1x5(Float_t _E1x5)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(160);//ERROR160
		return;
	}
	data_->E1x5_[number_] = _E1x5;
}

void IOPhoton::E2x5(Float_t _E2x5)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(161);//ERROR161
		return;
	}
	data_->E2x5_[number_] = _E2x5;
}

void IOPhoton::E3x3(Float_t _E3x3)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(162);//ERROR162
		return;
	}
	data_->E3x3_[number_] = _E3x3;
}

void IOPhoton::E5x5(Float_t _E5x5)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(163);//ERROR163
		return;
	}
	data_->E5x5_[number_] = _E5x5;
}

void IOPhoton::MaxCrystalEnergy(Float_t _MaxCrystalEnergy)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(164);//ERROR164
		return;
	}
	data_->MaxCrystalEnergy_[number_] = _MaxCrystalEnergy;
}

void IOPhoton::SigmaIEtaIEta(Float_t _SigmaIEtaIEta)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(165);//ERROR165
		return;
	}
	data_->SigmaIEtaIEta_[number_] = _SigmaIEtaIEta;
}

void IOPhoton::SigmaIPhiIPhi(Float_t _SigmaIPhiIPhi)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(166);//ERROR166
		return;
	}
	data_->SigmaIPhiIPhi_[number_] = _SigmaIPhiIPhi;
}

void IOPhoton::SigmaIEtaIPhi(Float_t _SigmaIEtaIPhi)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(167);//ERROR167
		return;
	}
	data_->SigmaIEtaIPhi_[number_] = _SigmaIEtaIPhi;
}

void IOPhoton::EHCalTowerOverECalD1(Float_t _EHCalTowerOverECalD1)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(168);//ERROR168
		return;
	}
	data_->EHCalTowerOverECalD1_[number_] = _EHCalTowerOverECalD1;
}

void IOPhoton::EHCalTowerOverECalD2(Float_t _EHCalTowerOverECalD2)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(169);//ERROR169
		return;
	}
	data_->EHCalTowerOverECalD2_[number_] = _EHCalTowerOverECalD2;
}



BaseIO::BaseIO(std::string treename, bool writable) : 
IOMuon_container_(200, "IOMuon"),
PrimaryVertex_container_(200, "PrimaryVertex"),
IOEventInfo_container_(1, "IOEventInfo"),
IOPFJet_container_(200, "IOPFJet"),
GenInfo_container_(1, "GenInfo"),
IOMET_container_(10, "IOMET"),
SelectedGenParticle_container_(10000, "SelectedGenParticle"),
AllGenParticle_container_(10000, "AllGenParticle"),
IOElectron_container_(200, "IOElectron"),
IOTrack_container_(1000, "IOTrack"),
IOBeamSpot_container_(1, "IOBeamSpot"),
IOPhoton_container_(200, "IOPhoton"),
writable_(writable),
errorcount_(0),
tree_(0),
copytree_(0),
treename_(treename)
{
	IOMuon::baseio = this;
	Data_IOMuon::baseio = this;
	PrimaryVertex::baseio = this;
	Data_PrimaryVertex::baseio = this;
	IOEventInfo::baseio = this;
	Data_IOEventInfo::baseio = this;
	IOPFJet::baseio = this;
	Data_IOPFJet::baseio = this;
	IOString::baseio = this;
	Data_IOString::baseio = this;
	GenInfo::baseio = this;
	Data_GenInfo::baseio = this;
	IOMET::baseio = this;
	Data_IOMET::baseio = this;
	IOSuperCluster::baseio = this;
	Data_IOSuperCluster::baseio = this;
	PFIsolation::baseio = this;
	Data_PFIsolation::baseio = this;
	SelectedGenParticle::baseio = this;
	Data_SelectedGenParticle::baseio = this;
	AllGenParticle::baseio = this;
	Data_AllGenParticle::baseio = this;
	IOElectron::baseio = this;
	Data_IOElectron::baseio = this;
	IOTrack::baseio = this;
	Data_IOTrack::baseio = this;
	IOBeamSpot::baseio = this;
	Data_IOBeamSpot::baseio = this;
	IOPhoton::baseio = this;
	Data_IOPhoton::baseio = this;
}

BaseIO::~BaseIO()
{
}

void BaseIO::SetError(Int_t errn)
{
	error_[errorcount_] = errn;
	errorcount_++;
}

UInt_t BaseIO::NumErrors()
{
	return(errorcount_);
}

void BaseIO::SetFile(TFile* file)
{
	if(writable_)
	{
		file->cd();
		tree_ = new TTree(treename_.c_str(), treename_.c_str(), 1);
		tree_->Branch("ERROR_COUNT", &errorcount_, "ERROR_COUNT/i");
		tree_->Branch("ERROR", error_, "ERROR[ERROR_COUNT]/I");
		IOMuon_container_.SetUpWrite(tree_);
		PrimaryVertex_container_.SetUpWrite(tree_);
		IOEventInfo_container_.SetUpWrite(tree_);
		IOPFJet_container_.SetUpWrite(tree_);
		GenInfo_container_.SetUpWrite(tree_);
		IOMET_container_.SetUpWrite(tree_);
		SelectedGenParticle_container_.SetUpWrite(tree_);
		AllGenParticle_container_.SetUpWrite(tree_);
		IOElectron_container_.SetUpWrite(tree_);
		IOTrack_container_.SetUpWrite(tree_);
		IOBeamSpot_container_.SetUpWrite(tree_);
		IOPhoton_container_.SetUpWrite(tree_);
	}
	else
	{
		file->GetObject(treename_.c_str(), tree_);
		IOMuon_container_.SetUpRead(tree_);
		PrimaryVertex_container_.SetUpRead(tree_);
		IOEventInfo_container_.SetUpRead(tree_);
		IOPFJet_container_.SetUpRead(tree_);
		GenInfo_container_.SetUpRead(tree_);
		IOMET_container_.SetUpRead(tree_);
		SelectedGenParticle_container_.SetUpRead(tree_);
		AllGenParticle_container_.SetUpRead(tree_);
		IOElectron_container_.SetUpRead(tree_);
		IOTrack_container_.SetUpRead(tree_);
		IOBeamSpot_container_.SetUpRead(tree_);
		IOPhoton_container_.SetUpRead(tree_);
	}
}

bool BaseIO::IsWritable() const {return writable_;}
void BaseIO::Fill(){
	tree_->Fill();
}
void BaseIO::StartFilling(){
	IOMuon_container_.Fill();
	PrimaryVertex_container_.Fill();
	IOEventInfo_container_.Fill();
	IOPFJet_container_.Fill();
	GenInfo_container_.Fill();
	IOMET_container_.Fill();
	SelectedGenParticle_container_.Fill();
	AllGenParticle_container_.Fill();
	IOElectron_container_.Fill();
	IOTrack_container_.Fill();
	IOBeamSpot_container_.Fill();
	IOPhoton_container_.Fill();
}
UInt_t BaseIO::GetEntries() {return tree_->GetEntries();}
void BaseIO::GetEntry(UInt_t n) {tree_->GetEntry(n);}
UInt_t BaseIO::NumIOMuons()
{
	return IOMuon_container_.count_;
}
IOMuon BaseIO::GetIOMuon(UInt_t n)
{
	return IOMuon(&IOMuon_container_, n);
}
void BaseIO::LoadIOMuon(bool load)
{
	IOMuon_container_.Load(tree_, load);
}

UInt_t BaseIO::NumPrimaryVertexs()
{
	return PrimaryVertex_container_.count_;
}
PrimaryVertex BaseIO::GetPrimaryVertex(UInt_t n)
{
	return PrimaryVertex(&PrimaryVertex_container_, n);
}
void BaseIO::LoadPrimaryVertex(bool load)
{
	PrimaryVertex_container_.Load(tree_, load);
}

UInt_t BaseIO::NumIOEventInfos()
{
	return IOEventInfo_container_.count_;
}
IOEventInfo BaseIO::GetIOEventInfo(UInt_t n)
{
	return IOEventInfo(&IOEventInfo_container_, n);
}
void BaseIO::LoadIOEventInfo(bool load)
{
	IOEventInfo_container_.Load(tree_, load);
}

UInt_t BaseIO::NumIOPFJets()
{
	return IOPFJet_container_.count_;
}
IOPFJet BaseIO::GetIOPFJet(UInt_t n)
{
	return IOPFJet(&IOPFJet_container_, n);
}
void BaseIO::LoadIOPFJet(bool load)
{
	IOPFJet_container_.Load(tree_, load);
}

UInt_t BaseIO::NumGenInfos()
{
	return GenInfo_container_.count_;
}
GenInfo BaseIO::GetGenInfo(UInt_t n)
{
	return GenInfo(&GenInfo_container_, n);
}
void BaseIO::LoadGenInfo(bool load)
{
	GenInfo_container_.Load(tree_, load);
}

UInt_t BaseIO::NumIOMETs()
{
	return IOMET_container_.count_;
}
IOMET BaseIO::GetIOMET(UInt_t n)
{
	return IOMET(&IOMET_container_, n);
}
void BaseIO::LoadIOMET(bool load)
{
	IOMET_container_.Load(tree_, load);
}

UInt_t BaseIO::NumSelectedGenParticles()
{
	return SelectedGenParticle_container_.count_;
}
SelectedGenParticle BaseIO::GetSelectedGenParticle(UInt_t n)
{
	return SelectedGenParticle(&SelectedGenParticle_container_, n);
}
void BaseIO::LoadSelectedGenParticle(bool load)
{
	SelectedGenParticle_container_.Load(tree_, load);
}

UInt_t BaseIO::NumAllGenParticles()
{
	return AllGenParticle_container_.count_;
}
AllGenParticle BaseIO::GetAllGenParticle(UInt_t n)
{
	return AllGenParticle(&AllGenParticle_container_, n);
}
void BaseIO::LoadAllGenParticle(bool load)
{
	AllGenParticle_container_.Load(tree_, load);
}

UInt_t BaseIO::NumIOElectrons()
{
	return IOElectron_container_.count_;
}
IOElectron BaseIO::GetIOElectron(UInt_t n)
{
	return IOElectron(&IOElectron_container_, n);
}
void BaseIO::LoadIOElectron(bool load)
{
	IOElectron_container_.Load(tree_, load);
}

UInt_t BaseIO::NumIOTracks()
{
	return IOTrack_container_.count_;
}
IOTrack BaseIO::GetIOTrack(UInt_t n)
{
	return IOTrack(&IOTrack_container_, n);
}
void BaseIO::LoadIOTrack(bool load)
{
	IOTrack_container_.Load(tree_, load);
}

UInt_t BaseIO::NumIOBeamSpots()
{
	return IOBeamSpot_container_.count_;
}
IOBeamSpot BaseIO::GetIOBeamSpot(UInt_t n)
{
	return IOBeamSpot(&IOBeamSpot_container_, n);
}
void BaseIO::LoadIOBeamSpot(bool load)
{
	IOBeamSpot_container_.Load(tree_, load);
}

UInt_t BaseIO::NumIOPhotons()
{
	return IOPhoton_container_.count_;
}
IOPhoton BaseIO::GetIOPhoton(UInt_t n)
{
	return IOPhoton(&IOPhoton_container_, n);
}
void BaseIO::LoadIOPhoton(bool load)
{
	IOPhoton_container_.Load(tree_, load);
}

}
