// orbit.cpp
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include "PlanetOrbit.h"
#include "geometry/Point3D.h"
#include "utility/OptionsManager.h"
#include "parser/Hash.h"
#include "constants.h"
#include <stdio.h>

const double epsilon = 0.000001;

PlanetOrbit::PlanetOrbit(double _period, double _longAtEpoch, double _longAtPerihelion, double _eccentricity, double _semiMajor, double _inclination, double _ascendingNode) :
   period(_period),
   longAtEpoch(_longAtEpoch),
   longAtPerihelion(_longAtPerihelion),
   eccentricity(_eccentricity),
   semiMajor(_semiMajor),
   inclination(_inclination),
   ascendingNode(_ascendingNode)
{
}

PlanetOrbit::~PlanetOrbit() {
}

/**
 * Calculations taken from Practical Astronomy Wih Your Calculator (3rd Edition)
 * by Peter Duffett-Smith. Copyright (C) Cambridge University Press 1979.
 * Section 54 - Calculating the Coordinates of a Planet.
 */
Point3D PlanetOrbit::calculateLocation(double days) {
   // Calculate the mean anomaly
   double M = (360.0 / TROPICAL_YEAR) * (days / period) + longAtEpoch - longAtPerihelion;
   
   // Find solution for Kepler's Law
   double E = kepler(M);
   
   // Find tan(v/2)
   double tanV = pow( ((1.0 + eccentricity) / (1.0 - eccentricity)), 0.5) * tan(E / 2.0);
   
   // Take inverse tangent and multiply by 2 to get v. Convert from radians to degrees.
   double v = atan(tanV) * 2.0 * RAD_TO_DEG;
   
   // Calculate heleocentric longitude
   double l = v + longAtPerihelion;
   adjustBearing(l);
   
   // Calculate length of radius vector
   double r = (semiMajor * (1.0 - pow(eccentricity, 2.0))) / (1.0 + eccentricity * cos(v * DEG_TO_RAD));
   
   // Calculate the heleocentric latitude
   double latitude = asin(sin((l - ascendingNode) * DEG_TO_RAD) * sin(inclination * DEG_TO_RAD));
   latitude = latitude * RAD_TO_DEG;
   
   // Calculate projected heleocentric longitude
   double x = cos((l - ascendingNode) * DEG_TO_RAD);
   double y = sin((l - ascendingNode) * DEG_TO_RAD) * cos(inclination * DEG_TO_RAD);
   double longitude = atan2f(y, x);
   longitude = longitude * RAD_TO_DEG + ascendingNode;
   adjustBearing(longitude);

   // Calculate length of projected radius vector
   double radius = r * cos(latitude * DEG_TO_RAD);
   radius = radius * AU_TO_KM * OptionsManager::instance().getOptions()->getDouble("scale");
   
   latitude = latitude * DEG_TO_RAD;
   longitude = longitude * DEG_TO_RAD;
   double cosLat = cos(latitude);
   return Point3D(cosLat * cos(longitude) * radius, sin(latitude) * radius, -cosLat * sin(longitude) * radius);
}

double PlanetOrbit::kepler(double M) {
   adjustBearing(M);
   M = M * DEG_TO_RAD;
   double E = M;
   double g = E - (eccentricity * sin(E)) - M;
   
   while(g > epsilon) {
      double deltaE = g / (1.0 - eccentricity * cos(E));
      E = E - deltaE;
      g = E - (eccentricity * sin(E)) - M;
   }
   return E;
}
