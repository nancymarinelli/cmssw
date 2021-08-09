#ifndef ECAL_PHASE2_AMPLITUDE_RECONSTRUCTOR_H
#define ECAL_PHASE2_AMPLITUDE_RECONSTRUCTOR_H

#include <vector>
#include <cstdint>

class EcalPhase2TPGWeightIdMap;
class EcalTPGWeightGroup;

  /** 
   \ class EcalPhase2AmplitudeReconstructor
   
   *  input: ?? bits
   *  output: ?? bits
   *  
   */
class EcalPhase2AmplitudeReconstructor {


 private:
  int inputsAlreadyIn_;
  int buffer_[12];
  int weights_[12];
  int shift_;
  int setInput(int input);
  void process();
  
  int processedOutput_;

 public:
  EcalPhase2AmplitudeReconstructor();
  virtual ~EcalPhase2AmplitudeReconstructor();
  virtual void process(std::vector<int> & addout, std::vector<int> & output);
  void setParameters(uint32_t raw,const EcalPhase2TPGWeightIdMap * ecaltpgWeightMap, const EcalTPGWeightGroup *ecaltpgWeightGroup );
  
};

#endif

