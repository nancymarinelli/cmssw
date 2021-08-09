#ifndef EcalPhase2TPGWeightIdMap_h
#define EcalPhase2TPGWeightIdMap_h

#include "CondFormats/Serialization/interface/Serializable.h"

#include <map>
#include "CondFormats/EcalObjects/interface/EcalPhase2TPGWeights.h"
#include <cstdint>

class EcalPhase2TPGWeightIdMap {
public:
  typedef std::map<uint32_t, EcalPhase2TPGWeights> EcalPhase2TPGWeightMap;
  typedef std::map<uint32_t, EcalPhase2TPGWeights>::const_iterator EcalPhase2TPGWeightMapItr;

  EcalPhase2TPGWeightIdMap();
  ~EcalPhase2TPGWeightIdMap();

  const EcalPhase2TPGWeightMap& getMap() const { return map_; }
  void setValue(const uint32_t& id, const EcalPhase2TPGWeights& value);

private:
  EcalPhase2TPGWeightMap map_;

  COND_SERIALIZABLE;
};

#endif
