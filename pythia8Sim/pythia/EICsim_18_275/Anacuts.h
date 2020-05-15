#include "TH1.h"
#include "TH2.h"
#include <map>
#include <string>
#include "Pythia8/Pythia.h"
#include "Pythia8/Event.h"
using namespace std;

using namespace Pythia8; 
namespace Cuts{
  double eta = 1;
  // double eta = 2;
  double rapidity = 1;
  // double rapidity = 2;
}

TH1F* hNevents;
TH1F* hdNdeta;
// TH2F* hpi;
// TH2F* hD0;

TH1F* Qhist;
TH1F* Whist;
TH1F* xhist;
TH2F* Qxhist;
TH1F* yhist;
TH1F* pTehist;
TH1F* pTrhist;
TH1F* pTdhist;
TH1F* hCharm1;
TH1F* hCharm2;
TH1F* hEvent1;
TH1F* hEvent2;
TH1F* hSigmaX1; 
TH1F* hSigmaX2;
TH1F* hCharmSigmaX1; 
TH1F* hCharmSigmaX2;

TH2F* hXsection;
TH2F* hCharmXsection;
TH1F* hSigmaQ2;
//////
TH3F* hD0;
TH3F* hpi;
TH3F* hLambda;
TH3F* hK;
TH3F* hp;
TH1F* hD0pt;
TH3F* hD0Pi;
TH3F* hD0K;
TH3F* hDplus;
TH3F* hDs;
TH3F* hBplus;
TH3F* hLc; 
TH3F* hUpsilon;
TH3F* hUpsilon2s;
TH3F* hUpsilonMu;
TH3F* hJphi;
TH3F* hJphiMu;
///

void InitHists();
void WriteFile(TFile* f);

void bookHists(TFile* f);

void fillXsection(double Q2,double x,double y, bool haveCharm);

void fillHadron(Event & event, int i, double Q2, double x);
