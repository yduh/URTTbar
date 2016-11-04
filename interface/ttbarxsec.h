#ifndef TTBARAN_H
#define TTBARAN_H
#include <iostream>
#include <list>
#include <set>
#include "AnalyzerBase.h"
#include "URStreamer.h"
#include "URDriver.h"
#include "IDMuon.h"
#include "IDElectron.h"
#include "IDJet.h"
#include "IDMet.h"
#include "GenObject.h"
#include "TTBarGenPlots.h"
#include "TTBarPlots.h"
#include "TTBarSolver.h"
#include "TTBarResponse.h"
#include "TTBarResponse2D.h"
#include "Permutation.h"
#include "BtagEff.h"
//#include "JetScale.h"
#include "JetScaler.h"
#include "BTagWeight.h"
//#include "NeutrinoSolver.h"

using namespace std;
class PDFuncertainty;

class ttbar : public AnalyzerBase
{
	friend class TTBarGenPlots;
    friend class TTBarPlots;
    friend class TTBarResponse;
    friend class BTagWeight;
    friend class JetScaler;
    //friend class NeutrinoSolver;

	private:
		int isDA = 0;
		map<int, set<int> >  runinfo;
		PDFuncertainty* pdfunc;
		//Collections
		//Gen:
		bool FULLHAD;
		bool SEMILEP;
		bool FULLLEP;
		bool SEMILEPACC;
		list<GenObject> sgenparticles;
		vector<GenObject*> genfincls;
	  	vector<GenObject*> genbpartons;
	  	vector<GenObject*> gencpartons;
		//GenObject* gent;
		//GenObject* gentbar;
		//GenObject* genbl;
		//GenObject* genbh;
		//TLorentzVector gentoplep;
		//TLorentzVector gentophad;
        TLorentzVector gentq;
        TLorentzVector gentqbar;
        TLorentzVector gentqlep;
        TLorentzVector gentqhad;
        TLorentzVector gentqhad_3j;
        TLorentzVector gentqhad_miss;
        TLorentzVector gentqhad_misspartner;
        TLorentzVector gent;
        TLorentzVector gentbar;
        TLorentzVector gentlep;
        TLorentzVector genthad;


		list<Genjet> sgenjets;
		vector<Genjet*> genaddjets;

		Permutation genallper;
		Permutation psper;
		Permutation* genper;
		Permutation rightper;
		Permutation testper;
		Permutation bestper;

		//reco
		list<IDJet> sjets;
		vector<IDJet*> cleanedjets;
		vector<IDJet*> reducedjets;
		list<IDMuon> smuons;
		vector<IDMuon*> loosemuons;
		vector<IDMuon*> tightmuons;
		list<IDElectron> selectrons;
		vector<IDElectron*> looseelectrons;
		vector<IDElectron*> mediumelectrons;
		IDMet met;

		//hists
                TH2DCollection reco3j2d;
                TH1DCollection reco3j1d;
                TH2DCollection right3j2d;
                TH1DCollection right3j1d;
                TH2DCollection wrong3j2d;
                TH1DCollection wrong3j1d;
                TH2DCollection semi3j2d;
                TH1DCollection semi3j1d;
                TH2DCollection other3j2d;
                TH1DCollection other3j1d;
                TH2DCollection gen3j2d;
                TH1DCollection gen3j1d;
                TH1DCollection alpha3j1d;
                TH2DCollection alpha3j2d;
                TH1DCollection missj1d;
                TH2DCollection missj2d;
		TH1DCollection gen1d;
		TH2DCollection gen2d;
		TH1DCollection reco1d;
		TH2DCollection reco2d;
		TH1DCollection truth1d;
		TH2DCollection truth2d;
                TH1DCollection yuka1d_gen;
                //TH1DCollection yuka1d_offshell;
                TH1DCollection yuka1d_reco;
                TH1DCollection yuka1d_reco_right;
                TH1DCollection yuka1d_reco_wrong;
                TH1DCollection yuka1d_reco_semi;
                TH1DCollection yuka1d_reco_other;
                TH2DCollection yuka2d_gen;
                //TH2DCollection yuka2d_offshell;
                TH2DCollection yuka2d_reco;
                TH2DCollection yuka2d_reco_right;
                TH2DCollection yuka2d_reco_wrong;
                TH2DCollection yuka2d_reco_semi;
                TH2DCollection yuka2d_reco_other;

