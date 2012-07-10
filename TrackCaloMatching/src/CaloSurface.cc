//
// $Id: CaloSurface.cc,v 1.1 2012/07/10 00:02:19 gianipez Exp $
// $Author: gianipez $
// $Date: 2012/07/10 00:02:19 $
//
// Original author G. Pezzullo & G. Tassielli
//

#include "TrackCaloMatching/inc/CaloSurface.hh"
#include "art/Utilities/Exception.h"
#include <cassert>

using namespace std;

namespace mu2e{

CaloSurface::CaloSurface(const HepTransformation& transf,  double hSide1,  double hSide2, Hep3Vector norm) : DetSurface(transf){
        _HalfSide1 = hSide1;
        _HalfSide2 = hSide2;
        _norm = norm;
}

//CaloSurface::CaloSurface(const HepTransformation& transf) : DetSurface(transf){}

//
// copy constructor
CaloSurface::CaloSurface( const DetSurface& p ) : DetSurface(p), _HalfSide1(0.0), _HalfSide2(0.0){}

//--------------
// Destructor --
//--------------
CaloSurface::~CaloSurface( ) {}


// Member Functions

inline double CaloSurface::GetHalfSide1() const {
        if(_HalfSide1 == 0.0){
                throw cet::exception("CONFIG")
                << "Side1 of the CaloSurface has dimension 0.0! "
                << endl;
        }
        return _HalfSide1;
}

inline double CaloSurface::GetHalfSide2() const {
        if(_HalfSide2 == 0.0){
                throw cet::exception("CONFIG")
                << "Side2 of the CaloSurface has dimension 0.0! "
                << endl;
        }
        return _HalfSide2;
}

void CaloSurface::SetHalfSide1(double side1){
        _HalfSide1 = side1;
}

void CaloSurface::SetHalfSide2(double side2){
        _HalfSide2 = side2;
}

DetSurface* CaloSurface::copyOf(double fparam) const{
        if (fparam == 0.0) {
                return (new CaloSurface(*transform(), _HalfSide1, _HalfSide2, _norm ) );
        } else {
                // assume that the family is generated by scaling the 2
                // parameters by the same factor
                return (new CaloSurface(*transform(), _HalfSide1*fparam, _HalfSide2*fparam, _norm ) );
        }
}

CaloSurface* CaloSurface::copy(){
        return   new CaloSurface(*transform(), _HalfSide1, _HalfSide2, _norm );
}

double CaloSurface::normTo(const HepPoint& x, Hep3Vector& n) const
{
        SurfacePoint uu;
        int isOnSurf = surfacePoint(x, uu);
        n = gotoGlobal( normal(uu) );

        HepPoint local = gotoLocal(x);

        double distance = 0.0;

        distance = local.y();

        return distance;
}

double CaloSurface::normalTo(const HepPoint& x, Hep3Vector& n, SurfacePoint&u) const
{
        SurfacePoint uu;
        int isOnSurf = surfacePoint(x, uu);

        u = uu;
        n = gotoGlobal( normal(uu) );

        HepPoint local = gotoLocal(x);

        double distance = 0.0;
        //on the local frame the x and z axes represent the direction of the two sides of the surface, the y axes is the direction normal to the surface, so
        //the distance from the surface of a PointVector given in the local frame is easy the y coordinate

        distance = local.y();

        return -distance;
}

double CaloSurface::firstDeriv(const HepPoint& thePoint, const Hep3Vector& theDirection) const
{
        Hep3Vector theNormal;
        normTo(thePoint, theNormal);
        return theDirection.unit() * _norm;//theNormal;
}

DetSurface::intertype CaloSurface::distTo(const HepPoint& p, const Hep3Vector& dir, double& dist,
                intermode mode) const
{
        DetSurface::intertype retval = DetSurface::nointersect;

        HepPoint x = gotoLocal(p);

        Hep3Vector v = (gotoLocal(dir)).unit();

        double sign = v.y()/fabs(v.y());

        HepPoint c(0.0, 0.0, 0.0);
        HepPoint Gc = gotoGlobal(c);

        SurfacePoint u(0.0, 0.0);

        Hep3Vector tmpN = normal(u);

        Hep3Vector n = _norm;//normal(u);

        Hep3Vector tmpX(x.x(), x.y(), x.z());

        double a =  tmpX*n;
        double sn = std::fabs( v*n );

        double dist1 =  a / sn;

        dist1 *= sign;

        dist = -dist1;


        if(std::fabs(x.y()) < 20.0) retval = DetSurface::intersect;

        return retval;

}

double CaloSurface::curvature(const SurfacePoint& uv) const
{
        return 0.0;
}


void CaloSurface::segmentMinMax(const HepPoint& pointOne, const HepPoint& pointTwo,
                double& mindist, double& maxdist) const
{
        HepPoint loc1 = gotoLocal(pointOne);
        HepPoint loc2 = gotoLocal(pointTwo);

        if(loc1.y()< 0.0 || loc2.y() < 0.0){
                mindist = 0.0;
        }else if(loc1.y()< loc2.y() ){
                mindist = loc1.y();
                maxdist = loc2.y();
        }else {
                mindist = loc2.y();
                maxdist = loc1.y();
        }


}

HepPoint CaloSurface::spacePoint(const SurfacePoint& uv) const
{
        double x = uv[0];
        double z = uv[1];
        double y= 0.0;

        return gotoGlobal(HepPoint(x, y, z));
}


Hep3Vector CaloSurface::normal(const SurfacePoint& uv) const
{
        Hep3Vector localNorm = gotoGlobal( _norm );
        return localNorm;
}

Hep3Vector CaloSurface::surfaceDirection(const SurfacePoint& uv, int idir) const
{

        double x = uv[0];
        double z = uv[1];
        Hep3Vector tangent(x, 0., z);

        return tangent.unit();
}

int CaloSurface::surfacePoint(const HepPoint& thePoint, SurfacePoint& uv, double tol ) const
{
        HepPoint theLocalPoint(gotoLocal(thePoint));
        uv[0] = theLocalPoint.x();
        uv[1] = theLocalPoint.z();

        int res = 1;

        if(fabs(uv[0]) <= _HalfSide1 && fabs(uv[1]) <= _HalfSide2 && fabs( theLocalPoint.y() ) < tol ){
                res = 0;
        }

        return res;
}


bool CaloSurface::wrappedCoordinate(int theCoordNumber) const
{
        //I didn't implemented it so far because I didn't use it for doing the extrapolation

        //  the wrapped coordinate is case 0, case 1 is the coordinate
        //  along the z axis, to be consistent with DetCylinder
        switch (theCoordNumber) {
        case 0:
                return true;
        case 1:
        default:
                return false;
        }
}

bool CaloSurface::operator==(const CaloSurface& otherSurface)const{
        if(otherSurface.GetHalfSide1() == _HalfSide1 && otherSurface.GetHalfSide2() == _HalfSide2 ){
                return true;
        }else {
                return false;
        }
}
void CaloSurface::print(std::ostream& os) const{
        os << "CaloSurface: HalfSide1 = "<<_HalfSide1 << " [mm]"<< std::endl;
        os << "HalfSide2 = "<< _HalfSide2 << " [mm]"<<std::endl;

}



}



