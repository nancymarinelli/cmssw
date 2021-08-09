#include "CondFormats/EcalObjects/interface/EcalPhase2TPGTimeWeightIdMap.h"

EcalPhase2TPGTimeWeightIdMap::EcalPhase2TPGTimeWeightIdMap() {}

EcalPhase2TPGTimeWeightIdMap::~EcalPhase2TPGTimeWeightIdMap() {}

void EcalPhase2TPGTimeWeightIdMap::setValue(const uint32_t& id, const EcalPhase2TPGTimeWeights& value) { map_[id] = value; }
