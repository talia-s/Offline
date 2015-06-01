//
// class to define a set of allowed panel states and allow iterating through them
// This also interacts with the hits themselves
// Dave Brown

#include "KalmanTests/inc/PanelStateIterator.hh"
#include <algorithm>

namespace mu2e {
  namespace PanelAmbig {
    
    PanelStateIterator::PanelStateIterator(TSHV const& tshv,HSV const& allowedHS) : _allowedHS(allowedHS) {
      // Define the 1st valid sate.  if no hit states are allowed, all hits are fixed (ignored)
      HSV hsv;
      for(auto tsh : tshv) {
	if(_allowedHS.size() > 0){
	  // hits whose current state isn't allowed will be held frozen (ignored)
	  HitState tshs(tsh);
	  HSV::const_iterator ish = std::find(_allowedHS.begin(),_allowedHS.end(),tshs);
	  if(ish == _allowedHS.end())
	    hsv.push_back(HitState(HitState::ignore));
	  else
	    hsv.push_back(_allowedHS[0]);
	} else
	  hsv.push_back(HitState(HitState::ignore));
      }
      _allowedPS.push_back(hsv);
      // loop over all possible states and record them
      while(increment(hsv)) {
	_allowedPS.push_back(PanelState(hsv));
      }
      // set current to the 1st allowed state
      reset();
    }

    bool PanelStateIterator::increment(HSV& hsv) {
      bool retval(false); // default is failure
      HSV::iterator ihs = hsv.begin();
      do {
	if(increment(*ihs)){
	  retval = true;
	  break;
	} else {
	  // we're at the end of allowed states for this hit; reset it and try incrementing the next
	  reset(*ihs);
	  ++ihs;
	}
      } while(ihs != hsv.end());
      return retval;
    }

    bool PanelStateIterator::increment(HitState& hs) {
      bool retval(false);
      static HitState ignore(HitState::ignore);
      if(hs != ignore) {
	// find iterator to this state in the allowed states
	HSV::const_iterator ihs = std::find(_allowedHS.begin(), _allowedHS.end(),hs);
	// try incrementing: if successful, update the hit state, and we're done
	if(ihs != _allowedHS.end()) ++ihs;
	if(ihs != _allowedHS.end()){
	  // success: update state
	  hs = *ihs;
	  retval = true;
	}
      }
      return retval;
    }

    void PanelStateIterator::reset(HitState& hs) {
      static HitState ignore(HitState::ignore);
      if(hs != ignore && _allowedHS.size() > 0) {
	hs = _allowedHS[0];
      }
    }

  } // PanelAmbig namespace
} // mu2e namespace
