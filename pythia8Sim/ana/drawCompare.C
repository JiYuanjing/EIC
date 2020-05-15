void drawCompare()
{
  TFile* f1 = new TFile("hSigmaX1.root");
  TH1F* h1 = (TH1F*)f1->Get("hSigmaX1");
  h1->SetDirectory(0);
  f1->Close();

  TFile* f2 = new TFile("hSigmaX2.root");
  TH1F* h2 = (TH1F*)f2->Get("hSigmaX2");
  h2->SetDirectory(0);
  f2->Close();

  TFile* f1_au = new TFile("withAuPdf/hSigmaX1.root");
  TH1F* h1_au = (TH1F*)f1_au->Get("hSigmaX1");
  h1_au->SetDirectory(0);
  f1_au->Close();

  TFile* f2_au = new TFile("withAuPdf/hSigmaX2.root");
  TH1F* h2_au = (TH1F*)f2_au->Get("hSigmaX2");
  h2_au->SetDirectory(0);
  f2_au->Close();

  h2_au->Divide(h2);
  h2_au->Draw();
  h2_au->GetYaxis()->SetTitle("eAu/ep");
  
  h1_au->Divide(h1);
  h1_au->Draw("same");

  TFile* f1c = new TFile("hCharmSigmaX1.root");
  TH1F* h1c = (TH1F*)f1c->Get("hCharmSigmaX1");
  h1c->SetDirectory(0);
  f1c->Close();

  TFile* f2c = new TFile("hCharmSigmaX2.root");
  TH1F* h2c = (TH1F*)f2c->Get("hCharmSigmaX2");
  h2c->SetDirectory(0);
  f2c->Close();

  TFile* f1_auc = new TFile("withAuPdf/hCharmSigmaX1.root");
  TH1F* h1_auc = (TH1F*)f1_auc->Get("hCharmSigmaX1");
  h1_auc->SetDirectory(0);
  f1_auc->Close();

  TFile* f2_auc = new TFile("withAuPdf/hCharmSigmaX2.root");
  TH1F* h2_auc = (TH1F*)f2_auc->Get("hCharmSigmaX2");
  h2_auc->SetDirectory(0);
  f2_auc->Close();

  h2_auc->Divide(h2c);
  h2_auc->Draw("same");
  h2_auc->SetMarkerStyle(24);
  // h2_auc->GetYaxis()->SetRangeUser(0,1.2);
  
  h1_auc->Divide(h1c);
  h1_auc->Draw("same");
  h1_auc->SetMarkerStyle(24);


  TLegend* leg = new TLegend(0.2,0.2,0.5,0.5);
  leg->AddEntry(h1_auc,"charm, Q^{2}=4.4GeV");  
  leg->AddEntry(h2_auc,"charm, Q^{2}=139GeV");  
  leg->AddEntry(h1_au,"total, Q^{2}=4.4GeV");  
  leg->AddEntry(h2_au,"total, Q^{2}=139GeV");  
  leg->Draw();

}
