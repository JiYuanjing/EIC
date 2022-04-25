#include "TH1.h"
#include "TH2.h"
#include <map>
#include <string>
using namespace std;
namespace Cuts{
  double eta = 1;
  double rapidity = 1;
  double fwdEta[2]={2,5};
}
std::map<int,std::string> ParticleName = {
  // {4122,"Lc"},{411,"Dplus"},{421,"D0"},{413,"Dstar"},{431,"Ds"},{511,"B0"},{521,"Bplus"},{523,"Bstar"},{531,"Bs"},{541,"Bc"},{5122,"Lb"},{-2212,"antiproton"},{-211,"pi"},{310,"k"},{3122,"lambda"}
  {4122,"Lc"},{411,"Dplus"},{421,"D0"},{413,"Dstar"},{431,"Ds"},{511,"B0"},{521,"Bplus"},{523,"Bstar"},{531,"Bs"},{541,"Bc"},{5122,"Lb"},{2212,"proton"},{211,"pi"},{310,"ks"},{3122,"lambda"},{2112 ,"n"},{2224,"deltapp"},{4222,"sigmacpp"},{4212,"sigmacp"},{4112,"sigmac0"},{4224,"sigmacpps"},{4214,"sigmacps"},{4114,"sigmac0s"},{4232,"xicp"},{4132,"xic0"},{4322,"xicpr"},{4312,"xic0r"},{4324,"xicpps"},{4314,"xic0s"},{4332,"omegac0"},{4334,"omegac0s"},{4412,"xiccp"},{4422,"xiccpp"},{4414,"xiccps"},{4424,"xiccpps"},{4432,"omegaccp"},{4434,"omegaccps"},{4444,"omegacccpp"}
}; 
// int const NParticle = 15;
int const NParticle = 38;

std::vector <string> charms={"D0","Dstar","Dplus","Ds","Lc"};
std::vector <string> beautys={"B0","Bstar","Bplus","Bs","Bc","Lb"};
std::vector <string> ups= {"pi","proton"};
std::vector <string> stranges= {"ks","lambda"};

TH2D* hparticle[NParticle];
std::map<int,TH2*> histmap;
TH1D* hmult[2]; // 1 forward and 0 middle
TH1D* hNevents;
TH1D* hdNdeta;
void InitHists();
void WriteFile(TFile* f);

//used for analysis
std::map<string,TH2*> hnamemap;
void bookHists(TFile* f);
