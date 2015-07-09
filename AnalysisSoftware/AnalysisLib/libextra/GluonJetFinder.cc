#include "GluonJetFinder.h"
#include <TMath.h>
#include <TFile.h>
#include <TTree.h>

#include <helper.h>
#include <Analyse.h>

#include <iostream>
#include <fstream>

using namespace std;
using namespace TMath;

int GluonJetFinder::getptbin(const JetInfo& A)
{
	return lower_bound(ptbinning.begin(), ptbinning.end(), A.pt) - ptbinning.begin();
}

int GluonJetFinder::getetabin(const JetInfo& A)
{
	return lower_bound(etabinning.begin(), etabinning.end(), A.eta) - etabinning.begin();
}

int GluonJetFinder::getrhobin(const JetInfo& A)
{
	return lower_bound(rhobinning.begin(), rhobinning.end(), A.rho) - rhobinning.begin();
}

int GluonJetFinder::getmultibin(const JetInfo& A)
{
	return lower_bound(multibinning.begin(), multibinning.end(), A.multi) - multibinning.begin();
}

int GluonJetFinder::getareabin(const JetInfo& A)
{
	return lower_bound(areabinning.begin(), areabinning.end(), A.area) - areabinning.begin();
}


float GluonJetFinder::getptbinlow(float val)
{
	return *(--lower_bound(ptbinning.begin(), ptbinning.end(), val));
}

float GluonJetFinder::getptbinhigh(float val)
{
	return *(upper_bound(ptbinning.begin(), ptbinning.end(), val));
}

float GluonJetFinder::getetabinlow(float val)
{
	return *(--lower_bound(etabinning.begin(), etabinning.end(), val));
}

float GluonJetFinder::getetabinhigh(float val)
{
	return *(upper_bound(etabinning.begin(), etabinning.end(), val));
}

float GluonJetFinder::getrhobinlow(float val)
{
	return *(--lower_bound(rhobinning.begin(), rhobinning.end(), val));
}

float GluonJetFinder::getrhobinhigh(float val)
{
	return *(upper_bound(rhobinning.begin(), rhobinning.end(), val));
}

float GluonJetFinder::getmultibinlow(float val)
{
	return *(lower_bound(multibinning.begin(), multibinning.end(), val));
}

float GluonJetFinder::getmultibinhigh(float val)
{
	return *(upper_bound(multibinning.begin(), multibinning.end(), val));
}

float GluonJetFinder::getareabinlow(float val)
{
	return *(--lower_bound(areabinning.begin(), areabinning.end(), val));
}

float GluonJetFinder::getareabinhigh(float val)
{
	return *(upper_bound(areabinning.begin(), areabinning.end(), val));
}


