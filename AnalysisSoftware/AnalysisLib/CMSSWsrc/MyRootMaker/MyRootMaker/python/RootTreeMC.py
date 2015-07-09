from MyRootMaker.MyRootMaker.RootMakerTemplateMC_cfg import *

process.source.fileNames = cms.untracked.vstring(
'root://xrootd.unl.edu//store/mc/Spring14dr/DYJetsToLL_M-50_13TeV-madgraph-pythia8/AODSIM/PU_S14_POSTLS170_V6-v1/00000/00BBF612-84CC-E311-AB94-00266CF270A8.root'
#'file:/afs/cern.ch/work/h/hindrich/RootMaker/configfiles/MCSpring14/Spring14Test.root'
#'file:/afs/cern.ch/work/h/hindrich/URsoftware/DYJetsToLL_M-50_13TeV-madgraph-pythia8-tauola_v2_TEST.root'
#'file:/afs/cern.ch/work/h/hindrich/URsoftware/VBF_ZZTo2e2mu_SignalContinInterf_SM_M-125p6_13TeV-phantom123-pythia8.root'
#'root://xrootd.unl.edu//store/mc/Spring14dr/VBF_ZZTo4mu_SignalContinInterf_SM_M-125p6_13TeV-phantom123-pythia8/AODSIM/PU_S14_POSTLS170_V6-v1/00000/D89A38B9-D309-E411-9926-002618943926.root'
)

#process.GlobalTag.globaltag = cms.string('START53_V27::All')
process.GlobalTag.globaltag = cms.string('POSTLS170_V6::All')

process.load('RecoBTag/Configuration/RecoBTag_cff')
process.btag = cms.Path(process.btagging)

process.makeroottree.RecMuonNum = cms.untracked.int32(0)
process.makeroottree.HLTriggerSelection = cms.untracked.vstring()
process.makeroottree.GenAllParticles = cms.untracked.bool(True)
#process.makeroottree.GenSomeParticles = cms.untracked.bool(True)

process.schedule = cms.Schedule(
#process.vertex_step,
#process.filters_step,
process.jet_step,
#process.jetpuid_step,
process.btag,
#process.pat_step,
process.electron_step,
process.jetflavour_step,
#process.pfiso_step,
process.roottree_step)
