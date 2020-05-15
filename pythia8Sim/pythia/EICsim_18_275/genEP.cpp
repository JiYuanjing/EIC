#include <vector>
// #include "Pythia.h"
// #include "Dire/Dire.h"
#include "Pythia8/Pythia.h"
#include "Pythia8/Event.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMath.h"
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
  // cout <<xmlDB<<endl;
  Pythia pythia(xmlDB.c_str()); // the init parameters are read from xml files
  // Pythia pythia; // the init parameters are read from xml files
  // Dire dire;
  // stored in the xmldoc directory. This includes
  // particle data and decay definitions.
  cout <<"init pythia"<<endl; 
  Event& event = pythia.event;

  //  Read in runcard
  // dire.init(pythia, runcard);
  pythia.readFile(runcard);
  cout << "Runcard '" << runcard << "' loaded." << endl;
  
  Settings& settings = pythia.settings;
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
  InitHists(); 

  for(int ievent = 0;ievent < maxNumberOfEvents;ievent++) {
    if (ievent%10000==0) cout<<"event"<<ievent<<endl;
    hNevents->Fill(1);
    if (!pythia.next()) {
      if (++iErrors < maxErrors) continue;
      cout << "Error: too many errors in event generation - check your settings & code" << endl;
      break;
    }
    if (event.size()<7) continue;
    Vec4 pProton = event[1].p();
    Vec4 peIn    = event[4].p();
    Vec4 peOut   = event[6].p();
    Vec4 pPhoton = peIn - peOut;
 
    // Q2, W2, Bjorken x, y.
    double Q2 = - pPhoton.m2Calc();
    double W2 = (pProton + pPhoton).m2Calc();
    double x  = Q2 / (2. * pProton * pPhoton);
    double y  = (pProton * pPhoton) / (pProton * peIn);
    bool passcut =  y>0.01 && y<0.95;
    // if (!passcut) continue;

    // Fill kinematics histograms.
    Qhist->Fill( sqrt(Q2) );
    Whist->Fill( TMath::Log(sqrt(W2)) );
    xhist->Fill( x );
    yhist->Fill( y );
    pTehist->Fill( event[6].pT() );
    Qxhist->Fill( TMath::Log10(x), TMath::Log10(Q2) );
    // cout << x << endl;

    bool haveCharm=false;
    for(int i=0;i<event.size();i++){
      if (!passcut) continue;
      if (fabs(event[i].id()) == 4) haveCharm=true;
      fillHadron(event,i,Q2,x);
    }
   
    // if (passcut){
       fillXsection( Q2, x, y, haveCharm);
    // }
    // factor=factor*Q2*Q2; 
    if (haveCharm)
    {
      if (Q2>5) hCharm1->Fill(TMath::Log10(x));
      if (Q2>20) hCharm2->Fill(TMath::Log10(x));
    }
    else 
    {
      if (Q2>5) hEvent1->Fill(TMath::Log10(x));
      if (Q2>20) hEvent2->Fill(TMath::Log10(x));
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
  cout<<"start initialize hists..."<<endl;
  hNevents = new TH1F("hNevents","hNevents",1,0,2);
  hdNdeta = new TH1F("hdNdeta","hdNdeta;#eta",150,-5,5);
  // particle
  hpi = new TH3F("hpi","hpi;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hK = new TH3F("hK","hK;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hp = new TH3F("hp","hp;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hLambda = new TH3F("hLambda","hLambda;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hD0 = new TH3F("hD0","hD0;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hD0pt = new TH1F("hD0Q2","hD0Q2;Log10(Q2);",150,0,4);
  hD0Pi = new TH3F("hD0Pi","hD0Pi;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hD0K = new TH3F("hD0K","hD0K;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hDplus = new TH3F("hDplus","hDplus;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hDs = new TH3F("hDs","hDs;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hLc = new TH3F("hLc","hLc;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hUpsilon = new TH3F("hUpsilon","hUpsilon;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hUpsilon2s = new TH3F("hUpsilon2s","hUpsilon;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hUpsilonMu = new TH3F("hUpsilonMu","hUpsilon;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hBplus = new TH3F("hBplus","hUpsilon;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hJphi = new TH3F("hJphi","hJphi;#eta;Log10(p);charge",150,-7,7,100,-2,3,2,-1.5,1.5);
  hJphiMu = new TH3F("hJphiMu","hJphiMu;#eta;Log10(p);charge",150, -7,7,100,-2,3,2,-1.5,1.5);
  //
  Qhist = new TH1F("hQ","Q [GeV]", 100, 0., 50.);
  Whist = new TH1F("hW","Log(W) log[GeV]", 60, 0., 3);
  xhist = new TH1F("hx","x", 100, 0., 1.);
  Qxhist = new TH2F("hQx","x vs Q^{2};Log(x);Log(Q^{2})",50,-5,0,60,0,3);
  yhist = new TH1F("hy","y", 100, 0., 1.);
  pTehist = new TH1F("heScat","pT of scattered electron [GeV]", 100, 0., 50.);
  pTrhist = new TH1F("hparton","pT of radiated parton [GeV]", 100, 0., 50.);
  pTdhist = new TH1F("hratio","ratio pT_parton/pT_electron", 100, 0., 5.);
  hCharm1 = new TH1F("hCharm1","hCharm1", 30,-3,0);
  hCharm2 = new TH1F("hCharm2","hCharm1", 30,-3,0);
  hEvent1 = new TH1F("hEvent1","hEvent1", 30,-3,0);
  hEvent2 = new TH1F("hEvent2","hEvent2", 30,-3,0);
  hSigmaX1 = new TH1F("hSigmaX1","hSigmaX1", 20,-4,0);
  hSigmaX1->Sumw2();
  hSigmaX2 = new TH1F("hSigmaX2","hSigmaX2", 20,-4,0);
  hSigmaX2->Sumw2();

  hCharmSigmaX1 = new TH1F("hCharmSigmaX1","hSigmaX1", 20,-4,0);
  hCharmSigmaX1->Sumw2();
  hCharmSigmaX2 = new TH1F("hCharmSigmaX2","hSigmaX2", 20,-4,0);
  hCharmSigmaX2->Sumw2();

  hXsection = new TH2F("hXsection","hXsection", 12,0,3, 20,-4,0); 
  hXsection->Sumw2();
  hCharmXsection = new TH2F("hCharmXsection","hCharmXsection", 12,0,3, 20,-4,0); 
  hCharmXsection->Sumw2();

  hSigmaQ2 = new TH1F("hSigmaQ2","hSigmaQ2", 12, 0, 3);
  hSigmaQ2->Sumw2();
}

void WriteFile(TFile* f){
  f->cd();
  hNevents->Write();
  hdNdeta->Write();
  // hpi->Write();
  // hD0->Write();
  Qhist->Write();
  Whist->Write();
  xhist->Write();
  Qxhist->Write();
  yhist->Write();
  pTehist->Write();
  pTrhist->Write();
  pTdhist->Write();
  hCharm1->Write();
  hCharm2->Write();
  hEvent1->Write();
  hEvent2->Write();
  hSigmaX1->Write();
  hSigmaX2->Write();
  hCharmSigmaX1->Write();
  hCharmSigmaX2->Write();

  hSigmaQ2->Write();
  hXsection->Write();
  hCharmXsection->Write();
  //
  hD0pt->Write();
  hD0->Write(); 
  hDs->Write();
  hD0Pi->Write();
  hD0K->Write();
  hDplus->Write();
  hBplus->Write();
  hLc->Write(); 
  hUpsilon->Write();
  hUpsilon2s->Write();
  hUpsilonMu->Write();
  hJphi->Write();
  hJphiMu->Write();
  hp->Write();
  hpi->Write();
  hLambda->Write();
  hK->Write();
}

void fillHadron(Event& event, int i, double Q2, double x)
{
   bool isfinal = event[i].isFinal();  
   bool ischarge = event[i].isCharged();
   double eta = event[i].eta();
   if (fabs(event[i].id()) == 421) 
   {
     // hD0->Fill(eta, TMath::Log10(event[i].pAbs()),event[i].charge());
     hD0->Fill(eta, TMath::Log10(event[i].pAbs()),event[i].id()>0?1:-1);
     hD0pt->Fill(TMath::Log10(Q2));
   }
   if (fabs(event[i].id()) == 521) 
   {
     hBplus->Fill(eta, TMath::Log10(event[i].pAbs()),event[i].charge());
   }
   if (fabs(event[i].id()) == 443 )
   {
     hJphi->Fill(eta, TMath::Log10(event[i].pAbs()), event[i].charge() );
   }
   if (fabs(event[i].id()) == 553 ) 
   {
     hUpsilon->Fill(eta, TMath::Log10(event[i].pAbs()), event[i].charge() );
   }
   if (fabs(event[i].id()) == 4122 ) 
   {
     hLc->Fill(eta, TMath::Log10(event[i].pAbs()), event[i].charge() );
   }
   if (fabs(event[i].id()) == 321 ) 
   {
     hK->Fill(eta, TMath::Log10(event[i].pAbs()), event[i].charge() );
   }
   if (fabs(event[i].id()) == 2212 && event[i].isFinal() ) 
   {
     hp->Fill(eta, TMath::Log10(event[i].pAbs()), event[i].charge() );
   }     
   if (fabs(event[i].id()) == 3122 ) 
   {
     hLambda->Fill(eta, TMath::Log10(event[i].pAbs()), event[i].id()>0?1:-1);
   }
   if (fabs(event[i].id()) == 431 ) 
   {
     hDs->Fill(eta, TMath::Log10(event[i].pAbs()), event[i].charge() );
   }
   if (fabs(event[i].id()) == 411 ) 
   {
     hDplus->Fill(eta, TMath::Log10(event[i].pAbs()), event[i].charge());
   }

   if (!isfinal || !ischarge) return;
   if (fabs(event[i].id()) == 211 || fabs(event[i].id()) == 321)
   {
     if (fabs(event[i].id())==211)
       hpi->Fill(eta, TMath::Log10(event[i].pAbs()), event[i].charge());
     if (event[i].mother1()>0 && event[i].mother2()==0) 
     {
       if (fabs(event[event[i].mother1()].id()) == 421)
       {
          if (fabs(event[i].id())==321) hD0K->Fill(eta, TMath::Log10(event[i].pAbs()), event[i].charge());
          if (fabs(event[i].id())==211) hD0Pi->Fill(eta, TMath::Log10(event[i].pAbs()), event[i].charge());
       }
     }
   }
   if (fabs(event[i].id())==13) 
   {
      if (event[i].mother1()>0 && event[i].mother2()==0) 
      {
         if (fabs(event[event[i].mother1()].id()) == 443 )
         {
            hJphiMu->Fill(eta, TMath::Log10(event[i].pAbs()), event[i].charge());
         }
         if (fabs(event[event[i].mother1()].id()) == 553 )
         {
            hUpsilonMu->Fill(eta, TMath::Log10(event[i].pAbs()), event[i].charge());
         }
      }
   }
}

void fillXsection(double Q2,double x,double y, bool haveCharm)
{
  double factor = Q2*Q2*x*0.5/3.1415*137.036*137.036/(1+(1-y)*(1-y));
  // double factor = 1;
  double QL1 = 0.575, QH1 = 0.675;
  double QL2 = 2.075, QH2 = 2.175;
  // if ( fabs(TMath::Log10(Q2)-0.625)<0.05) 
  if ( TMath::Log10(Q2)<QH1 && TMath::Log10(Q2)>QL1) 
  {  
    hSigmaX1->Fill(TMath::Log10(x),factor); 
    if (haveCharm) 
       hCharmSigmaX1->Fill(TMath::Log10(x),factor); 
  }
  // if (fabs(TMath::Log10(Q2)-2.125)<0.05)
  if (TMath::Log10(Q2)<QH2 && TMath::Log10(Q2)>QL2)
  {
    hSigmaX2->Fill(TMath::Log10(x),factor); 
    if (haveCharm) 
       hCharmSigmaX2->Fill(TMath::Log10(x),factor); 
  }
  if (fabs(TMath::Log10(x)-TMath::Log10(3.2*0.01))<0.1) hSigmaQ2->Fill( TMath::Log10(Q2), factor); 
  hXsection->Fill(TMath::Log10(Q2),TMath::Log10(x),factor); 
  if (haveCharm) hCharmXsection->Fill(TMath::Log10(Q2),TMath::Log10(x),factor); 
   
}