void GluonJetFinder::mysort()
{
	sort(jetlist.begin(), jetlist.end(), [](const JetInfo& A, const JetInfo& B){return A.pt < B.pt;});

	for(size_t ipt = 0 ; ipt < ptbinning.size()-1 ; ++ipt)
	{
		vector<JetInfo>::iterator lowpt = lower_bound(jetlist.begin(), jetlist.end(), ptbinning[ipt], [](const JetInfo& A, const float val){return(A.pt < val);});
		vector<JetInfo>::iterator highpt = lower_bound(jetlist.begin(), jetlist.end(), ptbinning[ipt+1], [](const JetInfo& A, const float val){return(A.pt < val);});
		int lowpospt = lowpt - jetlist.begin();
		int highpospt = highpt - jetlist.begin();
		cout << lowpt-jetlist.begin() << " Apt " << highpt - jetlist.begin() << " : " << ptbinning[ipt] << " " << ptbinning[ipt+1] << endl;
		sort(lowpt, highpt, [](const JetInfo& A, const JetInfo& B){return A.eta < B.eta;});

		for(size_t ieta = 0 ; ieta < etabinning.size()-1 ; ++ieta)
		{
			vector<JetInfo>::iterator loweta = lower_bound(jetlist.begin() + lowpospt, jetlist.begin() + highpospt, etabinning[ieta], [](const JetInfo& A, const float val){return(A.eta < val);});
			vector<JetInfo>::iterator higheta = lower_bound(jetlist.begin() + lowpospt, jetlist.begin() + highpospt, etabinning[ieta+1], [](const JetInfo& A, const float val){return(A.eta < val);});
			int lowposeta = loweta - jetlist.begin();
			int highposeta = higheta - jetlist.begin();
			cout << loweta-jetlist.begin() << " Aeta " << higheta - jetlist.begin() << " : " << etabinning[ieta] << " " << etabinning[ieta+1] << endl;
			sort(loweta, higheta, [](const JetInfo& A, const JetInfo& B){return A.rho < B.rho;});

			for(size_t irho = 0 ; irho < rhobinning.size()-1 ; ++irho)
			{
				vector<JetInfo>::iterator lowrho = lower_bound(jetlist.begin() + lowposeta, jetlist.begin() + highposeta, rhobinning[irho], [](const JetInfo& A, const float val){return(A.rho < val);});
				vector<JetInfo>::iterator highrho = lower_bound(jetlist.begin() + lowposeta, jetlist.begin() + highposeta, rhobinning[irho+1], [](const JetInfo& A, const float val){return(A.rho < val);});
				int lowposrho = lowrho - jetlist.begin();
				int highposrho = highrho - jetlist.begin();
				//cout << lowrho-jetlist.begin() << " Arho " << highrho - jetlist.begin() << " : " << rhobinning[irho] << " " << rhobinning[irho+1] << endl;
				sort(lowrho, highrho, [](const JetInfo& A, const JetInfo& B){return A.multi < B.multi;});


				for(size_t imulti = 0 ; imulti < multibinning.size()-1 ; ++imulti)
				{
					vector<JetInfo>::iterator lowmulti = lower_bound(jetlist.begin() + lowposrho, jetlist.begin() + highposrho, multibinning[imulti], [](const JetInfo& A, const float val){return(A.multi < val);});
					vector<JetInfo>::iterator highmulti = lower_bound(jetlist.begin() + lowposrho, jetlist.begin() + highposrho, multibinning[imulti+1], [](const JetInfo& A, const float val){return(A.multi < val);});
					//int lowposmulti = lowmulti - jetlist.begin();
					//int highposmulti = highmulti - jetlist.begin();
					//cout << lowmulti-jetlist.begin() << " Amulti " << highmulti - jetlist.begin() << " : " << multibinning[imulti] << " " << multibinning[imulti+1] << endl;
					sort(lowmulti, highmulti, [](const JetInfo& A, const JetInfo& B){return A.area < B.area;});

				}
			}
		}
	}
}

bool GluonJetFinder::operator()(const JetInfo& A, const JetInfo& B)
{
	int bina = getptbin(A);
	int binb = getptbin(B);
//	cout << bina << " " << A.pt << " " << binb << " " << B.pt << endl;
	if(bina < binb) return true;	
	if(bina == binb)
	{
		bina = getetabin(A);
		binb = getetabin(B);
		if(bina < binb) return true;	
		if(bina == binb)
		{
			bina = getrhobin(A);
			binb = getrhobin(B);
			if(bina < binb) return true;	
			if(bina == binb)
			{
				bina = getmultibin(A);
				binb = getmultibin(B);
				if(bina < binb) return true;	
				if(bina == binb)
				{
					bina = getareabin(A);
					binb = getareabin(B);
					if(bina < binb) return true;	
				}
			}
		}
	}
	return false;
//	int bina = A.bpt;
//	int binb = B.bpt;
//	if(bina < binb) return true;	
//	if(bina == binb)
//	{
//		bina = A.beta;
//		binb = B.beta;
//		if(bina < binb) return true;	
//		if(bina == binb)
//		{
//			bina = A.brho;
//			binb = B.brho;
//			if(bina < binb) return true;	
//			if(bina == binb)
//			{
//				bina = A.bmulti;
//				binb = B.bmulti;
//				if(bina < binb) return true;	
//				if(bina == binb)
//				{
//					bina = A.barea;
//					binb = B.barea;
//					if(bina < binb) return true;	
//				}
//			}
//		}
//	}
//	return false;
}

