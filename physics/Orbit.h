/*
 *  Orbit.h
 *  SolarSystem
 *
 *  Created by Eric Saari on 11/25/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ORBIT_H_
#define _ORBIT_H_

class Point3D;

class Orbit {

public:
   virtual ~Orbit() {};
   virtual Point3D calculateLocation(double days) = 0;
   virtual double getPeriod() const = 0;
   
   void adjustBearing(double &b) {
      while(b < 0) {
         b += 360.0;
      }
      while(b > 360.0) {
         b -= 360.0;
      }
   }
};

#endif
