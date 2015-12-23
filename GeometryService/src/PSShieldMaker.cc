//
// Original author Andrei Gaponenko

#include <algorithm>
#include <sstream>

#include "cetlib/exception.h"

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Units/SystemOfUnits.h"

#include "GeometryService/inc/PSShieldMaker.hh"
#include "ProductionSolenoidGeom/inc/PSShield.hh"

#include "ConfigTools/inc/SimpleConfig.hh"

namespace mu2e {

  PSShield::Groove PSShieldMaker::readGroove(int i, const SimpleConfig& c) {
      std::ostringstream prefix;
      prefix<<"PSShield.groove"<<1+i<<".";

      return PSShield::Groove(
                              c.getHep3Vector(prefix.str()+"refPoint"),
                              c.getDouble(prefix.str()+"theta")*CLHEP::degree,
                              c.getDouble(prefix.str()+"phi")*CLHEP::degree,
                              c.getDouble(prefix.str()+"r")*CLHEP::mm,
                              c.getDouble(prefix.str()+"halfLengh")*CLHEP::mm
                              );
  }

  std::unique_ptr<PSShield> PSShieldMaker::make(const SimpleConfig& c,
                                              const CLHEP::Hep3Vector& psEndRefPoint,
                                              const CLHEP::Hep3Vector& productionTargetCenter
                                              )
  {
    std::unique_ptr<PSShield> res(new PSShield());

    std::vector<double> zPlane;
    c.getVectorDouble("PSShield.zPlane", zPlane);
    if(!std::is_sorted(zPlane.begin(), zPlane.end())) {
      throw cet::exception("GEOM")<<"PSShieldMaker::make(): coordinates in the zPlane vector must be non-decreasing\n";
    }

    // Compute placement of the shield
    const CLHEP::Hep3Vector shieldOriginInMu2e(psEndRefPoint.x(),

                                               psEndRefPoint.y(),

                                               productionTargetCenter.z()
                                               + c.getDouble("PSShield.zOffsetFromProductionTarget")
                                               - zPlane[0]
                                               );

    //----------------------------------------------------------------
    // Read in the shells

    const int nShells = c.getInt("PSShield.nShells");
    res->shells_.reserve(nShells);
    for(int ishell = 1; ishell <= nShells; ++ishell) {
      std::ostringstream osinner, osouter;
      osinner << ishell;
      osouter << ishell + 1;

      std::vector<double> rIn, rOut;
      c.getVectorDouble("PSShield.r"+osinner.str(), rIn, zPlane.size());
      c.getVectorDouble("PSShield.r"+osouter.str(), rOut, zPlane.size());
      const std::string material = c.getString("PSShield.material"+osinner.str());

      res->shells_.emplace_back(zPlane, rIn, rOut, shieldOriginInMu2e, material);
    }

    //----------------------------------------------------------------
    // Read in the grooves

    const int nGrooves = c.getInt("PSShield.nGrooves");
    res->grooves_.reserve(nGrooves);
    for(int i=0; i<nGrooves; ++i) {
      res->grooves_.emplace_back(readGroove(i, c));
    }

    if(c.getInt("PSShield.verbosityLevel") > 0) {
      std::cout<<*res.get()<<std::endl;
    }


    //----------------------------------------------------------------
    // Read in the proton beam inlet information.  This is a tube
    // representing the beam pipe extended into HRS.
    // Added by David Norvil Brown, Louisville, March 2015

    std::string pipeMaterial = c.getString("PSShield.inlet.materialName");
    double pipeIR = c.getDouble("PSShield.inlet.innerR")*CLHEP::mm;
    double pipeOR = c.getDouble("PSShield.inlet.outerR")*CLHEP::mm;
    double pipeLength = c.getDouble("PSShield.inlet.length")*CLHEP::mm;
    double angle1 = c.getDouble("PSShield.inlet.angleY")*CLHEP::degree;
    double angle2 = c.getDouble("PSShield.inlet.angleX")*CLHEP::degree;

    res->beamAngleY_ = angle1;
    res->beamAngleX_ = angle2;
    res->beamInletCenter_ = c.getHep3Vector("PSShield.inlet.center");
    CLHEP::HepRotation pipeRotat(CLHEP::HepRotation::IDENTITY);
    pipeRotat.rotateY(angle1);
    pipeRotat.rotateX(angle2);

    res->beamInlet_ = Tube(pipeMaterial, shieldOriginInMu2e,
                           pipeIR, pipeOR, pipeLength/2.0, 0, CLHEP::twopi,
                           pipeRotat);

    return res;
  }

} // namespace mu2e