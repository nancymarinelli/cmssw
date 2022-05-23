import FWCore.ParameterSet.Config as cms

EBPhase2TPGParamProducer = cms.EDAnalyzer("EcalEBPhase2TPParamProducer",
                                                inputFile = 
cms.untracked.string('../../../SimCalorimetry/EcalEBTrigPrimProducers/data/avg-pulse.root'),
                                                outputFile = 
cms.untracked.string('../../../SimCalorimetry/EcalEBTrigPrimProducers/data/AmpTimeOnPeakXtalWeights.txt.gz'),
                                                nSamplesToUse = cms.uint32(12),
                                                useBXPlusOne = cms.bool(True),
                                                phaseShift  = cms.double (0.),
                                                nWeightGroups = cms.uint32(61200),
                                                Et_sat = cms.double(1998.36),
                                                xtal_LSB = cms.double(0.0488)
 
## If nSamplesToUse is 8 ==> useBXPlusOne is False                                                                                                                                               
## If nSamplesToUse is 6 ==> useBXPlusOne is False                                                                                                                                               

)

