#include "../interface/BASEIOLUMI.h"

namespace BASEIOLUMI{
//Data_IOLumiInfo Data_IOLumiInfo Data_IOLumiInfo Data_IOLumiInfo Data_IOLumiInfo Data_IOLumiInfo Data_IOLumiInfo Data_IOLumiInfo Data_IOLumiInfo Data_IOLumiInfo 
BaseIO* Data_IOLumiInfo::baseio = 0;
Data_IOLumiInfo::Data_IOLumiInfo(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
, FileNames_(new Data_IOString(size_*1000, prefix_ + "_FileNames"))
{
	RunNumber_ = new UInt_t[size_];
	BlockNumber_ = new UInt_t[size_];
	EventsProcessed_ = new UInt_t[size_];
	EventsFiltered_ = new UInt_t[size_];
	HLTPrescaleColumn_ = new Int_t[size_];
	LumiValue_ = new Float_t[size_];
	LumiValueUnc_ = new Float_t[size_];
	AveragePU_ = new Float_t[size_];
	FileNames_num_ = new UInt_t[size_];
}

Data_IOLumiInfo::~Data_IOLumiInfo()
{
	delete[] RunNumber_;
	delete[] BlockNumber_;
	delete[] EventsProcessed_;
	delete[] EventsFiltered_;
	delete[] HLTPrescaleColumn_;
	delete[] LumiValue_;
	delete[] LumiValueUnc_;
	delete[] AveragePU_;
	delete FileNames_;
	delete[] FileNames_num_;
}

void Data_IOLumiInfo::Fill()
{
	count_ = 0;
	FileNames_->Fill();
}

void Data_IOLumiInfo::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_RunNumber").c_str(), RunNumber_, (prefix_ + "_RunNumber[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_BlockNumber").c_str(), BlockNumber_, (prefix_ + "_BlockNumber[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_EventsProcessed").c_str(), EventsProcessed_, (prefix_ + "_EventsProcessed[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_EventsFiltered").c_str(), EventsFiltered_, (prefix_ + "_EventsFiltered[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_HLTPrescaleColumn").c_str(), HLTPrescaleColumn_, (prefix_ + "_HLTPrescaleColumn[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_LumiValue").c_str(), LumiValue_, (prefix_ + "_LumiValue[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_LumiValueUnc").c_str(), LumiValueUnc_, (prefix_ + "_LumiValueUnc[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_AveragePU").c_str(), AveragePU_, (prefix_ + "_AveragePU[" + prefix_ + "_count]/F").c_str());
	tree->Branch((prefix_ + "_FileNames_num").c_str(), FileNames_num_, (prefix_ + "_FileNames_num[" + prefix_ + "_count]/i").c_str());
	FileNames_->SetUpWrite(tree);
}

void Data_IOLumiInfo::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_RunNumber").c_str(), RunNumber_);
	tree->SetBranchAddress((prefix_ + "_BlockNumber").c_str(), BlockNumber_);
	tree->SetBranchAddress((prefix_ + "_EventsProcessed").c_str(), EventsProcessed_);
	tree->SetBranchAddress((prefix_ + "_EventsFiltered").c_str(), EventsFiltered_);
	tree->SetBranchAddress((prefix_ + "_HLTPrescaleColumn").c_str(), HLTPrescaleColumn_);
	tree->SetBranchAddress((prefix_ + "_LumiValue").c_str(), LumiValue_);
	tree->SetBranchAddress((prefix_ + "_LumiValueUnc").c_str(), LumiValueUnc_);
	tree->SetBranchAddress((prefix_ + "_AveragePU").c_str(), AveragePU_);
	tree->SetBranchAddress((prefix_ + "_FileNames_num").c_str(), FileNames_num_);
	FileNames_->SetUpRead(tree);
}

void Data_IOLumiInfo::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_RunNumber").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_BlockNumber").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_EventsProcessed").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_EventsFiltered").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_HLTPrescaleColumn").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_LumiValue").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_LumiValueUnc").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_AveragePU").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_FileNames_num").c_str(), load);
	FileNames_->Load(tree, load);
}



//IOLumiInfo == IOLumiInfo == IOLumiInfo == IOLumiInfo == IOLumiInfo == IOLumiInfo == IOLumiInfo == IOLumiInfo == IOLumiInfo == IOLumiInfo == IOLumiInfo == IOLumiInfo == IOLumiInfo == IOLumiInfo == IOLumiInfo == 
BaseIO* IOLumiInfo::baseio = 0;
IOLumiInfo::IOLumiInfo(Data_IOLumiInfo* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

IOLumiInfo::IOLumiInfo(const IOLumiInfo& _iolumiinfo) : 
number_( _iolumiinfo.number_),
data_( _iolumiinfo.data_)
{
}

void IOLumiInfo::Init()
{
	if(baseio->IsWritable())
	{
		if(number_ == 0) {data_->FileNames_num_[number_] = 0;}
		else {data_->FileNames_num_[number_] = data_->FileNames_num_[number_-1];}
		data_->count_ = number_+1;
	}
}

UInt_t IOLumiInfo::RunNumber() const
{
	return data_->RunNumber_[number_];
}

UInt_t IOLumiInfo::BlockNumber() const
{
	return data_->BlockNumber_[number_];
}

UInt_t IOLumiInfo::EventsProcessed() const
{
	return data_->EventsProcessed_[number_];
}

UInt_t IOLumiInfo::EventsFiltered() const
{
	return data_->EventsFiltered_[number_];
}

Int_t IOLumiInfo::HLTPrescaleColumn() const
{
	return data_->HLTPrescaleColumn_[number_];
}

Float_t IOLumiInfo::LumiValue() const
{
	return data_->LumiValue_[number_];
}

Float_t IOLumiInfo::LumiValueUnc() const
{
	return data_->LumiValueUnc_[number_];
}

Float_t IOLumiInfo::AveragePU() const
{
	return data_->AveragePU_[number_];
}

UInt_t IOLumiInfo::Num_FileNames() const
{
	return number_ == 0 ? data_->FileNames_num_[number_] : data_->FileNames_num_[number_] - data_->FileNames_num_[number_-1];
}

IOString IOLumiInfo::FileNames(UInt_t n) const
{
	if(baseio->IsWritable())
	{
		data_->FileNames_num_[number_]++;
	}
	return number_ == 0 ? IOString(data_->FileNames_, n) : IOString(data_->FileNames_, data_->FileNames_num_[number_-1]  + n);
}

void IOLumiInfo::RunNumber(UInt_t _RunNumber)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(1);//ERROR1
		return;
	}
	data_->RunNumber_[number_] = _RunNumber;
}

void IOLumiInfo::BlockNumber(UInt_t _BlockNumber)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(2);//ERROR2
		return;
	}
	data_->BlockNumber_[number_] = _BlockNumber;
}

