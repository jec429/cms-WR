#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <iostream>
#include <string>

void cutTTree_Mlljj(TTree* tree, TH1F* h1, float xsec);
void generateHisto_Mlljj(TH1F* h1);
std::vector<TH1F*> makeHisto_Mlljj();
void pseudoData_Mlljj(TH1F* h_pdata,TH1F* h_template, Int_t nevents); // Throw random from input histo
Double_t fit_piece(Double_t *x, Double_t *par);
TFractionFitter* Fit(TH1F* data,TH1F* mc0,TH1F* mc1);

void newFit(){
  gStyle->SetOptFit(1111);
  std::vector<TH1F*> templates = makeHisto_Mlljj();            // ttbar, DY, ttbar_emu
  std::vector<Double_t> values;
  std::vector<Double_t> errors;
  //TH1F *data = new TH1F("data","",55,500,6000);                // data histogram
  TH1F *mc0 = new TH1F("mc0","",55,500,6000);                  // DY function histogram
  TH1F *mc1 = new TH1F("mc1","",55,500,6000);;                 // ttbar emu MC histogram  
 
  TH1F *gc0 = new TH1F("gc0","",55,500,6000);                  // DY MC histogram
  TH1F *gc1 = new TH1F("gc1","",55,500,6000);;                 // ttbar MC histogram  

  //data->Smooth(10);
  //mc1->Smooth(10);

  for(int i=0; i<10000; i++){
    pseudoData_Mlljj(gc0,templates[0],300);
    pseudoData_Mlljj(gc1,templates[1],700);
    generateHisto_Mlljj(mc0);
    pseudoData_Mlljj(mc1,templates[2],1000); 

    TH1F *data = (TH1F*)gc0->Clone();
    data->Add(gc1);

    TFractionFitter* fit = Fit(data,mc0,mc1);
    if(fit == 0)
      continue;
    //TH1F* result = (TH1F*) fit->GetPlot();
    //TCanvas *c_new = new TCanvas("c_new","",200,10,700,500);
    //result->SetLineColor(kRed);
    //data->Draw();
    //result->Draw("same"); 
    mc0->SetLineColor(kGreen);
    mc1->SetLineColor(kMagenta);
    //mc0->Draw("same");
    //mc1->Draw("same");
    Double_t value, error;
    fit->GetResult(0,value,error);
    values.push_back(value);
    errors.push_back(error);
    data->Reset();
    mc0->Reset();
    mc1->Reset();
    gc0->Reset();
    gc1->Reset();
  }

  TH1F *fraction = new TH1F("fraction","",100,-1.0,1.0);
  TH1F *pull = new TH1F("pull","",100,-5.0,5.0);

  vector<Double_t>::iterator ItA = values.begin();
  vector<Double_t>::iterator ItB = errors.begin();
  while(true)
    {
      pull->Fill((*ItA-0.3)/ *ItB);
      fraction->Fill((*ItA-0.3)/ 0.3);
      if(ItA != values.end())
	++ItA;
      if(ItB != errors.end())
	++ItB;
      if(ItA == values.end() && ItB == errors.end())
	break;
    }

  TCanvas *c1 = new TCanvas("c1","",200,10,700,500);
  fraction->Draw();
  fraction->Fit("gaus");
  TCanvas *c2 = new TCanvas("c2","",200,10,700,500);
  pull->Draw();
  pull->Fit("gaus");
  
}

TFractionFitter* Fit(TH1F* data,TH1F* mc0,TH1F* mc1){
  TObjArray *mc = new TObjArray(3);        //MC histograms are put in this array
  mc->Add(mc0);
  mc->Add(mc1);
  TFractionFitter* fit = new TFractionFitter(data, mc, "Q"); // initialise  
  //fit->Constrain(1, 0.0, 1.0);               // constrain fraction 1 to be between 0 and 1
  fit->SetRangeX(3,50);                    // use only the first 13 bins in the fit
  Int_t status = fit->Fit();               // perform the fit
  //std::cout << "fit status: " << status << std::endl;
  if(status == 0)  
    return fit;
  else
    return 0;
}
void pseudoData_Mlljj(TH1F* h_pdata,TH1F* h_template, Int_t nevents){
  h_pdata->FillRandom(h_template,nevents);
}
void generateHisto_Mlljj(TH1F* h1){
  TF1* my_func = new TF1("my_func","exp(-0.00307293*x)");
  TF1* my_func2 = new TF1("my_func2",fit_piece,500,6000,4);
  my_func2->SetParameter(0,8.88759e+03);
  my_func2->SetParameter(1,-3.29987e-03);
  my_func2->SetParameter(2,2.22074e+03);
  my_func2->SetParameter(3,-2.65155e-03);
  h1->FillRandom("my_func",100000);
}
Double_t fit_piece(Double_t *x, Double_t *par){
  Double_t xv =x[0];
  Double_t f =  (xv<2500)*(par[0]*exp(par[1]*xv)) + (xv>=2500)*(par[2]*exp(par[3]*xv));
  return f;
}