        TTBarGenPlots ttp_genall;
        TTBarGenPlots ttp_genacc;
        TTBarGenPlots ttp_genjet;

		TTBarPlots ttp_truth;
		TTBarPlots ttp_right;
		TTBarPlots ttp_wrong;
		TTBarPlots ttp_semi;
		TTBarPlots ttp_other;
		TTBarPlots ttp_all;

	//	TTBarPlots ttp_jetspos_right;
	//	TTBarPlots ttp_jetspos_wrong;
	//	TTBarPlots ttp_hadjets_right;
	//	TTBarPlots ttp_hadjets_wrong;
	//	TTBarPlots ttp_jets_right;
	//	TTBarPlots ttp_jets_wrong;
	//	TTBarPlots ttp_blep_right;
	//	TTBarPlots ttp_blep_wrong;
		//TTBarPlots ttp_whad_right;
		//TTBarPlots ttp_whad_wrong;

		TTBarPlots ttp_tlepthad_right;
		TTBarPlots ttp_tlep_right;
		TTBarPlots ttp_thad_right;
		TTBarPlots ttp_nn_right;
		TTBarPlots ttp_nsemi_right;

		TTBarResponse response;
		TTBarResponse2D response2d;
		TTBarResponse response_ps;

		BtagEff btageff;
		BTagWeight btagweight;

                //NeutrinoSolver NS_missj;
		//ttbar solver
		TTBarSolver ttsolver;

		JetScaler jetscaler;

		//configuration
		bool PDFTEST;
		bool PSEUDOTOP;
		bool BTAGMODE;
		bool JETSCALEMODE;
		bool MUONS;
		bool ELECTRONS;
		double B_TIGHT;
		double B_MEDIUM;
		double B_LOOSE;
		int cnbtag;
		size_t cnusedjets;
		double clikelihoodcut;
		double cwjetptsoft;
		double cwjetpthard;
		double cbjetptsoft;
		double cbjetpthard;
		double cjetetamax;
		double clptmin;
		double cletamax;
		double cpwjetptsoft;
		double cpwjetpthard;
		double cpbjetptsoft;
		double cpbjetpthard;
		double cpjetetamax;
		double cplptmin;
		double cpletamax;
		double cpjetsep;
		double csigmajet;
		double cjetres;
		double csigmamet;
		double ctopptweight;
		double cttptweight;
		double ctoprapweight;
		int cfacscale;
		int crenscale;
		int cbtagunc;
		int cltagunc;
		int cpileup;
		bool HERWIGPP;
		bool PYTHIA6;
                bool TTMC;
                bool SCALEUP;
                bool SCALEDOWN;
                string cjecuncertainty;
                string yukawasf;
                int njetsmin;
                int njetsmax;
		//
		double jetptmin;
	
		double weight;
                double weightparametrize;
                double weightparametrize_origin;
		double mcweight;

		//binning vectors
		vector<double> topptbins;
		vector<double> topybins;
		vector<double> ttmbins;
		vector<double> ttybins;
		vector<double> ttptbins;
		vector<double> metbins;
		vector<double> jetbins;
		vector<double> nobins;
		vector<double> dybins;
		vector<double> btagpt;
		vector<double> runbins;

//		JetScale jetscale;

		TH1D* puhist;
		TH2D* musfhist;
		TH2D* elsfhist;
		TH2D* mutrgsfhist;
		TH2D* eltrgsfhist;
                TH1D* yukahist_mtt;
                TH1D* yukahist_dely;
                TH1D* yukahist_beta;
                TH2D* yukahist_2d;
                TGraph* grlikelihood3j_nsd;
                TGraph* grlikelihood3j_thad;
                //TGraph* grlikelihood3j_nsptmin;
                //TGraph* grlikelihood3j_nsetamax;
                TH2D* likelihood3j_nspteta_2d;

	public:

		ttbar(const std::string output_filename);
		~ttbar();

		//This method is called once per job at the beginning of the analysis
		//book here your histograms/tree and run every initialization needed
		virtual void begin();
		//virtual void end();
		virtual void analyze();

		void SelectGenParticles(URStreamer& event);
		void SelectPseudoTop(URStreamer& event);
		void SelectRecoParticles(URStreamer& event);
		void AddGenJetSelection(URStreamer& event);
		void ttanalysis(URStreamer& event);

		static void setOptions() {}
};

#endif
