#ifndef EcaltrigprimProducer_h
#define EcaltrigprimProducer_h
  
/** \class EcalEBTrigPrimProducer
 *
 * EcalTrigPrimProducer produces a EcalTrigPrimDigiCollection
 * Simulation as close as possible to hardware
 * Main algorithm is EcalTrigPrimFunctionalAlgo which is now
 * templated to take EBdataFrames/EEDataFrames as input
 *
 * \author Ursula Berthon, Stephanie Baffioni, Pascal Paganini,   LLR Palaiseau
 *
 * \version   1st Version may 2006
 * \version   2nd Version jul 2006
 * \version   3rd Version nov 2006
 * \version   4th Version apr 2007   full endcap
 *
 ************************************************************/

#include <memory>
 
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
 
#include "DataFormats/Common/interface/Handle.h"
 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
  
class EcalEBTrigPrimTestAlgo;
//class EcalRecHitCollection;

 
class EcalEBTrigPrimProducer : public edm::stream::EDProducer<>
{
 public:
  
  explicit EcalEBTrigPrimProducer(const edm::ParameterSet& conf);
  
  virtual ~EcalEBTrigPrimProducer();
  

  void beginRun(const edm::Run& run, const edm::EventSetup& es) override;
  void endRun(const edm::Run&, const edm::EventSetup&) override;
  void produce(edm::Event& e, const edm::EventSetup& c) override;

  private:
  std::unique_ptr<EcalEBTrigPrimTestAlgo> algo_;
  bool barrelOnly_;
  bool tcpFormat_;
  bool debug_;
  bool famos_;
  edm::EDGetTokenT<EcalRecHitCollection> tokenEB_;

  int binOfMaximum_;
  bool fillBinOfMaximumFromHistory_;

};
  
#endif
 


