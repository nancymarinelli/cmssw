import FWCore.ParameterSet.Config as cms

process = cms.Process("PROdTPA")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '91X_upgrade2023_realistic_v3', '')

process.load("Geometry.CaloEventSetup.CaloGeometry_cff")

process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.load("CalibCalorimetry.EcalTPGTools.ecalTPGScale_cff")

process.source = cms.Source("PoolSource",

fileNames = cms.untracked.vstring('file:EBTP_PhaseII_SingleNeu_Clu5x5_PU200.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

from SimCalorimetry.EcalTrigPrimProducers.ecalTrigPrimESProducer_cff import *

process.tpAnalyzer = cms.EDAnalyzer("EcalEBTrigPrimAnalyzer",
                                    outFileName = cms.string('histos_EBTP_PhaseII_SingleNeu_Clu5x5_PU200.root'),
                                    inputRecHitsEB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
                                    barrelEcalDigis = cms.InputTag("simEcalDigis","ebDigis"),
                                    RecoContentAvailable =  cms.bool(False),
                                    AnalyzeRecHits = cms.bool(False),
                                    AnalyzeElectrons = cms.bool(False),
                                    Debug = cms.bool(False),
#                                    inputTP = cms.InputTag("EcalEBTrigPrimProducer")
                                    inputTP = cms.InputTag("simEcalEBTriggerPrimitiveDigis"),
                                    inputClusterTP = cms.InputTag("simEcalEBClusterTriggerPrimitiveDigis"),
                                    bxInfos = cms.InputTag("addPileupInfo"),
                                    genParticles = cms.InputTag("genParticles"),
                                    eleCollection = cms.InputTag("gedGsfElectrons"),
                                    phoCollection = cms.InputTag("gedPhotons")   
)



process.p = cms.Path(process.tpAnalyzer)
