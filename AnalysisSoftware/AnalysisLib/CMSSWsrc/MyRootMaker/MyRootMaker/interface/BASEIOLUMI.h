#ifndef CLASS_BASEIOLUMI
#define CLASS_BASEIOLUMI
#include "TTree.h"
#include "TFile.h"
#include <string>
#include <iostream>
using namespace std;
namespace BASEIOLUMI{
class BaseIO;
class Data_IOLumiInfo;
class IOLumiInfo;
class Data_IOString;
class IOString;

class Data_IOLumiInfo
 {
	friend class IOLumiInfo;
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t size_;
		std::string prefix_;
		UInt_t count_;
		UInt_t* RunNumber_;
		UInt_t* BlockNumber_;
		UInt_t* EventsProcessed_;
		UInt_t* EventsFiltered_;
		Int_t* HLTPrescaleColumn_;
		Float_t* LumiValue_;
		Float_t* LumiValueUnc_;
		Float_t* AveragePU_;
		UInt_t* FileNames_num_;
		Data_IOString* FileNames_;
	public:
		void Fill();
		Data_IOLumiInfo(UInt_t size, std::string prefix);
		~Data_IOLumiInfo();
		void SetUpWrite(TTree* tree);
		void SetUpRead(TTree* tree);
		void Load(TTree* tree, bool load);
 };


class IOLumiInfo
 {
	friend class BaseIO;
	private:
		static BaseIO* baseio;
		UInt_t number_;
		Data_IOLumiInfo* data_;
	public:
		IOLumiInfo(Data_IOLumiInfo* data, UInt_t number);
		IOLumiInfo(const IOLumiInfo& _iolumiinfo);
		void Init();
		UInt_t RunNumber() const;
		UInt_t BlockNumber() const;
		UInt_t EventsProcessed() const;
		UInt_t EventsFiltered() const;
		Int_t HLTPrescaleColumn() const;
		Float_t LumiValue() const;
		Float_t LumiValueUnc() const;
		Float_t AveragePU() const;
		IOString FileNames(UInt_t n) const;
		UInt_t Num_FileNames() const;
		void RunNumber(UInt_t _RunNumber);
		void BlockNumber(UInt_t _BlockNumber);
		void EventsProcessed(UInt_t _EventsProcessed);
		void EventsFiltered(UInt_t _EventsFiltered);
		void HLTPrescaleColumn(Int_t _HLTPrescaleColumn);
		void LumiValue(Float_t _LumiValue);
		void LumiValueUnc(Float_t _LumiValueUnc);
		void AveragePU(Float_t _AveragePU);
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
	friend class IOLumiInfo;
	friend class IOString;
	private:
		Data_IOLumiInfo IOLumiInfo_container_;
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
		UInt_t NumIOLumiInfos();
		IOLumiInfo GetIOLumiInfo(UInt_t n);
		void LoadIOLumiInfo(bool load);
};
}
#endif
