#ifndef EcalPreshowerGeometry_h
#define EcalPreshowerGeometry_h

#include "Geometry/EcalCommonData/interface/EcalPreshowerNumberingScheme.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/PreshowerStrip.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/EcalPreshowerGeometryRecord.h"
#include "CondFormats/AlignmentRecord/interface/ESAlignmentRcd.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "Geometry/Records/interface/PEcalPreshowerRcd.h"
#include <vector>

class EcalPreshowerGeometry : public CaloSubdetectorGeometry
{
   public:

      typedef std::vector<PreshowerStrip> CellVec ;

      typedef CaloCellGeometry::CCGFloat CCGFloat ;
      typedef CaloCellGeometry::Pt3D     Pt3D     ;
      typedef CaloCellGeometry::Pt3DVec  Pt3DVec  ;

      typedef IdealGeometryRecord         IdealRecord   ;
      typedef EcalPreshowerGeometryRecord AlignedRecord ;
      typedef ESAlignmentRcd              AlignmentRecord ;
      typedef PEcalPreshowerRcd           PGeometryRecord ;

      typedef EcalPreshowerNumberingScheme NumberingScheme ;
      typedef CaloSubdetectorGeometry::ParVec ParVec ;
      typedef CaloSubdetectorGeometry::ParVecVec ParVecVec ;
      typedef ESDetId DetIdType ;

      enum { k_NumberOfCellsForCorners = ESDetId::kSizeForDenseIndexing } ;

      enum { k_NumberOfShapes = 4 } ;

      enum { k_NumberOfParametersPerShape = 4 } ;

      static std::string dbString() { return "PEcalPreshowerRcd" ; }

      virtual unsigned int numberOfShapes() const { return k_NumberOfShapes ; }
      virtual unsigned int numberOfParametersPerShape() const { return k_NumberOfParametersPerShape ; }

      EcalPreshowerGeometry() ;
  
      /// The EcalPreshowerGeometry will delete all its cell geometries at destruction time
      virtual ~EcalPreshowerGeometry();

      void setzPlanes( CCGFloat z1minus, 
		       CCGFloat z2minus,
		       CCGFloat z1plus, 
		       CCGFloat z2plus ) ;

      // Get closest cell
      virtual DetId getClosestCell( const GlobalPoint& r ) const ;


      // Get closest cell in arbitrary plane (1 or 2)
      virtual DetId getClosestCellInPlane( const GlobalPoint& r     ,
					   int                plane   ) const ;


      virtual void initializeParms() ;
      virtual unsigned int numberOfTransformParms() const { return 3 ; }

      static std::string hitString() { return "EcalHitsES" ; }

      static std::string producerTag() { return "EcalPreshower" ; }

      static unsigned int numberOfAlignments() { return 8 ; }

      static unsigned int alignmentTransformIndexLocal( const DetId& id ) ;

      static unsigned int alignmentTransformIndexGlobal( const DetId& id ) ;

      static DetId detIdFromLocalAlignmentIndex( unsigned int iLoc ) ;

      static void localCorners( Pt3DVec&        lc  ,
				const CCGFloat* pv  ,
				unsigned int    i   ,
				Pt3D&           ref   ) ;

      virtual void newCell( const GlobalPoint& f1 ,
			    const GlobalPoint& f2 ,
			    const GlobalPoint& f3 ,
			    const CCGFloat*    parm ,
			    const DetId&       detId   ) ;
   protected:

      virtual const CaloCellGeometry* cellGeomPtr( uint32_t index ) const ;

   private:

      const CCGFloat m_xWidWaf      ;
      const CCGFloat m_xInterLadGap ;
      const CCGFloat m_xIntraLadGap ;

      const CCGFloat m_yWidAct      ;
      const CCGFloat m_yCtrOff      ;

      CCGFloat m_zplane[4];

      CellVec m_cellVec ;
};


#endif

