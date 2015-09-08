import os,sys
import FWCore.ParameterSet.Config as cms

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

os.system('das_client.py --query="file dataset=/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/jchavesb-skim_50ns_sideband-4f011c8726985896d9072f83ba7d78ab/USER instance=prod/phys03" --limit=0 > pappy.txt')

fn = 'pappy.txt'

sec_file = open(fn, 'r')

mysecfilelist = []
for line in sec_file:
    # add as many files as you wish this way
    if 'skim' in line:
        #mysecfilelist.append('file:'+line.strip())
        mysecfilelist.append(line.strip())
process.source.fileNames = mysecfilelist
outfn = '~/eos/WR_skims/skim_ttree.root'
process.TFileService = cms.Service('TFileService', fileName = cms.string(outfn))

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.triggerFilter = hltHighLevel.clone()
process.triggerFilter.HLTPaths = ['HLT_Mu45_eta2p1_v*','HLT_Mu50_v*','HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v*']
#process.triggerFilter.HLTPaths = ['HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL_v*']
process.triggerFilter.andOr = True # = OR
#for name, path in process.paths.items():
 #   if not name.startswith('eventCleaning'):
  #      path.insert(0, process.triggerFilter)
process.ptrig = cms.Path(process.triggerFilter)

process.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('ptrig'))

process.load("CondCore.DBCommon.CondDBCommon_cfi")
from CondCore.DBCommon.CondDBSetup_cfi import *
process.jec = cms.ESSource("PoolDBESSource",
      DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0)
        ),
      timetype = cms.string('runnumber'),
      toGet = cms.VPSet(
      cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Summer15_50nsV2_MC_AK4PFchs'),
            # tag    = cms.string('JetCorrectorParametersCollection_Summer12_V3_MC_AK5PF'),
            label  = cms.untracked.string('AK4PFchs')
            ),
      ## here you add as many jet types as you need
      ## note that the tag name is specific for the particular sqlite file 
      ), 
      connect = cms.string('sqlite:Summer15_50nsV2_MC.db')
)
## add an es_prefer statement to resolve a possible conflict from simultaneous connection to a global tag
process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')


from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetCorrFactorsUpdated
process.patJetCorrFactorsReapplyJEC = patJetCorrFactorsUpdated.clone(
  src = cms.InputTag("slimmedJets"),
  levels = cms.vstring('L1FastJet', 
        'L2Relative', 
        'L3Absolute'),
  payload = cms.string('AK4PFchs') ) # Make sure to choose the appropriate levels and payload here!

from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetsUpdated
process.patJetsReapplyJEC = patJetsUpdated.clone(
  jetSource = cms.InputTag("slimmedJets"),
  jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJEC"))
  )

#process.p += cms.Sequence( process.patJetCorrFactorsReapplyJEC + process. patJetsReapplyJEC )


#process.p = cms.Path(process.patJetCorrFactorsReapplyJEC + process. patJetsReapplyJEC + process.wRsubleadingElectron + process.wRlooseJet + process.triggerFilter  + process.MakeTTree_Electrons)
process.p = cms.Path(process.patJetCorrFactorsReapplyJEC + process. patJetsReapplyJEC + process.wRtunePMuons + process.wRsubleadingMuon + process.wRlooseJet + process.triggerFilter  + process.MakeTTree_Muons)
#process.p = cms.Path(process.patJetCorrFactorsReapplyJEC + process. patJetsReapplyJEC + process.wRtunePMuons + process.wRsubleadingMuon + process.wRlooseJet + process.triggerFilter  + process.MakeTTree_EleMuon)
