void treeCutter(){

  TH1::SetDefaultSumw2();
  // Drell-Yan
  TString pre_name = Form("../../../miniaod_v2/dysamples_powheg/");
  TString pre_name_ee = Form("../../../miniaod_v2/electrons/");
  TString fnames[] = {"skim_ttree_120to200.root","skim_ttree_200to400.root","skim_ttree_400to800.root","skim_ttree_800to1400.root","skim_ttree_1400to2300.root","skim_ttree_2300to3500.root","skim_ttree_3500to4500.root","skim_ttree_4500to6000.root","skim_ttree_6000toInf.root"};

  for (int i=0;i<9;i++) {    
    TFile * hfile = new TFile((pre_name+fnames[i]).Data());
    TTree *tree = (TTree*)hfile->Get("MakeTTree_Muons/t"); 
    TFile * hfile_ee = new TFile((pre_name_ee+fnames[i]).Data());
    TTree *tree_ee = (TTree*)hfile_ee->Get("MakeTTree_Electrons/t"); 

    TString cut = "Mlljj>600 && dilepton_mass>200 && leading_lepton_pt>40 && subleading_lepton_pt>40 && (leading_lepton_pt>60 || subleading_lepton_pt>60) && leading_jet_pt>40 && subleading_jet_pt>40 && dR_leadLepton_subleadLepton > 0.4 && dR_leadLepton_leadJet > 0.4 && dR_leadLepton_subleadJet > 0.4 && dR_subleadLepton_leadJet > 0.4 && dR_subleadLepton_subleadJet > 0.4 && fabs(leading_jet_eta)<2.4 && fabs(subleading_jet_eta)<2.4";
    
    tree->SetBranchStatus("*",    0);
    tree->SetBranchStatus("Mlljj",    1);
    tree->SetBranchStatus("dilepton_mass",    1);
    tree->SetBranchStatus("leading_lepton_pt",    1);
    tree->SetBranchStatus("subleading_lepton_pt",    1);
    tree->SetBranchStatus("leading_jet_pt",    1);
    tree->SetBranchStatus("subleading_jet_pt",    1);
    tree->SetBranchStatus("leading_jet_eta",    1);
    tree->SetBranchStatus("subleading_jet_eta",    1);
    tree->SetBranchStatus("dR_leadLepton_subleadLepton",    1);
    tree->SetBranchStatus("dR_leadLepton_leadJet",    1);
    tree->SetBranchStatus("dR_leadLepton_subleadJet",    1);
    tree->SetBranchStatus("dR_subleadLepton_leadJet",    1);
    tree->SetBranchStatus("dR_subleadLepton_subleadJet",    1);
    tree->SetBranchStatus("weight",    1);

    tree_ee->SetBranchStatus("*",    0);
    tree_ee->SetBranchStatus("Mlljj",    1);
    tree_ee->SetBranchStatus("dilepton_mass",    1);
    tree_ee->SetBranchStatus("leading_lepton_pt",    1);
    tree_ee->SetBranchStatus("subleading_lepton_pt",    1);
    tree_ee->SetBranchStatus("leading_jet_pt",    1);
    tree_ee->SetBranchStatus("subleading_jet_pt",    1);
    tree_ee->SetBranchStatus("leading_jet_eta",    1);
    tree_ee->SetBranchStatus("subleading_jet_eta",    1);
    tree_ee->SetBranchStatus("dR_leadLepton_subleadLepton",    1);
    tree_ee->SetBranchStatus("dR_leadLepton_leadJet",    1);
    tree_ee->SetBranchStatus("dR_leadLepton_subleadJet",    1);
    tree_ee->SetBranchStatus("dR_subleadLepton_leadJet",    1);
    tree_ee->SetBranchStatus("dR_subleadLepton_subleadJet",    1);
    tree_ee->SetBranchStatus("weight",    1);

    TString s2( "DY_Selected_MuMu"+fnames[i](11,100) );
    TFile *newfile = new TFile(s2.Data(),"recreate");
    TTree *newtree = tree->CopyTree(cut);
    newtree->SetName((fnames[i](11,8)).Data());
    newtree->Write();
    TString s2_ee( "DY_Selected_EE"+fnames[i](11,100) );
    TFile *newfile_ee = new TFile(s2_ee.Data(),"recreate");
    TTree *newtree_ee = tree_ee->CopyTree(cut);
    newtree_ee->SetName((fnames[i](11,8)).Data());
    newtree_ee->Write();

  }


}
