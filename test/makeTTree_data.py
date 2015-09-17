import os,sys
import FWCore.ParameterSet.Config as cms
from ExoAnalysis.cmsWR.JEC_cff import *

process = cms.Process('ANA')

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.load("Configuration.StandardSequences.Geometry_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# NOTE: the pick the right global tag!
#    for Spring15 50ns : global tag is MCRUN2_74_V9A
#  as a rule, find the global tag in the DAS under the Configs for given dataset
process.GlobalTag.globaltag = 'MCRUN2_74_V9A'
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
# turn on VID producer, indicate data format  to be
dataFormat = DataFormat.MiniAOD

switchOnVIDElectronIdProducer(process, dataFormat)

# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_50ns_V1_cff',
                 'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff']

#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)
    
process.load('ExoAnalysis.cmsWR.treeMaker_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

fn = 'file:/afs/cern.ch/user/j/jchavesb/work/WR_skims/skim_sideband_data.root'
#fn = 'file:/afs/cern.ch/user/j/jchavesb/work/WR_skims/skim_sideband_data_electron.root'
#fn = '/store/user/skalafut/DoubleEG/realData_DoubleEG_13TeV_50ns_eejj_signalAndLowMassRegionSkim_atFNALLPC/150727_142936/0000/realData_electronLowMassRegionSkim_1.root'
process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(fn),
                            )

os.system('das_client.py --query="file dataset=/SingleMuon/Run2015B-05Aug2015-v1/MINIAOD instance=prod/phys03" --limit=0 > pappy.txt')

sec_file = open('pappy.txt', 'r')
mysecfilelist = []
for line in sec_file:
    # add as many files as you wish this way
    mysecfilelist.append(line.strip())
process.source.fileNames = mysecfilelist

out_fn = '~/eos/WR_skims/skim_ttree_data_05Aug2015.root'
if 'electron' in fn:
    out_fn = 'skim_ttree_data_electron.root'
    
process.TFileService = cms.Service('TFileService', fileName = cms.string(out_fn))

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

import FWCore.PythonUtilities.LumiList as LumiList
process.source.lumisToProcess = LumiList.LumiList(filename = 'Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON.txt').getVLuminosityBlockRange()

JEC_correction(process)

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.triggerFilter = hltHighLevel.clone()
process.triggerFilter.HLTPaths = ['HLT_Mu45_eta2p1_v*','HLT_Mu50_v*']
#process.triggerFilter.HLTPaths = ['HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL_v*']
if 'electron' in fn:
    process.triggerFilter.HLTPaths = ['HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v*']
process.triggerFilter.andOr = True # = OR
#for name, path in process.paths.items():
 #   if not name.startswith('eventCleaning'):
  #      path.insert(0, process.triggerFilter)
process.ptrig = cms.Path(process.triggerFilter)

process.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('ptrig'))
process.MakeTTree_Muons.is_mc = cms.bool(False)
process.MakeTTree_EleMuon.is_mc = cms.bool(False)

#process.p = cms.Path(process.patJetCorrFactorsReapplyJEC + process. patJetsReapplyJEC + process.wRsubleadingElectron + process.wRlooseJet + process.triggerFilter  + process.egmGsfElectronIDSequence * process.MakeTTree_Electrons)
#process.p = cms.Path(process.patJetCorrFactorsReapplyJEC + process. patJetsReapplyJEC + process.wRtunePMuons + process.wRsubleadingMuon + process.wRlooseJet + process.triggerFilter  + process.egmGsfElectronIDSequence * process.MakeTTree_EleMuon)   
process.p = cms.Path(process.patJetCorrFactorsReapplyJEC + process. patJetsReapplyJEC + process.wRtunePMuons + process.wRsubleadingMuon + process.wRlooseJet + process.triggerFilter  + process.MakeTTree_Muons)

