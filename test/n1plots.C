#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <iostream>
#include <string>

vector<TH1F*> MakeNHistos(TString hname, int n, int bins, float x_min, float x_max){
  vector<TH1F*> NHistos;
  for(int i=0; i<n; i++)
    {
      TString full_name = Form("%s_%d",hname.Data(),i);      
      TH1F *h = new TH1F(full_name.Data(),"",bins,x_min,x_max);
      NHistos.push_back(h);
    }

  return NHistos;

}

void Fill_Histo(std::vector<TH1F*> h1, TTree* tree){  
  
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
  std::vector<bool>* isGlobal;
  std::vector<int> numberOfValidMuonHits;
  std::vector<int> numberOfMatchedStations;
  std::vector<float> sigmapt;
  std::vector<float> dxy;
  std::vector<float> dz;
  std::vector<int> numberOfValidPixelHits;
  std::vector<int> trackerLayersWithMeasurement;
  Float_t weight;
  Float_t angle3D;
		   
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

  //TBranch* isGlobal;

  //tree->SetBranchAddress("isGlobal",&isGlobal);
    /*tree->SetBranchAddress("numberOfValidMuonHits",&numberOfValidMuonHits);
  tree->SetBranchAddress("numberOfMatchedStations",&numberOfMatchedStations);
  tree->SetBranchAddress("sigmapt",&sigmapt);
  tree->SetBranchAddress("dxy",&dxy);
  tree->SetBranchAddress("dz",&dz);
  tree->SetBranchAddress("numberOfValidPixelHits",&numberOfValidPixelHits);
  tree->SetBranchAddress("trackerLayersWithMeasurement",&trackerLayersWithMeasurement);
  */
  tree->SetBranchAddress("angle3D",&angle3D);
  tree->SetBranchAddress("weight",&weight);

  for (Int_t ev = 0; ev < nentries; ev++) {
    tree->GetEntry(ev); 
    if(l1_pt>60 && l2_pt>50 && j1_pt>40 && j2_pt>40 && Mll>200 && Mlljj>600 && dR_l1l2>0.1 && dR_l1j1>0.4 && dR_l1j2>0.4 && dR_l2j1>0.4 && dR_l2j2 > 0.4)
      {
	h1[0]->Fill(Mlljj);
	h1[1]->Fill(Mll);
	h1[2]->Fill(l1_pt);
	h1[3]->Fill(l2_pt);
	h1[4]->Fill(j1_pt);
	h1[5]->Fill(j2_pt);
	h1[6]->Fill(l1_eta);
	h1[7]->Fill(l2_eta);
	h1[8]->Fill(j1_eta);
	h1[9]->Fill(j2_eta);
	h1[10]->Fill(l1_phi);
	h1[11]->Fill(l2_phi);
	h1[12]->Fill(j1_phi);
	h1[13]->Fill(j2_phi);	
	h1[14]->Fill(nleptons);	
	h1[15]->Fill(njets);	
	h1[16]->Fill(nvertices);	
	h1[17]->Fill(dR_l1l2);
	h1[18]->Fill(dR_j1j2);
	h1[19]->Fill(dR_l1j1);
	h1[20]->Fill(dR_l1j2);
	h1[21]->Fill(dR_l2j1);
	h1[22]->Fill(dR_l2j2);
	/*
	if(isGlobal.size() > 0){
	  if((isGlobal)[0])
	    h1[23]->Fill(1);
	  else
	    h1[23]->Fill(0);	  
	  h1[25]->Fill((numberOfValidMuonHits)[0]);
	  h1[27]->Fill((numberOfMatchedStations)[0]);
	  h1[29]->Fill((sigmapt)[0]);
	  h1[31]->Fill((dxy)[0]);
	  h1[33]->Fill((dz)[0]);
	  h1[35]->Fill((numberOfValidPixelHits)[0]);
	  h1[37]->Fill((trackerLayersWithMeasurement)[0]);
	}
	if(isGlobal.size() > 1){
	  if((isGlobal)[1])
	    h1[24]->Fill(1);
	  else
	    h1[24]->Fill(0);
	  h1[26]->Fill((numberOfValidMuonHits)[1]);
	  h1[28]->Fill((numberOfMatchedStations)[1]);
	  h1[30]->Fill((sigmapt)[1]);
	  h1[32]->Fill((dxy)[1]);
	  h1[34]->Fill((dz)[1]);
	  h1[36]->Fill((numberOfValidPixelHits)[1]);
	  h1[38]->Fill((trackerLayersWithMeasurement)[1]);
	}
	*/
	h1[39]->Fill(log10(TMath::Pi() - angle3D));	
      }
  }
}

