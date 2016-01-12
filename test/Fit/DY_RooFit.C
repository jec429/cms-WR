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

  TH1F *h_sign_Mlljj_200 = new TH1F("h_sign_Mlljj_200","",44,600,5000);

  for (auto t: trees) {    
    cutTTree_Mlljj(t.first,h_sign_Mlljj_200,t.second);
  }

  RooRealVar x("x","M_{lljj}",600,5000) ;
  x.setBins(44) ;
  RooDataHist data("data","data",RooArgList(x),h_sign_Mlljj_200);
  
   // DYJets Fit
  RooRealVar alpha("alpha","alpha",4000.,8000.) ;
  RooRealVar beta("beta","beta",0.0,0.1) ;

  RooRealVar c0("c0","c0",90.,100.) ;
  RooRealVar c1("c1","c1",20.0,30) ;
  RooRealVar c2("c2","c2",0.0,5) ;
  
  RooRealVar d0("d0","d0",10.0,20) ;
  RooRealVar d1("d1","d1",15.0,25) ;
  RooRealVar d2("d2","d2",2.0,5) ;
  RooRealVar d3("d3","d3",0.0,1) ;

  RooRealVar e0("e0","e0",4000.,8000.) ;
  RooRealVar e1("e1","e1",0.0,0.1) ;

  RooRealVar beta2("beta2","beta2",-0.1,0.1) ;

  RooRealVar g0("g0","g0",-0.1,0.1) ;
  RooRealVar g1("g1","g1",-0.1,0.1) ;
  RooRealVar g2("g2","g2",0,100000) ;
  RooRealVar g3("g3","g3",0,100000) ;
  

  RooGenericPdf f1("f1","exp(-beta*x)",RooArgSet(x,beta)) ; 
  //f1.fitTo(data, SumW2Error(kTRUE), Range(700,5000));
  f1.fitTo(data, SumW2Error(kTRUE), PrintLevel(3));
  RooExponential model("model","model",x,beta2) ;
  model.fitTo(data, SumW2Error(kTRUE), PrintLevel(3));
  RooGenericPdf f2("f2","(c0*TMath::Power((1-x/13000),c1))/TMath::Power((x/13000),c2)",RooArgSet(x,c0,c1,c2)) ; 
  //f2.fitTo(data, SumW2Error(kTRUE), Range(700,5000));
  f2.fitTo(data, SumW2Error(kTRUE), PrintLevel(3));
  RooGenericPdf f3("f3","(d0*TMath::Power((1-x/13000),d1))/TMath::Power((x/13000),(d2+d3*TMath::Log(x/13000)))",RooArgSet(x,d0,d1,d2,d3)) ; 
  //f3.fitTo(data, SumW2Error(kTRUE), Range(700,5000));
  f3.fitTo(data, SumW2Error(kTRUE), PrintLevel(3));
  RooGenericPdf f4("f4","e0*exp(-e1*x^0.9)",RooArgSet(x,e0,e1)) ; 
  //f4.fitTo(data, SumW2Error(kTRUE), Range(700,5000));
  f4.fitTo(data, SumW2Error(kTRUE), PrintLevel(3));

  RooExponential exp0("exp0","exp0",x,g0) ;
  RooExponential exp1("exp1","exp1",x,g1) ;
  RooRealSumPdf sum_exp("sum_exp","sum_exp",RooArgList(exp0,exp1),RooArgList(g2,g3),kTRUE) ;
  sum_exp.fitTo(data, SumW2Error(kTRUE), PrintLevel(3));

  RooPlot* frame11 = x.frame() ;
  data.plotOn(frame11) ;
  f1.plotOn(frame11) ;
  //f2.plotOn(xframe , LineColor(kRed)) ;
  //f3.plotOn(xframe , LineColor(kGreen)) ;
  //f4.plotOn(xframe , LineColor(kCyan)) ;
  //model.plotOn(frame51 , LineColor(kBlack)) ;

  RooHist* hresid1 = frame11->residHist() ;
  RooHist* hpull1 = frame11->pullHist() ;

  RooPlot* frame12 = x.frame(Title("Residual Distribution")) ;
  frame12->addPlotable(hresid1,"P") ;

  RooPlot* frame13 = x.frame(Title("Pull Distribution")) ;
  frame13->addPlotable(hpull1,"P") ;

  TCanvas* ca1 = new TCanvas("ca1","ca1",900,300) ;
  ca1->Divide(3) ;
  ca1->cd(1) ; gPad->SetLeftMargin(0.15) ; frame11->GetYaxis()->SetTitleOffset(1.6) ; frame11->Draw() ;
  ca1->cd(2) ; gPad->SetLeftMargin(0.15) ; frame12->GetYaxis()->SetTitleOffset(1.6) ; frame12->Draw() ;
  ca1->cd(3) ; gPad->SetLeftMargin(0.15) ; frame13->GetYaxis()->SetTitleOffset(1.6) ; frame13->Draw() ;

  RooPlot* frame21 = x.frame() ;
  data.plotOn(frame21) ;
  f2.plotOn(frame21) ;
  //f2.plotOn(xframe , LineColor(kRed)) ;
  //f3.plotOn(xframe , LineColor(kGreen)) ;
  //f4.plotOn(xframe , LineColor(kCyan)) ;
  //model.plotOn(frame51 , LineColor(kBlack)) ;

  RooHist* hresid2 = frame21->residHist() ;
  RooHist* hpull2 = frame21->pullHist() ;

  RooPlot* frame22 = x.frame(Title("Residual Distribution")) ;
  frame22->addPlotable(hresid2,"P") ;

  RooPlot* frame23 = x.frame(Title("Pull Distribution")) ;
  frame23->addPlotable(hpull2,"P") ;

  TCanvas* ca2 = new TCanvas("ca2","ca2",900,300) ;
  ca2->Divide(3) ;
  ca2->cd(1) ; gPad->SetLeftMargin(0.15) ; frame21->GetYaxis()->SetTitleOffset(1.6) ; frame21->Draw() ;
  ca2->cd(2) ; gPad->SetLeftMargin(0.15) ; frame22->GetYaxis()->SetTitleOffset(1.6) ; frame22->Draw() ;
  ca2->cd(3) ; gPad->SetLeftMargin(0.15) ; frame23->GetYaxis()->SetTitleOffset(1.6) ; frame23->Draw() ;

  RooPlot* frame31 = x.frame() ;
  data.plotOn(frame31) ;
  f3.plotOn(frame31) ;
  //f2.plotOn(xframe , LineColor(kRed)) ;
  //f3.plotOn(xframe , LineColor(kGreen)) ;
  //f4.plotOn(xframe , LineColor(kCyan)) ;
  //model.plotOn(frame51 , LineColor(kBlack)) ;

  RooHist* hresid3 = frame31->residHist() ;
  RooHist* hpull3 = frame31->pullHist() ;

  RooPlot* frame32 = x.frame(Title("Residual Distribution")) ;
  frame32->addPlotable(hresid3,"P") ;

  RooPlot* frame33 = x.frame(Title("Pull Distribution")) ;
  frame33->addPlotable(hpull3,"P") ;

  TCanvas* ca3 = new TCanvas("ca3","ca3",900,300) ;
  ca3->Divide(3) ;
  ca3->cd(1) ; gPad->SetLeftMargin(0.15) ; frame31->GetYaxis()->SetTitleOffset(1.6) ; frame31->Draw() ;
  ca3->cd(2) ; gPad->SetLeftMargin(0.15) ; frame32->GetYaxis()->SetTitleOffset(1.6) ; frame32->Draw() ;
  ca3->cd(3) ; gPad->SetLeftMargin(0.15) ; frame33->GetYaxis()->SetTitleOffset(1.6) ; frame33->Draw() ;

  RooPlot* frame41 = x.frame() ;
  data.plotOn(frame41) ;
  f4.plotOn(frame41) ;
  //f2.plotOn(xframe , LineColor(kRed)) ;
  //f3.plotOn(xframe , LineColor(kGreen)) ;
  //f4.plotOn(xframe , LineColor(kCyan)) ;
  //model.plotOn(frame51 , LineColor(kBlack)) ;

  RooHist* hresid4 = frame41->residHist() ;
  RooHist* hpull4 = frame41->pullHist() ;

  RooPlot* frame42 = x.frame(Title("Residual Distribution")) ;
  frame42->addPlotable(hresid4,"P") ;

  RooPlot* frame43 = x.frame(Title("Pull Distribution")) ;
  frame43->addPlotable(hpull4,"P") ;

  TCanvas* ca4 = new TCanvas("ca4","ca4",900,300) ;
  ca4->Divide(3) ;
  ca4->cd(1) ; gPad->SetLeftMargin(0.15) ; frame41->GetYaxis()->SetTitleOffset(1.6) ; frame41->Draw() ;
  ca4->cd(2) ; gPad->SetLeftMargin(0.15) ; frame42->GetYaxis()->SetTitleOffset(1.6) ; frame42->Draw() ;
  ca4->cd(3) ; gPad->SetLeftMargin(0.15) ; frame43->GetYaxis()->SetTitleOffset(1.6) ; frame43->Draw() ;

  RooPlot* frame51 = x.frame() ;
  data.plotOn(frame51) ;
  //f1.plotOn(xframe) ;
  //f2.plotOn(xframe , LineColor(kRed)) ;
  //f3.plotOn(xframe , LineColor(kGreen)) ;
  //f4.plotOn(xframe , LineColor(kCyan)) ;
  model.plotOn(frame51) ;

  RooHist* hresid5 = frame51->residHist() ;
  RooHist* hpull5 = frame51->pullHist() ;

  RooPlot* frame52 = x.frame(Title("Residual Distribution")) ;
  frame52->addPlotable(hresid5,"P") ;

  RooPlot* frame53 = x.frame(Title("Pull Distribution")) ;
  frame53->addPlotable(hpull5,"P") ;

  TCanvas* ca5 = new TCanvas("ca5","ca5",900,300) ;
  ca5->Divide(3) ;
  ca5->cd(1) ; gPad->SetLeftMargin(0.15) ; frame51->GetYaxis()->SetTitleOffset(1.6) ; frame51->Draw() ;
  ca5->cd(2) ; gPad->SetLeftMargin(0.15) ; frame52->GetYaxis()->SetTitleOffset(1.6) ; frame52->Draw() ;
  ca5->cd(3) ; gPad->SetLeftMargin(0.15) ; frame53->GetYaxis()->SetTitleOffset(1.6) ; frame53->Draw() ;

  RooPlot* frame61 = x.frame() ;
  data.plotOn(frame61) ;
  //f1.plotOn(xframe) ;
  //f2.plotOn(xframe , LineColor(kRed)) ;
  //f3.plotOn(xframe , LineColor(kGreen)) ;
  //f4.plotOn(xframe , LineColor(kCyan)) ;
  sum_exp.plotOn(frame61) ;

  RooHist* hresid6 = frame61->residHist() ;
  RooHist* hpull6 = frame61->pullHist() ;

  RooPlot* frame62 = x.frame(Title("Residual Distribution")) ;
  frame62->addPlotable(hresid6,"P") ;

  RooPlot* frame63 = x.frame(Title("Pull Distribution")) ;
  frame63->addPlotable(hpull6,"P") ;

  TCanvas* ca6 = new TCanvas("ca6","ca6",900,300) ;
  ca6->Divide(3) ;
  ca6->cd(1) ; gPad->SetLeftMargin(0.15) ; frame61->GetYaxis()->SetTitleOffset(1.6) ; frame61->Draw() ;
  ca6->cd(2) ; gPad->SetLeftMargin(0.15) ; frame62->GetYaxis()->SetTitleOffset(1.6) ; frame62->Draw() ;
  ca6->cd(3) ; gPad->SetLeftMargin(0.15) ; frame63->GetYaxis()->SetTitleOffset(1.6) ; frame63->Draw() ;

  cout << "chi^2 = " << frame11->chiSquare(2) << endl ;
  cout << "chi^2 = " << frame21->chiSquare() << endl ;
  cout << "chi^2 = " << frame31->chiSquare() << endl ;
  cout << "chi^2 = " << frame41->chiSquare() << endl ;
  cout << "chi^2 = " << frame51->chiSquare() << endl ;
  cout << "chi^2 = " << frame61->chiSquare() << endl ;

  /*
  TF1 *r_f1=(TF1*)f1.asTF( x );
  TF1 *r_f2=(TF1*)f2.asTF( x );
  TF1 *r_f3=(TF1*)f3.asTF( x );
  TF1 *r_f4=(TF1*)f4.asTF( x );
  cout<<r_f1->Eval(1000)<<endl;

  
  // TF1* r_f1 = new TF1("r_f1","[0]*exp(-[1]*x)",700,5000);
  // r_f1->SetParameter(0,alpha.getVal());
  // r_f1->SetParameter(1,beta.getVal());
  // TF1* r_f2 = new TF1("r_f2","([0]*(1-x/13000)^[1])/(x/13000)^[2]",700,5000);
  // r_f2->SetParameter(0,c0.getVal());
  // r_f2->SetParameter(1,c1.getVal());
  // r_f2->SetParameter(2,c2.getVal());
  // TF1* r_f3 = new TF1("r_f3","([0]*(1-x/13000)^[1])/(x/13000)^([2]+[3]*TMath::Log(x/13000))",700,5000);
  // r_f3->SetParameter(0,d0.getVal());
  // r_f3->SetParameter(1,d1.getVal());
  // r_f3->SetParameter(2,d2.getVal());
  // r_f3->SetParameter(3,d3.getVal());

   TH1F *res_f1 = new TH1F("res_f1","",44,600,5000);
   TH1F *res_f2 = new TH1F("res_f2","",44,600,5000);
   TH1F *res_f3 = new TH1F("res_f3","",44,600,5000);
   TH1F *res_f4 = new TH1F("res_f4","",44,600,5000);
   for(int i=1;i<44;i++){  
     res_f1->SetBinContent(i,(h_sign_Mlljj_200->GetBinContent(i)-r_f1->Eval(h_sign_Mlljj_200->GetXaxis()->GetBinCenter(i)))/r_f1->Eval(h_sign_Mlljj_200->GetXaxis()->GetBinCenter(i)));
     res_f2->SetBinContent(i,(h_sign_Mlljj_200->GetBinContent(i)-r_f2->Eval(h_sign_Mlljj_200->GetXaxis()->GetBinCenter(i)))/r_f2->Eval(h_sign_Mlljj_200->GetXaxis()->GetBinCenter(i)));
     res_f3->SetBinContent(i,(h_sign_Mlljj_200->GetBinContent(i)-r_f3->Eval(h_sign_Mlljj_200->GetXaxis()->GetBinCenter(i)))/r_f3->Eval(h_sign_Mlljj_200->GetXaxis()->GetBinCenter(i)));    
     res_f4->SetBinContent(i,(h_sign_Mlljj_200->GetBinContent(i)-r_f4->Eval(h_sign_Mlljj_200->GetXaxis()->GetBinCenter(i)))/r_f4->Eval(h_sign_Mlljj_200->GetXaxis()->GetBinCenter(i)));    
   }
  
  // //h_sign_Mlljj_200->Fit(r_f2,"RL");

  TCanvas *canvas = new TCanvas("canvas","",200,10,700,500);
  //r_f1->GetYaxis()->SetRangeUser(-10,10);
  h_sign_Mlljj_200->Draw();
  //r_f1->SetLineColor(kBlue);
  //r_f1->Draw("same");
  //r_f2->SetLineColor(kRed);
  //r_f2->Draw("same");
  //r_f3->SetLineColor(kGreen);
  //r_f3->Draw("same");
  TCanvas *canvas2 = new TCanvas("canvas2","",200,10,700,500);
  res_f1->GetYaxis()->SetRangeUser(-100,100);
  res_f1->Draw();
  res_f2->SetLineColor(kRed);
  res_f2->Draw("same");
  res_f3->SetLineColor(kGreen);
  res_f3->Draw("same");
  res_f4->SetLineColor(kCyan);
  res_f4->Draw("same");
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
