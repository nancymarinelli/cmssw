#ifndef ECAL_PHASE2_TP_FORMAT_H
#define ECAL_PHASE2_TP_FORMAT_H

#include "DataFormats/EcalDigi/interface/EcalEBTriggerPrimitiveSample.h"

#include <vector>
#include <cstdint>


/* 
    \class EcalPhase2TPFormat

*/

class EcalPhase2TPFormat {
private:

  std::vector<int>  inputAmp_;
  std::vector<int64_t>  inputTime_;

  int setInput(std::vector<int> & inputAmp, std::vector<int64_t> &inputTime);
  

public:
  EcalPhase2TPFormat();
  virtual ~EcalPhase2TPFormat();
  virtual void process(std::vector<int> & ampl, std::vector<int64_t> & time, std::vector<int> & outampl, std::vector<int64_t> & outtime);

};
#endif
