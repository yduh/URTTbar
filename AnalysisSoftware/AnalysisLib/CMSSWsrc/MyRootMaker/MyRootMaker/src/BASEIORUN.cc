#include "../interface/BASEIORUN.h"

namespace BASEIORUN{
//Data_IORunInfo Data_IORunInfo Data_IORunInfo Data_IORunInfo Data_IORunInfo Data_IORunInfo Data_IORunInfo Data_IORunInfo Data_IORunInfo Data_IORunInfo 
BaseIO* Data_IORunInfo::baseio = 0;
Data_IORunInfo::Data_IORunInfo(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
, HLTNames_(new Data_IOString(size_, prefix_ + "_HLTNames"))
, HLTNamesMuMatched_(new Data_IOString(size_, prefix_ + "_HLTNamesMuMatched"))
, HLTNamesElMatched_(new Data_IOString(size_, prefix_ + "_HLTNamesElMatched"))
, HLTNamesTauMatched_(new Data_IOString(size_, prefix_ + "_HLTNamesTauMatched"))
, HLTNamesPhMatched_(new Data_IOString(size_, prefix_ + "_HLTNamesPhMatched"))
, HLTNamesJetMatched_(new Data_IOString(size_, prefix_ + "_HLTNamesJetMatched"))
{
	RunNumber_ = new UInt_t[size_];
	NumHLTs_ = new Int_t[size_];
	HLTPrescaleColumns_ = new Int_t[size_];
	HLTPrescaleTable_num_ = new UInt_t[size_];
	HLTPrescaleTable_ = new Int_t[size_*10000];
	HLTPrescaleTable_countmax_ = size_*10000;
	HLTSeedAlgoPrescaleTable_num_ = new UInt_t[size_];
	HLTSeedAlgoPrescaleTable_ = new Int_t[size_*10000];
	HLTSeedAlgoPrescaleTable_countmax_ = size_*10000;
	HLTSeedTechPrescaleTable_num_ = new UInt_t[size_];
	HLTSeedTechPrescaleTable_ = new Int_t[size_*10000];
	HLTSeedTechPrescaleTable_countmax_ = size_*10000;
}

Data_IORunInfo::~Data_IORunInfo()
{
	delete[] RunNumber_;
	delete[] NumHLTs_;
	delete[] HLTPrescaleColumns_;
	delete HLTNames_;
	delete HLTNamesMuMatched_;
	delete HLTNamesElMatched_;
	delete HLTNamesTauMatched_;
	delete HLTNamesPhMatched_;
	delete HLTNamesJetMatched_;
	delete[] HLTPrescaleTable_;
	delete[] HLTPrescaleTable_num_;
	delete[] HLTSeedAlgoPrescaleTable_;
	delete[] HLTSeedAlgoPrescaleTable_num_;
	delete[] HLTSeedTechPrescaleTable_;
	delete[] HLTSeedTechPrescaleTable_num_;
}

void Data_IORunInfo::Fill()
{
	count_ = 0;
	HLTNames_->Fill();
	HLTNamesMuMatched_->Fill();
	HLTNamesElMatched_->Fill();
	HLTNamesTauMatched_->Fill();
	HLTNamesPhMatched_->Fill();
	HLTNamesJetMatched_->Fill();
	HLTPrescaleTable_count_ = 0;
	HLTSeedAlgoPrescaleTable_count_ = 0;
	HLTSeedTechPrescaleTable_count_ = 0;
}

void Data_IORunInfo::SetUpWrite(TTree* tree)
{
	tree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());
	tree->Branch((prefix_ + "_RunNumber").c_str(), RunNumber_, (prefix_ + "_RunNumber[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_NumHLTs").c_str(), NumHLTs_, (prefix_ + "_NumHLTs[" + prefix_ + "_count]/I").c_str());
	tree->Branch((prefix_ + "_HLTPrescaleColumns").c_str(), HLTPrescaleColumns_, (prefix_ + "_HLTPrescaleColumns[" + prefix_ + "_count]/I").c_str());
	HLTNames_->SetUpWrite(tree);
	HLTNamesMuMatched_->SetUpWrite(tree);
	HLTNamesElMatched_->SetUpWrite(tree);
	HLTNamesTauMatched_->SetUpWrite(tree);
	HLTNamesPhMatched_->SetUpWrite(tree);
	HLTNamesJetMatched_->SetUpWrite(tree);
	tree->Branch((prefix_ + "_HLTPrescaleTable_count").c_str(), &HLTPrescaleTable_count_, (prefix_ + "_HLTPrescaleTable_count/i").c_str());
	tree->Branch((prefix_ + "_HLTPrescaleTable_num").c_str(), HLTPrescaleTable_num_, (prefix_ + "_HLTPrescaleTable_num[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_HLTPrescaleTable").c_str(), HLTPrescaleTable_, (prefix_ + "_HLTPrescaleTable[" + prefix_ + "_HLTPrescaleTable_count]/I").c_str());
	tree->Branch((prefix_ + "_HLTSeedAlgoPrescaleTable_count").c_str(), &HLTSeedAlgoPrescaleTable_count_, (prefix_ + "_HLTSeedAlgoPrescaleTable_count/i").c_str());
	tree->Branch((prefix_ + "_HLTSeedAlgoPrescaleTable_num").c_str(), HLTSeedAlgoPrescaleTable_num_, (prefix_ + "_HLTSeedAlgoPrescaleTable_num[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_HLTSeedAlgoPrescaleTable").c_str(), HLTSeedAlgoPrescaleTable_, (prefix_ + "_HLTSeedAlgoPrescaleTable[" + prefix_ + "_HLTSeedAlgoPrescaleTable_count]/I").c_str());
	tree->Branch((prefix_ + "_HLTSeedTechPrescaleTable_count").c_str(), &HLTSeedTechPrescaleTable_count_, (prefix_ + "_HLTSeedTechPrescaleTable_count/i").c_str());
	tree->Branch((prefix_ + "_HLTSeedTechPrescaleTable_num").c_str(), HLTSeedTechPrescaleTable_num_, (prefix_ + "_HLTSeedTechPrescaleTable_num[" + prefix_ + "_count]/i").c_str());
	tree->Branch((prefix_ + "_HLTSeedTechPrescaleTable").c_str(), HLTSeedTechPrescaleTable_, (prefix_ + "_HLTSeedTechPrescaleTable[" + prefix_ + "_HLTSeedTechPrescaleTable_count]/I").c_str());
}

void Data_IORunInfo::SetUpRead(TTree* tree)
{
	tree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);
	tree->SetBranchAddress((prefix_ + "_RunNumber").c_str(), RunNumber_);
	tree->SetBranchAddress((prefix_ + "_NumHLTs").c_str(), NumHLTs_);
	tree->SetBranchAddress((prefix_ + "_HLTPrescaleColumns").c_str(), HLTPrescaleColumns_);
	HLTNames_->SetUpRead(tree);
	HLTNamesMuMatched_->SetUpRead(tree);
	HLTNamesElMatched_->SetUpRead(tree);
	HLTNamesTauMatched_->SetUpRead(tree);
	HLTNamesPhMatched_->SetUpRead(tree);
	HLTNamesJetMatched_->SetUpRead(tree);
	tree->SetBranchAddress((prefix_ + "_HLTPrescaleTable_count").c_str(), &HLTPrescaleTable_count_);
	tree->SetBranchAddress((prefix_ + "_HLTPrescaleTable_num").c_str(), HLTPrescaleTable_num_);
	tree->SetBranchAddress((prefix_ + "_HLTPrescaleTable").c_str(), HLTPrescaleTable_);
	tree->SetBranchAddress((prefix_ + "_HLTSeedAlgoPrescaleTable_count").c_str(), &HLTSeedAlgoPrescaleTable_count_);
	tree->SetBranchAddress((prefix_ + "_HLTSeedAlgoPrescaleTable_num").c_str(), HLTSeedAlgoPrescaleTable_num_);
	tree->SetBranchAddress((prefix_ + "_HLTSeedAlgoPrescaleTable").c_str(), HLTSeedAlgoPrescaleTable_);
	tree->SetBranchAddress((prefix_ + "_HLTSeedTechPrescaleTable_count").c_str(), &HLTSeedTechPrescaleTable_count_);
	tree->SetBranchAddress((prefix_ + "_HLTSeedTechPrescaleTable_num").c_str(), HLTSeedTechPrescaleTable_num_);
	tree->SetBranchAddress((prefix_ + "_HLTSeedTechPrescaleTable").c_str(), HLTSeedTechPrescaleTable_);
}

