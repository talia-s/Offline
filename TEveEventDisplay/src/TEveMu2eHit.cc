#include "TEveEventDisplay/src/TEveMu2e_base_classes/TEveMu2eHit.h"
#include "TEveEventDisplay/src/dict_classes/GeomUtils.h"
using namespace mu2e;
namespace mu2e{

  TEveMu2eHit::TEveMu2eHit(){}

  void TEveMu2eHit::DrawHit3D(const std::string &pstr, Int_t n, CLHEP::Hep3Vector pointInMu2e, int energylevel, TEveElementList *HitList)
  {
    this->SetTitle((DataTitle(pstr, n)).c_str());
    hep3vectorTocm(pointInMu2e);
    this->SetNextPoint(pointInMu2e.x(), pointInMu2e.y(), pointInMu2e.z()); 
    int colors[] = {-7, 3, -6, -1, 9, 0, -4, 10, 1};
    this->SetMarkerColor(kSpring + colors[energylevel]);
    this->SetMarkerSize(3);
    this->SetPickable(kTRUE);
    if(AddErrorBar){ 
      TEveLine *error = new TEveLine();
      auto const& p = fComboHit.pos();
      auto const& w = fComboHit.wdir();
      auto const& s = fComboHit.wireRes();
      double x1 = (p.x()+s*w.x());
      double x2 = (p.x()-s*w.x());
      double z1 = (p.z()+s*w.z());
      double z2 = (p.z()-s*w.z());
      double y1 = (p.y()+s*w.y());
      double y2 = (p.y()-s*w.y());
      std::string errorbar = "ErrorBar Length: %d, %d, %d"; 
      error->SetTitle(Form(errorbar.c_str(), (x1 - x2), (y1 - y2), (z1 - z2)));
      CLHEP::Hep3Vector trackerCentrMu2e = GetTrackerCenter();
      hep3vectorTocm(trackerCentrMu2e);
      error->SetPoint(0, pointmmTocm(x1)+trackerCentrMu2e.x(),pointmmTocm(y1)+trackerCentrMu2e.y(),pointmmTocm(z1)+trackerCentrMu2e.z());
      error->SetNextPoint(pointmmTocm(x2)+trackerCentrMu2e.x(), pointmmTocm(y2)+trackerCentrMu2e.y(),pointmmTocm(z2)+trackerCentrMu2e.z());
      error->SetLineColor(kRed);
      error->SetPickable(kTRUE);
      HitList->AddElement(error);
    }
    HitList->AddElement(this);
  }

  void TEveMu2eHit::DrawHit2D(const std::string &pstr, Int_t n, CLHEP::Hep3Vector pointInMu2e, int energylevel, TEveElementList *HitList)
  {
    this->SetTitle((DataTitle(pstr, n)).c_str());
    hep3vectorTocm(pointInMu2e);
    this->SetNextPoint(pointInMu2e.x(), pointInMu2e.y(), pointInMu2e.z()); 
    int colors[] = {-7, 3, -6, -1, 9, 0, -4, 10, 1};
    this->SetMarkerColor(kSpring + colors[energylevel]);
    this->SetMarkerSize(mSize);
    this->SetPickable(kTRUE);

    if(AddErrorBar){ 
      TEveLine *error = new TEveLine();
      auto const& p = fComboHit.pos();
      auto const& w = fComboHit.wdir();
      auto const& s = fComboHit.wireRes();
      double x1 = (p.x()+s*w.x());
      double x2 = (p.x()-s*w.x());
      double z1 = (p.z()+s*w.z());
      double z2 = (p.z()-s*w.z());
      double y1 = (p.y()+s*w.y());
      double y2 = (p.y()-s*w.y());
      
      std::string errorbar = "ErrorBar Length: %d, %d, %d"; 
      error->SetTitle(Form(errorbar.c_str(), (x1 - x2), (y1 - y2), (z1 - z2)));
      error->SetPoint(0, pointmmTocm(x1),pointmmTocm(y1),pointmmTocm(z1));
      error->SetNextPoint(pointmmTocm(x2),pointmmTocm(y2),pointmmTocm(z2));
      error->SetLineColor(kRed);
      error->SetPickable(kTRUE);
      HitList->AddElement(error);
    }
    HitList->AddElement(this);
    }
  }
