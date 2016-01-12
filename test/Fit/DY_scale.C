#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <iostream>
#include <string>

void cutTTree_Mll(TTree* tree, TH1F* h1, float xsec);
void DY_scale(){
  TH1::SetDefaultSumw2();
  // Drell-Yan
  TString pre_name = Form("../../../miniaod_v2/dysamples_powheg/");
  TString fnames[] = {"skim_ttree_120to200.root","skim_ttree_200to400.root","skim_ttree_400to800.root","skim_ttree_800to1400.root","skim_ttree_1400to2300.root","skim_ttree_2300to3500.root","skim_ttree_3500to4500.root","skim_ttree_4500to6000.root","skim_ttree_6000toInf.root"};
  TFile * tfile = new TFile("../../../miniaod_v2/skim_ttree_ttbar.root");
  TFile * dyfile = new TFile("../../../miniaod_v2/skim_ttree_dyjets.root");
  TFile * dfile = new TFile("../../../miniaod_v2/skim_ttree_data.root");

  TTree *treetreebar;
  treetreebar = (TTree*)tfile->Get("MakeTTree_Muons/t"); 
  TTree *dy_tree;
  dy_tree = (TTree*)dyfile->Get("MakeTTree_Muons/t"); 
  TTree *data_tree;
  data_tree = (TTree*)dfile->Get("MakeTTree_Muons/t"); 

  std::vector<std::pair<TTree*,float>> trees;
  float xsecs[] = {19.32,2.731,0.241,0.01678,0.00139,0.00008948,0.000004135,4.56e-7,2.066e-8};

  for (int i=0;i<9;i++) {    
    TFile * hfile = new TFile((pre_name+fnames[i]).Data());
    TTree *tree = (TTree*)hfile->Get("MakeTTree_Muons/t"); 
    trees.push_back(std::make_pair(tree,xsecs[i]));
  }

  TH1F *h_sign_Mll_200 = new TH1F("h_sign_Mll_200","",100,70,110);
  TH1F *t_sign_Mll = new TH1F("t_sign_Mll","",100,70,110);
  TH1F *dy_sign_Mll = new TH1F("dy_sign_Mll","",100,70,110);
  TH1F *data_Mll = new TH1F("data_Mll","",100,70,110);

  for (auto t: trees) {    
    cutTTree_Mll(t.first,h_sign_Mll_200,t.second);
  }

  cutTTree_Mll(dy_tree,dy_sign_Mll,6025.2);
  cutTTree_Mll(treetreebar,t_sign_Mll,57.35);
  cutTTree_Mll(data_tree,data_Mll,1.0);

  dy_sign_Mll->Scale(2460.0/(28747969*0.8));
  t_sign_Mll->Scale(2460.0/(30479698));
  h_sign_Mll_200->Scale(2460.0/100000);

  cout<<"Scale Factor"<<endl;
  cout<<data_Mll->Integral()<<" "<<dy_sign_Mll->Integral()<<" "<<t_sign_Mll->Integral()<<endl;
  float data_count = data_Mll->Integral(data_Mll->GetXaxis()->FindBin(60),data_Mll->GetXaxis()->FindBin(120));
  float dy_count = dy_sign_Mll->Integral(data_Mll->GetXaxis()->FindBin(60),data_Mll->GetXaxis()->FindBin(120));
  float tt_count = t_sign_Mll->Integral(data_Mll->GetXaxis()->FindBin(60),data_Mll->GetXaxis()->FindBin(120));

  float data_count2 = data_Mll->Integral(data_Mll->GetXaxis()->FindBin(200),data_Mll->GetXaxis()->FindBin(2000));
  float dy_count2 = dy_sign_Mll->Integral(data_Mll->GetXaxis()->FindBin(200),data_Mll->GetXaxis()->FindBin(2000));
  float dy_count_pow = h_sign_Mll_200->Integral(data_Mll->GetXaxis()->FindBin(200),data_Mll->GetXaxis()->FindBin(2000));
  float tt_count2 = t_sign_Mll->Integral(data_Mll->GetXaxis()->FindBin(200),data_Mll->GetXaxis()->FindBin(2000));

  cout<<data_count<<" "<<dy_count<<" "<<tt_count<<endl;
  cout<<data_count2<<" "<<dy_count2<<" "<<tt_count2<<endl;
  cout<<"Ratios"<<endl;
  cout<<(data_Mll->Integral()-t_sign_Mll->Integral())/dy_sign_Mll->Integral()<<endl;
  cout<<(data_count-tt_count)/dy_count<<endl;
  cout<<(data_count2-tt_count2)/dy_count2<<endl;
  cout<<(data_count2-tt_count2)/dy_count_pow<<endl;


  TCanvas *canvas1 = new TCanvas("canvas1","",200,10,700,500);
  data_Mll->Draw();
  t_sign_Mll->SetLineColor(kGreen);
  t_sign_Mll->Draw("same");
  dy_sign_Mll->SetLineColor(kRed);
  dy_sign_Mll->Draw("same");
  h_sign_Mll_200->SetLineColor(kMagenta);
  h_sign_Mll_200->Draw("same");

  TCanvas *canvas2 = new TCanvas("canvas2","",200,10,700,500);
  TH1F *ratio_Mll = new TH1F("ratio_Mll","",100,0,500);
  TH1F *diff_Mll = new TH1F("diff_Mll","",100,0,500);
  float diff_count = 0.0;
  for(int i=1; i< 100; i++){
    if(dy_sign_Mll->GetBinContent(i) > 0.01)
      ratio_Mll->SetBinContent(i,(float(data_Mll->GetBinContent(i))-float(t_sign_Mll->GetBinContent(i)))/float(dy_sign_Mll->GetBinContent(i)));
    else
      ratio_Mll->SetBinContent(i,0);
    diff_Mll->SetBinContent(i,(float(data_Mll->GetBinContent(i))-float(t_sign_Mll->GetBinContent(i))-1.14578*float(dy_sign_Mll->GetBinContent(i))));
    if(i>40)
      diff_count += fabs(float(data_Mll->GetBinContent(i))-float(t_sign_Mll->GetBinContent(i))-1.14578*float(dy_sign_Mll->GetBinContent(i)));
  }

  cout<<"Count"<<endl;
  cout<<diff_count<<endl;

  diff_Mll->SetLineColor(kRed);
  diff_Mll->Draw();
  ratio_Mll->Draw("same");
}


