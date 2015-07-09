#include <ME.h>

#include <vector>
#include <chrono>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <TVector3.h>
#include <TLorentzVector.h>
#include <TMath.h>

//#include <Analyse.h>


using namespace std;
using namespace TMath;

ME::ME(string _name) : name(_name)
{
	using namespace chrono;
	system_clock::time_point tp = system_clock::now();
	system_clock::duration dtn = tp.time_since_epoch();
	stringstream ssfifoname;
	ssfifoname << "/tmp/" << name;// << "_" << dtn.count();	
	fifoname = ssfifoname.str();
	mkfifo((fifoname+"_OUT").c_str(), 0666);
	mkfifo((fifoname+"_IN").c_str(), 0666);
	pid_t pid = vfork();
	if(pid == 0)
	{
		execlp(("matrix_" + name).c_str(), ("matrix_"+name).c_str() , (char*)0);
	}
	fifo_out.open((fifoname+"_OUT").c_str(), fstream::out);
	fifo_in.open((fifoname+"_IN").c_str(), fstream::in);
}

ME::~ME()
{
	fifo_out << "0.0 0.0 0.0" << endl;
	fifo_out << "0.0 0.0 0.0" << endl;
	fifo_out << "0.0 0.0 0.0" << endl;
	fifo_out << "0.0 0.0 0.0" << endl;
	fifo_out << "0.0 0.0 0.0" << endl;
	fifo_out << "0.0 0.0 0.0" << endl;
	fifo_out << "0.0 0.0 0.0" << endl;
	fifo_out << "0.0 0.0 0.0" << endl;
	fifo_out.close();
	fifo_in.close();
	unlink((fifoname+"_OUT").c_str());
	unlink((fifoname+"_IN").c_str());
}

void ME::Write(const vector<TLorentzVector*>& vecs)
{
	for(TLorentzVector* tv : vecs)
	{
		fifo_out << tv->Px() << " " << tv->Py() << " " << tv->Pz() << endl;
	}
}
double ME::Eval()
{
	double res = -1.;
	double ressum = -1.;
	fifo_in >> res >> ressum;
	return ressum;
}


MECollection::MECollection(const vector<string>& signames, const vector<string>& bkgnames) : Js(4), vecs(8)
{
	for(const string& bkgname : bkgnames)
	{
		mesbkg.push_back(new ME(bkgname));
	}
	for(const string& signame : signames)
	{
		messig.push_back(new ME(signame));
	}
	vresmaxsig.resize(messig.size());
	vresmaxbkg.resize(mesbkg.size());
}

MECollection::~MECollection()
{
	for(size_t i = 0 ; i < messig.size() ; ++i)
	{
		delete messig[i];
	}
	for(size_t i = 0 ; i < mesbkg.size() ; ++i)
	{
		delete mesbkg[i];
	}
}	

double MECollection::Prob()
{
	double prob = 0.;
	for(double val : vresmaxsig)
	{
		prob += Log(val);
	}
	for(double val : vresmaxbkg)
	{
		prob -= Log(val);
	}
	return(prob);
}	

