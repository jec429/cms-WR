#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <iostream>
#include <string>

void cutTTree_Mlljj(TTree* tree, TH1F* h1, float xsec);
void Workspace_DY(){
  using namespace RooFit ;

  TH1::SetDefaultSumw2();
  // Drell-Yan
  TString pre_name = Form("../../../miniaod_v2/dysamples_powheg/");
  TString pre_name_ee = Form("../../../miniaod_v2/electrons/");
  TString fnames[] = {"skim_ttree_120to200.root","skim_ttree_200to400.root","skim_ttree_400to800.root","skim_ttree_800to1400.root","skim_ttree_1400to2300.root","skim_ttree_2300to3500.root","skim_ttree_3500to4500.root","skim_ttree_4500to6000.root","skim_ttree_6000toInf.root"};
  float xsecs[] = {19.32,2.731,0.241,0.01678,0.00139,0.00008948,0.000004135,4.56e-7,2.066e-8};

  std::vector<std::pair<TTree*,float>> trees;
  std::vector<std::pair<TTree*,float>> trees_ee;

  for (int i=0;i<9;i++) {    
    TFile * hfile = new TFile((pre_name+fnames[i]).Data());
    TTree *tree = (TTree*)hfile->Get("MakeTTree_Muons/t"); 
    trees.push_back(std::make_pair(tree,xsecs[i]));
    TFile * hfile_ee = new TFile((pre_name_ee+fnames[i]).Data());
    TTree *tree_ee = (TTree*)hfile_ee->Get("MakeTTree_Electrons/t"); 
    trees_ee.push_back(std::make_pair(tree_ee,xsecs[i]));
  }

  TH1F *h_sign_Mlljj_200 = new TH1F("h_sign_Mlljj_200","",44,600,5000);
  TH1F *h_sign_Mlljj_200_ee = new TH1F("h_sign_Mlljj_200_ee","",44,600,5000);

  for (auto t: trees) {    
    cutTTree_Mlljj(t.first,h_sign_Mlljj_200,t.second);
  }
  for (auto t: trees_ee) {
    cutTTree_Mlljj(t.first,h_sign_Mlljj_200_ee,t.second);
  }

  h_sign_Mlljj_200_ee->Draw();
  /*
  RooWorkspace myWS("myWS");
  
  myWS.factory("x[600,5000]");
  myWS.var("x")->setBins(44);
  
  myWS.factory("B[-1,1]");
  myWS.var("B")->setConstant(false);
  myWS.factory("C[-1,1]");
  myWS.var("C")->setConstant(false);  
  myWS.factory("Exponential::bkgPdf(x,B)");
  myWS.factory("EXPR::bkgPdf2('exp(-C*x^0.9)',x,C)");
  // Electrons
  myWS.factory("B_ee[-1,1]");
  myWS.var("B_ee")->setConstant(false);
  myWS.factory("C_ee[-1,1]");
  myWS.var("C_ee")->setConstant(false);
  myWS.factory("Exponential::bkgPdf_ee(x,B_ee)");
  myWS.factory("EXPR::bkgPdf2_ee('exp(-C_ee*x^0.9)',x,C_ee)");
  
  RooDataHist data("data","data",RooArgList(*myWS.var("x")),h_sign_Mlljj_200);
  myWS.import(data,Rename("data")) ;
  RooDataHist data_ee("data_ee","data_ee",RooArgList(*myWS.var("x")),h_sign_Mlljj_200_ee);
  myWS.import(data_ee,Rename("data_ee")) ;

  myWS.pdf("bkgPdf")->fitTo(data, SumW2Error(kTRUE));
  myWS.pdf("bkgPdf2")->fitTo(data, SumW2Error(kTRUE));
  myWS.pdf("bkgPdf_ee")->fitTo(data_ee, SumW2Error(kTRUE));
  myWS.pdf("bkgPdf2_ee")->fitTo(data_ee, SumW2Error(kTRUE));

  RooPlot* frame1 = (*myWS.var("x")).frame();
  data.plotOn(frame1);
  myWS.pdf("bkgPdf")->plotOn(frame1);
  RooHist* hresid1 = frame1->residHist();
  RooHist* hpull1 = frame1->pullHist();

  RooPlot* frame2 = (*myWS.var("x")).frame();
  data.plotOn(frame2);
  myWS.pdf("bkgPdf2")->plotOn(frame2);
  RooHist* hresid2 = frame2->residHist();
  RooHist* hpull2 = frame2->pullHist();

  RooPlot* frame1_ee = (*myWS.var("x")).frame();
  data_ee.plotOn(frame1_ee);
  myWS.pdf("bkgPdf_ee")->plotOn(frame1_ee);
  RooHist* hresid1_ee = frame1_ee->residHist();
  RooHist* hpull1_ee = frame1_ee->pullHist();

  RooPlot* frame2_ee = (*myWS.var("x")).frame();
  data_ee.plotOn(frame2_ee);
  myWS.pdf("bkgPdf2_ee")->plotOn(frame2_ee);
  RooHist* hresid2_ee = frame2_ee->residHist();
  RooHist* hpull2_ee = frame2_ee->pullHist();

  myWS.import(*frame1);
  myWS.import(*hresid1);
  myWS.import(*hpull1);
  myWS.import(*frame2);
  myWS.import(*hresid2);
  myWS.import(*hpull2);

  myWS.import(*frame1_ee);
  myWS.import(*hresid1_ee);
  myWS.import(*hpull1_ee);
  myWS.import(*frame2_ee);
  myWS.import(*hresid2_ee);
  myWS.import(*hpull2_ee);

  frame1_ee->Draw();

  TFile *f = new TFile("workspace_DY.root", "RECREATE");
  myWS.Write();
  f->Write();
  */
}

void cutTTree_Mlljj(TTree* tree, TH1F* h1, float xsec){
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

    if(Mlljj>0 && Mlljj>600 && l1_pt>40 && l2_pt>40 && (l1_pt>60 || l2_pt>60) && j1_pt>40 && j2_pt>40 && Mll>200 && dR_l1l2 > 0.4 && dR_l1j1 > 0.4 && dR_l1j2 > 0.4 && dR_l2j1 > 0.4 && dR_l2j2 > 0.4 && fabs(j1_eta)<2.4 && fabs(j2_eta)<2.4){
	h1->Fill(Mlljj,reweight*xsec);
    }
  }
}
