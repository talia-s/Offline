//
// Build a dictionary.
//
// $Id: classes.h,v 1.13 2012/07/10 00:02:20 gianipez Exp $
// $Author: gianipez $
// $Date: 2012/07/10 00:02:20 $
//
// Original author Rob Kutschke
//

#include <vector>

#include "art/Persistency/Common/Wrapper.h"
#include "art/Persistency/Common/Assns.h"

#include "RecoDataProducts/inc/StrawHitCollection.hh"
#include "RecoDataProducts/inc/StrawClusterCollection.hh"
#include "RecoDataProducts/inc/CaloHitCollection.hh"
#include "RecoDataProducts/inc/CaloCrystalHitCollection.hh"
#include "RecoDataProducts/inc/KalRepPayloadCollection.hh"
#include "RecoDataProducts/inc/KalRepExtensionPayloadCollection.hh"
#include "RecoDataProducts/inc/ExtMonUCITofHitCollection.hh"
#include "RecoDataProducts/inc/HoughCircleCollection.hh"
#include "RecoDataProducts/inc/SubEventCollection.hh"
#include "RecoDataProducts/inc/TrackerHitTimeClusterCollection.hh"
#include "RecoDataProducts/inc/SctrSttnClusterGroupCollection.hh"
#include "RecoDataProducts/inc/ZRotStrawHitMapCollection.hh"
#include "RecoDataProducts/inc/TrackerHitByID.hh"
#include "RecoDataProducts/inc/CaloClusterCollection.hh"
#include "RecoDataProducts/inc/TrackSeedCollection.hh"
#include "RecoDataProducts/inc/TrackClusterLink.hh"

// Cannot use the typedefs in here - not sure why.
template class art::Ptr<mu2e::CaloHit>;
template class std::vector<art::Ptr<mu2e::CaloHit> >;
template class art::Ptr<mu2e::ExtMonUCITofHit>;
template class std::vector<art::Ptr<mu2e::ExtMonUCITofHit> >;
template class art::Ptr<mu2e::StrawHit>;
template class std::vector<art::Ptr<mu2e::StrawHit> >;
template class art::Ptr<mu2e::TrackerHitTimeCluster>;
template class std::vector<art::Ptr<mu2e::TrackerHitTimeCluster> >;
template class std::multimap<unsigned long int, art::Ptr<mu2e::StrawHit> >;
template class art::Ptr<mu2e::CaloCrystalHit>;
template class std::vector<art::Ptr<mu2e::CaloCrystalHit> >;
template class art::Ptr<mu2e::CaloCluster>;
template class std::vector<art::Ptr<mu2e::CaloCluster> >;
template class art::Ptr<mu2e::TrackSeed>;
template class std::vector<art::Ptr<mu2e::TrackSeed> >;

template class art::Wrapper<mu2e::StrawHitCollection>;
template class art::Wrapper<mu2e::StrawClusterCollection>;
template class art::Wrapper<mu2e::CaloHitCollection>;
template class art::Wrapper<mu2e::CaloCrystalHitCollection>;
template class art::Wrapper<mu2e::CaloClusterCollection>;
template class art::Wrapper<mu2e::KalRepPayloadCollection>;
template class art::Wrapper<mu2e::KalRepExtensionPayloadCollection>;


template class art::Wrapper<mu2e::ExtMonUCITofHitCollection>;
template class art::Wrapper<mu2e::HoughCircleCollection>;
template class art::Wrapper<mu2e::SubEventCollection>;
template class art::Wrapper<mu2e::TrackerHitTimeClusterCollection>;
template class art::Wrapper<mu2e::SctrSttnClusterGroupCollection>;
template class art::Wrapper<mu2e::ZRotStrawHitMapCollection>;
template class art::Wrapper<mu2e::TrackerHitByID>;
template class art::Wrapper<mu2e::TrackSeedCollection>;


// A way to instantiate a typedef without keeping sync with its definition
namespace {
  struct Instantiations {
    mu2e::TrackClusterLink tcl;
  };
}
template class art::Wrapper<mu2e::TrackClusterLink>;
