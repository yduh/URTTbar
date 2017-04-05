#include "IDJet.h"
#include "ttbarxsec.h"

#include <algorithm>
#include <cmath>
#include <regex>

#include <TF1.h>

using namespace std;

void BTagReader::Init(const string& filename, const string& measurement, const string& type, int jets, int wp)
{
	fstream fs(filename.c_str(), fstream::in);
	string line;
	regex regstart("\\s*\"");
	regex regend("\"\\s*");
	int found = 0;
	while(!fs.eof())
	{
		getline(fs, line);
		if(line.size() == 0 || line[0] == '{') continue;
		vector<string> vals = string_split(line, {",", ";"});
		if(vals.size() != 11) continue;

		if(stringtotype<int>(trim(vals[0])) != wp) continue;
		if(trim(vals[1]) != measurement) continue;
		if(trim(vals[2]) != type) continue;
		if(stringtotype<int>(trim(vals[3])) != jets) continue;
		//cout << line << endl;

		Bin eta(stringtotype<double>(trim(vals[4])), stringtotype<double>(trim(vals[5])));  
		Bin pt(stringtotype<double>(trim(vals[6])), stringtotype<double>(trim(vals[7])));   
		Bin discr(stringtotype<double>(trim(vals[8])), stringtotype<double>(trim(vals[9])));
		stringstream fname;
		fname << measurement << "_" << type << "_" << jets << "_" << wp;
		string formular = regex_replace(vals[10], regstart, "");
		formular = regex_replace(formular, regend, "");
		//cout << formular << endl;
		TF1* func = new TF1(fname.str().c_str(), formular.c_str(), 0., 7000.);
		//cout << func->Eval(40) << endl;
		data[eta][pt][discr] = func;
		found++;
	}
		if(found == 0) cerr << "BTagReader: Configuration not found:" << measurement << "_" << type << "_" << jets << "_" << wp << endl;
}

double BTagReader::Get(const IDJet& jet)
{
	return data[Abs(jet.Eta())][jet.Pt()][jet.csvIncl()]->Eval(jet.Pt());
}



BTagWeight::BTagWeight()
{
}

void BTagWeight::Init(ttbar* an, const string& csvfilename, const string& efffilename, double bunc, double lunc)
{
	btyp = bunc;
	ltyp = lunc;
	AN = an;
	TDirectory* dir = gDirectory;
	probfile = new TFile(efffilename.c_str(), "READ");
	string mcname("P8");
	hborigL = dynamic_cast<TH1D*>(probfile->Get((mcname + "_L_B").c_str()));
	hcorigL = dynamic_cast<TH1D*>(probfile->Get((mcname + "_L_C").c_str()));
	hlorigL = dynamic_cast<TH1D*>(probfile->Get((mcname + "_L_L").c_str()));
	hborigM = dynamic_cast<TH1D*>(probfile->Get((mcname + "_M_B").c_str()));
	hcorigM = dynamic_cast<TH1D*>(probfile->Get((mcname + "_M_C").c_str()));
	hlorigM = dynamic_cast<TH1D*>(probfile->Get((mcname + "_M_L").c_str()));
	if(an->HERWIGPP){mcname = "hpp";}
	if(an->PYTHIA6){mcname = "P6";}
	if(an->ISRUP){mcname = "isrup";}
	if(an->ISRDOWN){mcname = "isrdown";}
	if(an->FSRUP){mcname = "fsrup";}
	if(an->FSRDOWN){mcname = "fsrdown";}
	if(an->TUNEUP){mcname = "tuneup";}
	if(an->TUNEDOWN){mcname = "tunedown";}
	if(an->cbfrag == -1.){mcname = "bfragdown";}
	if(an->cbfrag == 1.){mcname = "bfragup";}
	if(an->cbdecay == -1.){mcname = "bdecaydown";}
	if(an->cbdecay == 1.){mcname = "bdecayup";}
	hbeffL = dynamic_cast<TH1D*>(probfile->Get((mcname + "_L_B").c_str()));
	hceffL = dynamic_cast<TH1D*>(probfile->Get((mcname + "_L_C").c_str()));
	hleffL = dynamic_cast<TH1D*>(probfile->Get((mcname + "_L_L").c_str()));
	hbeffM = dynamic_cast<TH1D*>(probfile->Get((mcname + "_M_B").c_str()));
	hceffM = dynamic_cast<TH1D*>(probfile->Get((mcname + "_M_C").c_str()));
	hleffM = dynamic_cast<TH1D*>(probfile->Get((mcname + "_M_L").c_str()));

	BL.resize(3);
	string mes("comb");
	//string mes("mujets");
	//string mes("TnP");
	BL[0].Init(csvfilename, mes, "central", 0, 0);
	BL[1].Init(csvfilename, mes, "up", 0, 0);
	BL[2].Init(csvfilename, mes, "down", 0, 0);
	
	BM.resize(3);
	BM[0].Init(csvfilename, mes, "central", 0, 1);
	BM[1].Init(csvfilename, mes, "up", 0, 1);
	BM[2].Init(csvfilename, mes, "down", 0, 1);

	mes = "comb";
	CL.resize(3);
	CL[0].Init(csvfilename, mes, "central", 1, 0);
	CL[1].Init(csvfilename, mes, "up", 1, 0);
	CL[2].Init(csvfilename, mes, "down", 1, 0);

	CM.resize(3);
	CM[0].Init(csvfilename, mes, "central", 1, 1);
	CM[1].Init(csvfilename, mes, "up", 1, 1);
	CM[2].Init(csvfilename, mes, "down", 1, 1);

	LL.resize(3);
	LL[0].Init(csvfilename, "incl", "central", 2, 0);
	LL[1].Init(csvfilename, "incl", "up", 2, 0);
	LL[2].Init(csvfilename, "incl", "down", 2, 0);

	LM.resize(3);
	LM[0].Init(csvfilename, "incl", "central", 2, 1);
	LM[1].Init(csvfilename, "incl", "up", 2, 1);
	LM[2].Init(csvfilename, "incl", "down", 2, 1);

	dir->cd();
}

