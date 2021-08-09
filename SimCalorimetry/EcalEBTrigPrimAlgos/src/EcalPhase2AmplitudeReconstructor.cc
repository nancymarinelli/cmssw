#include <SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalPhase2AmplitudeReconstructor.h>
#include "CondFormats/EcalObjects/interface/EcalPhase2TPGWeightIdMap.h"
#include "CondFormats/EcalObjects/interface/EcalTPGWeightGroup.h"

#include "CondFormats/EcalObjects/interface/EcalTPGGroups.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <iostream>

EcalPhase2AmplitudeReconstructor::EcalPhase2AmplitudeReconstructor()
  :inputsAlreadyIn_(0), shift_(12) {
  }

EcalPhase2AmplitudeReconstructor::~EcalPhase2AmplitudeReconstructor(){}

int EcalPhase2AmplitudeReconstructor::setInput(int input)
{
  if(input>0X1FFF)
    {
      std::cout<<"ERROR IN INPUT OF AMPLITUDE FILTER"<<std::endl;
      return -1;
    }
  if(inputsAlreadyIn_<12)
    {
      //std::cout << " EcalPhase2AmplitudeReconstructor::setInput inputsAlreadyIn_<5 input " << input << std::endl;
      buffer_[inputsAlreadyIn_]=input;
      inputsAlreadyIn_++;
    }
  else
    {
      for(int i=0; i<11; i++)
      {
         buffer_[i]=buffer_[i+1];
	 //std::cout << " EcalPhase2AmplitudeReconstructor::setInput inputsAlreadyIn buffer " << buffer_[i] << std::endl; 
      }
      buffer_[11]=input;
    }
  return 1;
}

void EcalPhase2AmplitudeReconstructor::process(std::vector<int> &addout,std::vector<int> &output)
{
  // test
  std::cout << "  EcalPhase2AmplitudeReconstructor::process(std::vector<int> &addout size  " << addout.size() << std::endl;  

  inputsAlreadyIn_=0;
  for (unsigned int i =0;i<12;i++){
     buffer_[i]=0;//FIXME: 5
  }
  
  // test end

  
  for (unsigned int i =0;i<addout.size();i++){
    
    setInput(addout[i]);
    for (unsigned int j =0;j<12;j++){
      // std::cout << " buffer_ " << buffer_[j];
    }
    //std::cout << "  " << std::endl;
    if (i == 11) {
        process();
	//   this is the compression to 10 bits to go in the DF. To be done as last thing before building the TP 
        // output[0] = processedOutput_ >> 3 ;
	output[0] = processedOutput_  ;
    }
    else if (i == 15){
        process();
	//This is the compression to 10 bits to go in the DF. To be done as last thing before building the TP
        //output[1] = processedOutput_ >> 3 ;
	output[1] = processedOutput_  ;
    }
  }
  return;
}

void EcalPhase2AmplitudeReconstructor::process()
{
  //UB FIXME: 5
  processedOutput_ = 0;
  if(inputsAlreadyIn_<12) return;
  int output=0;
  for(int i=0; i<12; i++)
  {

    output+=(weights_[i]*buffer_[i]);
    //std::cout << " AmplitudeFilter buffer " << buffer_[i] << " weight " << weights_[i] << " output " << output << std::endl;
  }
  if(output<0) output=0;
  output = output >> shift_;
  if(output>0X1FFF)  output=0X1FFF;
  processedOutput_ = output;
}

void EcalPhase2AmplitudeReconstructor::setParameters(uint32_t raw,const EcalPhase2TPGWeightIdMap * ecaltpgWeightMap, const EcalTPGWeightGroup 
*ecaltpgWeightGroup )
{
  uint32_t params_[12];
  const EcalTPGGroups::EcalTPGGroupsMap & groupmap = ecaltpgWeightGroup -> getMap();
  std::cout << " EcalPhase2AmplitudeReconstructor::setParameters groupmap size " << groupmap.size() << std::endl;
  EcalTPGGroups::EcalTPGGroupsMapItr it = groupmap.find(raw);
  if (it!=groupmap.end()) {
    uint32_t weightid =(*it).second;
    const EcalPhase2TPGWeightIdMap::EcalPhase2TPGWeightMap & weightmap = ecaltpgWeightMap -> getMap();
    EcalPhase2TPGWeightIdMap::EcalPhase2TPGWeightMapItr itw = weightmap.find(weightid);
    
    (*itw).second.getValues(params_[0],params_[1],params_[2],params_[3],params_[4],params_[5],params_[6],params_[7],params_[8],params_[9],params_[10],params_[11]);

    std::cout << " EcalPhase2AmplitudeReconstructor::setParameters weights after the map  " << params_[0] << " " << params_[1] << " " << params_[2] << " " << params_[3] << " " << params_[4] << " "\
 << params_[5] << " " << params_[6] << " " << params_[7] << " " << params_[8] << " " << params_[9] << " " << params_[10] << " " << params_[11] <<  std::endl;

    // we have to transform negative coded in 13 bits into negative coded in 32 bits                                                                                                                 
    // maybe this should go into the getValue method??                                                                                                                                  

    for (int i=0;i<12;++i){
      weights_[i] = (params_[i] & 0x1000) ?    (int)( params_[i] | 0xfffff000) : (int)(params_[i]);

      std::cout << " EcalPhase2AmplitudeReconstructor::setParameters weights after the cooking " << weights_[i] << std::endl;
    }
    std::cout << std::endl;                                                                                                                                                                       
  }
  else edm::LogWarning("EcalTPG")<<" could not find EcalTPGGroupsMap entry for "<<raw;
}


