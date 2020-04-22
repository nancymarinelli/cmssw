import FWCore.ParameterSet.Config as cms
import os

particleFlowSuperClusterECALBox = cms.EDProducer(
    "PFECALSuperClusterProducer",
    # verbosity 
    verbose = cms.untracked.bool(False),
    #clustering type: "Box" or "Mustache"
    ClusteringType = cms.string("Box"),
    #energy weighting: "Raw", "CalibratedNoPS", "CalibratedTotal"
    EnergyWeight = cms.string("Raw"),

    #this overrides both dphi cuts below if true!
    useDynamicDPhiWindow = cms.bool(False),
    
    #PFClusters collection
    PFClusters = cms.InputTag("particleFlowClusterECAL"),
    ESAssociation = cms.InputTag("particleFlowClusterECAL"),
    BeamSpot = cms.InputTag("offlineBeamSpot"),    
                                                  
    PFBasicClusterCollectionBarrel = cms.string("particleFlowBasicClusterECALBarrel"),                                       
    PFSuperClusterCollectionBarrel = cms.string("particleFlowSuperClusterECALBarrel"),
    PFBasicClusterCollectionEndcap = cms.string("particleFlowBasicClusterECALEndcap"),                                       
    PFSuperClusterCollectionEndcap = cms.string("particleFlowSuperClusterECALEndcap"),
    PFBasicClusterCollectionPreshower = cms.string("particleFlowBasicClusterECALPreshower"),
    PFSuperClusterCollectionEndcapWithPreshower = cms.string("particleFlowSuperClusterECALEndcapWithPreshower"),                                          

    #use preshower ?
    use_preshower = cms.bool(True),

    # are the seed thresholds Et or Energy?
    seedThresholdIsET = cms.bool(True),

    # regression setup
    useRegression = cms.bool(False), #regressions are mustache only
    regressionConfig = cms.PSet(
       regressionKeyEB = cms.string('pfscecal_EBCorrection_offline_v2'),
       uncertaintyKeyEB = cms.string('pfscecal_EBUncertainty_offline_v2'),
       regressionKeyEE = cms.string('pfscecal_EECorrection_offline_v2'),
       uncertaintyKeyEE = cms.string('pfscecal_EEUncertainty_offline_v2'),
       vertexCollection = cms.InputTag("offlinePrimaryVertices"),
       ecalRecHitsEB = cms.InputTag('ecalRecHit','EcalRecHitsEB'),
       ecalRecHitsEE = cms.InputTag('ecalRecHit','EcalRecHitsEE'),
       applySigmaIetaIphiBug = cms.bool(False)
       ),

    #threshold for final SuperCluster Et
    thresh_SCEt = cms.double(4.0),    
    
    # threshold in ECAL
    thresh_PFClusterSeedBarrel = cms.double(3.0),
    thresh_PFClusterBarrel = cms.double(0.5),

    thresh_PFClusterSeedEndcap = cms.double(5.0),
    thresh_PFClusterEndcap = cms.double(0.5),

    # window width in ECAL
    phiwidth_SuperClusterBarrel = cms.double(0.28),
    etawidth_SuperClusterBarrel = cms.double(0.04),

    phiwidth_SuperClusterEndcap = cms.double(0.28),
    etawidth_SuperClusterEndcap = cms.double(0.04),

    # turn on merging of the seed cluster to its nearest neighbors
    # that share a rechit
    doSatelliteClusterMerge = cms.bool(False),
    satelliteClusterSeedThreshold = cms.double(50.0),
    satelliteMajorityFraction = cms.double(0.5),
    dropUnseedable = cms.bool(False),
    #thresh_PFClusterMustacheOutBarrel = cms.double(0.),
    #thresh_PFClusterMustacheOutEndcap = cms.double(0.),                                             

 # old mustache parameters, before optimization 2019-2020                                                                                                                                      
    p00 = cms.double(-0.107537),
    p01 = cms.double(0.590969),
    p02 = cms.double(-0.076494),
    p10 = cms.double(-0.0268843),
    p11 = cms.double(0.147742),
    p12 = cms.double(-0.0191235),
    w00 = cms.double(-0.00571429),
    w01 = cms.double(-0.002),
    w10 = cms.double(0.0135714),
    w11 = cms.double(0.001),
    yoffsetEB = cms.double(7.151e-02),
    scaleEB = cms.double(5.656e-01),
    xoffsetEB = cms.double(2.931e-01),
    widthEB = cms.double(2.976e-01),
    yoffsetEE_0 = cms.double(5.058e-02),
    scaleEE_0 = cms.double(7.131e-01),
    xoffsetEE_0 = cms.double(1.668e-02),
    widthEE_0 = cms.double(4.114e-01),
    yoffsetEE_1 = cms.double(-9.913e-02),
    scaleEE_1 = cms.double(4.404e+01),
    xoffsetEE_1 = cms.double(-5.326e+00),
    widthEE_1 = cms.double(1.184e+00),
    yoffsetEE_2 = cms.double(-6.346e-01),
    scaleEE_2 = cms.double(1.317e+01),
    xoffsetEE_2 = cms.double(-7.037e+00),
    widthEE_2 = cms.double(2.836e+00),


    #corrections
    applyCrackCorrections = cms.bool(False)                                          
                                              
)