void n1plots(){
  std::vector<TTree*> trees ;
  std::vector<std::vector<TH1F*>> histos;
  int nhistos = 40;

  std::vector<TH1F*> h_Mlljj = MakeNHistos("h_Mlljj",7,50,0,6000);
  std::vector<TH1F*> h_Mll = MakeNHistos("h_Mll",7,50,50,3000);
  std::vector<TH1F*> h_l1pt = MakeNHistos("h_l1pt",7,30,0,400);
  std::vector<TH1F*> h_l2pt = MakeNHistos("h_l2pt",7,30,0,200);
  std::vector<TH1F*> h_j1pt = MakeNHistos("h_j1pt",7,30,0,500);
  std::vector<TH1F*> h_j2pt = MakeNHistos("h_j2pt",7,30,0,500);
  std::vector<TH1F*> h_l1eta = MakeNHistos("h_l1eta", 7,30,-3.,3.);
  std::vector<TH1F*> h_l2eta = MakeNHistos("h_l2eta", 7,30,-3.,3.);
  std::vector<TH1F*> h_j1eta = MakeNHistos("h_j1eta", 7,30,-3.,3.);
  std::vector<TH1F*> h_j2eta = MakeNHistos("h_j2eta", 7,30,-3.,3.);
  std::vector<TH1F*> h_l1phi = MakeNHistos("h_l1phi", 7,30,-3.15,3.15);
  std::vector<TH1F*> h_l2phi = MakeNHistos("h_l2phi", 7,30,-3.15,3.15);
  std::vector<TH1F*> h_j1phi = MakeNHistos("h_j1phi", 7,30,-3.15,3.15);
  std::vector<TH1F*> h_j2phi = MakeNHistos("h_j2phi", 7,30,-3.15,3.15);
  std::vector<TH1F*> h_nleptons = MakeNHistos("h_nleptons", 7,10,0,10);
  std::vector<TH1F*> h_njets = MakeNHistos("h_njets", 7,25,0,25);
  std::vector<TH1F*> h_nvertices = MakeNHistos("h_nvertices", 7,50,0,50);
  std::vector<TH1F*> h_dR_l1l2 = MakeNHistos("h_dR_l1l2", 7,20,0,5);
  std::vector<TH1F*> h_dR_j1j2 = MakeNHistos("h_dR_j1j2", 7,20,0,5);
  std::vector<TH1F*> h_dR_l1j1 = MakeNHistos("h_dR_l1j1", 7,20,0,5);
  std::vector<TH1F*> h_dR_l1j2 = MakeNHistos("h_dR_l1j2", 7,20,0,5);
  std::vector<TH1F*> h_dR_l2j1 = MakeNHistos("h_dR_l2j1", 7,20,0,5);
  std::vector<TH1F*> h_dR_l2j2 = MakeNHistos("h_dR_l2j2", 7,20,0,5);
  // Muon ID
  std::vector<TH1F*> h_isGlobal_0 = MakeNHistos("h_isGlobal_0", 7,2,0,2);
  std::vector<TH1F*> h_isGlobal_1 = MakeNHistos("h_isGlobal_1", 7,2,0,2);
  std::vector<TH1F*> h_numberofhits_0 = MakeNHistos("h_numberofhits_0", 7,30,0,30);
  std::vector<TH1F*> h_numberofhits_1 = MakeNHistos("h_numberofhits_1", 7,30,0,30);
  std::vector<TH1F*> h_numberofstations_0 = MakeNHistos("h_numberofstations_0", 7,6,0,6);
  std::vector<TH1F*> h_numberofstations_1 = MakeNHistos("h_numberofstations_1", 7,6,0,6);
  std::vector<TH1F*> h_sigmapt_0 = MakeNHistos("h_sigmapt_0", 7,50,0,0.5);
  std::vector<TH1F*> h_sigmapt_1 = MakeNHistos("h_sigmapt_1", 7,50,0,0.5);
  std::vector<TH1F*> h_dxy_0 = MakeNHistos("h_dxy_0", 7,50,0,0.1);
  std::vector<TH1F*> h_dxy_1 = MakeNHistos("h_dxy_1", 7,50,0,0.1);
  std::vector<TH1F*> h_dz_0 = MakeNHistos("h_dz_0", 7,50,0,0.1);
  std::vector<TH1F*> h_dz_1 = MakeNHistos("h_dz_1", 7,50,0,0.1);
  std::vector<TH1F*> h_numberofpixels_0 = MakeNHistos("h_numberofpixels_0", 7,10,0,10);
  std::vector<TH1F*> h_numberofpixels_1 = MakeNHistos("h_numberofpixels_1", 7,10,0,10);
  std::vector<TH1F*> h_numberoflayers_0 = MakeNHistos("h_numberoflayers_0", 7,20,0,20);
  std::vector<TH1F*> h_numberoflayers_1 = MakeNHistos("h_numberoflayers_1", 7,20,0,20);

  std::vector<TH1F*> h_angle3D = MakeNHistos("h_angle3D", 7,50,-1,1);

  for(int j = 0; j<6; j++){
    TString full_name = Form("~/work/WR_skims/LowerPt/skim_ttree_signal_%d.root",2000+200*j);
    //std::cout<<full_name<<std::endl;
    TFile * hfile = new TFile(full_name.Data());
    TTree *tree = (TTree*)hfile->Get("MakeTTree_Muons/t");  
    std::vector<TH1F*> hs(nhistos);
    trees.push_back(tree);
    histos.push_back(hs);
    histos[j][0] = h_Mlljj[j];
    histos[j][1] = h_Mll[j];
    histos[j][2] = h_l1pt[j];
    histos[j][3] = h_l2pt[j];
    histos[j][4] = h_j1pt[j];
    histos[j][5] = h_j2pt[j];
    histos[j][6] = h_l1eta[j];
    histos[j][7] = h_l2eta[j];
    histos[j][8] = h_j1eta[j];
    histos[j][9] = h_j2eta[j];
    histos[j][10] = h_l1phi[j];
    histos[j][11] = h_l2phi[j];
    histos[j][12] = h_j1phi[j];
    histos[j][13] = h_j2phi[j];
    histos[j][14] = h_nleptons[j];
    histos[j][15] = h_njets[j];
    histos[j][16] = h_nvertices[j];
    histos[j][17] = h_dR_l1l2[j];
    histos[j][18] = h_dR_j1j2[j];
    histos[j][19] = h_dR_l1j1[j];
    histos[j][20] = h_dR_l1j2[j];
    histos[j][21] = h_dR_l2j1[j];
    histos[j][22] = h_dR_l2j2[j];
    histos[j][23] = h_isGlobal_0[j];
    histos[j][24] = h_isGlobal_1[j];
    histos[j][25] = h_numberofhits_0[j];
    histos[j][26] = h_numberofhits_1[j];
    histos[j][27] = h_numberofstations_0[j];
    histos[j][28] = h_numberofstations_1[j];
    histos[j][29] = h_sigmapt_0[j];
    histos[j][30] = h_sigmapt_1[j];
    histos[j][31] = h_dxy_0[j];
    histos[j][32] = h_dxy_1[j];
    histos[j][33] = h_dz_0[j];
    histos[j][34] = h_dz_1[j];
    histos[j][35] = h_numberofpixels_0[j];
    histos[j][36] = h_numberofpixels_1[j];
    histos[j][37] = h_numberoflayers_0[j];
    histos[j][38] = h_numberoflayers_1[j];
    histos[j][39] = h_angle3D[j];

    Fill_Histo(histos[j],tree);  

    std::cout<<j<<" "<<histos[j][0]->GetEntries()<<std::endl;
 
  }

  TString xtitles[] = {"Mlljj","dilepton mass","leading lepton p_{T}","subleading lepton p_{T}","leading jet p_{T}","subleading jet p_{T}","leading lepton #eta","subleading lepton #eta","leading jet #eta","subleading jet #eta","leading lepton #phi","subleading lepton #phi","leading jet #phi","subleading jet #phi","number of leptons","number of jets","number of vertices","dR lepton 1 lepton 2","dR jet 1 jet 2","dR lepton 1 jet 1","dR lepton 1 jet 2","dR lepton 2 jet 1","dR lepton 2 jet 2","isGlobal Muon 1","isGlobal Muon 2","Number of hits Muon 1","Number of hits Muon 2","Number of stations Muon 1","Number of stations Muon 2","#sigma_{pT} Muon 1","#sigma_{pT} Muon 2","dxy Muon 1","dxy Muon 2","dz Muon 1","dz Muon 2","Number of pixels Muon 1","Number of pixels Muon 2","Number of layers Muon 1","Number of layers Muon 2","3D angle"};

  TString fnames[] = {"Mlljj","Mll","l1_pt","l2_pt","j1_pt","j2_pt","l1_eta","l2_eta","j1_eta","j2_eta","l1_phi","l2_phi","j1_phi","j2_phi","nleptons","njets","nvertices","dR_l1l2","dR_j1j2","dR_l1j1","dR_l1j2","dR_l2j1","dR_l2j2","isGlobal0","isGlobal1","nhits0","nhits1","nstations0","nstations1","sigmapt0","sigmapt1","dxy0","dxy1","dz0","dz1","npixels0","npixels1","nlayers0","nlayers1","angle3D"};

  for(int i = 0; i != nhistos; i++){
    TCanvas* mycanvas = new TCanvas( "mycanvas", "", 0, 0, 600, 600 ) ;
    histos[0][i]->SetLineColor(1);
    histos[0][i]->Draw();
    for(int j = 1; j<6; j++){
      histos[j][i]->SetLineColor(1+j);
      histos[j][i]->Draw("same");     
    }
    TString fn = "~/www/plots/WR/skimmed/signal/nm1/";
    TString fn_pdf = fn + fnames[i].Data() + ".pdf";
    TString fn_png = fn + fnames[i].Data() + ".png"; 
    //mycanvas->Print(fn_pdf.Data());
    //mycanvas->Print(fn_png.Data());
    mycanvas->Close();
  }
}