GluonJetFinder::GluonJetFinder(string filename, string treename)
{
	for(int i = 0 ; i < 60 ; ++i)
	{
		multibinning.push_back(i);
		areabinning.push_back(i*0.002);
	}

	TFile* file = TFile::Open(filename.c_str(), "read");
	TTree* tree = dynamic_cast<TTree*>(file->Get(treename.c_str()));
	Float_t pt;
	Float_t eta;
	Float_t rho;
	Float_t multi;
	Float_t area;
	tree->SetBranchAddress("pt", &pt);
	tree->SetBranchAddress("eta", &eta);
	tree->SetBranchAddress("rho", &rho);
	tree->SetBranchAddress("multi", &multi);
	tree->SetBranchAddress("area", &area);
	cout  << tree->GetEntries() << endl;
	for(UInt_t e = 0 ; e < tree->GetEntries() ; ++e)
	//for(UInt_t e = 0 ; e < 60000 ; ++e)
	{
		tree->GetEntry(e);
		//jetlist.push_back(JetInfo{pt, eta, rho, multi, area, getptbin(pt), getetabin(eta), getrhobin(rho), getmultibin(multi), getareabin(area)});
		jetlist.push_back(JetInfo{pt, eta, rho, multi, area});
	}

	file->Close();
	if(treename != "JetInfo")
	{
		//sort(jetlist.begin(), jetlist.end(), *this);
		mysort();
		fstream outfiletxt((FNfilename(treename) + ".txt").c_str(), fstream::out);
		TFile* outfile = TFile::Open((FNfilename(treename) + ".root").c_str(), "recreate");
		TTree* treeout = new TTree("JetInfo", "JetInfo", 1);
		treeout->Branch("pt", &pt, "pt/F");
		treeout->Branch("eta", &eta, "eta/F");
		treeout->Branch("rho", &rho, "rho/F");
		treeout->Branch("multi", &multi, "multi/F");
		treeout->Branch("area", &area, "area/F");
		for(size_t n = 0 ; n < jetlist.size() ; ++n)
		{
			outfiletxt << n << " " << jetlist[n].pt << " " << jetlist[n].eta << " " << jetlist[n].rho << " " << jetlist[n].multi << " " << jetlist[n].area << endl;
			pt = jetlist[n].pt;
			eta = jetlist[n].eta;
			rho = jetlist[n].rho;
			multi = jetlist[n].multi;
			area = jetlist[n].area;
			treeout->Fill();
		}
		outfile->Write();
		outfile->Close();
		outfiletxt.close();
	}
}

double GluonJetFinder::get(float pt, float eta, float rho, float multi, float area)
{
	vector<JetInfo>::const_iterator low = lower_bound(jetlist.begin(), jetlist.end(), getptbinlow(pt), [](const JetInfo& A, const float val){return(A.pt < val);});
	vector<JetInfo>::const_iterator high = lower_bound(jetlist.begin(), jetlist.end(), getptbinhigh(pt), [](const JetInfo& A, const float val){return(A.pt < val);});
	//cout << pt << " " << getptbinlow(pt) << "-" << getptbinhigh(pt) << ": " << high - low << endl;

	low = lower_bound(low, high, getetabinlow(eta), [](const JetInfo& A, const float val){return(A.eta < val);});
	high = lower_bound(low, high, getetabinhigh(eta), [](const JetInfo& A, const float val){return(A.eta < val);});
	//cout << eta << " " << getetabinlow(eta) << "-" << getetabinhigh(eta) << ": " << low-jetlist.begin() << " " << high - low << endl;

	low = lower_bound(low, high, getrhobinlow(rho), [](const JetInfo& A, const float val){return(A.rho < val);});
	high = lower_bound(low, high, getrhobinhigh(rho), [](const JetInfo& A, const float val){return(A.rho < val);});
	//cout << rho << " " << getrhobinlow(rho) << "-" << getrhobinhigh(rho) << ": " << low-jetlist.begin() << " "<< high - low << endl;
	double norm = high-low;

	low = lower_bound(low, high, getmultibinlow(multi), [](const JetInfo& A, const float val){return(A.multi < val);});
	high = lower_bound(low, high, getmultibinhigh(multi), [](const JetInfo& A, const float val){return(A.multi < val);});
	//cout << multi << " " << getmultibinlow(multi) << "-" << getmultibinhigh(multi) << ": " << low-jetlist.begin() << " "<< high - low << endl;

	low = lower_bound(low, high, getareabinlow(area), [](const JetInfo& A, const float val){return(A.area < val);});
	high = lower_bound(low, high, getareabinhigh(area), [](const JetInfo& A, const float val){return(A.area < val);});
	//cout << area << " " << getareabinlow(area) << "-" << getareabinhigh(area) << ": " << low-jetlist.begin() << " "<< high - low << endl;

//	cout << high-low << " " << norm << " " << (high-low)/norm << endl;
//	if(high-low < 10){return(-1.);}
	return Max(double(high-low), 0.01)/norm;
}

GluonFinderTMVA::GluonFinderTMVA(string _prefix) : prefix(_prefix)
{
}