void IOLumiInfo::EventsProcessed(UInt_t _EventsProcessed)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(3);//ERROR3
		return;
	}
	data_->EventsProcessed_[number_] = _EventsProcessed;
}

void IOLumiInfo::EventsFiltered(UInt_t _EventsFiltered)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(4);//ERROR4
		return;
	}
	data_->EventsFiltered_[number_] = _EventsFiltered;
}

void IOLumiInfo::HLTPrescaleColumn(Int_t _HLTPrescaleColumn)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(5);//ERROR5
		return;
	}
	data_->HLTPrescaleColumn_[number_] = _HLTPrescaleColumn;
}

void IOLumiInfo::LumiValue(Float_t _LumiValue)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(6);//ERROR6
		return;
	}
	data_->LumiValue_[number_] = _LumiValue;
}

void IOLumiInfo::LumiValueUnc(Float_t _LumiValueUnc)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(7);//ERROR7
		return;
	}
	data_->LumiValueUnc_[number_] = _LumiValueUnc;
}

void IOLumiInfo::AveragePU(Float_t _AveragePU)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(8);//ERROR8
		return;
	}
	data_->AveragePU_[number_] = _AveragePU;
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
		baseio->SetError(9);//ERROR9
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



BaseIO::BaseIO(std::string treename, bool writable) : 
IOLumiInfo_container_(1, "IOLumiInfo"),
writable_(writable),
errorcount_(0),
tree_(0),
copytree_(0),
treename_(treename)
{
	IOLumiInfo::baseio = this;
	Data_IOLumiInfo::baseio = this;
	IOString::baseio = this;
	Data_IOString::baseio = this;
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
		IOLumiInfo_container_.SetUpWrite(tree_);
	}
	else
	{
		file->GetObject(treename_.c_str(), tree_);
		IOLumiInfo_container_.SetUpRead(tree_);
	}
}

bool BaseIO::IsWritable() const {return writable_;}
void BaseIO::Fill(){
	tree_->Fill();
}
void BaseIO::StartFilling(){
	IOLumiInfo_container_.Fill();
}
UInt_t BaseIO::GetEntries() {return tree_->GetEntries();}
void BaseIO::GetEntry(UInt_t n) {tree_->GetEntry(n);}
UInt_t BaseIO::NumIOLumiInfos()
{
	return IOLumiInfo_container_.count_;
}
IOLumiInfo BaseIO::GetIOLumiInfo(UInt_t n)
{
	return IOLumiInfo(&IOLumiInfo_container_, n);
}
void BaseIO::LoadIOLumiInfo(bool load)
{
	IOLumiInfo_container_.Load(tree_, load);
}

}
