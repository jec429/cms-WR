#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <iostream>
#include <string>

void cutTTree_Mlljj(TTree* tree, TH1F* h1, float xsec);
void FractionFit(){
  using namespace RooFit;

  TH1::SetDefaultSumw2();
  // Drell-Yan
  TString pre_name = Form("../../../miniaod_v2/dysamples_powheg/");
  TString fnames[] = {"skim_ttree_120to200.root","skim_ttree_200to400.root","skim_ttree_400to800.root","skim_ttree_800to1400.root","skim_ttree_1400to2300.root","skim_ttree_2300to3500.root","skim_ttree_3500to4500.root","skim_ttree_4500to6000.root","skim_ttree_6000toInf.root"};
  float xsecs[] = {19.32,2.731,0.241,0.01678,0.00139,0.00008948,0.000004135,4.56e-7,2.066e-8};
  TFile * tfile = new TFile("../../../miniaod_v2/skim_ttree_ttbar.root");

  TTree *treetreebar;
  treetreebar = (TTree*)tfile->Get("MakeTTree_Muons/t"); 

  std::vector<std::pair<TTree*,float>> trees;

  for (int i=0;i<9;i++) {    
    TFile * hfile = new TFile((pre_name+fnames[i]).Data());
    TTree *tree = (TTree*)hfile->Get("MakeTTree_Muons/t"); 
    trees.push_back(std::make_pair(tree,xsecs[i]));
  }

  TH1F *h_sign_Mlljj_200 = new TH1F("h_sign_Mlljj_200","",55,500,6000);
  TH1F *t_sign_Mlljj = new TH1F("t_sign_Mlljj","",55,500,6000);

  for (auto t: trees) {    
    cutTTree_Mlljj(t.first,h_sign_Mlljj_200,t.second);
  }

  cutTTree_Mlljj(treetreebar,t_sign_Mlljj,57.35);

  t_sign_Mlljj->Scale(0.7*2000/t_sign_Mlljj->Integral());
  h_sign_Mlljj_200->Scale(0.3*2000/h_sign_Mlljj_200->Integral());

  TH1F *data_Mlljj = (TH1F*) t_sign_Mlljj->Clone("data_Mlljj");
  data_Mlljj->Add(h_sign_Mlljj_200);

  RooRealVar x("x","M_{lljj}",500,6000) ;
  x.setBins(55) ;

  // The 'observed' data
  //RooDataSet* obsData = model0.generate(x,AllBinned(),NumEvents(2000)) ;  
  RooDataHist obsData("obsData","obsData",RooArgList(x),data_Mlljj);  

  // Make HistFuncs for sigma and background to serve as templates
  RooDataHist dh_sig("dh_sig","dh_sig",RooArgList(x),t_sign_Mlljj);
  RooDataHist dh_bkg("dh_bkg","dh_bkg",RooArgList(x),h_sign_Mlljj_200);

  RooRealVar Asig("Asig","Asig",1.0,0.0,1.5) ;
  RooRealVar Abkg("Abkg","Abkg",1.0,0.0,1.5) ;

  // ***** Case 0 - 'Rigid templates' *****

  // Construct histogram shapes for signal and background
  RooHistFunc p_h_sig("p_h_sig","p_h_sig",x,dh_sig) ;
  RooHistFunc p_h_bkg("p_h_bkg","p_h_bkg",x,dh_bkg) ;  

  // Construct the sum of these
  RooRealSumPdf model0("model0","model0",RooArgList(p_h_sig,p_h_bkg),RooArgList(Asig,Abkg),kTRUE) ;
 
  //model0.fitTo(obsData) ;
  Double_t asig_val0 = Asig.getVal() ; Double_t asig_err0 = Asig.getError() ;
  //model1.fitTo(*obsData) ;
  //Double_t asig_val1 = Asig.getVal() ; Double_t asig_err1 = Asig.getError() ;
  //model2.fitTo(*obsData) ;
  //Double_t asig_val2 = Asig.getVal() ; Double_t asig_err2 = Asig.getError() ;
  
  cout << "Asig [normal ] = " << asig_val0 << " +/- " << asig_err0 << endl ;
  //cout << "Asig [BB     ] = " << asig_val1 << " +/- " << asig_err1 << endl ;
  //cout << "Asig [BBlight] = " << asig_val2 << " +/- " << asig_err2 << endl ;

  RooPlot* xframe = x.frame() ;
  obsData.plotOn(xframe) ;
  model0.plotOn(xframe) ;
  p_h_sig.plotOn(xframe , LineColor(kRed)) ;
  p_h_bkg.plotOn(xframe , LineColor(kGreen)) ;
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
