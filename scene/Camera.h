#ifndef _CAMERA_H
#define _CAMERA_H

#include "geometry/Vector3D.h"
#include "geometry/Point3D.h"
#include "geometry/Matrix.h"

class Camera {

public:
   Camera() { }
   void set(const Point3D &eye, const Point3D &look, const Vector3D &up);
   void loadGLMatrix();
   void slide(double delU, double delV, double delN);
   void roll(double angle);
   void pitch(double angle);
   void yaw(double angle);
   void undoTranslate();
   Vector3D getLocation() const { return eVec; }

   Matrix4x4 m;

private:
   Vector3D u, v, n, eVec;

   void setModelViewMatrix();
};

#endif
