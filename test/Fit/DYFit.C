#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <iostream>
#include <string>
#include "Fit.hh"

void cutTTree_Mll(TTree* tree, TH1F* h1, float xsec, float Mllcut, float Mllcut_2);
Double_t fit_piece(Double_t *x, Double_t *par);
void tmp(){

  TH1::SetDefaultSumw2();
  // Drell-Yan
  TString pre_name = Form("miniaod_v2/dysamples_powheg/");
  TString fnames[] = {"skim_ttree_120to200.root","skim_ttree_200to400.root","skim_ttree_400to800.root","skim_ttree_800to1400.root","skim_ttree_1400to2300.root","skim_ttree_2300to3500.root","skim_ttree_3500to4500.root","skim_ttree_4500to6000.root","skim_ttree_6000toInf.root"};
  float xsecs[] = {19.32,2.731,0.241,0.01678,0.00139,0.00008948,0.000004135,4.56e-7,2.066e-8};

  std::vector<std::pair<TTree*,float>> trees;

  for (int i=0;i<9;i++) {    
    TFile * hfile = new TFile((pre_name+fnames[i]).Data());
    TTree *tree = (TTree*)hfile->Get("MakeTTree_Muons/t"); 
    trees.push_back(std::make_pair(tree,xsecs[i]));
  }

  TH1F *h_sign_Mlljj_120 = new TH1F("h_sign_Mlljj_120","",55,500,6000);
  TH1F *h_sign_Mlljj_200 = new TH1F("h_sign_Mlljj_200","",55,500,6000);

  for (auto t: trees) {    
    cutTTree_Mll(t.first,h_sign_Mlljj_120,t.second,50,200);
    cutTTree_Mll(t.first,h_sign_Mlljj_200,t.second,200,6000);
  }

  float max_range = 5000.0;

  TF1 * f0 = new TF1("f0","[0]*exp([1]*x)+[2]*exp([3]*x)",700,max_range);
  TF1 * f1 = new TF1("f1","[0]*exp([1]*x)",700,max_range);
  TF1 * f2 = new TF1("f2","([0])/(x/13000)^[2]",700,max_range);
  TF1 * f3 = new TF1("f3","([0]*(1-x/13000)^[1])/(x/13000)^[2]",700,max_range);
  TF1 * f4 = new TF1("f4","([0]*(1-x/13000)^[1])/(x/13000)^([2]+[3]*TMath::Log(x/13000))",700,max_range);
  TF1 * f5 = new TF1("f5","x^[0]*exp([1]+[2]*x+[3]*x^2+[4]*x^3)",700,max_range);

  TF1 *myfunc = new TF1("myfunc",fit_piece,700,max_range,4);

  TH1F *h_sign_Mlljj_200_thrown = new TH1F("h_sign_Mlljj_200_thrown","",55,500,6000);

  // h_sign_Mlljj_200->DrawCopy("histo");
  // h_sign_Mlljj_200->Smooth(10);
  // h_sign_Mlljj_200->SetLineColor(kRed);
  // h_sign_Mlljj_200->Draw("histo same");

  h_sign_Mlljj_200_thrown->FillRandom(h_sign_Mlljj_200,10000);
  //h_sign_Mlljj_200->Scale(1/(5400*2460.));
  h_sign_Mlljj_200_thrown->Scale(1/(5400*2460.));

  //betterFit(h_sign_Mlljj_120,f1,"RWL");
  //betterFit(h_sign_Mlljj_200_thrown,f1,"RWL");
  betterFit(h_sign_Mlljj_200,f1,"RWL");
  //betterFit(h_sign_Mlljj_200,myfunc,"");
}

Double_t fit_piece(Double_t *x, Double_t *par){
  Double_t xv =x[0];
  Double_t f =  (xv<2500)*(par[0]*exp(par[1]*xv)) + (xv>=2500)*(par[2]*exp(par[3]*xv));
  return f;
}

void cutTTree_Mll(TTree* tree, TH1F* h1, float xsec, float Mllcut, float Mllcut_2){
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

    if(Mlljj>0 && Mlljj>600 && l1_pt>40 && l2_pt>40 && (l1_pt>60 || l2_pt>60) && j1_pt>40 && j2_pt>40 && Mll>Mllcut && Mll<Mllcut_2 && dR_l1l2 > 0.4 && dR_l1j1 > 0.4 && dR_l1j2 > 0.4 && dR_l2j1 > 0.4 && dR_l2j2 > 0.4 && fabs(j1_eta)<2.4 && fabs(j2_eta)<2.4){
	h1->Fill(Mlljj,reweight*xsec);
    }
  }
}