void Data_IORunInfo::Load(TTree* tree, bool load)
{
	tree->SetBranchStatus((prefix_ + "_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_RunNumber").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_NumHLTs").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_HLTPrescaleColumns").c_str(), load);
	HLTNames_->Load(tree, load);
	HLTNamesMuMatched_->Load(tree, load);
	HLTNamesElMatched_->Load(tree, load);
	HLTNamesTauMatched_->Load(tree, load);
	HLTNamesPhMatched_->Load(tree, load);
	HLTNamesJetMatched_->Load(tree, load);
	tree->SetBranchStatus((prefix_ + "_HLTPrescaleTable_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_HLTPrescaleTable_num").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_HLTPrescaleTable").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_HLTSeedAlgoPrescaleTable_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_HLTSeedAlgoPrescaleTable_num").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_HLTSeedAlgoPrescaleTable").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_HLTSeedTechPrescaleTable_count").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_HLTSeedTechPrescaleTable_num").c_str(), load);
	tree->SetBranchStatus((prefix_ + "_HLTSeedTechPrescaleTable").c_str(), load);
}



//IORunInfo == IORunInfo == IORunInfo == IORunInfo == IORunInfo == IORunInfo == IORunInfo == IORunInfo == IORunInfo == IORunInfo == IORunInfo == IORunInfo == IORunInfo == IORunInfo == IORunInfo == 
BaseIO* IORunInfo::baseio = 0;
IORunInfo::IORunInfo(Data_IORunInfo* data, UInt_t number) : 
number_(number),
data_(data)
{
	Init();
}

