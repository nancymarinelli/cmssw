#include <SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalPhase2Linearizer.h>

//#include <CondFormats/EcalObjects/interface/EcalTPGLinearizationConst.h>
//#include <CondFormats/EcalObjects/interface/EcalTPGPedestals.h>
//#include <CondFormats/EcalObjects/interface/EcalTPGCrystalStatus.h>

#include "FWCore/MessageLogger/interface/MessageLogger.h"

EcalPhase2Linearizer::EcalPhase2Linearizer(bool famos)
  : famos_(famos), init_(false),
    peds_(nullptr),
    linConsts_(nullptr),
    badXStatus_(nullptr)
{
}

EcalPhase2Linearizer::~EcalPhase2Linearizer(){
  if (init_) {
    for (int i=0;i<(int)vectorbadXStatus_.size();i++){
      delete vectorbadXStatus_[i];
    }
  }
}

//void EcalPhase2Linearizer::setParameters(EBDetId detId, const EcalLiteDTUPedestalsMap*  ecaltpPed, const EcalTPGLinearizationConst *ecaltpLin, const EcalTPGCrystalStatus *ecaltpBadX, std::vector<std::vector<int>> & paramb_out)
void EcalPhase2Linearizer::setParameters(EBDetId detId, const EcalLiteDTUPedestalsMap*  ecaltpPed, const EcalTPGLinearizationConst *ecaltpLin, const EcalTPGCrystalStatus *ecaltpBadX, std::map<uint32_t,std::vector<uint>> & paramb_out)

{

 /*
  EcalLiteDTUPedestalsMap::const_iterator itped = ecaltpPed->getMap().find(detId);
  if (itped != ecaltpPed->end())
    peds_ = &(*itped);
  else
    std::cout << " could not find EcalLiteDTUPedestal entry for " << detId << std::endl;
  */
  coeffs_ = paramb_out[detId.rawId()];
  //coeffs_.push_back(paramb_out[detId.rawId()][0]);
  //coeffs_.push_back(paramb_out[detId.rawId()][1]);

  EcalTPGLinearizationConst::const_iterator itlin = ecaltpLin->getMap().find(detId);
  if (itlin != ecaltpLin->end())
    linConsts_ = &(*itlin);
  else
    std::cout << " could not find EcalTPGLinearizationConst entry for " << detId << std::endl;


  const EcalTPGCrystalStatusMap &badXMap = ecaltpBadX->getMap();
  EcalTPGCrystalStatusMapIterator itbadX = badXMap.find(detId.rawId());

  if (itbadX != badXMap.end()) {
    badXStatus_ = &(*itbadX);
  } else {
    edm::LogWarning("EcalTPG") << " could not find EcalTPGCrystalStatusMap entry for " << detId.rawId();
    badXStatus_ = new EcalTPGCrystalStatusCode();
    vectorbadXStatus_.push_back(&(*badXStatus_));
    init_ = true;
  }


}

int EcalPhase2Linearizer::doIt()
{
  
  
  int output = (uncorrectedSample_ - base_);  //Substract base
  if (output < 0) {
    output = 0;
  }
  output = (output * mult_) >> shift_;
  // protect against saturation
  // ...........


  if(famos_ || output<0) return 0;
  
  return output;
}
 
int EcalPhase2Linearizer::setInput(const EcalLiteDTUSample &RawSam)

{

  

  uncorrectedSample_ = RawSam.adc();  //uncorrectedSample_ 
  gainID_ = RawSam.gainId();
  coeff_ = coeffs_[gainID_];

  base_ = coeff_ & 0xFFF;
  shift_ = (coeff_ & 0xF000) >> 12;
  mult_ = (coeff_ & 0xFF0000) >> 16;

  //base_ = peds_->mean(gainID_);
  /*
  if ( gainID_ == 0){
    mult_ = linConsts->mult_x10;
    shift_ = linConsts->shift_x10;
  }
  else {
    mult_ = linConsts->mult_x1;
    shift_ = linConsts->shift_x1;
  }
  */
  //mult_ = gainDivideByTen_; // gain 10
  
  //if ( gainID_ == 1) mult_=1.;

  //std::cout << " EcalPhase2Linearizer::setInput() RawSam.raw() " << RawSam.raw() <<  " Gain " << gainID_ << " Ped " << base_ << " mult " << mult_ << std::endl;




  if (famos_)
    base_ = 200;  //FIXME by preparing a correct TPG.txt for Famos

  return 1;

}

void EcalPhase2Linearizer::process(const EBDigiCollectionPh2::Digi &df, std::vector<int> & output_percry) 
{

  //We know a tower numbering is:                                                                                                                               
  // S1 S2 S3 S4 S5                                                                                                                                               
  //                                                                                                                                                              
  // 4  5  14 15 24                                                                                                                                               
  // 3  6  13 16 23                                                                                                                                               
  // 2  7  12 17 22                                                                                                                                               
  // 1  8  11 18 21                                                                                                                                               
  // 0  9  10 19 20                                                                                                                                               
                                                                                                                                                                
 
  //  for (int i=0;i<df.size();i++) {                                                                                                                               
  // std::cout <<  df[i] << " ";                                                                                                                                 
  //}                                                                                                                                                             
  //std::cout  << " " << std::endl;;                                                                                                                                 

  std::cout << " mult " << " " ;
  for (int i=0;i<df.size();i++) {                                                                                                                      
    EcalLiteDTUSample thisSample = df[i];         
    setInput(thisSample);       
    std::cout << mult_ << " ";                                                                                                                               
    output_percry[i]=doIt();                                                                                                                                    
  }                                                                                                                                                             
  std::cout  << " " << std::endl;                                                                                                                                 
                                                                                                                                                                
  //std::cout << " EcalPhase2Linearizer::process(const  .. Final output " << std::endl;                                                                           
  //for (int i=0;i<df.size();i++) {                                                                                                                               
    //std::cout << " output_percry " << output_percry[i]<< " ";                                                                                                    
    
  //}                                                                                                                                                             
                                                                                                                                                                
  return;           

}
