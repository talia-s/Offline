#include "CLHEP/Vector/Rotation.h"
#include "CLHEP/Vector/ThreeVector.h"

#include "Offline/PTMGeom/inc/PTMPWC.hh"
#include "Offline/PTMGeom/inc/PTM.hh"
#include "Offline/PTMGeom/inc/PTMHead.hh"
#include "Offline/PTMGeom/inc/PTMStand.hh"
#include "Offline/GeometryService/inc/PTMMaker.hh"
#include "Offline/GeomPrimitives/inc/Box.hh"
//
// construct and return a PTM
//
// original author Helenka Casler
//

using namespace std;
namespace mu2e {
  std::unique_ptr<PTM> PTMMaker::make(SimpleConfig const& _config) {
    double xPosInMu2e = _config.getDouble("PTM.positionX");
    double yPosInMu2e = _config.getDouble("PTM.positionY");
    double zPosInMu2e = _config.getDouble("PTM.positionZ");
    CLHEP::Hep3Vector originInMu2e = CLHEP::Hep3Vector(xPosInMu2e, yPosInMu2e, zPosInMu2e);

    double yRotInMu2e = _config.getDouble("PTM.rotY");
    double xRotInMu2e = _config.getDouble("PTM.rotX");
    CLHEP::HepRotation rotationInMu2e = CLHEP::HepRotation();
    rotationInMu2e.rotateX(xRotInMu2e*CLHEP::deg);
    rotationInMu2e.rotateY(yRotInMu2e*CLHEP::deg);

    // the PWCs:

    double pwcSeparation = _config.getDouble("PTM.pwcSeparation");
    double frameHeight = _config.getDouble("PTM.frameHeight");
    double frameWidth = _config.getDouble("PTM.frameWidth");
    double frameThick = _config.getDouble("PTM.frameThick");
    double outerPlateThick = _config.getDouble("PTM.outerPlateLength");
    std::string frameMaterialName = _config.getString("PTM.frameMaterial");

    int framesInDetector = _config.getInt("PTM.framesInDetector");
    int outerPlatesInDetector = _config.getInt("PTM.outerPlatesInDetector");
    double ground1Zframes = _config.getDouble("PTM.ground1Zframes");
    double hv1Zframes = _config.getDouble("PTM.hv1Zframes");
    double hv2Zframes = _config.getDouble("PTM.hv2Zframes");
    double hv3Zframes = _config.getDouble("PTM.hv3Zframes");
    double ground2Zframes = _config.getDouble("PTM.ground2Zframes");

    double windowHeight = _config.getDouble("PTM.windowHeight");
    double windowWidth = _config.getDouble("PTM.windowWidth");
    double windowThick = _config.getDouble("PTM.windowThick");
    std::string windowMaterialName = _config.getString("PTM.windowMaterial");

    std::string gasMaterialName = _config.getString("PTM.innerGas");
    int numVertWires = _config.getInt("PTM.vertWiresPerPlane");
    int numHorizWires = _config.getInt("PTM.horizWiresPerPlane");

    // The holder:

    std::string holderMaterialName = _config.getString("PTM.holder.extrusionMaterial");
    double holderExtrusionWidth = _config.getDouble("PTM.holder.extrusionWidth");
    double holderShortExtrusionPos = _config.getDouble("PTM.holder.supportDistFromCenter");
    double holderLongExtrusionSep = _config.getDouble("PTM.holder.separatorGap");


    // mother volume contains both detectors, container contains one detector
    // each is slightly larger than all its contents
    double motherMargin = _config.getDouble("PTM.motherMargin");
    double containerMargin = _config.getDouble("PTM.containerMargin");

    // the PTM is made of two identical PWC's, placed such that their center
    // lines match up.

    // First, the "head" portion: the PWC's and their holder
    // "Near" PWC -- the more upstream of the two.
    CLHEP::Hep3Vector nearPWCPos = CLHEP::Hep3Vector(0.0, 0.0, -0.5*pwcSeparation);
    std::shared_ptr<PTMPWC> nearPWC( new PTMPWC("_1", frameHeight, frameWidth, frameThick, outerPlateThick, frameMaterialName,
                                     windowHeight, windowWidth, windowThick, windowMaterialName,
                                     gasMaterialName, numVertWires, numHorizWires, nearPWCPos, 0, containerMargin,
                                     framesInDetector, outerPlatesInDetector,
                                     ground1Zframes, hv1Zframes, hv2Zframes, hv3Zframes, ground2Zframes) );
    // "Far" PWC
    CLHEP::Hep3Vector farPWCPos = CLHEP::Hep3Vector(0.0, 0.0, 0.5*pwcSeparation);
    int farWireNumStart = numHorizWires + numVertWires;
    std::shared_ptr<PTMPWC> farPWC( new PTMPWC("_2", frameHeight, frameWidth, frameThick, outerPlateThick, frameMaterialName,
                                     windowHeight, windowWidth, windowThick, windowMaterialName,
                                     gasMaterialName, numVertWires, numHorizWires, farPWCPos, farWireNumStart, containerMargin,
                                     framesInDetector, outerPlatesInDetector,
                                     ground1Zframes, hv1Zframes, hv2Zframes, hv3Zframes, ground2Zframes) );

    // the holder consists of Al extrusions that hold the PWC's at the correct separation -- one in each corner.
    // It is supported partway down by some connecting extrusions of the same type
    double holderLength = pwcSeparation - 0.5*nearPWC->totalThick() - 0.5*farPWC->totalThick(); // separation is measured center-to-center
    std::shared_ptr<Box> holderExtrusionLong(new Box(0.5*holderExtrusionWidth, 0.5*holderExtrusionWidth, 0.5*holderLength));
    double shortExtrusionLength = 0.5*(holderLongExtrusionSep - holderExtrusionWidth);
    std::shared_ptr<Box> holderExtrusionShort(new Box(0.5*holderExtrusionWidth, 0.5*holderExtrusionWidth, shortExtrusionLength));

    std::shared_ptr<PTMHead> ptmHead(new PTMHead(originInMu2e, 
                                                 rotationInMu2e, 
                                                 nearPWC, 
                                                 farPWC, 
                                                 pwcSeparation, 
                                                 holderExtrusionLong,
                                                 holderExtrusionShort,
                                                 holderMaterialName,
                                                 holderLongExtrusionSep,
                                                 holderShortExtrusionPos,
                                                 motherMargin));
    std::shared_ptr<PTMStand> ptmStand(new PTMStand()); // TODO
    std::unique_ptr<PTM> ptmon(new PTM(originInMu2e, rotationInMu2e, ptmStand, ptmHead));
    return ptmon;
  } // PTMMaker::make()

} // namespace mu2e