IORunInfo::IORunInfo(const IORunInfo& _ioruninfo) : 
number_( _ioruninfo.number_),
data_( _ioruninfo.data_)
{
}

void IORunInfo::Init()
{
	if(baseio->IsWritable())
	{
		if(number_ == 0) {data_->HLTPrescaleTable_num_[number_] = 0;}
		else {data_->HLTPrescaleTable_num_[number_] = data_->HLTPrescaleTable_num_[number_-1];}
		if(number_ == 0) {data_->HLTSeedAlgoPrescaleTable_num_[number_] = 0;}
		else {data_->HLTSeedAlgoPrescaleTable_num_[number_] = data_->HLTSeedAlgoPrescaleTable_num_[number_-1];}
		if(number_ == 0) {data_->HLTSeedTechPrescaleTable_num_[number_] = 0;}
		else {data_->HLTSeedTechPrescaleTable_num_[number_] = data_->HLTSeedTechPrescaleTable_num_[number_-1];}
		data_->count_ = number_+1;
	}
}

UInt_t IORunInfo::RunNumber() const
{
	return data_->RunNumber_[number_];
}

Int_t IORunInfo::NumHLTs() const
{
	return data_->NumHLTs_[number_];
}

Int_t IORunInfo::HLTPrescaleColumns() const
{
	return data_->HLTPrescaleColumns_[number_];
}

IOString IORunInfo::HLTNames() const
{
	return IOString(data_->HLTNames_, number_);
}

IOString IORunInfo::HLTNamesMuMatched() const
{
	return IOString(data_->HLTNamesMuMatched_, number_);
}

IOString IORunInfo::HLTNamesElMatched() const
{
	return IOString(data_->HLTNamesElMatched_, number_);
}

IOString IORunInfo::HLTNamesTauMatched() const
{
	return IOString(data_->HLTNamesTauMatched_, number_);
}

IOString IORunInfo::HLTNamesPhMatched() const
{
	return IOString(data_->HLTNamesPhMatched_, number_);
}

IOString IORunInfo::HLTNamesJetMatched() const
{
	return IOString(data_->HLTNamesJetMatched_, number_);
}

UInt_t IORunInfo::Num_HLTPrescaleTable() const
{
	return number_ == 0 ? data_->HLTPrescaleTable_num_[number_] : data_->HLTPrescaleTable_num_[number_] - data_->HLTPrescaleTable_num_[number_-1];
}

Int_t IORunInfo::HLTPrescaleTable(UInt_t n) const
{
	return number_ == 0 ? data_->HLTPrescaleTable_[n] : data_->HLTPrescaleTable_[data_->HLTPrescaleTable_num_[number_-1]  + n];
}

UInt_t IORunInfo::Num_HLTSeedAlgoPrescaleTable() const
{
	return number_ == 0 ? data_->HLTSeedAlgoPrescaleTable_num_[number_] : data_->HLTSeedAlgoPrescaleTable_num_[number_] - data_->HLTSeedAlgoPrescaleTable_num_[number_-1];
}

Int_t IORunInfo::HLTSeedAlgoPrescaleTable(UInt_t n) const
{
	return number_ == 0 ? data_->HLTSeedAlgoPrescaleTable_[n] : data_->HLTSeedAlgoPrescaleTable_[data_->HLTSeedAlgoPrescaleTable_num_[number_-1]  + n];
}

UInt_t IORunInfo::Num_HLTSeedTechPrescaleTable() const
{
	return number_ == 0 ? data_->HLTSeedTechPrescaleTable_num_[number_] : data_->HLTSeedTechPrescaleTable_num_[number_] - data_->HLTSeedTechPrescaleTable_num_[number_-1];
}

Int_t IORunInfo::HLTSeedTechPrescaleTable(UInt_t n) const
{
	return number_ == 0 ? data_->HLTSeedTechPrescaleTable_[n] : data_->HLTSeedTechPrescaleTable_[data_->HLTSeedTechPrescaleTable_num_[number_-1]  + n];
}

