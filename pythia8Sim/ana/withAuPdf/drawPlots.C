#include "myStyle.h"
#include "TPDF.h"

void drawParticle(TString histname, TString part, TString anitpart, double Lumi, TPDF* pdf, TFile* file);
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
  // TFile* f = new TFile("fullphase3.root"); 
  TString dirname = "";
  TString filename = "May05_noref";
  TFile* f = new TFile((dirname+filename+".root").Data()); 
  // TFile* f = new TFile("test.root"); 
  // TFile* f = new TFile("Apr22_3.root"); 
  // TFile* f = new TFile("Apr22_s2000.root"); 
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF(Form("plots_%s.pdf",filename.Data()));
  gPad->SetLogy();

  TH1* hN = (TH1F*)f->Get("hNevents");
  double const pythiaXsection = 3.25; //1e-3 mb
  double Lumi = hN->GetEntries()*1.e-9/pythiaXsection;

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
  // hDplus->GetZaxis()->SetRangeUser(1e-6,1);
  // TH2F* h2Dplus = (TH2F*)hDplus->Project3D("yx");
  // h2Dplus->Draw("colz");
  // drawLatex(0.2,0.82,"D^{+}");
  // drawLatex(0.2,0.77,Form("integral Lumilosity = %f fb^{-1}", Lumi));
  // drawLatex(0.2,0.72,Form("Counts = %0.0f",h2Dplus->GetEntries() ));
  // gPad->SetLogz();
  // addpdf(pdf);
  //
  // hDplus->GetZaxis()->SetRangeUser(-1,-1e-6);
  // TH2F* h2Dminus = (TH2F*)hDplus->Project3D("yx");
  // h2Dminus->Draw("colz");
  // drawLatex(0.2,0.82,"D^{-}");
  // drawLatex(0.2,0.77,Form("integral Lumilosity = %f fb^{-1}", Lumi));
  // drawLatex(0.2,0.72,Form("Counts = %0.0f",h2Dminus->GetEntries() ));
  // gPad->SetLogz();
  // addpdf(pdf);

  drawParticle( "hDplus", "D^{+}", "D^{-}",Lumi, pdf, f);
  drawParticle( "hD0", "D^{0}", "#bar{D^{0}}",Lumi, pdf, f);
  drawParticle( "hDs", "D_{s}^{+}", "D_{s}^{-}",Lumi, pdf, f);
  drawParticle( "hLc", "#Lambda_{c}^{+}", "#Lambda_{c}^{-}",Lumi, pdf, f);
  drawParticle( "hLambda", "#Lambda^{+}", "#Lambda^{-}",Lumi, pdf, f);
  drawParticle( "hK", "K^{+}", "K^{-}",Lumi, pdf, f);
  drawParticle( "hpi", "#pi^{+}", "#pi^{-}",Lumi, pdf, f);
  drawParticle( "hp", "p", "#bar{p}",Lumi, pdf, f);

  TH1F* hD0Q2 = (TH1F*)f->Get("hD0Q2");
  hD0Q2->Scale(1./Lumi*1e-6);
  double whD0q2 = hD0Q2->GetBinWidth(1);
  for (int i=0;i<hD0Q2->GetNbinsX();i++)
  {
    double temp = hD0Q2->GetBinContent(i+1);
    hD0Q2->SetBinContent(i+1 , temp/(pow(10,hD0Q2->GetBinLowEdge(i+1)+whD0q2)-pow(10 , hD0Q2->GetBinLowEdge(i+1))));
    // cout << pow(10,hD0Q2->GetBinLowEdge(i+1)+whD0q2)<<" "<<pow(10 , hD0Q2->GetBinLowEdge(i+1))<<endl;
  }
  hD0Q2->Draw(); 
  hD0Q2->GetYaxis()->SetTitle("d#sigma/dQ2(nb/GeV^{2})");
  gPad->SetLogy();
  addpdf(pdf);

  //
  pdf->On();
  pdf->Close();

}
void drawParticle(TString histname, TString part, TString anitpart, double Lumi, TPDF* pdf, TFile* f)
{
  TH3F* hLambda = (TH3F*)f->Get(histname.Data());
  hLambda->GetXaxis()->SetTitle("#eta");
  hLambda->GetYaxis()->SetTitle("Log_{10}(p)");
  hLambda->GetZaxis()->SetRangeUser(1e-6,1);
  // hLambda->Scale(1./hN->GetEntries());
  TH2F* h2Lambda = (TH2F*)hLambda->Project3D("yx");
  h2Lambda->Draw("colz");
  drawLatex(0.2,0.82,part.Data());
  drawLatex(0.2,0.77,Form("integral Lumilosity = %f fb^{-1}", Lumi));
  drawLatex(0.2,0.72,Form("Counts = %0.2f M",h2Lambda->GetEntries()/1000000 ));
  gPad->SetLogz();
  addpdf(pdf);
  hLambda->GetZaxis()->SetRangeUser(-1,-1e-6);
  // hLambda->Scale(1./hN->GetEntries());
  TH2F* h2Lambdabar = (TH2F*)hLambda->Project3D("yx");
  h2Lambdabar->Draw("colz");
  drawLatex(0.2,0.82,anitpart.Data());
  drawLatex(0.2,0.77,Form("integral Lumilosity = %0.4f fb^{-1}", Lumi));
  drawLatex(0.2,0.72,Form("Counts = %0.2f M",h2Lambdabar->GetEntries()/1000000 ));
  gPad->SetLogz();
  addpdf(pdf);
}
