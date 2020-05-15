#include "myStyle.h"

void drawPlots()
{
  SetMyStyle();
  gStyle->SetPalette(1);
  // TFile* f = new TFile("Apr22.root"); 
  TFile* f = new TFile("Apr22_3.root"); 
  // TFile* f = new TFile("Apr22_s2000.root"); 
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("plots.pdf");
  gPad->SetLogy();

  TH1* hN = (TH1F*)f->Get("hNevents");

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

  TH2F* hpi = (TH2F*)f->Get("hpi");
  hpi->GetXaxis()->SetTitle("#eta");
  hpi->GetYaxis()->SetTitle("Log_{10}(p)");
  hpi->Draw("colz");
  gPad->SetLogz();
  addpdf(pdf);

  TH2F* hD0 = (TH2F*)f->Get("hD0");
  hD0->GetXaxis()->SetTitle("#eta");
  hD0->GetYaxis()->SetTitle("Log_{10}(p)");
  hD0->Scale(1./hN->GetEntries());
  hD0->Draw("colz");
  gPad->SetLogz();
  addpdf(pdf);
  TH1F* hD0p = (TH1F*)hD0->ProjectionY();
  hD0p->Scale(1./hD0p->GetBinWidth(1));
  hD0p->Draw();
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
  TH1F* hSigma = (TH1F*)f->Get("hSigma");
  double width = hSigma->GetBinWidth(1);
  for (int i=1;i<=hSigma->GetNbinsX();i++)
  {
    hSigma->SetBinError(i,hSigma->GetBinError(i)/(pow(10,hSigma->GetBinLowEdge(i)+width)-pow(10,hSigma->GetBinLowEdge(i)))/0.02/hN->GetEntries()*2.38e-3);
    hSigma->SetBinContent(i,hSigma->GetBinContent(i)/(pow(10,hSigma->GetBinLowEdge(i)+width)-pow(10,hSigma->GetBinLowEdge(i)))/0.02/hN->GetEntries()*2.36e-3);
  }
  // hSigma->Scale(1./hN->GetEntries());
  hSigma->Draw();
  hSigma->GetXaxis()->SetTitle("Log10(x)");
  hSigma->GetXaxis()->SetRangeUser(-3.5,0);
  addpdf(pdf);

  pdf->On();
  pdf->Close();

}

