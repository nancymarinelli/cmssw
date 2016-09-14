#ifndef EcalEBTrigPrimTestAlgo_h
#define EcalEBTrigPrimTestAlgo_h
/** \class EcalEBTrigPrimTestAlgo
 * forPhase II 
 * As of now we do not know yet how the electronics would look like
 * so for now we build some machinery to produce TPs which are taken from the RecHits
 *
 ************************************************************/
#include <sys/time.h>
#include <iostream>
#include <vector>

#include "Geometry/CaloTopology/interface/EcalTrigTowerConstituentsMap.h"

#include "DataFormats/Common/interface/SortedCollection.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"


 

#include <map>
#include <utility>


class EcalTrigTowerDetId;
class ETPCoherenceTest;
class EcalTriggerPrimitiveSample;
class CaloSubdetectorGeometry;
class EBDataFrame;


 
class EcalEBTrigPrimTestAlgo
{  
 public:
  
  explicit EcalEBTrigPrimTestAlgo(const edm::EventSetup & setup, int binofmax, bool tcpFormat, bool barrelOnly, bool debug, bool famos);

  virtual ~EcalEBTrigPrimTestAlgo();

  
  void run(const edm::EventSetup &, const EcalRecHitCollection *col, EcalTrigPrimDigiCollection & result, EcalTrigPrimDigiCollection & resultTcp);
 

 private:

  void init(const edm::EventSetup&);
  template <class T>  
    void initStructures(std::vector<std::vector<std::pair<int,std::vector<T> > > > & towMap);
  template <class T> 
    void clean(std::vector<std::vector<std::pair<int,std::vector<T> > > > &towerMap);
  template <class Coll> 
    void fillMap(Coll const * col, std::vector<std::vector<std::pair<int,std::vector<typename Coll::Digi> > > > &towerMap);
  int findStripNr(const EBDetId &id);
  int findStripNr(const EEDetId &id);

  // FIXME: temporary until hashedIndex works alsom for endcap
  int getIndex(const  EBDigiCollection *, EcalTrigTowerDetId& id) {return id.hashedIndex();}
  // mind that eta is continuous between barrel+endcap
  int getIndex(const  EEDigiCollection *, EcalTrigTowerDetId& id) {
    int ind=(id.ietaAbs()-18)*72 + id.iphi();
    if (id.zside()<0) ind+=792;
    return ind;
  }

  edm::ESHandle<EcalTrigTowerConstituentsMap> eTTmap_;
  //  const CaloSubdetectorGeometry *theEndcapGeometry;
  edm::ESHandle<CaloGeometry> theGeometry;


  float threshold;

  int binOfMaximum_;
  int maxNrSamples_;

  bool tcpFormat_;
  bool barrelOnly_;
  bool debug_;  
  bool famos_;  


  int nrTowers_;   // nr of towers found by fillmap method

  // data structures kept during the whole run
  std::vector<std::vector<int> > striptp_;
  std::vector<std::vector<std::pair<int,std::vector<EBDataFrame> > > > towerMapEB_;
  std::vector<std::vector<std::pair<int,std::vector<EEDataFrame> > > > towerMapEE_;
  std::vector<std::pair<int,EcalTrigTowerDetId> > hitTowers_;
  std::vector<EcalTriggerPrimitiveSample> towtp_;
  std::vector<EcalTriggerPrimitiveSample> towtp2_;

  enum {nbMaxStrips_=5};
  enum {nbMaxXtals_=5};
};



#endif