particleFlowSuperClusterECALMustache = cms.EDProducer(
    "PFECALSuperClusterProducer",
    # verbosity 
    verbose = cms.untracked.bool(False),
    #clustering type: "Box" or "Mustache"
    ClusteringType = cms.string("Mustache"),
    #energy weighting: "Raw", "CalibratedNoPS", "CalibratedTotal"
    EnergyWeight = cms.string("Raw"),

    #this overrides both dphi cuts below if true!
    useDynamicDPhiWindow = cms.bool(True), 
                                              
    #PFClusters collection
    PFClusters = cms.InputTag("particleFlowClusterECAL"),
    ESAssociation = cms.InputTag("particleFlowClusterECAL"),
    BeamSpot = cms.InputTag("offlineBeamSpot"),
                                              
    PFBasicClusterCollectionBarrel = cms.string("particleFlowBasicClusterECALBarrel"),                                       
    PFSuperClusterCollectionBarrel = cms.string("particleFlowSuperClusterECALBarrel"),
    PFBasicClusterCollectionEndcap = cms.string("particleFlowBasicClusterECALEndcap"),                                       
    PFSuperClusterCollectionEndcap = cms.string("particleFlowSuperClusterECALEndcap"),
    PFBasicClusterCollectionPreshower = cms.string("particleFlowBasicClusterECALPreshower"),
    PFSuperClusterCollectionEndcapWithPreshower = cms.string("particleFlowSuperClusterECALEndcapWithPreshower"),                                          

    # are the seed thresholds Et or Energy?
    seedThresholdIsET = cms.bool(True),
    # regression setup
    useRegression = cms.bool(True),
    regressionConfig = cms.PSet(
       regressionKeyEB = cms.string('pfscecal_EBCorrection_offline_v2'),
       uncertaintyKeyEB = cms.string('pfscecal_EBUncertainty_offline_v2'),
       regressionKeyEE = cms.string('pfscecal_EECorrection_offline_v2'),
       uncertaintyKeyEE = cms.string('pfscecal_EEUncertainty_offline_v2'),
       vertexCollection = cms.InputTag("offlinePrimaryVertices"),
       ecalRecHitsEB = cms.InputTag('ecalRecHit','EcalRecHitsEB'),
       ecalRecHitsEE = cms.InputTag('ecalRecHit','EcalRecHitsEE'),
       applySigmaIetaIphiBug = cms.bool(False)
       ),
       
    #threshold for final SuperCluster Et
    thresh_SCEt = cms.double(4.0),
    
    # threshold in ECAL
    thresh_PFClusterSeedBarrel = cms.double(1.0),
    thresh_PFClusterBarrel = cms.double(0.0),

    thresh_PFClusterSeedEndcap = cms.double(1.0),
    thresh_PFClusterEndcap = cms.double(0.0),

    # window width in ECAL ( these don't mean anything for Mustache )
    phiwidth_SuperClusterBarrel = cms.double(0.6),
    etawidth_SuperClusterBarrel = cms.double(0.04),

    phiwidth_SuperClusterEndcap = cms.double(0.6),
    etawidth_SuperClusterEndcap = cms.double(0.04),

    # threshold in preshower
    thresh_PFClusterES = cms.double(0.),           

    # turn on merging of the seed cluster to its nearest neighbors
    # that share a rechit
    doSatelliteClusterMerge = cms.bool(False),
    satelliteClusterSeedThreshold = cms.double(50.0),
    satelliteMajorityFraction = cms.double(0.5),
    dropUnseedable = cms.bool(False),
    #thresh_PFClusterMustacheOutBarrel = cms.double(0.),
    #thresh_PFClusterMustacheOutEndcap = cms.double(0.), 

    #corrections
    applyCrackCorrections = cms.bool(False)
                                              
)

#define the default clustering type
particleFlowSuperClusterECAL = particleFlowSuperClusterECALMustache.clone()

from Configuration.Eras.Modifier_pp_on_AA_2018_cff import pp_on_AA_2018
pp_on_AA_2018.toModify(particleFlowSuperClusterECAL, useDynamicDPhiWindow = False)
pp_on_AA_2018.toModify(particleFlowSuperClusterECAL, phiwidth_SuperClusterBarrel = 0.20)
pp_on_AA_2018.toModify(particleFlowSuperClusterECAL, phiwidth_SuperClusterEndcap = 0.20)
