import FWCore.ParameterSet.Config as cms

process = cms.Process("EBTPGTest")

process.load('Configuration.StandardSequences.Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.EventContent.EventContent_cff')
process.MessageLogger.categories = cms.untracked.vstring('EBPhaseIITPStudies', 'FwkReport')
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
   reportEvery = cms.untracked.int32(1)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20) )

process.source = cms.Source("PoolSource",
#   fileNames = cms.untracked.vstring('file:/hdfs/store/mc/TTI2023Upg14D/SingleElectronFlatPt0p2To50/GEN-SIM-DIGI-RAW/PU140bx25_PH2_1K_FB_V3-v2/00000/80EE54BB-1EE6-E311-877F-002354EF3BE0.root')
   #fileNames = cms.untracked.vstring('file:root://cmsxrootd.fnal.gov///store/relval/CMSSW_8_1_0_pre9/RelValSingleElectronPt35Extended/GEN-SIM-DIGI-RAW/81X_mcRun2_asymptotic_v2_2023LReco-v1/10000/0E8B87C3-4953-E611-A003-0CC47A4D762A.root')
   fileNames = cms.untracked.vstring('file:root://cmsxrootd.fnal.gov///store/relval/CMSSW_8_1_0_pre9/RelValSingleElectronPt35Extended/GEN-SIM-RECO/81X_mcRun2_asymptotic_v2_2023LReco-v1/10000/72FC7A8C-4E53-E611-95D6-0CC47A78A360.root')
   #fileNames = cms.untracked.vstring('file:root://cmsxrootd.fnal.gov///store/relval/CMSSW_8_0_5/RelValQCD_FlatPt_15_3000HS_13/GEN-SIM-RECO/PU25ns_80X_mcRun2_asymptotic_2016_miniAODv2_v0_gs71xJecGT-v1/00000/06A1518F-3311-E611-BC5A-0CC47A78A496.root')
)

# All this stuff just runs the various EG algorithms that we are studying
                         
# ---- Global Tag :
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'PH2_1K_FB_V3::All', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

# Choose a 2030 geometry!
#process.load('Configuration.Geometry.GeometryExtended2023simReco_cff') # Has CaloTopology, but no ECal endcap, don't use!
process.load('Configuration.Geometry.GeometryExtended2023GRecoReco_cff') # using this geometry because I'm not sure if the tilted geometry is vetted yet
#process.load('Configuration.Geometry.GeometryExtended2023tiltedReco_cff') # this one good?

#process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
#XXX process.load('Configuration.StandardSequences.L1TrackTrigger_cff')
#XXX process.load('Geometry.TrackerGeometryBuilder.StackedTrackerGeometry_cfi')
#XXX process.load('IOMC.EventVertexGenerators.VtxSmearedHLLHC_cfi')
#XXX process.load('IOMC.EventVertexGenerators.VtxSmearedHLLHC_cfi')

#XXX process.load('Configuration/StandardSequences/L1HwVal_cff')
#XXX process.load('Configuration.StandardSequences.RawToDigi_cff')
#XXX #XXX process.load("SLHCUpgradeSimulations.L1CaloTrigger.SLHCCaloTrigger_cff")
#XXX 
#XXX 
#XXX process.slhccalo = cms.Path( process.RawToDigi)
#XXX 
#XXX 
#XXX # run L1Reco to produce the L1EG objects corresponding
#XXX # to the current trigger
#XXX process.load('Configuration.StandardSequences.L1Reco_cff')
#XXX process.L1Reco = cms.Path( process.l1extraParticles )
#XXX 
#XXX 
#XXX 
#XXX # --------------------------------------------------------------------------------------------
#XXX #
#XXX # ----    Produce the L1EGCrystal clusters (code of Sasha Savin)
#XXX 
#XXX # first you need the ECAL RecHIts :
#XXX process.load('Configuration.StandardSequences.Reconstruction_cff')
#XXX #process.bunchSpacingProducer = cms.EDProducer("BunchSpacingProducer")
#XXX #process.bsProd = cms.Path( process.bunchSpacingProducer )
#XXX #process.reconstruction_step = cms.Path( process.bunchSpacingProducer + process.hbheprereco + process.calolocalreco )
#XXX process.reconstruction_step = cms.Path( process.bunchSpacingProducer + process.hbheUpgradeReco + process.calolocalreco )



process.simEcalEBTriggerPrimitiveDigis = cms.EDProducer("EcalEBTrigPrimProducer",
    BarrelOnly = cms.bool(True),
    barrelEcalHits = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    binOfMaximum = cms.int32(6), ## optional from release 200 on, from 1-10
    Famos = cms.bool(False),
    TcpOutput = cms.bool(False),
    Debug = cms.bool(False),
    Label = cms.string('simEcalUnsuppressedDigis')
)




#process.pNancy = cms.Path( process.EcalEBTrigPrimProducer )
process.pNancy = cms.Path( process.simEcalEBTriggerPrimitiveDigis )



process.Out = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "EBTP_PhaseII.root" ),
    fastCloning = cms.untracked.bool( False ),
    outputCommands = cms.untracked.vstring("keep *_EcalEBTrigPrimProducer_*_*",
                                           "keep *_TriggerResults_*_*",
                                           "keep *_simEcalEBTriggerPrimitiveDigis_*_*")
)

process.end = cms.EndPath( process.Out )



#print process.dumpPython()
dump_file = open("dump_file.py", "w")
dump_file.write(process.dumpPython())

