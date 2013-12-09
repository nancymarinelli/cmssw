#include "RecoEgamma/EgammaIsolationAlgos/plugins/ParticleBasedIsoProducer.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"


#include "DataFormats/Common/interface/ValueMap.h"



ParticleBasedIsoProducer::ParticleBasedIsoProducer(const edm::ParameterSet& conf) : conf_(conf) {

  photonTmpProducer_       = conf_.getParameter<edm::InputTag>("photonTmpProducer");
  photonProducer_       = conf_.getParameter<edm::InputTag>("photonProducer");
  electronProducer_     = conf_.getParameter<edm::InputTag>("electronProducer");
  electronTmpProducer_     = conf_.getParameter<edm::InputTag>("electronTmpProducer");

  photonProducerT_   = 
    consumes<reco::PhotonCollection>(photonProducer_);


  photonTmpProducerT_   = 
    consumes<reco::PhotonCollection>(photonTmpProducer_);


  electronProducerT_   = 
    consumes<reco::GsfElectronCollection>(electronProducer_);

  electronTmpProducerT_   = 
    consumes<reco::GsfElectronCollection>(electronTmpProducer_);

  pfCandidates_      = 
    consumes<reco::PFCandidateCollection>(conf_.getParameter<edm::InputTag>("pfCandidates"));
  
  pfEgammaCandidates_      = 
    consumes<reco::PFCandidateCollection>(conf_.getParameter<edm::InputTag>("pfEgammaCandidates"));

  valueMapPFCandPhoton_ = conf_.getParameter<std::string>("valueMapPhoToEG");
  valueMapPhoPFCandIso_ = conf_.getParameter<std::string>("valueMapPhoPFblockIso");
  valueMapPFCandEle_    = conf_.getParameter<std::string>("valueMapEleToEG");
  valueMapElePFCandIso_ = conf_.getParameter<std::string>("valueMapElePFblockIso");

  produces< edm::ValueMap<std::vector<std::pair<reco::PFCandidateRef, bool> > > > (valueMapPhoPFCandIso_); 
  produces< edm::ValueMap<std::vector<std::pair<reco::PFCandidateRef, bool> > > > (valueMapElePFCandIso_); 

}

ParticleBasedIsoProducer::~ParticleBasedIsoProducer() {


}


void ParticleBasedIsoProducer::beginRun(const edm::Run & run, const edm::EventSetup& c) {

    thePFBlockBasedIsolation_ = new PfBlockBasedIsolation();

}

void ParticleBasedIsoProducer::endRun(const edm::Run & run, const edm::EventSetup& c) {

  delete thePFBlockBasedIsolation_;

}



