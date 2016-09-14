/** \class EcalEBTrigPrimTestAlgo
 *
 * EcalEBTrigPrimTestAlgo 
 * starting point for Phase II: build TPs out of RecHit to start building the
 * infrastructures
 *
 *
 ************************************************************/
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>

#include "FWCore/MessageLogger/interface/MessageLogger.h"

//#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
//#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
#include "Geometry/EcalMapping/interface/EcalMappingRcd.h"

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalEBTrigPrimTestAlgo.h"

#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDigi/interface/EBDataFrame.h"
#include "DataFormats/EcalDigi/interface/EEDataFrame.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EcalTriggerElectronicsId.h"

#include "CondFormats/EcalObjects/interface/EcalTPGPedestals.h"
#include "CondFormats/DataRecord/interface/EcalTPGPedestalsRcd.h"



#include <TTree.h>
#include <TMath.h>


//----------------------------------------------------------------------

EcalEBTrigPrimTestAlgo::EcalEBTrigPrimTestAlgo(const edm::EventSetup & setup,int binofmax,bool tcpFormat, bool barrelOnly,bool debug,bool famos): binOfMaximum_(binofmax),
  tcpFormat_(tcpFormat), barrelOnly_(barrelOnly), debug_(debug), famos_(famos)

{

 this->init(setup);
}

//----------------------------------------------------------------------
void EcalEBTrigPrimTestAlgo::init(const edm::EventSetup & setup) {
  if (!barrelOnly_) {
    //edm::ESHandle<CaloGeometry> theGeometry;
    //    edm::ESHandle<CaloSubdetectorGeometry> theEndcapGeometry_handle;
    setup.get<CaloGeometryRecord>().get( theGeometry );
    setup.get<IdealGeometryRecord>().get(eTTmap_);
  }
  // endcap mapping
  edm::ESHandle< EcalElectronicsMapping > ecalmapping;
  setup.get< EcalMappingRcd >().get(ecalmapping);
  //theMapping_ = ecalmapping.product();


  
}
//----------------------------------------------------------------------

EcalEBTrigPrimTestAlgo::~EcalEBTrigPrimTestAlgo() 
{

}
//----------------------------------------------------------------------
void EcalEBTrigPrimTestAlgo::run(const edm::EventSetup & setup, EcalRecHitCollection const * rh,
				 EcalTrigPrimDigiCollection & result,
				 EcalTrigPrimDigiCollection & resultTcp)
{

  std::cout << "  EcalEBTrigPrimTestAlgo: Testing that the algorythm is well plugged " << std::endl;
  std::cout << "  EcalEBTrigPrimTestAlgo: recHit size " << rh->size() << std::endl;

  edm::ESHandle<CaloSubdetectorGeometry> theBarrelGeometry_handle;
  setup.get<EcalBarrelGeometryRecord>().get("EcalBarrel",theBarrelGeometry_handle);
  const CaloSubdetectorGeometry *theBarrelGeometry;
  theBarrelGeometry = &(*theBarrelGeometry_handle);

  
  EcalTriggerPrimitiveDigi tp;
  std::vector<EcalTriggerPrimitiveSample> tpSam[10];

  int nSample=1;  
  uint16_t fEt;

  for (unsigned int i=0;i<rh->size();i++) {
    const EBDetId & myid1=(*rh)[i].id();
    if ( (*rh)[i].energy() < 0.2 ) continue;
    tp=  EcalTriggerPrimitiveDigi(  myid1);   
    tp.setSize(nSample);
    int nSam=0;

    for (int iSample=0; iSample<nSample; iSample++) {
      std::cout << " DetId " << myid1 << "Subdetector " << myid1.subdet() << " ieta " << myid1.ieta() << " iphi " << myid1.iphi() << std::endl;
      
      float theta =  theBarrelGeometry->getGeometry(myid1)->getPosition().theta();
      uint16_t et=((*rh)[i].energy())*sin(theta);
      fEt=et;
      std::cout << " Et before formatting " << et << " " << fEt << std::endl;
      if (fEt >0xfff)
	fEt=0xfff;
      fEt >>=2;
      if (fEt>0x3ff) fEt=0x3ff;
      std::cout << " Et after formatting " << fEt << std::endl;
      EcalTriggerPrimitiveSample mysam(et);
      tp.setSample(nSam, mysam );
      nSam++;
      std::cout << "in TestAlgo" <<" tp size "<<tp.size() << std::endl;
    }
 
    

    if (!tcpFormat_)
      result.push_back(tp);
    else 
      resultTcp.push_back(tp);

   
    std::cout << " result size " << result.size() << std::endl;

  }

  

}

//----------------------------------------------------------------------

int  EcalEBTrigPrimTestAlgo::findStripNr(const EBDetId &id){
  int stripnr;
  int n=((id.ic()-1)%100)/20; //20 corresponds to 4 * ecal_barrel_crystals_per_strip FIXME!!
  if (id.ieta()<0) stripnr = n+1;
  else stripnr =nbMaxStrips_ - n; 
  return stripnr;
}

