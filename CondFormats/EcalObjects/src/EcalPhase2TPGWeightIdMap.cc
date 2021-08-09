#include "CondFormats/EcalObjects/interface/EcalPhase2TPGWeightIdMap.h"

EcalPhase2TPGWeightIdMap::EcalPhase2TPGWeightIdMap() {}

EcalPhase2TPGWeightIdMap::~EcalPhase2TPGWeightIdMap() {}

void EcalPhase2TPGWeightIdMap::setValue(const uint32_t& id, const EcalPhase2TPGWeights& value) { map_[id] = value; }
