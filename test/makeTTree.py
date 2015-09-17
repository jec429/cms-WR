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

process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring('file:/afs/cern.ch/user/j/jchavesb/work/WR_skims/DYJets/skim_signal_1.root'),
                            )

os.system('das_client.py --query="file dataset=/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM instance=prod/phys03" --limit=0 > pappy.txt')

fn = 'pappy.txt'

sec_file = open(fn, 'r')

mysecfilelist = []
for line in sec_file:
    # add as many files as you wish this way
    if 'skim' in line:
        #mysecfilelist.append('file:'+line.strip())
        mysecfilelist.append(line.strip())
process.source.fileNames = mysecfilelist
outfn = 'skim_ttree.root'
process.TFileService = cms.Service('TFileService', fileName = cms.string(outfn))

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

JEC_correction(process)

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.triggerFilter = hltHighLevel.clone()
#process.triggerFilter.HLTPaths = ['HLT_Mu45_eta2p1_v*','HLT_Mu50_v*']
#process.triggerFilter.HLTPaths = ['HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v*']
process.triggerFilter.HLTPaths = ['HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL_v*']
process.triggerFilter.andOr = True # = OR
#for name, path in process.paths.items():
 #   if not name.startswith('eventCleaning'):
  #      path.insert(0, process.triggerFilter)
process.ptrig = cms.Path(process.triggerFilter)

process.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('ptrig'))

#process.p = cms.Path(process.patJetCorrFactorsReapplyJEC + process. patJetsReapplyJEC + process.wRsubleadingElectron + process.wRlooseJet + process.triggerFilter + process.triggerFilter  + process.egmGsfElectronIDSequence * process.MakeTTree_Electrons)
#process.p = cms.Path(process.patJetCorrFactorsReapplyJEC + process. patJetsReapplyJEC + process.wRtunePMuons + process.wRsubleadingMuon + process.wRlooseJet + process.triggerFilter  + process.MakeTTree_Muons)
process.p = cms.Path(process.patJetCorrFactorsReapplyJEC + process. patJetsReapplyJEC + process.wRtunePMuons + process.wRsubleadingMuon + process.wRlooseJet + process.triggerFilter  + process.egmGsfElectronIDSequence * process.MakeTTree_EleMuon)