double BTagWeight::scale(vector<BTagReader>& SF, IDJet* jet, double err)
{
	if(err < 0.) return SF[2].Get(*jet);	
	if(err > 0.) return SF[1].Get(*jet);	
	return SF[0].Get(*jet);
}

double BTagWeight::scalebM(IDJet* jet, double err)
{
	return scale(BM, jet, err);
}
double BTagWeight::scalecM(IDJet* jet, double err)
{
	return scale(CM, jet, err);
}
double BTagWeight::scalelM(IDJet* jet, double err)
{
	return scale(LM, jet, err);
}

double BTagWeight::scalebL(IDJet* jet, double err)
{
	return scale(BL, jet, err);
}
double BTagWeight::scalecL(IDJet* jet, double err)
{
	return scale(CL, jet, err);
}
double BTagWeight::scalelL(IDJet* jet, double err)
{
	return scale(LL, jet, err);
}


double BTagWeight::SF(vector<IDJet*>& jets)
{
	double PD=1.;
	double PM=1.;
	for(IDJet* jet : jets)
	{
		if(jet->Pt() >= 670. || jet->Pt() <= 30.) continue;
		if(Abs(jet->Eta()) >= 2.4) continue;
		if(jet->csvIncl() >= 1. || jet->csvIncl() <= 0.) continue;
		//if(jet == bhad || jet == blep) {continue;}
		double pt = jet->Pt();
		int hbin = hbeffM->FindFixBin(pt);
		if(find_if(AN->genbjets.begin(), AN->genbjets.end(), [&](GenObject* bp){return jet->DeltaR(*bp) < 0.3;}) != AN->genbjets.end())
		{
			AN->truth1d["Eff_Ball"]->Fill(pt, AN->weight);
			double effM = hbeffM->GetBinContent(hbin);
			double effMorig = hborigM->GetBinContent(hbin);
			double effL = hbeffL->GetBinContent(hbin);
			double effLorig = hborigL->GetBinContent(hbin);
			if(jet->csvIncl() > AN->B_MEDIUM)
			{
				AN->truth1d["Eff_BpassingM"]->Fill(pt, AN->weight);
				PM *= effM;
				PD *= effMorig * (scalebM(jet, btyp));
			}
			else if(jet->csvIncl() > AN->B_LOOSE)
			{
				AN->truth1d["Eff_BpassingL"]->Fill(pt, AN->weight);
				PM *= effL;
				PD *= (effLorig+effMorig)*scalebL(jet, btyp) - effMorig*scalebM(jet, btyp);
			}
			else
			{
				PM *= 1. - effM - effL;
				PD *= 1. - (effLorig+effMorig)*(scalebL(jet, btyp));
			}
		}
		else if(find_if(AN->gencjets.begin(), AN->gencjets.end(), [&](GenObject* bp){return jet->DeltaR(*bp) < 0.3;}) != AN->gencjets.end())
		{
			AN->truth1d["Eff_Call"]->Fill(pt, AN->weight);
			double effM = hceffM->GetBinContent(hbin);
			double effMorig = hcorigM->GetBinContent(hbin);
			double effL = hceffL->GetBinContent(hbin);
			double effLorig = hcorigL->GetBinContent(hbin);
			if(jet->csvIncl() > AN->B_MEDIUM)
			{
				AN->truth1d["Eff_CpassingM"]->Fill(pt, AN->weight);
				PM *= effM;
				PD *= effMorig * scalecM(jet, btyp);
			}
			else if(jet->csvIncl() > AN->B_LOOSE)
			{
				AN->truth1d["Eff_CpassingL"]->Fill(pt, AN->weight);
				PM *= effL;
				PD *= (effLorig+effMorig)*scalecL(jet, btyp) - effMorig*scalecM(jet, btyp);
			}
			else
			{
				PM *= 1. - effM - effL;
				PD *= 1. - (effMorig+effLorig)*scalecL(jet, btyp);
			}
		}
		else
		{
			AN->truth1d["Eff_Lall"]->Fill(pt, AN->weight);
			double effM = hleffM->GetBinContent(hbin);
			double effMorig = hlorigM->GetBinContent(hbin);
			double effL = hleffL->GetBinContent(hbin);
			double effLorig = hlorigL->GetBinContent(hbin);
			if(jet->csvIncl() > AN->B_MEDIUM)
			{
				AN->truth1d["Eff_LpassingM"]->Fill(pt, AN->weight);
				PM *= effM;
				PD *= effMorig * scalelM(jet, ltyp);
			}
			else if(jet->csvIncl() > AN->B_LOOSE)
			{
				AN->truth1d["Eff_LpassingL"]->Fill(pt, AN->weight);
				PM *= effL;
				PD *= (effLorig+effMorig)*scalelL(jet, ltyp) - effMorig*scalelM(jet, ltyp);
			}
			else
			{
				PM *= 1. - effM - effL;
				PD *= 1. - (effMorig+effLorig)*scalelL(jet, ltyp);
			}
		}
	}
	
	if(PM <= 0. || PD <= 0.){cout << PM << " BTWERROR " << PD << endl; return(1.);}
	return(PD/PM);
}
