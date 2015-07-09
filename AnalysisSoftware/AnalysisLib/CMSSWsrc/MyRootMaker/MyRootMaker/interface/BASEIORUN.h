#ifndef CLASS_BASEIORUN
#define CLASS_BASEIORUN
#include "TTree.h"
#include "TFile.h"
#include <string>
#include <iostream>
using namespace std;
namespace BASEIORUN{
class BaseIO;
class Data_IORunInfo;
class IORunInfo;
class Data_IOString;
class IOString;

class Data_IORunInfo
 {
	friend class IORunInfo;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		UInt_t* RunNumber_;
		Int_t* NumHLTs_;
		Int_t* HLTPrescaleColumns_;
		Data_IOString* HLTNames_;
		Data_IOString* HLTNamesMuMatched_;
		Data_IOString* HLTNamesElMatched_;
		Data_IOString* HLTNamesTauMatched_;
		Data_IOString* HLTNamesPhMatched_;
		Data_IOString* HLTNamesJetMatched_;
		UInt_t HLTPrescaleTable_count_;
		UInt_t HLTPrescaleTable_countmax_;
		UInt_t* HLTPrescaleTable_num_;
		Int_t* HLTPrescaleTable_;
		UInt_t HLTSeedAlgoPrescaleTable_count_;
		UInt_t HLTSeedAlgoPrescaleTable_countmax_;
		UInt_t* HLTSeedAlgoPrescaleTable_num_;
		Int_t* HLTSeedAlgoPrescaleTable_;
		UInt_t HLTSeedTechPrescaleTable_count_;
		UInt_t HLTSeedTechPrescaleTable_countmax_;
		UInt_t* HLTSeedTechPrescaleTable_num_;
		Int_t* HLTSeedTechPrescaleTable_;
	public:
		void Fill();
		Data_IORunInfo(UInt_t size, std::string prefix);
		~Data_IORunInfo();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class IORunInfo
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_IORunInfo* data_;
	public:
		IORunInfo(Data_IORunInfo* data, UInt_t number);
		IORunInfo(const IORunInfo& _ioruninfo);
		void Init();
		UInt_t RunNumber() const;
		Int_t NumHLTs() const;
		Int_t HLTPrescaleColumns() const;
		IOString HLTNames() const;
		IOString HLTNamesMuMatched() const;
		IOString HLTNamesElMatched() const;
		IOString HLTNamesTauMatched() const;
		IOString HLTNamesPhMatched() const;
		IOString HLTNamesJetMatched() const;
		Int_t HLTPrescaleTable(UInt_t n) const;
		UInt_t Num_HLTPrescaleTable() const;
		Int_t HLTSeedAlgoPrescaleTable(UInt_t n) const;
		UInt_t Num_HLTSeedAlgoPrescaleTable() const;
		Int_t HLTSeedTechPrescaleTable(UInt_t n) const;
		UInt_t Num_HLTSeedTechPrescaleTable() const;
		void RunNumber(UInt_t _RunNumber);
		void NumHLTs(Int_t _NumHLTs);
		void HLTPrescaleColumns(Int_t _HLTPrescaleColumns);
		void HLTPrescaleTable(Int_t _HLTPrescaleTable, UInt_t n);
		void HLTSeedAlgoPrescaleTable(Int_t _HLTSeedAlgoPrescaleTable, UInt_t n);
		void HLTSeedTechPrescaleTable(Int_t _HLTSeedTechPrescaleTable, UInt_t n);
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


class BaseIO
{
	friend class IORunInfo;
	friend class IOString;
	private:
		Data_IORunInfo IORunInfo_container_;
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
		UInt_t NumIORunInfos();
		IORunInfo GetIORunInfo(UInt_t n);
		void LoadIORunInfo(bool load);
};
}
#endif
