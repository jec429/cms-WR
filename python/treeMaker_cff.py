import FWCore.ParameterSet.Config as cms


# TTree producer
""" \addtogroup TTree producer
@{
"""

from ExoAnalysis.cmsWR.skimMuon_cff import *
from ExoAnalysis.cmsWR.skimElectron_cff import *
from ExoAnalysis.cmsWR.skimEMu_cff import *

TFileService = cms.Service('TFileService', fileName = cms.string('made_ttree.root'))

MakeTTree_Muons = cms.EDAnalyzer('TTreeMaker',
                            muons_src = cms.InputTag('wRsubleadingMuon'),
                            electrons_src = cms.InputTag('slimmedElectrons'),
                            jets_src = cms.InputTag('wRlooseJet'),
                            genparticles_src = cms.InputTag('prunedGenParticles'),
                            genjets_src = cms.InputTag('slimmedGenJets'),
                            primary_vertex_src = cms.InputTag('offlineSlimmedPrimaryVertices'),
                            beamSpot = cms.InputTag('offlineBeamSpot'),
                            isolation_dR = cms.double(0.0),
                            leading_lepton_pt_cut = cms.double(0),
                            lepton_eta_cut = cms.double(5),
                            subleading_lepton_pt_cut = cms.double(0),
                            leading_jet_pt_cut = cms.double(0),
                            jet_eta_cut = cms.double(5),
                            subleading_jet_pt_cut = cms.double(0),
                            Mlljj_cut = cms.double(0.0),
                            Mll_cut = cms.double(0.0),
                            muon_mode = cms.bool(True),
                            electron_mode = cms.bool(False),
                            conversionsMiniAOD  = cms.InputTag('reducedEgamma:reducedConversions'),
                            #
                            # ID decisions (common to all formats)
                            #
                            eleVetoIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V1-standalone-veto"),
                            eleLooseIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V1-standalone-loose"),
                            eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V1-standalone-medium"),
                            eleTightIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V1-standalone-tight"),
                            eleHEEPIdMap = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV60"),
                            # An example of configuration for accessing the full cut flow info for
                            # one case is shown below.
                            # The map name for the full info is the same as the map name of the
                            # corresponding simple pass/fail map above, they are distinguished by
                            # the type of the content.
                            eleHEEPIdFullInfoMap = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV60"),
                            # This is a fairly verbose mode if switched on, with full cut flow 
                            # diagnostics for each candidate. Use it in a low event count test job.
                            eleIdVerbose = cms.bool(False)
                            )

MakeTTree_Electrons = MakeTTree_Muons.clone(muon_mode = cms.bool(False),
                                            electron_mode = cms.bool(True))
MakeTTree_EleMuon = MakeTTree_Muons.clone(muon_mode = cms.bool(True),
                                            electron_mode = cms.bool(True))
"""
@}
"""