void ParticleBasedIsoProducer::produce(edm::Event& theEvent, const edm::EventSetup& c) {

  edm::Handle<reco::PhotonCollection> photonHandle;
  theEvent.getByToken(photonProducerT_,photonHandle);
  if ( photonHandle.isValid()) {
  } else {
    edm::LogError("ParticleBasedIsoProducer") << "Error! Can't get the product " <<   photonProducer_.label() << "\n";
  }



  edm::Handle<reco::PhotonCollection> photonTmpHandle;
  theEvent.getByToken(photonTmpProducerT_,photonTmpHandle);
  if ( photonTmpHandle.isValid()) {
  } else {
    edm::LogError("ParticleBasedIsoProducer") << "Error! Can't get the product " <<   photonProducer_.label() << "\n";
  }




  edm::Handle<reco::GsfElectronCollection> electronTmpHandle;
  theEvent.getByToken(electronTmpProducerT_,electronTmpHandle);
  if ( electronTmpHandle.isValid()) {
  } else {
    edm::LogError("ParticleBasedIsoProducer") << "Error! Can't get the product " <<   electronTmpProducer_.label() << "\n";
  }


  edm::Handle<reco::GsfElectronCollection> electronHandle;
  theEvent.getByToken(electronProducerT_,electronHandle);
  if ( electronHandle.isValid()) {
  } else {
    edm::LogError("ParticleBasedIsoProducer") << "Error! Can't get the product " <<   electronProducer_.label() << "\n";
  }



  
  edm::Handle<reco::PFCandidateCollection> pfEGCandidateHandle;
  // Get the  PF refined cluster  collection
  theEvent.getByToken(pfEgammaCandidates_,pfEGCandidateHandle);
  if (!pfEGCandidateHandle.isValid()) {
    edm::LogError("ParticleBasedIsoProducer") 
      << "Error! Can't get the pfEgammaCandidates";
  }
  
  edm::Handle<reco::PFCandidateCollection> pfCandidateHandle;
  // Get the  PF candidates collection
  theEvent.getByToken(pfCandidates_,pfCandidateHandle);
  if (!pfCandidateHandle.isValid()) {
    edm::LogError("ParticleBasedIsoProducer") 
      << "Error! Can't get the pfCandidates";
  }
  
  edm::ValueMap<reco::PhotonRef> pfEGCandToPhotonMap;
  edm::Handle<edm::ValueMap<reco::PhotonRef> > pfEGCandToPhotonMapHandle;
  theEvent.getByLabel("gedPhotonsTmp",valueMapPFCandPhoton_,pfEGCandToPhotonMapHandle);
  if ( ! pfEGCandToPhotonMapHandle.isValid()) {
    edm::LogInfo("ParticleBasedIsoProducer") << "Error! Can't get the product: valueMapPhotons " << std::endl;
  }
  pfEGCandToPhotonMap = *(pfEGCandToPhotonMapHandle.product());



  edm::ValueMap<reco::GsfElectronRef> pfEGCandToElectronMap;
  edm::Handle<edm::ValueMap<reco::GsfElectronRef> > pfEGCandToElectronMapHandle;
  theEvent.getByLabel("gedGsfElectronsTmp",valueMapPFCandEle_,pfEGCandToElectronMapHandle);
  if ( ! pfEGCandToElectronMapHandle.isValid()) {
    edm::LogInfo("ParticleBasedIsoProducer") << "Error! Can't get the product: valueMapElectron " << std::endl;
  }
  pfEGCandToElectronMap = *(pfEGCandToElectronMapHandle.product());



  std::vector<std::vector<std::pair<reco::PFCandidateRef, bool>>> pfCandIsoPairVecPho;
  std::vector<std::pair<reco::PFCandidateRef, bool>> pfCandIsoPair;
  ///// Isolation for photons 
  //  std::cout << " ParticleBasedIsoProducer  photonHandle size " << photonHandle->size() << std::endl;
  for(unsigned int lSC=0; lSC < photonTmpHandle->size(); lSC++) {
    reco::PhotonRef phoRef(reco::PhotonRef(photonTmpHandle, lSC));

    // loop over the unbiased candidates to retrieve the ref to the unbiased candidate corresponding to this photon
    unsigned nObj = pfEGCandidateHandle->size();
    reco::PFCandidateRef pfEGCandRef;

    std::vector<std::pair<reco::PFCandidateRef, bool>> pfCandIsoPair;
    for(unsigned int lCand=0; lCand < nObj; lCand++) {
      pfEGCandRef=reco::PFCandidateRef(pfEGCandidateHandle,lCand);
      reco::PhotonRef myPho= (pfEGCandToPhotonMap)[pfEGCandRef];
      
      if ( myPho.isNonnull() ) {
	//std::cout << "ParticleBasedIsoProducer photons PF SC " << pfEGCandRef->superClusterRef()->energy() << " Photon SC " << myPho->superCluster()->energy() << std::endl;
	if (myPho != phoRef) continue;
	//std::cout << " ParticleBasedIsoProducer photons This is my egammaunbiased guy energy " <<  pfEGCandRef->superClusterRef()->energy() << std::endl;
	pfCandIsoPair=thePFBlockBasedIsolation_->calculate (myPho->p4(),  pfEGCandRef, pfCandidateHandle);
	/////// debug
	//	for ( std::vector<std::pair<reco::PFCandidateRef, bool>>::iterator iPair=pfCandIsoPair.begin(); iPair<pfCandIsoPair.end(); iPair++) {
	//  std::cout << " ParticleBasedIsoProducer photons  checking the pfCand bool pair " << (iPair->first)->particleId() << " " <<  iPair->second << std::endl; 
	//	}
	
	
      }
      
    }
    
    pfCandIsoPairVecPho.push_back(pfCandIsoPair); 
  }
 


  ////////////isolation for electrons 
  std::vector<std::vector<std::pair<reco::PFCandidateRef, bool>>> pfCandIsoPairVecEle;
  pfCandIsoPair.clear();
  // std::cout << " cleaning pfCandIsoPair size " << pfCandIsoPair.size() <<  std::endl;
  //  std::cout << " ParticleBasedIsoProducer  electronHandle size " << electronHandle->size() << std::endl;
  for(unsigned int lSC=0; lSC < electronTmpHandle->size(); lSC++) {
    reco::GsfElectronRef eleRef(reco::GsfElectronRef(electronTmpHandle, lSC));

    // loop over the unbiased candidates to retrieve the ref to the unbiased candidate corresponding to this electron
    unsigned nObj = pfEGCandidateHandle->size();
    reco::PFCandidateRef pfEGCandRef;

    std::vector<std::pair<reco::PFCandidateRef, bool>> pfCandIsoPair;
    for(unsigned int lCand=0; lCand < nObj; lCand++) {
      pfEGCandRef=reco::PFCandidateRef(pfEGCandidateHandle,lCand);
      reco::GsfElectronRef myEle= (pfEGCandToElectronMap)[pfEGCandRef];
      
      if ( myEle.isNonnull() ) {
	//std::cout << "ParticleBasedIsoProducer Electorns PF SC " << pfEGCandRef->superClusterRef()->energy() << " Photon SC " << myEle->superCluster()->energy() << std::endl;
	if (myEle != eleRef) continue;
	//	std::cout << " ParticleBasedIsoProducer  Electrons This is my egammaunbiased guy energy " <<  pfEGCandRef->superClusterRef()->energy() << std::endl;
	pfCandIsoPair=thePFBlockBasedIsolation_->calculate (myEle->p4(),  pfEGCandRef, pfCandidateHandle);
	/////// debug
	//for ( std::vector<std::pair<reco::PFCandidateRef, bool>>::iterator iPair=pfCandIsoPair.begin(); iPair<pfCandIsoPair.end(); iPair++) {
	// std::cout << " ParticleBasedIsoProducer Electrons  checking the pfCand bool pair " << (iPair->first)->particleId() << " " <<  iPair->second << std::endl; 
	//	}
	
	
      }
      
    }
    
    pfCandIsoPairVecEle.push_back(pfCandIsoPair); 
  }
 




  std::auto_ptr<edm::ValueMap<std::vector<std::pair<reco::PFCandidateRef, bool>>> >  
    phoToPFCandIsoMap_p(new edm::ValueMap<std::vector<std::pair<reco::PFCandidateRef, bool>>>());
  edm::ValueMap<std::vector<std::pair<reco::PFCandidateRef, bool>>>::Filler 
    fillerPhotons(*phoToPFCandIsoMap_p);
  
  //// fill the isolation value map for photons
  //  std::cout << " ParticleBasedIsoProducer::produce  pfCandIsoPairVecPho size " << pfCandIsoPairVecPho.size() << std::endl;
  fillerPhotons.insert(photonHandle,pfCandIsoPairVecPho.begin(),pfCandIsoPairVecPho.end());
  fillerPhotons.fill(); 
  theEvent.put(phoToPFCandIsoMap_p,valueMapPhoPFCandIso_);


  std::auto_ptr<edm::ValueMap<std::vector<std::pair<reco::PFCandidateRef, bool>>> >  
    eleToPFCandIsoMap_p(new edm::ValueMap<std::vector<std::pair<reco::PFCandidateRef, bool>>>());
  edm::ValueMap<std::vector<std::pair<reco::PFCandidateRef, bool>>>::Filler 
    fillerElectrons(*eleToPFCandIsoMap_p);
  
  //// fill the isolation value map for electrons 
  //  std::cout << " ParticleBasedIsoProducer::produce  pfCandIsoPairVecEle  size " << pfCandIsoPairVecEle.size() << std::endl;
  fillerElectrons.insert(electronHandle,pfCandIsoPairVecEle.begin(),pfCandIsoPairVecEle.end());
  fillerElectrons.fill(); 
  theEvent.put(eleToPFCandIsoMap_p,valueMapElePFCandIso_);









 
}
