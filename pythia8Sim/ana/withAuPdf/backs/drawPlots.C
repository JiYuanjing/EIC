#include "myStyle.h"

void drawPlots()
{
  SetMyStyle();
  gStyle->SetPalette(1);
  // TFile* f = new TFile("Apr22.root"); 
  // TFile* f = new TFile("test.root"); 
  // TFile* f = new TFile("test4.root"); 
  // TFile* f = new TFile("test.root"); 
  // TFile* f = new TFile("submit.root"); 
  // TFile* f = new TFile("addmoreHadron.root"); 
  TFile* f = new TFile("smallsample.root"); 
  // TFile* f = new TFile("fullphase.root"); 
  // TFile* f = new TFile("test.root"); 
  // TFile* f = new TFile("Apr22_3.root"); 
  // TFile* f = new TFile("Apr22_s2000.root"); 
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("plots.pdf");
  gPad->SetLogy();

  TH1* hN = (TH1F*)f->Get("hNevents");
  double Lumi = hN->GetEntries()*1.e-9/(3.25);

  TH1F* hQ = (TH1F*)f->Get("hQ");
  hQ->GetXaxis()->SetTitle("#sqrt(Q^{2})");
  hQ->Draw();
  addpdf(pdf);
  
  TH1F* hx = (TH1F*)f->Get("hx");
  hx->GetXaxis()->SetTitle("Log10(x)");
  hx->Draw();
  addpdf(pdf);

  TH1F* hW = (TH1F*)f->Get("hW");
  hW->GetXaxis()->SetTitle("Log10(W)");
  hW->Draw();
  addpdf(pdf);

  TH1F* hy = (TH1F*)f->Get("hy");
  hy->GetXaxis()->SetTitle("y");
  hy->Draw();
  addpdf(pdf);

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
  double event = (hN->GetBinContent(1)/2.384e-06)/1e16;
  hEvent1->SetLineColor(kRed);
  hEvent1->SetLineStyle(2);
  hEvent1->SetLineWidth(2);
  hEvent1->SetMarkerColor(kRed);
  hEvent1->SetMarkerStyle(22);
  hEvent1->Scale(1./event);
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
  hCharm1->Scale(1./event);
  hCharm1->Rebin();
  hCharm1->Draw("same");
  hEvent2->SetLineColor(kBlack);
  hEvent2->SetLineWidth(2);
  hEvent2->SetLineStyle(2);
  hEvent2->SetMarkerColor(kBlack);
  hEvent2->SetMarkerStyle(22);
  hEvent2->Scale(1./event);
  hEvent2->Rebin();
  hEvent2->Draw("same");
  hCharm2->SetLineStyle(1);
  hCharm2->SetLineColor(kBlack);
  hCharm2->SetMarkerColor(kBlack);
  hCharm2->SetMarkerStyle(20);
  hCharm2->SetLineWidth(2);
  hCharm2->Scale(1./event);
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

  // hCharm1->Divide(hEvent1);
  // hCharm1->Draw();
  gPad->SetLogy(0);
  TH1F* hSigma1 = (TH1F*)f->Get("hSigmaX1");
  double width = hSigma1->GetBinWidth(1);
  for (int i=1;i<=hSigma1->GetNbinsX();i++)
  {

    double dx = (pow(10,hSigma1->GetBinLowEdge(i)+width)-pow(10,hSigma1->GetBinLowEdge(i)));
    double dQ = 1.;
    double L = (hN->GetBinContent(1)/3.18e-3);
    // double xQ4 = 4.4*4.4*pow(10,hSigma->GetBinCenter(i));
    double xQ4 = 1.;
    double factor = xQ4/dx/dQ/L;
    hSigma1->SetBinError(i,hSigma1->GetBinError(i)*factor);
    hSigma1->SetBinContent(i,factor*hSigma1->GetBinContent(i));
  }
  hSigma1->Draw();
  hSigma1->GetXaxis()->SetTitle("Log10(x)");
  hSigma1->GetXaxis()->SetRangeUser(-3.5,0);
  hSigma1->GetYaxis()->SetRangeUser(0,0.3);
  hSigma1->GetYaxis()->SetTitle("#sigma_{r}/dxdQ^{2} (mb*GeV^{2})");
  // addpdf(pdf);

  TH1F* hSigma2 = (TH1F*)f->Get("hSigmaX2");
  width = hSigma2->GetBinWidth(1);
  for (int i=1;i<=hSigma2->GetNbinsX();i++)
  {
    double dx = (pow(10,hSigma2->GetBinLowEdge(i)+width)-pow(10,hSigma2->GetBinLowEdge(i)));
    double dQ = 10.;
    double L = (hN->GetBinContent(1)/3.25e-3);
    // double xQ4 = 4.4*4.4*pow(10,hSigma->GetBinCenter(i));
    double xQ4 = 1.;
    double factor = xQ4/dx/dQ/L;
    hSigma2->SetBinError(i,hSigma2->GetBinError(i)*factor);
    hSigma2->SetBinContent(i,factor*hSigma2->GetBinContent(i));
  }
  hSigma2->SetLineColor(kRed);
  hSigma2->SetMarkerColor(kRed);
  hSigma2->Draw("pcsame");
  hSigma2->GetXaxis()->SetTitle("Log10(x)");
  hSigma2->GetYaxis()->SetTitle("#sigma_{r}/dxdQ^{2} (mb*GeV^{2})");
  hSigma2->GetXaxis()->SetRangeUser(-3.5,0);
  TLegend* leg2 = new TLegend(0.65,0.7,0.88,0.88);
  leg2->AddEntry(hSigma1,"Q^{2}=4.4GeV^{2}","p");
  leg2->AddEntry(hSigma2,"Q^{2}=139GeV^{2}","p");
  leg2->Draw();

  addpdf(pdf);

  TH1F* hSigmaQ2 = (TH1F*)f->Get("hSigmaQ2");
  width = hSigmaQ2->GetBinWidth(1);
  for (int i=1;i<=hSigmaQ2->GetNbinsX();i++)
  {
    double dQ = (pow(10,hSigmaQ2->GetBinLowEdge(i)+width)-pow(10,hSigmaQ2->GetBinLowEdge(i)));
    double dx = 0.02;
    double L = (hN->GetBinContent(1)/3.25e-3);
    // double xQ4 = 4.4*4.4*pow(10,hSigma->GetBinCenter(i));
    double xQ4 = 1.;
    double factor = xQ4/dx/dQ/L;
    hSigmaQ2->SetBinError(i,hSigmaQ2->GetBinError(i)*factor);
    hSigmaQ2->SetBinContent(i,factor*hSigmaQ2->GetBinContent(i)-TMath::Log10(3.2*0.01));
  }
  hSigmaQ2->Draw();
  hSigmaQ2->GetXaxis()->SetTitle("Log10(Q)");
  hSigmaQ2->GetYaxis()->SetTitle("#sigma_{red}/dxdQ^{2}");
  hSigmaQ2->GetXaxis()->SetRangeUser(-3.5,0);
  addpdf(pdf);
  
  TH2F* hXsection = (TH2F*)f->Get("hXsection");
  TH1F* hsub[30];
  double Q2width = hXsection->GetXaxis()->GetBinWidth(1);
  double xwidth = hXsection->GetYaxis()->GetBinWidth(1);
  int lastBin = hXsection->GetYaxis()->FindBin(TMath::Log10(1.3e-4));
  int starBin = hXsection->GetYaxis()->FindBin(TMath::Log10(5e-1));

  for (int ib=starBin;ib>lastBin;ib--)
  {
      hsub[ib] = (TH1F*)hXsection->ProjectionX(Form("hsub_%0.1f",hXsection->GetYaxis()->GetBinCenter(ib)), ib, ib);
      for (int i=1;i<=hsub[ib]->GetNbinsX();i++)
      {
        double dQ = (pow(10,hsub[ib]->GetBinLowEdge(i)+Q2width)-pow(10,hsub[ib]->GetBinLowEdge(i)));
        double dx = (pow(10,hXsection->GetYaxis()->GetBinLowEdge(ib)+xwidth)-pow(10,hXsection->GetYaxis()->GetBinLowEdge(ib)));
        double L = (hN->GetBinContent(1)/3.25e-3);
        // double xQ4 = 4.4*4.4*pow(10,hSigma->GetBinCenter(i));
        double xQ4 = 1.;
        double factor = xQ4/dx/dQ/L;
        hsub[ib]->SetBinError(i,hsub[ib]->GetBinError(i)*factor);
        if (hsub[ib]->GetBinContent(i)>0) hsub[ib]->SetBinContent(i,factor*hsub[ib]->GetBinContent(i)-hXsection->GetYaxis()->GetBinCenter(ib) );
      }
      if (ib!=10){
        hsub[ib]->SetMarkerColor(ib);
        hsub[ib]->SetLineColor(ib);
      }
      else {
        hsub[ib]->SetMarkerColor(30);
        hsub[ib]->SetLineColor(30);
      }
      hsub[ib]->GetYaxis()->SetRangeUser( 0.1,4. );
      hsub[ib]->GetXaxis()->SetTitle("Log_{10}(Q^{2})");
      hsub[ib]->GetYaxis()->SetTitle("#sigma_{red}/dcdQ^{2}-Log_{10}(x)");
      if (ib<starBin) hsub[ib]->Draw("pcsame");
      else if (ib==starBin) hsub[ib]->Draw("pc");
      // hsub[ib]->Draw();
      // drawLatex( 0.7, (hXsection->GetYaxis()->GetBinCenter(ib)+4)/4.0,Form("%0.1f\times10^{%d}",pow(10,hXsection->GetYaxis()->GetBinCenter(ib))/pow(10,floor(hXsection->GetYaxis()->GetBinCenter(ib))), floor(hXsection->GetYaxis()->GetBinCenter(ib)) ) ,0.035 ,kBlack);
      cout << pow(10,hXsection->GetYaxis()->GetBinCenter(ib))<<endl;
  
  }
  addpdf(pdf);

  // TH3F* hpi = (TH3F*)f->Get("hpi");
  // hpi->GetXaxis()->SetTitle("#eta");
  // hpi->GetYaxis()->SetTitle("Log_{10}(p)");
  // TH2F* h2pi = (TH2F*)hpi->Project3D("yx");
  // h2pi->Draw("colz");
  // gPad->SetLogz();
  // addpdf(pdf);

  TH3F* hD0 = (TH3F*)f->Get("hD0");
  hD0->GetXaxis()->SetTitle("#eta");
  hD0->GetYaxis()->SetTitle("Log_{10}(p)");
  // hD0->Scale(1./hN->GetEntries());
  TH2F* h2D0 = (TH2F*)hD0->Project3D("yx");
  h2D0->Draw("colz");
  drawLatex(0.2,0.82,"D^{0}+Anti-D^{0}");
  drawLatex(0.2,0.77,Form("integral Lumilosity = %f fb^{-1}", Lumi));
  drawLatex(0.2,0.72,Form("Counts = %0.0f",h2D0->GetEntries() ));
  gPad->SetLogz();
  addpdf(pdf);

  TH3F* hD0Pi = (TH3F*)f->Get("hD0Pi");
  hD0Pi->GetXaxis()->SetTitle("#eta");
  hD0Pi->GetYaxis()->SetTitle("Log_{10}(p)");
  // hD0Pi->Scale(1./hN->GetEntries());
  TH2F* h2D0Pi = (TH2F*)hD0Pi->Project3D("yx");
  h2D0Pi->Draw("colz");
  drawLatex(0.2,0.82,"D^{0}->#pi");
  drawLatex(0.2,0.77,Form("integral Lumilosity = %f fb^{-1}", Lumi));
  drawLatex(0.2,0.72,Form("Counts = %0.0f",h2D0Pi->GetEntries() ));
  gPad->SetLogz();
  addpdf(pdf);

  TH3F* hD0K = (TH3F*)f->Get("hD0K");
  hD0K->GetXaxis()->SetTitle("#eta");
  hD0K->GetYaxis()->SetTitle("Log_{10}(p)");
  // hD0K->Scale(1./hN->GetEntries());
  TH2F* h2D0K = (TH2F*)hD0K->Project3D("yx");
  h2D0K->Draw("colz");
  drawLatex(0.2,0.82,"D^{0}->K");
  drawLatex(0.2,0.77,Form("integral Lumilosity = %f fb^{-1}", Lumi));
  drawLatex(0.2,0.72,Form("Counts = %0.0f",h2D0K->GetEntries() ));
  gPad->SetLogz();
  addpdf(pdf);

  TH3F* hDplus = (TH3F*)f->Get("hDplus");
  hDplus->GetXaxis()->SetTitle("#eta");
  hDplus->GetYaxis()->SetTitle("Log_{10}(p)");
  hDplus->GetZaxis()->SetRangeUser(1e-6,1);
  TH2F* h2Dplus = (TH2F*)hDplus->Project3D("yx");
  h2Dplus->Draw("colz");
  drawLatex(0.2,0.82,"D^{+}");
  drawLatex(0.2,0.77,Form("integral Lumilosity = %f fb^{-1}", Lumi));
  drawLatex(0.2,0.72,Form("Counts = %0.0f",h2Dplus->GetEntries() ));
  gPad->SetLogz();
  addpdf(pdf);

  TH3F* hLc = (TH3F*)f->Get("hLc");
  hLc->GetXaxis()->SetTitle("#eta");
  hLc->GetYaxis()->SetTitle("Log_{10}(p)");
  hLc->GetZaxis()->SetRangeUser(1e-6,1);
  // hLc->Scale(1./hN->GetEntries());
  TH2F* h2Lc = (TH2F*)hLc->Project3D("yx");
  h2Lc->Draw("colz");
  drawLatex(0.2,0.82,"#Lambda_{c}^{+}");
  drawLatex(0.2,0.77,Form("integral Lumilosity = %f fb^{-1}", Lumi));
  drawLatex(0.2,0.72,Form("Counts = %0.0f",h2Lc->GetEntries() ));
  gPad->SetLogz();
  addpdf(pdf);

  TH3F* hBplus = (TH3F*)f->Get("hBplus");
  hBplus->GetXaxis()->SetTitle("#eta");
  hBplus->GetYaxis()->SetTitle("Log_{10}(p)");
  hBplus->GetZaxis()->SetRangeUser(1e-6,1);
  TH2F* h2Bplus = (TH2F*)hBplus->Project3D("yx");
  h2Bplus->Draw("colz");
  drawLatex(0.2,0.82,"B^{+}");
  drawLatex(0.2,0.77,Form("integral Lumilosity = %f fb^{-1}", Lumi));
  drawLatex(0.2,0.72,Form("Counts = %0.0f",h2Bplus->GetEntries() ));
  gPad->SetLogz();
  addpdf(pdf);

  TH3F* hJphi = (TH3F*)f->Get("hJphi");
  hJphi->GetXaxis()->SetTitle("#eta");
  hJphi->GetYaxis()->SetTitle("Log_{10}(p)");
  hJphi->GetZaxis()->SetRangeUser(-1,1);
  // hJphi->Scale(1./hN->GetEntries());
  TH2F* h2Jphi = (TH2F*)hJphi->Project3D("yx");
  h2Jphi->Draw("colz");
  drawLatex(0.2,0.82,"#J/#Psi");
  drawLatex(0.2,0.77,Form("integral Lumilosity = %f fb^{-1}", Lumi));
  drawLatex(0.2,0.72,Form("Counts = %0.0f",h2Jphi->GetEntries() ));
  gPad->SetLogz();
  addpdf(pdf);

  // TH3F* hDplus = (TH3F*)f->Get("hDplus");
  // hDplus->GetXaxis()->SetTitle("#eta");
  // hDplus->GetYaxis()->SetTitle("Log_{10}(p)");
  hDplus->GetZaxis()->SetRangeUser(-1,-1e-6);
  TH2F* h2Dminus = (TH2F*)hDplus->Project3D("yx");
  h2Dminus->Draw("colz");
  drawLatex(0.2,0.82,"D^{-}");
  drawLatex(0.2,0.77,Form("integral Lumilosity = %f fb^{-1}", Lumi));
  drawLatex(0.2,0.72,Form("Counts = %0.0f",h2Dminus->GetEntries() ));
  gPad->SetLogz();
  addpdf(pdf);

  // TH3F* hLc = (TH3F*)f->Get("hLc");
  // hLc->GetXaxis()->SetTitle("#eta");
  // hLc->GetYaxis()->SetTitle("Log_{10}(p)");
  hLc->GetZaxis()->SetRangeUser(-1,-1e-6);
  // hLc->Scale(1./hN->GetEntries());
  TH2F* h2Lcbar = (TH2F*)hLc->Project3D("yx");
  h2Lcbar->Draw("colz");
  drawLatex(0.2,0.82,"#Lambda_{c}^{-}");
  drawLatex(0.2,0.77,Form("integral Lumilosity = %f fb^{-1}", Lumi));
  drawLatex(0.2,0.72,Form("Counts = %0.0f",h2Lcbar->GetEntries() ));

  gPad->SetLogz();
  addpdf(pdf);

  TH1F* hD0Q2 = (TH1F*)f->Get("hD0Q2");
  hD0Q2->Scale(1./Lumi*1e-6);
  double whD0q2 = hD0Q2->GetBinWidth(1);
  for (int i=0;i<hD0Q2->GetNbinsX();i++)
  {
    double temp = hD0Q2->GetBinContent(i+1);
    hD0Q2->SetBinContent(i+1 , temp/(pow(10,hD0Q2->GetBinLowEdge(i+1)+whD0q2)-pow(10 , hD0Q2->GetBinLowEdge(i+1))));
    cout << pow(10,hD0Q2->GetBinLowEdge(i+1)+whD0q2)<<" "<<pow(10 , hD0Q2->GetBinLowEdge(i+1))<<endl;
  }
  hD0Q2->Draw(); 
  hD0Q2->GetYaxis()->SetTitle("d#sigma/dQ2(nb/GeV^{2})");
  gPad->SetLogy();
  addpdf(pdf);

  //
  pdf->On();
  pdf->Close();

}

