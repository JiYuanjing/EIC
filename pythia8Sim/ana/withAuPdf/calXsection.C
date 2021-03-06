#include "myStyle.h"

void drawX(TFile* f ,TPDF* pdf, double Lumi, double QL1=0.575,double QH1=0.675 ,double QL2=2.075,double QH2=2.175, int mode = 1);
void drawXsection(TFile* f ,TPDF* pdf, TString histname ,double Lumi, double offset);
void calXsection()
{
  SetMyStyle();
  gStyle->SetPalette(1);
  // TFile* f = new TFile("data/fullphase3.root"); 
  // TFile* f = new TFile("data/fixbugMay04.root"); 
  // TFile* f = new TFile("data/May06_2.root"); 
  // TFile* f = new TFile("May06_low.root"); 
  // TFile* f = new TFile("May06_3.root"); 
  TFile* f = new TFile("May06_4.root"); 
  // TFile* f = new TFile("olddata/test_May06.root"); 
  // TFile* f = new TFile("data/withAuPDF/May04.root"); 
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("plots.pdf");
  gPad->SetLogy();

  TH1* hN = (TH1F*)f->Get("hNevents");
  //use mb for all calculation;
  //the estimated lumi is 10 fb-1 = 1e8 mb-1
  // double const pythiaXsection = 2.286*1e-3; //1e-3 mb
  // double const pythiaXsection = 3.24*1e-3; //1e-3 mb
  double const pythiaXsection = 2.53*1e-3; //1e-3 mb
  double const Lumi = hN->GetEntries()*0.3894/(pythiaXsection); //convert lumilosity to GeV2
  double const expLumi = 10; // in fb-1 unit
  double expLumiscale = expLumi*0.3894*1e12/Lumi;  // convert the unit 

  gPad->SetLogy(0);
  TH2F* hQx = (TH2F*)f->Get("hQx");
  hQx->GetXaxis()->SetTitle("Log_{10}(x)");
  hQx->GetYaxis()->SetTitle("Log_{10}(Q^{2})");
  hQx->Draw("colz");
  gPad->SetLogz();
  addpdf(pdf);

  //start Xsection
  TH1F* hEvent1 = (TH1F*)f->Get("hEvent1");
  TH1F* hEvent2 = (TH1F*)f->Get("hEvent2");
  TH1F* hCharm1 = (TH1F*)f->Get("hCharm1");
  TH1F* hCharm2 = (TH1F*)f->Get("hCharm2");
  // TH1F* hN= (TH1F*)f->Get("hNevents");
  // double event = (hN->GetBinContent(1)/2.384e-06)/1e16;
  hEvent1->SetLineColor(kRed);
  hEvent1->SetLineStyle(2);
  hEvent1->SetLineWidth(2);
  hEvent1->SetMarkerColor(kRed);
  hEvent1->SetMarkerStyle(22);
  hEvent1->Scale(expLumiscale);
  hEvent1->Rebin();
  hEvent1->GetXaxis()->SetTitle("Log10(x)");
  hEvent1->GetYaxis()->SetTitle("No. Events");
  hEvent1->GetYaxis()->SetRangeUser(1e4,1e9);
  hEvent1->GetXaxis()->SetRangeUser(-3,0);

  hEvent1->Draw();
  hCharm1->SetLineStyle(1);
  hCharm1->SetMarkerColor(kRed);
  hCharm1->SetMarkerStyle(20);
  hCharm1->SetLineWidth(2);
  hCharm1->Scale(expLumiscale);
  hCharm1->Rebin();
  hCharm1->Draw("same");
  hEvent2->SetLineColor(kBlack);
  hEvent2->SetLineWidth(2);
  hEvent2->SetLineStyle(2);
  hEvent2->SetMarkerColor(kBlack);
  hEvent2->SetMarkerStyle(22);
  hEvent2->Scale(expLumiscale);
  hEvent2->Rebin();
  hEvent2->Draw("same");
  hCharm2->SetLineStyle(1);
  hCharm2->SetLineColor(kBlack);
  hCharm2->SetMarkerColor(kBlack);
  hCharm2->SetMarkerStyle(20);
  hCharm2->SetLineWidth(2);
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
  drawX(f,pdf,Lumi, 0.575,0.675 ,2.075,2.175, 1);
  drawX(f,pdf,Lumi, 0.575,0.675 ,2.075,2.175, 2);
 
  TH1F* hSigmaQ2 = (TH1F*)f->Get("hSigmaQ2");
  double width = hSigmaQ2->GetBinWidth(1);
  for (int i=1;i<=hSigmaQ2->GetNbinsX();i++)
  {
    double dQ = (pow(10,hSigmaQ2->GetBinLowEdge(i)+width)-pow(10,hSigmaQ2->GetBinLowEdge(i)));
    double dx = 0.02;
    double xQ4 = 1.;
    double factor = xQ4/dx/dQ/Lumi;
    hSigmaQ2->SetBinError(i,hSigmaQ2->GetBinError(i)*factor);
    hSigmaQ2->SetBinContent(i,factor*hSigmaQ2->GetBinContent(i)-TMath::Log10(3.2*0.01));
  }
  hSigmaQ2->Draw();
  hSigmaQ2->GetXaxis()->SetTitle("Log10(Q)");
  hSigmaQ2->GetYaxis()->SetTitle("#sigma_{red}/dxdQ^{2}");
  hSigmaQ2->GetXaxis()->SetRangeUser(-3.5,0);
  addpdf(pdf);

  drawXsection(f, pdf, "hXsection", Lumi, 1.);
  drawXsection(f, pdf, "hCharmXsection", Lumi,10.);
  //
  pdf->On();
  pdf->Close();

}

