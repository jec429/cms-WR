#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <iostream>
#include <string>

void cutTTree_Mlljj(TTree* tree, TH1F* h1, float xsec);
void DY_RooFit(){
  using namespace RooFit ;

  TH1::SetDefaultSumw2();
  // Drell-Yan
  TString pre_name = Form("../../../miniaod_v2/dysamples_powheg/");
  TString fnames[] = {"skim_ttree_120to200.root","skim_ttree_200to400.root","skim_ttree_400to800.root","skim_ttree_800to1400.root","skim_ttree_1400to2300.root","skim_ttree_2300to3500.root","skim_ttree_3500to4500.root","skim_ttree_4500to6000.root","skim_ttree_6000toInf.root"};
  float xsecs[] = {19.32,2.731,0.241,0.01678,0.00139,0.00008948,0.000004135,4.56e-7,2.066e-8};

  std::vector<std::pair<TTree*,float>> trees;

  for (int i=0;i<9;i++) {    
    TFile * hfile = new TFile((pre_name+fnames[i]).Data());
    TTree *tree = (TTree*)hfile->Get("MakeTTree_Muons/t"); 
    trees.push_back(std::make_pair(tree,xsecs[i]));
  }

  TH1F *h_sign_Mlljj_200 = new TH1F("h_sign_Mlljj_200","",55,500,6000);

  for (auto t: trees) {    
    cutTTree_Mlljj(t.first,h_sign_Mlljj_200,t.second);
  }

  RooRealVar x("x","M_{lljj}",500,6000) ;
  x.setBins(55) ;
  RooDataHist data("data","data",RooArgList(x),h_sign_Mlljj_200);
  
   // DYJets Fit
  RooRealVar alpha("alpha","alpha",5000.,10000.) ;
  RooRealVar beta("beta","beta",0.0,0.1) ;

  RooRealVar c0("c0","c0",0.0,10000) ;
  RooRealVar c1("c1","c1",0.0,100) ;
  RooRealVar c2("c2","c2",0.0,100) ;
  
  RooRealVar d0("d0","d0",0.0,10000) ;
  RooRealVar d1("d1","d1",0.0,100) ;
  RooRealVar d2("d2","d2",0.0,100) ;
  RooRealVar d3("d3","d3",0.0,100) ;

  RooGenericPdf f1("f1","alpha*exp(-beta*x)",RooArgSet(x,alpha,beta)) ; 
  f1.fitTo(data, SumW2Error(kTRUE), Range(700,5000));
  RooGenericPdf f2("f2","(c0*TMath::Power((1-x/13000),c1))/TMath::Power((x/13000),c2)",RooArgSet(x,c0,c1,c2)) ; 
  f2.fitTo(data, SumW2Error(kTRUE), Range(700,5000));
  RooGenericPdf f3("f3","(d0*TMath::Power((1-x/13000),d1))/TMath::Power((x/13000),(d2+d3*TMath::Log(x/13000)))",RooArgSet(x,d0,d1,d2,d3)) ; 
  f3.fitTo(data, SumW2Error(kTRUE), Range(700,5000));

  RooPlot* xframe = x.frame() ;
  data.plotOn(xframe) ;
  f1.plotOn(xframe) ;
  f2.plotOn(xframe , LineColor(kRed)) ;
  f3.plotOn(xframe , LineColor(kGreen)) ;
  xframe->Draw() ;

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
