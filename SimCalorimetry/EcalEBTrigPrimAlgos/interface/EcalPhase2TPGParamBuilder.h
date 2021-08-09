#ifndef ECALPHASE2TPGPARAMBUILDER_H
#define ECALPHASE2TPGPARAMBUILDER_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/EcalDigi/interface/EcalLiteDTUSample.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "CondFormats/EcalObjects/interface/EcalLiteDTUPedestals.h"
//#include "CondFormats/EcalObjects/interface/EcalPhase2TPGLinearizationConst.h"
#include "DataFormats/EcalDigi/interface/EcalConstants.h"

//#include <CondFormats/EcalObjects/interface/EcalTPGPedestals.h>
//#include <CondFormats/EcalObjects/interface/EcalTPGLinearizationConst.h>

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "CondFormats/EcalObjects/interface/EcalTPGCrystalStatus.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

//class CaloSubdetectorGeometry;

  /** 
   \class EcalPhase2TPGParamBuilder
   \brief TPG Param Builder for Phase2
   *  
   */
  /*
  class coeffClass {
    public:
      coeffClass() {}
      int mults_[2];
      int pedestals_[2];
      int shifts_[2];
  }
  */

  //class EcalPhase2TPGParamBuilder : public edm::EDAnalyzer {
  class EcalPhase2TPGParamBuilder {
  public:
    EcalPhase2TPGParamBuilder(const edm::EventSetup & setup);
    virtual ~EcalPhase2TPGParamBuilder();
    void process(const EcalLiteDTUPedestalsMap* ecaltpPed, std::map<uint32_t,std::vector<uint>> & paramb_out);
    //void process(const EcalLiteDTUPedestalsMap* ecaltpPed, std::vector<std::vector<int>> & paramb_out);
    /*
    void setPointers(
		   const EcalLiteDTUPedestalsMap *ecaltpPed
)  
{
    parambPed_=ecaltpPed;
    }
    */
    


  private:
    bool computeLinearizerParam(double theta,float gainRatio,int& shift,int& mult);
    const float xtal_LSB_ = 0.5;
    const float calibCoeff_ = 1.;
    const EcalLiteDTUPedestalsMap * parambPed_;  
    int mult_;
    int shift_;
    int shiftTwo_ = 2;
    std::ofstream* out_file_;
    //std::vector<DetId>& ebCells_;
    static constexpr float gains_[ecalPh2::NGAINS] = {0.1, 1.};
    const CaloSubdetectorGeometry* theBarrelGeometry_;
    
    //const EcalLiteDTUPedestalsMap * parambPed_; 
    const EcalLiteDTUPedestals *peds_;


};




#endif