void cutTTree_Mll(TTree* tree, TH1F* h1, float xsec){
  int nentries = tree->GetEntries();
  Float_t Mlljj;
  Float_t l1_pt;
  Float_t l2_pt;
  Float_t j1_pt;
  Float_t j2_pt;
  Float_t l1_eta;
  Float_t l2_eta;
  Float_t j1_eta;
  Float_t j2_eta;
  Float_t l1_phi;
  Float_t l2_phi;
  Float_t j1_phi;
  Float_t j2_phi;
  Float_t Mll;
  Float_t dR_l1l2;
  Float_t dR_j1j2;
  Float_t dR_l1j1;
  Float_t dR_l1j2;
  Float_t dR_l2j1;
  Float_t dR_l2j2;
  UInt_t nleptons;
  UInt_t njets;
  UInt_t nvertices;
  Float_t weight;

  tree->SetBranchAddress("Mlljj",&Mlljj);
  tree->SetBranchAddress("leading_lepton_pt",&l1_pt);
  tree->SetBranchAddress("subleading_lepton_pt",&l2_pt);
  tree->SetBranchAddress("leading_lepton_eta",&l1_eta);
  tree->SetBranchAddress("subleading_lepton_eta",&l2_eta);
  tree->SetBranchAddress("leading_lepton_phi",&l1_phi);
  tree->SetBranchAddress("subleading_lepton_phi",&l2_phi);

  tree->SetBranchAddress("leading_jet_pt",&j1_pt);
  tree->SetBranchAddress("subleading_jet_pt",&j2_pt);
  tree->SetBranchAddress("leading_jet_eta",&j1_eta);
  tree->SetBranchAddress("subleading_jet_eta",&j2_eta);
  tree->SetBranchAddress("leading_jet_phi",&j1_phi);
  tree->SetBranchAddress("subleading_jet_phi",&j2_phi);

  tree->SetBranchAddress("dilepton_mass",&Mll);
  tree->SetBranchAddress("dR_leadLepton_subleadLepton",&dR_l1l2);
  tree->SetBranchAddress("dR_leadJet_subleadJet",&dR_j1j2);
  tree->SetBranchAddress("dR_leadLepton_leadJet",&dR_l1j1);
  tree->SetBranchAddress("dR_leadLepton_subleadJet",&dR_l1j2);
  tree->SetBranchAddress("dR_subleadLepton_leadJet",&dR_l2j1);
  tree->SetBranchAddress("dR_subleadLepton_subleadJet",&dR_l2j2);

  tree->SetBranchAddress("nleptons",&nleptons);
  tree->SetBranchAddress("njets",&njets);
  tree->SetBranchAddress("nvertices",&nvertices);
  tree->SetBranchAddress("weight",&weight);

  int count = 0;

  for (Int_t ev = 0; ev < nentries; ev++) {
    tree->GetEntry(ev); 
    float reweight = 1.0;
    if(xsec != 1.0)
      reweight = weight/fabs(weight);

    if(Mlljj>0 && Mlljj>600 && l1_pt>40 && l2_pt>40 && (l1_pt>60 || l2_pt>60) && j1_pt>40 && j2_pt>40 && Mll>50 && Mll<2000 && dR_l1l2 > 0.4 && dR_l1j1 > 0.4 && dR_l1j2 > 0.4 && dR_l2j1 > 0.4 && dR_l2j2 > 0.4 && fabs(j1_eta)<2.4 && fabs(j2_eta)<2.4){
      //if(Mlljj>0 && Mlljj>600 && Mll>50 && Mll<200 && l1_pt>40 && l2_pt>40 && (l1_pt>60 || l2_pt>60)){
      h1->Fill(Mll,reweight*xsec);
    }
  }
}
