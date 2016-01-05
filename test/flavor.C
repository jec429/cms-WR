#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <iostream>
#include <string>

void makeHisto(TTree* tree, TH1F* h1, bool signal);
void makeHisto_Mll(TTree* tree, TH1F* h1, bool signal);

void flavor(){
  
  TString pre_name = Form("~/eos/WR_skims/miniaod_v2/");

  TString side_name = pre_name + "skim_ttree_emu_ttbar.root";
  TString signal_name = pre_name + "skim_ttree_ttbar.root";
  TFile * hfile = new TFile(side_name.Data());
  TFile * gfile = new TFile(signal_name.Data());
  TTree *tree = (TTree*)hfile->Get("MakeTTree_EleMuon/t"); 
  TTree *tree2 = (TTree*)gfile->Get("MakeTTree_Muons/t"); 
  TH1F *h = new TH1F("h","sideband",50,550,2500);
  TH1F *g = new TH1F("g","signal",50,550,2500);
  TH1F *h_Mll = new TH1F("h_Mll","sideband",50,0,1000);
  TH1F *g_Mll = new TH1F("g_Mll","signal",50,0,1000);
  
  makeHisto(tree,h,true);
  makeHisto(tree2,g,true);
  makeHisto_Mll(tree,h_Mll,true);
  makeHisto_Mll(tree2,g_Mll,true);
  TCanvas *c1 = new TCanvas("c1","",200,10,700,500);
  //TPad *pad1 = new TPad("pad1","",0,0,1,0.5);
  //TPad *pad2 = new TPad("pad2","",0,0.5,1,1);
  //pad1->Draw();
  //pad1->cd();
  //c1->SetLogy();
  h->SetLineColor(kRed);
  g->SetLineColor(kBlue);
  //g->Scale(h->Integral()/g->Integral());
  h->Draw("histo");
  g->Draw("histo same");


  TLegend *leg = new TLegend( 0.78, 0.50, 0.98, 0.65 ) ; 
  leg->AddEntry( h, "EMU" ) ; 
  leg->AddEntry( g, "MUMU" ) ; 
  //Float_t sizeleg = leg->GetTextSize()*1.2;
  //leg->SetTextSize( sizeleg ) ;
  leg->SetFillColor( kWhite ) ; 
  leg->Draw(); 

  h->GetXaxis()->SetTitle("M_{lljj} GeV");
  cout<<"emu="<<h->Integral()<<" mumu="<<g->Integral()<<endl; 

  TCanvas *c2 = new TCanvas("c2","",200,10,700,500);
  c2->cd();
  //g->Divide(h);
  //g->GetXaxis()->SetRange(10,50);
  //TF1 *myline = new TF1("myline","[0]", 550, 3000);
  //myline->SetParName(0,"c0");
  //g->Fit("myline","RL");
  //g->Draw();
  h_Mll->SetLineColor(kRed);
  g_Mll->SetLineColor(kBlue);
  //g->Scale(h->Integral()/g->Integral());
  h_Mll->Draw("histo");
  g_Mll->Draw("histo same");

  cout<<"emu="<<h_Mll->Integral()<<" mumu="<<g_Mll->Integral()<<endl; 
  
  TFile f("flavor_histos.root","new");
  h->Write();
  g->Write();
  f.Write();
}

void makeHisto(TTree* tree, TH1F* h1, bool signal){
  
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
  //if(!is_data)
  tree->SetBranchAddress("weight",&weight);

  for (Int_t ev = 0; ev < nentries; ev++) {
    tree->GetEntry(ev); 
    float reweight = weight/fabs(weight);

    if(Mlljj>0 && Mlljj>600 && l1_pt>40 && l2_pt>40 && (l1_pt>60 || l2_pt>60) && j1_pt>40 && j2_pt>40 && Mll>200 && dR_l1l2 > 0.4 && dR_l1j1 > 0.4 && dR_l1j2 > 0.4 && dR_l2j1 > 0.4 && dR_l2j2 > 0.4 && fabs(j1_eta)<2.4 && fabs(j2_eta)<2.4)
      h1->Fill(Mlljj,reweight);

  }
}

void makeHisto_Mll(TTree* tree, TH1F* h1, bool signal){
  
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
  //if(!is_data)
  tree->SetBranchAddress("weight",&weight);

  for (Int_t ev = 0; ev < nentries; ev++) {
    tree->GetEntry(ev); 
    float reweight = weight/fabs(weight);

    if(Mlljj>0 && Mlljj>600 && l1_pt>40 && l2_pt>40 && (l1_pt>60 || l2_pt>60) && j1_pt>40 && j2_pt>40 && Mll>50 && dR_l1l2 > 0.4 && dR_l1j1 > 0.4 && dR_l1j2 > 0.4 && dR_l2j1 > 0.4 && dR_l2j2 > 0.4 && fabs(j1_eta)<2.4 && fabs(j2_eta)<2.4)
      //if(Mlljj>0 && Mlljj>600 && l1_pt>40 && l2_pt>40 && (l1_pt>60 || l2_pt>60) && j1_pt>40 && j2_pt>40 && Mll>50 && Mll<250 && dR_l1l2 > 0.4 && dR_l1j1 > 0.4 && dR_l1j2 > 0.4 && dR_l2j1 > 0.4 && dR_l2j2 > 0.4 && fabs(j1_eta)<2.4 && fabs(j2_eta)<2.4)
      h1->Fill(Mll,reweight);

  }
}

