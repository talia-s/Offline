//Author: S Middleton 2019
#ifndef CosmicTrackFinderData_HH
#define CosmicTrackFinderData_HH
#include "BTrk/TrkBase/TrkErrCode.hh"
#include "BTrk/TrkBase/TrkParticle.hh"
#include "RecoDataProducts/inc/TrkFitDirection.hh"
#include "RecoDataProducts/inc/StrawHitPositionCollection.hh"
#include "RecoDataProducts/inc/StrawHitFlagCollection.hh"
#include "RecoDataProducts/inc/StrawHitCollection.hh"
#include "RecoDataProducts/inc/StrawHitIndex.hh"
#include "RecoDataProducts/inc/ComboHit.hh"
#include "RecoDataProducts/inc/StrawHit.hh"
#include "MCDataProducts/inc/StrawDigiMC.hh"
#include "RecoDataProducts/inc/CosmicTrackSeed.hh" //CHANGE SEED
#include "Mu2eUtilities/inc/BuildMatrixSums.hh"
#include "TrkReco/inc/TrkFaceData.hh"

#include "Math/VectorUtil.h"
#include "Math/Vector2D.h"
//c++
#include <array>

using namespace ROOT::Math::VectorUtil;

namespace mu2e {
  typedef ROOT::Math::XYVectorF  XYVec;
  // struct for weighted positions
  class XYWVec : public XYVec {
  public :
    XYWVec(XYZVec pos, int face, float weight=1.0) : XYVec(pos.x(),pos.y()), _face(face), _weight(weight){}
    float weight() const { return _weight; }
    int   face() const { return _face; }

  private :
    int   _face;
    float _weight; // weight for this position
  };

  class TimeCluster;
  class CosmicTrackFinderData {
  public:
    
    enum { kMaxResidIndex = 500 };

    constexpr static uint16_t        kNMaxChHits = 150;
    
    //possible use for alignment so keep for now:
    struct ChannelID {
      int Station;
      int Plane; 
      int Face; 
      int Panel; 
    };

    struct Diag_t {
   
      int       nShFit; //after fit
      int       nChFit; //after fit

      //int       nChPPanel;
      //int       nChHits; //same as above at moment as nothing removed

      float    Final_hit_residualX[kMaxResidIndex];
      float    Final_hit_residualY[kMaxResidIndex];
      float    Final_hit_pullX[kMaxResidIndex];
      float    Final_hit_pullY[kMaxResidIndex];
      float    Initial_hit_residualX[kMaxResidIndex];
      float    Initial_hit_residualY[kMaxResidIndex];
      float    Initial_hit_pullX[kMaxResidIndex];
      float    Initial_hit_pullY[kMaxResidIndex];
      
      float    Final_ErrorsX[kMaxResidIndex];
      float    Final_ErrorsY[kMaxResidIndex];
      
      float    Initial_ErrorsX[kMaxResidIndex];
      float    Initial_ErrorsY[kMaxResidIndex];
      
      float    Final_ErrorsTot[kMaxResidIndex];
      float    Initial_ErrorsTot[kMaxResidIndex];
      
      int      CosmicTrackFitCounter;
      float    Final_chi2d_track;
      float    Final_chi2dX_track;
      float    Final_chi2dY_track;
      float    Initial_chi2dX_track;
      float    Initial_chi2dY_track;
      float    Initial_chi2d_track;
      float    Change_chi2dX_track;
      float    Change_chi2dY_track;
      float    Change_chi2d_track;
      float    True_chi2d_track;
      
      unsigned      niters;    
    };
    const art::Event*                 event;
    const art::Run*		      run;
    
    const TimeCluster*                _timeCluster;     // hides vector of its time cluster straw hit indices
    art::Ptr<TimeCluster>             _timeClusterPtr;
    
    CosmicTrackSeed                   _tseed;
    int                               _nStrawHits;      
    int                               _nComboHits;    
    //int                               _nXYSh; //SH at start
    int                               _nXYCh; //CH at start
    int                               _nFiltComboHits;  //ComboHits from the TimeCluster filtering 
    int                               _nFiltStrawHits;  //StrawHits from the TimeCluster filtering 
    //const CosmicTrackSeedCollection*         _stcol;
    const ComboHitCollection*         _chcol;
    const StrawHitCollection*         _shcol;
    const TimeClusterCollection*      _tccol;
    const StrawDigiMCCollection*      _mccol;
    ::BuildMatrixSums         _S;//USED?

//-----------------------------------------------------------------------------
// diagnostics, histogramming
//-----------------------------------------------------------------------------
    Diag_t             _diag;

    std::array<FaceZ_t,StrawId::_ntotalfaces>         _oTracker;//array of faces, length of number of faces
    ComboHitCollection                                _chHitsToProcess;
    std::vector<XYWVec>                               _chHitsWPos;
    std::vector<StrawDigiMC>		              _mcDigisToProcess;
//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
    CosmicTrackFinderData();
    ~CosmicTrackFinderData();

    //const CosmicTrackSeedCollection*   stcol(){return      _stcol;}
    const ComboHitCollection*         chcol () { return _chcol ; }
    const StrawHitCollection*         shcol () {return _shcol;}
    //const StrawHitFlagCollection*     shfcol() { return _shfcol; }
    const TimeClusterCollection*         tccol () { return _tccol ; }
    int           maxIndex          () { return kMaxResidIndex; }
    void          orderID           (ChannelID* X, ChannelID* O);
    void          deleteTrack ();
    void          print(const char* Title);
    void          clearMCVariables();
    void          clearTempVariables();
    void          clearResults();
    void          clearDiagnostics();
  };

};
#endif