void GluonFinderTMVA::Init()
{
	TDirectory* cdir = gDirectory;
	TDirectory* ndir = cdir->mkdir(prefix.c_str());
	ndir->cd();
	cout << prefix << endl;
	for(size_t irho = 0 ; irho < rhobinning.size() - 1 ; ++irho)
	{
		for(size_t ieta = 0 ; ieta < etabinning.size() - 1 ; ++ieta)
		{
			for(size_t ipt = 0 ; ipt < ptbinning.size() - 1 ; ++ipt)
			{
				stringstream trname;
				trname << "tree_" << ptbinning[ipt] << "-" << ptbinning[ipt+1] << "_" << etabinning[ieta] << "-" << etabinning[ieta+1] << "_" << rhobinning[irho] << "-" << rhobinning[irho+1];
				cout << trname.str() << endl;
				trees.push_back(new TTree(trname.str().c_str(), trname.str().c_str(), 1));
				trees.back()->Branch("multi", &multi, "multi/F");
				trees.back()->Branch("area", &area, "area/F");
				//trees.back()->Branch("arearatio", &arearatio, "arearatio/F");
				trees.back()->Branch("maxpt", &maxpt, "maxpt/F");
				//trees.back()->Branch("btag", &btag, "btag/F");
				//trees.back()->Branch("rho", &rho, "rho/F");
				trees.back()->Branch("weight", &weight, "weight/F");
				
				if(prefix != "GLUON") continue;
				readers.push_back(new TMVA::Reader("!Color:Silent"));
				readers.back()->AddVariable("multi", &multi);
				//readers.back()->AddVariable("henergy", &henergy);
				readers.back()->AddVariable("area", &area);
				//readers.back()->AddVariable("arearatio", &arearatio);
				readers.back()->AddVariable("maxpt", &maxpt);
				//readers.back()->AddVariable("btag", &btag);
				readers.back()->BookMVA("METHOD", ("/home/otto/weight_GluonFinder/weights_GluonFinder_" + trname.str() + "/TMVAClassification_BDTG.weights.xml").c_str());
			}

		}
	}
	cdir->cd();
}

size_t GluonFinderTMVA::getptbin(float pt)
{
	return lower_bound(ptbinning.begin(), ptbinning.end(), pt) - ptbinning.begin() - 1;
}
size_t GluonFinderTMVA::getetabin(float eta)
{
	return lower_bound(etabinning.begin(), etabinning.end(), eta) - etabinning.begin() - 1;
}
size_t GluonFinderTMVA::getrhobin(float rho)
{
	return lower_bound(rhobinning.begin(), rhobinning.end(), rho) - rhobinning.begin() - 1;
}
size_t GluonFinderTMVA::GetTree(float pt, float eta, float rho)
{
	size_t ptbin = getptbin(pt);
	size_t etabin = getetabin(eta);
	size_t rhobin = getrhobin(rho);

	//cout << ptbin << " pt " << pt << endl;
	//cout << etabin << " eta " << eta << endl;
	//cout << rhobin << " rho " << rho << endl;
	//cout << ptbin + (ptbinning.size()-1)*etabin + (ptbinning.size()-1)*(etabinning.size()-1)*rhobin << endl;
	return ptbin + (ptbinning.size()-1)*etabin + (ptbinning.size()-1)*(etabinning.size()-1)*rhobin;
}

void GluonFinderTMVA::SetVar(OJet* jet, float Weight)
{
	weight = Weight;
	multi = jet->ChargedMulti() + jet->NeutralMulti();
	area = Sqrt(Power(jet->PrincipalAxisA(), 2) + Power(jet->PrincipalAxisB(), 2));
	maxpt = jet->AllMaxPt()/jet->Pt();
	rho = GLAN->AK5PFRho();
	//if(Abs(jet->Eta()) < 1.5)
	//{
	//	multi = (multi - (21.6 + 1.22*rho))/(9.1 + 0.077*rho);
	//	area = (area - (0.017 + 8.29E-4*rho))/(0.012 + 2.31E-4*rho);
	//}
	//else if(Abs(jet->Eta()) < 2.5)
	//{
	//	multi = (multi - (19.1 + 0.0978*rho))/(8.1 + 0.15*rho);
	//	area = (area - (0.018 + 7.55E-4*rho))/(0.012 + 1.94E-4*rho);
	//}
	//else
	//{
	//	multi = (multi - (13.1 + 0.307*rho))/(4.68);
	//	area = (area - (0.019 + 6.87E-4*rho))/(0.012 + 1.47E-4*rho);
	//}
	//btag = jet->combinedSecondaryVertexMVABJetTags();
}

void GluonFinderTMVA::Fill(OJet* jet, float Weight)
{
	SetVar(jet, Weight);
	trees[GetTree(jet->Pt(), Abs(jet->Eta()), GLAN->AK5PFRho())]->Fill();	
}

double GluonFinderTMVA::Get(OJet* jet)
{
	SetVar(jet, 1.);
	return readers[GetTree(jet->Pt(), Abs(jet->Eta()), GLAN->AK5PFRho())]->EvaluateMVA("METHOD");	
}



