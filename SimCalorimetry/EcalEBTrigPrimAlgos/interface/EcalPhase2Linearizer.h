#ifndef ECAL_PHASE2_LINEARIZER_H
#define ECAL_PHASE2_LINEARIZER_H


#include "DataFormats/EcalDigi/interface/EcalLiteDTUSample.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "CondFormats/EcalObjects/interface/EcalLiteDTUPedestals.h"
//#include "CondFormats/EcalObjects/interface/EcalPhase2TPGLinearizationConst.h"

//#include <CondFormats/EcalObjects/interface/EcalTPGPedestals.h>
#include <CondFormats/EcalObjects/interface/EcalTPGLinearizationConst.h>

#include "CondFormats/EcalObjects/interface/EcalTPGCrystalStatus.h"

#include <vector> 

  /** 
   \class EcalPhase2Linearizer
   \brief Linearisation for Phase2 
   *  input: ??  bits  corresponding to input EBDataFrame
   *  output: ?? bits 
   *  
   */

  

  class EcalPhase2Linearizer  {


  private:
    bool famos_;
    int uncorrectedSample_;
    int gainID_;
    uint base_;
    uint mult_;
    uint shift_;
    int strip_;
    bool init_;
    float gainDivideByTen_ = 0.1;
    std::vector<uint> coeffs_;
    uint coeff_;
    
    const EcalLiteDTUPedestals *peds_;
    const EcalTPGLinearizationConstant  *linConsts_;
    //const EcalPhase2TPGLinearizationConstant  *linConsts_;
    const EcalTPGCrystalStatusCode *badXStatus_;
    
    std::vector<const EcalTPGCrystalStatusCode *> vectorbadXStatus_;
     	
    int setInput(const EcalLiteDTUSample  &RawSam) ;
    
 

    int doIt() ;


  public:
    EcalPhase2Linearizer(bool famos);
    virtual ~EcalPhase2Linearizer();

    void process(const EBDigiCollectionPh2::Digi &df,std::vector<int> & output_percry); 
    //void setParameters(EBDetId id, const EcalLiteDTUPedestalsMap* peds, const EcalTPGLinearizationConst *ecaltpLin, const EcalTPGCrystalStatus *ecaltpBadX, std::vector<std::vector<int>> & paramb_out); 
    void setParameters(EBDetId id, const EcalLiteDTUPedestalsMap* peds, const EcalTPGLinearizationConst *ecaltpLin, const EcalTPGCrystalStatus *ecaltpBadX, std::map<uint32_t,std::vector<uint>> & paramb_out);


};




#endif
