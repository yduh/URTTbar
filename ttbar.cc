#include <iostream>
#include "AnalyzerBase.h"
#include "URStreamer.h"
#include "URDriver.h"
#include "TH1F.h"
using namespace std;

class ttbar : public AnalyzerBase
{
public:
  ttbar(const std::string output_filename):
    AnalyzerBase("ttbar", output_filename),
    h_mupt_(0), h_mueta_(0)
  {};
  
  //This method is called once per job at the beginning of the analysis
  //book here your histograms/tree and run every initialization needed
  virtual void begin()
  {
    outFile_.cd();
    h_mupt_ =  new TH1F("mu_Pt","RECO Muon P_{t}", 150, 0, 300);
    h_mueta_ = new TH1F("mu_Eta","Muon |#eta|", 15, 0, 3);
  }

  //This method is called once every file, contains the event loop
  //run your proper analysis here
  virtual void analyze()
  {
    URStreamer event(tree_);
    while(event.next())
    {
      const vector<Muon> muons = event.muons();
      //loop over muons (by reference)
      cout << muons.size();
      for(vector<Muon>::const_iterator muon = muons.begin(); muon != muons.end(); ++muon)
      //for(size_t idx = 0; idx < muons.size(); ++idx)
      //for(auto& muon : muons)      
	{
	  cout << " " <<  muon->pt()<<","<<muon->eta()<< " ";
	  h_mupt_->Fill(muon->pt());
	  h_mueta_->Fill(muon->eta());
	}
      cout << endl;
    }
  }

  //this method is called at the end of the job, by default saves
  //every histogram/tree produced, override it if you need something more
  //virtual void end();

  //do you need command-line or cfg options? If so implement this 
  //method to book the options you need. CLI parsing is provided
  //by AnalysisFW/interface/URParser.h and uses boost::program_options
  //look here for a quickstart tutorial: 
  //http://www.boost.org/doc/libs/1_51_0/doc/html/program_options/tutorial.html
  static void setOptions() {}
private:
  // Nothing by default
  // Add your private variables/methods here
  TH1F *h_mupt_, *h_mueta_;

};

//make it executable
int main(int argc, char *argv[])
{
  URParser &parser = URParser::instance(argc, argv);
  URDriver<ttbar> test;
  return test.run();
}
