/*
 *  MoonOrbit.cpp
 *  SolarSystem
 *
 *  Created by Eric Saari on 11/25/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "MoonOrbit.h"
#include "geometry/Point3D.h"
#include "parser/Hash.h"
#include "constants.h"
#include "utility/OptionsManager.h"
#include "drawable/Drawable.h"
#include "drawable/DrawableManager.h"

MoonOrbit::MoonOrbit(Hash* h) :
   longAtEpoch(h->getDouble("LongAtEpoc")),
   longAtPerigree(h->getDouble("LongAtPerigree")),
   ascendingNode(h->getDouble("AscendingNode")),
   inclination(h->getDouble("Inclination")),
   eccentricity(h->getDouble("Eccentricity")),
   semiMajor(h->getDouble("SemiMajor")),
   angularSize(h->getDouble("AngularSize")),
   parralax(h->getDouble("Parralax"))
{
}

MoonOrbit::~MoonOrbit() {
}

Point3D MoonOrbit::calculateLocation(double days) {
   // Calculate the mean anomaly based on moment of perigree (N)
   double N = 360.0 / TROPICAL_YEAR * days;

   // Calculate the mean anomaly based on epoch (M)
   double M = N + longAtEpoch + longAtPerigree;
   adjustBearing(M);
   
   // Calculate true anomaly (v)
   double v = 360.0 / M_PI * eccentricity * sin(M * DEG_TO_RAD);
   
   // Calculate geocentric ecliptic longitude (lambda)
   double lambda = N + v + longAtEpoch;
   adjustBearing(lambda);
   
   // Calculate mean longitude (l)
   double l = 13.1763966 * days + longAtEpoch;
   adjustBearing(l);
   
   // Calculate corrected mean anomaly (Mm)
   double Mm = l - 0.111404 * days - longAtPerigree;
   adjustBearing(Mm);

   // Calculate ascending node's mean longitude (N)
   N = ascendingNode - 0.0529539 * days;
   adjustBearing(N);
   
   // Calculate correction for evection (Ev)
   double C = l - lambda;
   double Ev = 1.2739 * sin((2.0 * C - Mm) * DEG_TO_RAD);
   
   // Calculate annual equation (Ae)
   double Ae = 0.1858 * sin(M * DEG_TO_RAD);
   
   // Calculate third correction (A3)
   double A3 = 0.37 * sin(M * DEG_TO_RAD);
   
   // Correct the mean anomaly (Mm)
   Mm = Mm + Ev - Ae - A3;
   adjustBearing(Mm);

   // Calculate correction for equation of the center (Ec)
   double Ec = 6.2886 * sin(Mm);
   
   // Calculate fourth correction (A4)
   double A4 = 0.214 * sin(2.0 * Mm * DEG_TO_RAD);
   
   // Correct the latitude (l)
   l = l + Ev + Ec - Ae + A4;
   
   // Calculate the variation (V)
   double V = 0.6583 * sin(2.0 * (l - lambda) * DEG_TO_RAD);
   
   // Calculate the true longitude (l)
   l = l + V;
   
   // Correct mean longitude of ascending Node (N)
   N = N - 0.16 * sin(M * DEG_TO_RAD);
   
   // Calculate eliptic longitude (longitude)
   double y = sin((l - N) * DEG_TO_RAD) * cos(inclination * DEG_TO_RAD);
   double x = cos((l - N) * DEG_TO_RAD);
   double longitude = atan2(y, x) + N;
   
   // Calculate ecliptic latitude (latitude)
   double latitude = asin(sin((l - N) * DEG_TO_RAD) - sin(inclination * DEG_TO_RAD));
   
   // Calculate distance from planet (p)
   double p = semiMajor * (1.0 - pow(eccentricity, 2.0)) / (1.0 + eccentricity * cos((Mm + Ec) * DEG_TO_RAD));
   p = p * OptionsManager::instance().getOptions()->getDouble("scale");
   
   // Caluclate final position
   latitude = latitude * DEG_TO_RAD;
   longitude = longitude * DEG_TO_RAD;
   double cosLat = cos(latitude);

   return Point3D(cosLat * cos(longitude) * p, sin(latitude) * p, -cosLat * sin(longitude) * p);
}
