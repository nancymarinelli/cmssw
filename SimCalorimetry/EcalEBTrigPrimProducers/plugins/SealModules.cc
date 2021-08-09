#include "FWCore/Framework/interface/MakerMacros.h"

#include "EcalEBTrigPrimProducer.h"
#include "EcalEBTrigPrimAnalyzer.h"
#include "EcalPhase2TrigPrimESProducer.h"

DEFINE_FWK_MODULE(EcalEBTrigPrimProducer);
DEFINE_FWK_MODULE(EcalEBTrigPrimAnalyzer);
DEFINE_FWK_EVENTSETUP_MODULE(EcalPhase2TrigPrimESProducer);
