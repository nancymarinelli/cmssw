#include <SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalPhase2TPFormat.h>
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <iostream>

EcalPhase2TPFormat::EcalPhase2TPFormat()  {}

EcalPhase2TPFormat::~EcalPhase2TPFormat() {}

int EcalPhase2TPFormat::setInput(std::vector<int> & inputAmp, std::vector<int64_t> &  inputTime) {

  std::cout << "  EcalPhase2TPFormat::setInput  " << std::endl;
  int size=inputAmp.size();
  inputAmp_.resize(size);
  inputTime_.resize(size);


  for (unsigned int i = 0; i<inputAmp.size(); ++i ) {
    inputAmp_[i]  =inputAmp[i];
    inputTime_[i] =inputTime[i];
  }


  std::cout << "  EcalPhase2TPFormat::setInput  end of setInput" << std::endl;
  return 0;
}



void EcalPhase2TPFormat::process(std::vector<int> & amp,
				 std::vector<int64_t> & time,
				 std::vector<int> &outEt,
                                 std::vector<int64_t> &outTime) {
  
  //  setInput(amp, time);
  //  std::cout << "  EcalPhase2TPFormat::process(std::vector<int> & amp,std::vector<int64_t> & time,  std::vector<int> &output after setInput  " << std::endl;
  unsigned int size=amp.size();
  outEt.resize(size);
  outTime.resize(size);
  
  for (unsigned int i = 0; i<size; ++i ) {
    outEt[i]  = amp[i];
    outTime[i] = time[i];
  }


  for (unsigned int i = 0; i<size; ++i ) {
    // this is the energy compression to 10 bits to go in the DF. To be done as last thing before building the TP                                                                                             
    outEt[i] = outEt[i] >> 3 ;
    if (outEt[i] > 0x3FF ) outEt[i] = 0x3FF;


  }


  for (unsigned int i = 0; i<size; ++i ) {
    // this is the time compression to 5 bits to go in the DF. To be done as last thing before building the TP                                                                                                
    outTime [i]= outTime [i] >> 6 ;
    if ( outTime[i] > 0x10 ) outTime[i] = 0x10;
    else if  ( outTime[i] <  -0x10 ) outTime[i] = -0x10; 




  }



}