void IORunInfo::RunNumber(UInt_t _RunNumber)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(1);//ERROR1
		return;
	}
	data_->RunNumber_[number_] = _RunNumber;
}

void IORunInfo::NumHLTs(Int_t _NumHLTs)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(2);//ERROR2
		return;
	}
	data_->NumHLTs_[number_] = _NumHLTs;
}

void IORunInfo::HLTPrescaleColumns(Int_t _HLTPrescaleColumns)
{
	if(number_ >= data_->size_)
	{
		baseio->SetError(3);//ERROR3
		return;
	}
	data_->HLTPrescaleColumns_[number_] = _HLTPrescaleColumns;
}

void IORunInfo::HLTPrescaleTable(Int_t _HLTPrescaleTable, UInt_t n)
{
	if(number_ != 0){n = data_->HLTPrescaleTable_num_[number_-1] +n;}
	if(n >= data_->HLTPrescaleTable_countmax_)
	{
		baseio->SetError(4);//ERROR4
		return;
	}
	if(n != data_->HLTPrescaleTable_count_)
	{
		cerr << "Index already filled" << endl;
		return;
	}
	data_->HLTPrescaleTable_[n] = _HLTPrescaleTable;
	data_->HLTPrescaleTable_num_[number_]++;
	data_->HLTPrescaleTable_count_++;
}

void IORunInfo::HLTSeedAlgoPrescaleTable(Int_t _HLTSeedAlgoPrescaleTable, UInt_t n)
{
	if(number_ != 0){n = data_->HLTSeedAlgoPrescaleTable_num_[number_-1] +n;}
	if(n >= data_->HLTSeedAlgoPrescaleTable_countmax_)
	{
		baseio->SetError(5);//ERROR5
		return;
	}
	if(n != data_->HLTSeedAlgoPrescaleTable_count_)
	{
		cerr << "Index already filled" << endl;
		return;
	}
	data_->HLTSeedAlgoPrescaleTable_[n] = _HLTSeedAlgoPrescaleTable;
	data_->HLTSeedAlgoPrescaleTable_num_[number_]++;
	data_->HLTSeedAlgoPrescaleTable_count_++;
}

void IORunInfo::HLTSeedTechPrescaleTable(Int_t _HLTSeedTechPrescaleTable, UInt_t n)
{
	if(number_ != 0){n = data_->HLTSeedTechPrescaleTable_num_[number_-1] +n;}
	if(n >= data_->HLTSeedTechPrescaleTable_countmax_)
	{
		baseio->SetError(6);//ERROR6
		return;
	}
	if(n != data_->HLTSeedTechPrescaleTable_count_)
	{
		cerr << "Index already filled" << endl;
		return;
	}
	data_->HLTSeedTechPrescaleTable_[n] = _HLTSeedTechPrescaleTable;
	data_->HLTSeedTechPrescaleTable_num_[number_]++;
	data_->HLTSeedTechPrescaleTable_count_++;
}



//Data_IOString Data_IOString Data_IOString Data_IOString Data_IOString Data_IOString Data_IOString Data_IOString Data_IOString Data_IOString 
BaseIO* Data_IOString::baseio = 0;
Data_IOString::Data_IOString(UInt_t size, std::string prefix = "") : 
size_(size),
prefix_(prefix)
{
	str_num_ = new UInt_t[size_];
	str_ = new Char_t[size_*100000];
	str_countmax_ = size_*100000;
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
		baseio->SetError(7);//ERROR7
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
IORunInfo_container_(1, "IORunInfo"),
writable_(writable),
errorcount_(0),
tree_(0),
copytree_(0),
treename_(treename)
{
	IORunInfo::baseio = this;
	Data_IORunInfo::baseio = this;
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
		IORunInfo_container_.SetUpWrite(tree_);
	}
	else
	{
		file->GetObject(treename_.c_str(), tree_);
		IORunInfo_container_.SetUpRead(tree_);
	}
}

bool BaseIO::IsWritable() const {return writable_;}
void BaseIO::Fill(){
	tree_->Fill();
}
void BaseIO::StartFilling(){
	IORunInfo_container_.Fill();
}
UInt_t BaseIO::GetEntries() {return tree_->GetEntries();}
void BaseIO::GetEntry(UInt_t n) {tree_->GetEntry(n);}
UInt_t BaseIO::NumIORunInfos()
{
	return IORunInfo_container_.count_;
}
IORunInfo BaseIO::GetIORunInfo(UInt_t n)
{
	return IORunInfo(&IORunInfo_container_, n);
}
void BaseIO::LoadIORunInfo(bool load)
{
	IORunInfo_container_.Load(tree_, load);
}

}
