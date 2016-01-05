void makeHisto(TH1F* h1);
void simple_histos() {
  using namespace RooFit ;

  RooRealVar x("x","M_{lljj}",550,3000) ;
  x.setBins(50) ;

  // TTbar contribution
  TFile * flavor_file = new TFile("flavor_histos.root") ; // Eventually this histo is taken from data
  TH1* hh = (TH1*) flavor_file->Get("h") ;
  RooDataHist ttbar("ttbar","dataset with x",x,hh) ;   

  // DYJets contribution
  // Local fit
  RooRealVar alpha("alpha","alpha",1.0e+05,0.,1.0e+06) ;
  RooRealVar beta("beta","beta",0.1,0.0,1.0) ;
  RooRealVar alpha_up("alpha_up","alpha_up",1.0e+05,0.,1.0e+06) ;
  RooRealVar beta_up("beta_up","beta_up",0.1,0.0,1.0) ;
  RooRealVar alpha_down("alpha_down","alpha_down",1.0e+05,0.,1.0e+06) ;
  RooRealVar beta_down("beta_down","beta_down",0.1,0.0,1.0) ;

  TH1F *h0 = new TH1F("h0","",50,550,2500);  
  makeHisto(h0);
  //cout<<"Histo INTEGRAL="<<h0->Integral()<<endl;
  RooDataHist dyhist("dyhist","",x,h0);
  RooGenericPdf dyjets("dyjets","alpha*exp(-beta*x)",RooArgSet(x,alpha,beta)) ; 
  dyjets.fitTo(dyhist, SumW2Error(kTRUE));
  RooGenericPdf dyjets_up_fit("dyjets_up_fit","alpha_up*exp(-beta_up*x^(1.1))",RooArgSet(x,alpha_up,beta_up)) ; 
  dyjets_up_fit.fitTo(dyhist, SumW2Error(kTRUE));
  RooGenericPdf dyjets_down_fit("dyjets_down_fit","alpha_down*exp(-beta_down*x^(0.9))",RooArgSet(x,alpha_down,beta_down)) ; 
  dyjets_down_fit.fitTo(dyhist, SumW2Error(kTRUE));

  RooPlot* xframe = x.frame() ;
  dyhist.plotOn(xframe) ;
  dyjets.plotOn(xframe) ;
  dyjets_up_fit.plotOn(xframe , LineColor(kRed)) ;
  dyjets_down_fit.plotOn(xframe , LineColor(kGreen)) ;
  xframe->Draw() ;

  // Signal function. Use gaussian for now
  RooRealVar mean("mean","Mean of Gaussian",2000,550,3000) ;
  RooRealVar sigma("sigma","Width of Gaussian",300,0,1000) ;
  RooGaussian gauss("gauss","gauss(x,mean,sigma)",x,mean,sigma) ; 
 
  // ------------ Make histograms ---------------------------
  TFile *allHistFile = new TFile("simple-shapes-TH1.root", "RECREATE");
  TH1 *ttbar_nominal = ttbar.createHistogram("x"); ttbar_nominal->SetName("ttbar"); ttbar_nominal->Scale(50919./72835 * 57.35*2460/(30479698)) ; // Scale by flavor factor and lumi
  TH1 *ttbar_up = ttbar.createHistogram("x"); ttbar_up->SetName("ttbar_scaleUp"); ttbar_up->Scale(50919./72835*1.1 * 57.35*2460/(30479698)) ; // Scale by flavor factor and lumi
  TH1 *ttbar_down = ttbar.createHistogram("x"); ttbar_down->SetName("ttbar_scaleDown"); ttbar_down->Scale(50919./72835*0.9 * 57.35*2460/(30479698)) ; // Scale by flavor factor and lumi

  TH1 *dyjets_nominal = dyjets.createHistogram("x"); dyjets_nominal->SetName("dyjets"); dyjets_nominal->Scale(4467.64*2460/100000.) ; // Scale by DY integral and lumi
  TH1 *dyjets_up = dyjets_up_fit.createHistogram("x"); dyjets_up->SetName("dyjets_alpha_betaUp"); dyjets_up->Scale(4467.64*2460/100000.) ; // Scale by DY integral and by lumi
  TH1 *dyjets_down = dyjets_down_fit.createHistogram("x"); dyjets_down->SetName("dyjets_alpha_betaDown"); dyjets_down->Scale(4467.64*2460/100000.) ; // Scale by DY integral and by lumi

  TH1 *signal_nominal = gauss.createHistogram("x"); 
  signal_nominal->SetName("signal"); signal_nominal->Scale(0.062200*2460); // Cross-section scale and 2.460 fb-1
  sigma.setVal(1.6*300);
  TH1 *signal_up = gauss.createHistogram("x");  
  signal_up->SetName("signal_sigmaUp"); signal_up->Scale(0.062200*2460);
  sigma.setVal(0.7*300);
  TH1 *signal_down = gauss.createHistogram("x");  
  signal_down->SetName("signal_sigmaDown"); signal_down->Scale(0.062200*2460);

  TH1 *hdata_b =  ttbar.createHistogram("x"); hdata_b->Scale(41750./60506.); hdata_b->Add(dyjets_nominal); hdata_b->SetName("data_obs");

  allHistFile->WriteTObject(ttbar_nominal); allHistFile->WriteTObject(ttbar_up); allHistFile->WriteTObject(ttbar_down);
  allHistFile->WriteTObject(dyjets_nominal); allHistFile->WriteTObject(dyjets_up); allHistFile->WriteTObject(dyjets_down);
  allHistFile->WriteTObject(signal_nominal); allHistFile->WriteTObject(signal_up); allHistFile->WriteTObject(signal_down);
  allHistFile->WriteTObject(hdata_b);
}

void makeHisto(TH1F* h1){
  
  TString pre_name = Form("~/eos/WR_skims/miniaod_v2/dysamples_powheg/");
  TString fnames[] = {"skim_ttree_120to200.root","skim_ttree_200to400.root","skim_ttree_400to800.root","skim_ttree_800to1400.root","skim_ttree_1400to2300.root","skim_ttree_2300to3500.root","skim_ttree_3500to4500.root","skim_ttree_4500to6000.root","skim_ttree_6000toInf.root"};
  float xsecs[] = {19.32,2.731,0.241,0.01678,0.00139,0.00008948,0.000004135,4.56e-7,2.066e-8};

  for(int i=0;i<9;i++){
    TFile * hfile = new TFile((pre_name+fnames[i]).Data());
    TTree *tree = (TTree*)hfile->Get("MakeTTree_Muons/t"); 
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

    for (Int_t ev = 0; ev < nentries; ev++) {
      tree->GetEntry(ev); 
      float reweight = 1.0;
      reweight = weight/fabs(weight);

      if(Mlljj>0 && Mll>200 && l1_pt>60 && l2_pt>40 && j1_pt>40 && j2_pt>40 && Mll>50 && dR_l1l2 > 0.1 && dR_l1j1 > 0.4 && dR_l1j2 > 0.4 && dR_l2j1 > 0.4 && dR_l2j2 > 0.4 && fabs(j1_eta)<2.4 && fabs(j2_eta)<2.4){
	h1->Fill(Mlljj,reweight*xsecs[i]);
      }
    }
  }
}
