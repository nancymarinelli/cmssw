#ifndef EcalPhase2TPGTimeWeightIdMap_h
#define EcalPhase2TPGTimeWeightIdMap_h

#include "CondFormats/Serialization/interface/Serializable.h"

#include <map>
#include "CondFormats/EcalObjects/interface/EcalPhase2TPGTimeWeights.h"
#include <cstdint>

class EcalPhase2TPGTimeWeightIdMap {
public:
  typedef std::map<uint32_t, EcalPhase2TPGTimeWeights> EcalPhase2TPGTimeWeightMap;
  typedef std::map<uint32_t, EcalPhase2TPGTimeWeights>::const_iterator EcalPhase2TPGTimeWeightMapItr;

  EcalPhase2TPGTimeWeightIdMap();
  ~EcalPhase2TPGTimeWeightIdMap();

  const EcalPhase2TPGTimeWeightMap& getMap() const { return map_; }
  void setValue(const uint32_t& id, const EcalPhase2TPGTimeWeights& value);

private:
  EcalPhase2TPGTimeWeightMap map_;

  COND_SERIALIZABLE;
};

#endif
