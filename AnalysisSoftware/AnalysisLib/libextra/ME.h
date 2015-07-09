#ifndef MEH
#define MEH
#include <string>
#include <map>
#include <vector>
#include <fstream>

#include <OJet.h>

using namespace std;

class ME
{
	private:
		string name;
		string fifoname;
		fstream fifo_out;
		fstream fifo_in;
	public:
		ME(string _name);
		~ME();
		void Write(const vector<TLorentzVector*>& vecs);
		double Eval();
};

class MECollection
{
	private:
		vector<ME*> messig;
		vector<ME*> mesbkg;

		TLorentzVector L1;
		TLorentzVector L2;
		vector<TLorentzVector> Js;
		TLorentzVector all;
		TVector3 bvec;
		double pin;
		TLorentzVector I1;
		TLorentzVector I2;
		vector<TLorentzVector*> vecs;
		vector<double> vresmaxsig;
		vector<double> vresmaxbkg;
	public:
		MECollection(const vector<string>& signames, const vector<string>& bkgnames);
		~MECollection();
		double Prob();
		double BestCom(const vector<TLorentzVector*>& leps, const vector<OJet*>& js, vector<OJet*>* zjs = 0, vector<OJet*>* vjs = 0);
		double MELA(const vector<OMuon*>& leps, const vector<OJet*>& zjs, const vector<OJet*>& vbsjs);
		double BestCom(const vector<TLorentzVector*>& leps, const vector<TLorentzVector*>& js);
};



class MELAInfo
{
	private:
		vector<int> comb;
		vector<double> sig;
		vector<double> bkg;
	public:
		MELAInfo(int* Comb, float* Meladata, int numsig, int numbkg);
		MELAInfo();
		size_t NumJet(size_t n) const;
		float MaxBKG() const;
		float MaxSIG() const;
		float Prob() const;
};

class MELAReader
{
	private:
		map< int, map< int, map< int, MELAInfo > > > data;
	public:
		MELAReader(string filename);
		size_t NumJet(size_t n);
		float MaxSIG();
		float MaxBKG();
		float Prob();
};

#endif
