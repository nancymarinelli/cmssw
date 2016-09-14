/** \class EcalEBTrigPrimProducer
 * For Phase II
 * EcalEBTrigPrimProducer produces a EcalEBTrigPrimDigiCollection
 * out of RecHits. This is a simple starting point to fill in the chain
 * for Phase II
 * 
 *
 *
 ************************************************************/
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Provenance/interface/ProductID.h"
#include "DataFormats/Provenance/interface/ParameterSetID.h"
#include "DataFormats/Provenance/interface/Provenance.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"



/*
#include "CondFormats/DataRecord/interface/EcalTPGFineGrainEBGroupRcd.h"
#include "CondFormats/DataRecord/interface/EcalTPGLutGroupRcd.h"
#include "CondFormats/DataRecord/interface/EcalTPGLutIdMapRcd.h"
#include "CondFormats/DataRecord/interface/EcalTPGFineGrainEBIdMapRcd.h"
#include "CondFormats/DataRecord/interface/EcalTPGFineGrainTowerEERcd.h"
#include "CondFormats/DataRecord/interface/EcalTPGCrystalStatusRcd.h"
#include "CondFormats/DataRecord/interface/EcalTPGTowerStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalTPGFineGrainEBGroup.h"
#include "CondFormats/EcalObjects/interface/EcalTPGLutGroup.h"
#include "CondFormats/EcalObjects/interface/EcalTPGLutIdMap.h"
#include "CondFormats/EcalObjects/interface/EcalTPGFineGrainEBIdMap.h"
#include "CondFormats/EcalObjects/interface/EcalTPGFineGrainTowerEE.h"
#include "CondFormats/DataRecord/interface/EcalTPGLinearizationConstRcd.h"
#include "CondFormats/DataRecord/interface/EcalTPGPedestalsRcd.h"
#include "CondFormats/DataRecord/interface/EcalTPGSlidingWindowRcd.h"
#include "CondFormats/DataRecord/interface/EcalTPGWeightIdMapRcd.h"
#include "CondFormats/DataRecord/interface/EcalTPGWeightGroupRcd.h"
#include "CondFormats/DataRecord/interface/EcalTPGFineGrainStripEERcd.h"
#include "CondFormats/EcalObjects/interface/EcalTPGLinearizationConst.h"
#include "CondFormats/EcalObjects/interface/EcalTPGPedestals.h"
#include "CondFormats/EcalObjects/interface/EcalTPGSlidingWindow.h"
#include "CondFormats/EcalObjects/interface/EcalTPGWeightIdMap.h"
#include "CondFormats/EcalObjects/interface/EcalTPGWeightGroup.h"
#include "CondFormats/EcalObjects/interface/EcalTPGFineGrainStripEE.h"
#include "CondFormats/EcalObjects/interface/EcalTPGCrystalStatus.h"
#include "CondFormats/EcalObjects/interface/EcalTPGTowerStatus.h"
#include "CondFormats/DataRecord/interface/EcalTPGSpikeRcd.h"
#include "CondFormats/EcalObjects/interface/EcalTPGSpike.h"
#include "CondFormats/DataRecord/interface/EcalTPGStripStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalTPGStripStatus.h"
*/

#include "EcalEBTrigPrimProducer.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalEBTrigPrimTestAlgo.h"

EcalEBTrigPrimProducer::EcalEBTrigPrimProducer(const edm::ParameterSet&  iConfig):
  barrelOnly_(iConfig.getParameter<bool>("BarrelOnly")),
  tcpFormat_(iConfig.getParameter<bool>("TcpOutput")),
  debug_(iConfig.getParameter<bool>("Debug")),
  famos_(iConfig.getParameter<bool>("Famos"))

{  
  tokenEB_=consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("barrelEcalHits"));
  //register your products
  produces <EcalTrigPrimDigiCollection >();
  if (tcpFormat_) produces <EcalTrigPrimDigiCollection >("formatTCP");
}



void EcalEBTrigPrimProducer::beginRun(edm::Run const & run,edm::EventSetup const& setup) {
  //ProcessHistory is guaranteed to be constant for an entire Run
  //binOfMaximum_ = findBinOfMaximum(fillBinOfMaximumFromHistory_,binOfMaximum_,run.processHistory());

  algo_.reset( new EcalEBTrigPrimTestAlgo(setup,binOfMaximum_,tcpFormat_,barrelOnly_,debug_,famos_) );

}

void EcalEBTrigPrimProducer::endRun(edm::Run const& run,edm::EventSetup const& setup) {
  algo_.reset();
}


EcalEBTrigPrimProducer::~EcalEBTrigPrimProducer()
{}


// ------------ method called to produce the data  ------------
void
EcalEBTrigPrimProducer::produce(edm::Event& e, const edm::EventSetup&  iSetup)
{


  // get input collections
  edm::Handle<EcalRecHitCollection> barrelHitHandle;
  if (! e.getByToken(tokenEB_,barrelHitHandle)) {
    edm::EDConsumerBase::Labels labels;
    labelsForToken(tokenEB_, labels);
    edm::LogWarning("EcalTPG") <<" Couldnt find Barrel rechits "<<labels.module<<" and label "<<labels.productInstance<<"!!!";
  }



  //LogDebug("EcalTPG") <<" =================> Treating event  "<<e.id()<<", Number of EB rechits "<<barrelHitHandle.product()->size();
  std::cout << "EcalTPG" <<" =================> Treating event  "<<e.id()<<", Number of EB rechits "<<barrelHitHandle.product()->size() << std::endl;

  auto pOut = std::make_unique<EcalTrigPrimDigiCollection>();
  auto pOutTcp = std::make_unique<EcalTrigPrimDigiCollection>();
 

  // invoke algorithm 

  const EcalRecHitCollection *ebrh=NULL;
  ebrh=barrelHitHandle.product();
  algo_->run(iSetup,ebrh,*pOut,*pOutTcp);
  std::cout << "produce" << " For Barrel  "<<pOut->size()<<" TP  Digis were produced" << std::endl;

  //  debug prints if TP >0

  for (unsigned int i=0;i<pOut->size();++i) {
   
    for (int isam=0;isam<(*pOut)[i].size();++isam) {
      std::cout << " (*pOut)[i][isam].raw() "  <<  (*pOut)[i][isam].raw() << std::endl;
      std::cout << " (*pOut)[i][isam].compressedEt() "  <<  (*pOut)[i][isam].compressedEt() << std::endl;
    }
    std::cout << "EcalTPG" <<" For tower  "<<(((*pOut)[i])).id()<<", TP is "<<(*pOut)[i];
  }
  std::cout << "EcalTPG" <<"\n =================> For Barrel , "<<pOut->size()<<" TP  Digis were produced (including zero ones)" << std::endl;
 
   

  // put result into the Event
  e.put(std::move(pOut));
  if (tcpFormat_) e.put(std::move(pOutTcp),"formatTCP");
}
