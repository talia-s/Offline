//
// class to resolve hit ambiguities by panel, assuming a reasonable track
// fit as input
//
// $Id: HitAmbigResolver.hh,v 1.4 2014/08/01 18:56:10 gandr Exp $
// $Author: gandr $ 
// $Date: 2014/08/01 18:56:10 $
//
#ifndef HitAmbigResolverHack_HH
#define HitAmbigResolverHack_HH
#include "BaBar/BaBar.hh"
#include "KalmanTests/inc/AmbigResolver.hh"
#ifndef __GCCXML__
#include "fhiclcpp/ParameterSet.h"
#endif/*__GCCXML__*/
#include <cstddef>
#include <vector>

class TrkDifTraj;
class KalRep;

namespace mu2e {

  class HitAmbigResolverHack : public AmbigResolver {
    public:
      enum trajtype {reftraj=0};
// construct from parameter set
#ifndef __GCCXML__
    explicit HitAmbigResolverHack(fhicl::ParameterSet const&, double ExtErr);
#endif/*__GCCXML__*/
      virtual ~HitAmbigResolverHack();
// resolve a track.  Depending on the configuration, this might
// update the hit state and the t0 value.
      virtual void resolveTrk(KalFitResult& kfit) const;
    private:
// penalty function depends on the drift radius
      double penaltyError(double rdrift) const;
      double _mindrift; // minimum drift to assign an ambiguity.  Below this, an ambiguity of '0' is defined
      double _zeropenalty; // special penalty for drifts below the minimum
      bool _penalty; // apply penalty or notA
// exponential + linear fit to ambiguity mis-assignment
      double _expnorm;
      double _lambda;
      double _offset;
      double _slope;
      double _exterr;
  };
}
#endif