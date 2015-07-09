#include "RAMBO.h"

using namespace std;
using namespace TMath;

void RAMBO::MF(double x, double& F, double& dF)
{
	F = 0.;
	dF = 0.;
	for(int i = 0 ; i < nump ; i++)
	{
		double tF = Sqrt(masses[i]*masses[i] + x*x*tvecs[i].E()*tvecs[i].E());
		F += tF;
		dF += x*tvecs[i].E()*tvecs[i].E()/tF;
	}
}

RAMBO::RAMBO(int numparticles) : withpmasses(false),  nump(numparticles), tvecs(numparticles) 
{

}

void RAMBO::SetMasses(const vector<double> pmasses) 
{
	withpmasses = true;
	masses = pmasses;
}

const vector<TLorentzVector>& RAMBO::Get(double ECMS) 
{

	Sum.SetPxPyPzE(0., 0., 0., 0.);
	for(int i = 0 ; i < nump ; i++)
	{
		double ct = 2.*rand.Uniform() -1;
		double phi = 2*Pi()*rand.Uniform();
		double p0 = -1.*Log(rand.Uniform()*rand.Uniform());
		tvecs[i].SetPxPyPzE(p0*Sqrt(1.-ct*ct)*Cos(phi), p0*Sqrt(1.-ct*ct)*Sin(phi), p0*ct, p0);
		Sum += tvecs[i];
	}

	for(int i = 0 ; i < nump ; i++)
	{
		tvecs[i] *= ECMS/Sum.M();
		tvecs[i].Boost(-1.*Sum.BoostVector());
	}

	if(withpmasses)
	{
		double x = 1.;
		while(true)
		{
			//		cout << x << ", ";
			double xold = x;
			double F;
			double dF;
			MF(x, F, dF);
			x = x - (F-ECMS)/dF;
			if(Abs(xold-x) < 0.000001)
			{
				break;
			}
		}
		//	cout << x << endl;

		for(int i = 0 ; i < nump ; i++)
		{
			double p0 = Sqrt(masses[i]*masses[i] + x*x*tvecs[i].E()*tvecs[i].E());
			tvecs[i] *= x;
			tvecs[i].SetE(p0);
		}
	}

	return tvecs;
}

