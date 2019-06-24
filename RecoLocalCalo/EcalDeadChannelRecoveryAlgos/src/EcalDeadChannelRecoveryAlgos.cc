//
//  Original Author:   Stilianos Kesisoglou - Institute of Nuclear and Particle
// Physics NCSR Demokritos (Stilianos.Kesisoglou@cern.ch)
//          Created:   Wed Nov 21 11:24:39 EET 2012
//
//      Nov 21 2012:   First version of the code. Based on the old
// "EcalDeadChannelRecoveryAlgos.cc" code
//      Feb 14 2013:   Implementation of the criterion to select the "correct"
// max. cont. crystal.
//
//modified by S.Taroni, N. Marinelli 11 June 2019

#include "RecoLocalCalo/EcalDeadChannelRecoveryAlgos/interface/EcalDeadChannelRecoveryAlgos.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

template <typename T>
void EcalDeadChannelRecoveryAlgos<T>::setParameters(const edm::ParameterSet&ps) {
  bdtg.setParameters(ps);
}

  

template <typename T>
void EcalDeadChannelRecoveryAlgos<T>::setCaloTopology(
    const CaloTopology *topo) {
  nn.setCaloTopology(topo);
  bdtg.setCaloTopology(topo);
}


template <typename T>
EcalRecHit EcalDeadChannelRecoveryAlgos<T>::correct(
    const T id, const EcalRecHitCollection &hit_collection, std::string algo,
    double single8Cut, double sum8Cut,  bool *acceptFlag) {
  // recover as single dead channel
  double newEnergy = 0.0;
  if (algo == "NeuralNetworks") {
     newEnergy = this->nn.recover(id, hit_collection, sum8Cut, acceptFlag);
  }else if (algo=="BDTG"){
    *acceptFlag=false;
      newEnergy = this->bdtg.recover(id, hit_collection, single8Cut, sum8Cut, acceptFlag); //ADD here
     if (newEnergy>0.) *acceptFlag=true; //bdtg set to 0 if there is more than one channel in the matrix that is not reponding
  }else {
    edm::LogError("EcalDeadChannelRecoveryAlgos")
        << "Invalid algorithm for dead channel recovery.";
    *acceptFlag = false;
  }

  uint32_t flag = 0;
  return EcalRecHit(id, newEnergy, 0, flag);
}

template class EcalDeadChannelRecoveryAlgos<EBDetId>;
template class EcalDeadChannelRecoveryAlgos<EEDetId>;
