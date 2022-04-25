#include <vector>
#include <string>
#include "Pythia8/Pythia.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
//#include "Hists.h"
#include "Anacuts.h"
#define DEBUG 0
using namespace Pythia8; 

int main(int argc, char* argv[]) {
  if (argc != 4) {
    cout << "Usage: " << argv[0] << " runcard  rootfile xmldoc" << endl;
    return 2;
  }
  char* runcard  = argv[1];
  char* rootfile = argv[2];
  const string xmlDB = argv[3];
  // const int mode = atoi(argv[4]);
  //--------------------------------------------------------------
  //  Initialization
  //--------------------------------------------------------------

  time_t now1 = time(0);
  cout << "============================================================================" << endl;
  cout << "Executing program '" << argv[0] << "', start at: " << ctime(&now1);
  cout << "Arguments: " << argv[1] << " " << argv[2]<<" " <<argv[3]<< endl;
  cout << "============================================================================" << endl;

  //
  //  Create instance of Pythia 
  //
  cout <<xmlDB<<endl;
  Pythia pythia(xmlDB.c_str()); // the init parameters are read from xml files
  // stored in the xmldoc directory. This includes
  // particle data and decay definitions.
  cout <<"init pythia"<<endl; 
  //
  // Shorthand for (static) settings
  //
  Settings& settings = pythia.settings;

  //
  //  Read in runcard
  //
  pythia.readFile(runcard);  
  cout << "Runcard '" << runcard << "' loaded." << endl;

  int  maxNumberOfEvents = settings.mode("Main:numberOfEvents");
  int  maxErrors = settings.mode("Main:timesAllowErrors");
  bool showCS    = settings.flag("Init:showChangedSettings");
  bool showAS    = settings.flag("Init:showAllSettings");

  //  Initialize Pythia, ready to go
  pythia.init();
  // List changed or all data
  //
  // if (showCS) settings.listChanged();
  // if (showAS) settings.listAll();

  //--------------------------------------------------------------
  //  Event loop
  //--------------------------------------------------------------

  int iErrors = 0;
  double pt;
  double eta;
  double y;

  InitHists(); 

  for(int ievent = 0;ievent < maxNumberOfEvents;ievent++) {
      hNevents->Fill(1);
    if (ievent%10000==0) cout<<"event"<<ievent<<endl;
    if (!pythia.next()) {
      if (++iErrors < maxErrors) continue;
      cout << "Error: too many errors in event generation - check your settings & code" << endl;
      break;
    }
    //count the multipilicity

    int nCharge_fwd = 0;
    int nCharge_mid = 0;
    bool havecharm;
    for(int i=0;i<pythia.event.size();i++){
      if (pythia.event[i].id()==4) havecharm=1;
      bool isfinal = pythia.event[i].isFinal();  
      bool ischarge = pythia.event[i].isCharged();
      if (!isfinal || !ischarge) continue;
      double eta = pythia.event[i].eta(); 
      bool isFwd = fabs(eta)>Cuts::fwdEta[0]&&fabs(eta)<Cuts::fwdEta[1];
      bool isMid = fabs(eta)<Cuts::eta;

      if (isFwd) nCharge_fwd++;
      if (isMid) nCharge_mid++;

      hdNdeta->Fill(eta);
    }
    // if (havecharm) pythia.event.list();
    hmult[0]->Fill(nCharge_fwd);
    hmult[1]->Fill(nCharge_mid);
    for(int i=0;i<pythia.event.size();i++){
      pt = pythia.event[i].pT();
      eta = pythia.event[i].eta();
      y = pythia.event[i].y();

      // bool isinit = pythia.event[i].status();
      // if (fabs(eta)>=Cuts::eta) continue;
      if (fabs(y)>=Cuts::rapidity) continue;
      int id = pythia.event[i].id();
      bool findparticle = histmap.find(id)!=histmap.end();
      if (DEBUG) cout<<"start to fill hists"<<endl; 
      if (findparticle) {
        if (DEBUG) cout<<"find "<<ParticleName[id]<<endl;
        histmap[abs(id)]->Fill(pt,nCharge_fwd);
      }
      else continue;
    }

  }//end of event loop

  //--------------------------------------------------------------
  //  Finish up
  //--------------------------------------------------------------
  pythia.stat();
  cout << "Writing File" << endl;
  TFile* mFile = new TFile(rootfile,"recreate");
  WriteFile(mFile);
  mFile->Close();

  time_t now2 = time(0);
  cout << "============================================================================" << endl;
  cout << "Program finished at: " << ctime(&now2);
  cout << "============================================================================" << endl;

  return 0;
}
void InitHists(){
  int ip=0;
  std::map<int,std::string>::iterator it;
  cout<<"start initialize hists..."<<endl;
  typedef std::pair<int ,TH2*> pairhist;
  for (it=ParticleName.begin();it!=ParticleName.end();it++){
    hparticle[ip] = new TH2D(("h"+it->second).c_str(), (it->second+";p_{T}(GeV);Nch").c_str(),200,0,20,200,0,200);
    histmap.insert(pairhist(it->first,hparticle[ip]));
    hparticle[ip]->SetDirectory(0);
    cout<<"initialize "<<it->second<<endl;
    ip++;
  }
  hmult[0] = new TH1D("hnChargeMid","nChargeMid;Nch(|#eta|<1);Counts",200,0,200);
  hmult[0]->SetDirectory(0);
  hmult[1] = new TH1D("hnChargeFwd","nChargeFwd;Nch(2<|#eta|<5);Counts",200,0,200);
  hmult[1]->SetDirectory(0);
  hNevents = new TH1D("hNevents","hNevents",1,0,2);
  hdNdeta = new TH1D("hdNdeta","hdNdeta;#eta",150,-3,3);
}
void WriteFile(TFile* f){
  f->cd();
  for (int ip=0;ip<NParticle;ip++){
    hparticle[ip]->Write();
  }
  hmult[0]->Write();
  hmult[1]->Write();
  hNevents->Write();
  hdNdeta->Write();
}
