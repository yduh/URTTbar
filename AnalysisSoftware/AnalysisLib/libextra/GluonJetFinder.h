#ifndef GLUONJETFINDERH
#define GLUONJETFINDERH
#include <vector>
#include <string>
#include <OJet.h>

#include "TMVA/Reader.h"
#include "TMVA/Config.h"
#include "TMVA/Tools.h"
#include "TMVA/MethodCuts.h"

using namespace std;

struct JetInfo
{
	float pt;
	float eta;
	float rho;
	float multi;
	float area;
//	int bpt;
//	int beta;
//	int brho;
//	int bmulti;
//	int barea;
};

class GluonJetFinder
{
	private:
		vector<JetInfo> jetlist;
		//vector<float> ptbinning = {25, 35, 45, 55, 75, 95, 125, 155, 195, 235, 8000};
		vector<float> ptbinning = {25, 45, 65, 85, 115, 155, 195, 8000};
		vector<float> etabinning = {0., 1.5, 2.5, 6.};
		vector<float> rhobinning = {0., 9., 12., 100.};
		//vector<float> rhobinning = {0., 100.};
		vector<float> multibinning;
		vector<float> areabinning;
		int getptbin(const JetInfo& A);
		int getetabin(const JetInfo& A);
		int getrhobin(const JetInfo& A);
		int getmultibin(const JetInfo& A);
		int getareabin(const JetInfo& A);
		float getptbinlow(float val);
		float getetabinlow(float val);
		float getrhobinlow(float val);
		float getmultibinlow(float val);
		float getareabinlow(float val);
		float getptbinhigh(float val);
		float getetabinhigh(float val);
		float getrhobinhigh(float val);
		float getmultibinhigh(float val);
		float getareabinhigh(float val);
	public:
		GluonJetFinder(string filename, string treename);
		double get(float pt, float eta, float rho, float multi, float area);
		bool operator()(const JetInfo& A, const JetInfo& B);
		void mysort();
};

class GluonFinderTMVA
{
	private:
		string prefix;
		vector<TTree*> trees;
		vector<TMVA::Reader*> readers;

		vector<float> ptbinning = {25, 35, 45, 55, 65, 85, 105, 135, 8000};
		vector<float> etabinning = {0., 1.5, 2.5, 6.};
		vector<float> rhobinning = {0., 9., 12., 100.};

		Float_t multi;
		Float_t area;
		Float_t arearatio;
		Float_t maxpt;
		Float_t btag;
		Float_t rho;
		Float_t weight;
		size_t getptbin(float pt);
		size_t getetabin(float eta);
		size_t getrhobin(float rho);
		size_t GetTree(float pt, float eta, float rho);
		void SetVar(OJet* jet, float Weight);
	public:
		GluonFinderTMVA(string prefix);
		void Init();

		void Fill(OJet* jet, float Weight);
		double Get(OJet* jet);
};

#endif
