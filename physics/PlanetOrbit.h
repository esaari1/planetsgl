// orbit.h
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#ifndef _PLANETORBIT_H_
#define _PLANETORBIT_H_

#include "Orbit.h"

class Point3D;

class PlanetOrbit : public Orbit {

public:
   PlanetOrbit(double _period, double _longAtEpoch, double _longAtPerihelion, double _eccentricity, double _semiMajor, double inclination, double _ascendingNode);
   virtual ~PlanetOrbit();
   
   virtual Point3D calculateLocation(double days);
   virtual double getPeriod() const { return period; }
   
private:
   double kepler(double M);

   double period;
   double longAtEpoch;
   double longAtPerihelion;
   double eccentricity;
   double semiMajor;
   double inclination;
   double ascendingNode;
};

#endif
