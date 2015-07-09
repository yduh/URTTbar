#include "SuperCluster.h"
#include "TMath.h" 
#include "Analyse.h"

SuperCluster::SuperCluster(IOSuperCluster sc) : IOSuperCluster(sc), TLorentzVector(x(), y(), z(), TMath::Sqrt(x()*x() + y()*y() + z()*z())) 
{
	operator*=(IOSuperCluster::Energy()/P());
}