std::vector<TH1F*> makeHisto_Mlljj(){
  Int_t samples = 0;
  TH1::SetDefaultSumw2();
  // Drell-Yan
  TString pre_name = Form("miniaod_v2/dysamples_powheg/");
  TString fnames[] = {"skim_ttree_120to200.root","skim_ttree_200to400.root","skim_ttree_400to800.root","skim_ttree_800to1400.root","skim_ttree_1400to2300.root","skim_ttree_2300to3500.root","skim_ttree_3500to4500.root","skim_ttree_4500to6000.root","skim_ttree_6000toInf.root"};
  float xsecs[] = {19.32,2.731,0.241,0.01678,0.00139,0.00008948,0.000004135,4.56e-7,2.066e-8};
  
  TString fnames_1[] = {"skim_ttree_120to200_1.root","skim_ttree_200to400_1.root","skim_ttree_400to800_1.root","skim_ttree_800to1400_1.root","skim_ttree_1400to2300_1.root","skim_ttree_2300to3500_1.root","skim_ttree_3500to4500_1.root","skim_ttree_4500to6000_1.root","skim_ttree_6000toInf_1.root"};
  TString fnames_2[] = {"skim_ttree_120to200_1_2.root","skim_ttree_200to400_1_2.root","skim_ttree_400to800_1_2.root","skim_ttree_800to1400_1_2.root","skim_ttree_1400to2300_1_2.root","skim_ttree_2300to3500_1_2.root","skim_ttree_3500to4500_1_2.root","skim_ttree_4500to6000_1_2.root","skim_ttree_6000toInf_1_2.root"};
  TString fnames_3[] = {"skim_ttree_120to200_1_2_3.root","skim_ttree_200to400_1_2_3.root","skim_ttree_400to800_1_2_3.root","skim_ttree_800to1400_1_2_3.root","skim_ttree_1400to2300_1_2_3.root","skim_ttree_2300to3500_1_2_3.root","skim_ttree_3500to4500_1_2_3.root","skim_ttree_4500to6000_1_2_3.root","skim_ttree_6000toInf_1_2_3.root"};

  std::vector<std::pair<TTree*,float>> trees;

  for (int i=0;i<9;i++) {
    if(samples == 0){
      TFile * hfile = new TFile((pre_name+fnames[i]).Data());
      TTree *tree = (TTree*)hfile->Get("MakeTTree_Muons/t"); 
      trees.push_back(std::make_pair(tree,xsecs[i]));
    }
    else if(samples == 1){
      TFile * hfile = new TFile((pre_name+fnames_1[i]).Data());
      TTree *tree = (TTree*)hfile->Get("t"); 
      trees.push_back(std::make_pair(tree,xsecs[i]));
    }
    else if(samples == 2){
      TFile * hfile = new TFile((pre_name+fnames_2[i]).Data());
      TTree *tree = (TTree*)hfile->Get("t"); 
      trees.push_back(std::make_pair(tree,xsecs[i]));
    }
    else if(samples == 3){
      TFile * hfile = new TFile((pre_name+fnames_3[i]).Data());
      TTree *tree = (TTree*)hfile->Get("t"); 
      trees.push_back(std::make_pair(tree,xsecs[i]));
    }
  }
  // TTbar
  TFile * tfile = new TFile("miniaod_v2/skim_ttree_ttbar.root");
  TFile * tfile_emu = new TFile("miniaod_v2/skim_ttree_emu_ttbar.root");
  TFile * tfile_1 = new TFile("miniaod_v2/skim_ttree_ttbar_1.root");
  TFile * tfile_emu_1 = new TFile("miniaod_v2/skim_ttree_emu_ttbar_1.root");
  TFile * tfile_2 = new TFile("miniaod_v2/skim_ttree_ttbar_2.root");
  TFile * tfile_emu_2 = new TFile("miniaod_v2/skim_ttree_emu_ttbar_2.root");
  TFile * tfile_3 = new TFile("miniaod_v2/skim_ttree_ttbar_3.root");
  TFile * tfile_emu_3 = new TFile("miniaod_v2/skim_ttree_emu_ttbar_3.root");

  TTree *treetreebar;
  TTree *treetreebar_emu;

  if(samples == 0){
    treetreebar = (TTree*)tfile->Get("MakeTTree_Muons/t"); 
    treetreebar_emu = (TTree*)tfile_emu->Get("MakeTTree_EleMuon/t"); 
  }
  else if(samples == 1){
    treetreebar = (TTree*)tfile_1->Get("t"); 
    treetreebar_emu = (TTree*)tfile_emu_1->Get("t"); 
  }
  else if(samples == 2){
    treetreebar = (TTree*)tfile_2->Get("t"); 
    treetreebar_emu = (TTree*)tfile_emu_2->Get("t"); 
  }
  else if(samples == 3){
    treetreebar = (TTree*)tfile_3->Get("t"); 
    treetreebar_emu = (TTree*)tfile_emu_3->Get("t"); 
  }

  // HISTOGRAMS //
  // Drell-Yan
  TH1F *h_sign_Mlljj = new TH1F("h_sign_Mlljj","",55,500,6000);

  // TTbar
  TH1F *t_sign_Mlljj = new TH1F("t_sign_Mlljj","",55,500,6000);
  TH1F *t_sign_emu_Mlljj = new TH1F("t_sign_emu_Mlljj","",55,500,6000);

  std::cout<<"Drell-Yan"<<std::endl;
  for (auto t: trees) {    
    cutTTree_Mlljj(t.first,h_sign_Mlljj,t.second);
  }
  if(samples == 0)
    h_sign_Mlljj->Scale(2460./100000.);
  else
    h_sign_Mlljj->Scale(3*2460./100000.);

  std::cout<<"ttbar"<<std::endl;
  cutTTree_Mlljj(treetreebar,t_sign_Mlljj,57.35);

  std::cout<<"ttbar emu"<<std::endl;
  cutTTree_Mlljj(treetreebar_emu,t_sign_emu_Mlljj,1.0);         // Raw histogram


  if(samples == 0){
    //t_sign_emu_Mll->Scale(0.7*2460/(30479698));
    t_sign_Mlljj->Scale(2460./(30479698));
  }
  else {
    //t_sign_emu_Mll->Scale(3*0.7*2460/(30479698));
    t_sign_Mlljj->Scale(3*2460./(30479698));
  }

  cout<<"Integrals:"<<endl;
  cout<<h_sign_Mlljj->Integral(3,50)<< " " <<  t_sign_Mlljj->Integral(3,50) << endl;
  cout<<h_sign_Mlljj->Integral()<< " " <<  t_sign_Mlljj->Integral() << endl;

  //TCanvas *c0 = new TCanvas("c0","",200,10,700,500);  
  //t_sign_Mlljj->SetLineColor(kRed);
  //t_sign_Mlljj->Draw();
  //h_sign_Mlljj->Draw("same");

  //t_sign_Mlljj->Add(h_sign_Mlljj);
  
  std::vector<TH1F*> templates;
  templates.push_back(h_sign_Mlljj);
  templates.push_back(t_sign_Mlljj);
  templates.push_back(t_sign_emu_Mlljj);

  return templates;
}


void cutTTree_Mlljj(TTree* tree, TH1F* h1, float xsec){
  
  //h1->Sumw2();

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

    if(Mlljj>0 && Mlljj>600 && l1_pt>40 && l2_pt>40 && (l1_pt>60 || l2_pt>60) && j1_pt>40 && j2_pt>40 && Mll>120 && Mll>200 && dR_l1l2 > 0.4 && dR_l1j1 > 0.4 && dR_l1j2 > 0.4 && dR_l2j1 > 0.4 && dR_l2j2 > 0.4 && fabs(j1_eta)<2.4 && fabs(j2_eta)<2.4){
      if(xsec != 1.0)
	h1->Fill(Mlljj,reweight*xsec);
      else
	h1->Fill(Mlljj);
    }
  }
}
