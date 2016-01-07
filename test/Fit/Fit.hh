#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <iostream>
#include <string>

void betterFit(TH1F* h1, TF1* f1, TString fitpars){
  TCanvas *c1 = new TCanvas("c1","",200,10,700,500);
  c1->SetLogy();
  gStyle->SetOptFit(1111);
  h1->Fit(f1,fitpars);
  TCanvas *c2 = new TCanvas("c2","",200,10,700,500);
  TH1F* h2 = new TH1F("h2","",h1->GetNbinsX(),h1->GetXaxis()->GetXmin(),h1->GetXaxis()->GetXmax());
  int min_bin = 1;
  for(int i=1;i<h1->GetNbinsX();i++){
    h2->SetBinContent(i,(h1->GetBinContent(i)-f1->Eval(h1->GetBinCenter(i)))/f1->Eval(h1->GetBinCenter(i)));
  }
  h2->SetName("(h-f)/f");
  h2->Draw("hist e");
}
void bestFit(){

  cout<<"best"<<endl;

}
