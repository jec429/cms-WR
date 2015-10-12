from JMTucker.Tools.Sample import anon_samples

samples = anon_samples('''
#/SingleMuon/Run2015B-PromptReco-v1/MINIAOD
/SingleMuon/Run2015C-PromptReco-v1/MINIAOD
/SingleMuon/Run2015D-PromptReco-v3/MINIAOD

''', dbs_inst='global')

samplesMuEG = anon_samples('''
#/MuonEG/Run2015B-PromptReco-v1/MINIAOD
/MuonEG/Run2015C-PromptReco-v1/MINIAOD
/MuonEG/Run2015D-PromptReco-v3/MINIAOD

''', dbs_inst='global')

from JMTucker.Tools.CRAB3Submitter import CRABSubmitter
def cfg_modifier(cfg, sample):
    cfg.JobType.inputFiles = ['Summer15_50nsV2_MC.db']
    cfg.Data.lumiMask = 'Cert_246908-256869_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
cs = CRABSubmitter('WR_makettree_data_25ns',
        splitting = 'EventAwareLumiBased',
        pset_template_fn = 'makeTTree_data.py',
        units_per_job = 50000,
        total_units = -1,
        #aaa = True,
        publish_name='makettree_data_25ns',
        cfg_modifier = cfg_modifier,
        )
cs.submit_all(samples)

cs_emu = CRABSubmitter('WR_makettree_data_25ns_emu',
        splitting = 'EventAwareLumiBased',
        pset_template_fn = 'makeTTree_data_emu.py',
        units_per_job = 50000,
        total_units = -1,
        #aaa = True,
        publish_name='makettree_data_25ns_emu',
        cfg_modifier = cfg_modifier,
        )
cs_emu.submit_all(samplesMuEG)