double MECollection::BestCom(const vector<TLorentzVector*>& leps, const vector<TLorentzVector*>& js)
{
	//cout << "COM-Start" << endl;
	L1 = *leps[0];
	L2 = *leps[1];
	Js[0] = *js[0];
	Js[1] = *js[1];
	Js[2] = *js[2];
	Js[3] = *js[3];
	all = (L1 + L2 + Js[0] + Js[1] + Js[2] + Js[3]);
	bvec = -1.*all.BoostVector();
	pin = all.M()/2.;
	I1 = TLorentzVector(0., 0., pin , pin);
	I2 = TLorentzVector(0., 0., -1*pin , pin);
	L1.Boost(bvec);
	L2.Boost(bvec);
	Js[0].Boost(bvec);
	Js[1].Boost(bvec);
	Js[2].Boost(bvec);
	Js[3].Boost(bvec);
	vecs[0] = &I1;
	vecs[1] = &I2;
	vecs[2] = &L1;
	vecs[3] = &L2;
	double res = 0.;
	for(double& w : vresmaxsig){w = 0.;}
	for(double& w : vresmaxbkg){w = 0.;}
	for(int a = 0 ; a < 4 ; ++a)
	{
		for(int b = 0 ; b < 4 ; ++b)
		{
			if(b == a) continue;
			for(int c = 0 ; c < 4 ; ++c)
			{
				if(c == a || c == b) continue;
				for(int d = 0 ; d < 4 ; ++d)
				{
					if(d == a || d == b || d == c) continue;
					vecs[2] = &L1;
					vecs[3] = &L2;
					vecs[4] = &Js[a];
					vecs[5] = &Js[b];
					vecs[6] = &Js[c];
					vecs[7] = &Js[d];
					for(ME* me : messig)
					{
						me->Write(vecs);
					}
					for(ME* me : mesbkg)
					{
						me->Write(vecs);
					}
					for(size_t n = 0 ; n < messig.size() ; ++n)
					{
						res = messig[n]->Eval();
						if(res > vresmaxsig[n])
						{
							vresmaxsig[n] = res;
						}
					}
					for(size_t n = 0 ; n < mesbkg.size() ; ++n)
					{
						res = mesbkg[n]->Eval();
						if(res > vresmaxbkg[n]){vresmaxbkg[n] = res;}
					}
				}
			}
		}
	}

	//cout << "COM-End" << endl;
	return(Prob());
}

double MECollection::MELA(const vector<OMuon*>& leps, const vector<OJet*>& zjs, const vector<OJet*>& vbsjs)
{
	L1 = *leps[0];
	L2 = *leps[1];
	Js[0] = *zjs[0];
	Js[1] = *zjs[1];
	Js[2] = *vbsjs[0];
	Js[3] = *vbsjs[1];
	all = (L1 + L2 + Js[0] + Js[1] + Js[2] + Js[3]);
	bvec = -1.*all.BoostVector();
	pin = all.M()/2.;
	I1 = TLorentzVector(0., 0., pin , pin);
	I2 = TLorentzVector(0., 0., -1*pin , pin);
	L1.Boost(bvec);
	L2.Boost(bvec);
	Js[0].Boost(bvec);
	Js[1].Boost(bvec);
	Js[2].Boost(bvec);
	Js[3].Boost(bvec);
	vecs[0] = &I1;
	vecs[1] = &I2;
	vecs[2] = &L1;
	vecs[3] = &L2;
	vecs[4] = &Js[0];
	vecs[5] = &Js[1];
	vecs[6] = &Js[2];
	vecs[7] = &Js[3];
	for(ME* me : messig)
	{
		me->Write(vecs);
	}
	double res = 0.;
	for(size_t n = 0 ; n < messig.size() ; ++n)
	{
		res += messig[n]->Eval();
	}
	return Log(res);
}

double MECollection::BestCom(const vector<TLorentzVector*>& leps, const vector<OJet*>& js, vector<OJet*>* zjs, vector<OJet*>* vjs)
{
	//cout << "COM-Start" << endl;
	double resmax = 0.;
	int resv[4];
	for(int i = 0 ; i < 1 ; ++i)
	{
		L1 = *leps[0];
		L2 = *leps[1];
		Js[0] = *js[0];
		Js[1] = *js[1];
		Js[2] = *js[2];
		Js[3] = *js[3];
		//L1 *= rand.Gaus(1., 0.04);
		//L2 *= rand.Gaus(1., 0.04);
		//Js[0] *= rand.Gaus(1., 0.2);
		//Js[1] *= rand.Gaus(1., 0.2);
		//Js[2] *= rand.Gaus(1., 0.2);
		//Js[3] *= rand.Gaus(1., 0.2);
		all = (L1 + L2 + Js[0] + Js[1] + Js[2] + Js[3]);
		bvec = -1.*all.BoostVector();
		pin = all.M()/2.;
		I1 = TLorentzVector(0., 0., pin , pin);
		I2 = TLorentzVector(0., 0., -1*pin , pin);
		L1.Boost(bvec);
		L2.Boost(bvec);
		Js[0].Boost(bvec);
		Js[1].Boost(bvec);
		Js[2].Boost(bvec);
		Js[3].Boost(bvec);
		vecs[0] = &I1;
		vecs[1] = &I2;
		vecs[2] = &L1;
		vecs[3] = &L2;
		for(int a = 0 ; a < 4 ; ++a)
		{
			for(int b = 0 ; b < 4 ; ++b)
			{
				if(b == a) continue;
				for(int c = 0 ; c < 4 ; ++c)
				{
					if(c == a || c == b) continue;
					for(int d = 0 ; d < 4 ; ++d)
					{
						if(d == a || d == b || d == c) continue;
						vecs[4] = &Js[a];
						vecs[5] = &Js[b];
						vecs[6] = &Js[c];
						vecs[7] = &Js[d];
						for(ME* me : messig)
						{
							me->Write(vecs);
						}
						for(size_t n = 0 ; n < messig.size() ; ++n)
						{
							double res = messig[n]->Eval();
							if(res > resmax)
							{
								resmax = res;
								resv[0] = a;
								resv[1] = b;
								resv[2] = c;
								resv[3] = d;
							}
						}
					}
				}
			}
		}
	}

	zjs->push_back(js[resv[0]]);
	zjs->push_back(js[resv[1]]);
	vjs->push_back(js[resv[2]]);
	vjs->push_back(js[resv[3]]);
	return(resmax);
}

