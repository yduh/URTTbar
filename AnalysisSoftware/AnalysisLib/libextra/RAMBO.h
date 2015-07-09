#ifndef RAMBOH
#define RAMBOH
#include "TRandom3.h"
#include "TLorentzVector.h"
#include "TMath.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace TMath;

class RAMBO
{
	private:
		bool withpmasses;
		int nump;
		vector<TLorentzVector> tvecs;
		vector<double> masses;
		TRandom3 rand;
		TLorentzVector Sum;
		void MF(double x, double& F, double& dF);
	public:
		RAMBO(int numparticles);
		void SetMasses(const vector<double> pmasses);
		const vector<TLorentzVector>& Get(double ECMS);
};

#endif