void drawXsection(TFile* f ,TPDF* pdf, TString histname ,double Lumi, double offset)
{
  TH2F* hXsection = (TH2F*)f->Get(histname.Data());
  // TH2F* hXsectionError = (TH2F*)hXsection->Clone("hXsectionError");
  // TH2F* hXsectionErrorratio = (TH2F*)hXsection->Clone("hXsectionError");
  TH1F* hsub[30];
  TH1F* hsubCros[30];
  double Q2width = hXsection->GetXaxis()->GetBinWidth(1);
  double xwidth = hXsection->GetYaxis()->GetBinWidth(1);
  int lastBin = hXsection->GetYaxis()->FindBin(TMath::Log10(1.3e-4));
  int starBin = hXsection->GetYaxis()->FindBin(TMath::Log10(5e-1));

  for (int ib=starBin;ib>lastBin;ib--)
  {
      hsub[ib] = (TH1F*)hXsection->ProjectionX(Form("hsub_%0.1f",hXsection->GetYaxis()->GetBinCenter(ib)), ib, ib);
      // hsubCros[ib] = hsub[ib]->Clone(Form("hsub_%0.1f",hXsection->GetYaxis()->GetBinCenter(ib)));
      for (int i=1;i<=hsub[ib]->GetNbinsX();i++)
      {
        double dQ = (pow(10,hsub[ib]->GetBinLowEdge(i)+Q2width)-pow(10,hsub[ib]->GetBinLowEdge(i)));
        // double dx = (pow(10,hXsection->GetYaxis()->GetBinLowEdge(ib)+xwidth)-pow(10,hXsection->GetYaxis()->GetBinLowEdge(ib)));
        double dx = (pow(10,xwidth)-1)*pow(10,hXsection->GetYaxis()->GetBinLowEdge(ib));
        // double L = (hN->GetBinContent(1)/pythiaXsection);
        // double xQ4 = 4.4*4.4*pow(10,hSigma->GetBinCenter(i));
        double xQ4 = 1.;
        double factor = xQ4/dx/dQ/Lumi;
        double error = hsub[ib]->GetBinError(i)*factor;
        
        hsub[ib]->SetBinError(i,error);

        if (hsub[ib]->GetBinContent(i)>0 )
        {
          if (i<hsub[ib]->GetNbinsX() && hsub[ib]->GetBinContent(i+1)>0) 
            hsub[ib]->SetBinContent(i,factor*hsub[ib]->GetBinContent(i)-hXsection->GetYaxis()->GetBinCenter(ib)/offset );
          // double scalederror
          // hXsectionError->SetBinContent();
        }
        else 
        { 
          hsub[ib]->SetBinContent(i,0);
          hsub[ib]->SetBinError(i,0);
        }
        if (hsub[ib]->GetBinContent(i)>5) 
        {
          hsub[ib]->SetBinContent(i,0);
          hsub[ib]->SetBinError(i,0);

        }
      }
      // if (ib!=10){
        hsub[ib]->SetMarkerColor(ib%9+1);
        hsub[ib]->SetLineColor(ib%9+1);
      // }
      // else {
        // hsub[ib]->SetMarkerColor(30);
        // hsub[ib]->SetLineColor(30);
      // }
      hsub[ib]->GetXaxis()->SetTitle("Log_{10}(Q^{2})");
      if (fabs(offset-1)<1e-6) 
      {
        hsub[ib]->GetYaxis()->SetTitle("#sigma_{red}/dxdQ^{2}-Log_{10}(x)");
        hsub[ib]->GetYaxis()->SetRangeUser( 0.1/offset,4.5/offset );
      }
      if (fabs(offset-10)<1e-6) {
        hsub[ib]->GetYaxis()->SetTitle("#sigma_{red}^{c#bar{c}}/dxdQ^{2}-Log_{10}(x)/10");
        hsub[ib]->GetYaxis()->SetRangeUser( 0.1/offset,4./offset );
      }
      // if (ib<starBin) hsub[ib]->Draw("psame");
      if (ib<starBin) hsub[ib]->Draw("L p same");
      else if (ib==starBin) hsub[ib]->Draw("L p");
      // hsub[ib]->Draw();
      // drawLatex( 0.7, (hXsection->GetYaxis()->GetBinCenter(ib)+4)/4.0,Form("%0.1f\times10^{%d}",pow(10,hXsection->GetYaxis()->GetBinCenter(ib))/pow(10,floor(hXsection->GetYaxis()->GetBinCenter(ib))), floor(hXsection->GetYaxis()->GetBinCenter(ib)) ) ,0.035 ,kBlack);
      // cout << pow(10,hXsection->GetYaxis()->GetBinCenter(ib))<<endl;
  }
  addpdf(pdf);
  // f->Close();
}
void drawX(TFile* f ,TPDF* pdf, double Lumi, double QL1=0.575,double QH1=0.675 ,double QL2=2.075,double QH2=2.175, int mode = 1)
{
  TH1F* hSigma1;
  TH1F* hSigma2;
  if (mode==1)
    hSigma1 = (TH1F*)f->Get("hSigmaX1");
  if (mode==2)
    hSigma1 = (TH1F*)f->Get("hCharmSigmaX1");
  double width = hSigma1->GetBinWidth(1);
  for (int i=1;i<=hSigma1->GetNbinsX();i++)
  {
     double dx = (pow(10,hSigma1->GetBinLowEdge(i)+width)-pow(10,hSigma1->GetBinLowEdge(i)));
     double dQ = pow(10,QH1)-pow(10,QL1);
     // double dQ = 1;
     // double L = (hN->GetBinContent(1)/3.18e-3);
     // double xQ4 = 4.4*4.4*pow(10,hSigma->GetBinCenter(i));
     double xQ4 = 1.;
     double factor = xQ4/dx/dQ/Lumi;
     hSigma1->SetBinError(i,hSigma1->GetBinError(i)*factor);
     hSigma1->SetBinContent(i,factor*hSigma1->GetBinContent(i));
  }
  hSigma1->Draw();
  hSigma1->GetXaxis()->SetTitle("Log10(x)");
  hSigma1->GetXaxis()->SetRangeUser(-3.,0);
  hSigma1->GetYaxis()->SetRangeUser(0,0.7);
  if (mode==2) hSigma1->GetYaxis()->SetRangeUser(0,0.1);
  hSigma1->GetYaxis()->SetTitle("#sigma_{r}/dxdQ^{2}");
  if (mode==2) hSigma1->SaveAs("hCharmSigmaX1.root");
  if (mode==1) hSigma1->SaveAs("hSigmaX1.root");
  

  if (mode==1) hSigma2 = (TH1F*)f->Get("hSigmaX2");
  if (mode==2) hSigma2 = (TH1F*)f->Get("hCharmSigmaX2");
  width = hSigma2->GetBinWidth(1);
  for (int i=1;i<=hSigma2->GetNbinsX();i++)
  {
    double dx = (pow(10,hSigma2->GetBinLowEdge(i)+width)-pow(10,hSigma2->GetBinLowEdge(i)));
    double dQ = pow(10,QH2)-pow(10,QL2);
    // double dQ = 10;
    double xQ4 = 1.;
    double factor = xQ4/dx/dQ/Lumi;
    hSigma2->SetBinError(i,hSigma2->GetBinError(i)*factor);
    hSigma2->SetBinContent(i,factor*hSigma2->GetBinContent(i));
  }
  hSigma2->SetLineColor(kRed);
  hSigma2->SetMarkerColor(kRed);
  hSigma2->Draw("pcsame");
  hSigma2->GetXaxis()->SetTitle("Log10(x)");
  hSigma2->GetYaxis()->SetTitle("#sigma_{r}/dxdQ^{2}");
  hSigma2->GetXaxis()->SetRangeUser(-1.5,0);
  if (mode==2) hSigma2->SaveAs("hCharmSigmaX2.root");
  if (mode==1) hSigma2->SaveAs("hSigmaX2.root");
  // hSigma2->GetXaxis()->SetRangeUser(-3.5,0);
  TLegend* leg2 = new TLegend(0.65,0.7,0.88,0.88);
  leg2->AddEntry(hSigma1,"Q^{2}=4.4GeV^{2}","p");
  leg2->AddEntry(hSigma2,"Q^{2}=139GeV^{2}","p");
  leg2->Draw();

  addpdf(pdf);
}
