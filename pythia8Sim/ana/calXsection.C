#include "myStyle.h"

void drawX(TFile* f ,TPDF* pdf,TFile* fout, double Lumi, double QL1=0.575,double QH1=0.675 ,double QL2=2.075,double QH2=2.175,double s=4*20*100, int mode = 1, double expLumiscale=-1);
void drawXsection(TFile* f ,TPDF* pdf, TString histname , TFile* fout,double Lumi,double s, double offset, double expLumiscale=-1);
void drawEventplot(TFile* f, TPDF* pdf, TFile* fout, double expLumiscale);
void setHist(TH1* h, int color, int linestyle, double linewidth, int markerstyle)
{
  h->SetLineColor(color);
  h->SetLineStyle(linestyle);
  h->SetLineWidth(linewidth);
  h->SetMarkerColor(color);
  h->SetMarkerStyle(markerstyle);
}
void calXsection()
{
  SetMyStyle();
  gStyle->SetPalette(1);
  TFile* fout = new TFile("Xsection.root", "recreate");
  fout->mkdir("Charm");
  fout->mkdir("Inclusive");

  /* TFile* f = TFile::Open("May06_4.root");  */
  TFile* f = TFile::Open("May15.root"); 
  f->cd();
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("plots.pdf");
  gPad->SetLogy();

  TH1* hN = (TH1F*)f->Get("hNevents");
  //use mb for all calculation;
  //the estimated lumi is 10 fb-1 = 1e8 mb-1
  // double const pythiaXsection = 2.286*1e-3; // mb
  double const pythiaXsection = 3.24*1e-3; //unit mb, gotten from pythia 
  double const Lumi_mb = hN->GetEntries()/(pythiaXsection); //lumilosity unit mb-1
  double const Lumi = Lumi_mb*0.3894; //convert lumilosity to GeV2
  double const expLumi = 10; // in fb-1 unit
  double expLumiscale = expLumi*1e12/Lumi_mb;  // convert the unit 
  double s = 4*20*100; // collision energy

  gPad->SetLogy(0);
  TH2F* hQx = (TH2F*)f->Get("hQx");
  hQx->GetXaxis()->SetTitle("Log_{10}(x)");
  hQx->GetYaxis()->SetTitle("Log_{10}(Q^{2})");
  hQx->Draw("colz");
  gPad->SetLogz();
  addpdf(pdf);

  //draw DIS and Charm event vs x, scale to lumilosity = 10 fb-1
  drawEventplot( f, pdf, fout, expLumiscale);
  
  //start Xsection
  //reduced cross section vs x
  // QL QH refer to the selected Q2 range is (pow(QL,10), pow(QH,10))
  // the center value of the Q2 is (pow(QL,10), pow(QH,10))/2.
  // expLumiscale:scale the error bar to the expected lumilosity, if expLumiscale=-1, then do not scale 
  drawX(f,pdf,fout,Lumi,  0.575,0.675 ,2.075,2.175, s, 1, expLumiscale);
  drawX(f,pdf,fout,Lumi,  0.575,0.675 ,2.075,2.175, s, 2, expLumiscale);
 
  //reduced cross section vs Q2, in different x
  drawXsection(f, pdf,  "hXsection", fout,  Lumi, s, 1., expLumiscale);
  drawXsection(f, pdf,  "hCharmXsection",fout, Lumi,s, 10., expLumiscale);
  //
  pdf->On();
  pdf->Close();
}
//---------------------------------------
void drawXsection(TFile* f ,TPDF* pdf, TString histname , TFile* fout,double Lumi, double s,double offset, double expLumiscale)
{
  f->cd();
  TH2F* hXsection = (TH2F*)f->Get(histname.Data());
  TH1F* hsub[30];
  TGraphErrors* gerror[30];
  double Q2width = hXsection->GetXaxis()->GetBinWidth(1);
  double xwidth = hXsection->GetYaxis()->GetBinWidth(1);
  int lastBin = hXsection->GetYaxis()->FindBin(TMath::Log10(1.3e-4));
  int starBin = hXsection->GetYaxis()->FindBin(TMath::Log10(5e-1));
  bool isCharm=false;
  bool isInclusive=false;
  if (fabs(offset-1)<1e-6) isInclusive = true; 
  else if (fabs(offset-10)<1e-6) isCharm = true;

  TH1F* hdraw = (TH1F*)hXsection->ProjectionX("hdraw");
  hdraw->Scale(0);
  hdraw->GetXaxis()->SetTitle("Log_{10}(Q^{2})");

  if (isInclusive)
  {
    hdraw->GetYaxis()->SetTitle("#sigma_{red}/dxdQ^{2}-Log_{10}(x)");
    hdraw->GetYaxis()->SetRangeUser(0.1,4.5);
  }
  
  // charm event
  if (isCharm) 
  {  
    hdraw->GetYaxis()->SetTitle("#sigma_{red}^{c#bar{c}}/dxdQ^{2}");
    hdraw->GetYaxis()->SetTitle("#sigma_{red}^{c#bar{c}}/dxdQ^{2}-Log_{10}(x)/10");
    hdraw->GetYaxis()->SetRangeUser(0.01,0.4);
  }
  hdraw->Draw();
  for (int ib=starBin;ib>lastBin;ib--)
  {
     if (isInclusive) hsub[ib] = (TH1F*)hXsection->ProjectionX(Form("hsubInc_%0.1f",hXsection->GetYaxis()->GetBinCenter(ib)), ib, ib);
     if (isCharm) hsub[ib] = (TH1F*)hXsection->ProjectionX(Form("hsubCharm_%0.1f",hXsection->GetYaxis()->GetBinCenter(ib)), ib, ib);
     hsub[ib]->SetDirectory(0);
     double x[hsub[ib]->GetNbinsX()],y[hsub[ib]->GetNbinsX()],yerr[hsub[ib]->GetNbinsX()]; 
     int npoints=0;
     for (int iq=1;iq<=hsub[ib]->GetNbinsX();iq++)
     {
       double dQ = (pow(10,hsub[ib]->GetBinLowEdge(iq)+Q2width)-pow(10,hsub[ib]->GetBinLowEdge(iq)));
       // double dx = (pow(10,hXsection->GetYaxis()->GetBinLowEdge(ib)+xwidth)-pow(10,hXsection->GetYaxis()->GetBinLowEdge(ib)));
       double dx = (pow(10,xwidth)-1)*pow(10,hXsection->GetYaxis()->GetBinLowEdge(ib));
       double mean_x=0.5*(pow(10,xwidth)+1)*pow(10,hXsection->GetYaxis()->GetBinLowEdge(ib));
       double mean_Q2=(pow(10,hsub[ib]->GetBinLowEdge(iq)+Q2width)+pow(10,hsub[ib]->GetBinLowEdge(iq)))*0.5;
       double mean_y=mean_Q2/s/mean_x;
       double xQ4=mean_Q2*mean_Q2*mean_x;
       double factor = xQ4/dx/dQ/(2*3.141592/137.06/137.06*(1+(1-mean_y)*(1-mean_y)))/Lumi;
       double error;
       if (expLumiscale>0) 
         error = hsub[ib]->GetBinError(iq)*factor/sqrt(expLumiscale);
       else 
         error = hsub[ib]->GetBinError(iq)*factor;

       if (hsub[ib]->GetBinContent(iq)>0 )
       {
         if (iq<hsub[ib]->GetNbinsX() && hsub[ib]->GetBinContent(iq+1)>0)
         { 
            hsub[ib]->SetBinContent(iq,factor*hsub[ib]->GetBinContent(iq));
            hsub[ib]->SetBinError(iq,error);
            y[npoints]=hsub[ib]->GetBinContent(iq)-hXsection->GetYaxis()->GetBinCenter(ib)/offset;
            x[npoints]=hsub[ib]->GetBinCenter(iq);
            yerr[npoints]=error;
            npoints++;
         } 
       }
     }
     
     gerror[ib] = new TGraphErrors( npoints, x,y,0,yerr ); 
     hsub[ib]->SetMarkerColor(ib%9+1);
     hsub[ib]->SetLineColor(ib%9+1);
     hsub[ib]->GetXaxis()->SetTitle("Log_{10}(Q^{2})");
     gerror[ib]->GetXaxis()->SetTitle("Log_{10}(Q^{2})");
     gerror[ib]->SetMarkerColor(ib%9+1);
     gerror[ib]->SetLineColor(ib%9+1);

     // inclusive DIS 
     if (isInclusive)
     {
       gerror[ib]->SetName(Form("gIncXsec_%0.1f",hXsection->GetYaxis()->GetBinCenter(ib)));
       hsub[ib]->GetYaxis()->SetTitle("#sigma_{red}/dxdQ^{2}");
       gerror[ib]->GetYaxis()->SetTitle("#sigma_{red}/dxdQ^{2}-Log_{10}(x)");
     }
    
     // charm event
     if (isCharm) 
     {  
       gerror[ib]->SetName(Form("gCharmXsec_%0.1f",hXsection->GetYaxis()->GetBinCenter(ib)));
       hsub[ib]->GetYaxis()->SetTitle("#sigma_{red}^{c#bar{c}}/dxdQ^{2}");
       gerror[ib]->GetYaxis()->SetTitle("#sigma_{red}^{c#bar{c}}/dxdQ^{2}-Log_{10}(x)/10");
     }

     gerror[ib]->Draw("plsame"); 
  }
  addpdf(pdf);
  fout->cd();
  if (isCharm)
  {
    fout->cd("Charm");
  }
  else if (isInclusive)
  {
    fout->cd("Inclusive");
  }
  for (int ib=starBin;ib>lastBin;ib--)
  {
     hsub[ib]->Write();
     gerror[ib]->Write(); 
  }
}
//---------------------------------------
void drawX(TFile* f ,TPDF* pdf,TFile* fout, double Lumi, double QL1,double QH1,double QL2,double QH2, double s, int mode, double expLumiscale)
{
  f->cd();
  TH1F* hSigma1;
  TH1F* hSigma2;
  if (mode==1)
    hSigma1 = (TH1F*)f->Get("hSigmaX1");
  if (mode==2)
    hSigma1 = (TH1F*)f->Get("hCharmSigmaX1");
  double width = hSigma1->GetBinWidth(1);
  hSigma1->SetDirectory(0);
  for (int i=1;i<=hSigma1->GetNbinsX();i++)
  {
     double dx = (pow(10,hSigma1->GetBinLowEdge(i)+width)-pow(10,hSigma1->GetBinLowEdge(i)));
     double dQ = pow(10,QH1)-pow(10,QL1);
     double mean_x=pow(10,hSigma1->GetBinCenter(i));
     double mean_Q2=0.5*(pow(10,QL1)+pow(10,QH1));
     double mean_y=mean_Q2/s/mean_x;
     double xQ4 = mean_x*mean_Q2*mean_Q2;
     double factor = xQ4/dx/dQ/(2*3.141592/137.06/137.06*(1+(1-mean_y)*(1-mean_y)))/Lumi;
     if (expLumiscale>0) 
       hSigma1->SetBinError(i,hSigma1->GetBinError(i)*factor/sqrt(expLumiscale));
     else 
       hSigma1->SetBinError(i,hSigma1->GetBinError(i)*factor);
     hSigma1->SetBinContent(i,factor*hSigma1->GetBinContent(i));
  }
  hSigma1->Draw();
  hSigma1->GetXaxis()->SetTitle("Log10(x)");
  hSigma1->GetXaxis()->SetRangeUser(-3.,0);
  hSigma1->GetYaxis()->SetRangeUser(0,0.7);
  if (mode==2) hSigma1->GetYaxis()->SetRangeUser(0,0.1);
  hSigma1->GetYaxis()->SetTitle("#sigma_{r}/dxdQ^{2}");
  
  if (mode==1) hSigma2 = (TH1F*)f->Get("hSigmaX2");
  if (mode==2) hSigma2 = (TH1F*)f->Get("hCharmSigmaX2");
  hSigma2->SetDirectory(0);
  width = hSigma2->GetBinWidth(1);
  for (int i=1;i<=hSigma2->GetNbinsX();i++)
  {
    double dx = (pow(10,hSigma2->GetBinLowEdge(i)+width)-pow(10,hSigma2->GetBinLowEdge(i)));
    double dQ = pow(10,QH2)-pow(10,QL2);
    double mean_x=pow(10,hSigma2->GetBinCenter(i));
    double mean_Q2=0.5*(pow(10,QL2)+pow(10,QH2));
    double mean_y=mean_Q2/s/mean_x;
    double xQ4 = mean_x*mean_Q2*mean_Q2;
    double factor = xQ4/dx/dQ/(2*3.141592/137.06/137.06*(1+(1-mean_y)*(1-mean_y)))/Lumi;
    if (expLumiscale>0) 
      hSigma2->SetBinError(i,hSigma2->GetBinError(i)*factor/sqrt(expLumiscale));
    else  
      hSigma2->SetBinError(i,hSigma2->GetBinError(i)*factor);
    hSigma2->SetBinContent(i,factor*hSigma2->GetBinContent(i));
  }
  hSigma2->SetLineColor(kRed);
  hSigma2->SetMarkerColor(kRed);
  hSigma2->Draw("pcsame");
  hSigma2->GetXaxis()->SetTitle("Log10(x)");
  hSigma2->GetYaxis()->SetTitle("#sigma_{r}/dxdQ^{2}");
  hSigma2->GetXaxis()->SetRangeUser(-1.5,0);
  // hSigma2->GetXaxis()->SetRangeUser(-3.5,0);
 
  TLegend* leg2 = new TLegend(0.65,0.7,0.88,0.88);
  leg2->AddEntry(hSigma1,"Q^{2}=4.4GeV^{2}","p");
  leg2->AddEntry(hSigma2,"Q^{2}=139GeV^{2}","p");
  leg2->Draw();

  addpdf(pdf);

  if (mode==1) 
     fout->cd("Charm");
  if (mode==2)
     fout->cd("Inclusive");
  hSigma1->Write();
  hSigma2->Write();
}
//---------------------------------------
void drawEventplot(TFile* f, TPDF* pdf, TFile* fout, double expLumiscale)
{
  f->cd();
  TH1F* hEvent1 = (TH1F*)f->Get("hEvent1");
  TH1F* hEvent2 = (TH1F*)f->Get("hEvent2");
  TH1F* hCharm1 = (TH1F*)f->Get("hCharm1");
  TH1F* hCharm2 = (TH1F*)f->Get("hCharm2");
  // TH1F* hN= (TH1F*)f->Get("hNevents");
  // double event = (hN->GetBinContent(1)/2.384e-06)/1e16;

  setHist(hEvent1, kRed, 2, 2, 22); //Th1*, color, linestyle, linewidth, markerstyle
  hEvent1->Scale(expLumiscale);
  hEvent1->Rebin();
  hEvent1->GetXaxis()->SetTitle("Log10(x)");
  hEvent1->GetYaxis()->SetTitle("No. Events");
  hEvent1->GetYaxis()->SetRangeUser(1e4,1e9);
  hEvent1->GetXaxis()->SetRangeUser(-3,0);
  hEvent1->Draw();

  setHist(hCharm1, kRed, 1, 2, 20); //Th1*, color, linestyle, linewidth, markerstyle
  hCharm1->Scale(expLumiscale);
  hCharm1->Rebin();
  hCharm1->Draw("same");

  setHist(hEvent2, kBlack, 2, 2, 22); //Th1*, color, linestyle, linewidth, markerstyle
  hEvent2->Scale(expLumiscale);
  hEvent2->Rebin();
  hEvent2->Draw("same");

  setHist(hCharm2, kBlack, 1, 2, 20); //Th1*, color, linestyle, linewidth, markerstyle
  hCharm2->Scale(expLumiscale);
  hCharm2->Rebin();
  hCharm2->Draw("same"); 

  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  
  drawLatex(0.5,0.7,"Total DIS");
  drawLatex(0.5,0.35,"Charm event");

  TLegend* leg = new TLegend(0.2,0.7,0.38,0.88);
  leg->AddEntry(hCharm1,"Q^{2}>5GeV^{2}","p");
  leg->AddEntry(hCharm2,"Q^{2}>20GeV^{2}","p");

  leg->Draw();
  addpdf(pdf);
  gPad->SetLogy(0);
  
  fout->cd();
  hCharm1->Write();
  hCharm2->Write();
  hEvent2->Write();
  hEvent1->Write();
}