//MELAINFO /READER--------------------------------------------------------------------------------------------------

MELAInfo::MELAInfo()
{
	cout << "MELAInfo is not initializes correctly. Missing information?" << endl;
}

MELAInfo::MELAInfo(int* Comb, float* Meladata, int numsig, int numbkg) : comb(4), sig(numsig), bkg(numbkg)
{
	for(int i = 0 ; i < 4 ; ++i)
	{
		comb[i] = Comb[i]-1;
	}
	for(size_t i = 0 ; i < sig.size() ; ++i)
	{
		sig[i] = Meladata[i];
	}
	for(size_t i = 0 ; i < bkg.size() ; ++i)
	{
		bkg[i] = Meladata[i+sig.size()];
	}
	sort(sig.begin(), sig.end());
	sort(bkg.begin(), bkg.end());
}
size_t MELAInfo::NumJet(size_t n) const {return(comb[n]);}
float MELAInfo::MaxBKG() const {return(bkg.back());}  
float MELAInfo::MaxSIG() const {return(sig.back());}
float MELAInfo::Prob() const {return(sig.back()-bkg.back());}  

MELAReader::MELAReader(string filename)
{
	TFile* outfile = TFile::Open(filename.c_str(), "read");
	if(outfile != 0)
	{
		TTree* tr = 0;
		outfile->GetObject("mela", tr);
		Int_t info[3];
		tr->SetBranchAddress("info", info);
		Int_t comb[4];
		tr->SetBranchAddress("comb", comb);
		Float_t meladata[8];
		tr->SetBranchAddress("mela", meladata);

		for(int i = 0 ; i < tr->GetEntries() ; ++i)
		{
			tr->GetEntry(i);
			//cout << info[0] << " " << info[1] << " " << info[2] << endl;
			//cout << comb[0] << " " << comb[1] << " " << comb[2] << " " << comb[3] << endl;
			//MELAInfo newinfo(comb, meladata, 1 , 7);
			//data[info[0]][info[1]][info[2]] = newinfo;
			data[info[0]][info[1]].insert(pair<int, MELAInfo>(info[2], MELAInfo(comb, meladata, 1 , 7)));
		}
	}
	else
	{
		cout << "ERROR MELAReader::MELAReader: file could not be opened: " << filename << endl;
	}
}

size_t MELAReader::NumJet(size_t n)
{
//cout << "INFO: " << GLAN->Run() << " " << GLAN->LumiBlock() << " " << GLAN->Number() << endl;
//cout << data[GLAN->Run()][GLAN->LumiBlock()][int(GLAN->Number())].NumJet(n) << endl;
//cout << "HERE" << endl;
return data[GLAN->Run()][GLAN->LumiBlock()][int(GLAN->Number())].NumJet(n);
}
float MELAReader::MaxSIG() {return data[GLAN->Run()][GLAN->LumiBlock()][int(GLAN->Number())].MaxSIG();}
float MELAReader::MaxBKG() {return data[GLAN->Run()][GLAN->LumiBlock()][int(GLAN->Number())].MaxBKG();}
float MELAReader::Prob() {return data[GLAN->Run()][GLAN->LumiBlock()][int(GLAN->Number())].Prob();}


