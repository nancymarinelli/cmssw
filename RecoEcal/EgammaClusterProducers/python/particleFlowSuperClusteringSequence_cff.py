import FWCore.ParameterSet.Config as cms

#------------------
#Hybrid clustering:
#------------------
# Producer for Box Particle Flow Super Clusters
from RecoEcal.EgammaClusterProducers.particleFlowSuperClusterECAL_cfi import *
# Producer for energy corrections
#from RecoEcal.EgammaClusterProducers.correctedDynamicHybridSuperClusters_cfi import *
# PFECAL super clusters, either hybrid-clustering clone (Box) or mustache.



particleFlowSuperClusterECALMustacheNewParams = particleFlowSuperClusterECAL.clone()
particleFlowSuperClusterECALMustacheNewParams.PFSuperClusterCollectionBarrel = cms.string("particleFlowSuperClusterECALBarrelMustacheNewParams")
particleFlowSuperClusterECALMustacheNewParams.PFSuperClusterCollectionEndcapWithPreshower = cms.string("particleFlowSuperClusterECALEndcapWithPreshowerMustacheNewParams")
particleFlowSuperClusterECALMustacheNewParams.PFSuperClusterCollectionEndcap = cms.string("particleFlowSuperClusterECALEndcapMustacheNewParams")
particleFlowSuperClusterECALMustacheNewParams.PFBasicClusterCollectionBarrel = cms.string("particleFlowBasicClusterECALBarrelMustacheNewParams")                                       
particleFlowSuperClusterECALMustacheNewParams.PFBasicClusterCollectionEndcap = cms.string("particleFlowBasicClusterECALEndcapMustacheNewParams")                                       
particleFlowSuperClusterECALMustacheNewParams.PFBasicClusterCollectionPreshower = cms.string("particleFlowBasicClusterECALPreshowerMustacheNewParams")
particleFlowSuperClusterECALMustacheNewParams.p00 = cms.double(-0.107537)
particleFlowSuperClusterECALMustacheNewParams.p01 = cms.double(0.590969)
particleFlowSuperClusterECALMustacheNewParams.p02 = cms.double(-0.076494)
particleFlowSuperClusterECALMustacheNewParams.p10 = cms.double(-0.0268843)
particleFlowSuperClusterECALMustacheNewParams.p11 = cms.double(0.147742)
particleFlowSuperClusterECALMustacheNewParams.p12 = cms.double(-0.0191235)
particleFlowSuperClusterECALMustacheNewParams.w00 = cms.double(-0.00016325)
particleFlowSuperClusterECALMustacheNewParams.w01 = cms.double(0.00330476)
particleFlowSuperClusterECALMustacheNewParams.w10 = cms.double(0.00663276)
particleFlowSuperClusterECALMustacheNewParams.w11 = cms.double(-0.00461881)
particleFlowSuperClusterECALMustacheNewParams.yoffsetEB = cms.double(-0.0374196)
particleFlowSuperClusterECALMustacheNewParams.scaleEB = cms.double(2.53626)
particleFlowSuperClusterECALMustacheNewParams.xoffsetEB = cms.double(-1.18163))
particleFlowSuperClusterECALMustacheNewParams.widthEB = cms.double(0.782166)
particleFlowSuperClusterECALMustacheNewParams.yoffsetEE_0 = cms.double(0.083572)
particleFlowSuperClusterECALMustacheNewParams.scaleEE_0 = cms.double(0.656847)
particleFlowSuperClusterECALMustacheNewParams.xoffsetEE_0 = cms.double(0.0207699)
particleFlowSuperClusterECALMustacheNewParams.widthEE_0 = cms.double(0.298995)
particleFlowSuperClusterECALMustacheNewParams.yoffsetEE_1 = cms.double(-0.101296)
particleFlowSuperClusterECALMustacheNewParams.scaleEE_1 = cms.double(32.2549)
particleFlowSuperClusterECALMustacheNewParams.xoffsetEE_1 = cms.double(-5.5264)
particleFlowSuperClusterECALMustacheNewParams.widthEE_1 = cms.double(1.29536)
particleFlowSuperClusterECALMustacheNewParams.yoffsetEE_2 = cms.double(-0.42682)
particleFlowSuperClusterECALMustacheNewParams.scaleEE_2 = cms.double(10.9146)
particleFlowSuperClusterECALMustacheNewParams.xoffsetEE_2 = cms.double(-7.44059)
particleFlowSuperClusterECALMustacheNewParams.widthEE_2 = cms.double(2.85026)

particleFlowSuperClusteringTask = cms.Task(particleFlowSuperClusterECAL,particleFlowSuperClusterECALMustacheNewParams)
particleFlowSuperClusteringSequence = cms.Sequence(particleFlowSuperClusteringTask)



particleFlowSuperClusterHGCal = particleFlowSuperClusterECAL.clone()
from Configuration.Eras.Modifier_phase2_hgcal_cff import phase2_hgcal
phase2_hgcal.toModify(
    particleFlowSuperClusterHGCal,
    PFClusters = cms.InputTag('particleFlowClusterHGCal'),
    useRegression = cms.bool(False), #no HGCal regression yet
    use_preshower = cms.bool(False),
    PFBasicClusterCollectionEndcap = cms.string(""),
    PFSuperClusterCollectionEndcap = cms.string(""),
    PFSuperClusterCollectionEndcapWithPreshower = cms.string(""),
    thresh_PFClusterEndcap = cms.double(1.5e-1), # 150 MeV threshold
    dropUnseedable = cms.bool(True),
)

particleFlowSuperClusterHGCalFromMultiCl = particleFlowSuperClusterHGCal.clone()
phase2_hgcal.toModify(
    particleFlowSuperClusterHGCalFromMultiCl,
    PFClusters = cms.InputTag('particleFlowClusterHGCalFromMultiCl')
)
_phase2_hgcal_particleFlowSuperClusteringTask = particleFlowSuperClusteringTask.copy()
_phase2_hgcal_particleFlowSuperClusteringTask.add(particleFlowSuperClusterHGCal)
_phase2_hgcal_particleFlowSuperClusteringTask.add(particleFlowSuperClusterHGCalFromMultiCl)

phase2_hgcal.toReplaceWith( particleFlowSuperClusteringTask, _phase2_hgcal_particleFlowSuperClusteringTask )

