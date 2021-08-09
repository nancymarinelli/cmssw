#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalPhase2TPGParamBuilder.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
//#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
#include "Geometry/EcalMapping/interface/EcalMappingRcd.h"
#include "DataFormats/EcalDigi/interface/EcalConstants.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"



#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <fstream>

#include <iomanip>
#include <fstream>




EcalPhase2TPGParamBuilder::EcalPhase2TPGParamBuilder(edm::EventSetup const& setup) {
    
    //ecalPh2 phase2Constants;
    //Geometry
    out_file_ = new std::ofstream("EcalPhase2TPGParams.txt", std::ios::out);
    (*out_file_) << "COMMENT ====== barrel crystals ====== " << std::endl;
    //ESHandle<CaloGeometry> theGeometry;
    edm::ESHandle<CaloSubdetectorGeometry> theBarrelGeometry_handle;
    //setup.get<CaloGeometryRecord>()get(theGeometry);
    setup.get<EcalBarrelGeometryRecord>().get("EcalBarrel", theBarrelGeometry_handle);
    theBarrelGeometry_ = theBarrelGeometry_handle.product();
    //ebCells_ = theBarrelGeometry_->getValidDetIds(DetId::Ecal, EcalBarrel);
    
}

EcalPhase2TPGParamBuilder::~EcalPhase2TPGParamBuilder() {
    (*out_file_) << "EOF" << std::endl;
    out_file_->close();
    delete out_file_;
}

//void EcalPhase2TPGParamBuilder::process(const EcalLiteDTUPedestalsMap* ecaltpPed, std::vector<std::vector<int>> & paramb_out){
void EcalPhase2TPGParamBuilder::process(const EcalLiteDTUPedestalsMap* ecaltpPed, std::map<uint32_t,std::vector<uint>> & paramb_out){
    //Main xtal loop
    const std::vector<DetId>& ebCells_ = theBarrelGeometry_->getValidDetIds(DetId::Ecal, EcalBarrel);
    int paramb_ind = 0;
    for (std::vector<DetId>::const_iterator it = ebCells_.begin(); it != ebCells_.end(); ++it) {
        EBDetId id(*it);
        (*out_file_) << "CRYSTAL " << std::dec << id.rawId() << std::endl;
        //Get pedestaals from map
        EcalLiteDTUPedestalsMap::const_iterator itped = ecaltpPed->getMap().find(id);
        if (itped != ecaltpPed->end())
            peds_ = &(*itped);
        else
            std::cout << " could not find EcalLiteDTUPedestal entry for " << id << std::endl;
        
        double theta = theBarrelGeometry_->getGeometry(id)->getPosition().theta();
        int shift, mult;
        bool passedOrNot;
        int tmpPedByGain;
        std::vector<uint> tmpVec;
        for (int i =0; i <2; ++i){
            passedOrNot = computeLinearizerParam(theta,gains_[i],shift, mult);
            if (!passedOrNot) {
                std::cout << "unable to compute the parameters for SM=" << id.ism() << " xt=" << id.ic() << " "
                                   << id.rawId() << "\n";
                shift_ = 0;
                tmpPedByGain = 0;
                mult_ = 0;
                (*out_file_) << " 0x0" << " 0x0" << " 0x0" << std::endl;
            }
            else {
                shift_ = shift;
                mult_ = mult;
                tmpPedByGain = (int)(peds_->mean(i) + 0.5);
                (*out_file_) << std::hex << " 0x" << tmpPedByGain << " 0x" << mult_ << " 0x" << shift_
                            << std::endl;
            }
            //assuming overall coeff length of 4*6=24 bits
            //mult is 8 bits, shift is 4 bits, so peds are 12 bit
            //so in hex peds are first 3, shift is 1, mult is last 2
            int tmpCoeff = 0;
            
            tmpCoeff += 0xFFF & tmpPedByGain;
            tmpCoeff += (0xF & shift_) << 12;
            tmpCoeff += (0xFF & mult_) << 16;
            //paramb_out[paramb_ind][i] = tmpCoeff
            tmpVec.push_back(tmpCoeff);
            
        }
        //paramb_ind += 1;

        //paramb_out.push_back(tmpVec);
        paramb_out[id.rawId()] = tmpVec;
    }

}

bool EcalPhase2TPGParamBuilder::computeLinearizerParam(double theta,float gainRatio,int& shift,int& mult){
    //"Simplified" TPG (ignoring summing the digis and the G matrix weights) here would be
    //TPG(incomingDigi) = ((incomingDigi * mult_) >> (shift_ + shiftTwo_)) 
    //The shift_ here is to divide out the factor of 2 that the mult coefficient will be raised by.
    //The shiftTwo_ is the shift to get the amplitude down to 10 bits
    //The returned amplitude should be between 0 and 8191, so shiftTwo_ = 3
    //Want to determine mult_ and shift_.
    //To do this, note we want the max adc, when put into the TPG, to return the max output
    //so in the simplest case tpg(8191) = 1023
    //But due to, e.g., different theta positions, this will differ from crystal to crystal
    //So replace 8191 with ADC_sat, the ADC corresponding to Et_sat, which is the transverse energy saturation point
    //and note Et_sat = xtal_LSB * ADC_sat * gainRatio * calibCoeff * sin(theta)
    //(for now we let calibCoeff = 1)
    //Here xtal_LSB = 0.5GeV, and the gainRatio is either 1 or 0.1
    //solve for ADC_sat
    //ADC_sat = Et_sat/(xtal_LSB * gainRatio * calibCoeff * sin(theta))
    //and put this in the TPG equation above
    //(((Et_sat/(xtal_LSB * gainRatio * calibCoeff * sin(theta))) * mult_) >> (shift_ + shiftTwo_)) = 1023
    //Solve for mult_
    //mult_ = 1023*2^shift_*2^shiftTwo_*(xtal_LSB*gaintRatio*calibCoeff*sin(theta))/Et_sat
    //so let factor = 1023*2^shiftTwo_*(xtal_LSB*gaintRatio*calibCoeff*sin(theta))/Et_sat
    //and mult_ = factor*2^shift_
    //Et_sat should be around 2000GeV, I think. Not 100% how this works
    double Et_sat = 2000.;
    
    double factor = (1023*(xtal_LSB_*gainRatio*calibCoeff_*sin(theta))/Et_sat)*(1 << shiftTwo_);
    //first with shift_ = 0
    //add 0.5 (for rounding) and set to int
    //Here we are getting mult with a max bit length of 8 
    //and shift_ with a max bit length of 4
    mult = (int)(factor+0.5);
    for (shift=0;shift<15;shift++) {
        if (mult >= 128 && mult < 256)
            return mult;
        factor *= 2;
        mult = (int)(factor + 0.5);
    }
    return false;
    //std::cout << "unable to compute the parameters for SM=" << id.ism() << " xt=" << id.ic() << " "
    //                               << id.rawId() << "\n";
}
