/*
 *  MoonOrbit.h
 *  SolarSystem
 *
 *  Created by Eric Saari on 11/25/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MOONORBIT_H_
#define _MOONORBIT_H_

#include "Orbit.h"
#include "constants.h"

class Hash;

class MoonOrbit : public Orbit {
   
public:
   MoonOrbit(Hash* h);
   virtual ~MoonOrbit();
   
   virtual Point3D calculateLocation(double days);
   virtual double getPeriod() const { return 0; }
   
private:
   double longAtEpoch;
   double longAtPerigree;
   double ascendingNode;
   double inclination;
   double eccentricity;
   double semiMajor;
   double angularSize;
   double parralax;
};

#endif
