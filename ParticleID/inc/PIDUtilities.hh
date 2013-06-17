#ifndef ParticleID_Utilities_hh
#define ParticleID_Utilities_hh

//
// $Id: 
// $Author: 
// $Date: 
//
// Original author Vadim Rusu
//

// C++ includes
#include <vector>
#include <map>
// Framework includes:
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"

//ROOT includes
#include "TH1D.h"


namespace mu2e {
  class PIDUtilities{

  public:

    PIDUtilities() {}

    TH1D *th1dmorph(
		    TH1D *,TH1D *,
		    double par1,double par2,double parinterp,
		    double morphedhistnorm,
		    int idebug) const;  
    


  };
} // namespace mu2e

#endif /* ParticleID_Utilities_hh */
